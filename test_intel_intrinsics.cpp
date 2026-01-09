/**
 * @file test_intel_intrinsics.cpp
 * @brief Test de intrínsecos para Intel ICX
 *
 * Compilar con Intel ICX en Windows:
 *   source scripts/env_setup/setup_intel_env.bash
 *   icx -Qstd=c++20 -I include_new test_intel_intrinsics.cpp -o test_intel_intrinsics.exe
 */

#include "int128_base_tt.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test Intel ICX Intrinsics ===" << std::endl;

    // Test 1: Construcción básica
    std::cout << "[1] Construccion basica... ";
    uint128_t a(42);
    int128_t b(-123);
    std::cout << "OK" << std::endl;

    // Test 2: Operaciones aritméticas
    std::cout << "[2] Suma uint128... ";
    uint128_t sum = a + uint128_t(100);
    assert(sum == uint128_t(142));
    std::cout << "OK" << std::endl;

    // Test 3: Multiplicación
    std::cout << "[3] Multiplicacion... ";
    uint128_t prod = a * uint128_t(10);
    assert(prod == uint128_t(420));
    std::cout << "OK" << std::endl;

    // Test 4: División
    std::cout << "[4] Division... ";
    uint128_t quot = prod / uint128_t(10);
    assert(quot == uint128_t(42));
    std::cout << "OK" << std::endl;

    // Test 5: Operaciones bit
    std::cout << "[5] Leading zeros (clz64)... ";
    uint128_t val(0, 0x8000000000000000ULL); // MSB set in low part
    int lz = val.leading_zeros();
    assert(lz == 64); // 64 zeros in high, then MSB in low
    std::cout << "OK" << std::endl;

    // Test 6: Trailing zeros (ctz64)
    std::cout << "[6] Trailing zeros (ctz64)... ";
    uint128_t val2(0, 8); // 0b1000, 3 trailing zeros
    int tz = val2.trailing_zeros();
    assert(tz == 3);
    std::cout << "OK" << std::endl;

    // Test 7: int128_t con negativos
    std::cout << "[7] int128_t negativo to_string... ";
    int128_t neg(-42);
    std::string neg_str = neg.to_string();
    assert(neg_str == "-42");
    std::cout << "OK" << std::endl;

    // Test 8: Constexpr (verifica que compila)
    std::cout << "[8] Constexpr... ";
    constexpr uint128_t ce(123);
    constexpr uint128_t ce2 = ce + uint128_t(1);
    static_assert(ce2.low() == 124, "Constexpr must work");
    std::cout << "OK" << std::endl;

    // Test 9: Literales UDL
    std::cout << "[9] Literales UDL... ";
    using namespace int128_literals; // Namespace correcto para int128_base_t
    auto udl_val = 999_u128;
    assert(udl_val == uint128_t(999));
    std::cout << "OK" << std::endl;

    // Test 10: Parse string (usando constructor directo)
    std::cout << "[10] Parse string... ";
    uint128_t parsed("340282366920938463463374607431768211455"); // max value
    assert(parsed == uint128_t::max());
    std::cout << "OK" << std::endl;

    std::cout << "\n=== Todos los tests pasaron! ===" << std::endl;
    return 0;
}
