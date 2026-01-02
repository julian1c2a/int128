/**
 * @file test_constexpr_integration.cpp
 * @brief Test de integración de las operaciones constexpr modulares en uint128_t
 *
 * Verifica que todas las macros PUBLIC_METHODS se expandieron correctamente
 * y que las nuevas APIs están disponibles.
 */

#include "uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "===================================================\n";
    std::cout << "Test de Integración - Operaciones Constexpr Modulares\n";
    std::cout << "===================================================\n\n";

    // Test 1: Paridad (divisibility)
    std::cout << "1. Test de Paridad\n";
    {
        uint128_t even(0, 42);
        uint128_t odd(0, 43);

        assert(even.is_even() == true);
        assert(even.is_odd() == false);
        assert(odd.is_even() == false);
        assert(odd.is_odd() == true);

        std::cout << "   ✓ is_even() e is_odd() funcionan correctamente\n";
    }

    // Test 2: Divisibilidad (divisibility)
    std::cout << "\n2. Test de Divisibilidad\n";
    {
        uint128_t val(0, 120); // 120 = 2^3 * 3 * 5

        assert(val.is_multiple_of<2>() == true);
        assert(val.is_multiple_of<3>() == true);
        assert(val.is_multiple_of<4>() == true);
        assert(val.is_multiple_of<5>() == true);
        assert(val.is_multiple_of<7>() == false);

        std::cout << "   ✓ is_multiple_of<N>() funciona correctamente\n";
    }

    // Test 3: Extracción de potencias (factorization)
    std::cout << "\n3. Test de Factorización\n";
    {
        uint128_t val(0, 72); // 72 = 2^3 * 9 = 2^3 * 3^2

        auto [pow2_val, pow2_exp] = val.extract_power_of_2();
        assert(pow2_exp == 3);
        assert(pow2_val == uint128_t(0, 9));

        auto [pow3_val, pow3_exp] = pow2_val.extract_power_of<3>();
        assert(pow3_exp == 2);
        assert(pow3_val == uint128_t(0, 1));

        std::cout << "   ✓ extract_power_of_2() y extract_power_of<P>() funcionan\n";
    }

    // Test 4: Test de primalidad simple
    std::cout << "\n4. Test de Primalidad\n";
    {
        uint128_t prime(0, 17);
        uint128_t composite(0, 24);

        assert(prime.is_prime_candidate() == true);
        assert(composite.is_prime_candidate() == false);

        std::cout << "   ✓ is_prime_candidate() funciona correctamente\n";
    }

    // Test 5: Multiplicación por constante
    std::cout << "\n5. Test de Multiplicación Constante\n";
    {
        uint128_t val(0, 10);

        auto result = val.multiply_by<7>();
        assert(result == uint128_t(0, 70));

        auto result2 = val.multiply_by_power_of_2<3>(); // 10 * 2^3 = 80
        assert(result2 == uint128_t(0, 80));

        std::cout << "   ✓ multiply_by<N>() y multiply_by_power_of_2<E>() funcionan\n";
    }

    // Test 6: División por constante
    std::cout << "\n6. Test de División Constante\n";
    {
        uint128_t val(0, 100);

        auto result = val.divide_by<5>();
        assert(result == uint128_t(0, 20));

        auto result2 = val.divide_by_power_of_2<2>(); // 100 / 2^2 = 25
        assert(result2 == uint128_t(0, 25));

        std::cout << "   ✓ divide_by<D>() y divide_by_power_of_2<E>() funcionan\n";
    }

    // Test 7: Módulo (ya existía, pero verificamos)
    std::cout << "\n7. Test de Módulo\n";
    {
        uint128_t val(0, 123);

        auto result = val.mod<10>();
        assert(result == uint128_t(0, 3));

        auto result2 = val.mod_pot2<3>(); // 123 % 2^3 = 123 % 8 = 3
        assert(result2 == uint128_t(0, 3));

        std::cout << "   ✓ mod<R>() y mod_pot2<n>() funcionan\n";
    }

    std::cout << "\n===================================================\n";
    std::cout << "✅ TODOS LOS TESTS PASARON - Integración exitosa!\n";
    std::cout << "===================================================\n";

    return 0;
}
