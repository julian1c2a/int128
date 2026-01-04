/**
 * @file test_cross_signedness.cpp
 * @brief Test de operaciones entre uint128_t e int128_t, y con tipos builtin
 */

#include "include_new/int128_base.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

void test_constructor_conversion()
{
    std::cout << "=== Test: Constructor de conversión ===" << std::endl;

    // uint128_t -> int128_t
    uint128_t u1(0xFFFFFFFFFFFFFFFFull, 0x7FFFFFFFFFFFFFFFull); // Número grande positivo
    int128_t i1(u1);
    assert(i1.low() == u1.low());
    assert(i1.high() == u1.high());
    std::cout << "✓ uint128_t -> int128_t (positivo)" << std::endl;

    // int128_t -> uint128_t
    int128_t i2(-1); // -1 en complemento a 2 = 0xFFFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF
    uint128_t u2(i2);
    assert(u2.low() == 0xFFFFFFFFFFFFFFFFull);
    assert(u2.high() == 0xFFFFFFFFFFFFFFFFull);
    std::cout << "✓ int128_t(-1) -> uint128_t = MAX" << std::endl;

    // Número negativo grande
    int128_t i3(-12345);
    uint128_t u3(i3);
    assert(u3.low() == static_cast<uint64_t>(-12345));
    std::cout << "✓ int128_t(-12345) -> uint128_t" << std::endl;
}

void test_addition_builtin()
{
    std::cout << "\n=== Test: Suma con tipos builtin ===" << std::endl;

    uint128_t u1(100);
    u1 += 50; // uint128_t += int
    assert(u1.low() == 150);
    std::cout << "✓ uint128_t(100) += 50 = 150" << std::endl;

    uint128_t u2 = u1 + 25; // uint128_t + int
    assert(u2.low() == 175);
    std::cout << "✓ uint128_t(150) + 25 = 175" << std::endl;

    int128_t i1(200);
    i1 += -50; // int128_t += int (negativo)
    assert(i1.low() == 150);
    std::cout << "✓ int128_t(200) += -50 = 150" << std::endl;

    int128_t i2 = i1 + 100; // int128_t + int
    assert(i2.low() == 250);
    std::cout << "✓ int128_t(150) + 100 = 250" << std::endl;
}

void test_subtraction_builtin()
{
    std::cout << "\n=== Test: Resta con tipos builtin ===" << std::endl;

    uint128_t u1(100);
    u1 -= 30; // uint128_t -= int
    assert(u1.low() == 70);
    std::cout << "✓ uint128_t(100) -= 30 = 70" << std::endl;

    uint128_t u2 = u1 - 20; // uint128_t - int
    assert(u2.low() == 50);
    std::cout << "✓ uint128_t(70) - 20 = 50" << std::endl;

    int128_t i1(100);
    i1 -= 150; // int128_t -= int (resultado negativo)
    assert(i1.low() == static_cast<uint64_t>(-50));
    std::cout << "✓ int128_t(100) -= 150 = -50" << std::endl;

    int128_t i2 = i1 - (-25); // int128_t - int (doble negativo)
    assert(i2.low() == static_cast<uint64_t>(-25));
    std::cout << "✓ int128_t(-50) - (-25) = -25" << std::endl;
}

void test_multiplication_builtin()
{
    std::cout << "\n=== Test: Multiplicación con tipos builtin ===" << std::endl;

    uint128_t u1(100);
    u1 *= 5; // uint128_t *= int
    assert(u1.low() == 500);
    std::cout << "✓ uint128_t(100) *= 5 = 500" << std::endl;

    uint128_t u2 = u1 * 2; // uint128_t * int
    assert(u2.low() == 1000);
    std::cout << "✓ uint128_t(500) * 2 = 1000" << std::endl;

    int128_t i1(25);
    i1 *= -4; // int128_t *= int (resultado negativo)
    assert(i1.low() == static_cast<uint64_t>(-100));
    std::cout << "✓ int128_t(25) *= -4 = -100" << std::endl;

    int128_t i2 = i1 * (-2); // int128_t * int (doble negativo = positivo)
    assert(i2.low() == 200);
    std::cout << "✓ int128_t(-100) * (-2) = 200" << std::endl;
}

