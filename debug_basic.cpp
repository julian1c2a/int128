#include "include/uint128_t.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "🔍 DEBUGGING CASOS BÁSICOS" << std::endl;

    // Caso que está fallando: (2^128 - 1) * 2 debería dar overflow = 1
    uint128_t large(UINT64_MAX, UINT64_MAX); // 2^128 - 1

    std::cout << "\n=== CASO PROBLEMÁTICO ===" << std::endl;
    std::cout << "Número: uint128_t(0x" << std::hex << UINT64_MAX << ", 0x" << UINT64_MAX << ")"
              << std::endl;
    std::cout << "high() = 0x" << std::hex << large.high() << std::endl;
    std::cout << "low()  = 0x" << std::hex << large.low() << std::endl;
    std::cout << "Multiplicar por: 2" << std::endl;

    uint64_t result = large.fullmult_times_uint64(2);
    std::cout << "Resultado: 0x" << std::hex << result << std::endl;
    std::cout << "Esperado: 1 (porque (2^128 - 1) * 2 = 2^129 - 2)" << std::endl;
    std::cout << "¿Correcto?: " << (result == 1 ? "SÍ ✓" : "NO ✗") << std::endl;

    // Vamos a calcular manualmente:
    // (2^128 - 1) * 2 = 2 * 2^128 - 2 = 2^129 - 2
    // En binario: 2^128 - 1 = 111...111 (128 unos)
    // (111...111) * 2 = 1111...1110 (127 unos seguidos de un 0)
    // Esto es 2^129 - 2, así que el overflow (bits 128+) es 1

    std::cout << "\n=== ANÁLISIS MANUAL ===" << std::endl;
    std::cout << "2^128 - 1 = todos los bits en 1" << std::endl;
    std::cout << "(2^128 - 1) * 2 = 2^129 - 2" << std::endl;
    std::cout << "Bits 0-127: 2^128 - 2 = 0xFFFFFFFFFFFFFFFE en los bits bajos" << std::endl;
    std::cout << "Bits 128+:  1 (el bit 129)" << std::endl;

    return 0;
}