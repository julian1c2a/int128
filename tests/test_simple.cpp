#include "include/uint128_t.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

// Implementación simplificada para probar
uint64_t simple_fullmult_times_uint64(const uint128_t& number, uint64_t multiplier)
{
    // Método directo: (high * 2^64 + low) * multiplier
    // = high * multiplier * 2^64 + low * multiplier
    // Solo nos interesan los bits 128-191 del resultado

    uint64_t high = number.high();
    uint64_t low = number.low();

    // Función para multiplicar 64x64 -> 128 bits
    auto mul64x64 = [](uint64_t a, uint64_t b) -> std::pair<uint64_t, uint64_t> {
        uint64_t a_lo = a & 0xFFFFFFFFULL;
        uint64_t a_hi = a >> 32;
        uint64_t b_lo = b & 0xFFFFFFFFULL;
        uint64_t b_hi = b >> 32;

        uint64_t p0 = a_lo * b_lo;
        uint64_t p1 = a_lo * b_hi;
        uint64_t p2 = a_hi * b_lo;
        uint64_t p3 = a_hi * b_hi;

        uint64_t carry = (p0 >> 32) + (p1 & 0xFFFFFFFFULL) + (p2 & 0xFFFFFFFFULL);
        uint64_t result_low = p0 + ((p1 + p2) << 32);
        uint64_t result_high = p3 + (p1 >> 32) + (p2 >> 32) + (carry >> 32);

        return {result_low, result_high};
    };

    // low * multiplier (bits 0-127 del resultado)
    auto low_mult = mul64x64(low, multiplier);

    // high * multiplier (bits 64-191, desplazado 64 hacia la izquierda)
    auto high_mult = mul64x64(high, multiplier);

    // Los bits 128-191 vienen de:
    // 1. La parte alta de high * multiplier (bits 64-127 de high_mult van a 128-191)
    // 2. Cualquier carry de la suma en bits 64-127

    // Bits 64-127 del resultado = low_mult.second + high_mult.first
    uint64_t sum_bits_64_127 = low_mult.second + high_mult.first;
    uint64_t carry_to_128 = (sum_bits_64_127 < low_mult.second) ? 1 : 0;

    // Bits 128-191 del resultado = high_mult.second + carry_to_128
    return high_mult.second + carry_to_128;
}

int main()
{
    std::cout << "🔍 TESTING SIMPLE IMPLEMENTATION" << std::endl;

    // Caso específico: (2^128 - 1) * 2
    uint128_t large(UINT64_MAX, UINT64_MAX);
    uint64_t multiplier = 2;

    std::cout << "\n=== DATOS ===" << std::endl;
    std::cout << "Número: uint128_t(0x" << std::hex << UINT64_MAX << ", 0x" << UINT64_MAX << ")"
              << std::endl;
    std::cout << "Multiplicador: " << std::dec << multiplier << std::endl;

    uint64_t result = simple_fullmult_times_uint64(large, multiplier);

    std::cout << "\n=== RESULTADO ===" << std::endl;
    std::cout << "Resultado: 0x" << std::hex << result << " = " << std::dec << result << std::endl;
    std::cout << "Esperado: 1" << std::endl;
    std::cout << "¿Correcto?: " << (result == 1 ? "SÍ [OK]" : "NO [ERROR]") << std::endl;

    // Verificación adicional con Python
    std::cout << "\n=== VERIFICACIÓN MATEMÁTICA ===" << std::endl;
    std::cout << "(2^128 - 1) * 2 = 2^129 - 2" << std::endl;
    std::cout << "En binario: 1 seguido de (128 bits - 1) seguido de 0" << std::endl;
    std::cout << "Bits 128-191: solo el bit 128 está activo = 1" << std::endl;

    return 0;
}