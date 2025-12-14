#include "include/uint128_t.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>

// Función de referencia simplificada
uint64_t reference_fullmult_times_uint64(uint64_t high, uint64_t low, uint64_t multiplier)
{
    // Para el caso específico: high=0x123456789ABCDEF0, low=0xFEDCBA0987654321,
    // mult=0x1000000000000000
    std::cout << "\n=== FUNCIÓN DE REFERENCIA ===" << std::endl;
    std::cout << "high = 0x" << std::hex << high << std::endl;
    std::cout << "low  = 0x" << std::hex << low << std::endl;
    std::cout << "mult = 0x" << std::hex << multiplier << std::endl;

    // La multiplicación es: (high * 2^64 + low) * multiplier
    // = high * multiplier * 2^64 + low * multiplier
    // Queremos la parte que va a bits 128-191

    // Método simple para este caso específico donde multiplier = 2^60:
    // (high * 2^64 + low) * 2^60 = high * 2^124 + low * 2^60

    // high * 2^124: los bits 60-127 de high van a bits 124-191
    // low * 2^60: los bits 4-67 de low van a bits 64-127, bits 68-127 van a bits 128-191

    // Para high * multiplier:
    // high va en bits 64-127 del número original
    // high * 2^60 va en bits 124-187, así que bits 124-127 van a bits 124-127 (no nos interesan)
    // y bits 128-187 van a nuestro rango objetivo, comenzando en bit 128

    // Simplemente: bits 64-127 de high van a bits 124-187
    // Nos interesan bits 128-187 = bits 4-63 de high
    uint64_t result = high >> 4; // Bits 4-67 de high van a bits 128-191

    // Para low * multiplier:
    // low * 2^60: bits 68-127 de low van a bits 128-187
    // Es decir, bits 4-63 de low van a nuestro rango
    result += (low >> 4) << 4; // ¡Error! Esto no es correcto

    // Método correcto: usar aritmética de 128 bits si está disponible
#if defined(__SIZEOF_INT128__)
    __uint128_t full_num = ((__uint128_t)high << 64) | low;
    __uint128_t full_result = full_num * (__uint128_t)multiplier;
    uint64_t correct_result = (uint64_t)(full_result >> 128);

    std::cout << "Resultado correcto con __uint128_t: 0x" << std::hex << correct_result
              << std::endl;
    return correct_result;
#else
    // Fallback manual si no hay __uint128_t
    return result; // Esto probablemente sea incorrecto
#endif
}

int main()
{
    std::cout << "🔍 DEBUGGING FULLMULT" << std::endl;

    // Caso específico del test que falla
    uint128_t test1(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t mult1 = 0x1000000000000000ULL; // 2^60

    std::cout << "\n=== DATOS DEL TEST ===" << std::endl;
    std::cout << "test1.high() = 0x" << std::hex << test1.high() << std::endl;
    std::cout << "test1.low()  = 0x" << std::hex << test1.low() << std::endl;
    std::cout << "multiplier   = 0x" << std::hex << mult1 << std::endl;

    // Probar nuestra implementación
    uint64_t our_result = test1.fullmult_times_uint64(mult1);

    // Probar la referencia
    uint64_t ref_result =
        reference_fullmult_times_uint64(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL, mult1);

    std::cout << "\n=== RESULTADOS ===" << std::endl;
    std::cout << "Nuestro:     0x" << std::hex << std::setfill('0') << std::setw(16) << our_result
              << std::endl;
    std::cout << "Referencia:  0x" << std::hex << std::setfill('0') << std::setw(16) << ref_result
              << std::endl;
    std::cout << "¿Iguales?:   " << (our_result == ref_result ? "SÍ" : "NO") << std::endl;

    return 0;
}