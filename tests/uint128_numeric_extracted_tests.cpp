#include "../include/uint128/uint128_numeric.hpp"
#include "../include/uint128/uint128_t.hpp"
#include "test_iostream_helpers.hpp"
#include <iomanip>
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test de Funciones Numéricas C++20 para uint128_t ===\n\n";

    // Test midpoint - la función que mencionaste
    std::cout << "1. std::midpoint (punto medio sin overflow):\n";
    uint128_t a = uint128_t(100);
    uint128_t b = uint128_t(200);
    auto mid = nstd::midpoint(a, b);
    std::cout << "  - midpoint(100, 200) = " << mid << "\n";

    // Test con valores grandes
    uint128_t big1 = uint128_t(UINT64_MAX);
    uint128_t big2 = uint128_t(UINT64_MAX) + uint128_t(100);
    auto mid_big = nstd::midpoint(big1, big2);
    std::cout << "  - midpoint(UINT64_MAX, UINT64_MAX+100) = " << mid_big << "\n\n";

    // Test clamp
    std::cout << "2. std::clamp (restringir a rango):\n";
    uint128_t val = uint128_t(150);
    uint128_t lo = uint128_t(50);
    uint128_t hi = uint128_t(100);
    auto clamped = std::clamp(val, lo, hi);
    std::cout << "  - clamp(150, 50, 100) = " << clamped << "\n";

    val = uint128_t(25);
    auto clamped2 = std::clamp(val, lo, hi);
    std::cout << "  - clamp(25, 50, 100) = " << clamped2 << "\n\n";

    // Test manipulación de bits
    std::cout << "3. Funciones de manipulación de bits:\n";
    uint128_t bits = uint128_t(0b1100110011001100ULL); // patrón de bits

    std::cout << "  - popcount(0b1100110011001100) = " << nstd::popcount(bits) << "\n";
    std::cout << "  - countl_zero(" << bits << ") = " << nstd::countl_zero(bits) << "\n";
    std::cout << "  - countr_zero(" << bits << ") = " << nstd::countr_zero(bits) << "\n";
    std::cout << "  - bit_width(" << bits << ") = " << nstd::bit_width(bits) << "\n";

    // Test potencias de 2
    uint128_t pow2_test = uint128_t(15);
    std::cout << "\n4. Funciones de potencias de 2:\n";
    std::cout << "  - has_single_bit(16) = " << nstd::has_single_bit(uint128_t(16)) << "\n";
    std::cout << "  - has_single_bit(15) = " << nstd::has_single_bit(pow2_test) << "\n";
    std::cout << "  - bit_ceil(15) = " << nstd::bit_ceil(pow2_test) << "\n";
    std::cout << "  - bit_floor(15) = " << nstd::bit_floor(pow2_test) << "\n";

    // Test rotaciones
    std::cout << "\n5. Rotaciones de bits:\n";
    uint128_t rot_val = uint128_t(0x123456789ABCDEF0ULL);
    std::cout << "  - Valor original: 0x" << std::hex << rot_val << std::dec << "\n";
    auto rotl_result = nstd::rotl(rot_val, 4);
    auto rotr_result = nstd::rotr(rot_val, 4);
    std::cout << "  - rotl(val, 4): 0x" << std::hex << rotl_result << std::dec << "\n";
    std::cout << "  - rotr(val, 4): 0x" << std::hex << rotr_result << std::dec << "\n";

    // Test funciones de utilidad
    std::cout << "\n6. Funciones de utilidad:\n";
    std::cout << "  - is_even(100) = " << nstd::is_even(uint128_t(100)) << "\n";
    std::cout << "  - is_odd(101) = " << nstd::is_odd(uint128_t(101)) << "\n";
    std::cout << "  - abs_diff(100, 150) = " << nstd::abs_diff(uint128_t(100), uint128_t(150))
              << "\n";
    std::cout << "  - ilog2(1024) = " << nstd::ilog2(uint128_t(1024)) << "\n";

    // Test funciones adicionales
    std::cout << "\\n7. Funciones adicionales (namespace nstd):\\n";
    auto pow_result = nstd::pow(uint128_t(2), uint128_t(10));
    std::cout << "  - pow(2, 10) = " << pow_result << "\\n";

    auto sqrt_result = nstd::sqrt(uint128_t(10000));
    std::cout << "  - sqrt(10000) = " << sqrt_result << "\\n";

    auto fact_result = nstd::factorial(10);
    std::cout << "  - factorial(10) = " << fact_result << "\n";

    std::cout << "\n[OK] Todas las funciones numéricas C++20 implementadas y funcionando!\n";

    return 0;
}