void test_cross_signedness_addition()
{
    std::cout << "\n=== Test: Suma entre uint128_t e int128_t ===" << std::endl;

    uint128_t u1(100);
    int128_t i1(50);

    // uint128_t + int128_t
    uint128_t result_u = u1 + i1;
    assert(result_u.low() == 150);
    std::cout << "✓ uint128_t(100) + int128_t(50) = uint128_t(150)" << std::endl;

    // int128_t + uint128_t
    int128_t result_i = i1 + u1;
    assert(result_i.low() == 150);
    std::cout << "✓ int128_t(50) + uint128_t(100) = int128_t(150)" << std::endl;

    // uint128_t += int128_t
    u1 += i1;
    assert(u1.low() == 150);
    std::cout << "✓ uint128_t(100) += int128_t(50) = uint128_t(150)" << std::endl;

    // int128_t += uint128_t
    i1 += uint128_t(100);
    assert(i1.low() == 150);
    std::cout << "✓ int128_t(50) += uint128_t(100) = int128_t(150)" << std::endl;
}

void test_cross_signedness_subtraction()
{
    std::cout << "\n=== Test: Resta entre uint128_t e int128_t ===" << std::endl;

    uint128_t u1(100);
    int128_t i1(30);

    // uint128_t - int128_t
    uint128_t result_u = u1 - i1;
    assert(result_u.low() == 70);
    std::cout << "✓ uint128_t(100) - int128_t(30) = uint128_t(70)" << std::endl;

    // int128_t - uint128_t
    int128_t result_i = int128_t(100) - uint128_t(30);
    assert(result_i.low() == 70);
    std::cout << "✓ int128_t(100) - uint128_t(30) = int128_t(70)" << std::endl;

    // uint128_t -= int128_t
    u1 -= i1;
    assert(u1.low() == 70);
    std::cout << "✓ uint128_t(100) -= int128_t(30) = uint128_t(70)" << std::endl;
}

void test_cross_signedness_multiplication()
{
    std::cout << "\n=== Test: Multiplicación entre uint128_t e int128_t ===" << std::endl;

    uint128_t u1(100);
    int128_t i1(5);

    // uint128_t * int128_t
    uint128_t result_u = u1 * i1;
    assert(result_u.low() == 500);
    std::cout << "✓ uint128_t(100) * int128_t(5) = uint128_t(500)" << std::endl;

    // int128_t * uint128_t
    int128_t result_i = i1 * u1;
    assert(result_i.low() == 500);
    std::cout << "✓ int128_t(5) * uint128_t(100) = int128_t(500)" << std::endl;

    // uint128_t *= int128_t
    u1 *= i1;
    assert(u1.low() == 500);
    std::cout << "✓ uint128_t(100) *= int128_t(5) = uint128_t(500)" << std::endl;
}

void test_negative_cross()
{
    std::cout << "\n=== Test: Operaciones con negativos ===" << std::endl;

    // int128_t negativo + uint128_t
    int128_t i1(-50);
    uint128_t u1(100);

    int128_t result = i1 + u1;
    assert(result.low() == 50);
    std::cout << "✓ int128_t(-50) + uint128_t(100) = int128_t(50)" << std::endl;

    // uint128_t + int128_t negativo (se interpreta como unsigned grande)
    uint128_t u2(100);
    int128_t i2(-10);
    uint128_t result_u = u2 + i2;
    // -10 como uint128_t es un número muy grande, pero en aritmética modular:
    // 100 + (-10 interpretado como uint128_t) debería dar 90 en los bits bajos
    assert(result_u.low() == 90);
    std::cout << "✓ uint128_t(100) + int128_t(-10) = uint128_t(90)" << std::endl;
}

int main()
{
    std::cout << "╔═══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Test de Operaciones Cross-Signedness y Builtin Types    ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════╝" << std::endl;

    try {
        test_constructor_conversion();
        test_addition_builtin();
        test_subtraction_builtin();
        test_multiplication_builtin();
        test_cross_signedness_addition();
        test_cross_signedness_subtraction();
        test_cross_signedness_multiplication();
        test_negative_cross();

        std::cout << "\n╔═══════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║  ✅ TODOS LOS TESTS PASARON EXITOSAMENTE                 ║" << std::endl;
        std::cout << "╚═══════════════════════════════════════════════════════════╝" << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ERROR: " << e.what() << std::endl;
        return 1;
    }
}
