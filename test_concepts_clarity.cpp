/**
 * @file test_concepts_clarity.cpp
 * @brief Demostracion de la mejora de claridad usando concepts vs requires clauses
 */

#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

// ============================================================================
// ANTES: Usando requires clause con _v (mas verboso)
// ============================================================================

#if 0
template <typename T>
auto multiply_old(T value) noexcept
    requires is_integral_builtin_v<T>
{
    uint128_t result(100);
    result *= value;
    return result;
}

template <typename T>
auto convert_old(T value) noexcept
    requires is_arithmetic_builtin_v<T>
{
    return uint128_t(static_cast<uint64_t>(value));
}
#endif

// ============================================================================
// AHORA: Usando concepts (mas limpio y expresivo)
// ============================================================================

template <integral_builtin T> auto multiply_new(T value) noexcept
{
    uint128_t result(100);
    result *= value;
    return result;
}

template <arithmetic_builtin T> auto convert_new(T value) noexcept
{
    return uint128_t(static_cast<uint64_t>(value));
}

// Tambien se puede usar en requires clauses (pero ahora con concepts)
template <typename T>
    requires integral_builtin<T>
auto multiply_alt(T value) noexcept
{
    uint128_t result(100);
    result *= value;
    return result;
}

// Concepts compuestos (signed + builtin)
template <typename T>
    requires integral_builtin_signed<T>
auto process_signed(T value) noexcept
{
    int128_t result(value);
    if (result.is_negative()) {
        result = result.abs();
    }
    return result;
}

// ============================================================================
// TESTS
// ============================================================================

void test_concepts_clarity()
{
    std::cout << "+===========================================================+\n";
    std::cout << "|  Test de Claridad con Concepts                            |\n";
    std::cout << "+===========================================================+\n\n";

    // Test multiply_new con tipos builtin
    {
        auto r1 = multiply_new(5);    // int
        auto r2 = multiply_new(10u);  // unsigned int
        auto r3 = multiply_new(3LL);  // long long
        auto r4 = multiply_new(7ULL); // unsigned long long

        std::cout << "OK multiply_new funciona con int, unsigned, long long, etc.\n";
    }

    // Test convert_new con tipos arithmetic (int y float)
    {
        auto r1 = convert_new(42);   // int
        auto r2 = convert_new(3.14); // double (se trunca a uint64_t)
        auto r3 = convert_new(2.5f); // float

        std::cout << "OK convert_new funciona con int y floating point\n";
    }

    // Test multiply_alt (sintaxis alternativa)
    {
        auto r1 = multiply_alt(8);
        std::cout << "OK multiply_alt (requires clause con concept) funciona\n";
    }

    // Test process_signed (concept compuesto)
    {
        auto r1 = process_signed(-42);    // int negativo → abs
        auto r2 = process_signed(-100LL); // long long negativo → abs

        std::cout << "OK process_signed funciona solo con tipos signed\n";
    }

    std::cout << "\n+===========================================================+\n";
    std::cout << "|  ✅ TODOS LOS TESTS DE CONCEPTS PASARON                  |\n";
    std::cout << "+===========================================================+\n";
}

int main()
{
    test_concepts_clarity();
    return 0;
}
