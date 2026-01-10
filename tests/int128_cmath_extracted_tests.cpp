/**
 * @file int128_cmath_extracted_tests.cpp
 * @brief Tests completos de int128_base_cmath.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_cmath.hpp tests (90+ tests)
 * - test_cmath_template.cpp (18 tests)
 *
 * Total: 100+ tests cubriendo:
 * - gcd, lcm
 * - abs, sign
 * - pow, sqrt
 * - min, max, clamp
 * - midpoint, divmod
 * - bezout_coeffs
 */

#include "int128_base_cmath.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

static int tests_passed = 0;
static int tests_failed = 0;

void check(bool condition, const char *test_name)
{
    if (condition)
    {
        std::cout << "[OK]   " << test_name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "[FAIL] " << test_name << std::endl;
        tests_failed++;
    }
}

// =============================================================================
// SECTION: gcd (Greatest Common Divisor)
// =============================================================================

void test_gcd()
{
    // uint128_t
    check(gcd(uint128_t(48), uint128_t(18)) == uint128_t(6), "gcd(48, 18) == 6 (uint)");
    check(gcd(uint128_t(100), uint128_t(35)) == uint128_t(5), "gcd(100, 35) == 5 (uint)");
    check(gcd(uint128_t(0), uint128_t(5)) == uint128_t(5), "gcd(0, 5) == 5 (uint)");
    check(gcd(uint128_t(5), uint128_t(0)) == uint128_t(5), "gcd(5, 0) == 5 (uint)");
    check(gcd(uint128_t(0), uint128_t(0)) == uint128_t(0), "gcd(0, 0) == 0 (uint)");
    check(gcd(uint128_t(64), uint128_t(128)) == uint128_t(64), "gcd(64, 128) == 64 (uint)");
    check(gcd(uint128_t(17), uint128_t(19)) == uint128_t(1), "gcd(17, 19) == 1 coprimes");

    // int128_t basicos
    check(gcd(int128_t(12), int128_t(18)) == int128_t(6), "gcd(12, 18) == 6 (signed)");
    check(gcd(int128_t(48), int128_t(18)) == int128_t(6), "gcd(48, 18) == 6 (signed)");

    // int128_t con negativos
    check(gcd(int128_t(-12), int128_t(18)) == int128_t(6), "gcd(-12, 18) == 6");
    check(gcd(int128_t(12), int128_t(-18)) == int128_t(6), "gcd(12, -18) == 6");
    check(gcd(int128_t(-12), int128_t(-18)) == int128_t(6), "gcd(-12, -18) == 6");
    check(gcd(int128_t(-48), int128_t(18)) == int128_t(6), "gcd(-48, 18) == 6");

    // Valores grandes
    int128_t large1(1000000000LL);
    int128_t large2(500000000LL);
    check(gcd(large1, large2) == int128_t(500000000LL), "gcd(1e9, 5e8) == 5e8");

    // Mixed types con integral_builtin
    check(gcd(uint128_t(48), 18) == uint128_t(6), "gcd(uint128_t, int) works");
    check(gcd(12, uint128_t(18)) == uint128_t(6), "gcd(int, uint128_t) works");
    check(gcd(int128_t(12), 18) == int128_t(6), "gcd(int128_t, int) works");
}

// =============================================================================
// SECTION: lcm (Least Common Multiple)
// =============================================================================

void test_lcm()
{
    // uint128_t
    check(lcm(uint128_t(4), uint128_t(6)) == uint128_t(12), "lcm(4, 6) == 12 (uint)");
    check(lcm(uint128_t(12), uint128_t(18)) == uint128_t(36), "lcm(12, 18) == 36 (uint)");
    check(lcm(uint128_t(0), uint128_t(5)) == uint128_t(0), "lcm(0, 5) == 0 (uint)");
    check(lcm(uint128_t(1), uint128_t(5)) == uint128_t(5), "lcm(1, 5) == 5 (uint)");

    // int128_t basicos
    check(lcm(int128_t(4), int128_t(6)) == int128_t(12), "lcm(4, 6) == 12 (signed)");
    check(lcm(int128_t(21), int128_t(6)) == int128_t(42), "lcm(21, 6) == 42 (signed)");

    // int128_t con negativos
    check(lcm(int128_t(-4), int128_t(6)) == int128_t(12), "lcm(-4, 6) == 12");
    check(lcm(int128_t(4), int128_t(-6)) == int128_t(12), "lcm(4, -6) == 12");
    check(lcm(int128_t(-4), int128_t(-6)) == int128_t(12), "lcm(-4, -6) == 12");

    // Mixed types
    check(lcm(4, uint128_t(6)) == uint128_t(12), "lcm(int, uint128_t) works");
    check(lcm(int128_t(4), 6) == int128_t(12), "lcm(int128_t, int) works");
}

// =============================================================================
// SECTION: abs (Absolute Value)
// =============================================================================

void test_abs()
{
    // Positivos
    check(abs(int128_t(42)) == int128_t(42), "abs(42) == 42");
    check(abs(int128_t(12345)) == int128_t(12345), "abs(12345) == 12345");

    // Negativos
    check(abs(int128_t(-42)) == int128_t(42), "abs(-42) == 42");
    check(abs(int128_t(-12345)) == int128_t(12345), "abs(-12345) == 12345");

    // Cero
    check(abs(int128_t(0)) == int128_t(0), "abs(0) == 0");

    // Valores grandes
    int128_t large(-1000000000LL);
    check(abs(large) == int128_t(1000000000LL), "abs(-1e9) == 1e9");
}

// =============================================================================
// SECTION: sign
// =============================================================================

void test_sign()
{
    // Positivos
    check(sign(int128_t(1)) == int128_t(1), "sign(1) == 1");
    check(sign(int128_t(42)) == int128_t(1), "sign(42) == 1");
    check(sign(int128_t(12345)) == int128_t(1), "sign(12345) == 1");

    // Negativos
    check(sign(int128_t(-1)) == int128_t(-1), "sign(-1) == -1");
    check(sign(int128_t(-42)) == int128_t(-1), "sign(-42) == -1");
    check(sign(int128_t(-5)) == int128_t(-1), "sign(-5) == -1");

    // Cero
    check(sign(int128_t(0)) == int128_t(0), "sign(0) == 0");
}

// =============================================================================
// SECTION: pow (Power)
// =============================================================================

void test_pow()
{
    // uint128_t basicos
    check(pow(uint128_t(2), uint128_t(10)) == uint128_t(1024), "pow(2, 10) == 1024 (uint)");
    check(pow(uint128_t(3), uint128_t(4)) == uint128_t(81), "pow(3, 4) == 81 (uint)");
    check(pow(uint128_t(5), uint128_t(2)) == uint128_t(25), "pow(5, 2) == 25 (uint)");

    // int128_t basicos
    check(pow(int128_t(2), int128_t(3)) == int128_t(8), "pow(2, 3) == 8 (signed)");
    check(pow(int128_t(10), int128_t(5)) == int128_t(100000), "pow(10, 5) == 100000 (signed)");

    // Fast paths
    check(pow(int128_t(123), int128_t(0)) == int128_t(1), "pow(123, 0) == 1");
    check(pow(int128_t(123), int128_t(1)) == int128_t(123), "pow(123, 1) == 123");
    check(pow(int128_t(0), int128_t(5)) == int128_t(0), "pow(0, 5) == 0");
    check(pow(int128_t(1), int128_t(100)) == int128_t(1), "pow(1, 100) == 1");

    // Base negativa
    check(pow(int128_t(-2), int128_t(3)) == int128_t(-8), "pow(-2, 3) == -8");
    check(pow(int128_t(-2), int128_t(4)) == int128_t(16), "pow(-2, 4) == 16");
    check(pow(int128_t(-3), int128_t(2)) == int128_t(9), "pow(-3, 2) == 9");
    check(pow(int128_t(-1), int128_t(5)) == int128_t(-1), "pow(-1, 5) == -1");
    check(pow(int128_t(-1), int128_t(6)) == int128_t(1), "pow(-1, 6) == 1");

    // Mixed types
    check(pow(int128_t(2), 3) == int128_t(8), "pow(int128_t, int) works");
}

// =============================================================================
// SECTION: sqrt (Square Root)
// =============================================================================

void test_sqrt()
{
    // uint128_t
    check(sqrt(uint128_t(100)) == uint128_t(10), "sqrt(100) == 10 (uint)");
    check(sqrt(uint128_t(0)) == uint128_t(0), "sqrt(0) == 0 (uint)");
    check(sqrt(uint128_t(1)) == uint128_t(1), "sqrt(1) == 1 (uint)");

    // int128_t cuadrados perfectos
    check(sqrt(int128_t(0)) == int128_t(0), "sqrt(0) == 0");
    check(sqrt(int128_t(1)) == int128_t(1), "sqrt(1) == 1");
    check(sqrt(int128_t(4)) == int128_t(2), "sqrt(4) == 2");
    check(sqrt(int128_t(9)) == int128_t(3), "sqrt(9) == 3");
    check(sqrt(int128_t(16)) == int128_t(4), "sqrt(16) == 4");
    check(sqrt(int128_t(25)) == int128_t(5), "sqrt(25) == 5");
    check(sqrt(int128_t(100)) == int128_t(10), "sqrt(100) == 10");
    check(sqrt(int128_t(144)) == int128_t(12), "sqrt(144) == 12");
    check(sqrt(int128_t(10000)) == int128_t(100), "sqrt(10000) == 100");

    // No cuadrados perfectos (floor)
    check(sqrt(int128_t(2)) == int128_t(1), "sqrt(2) == 1 (floor)");
    check(sqrt(int128_t(3)) == int128_t(1), "sqrt(3) == 1 (floor)");
    check(sqrt(int128_t(5)) == int128_t(2), "sqrt(5) == 2 (floor)");
    check(sqrt(int128_t(8)) == int128_t(2), "sqrt(8) == 2 (floor)");
    check(sqrt(int128_t(15)) == int128_t(3), "sqrt(15) == 3 (floor)");
    check(sqrt(int128_t(99)) == int128_t(9), "sqrt(99) == 9 (floor)");

    // Propiedad: sqrt(n)^2 <= n < (sqrt(n)+1)^2
    for (int i = 1; i <= 20; ++i)
    {
        const int128_t n(i * i + i);
        const int128_t root = sqrt(n);
        const int128_t root_sq = root * root;
        const int128_t next_sq = (root + int128_t(1)) * (root + int128_t(1));
        check(root_sq <= n && n < next_sq, ("sqrt property for " + std::to_string(i * i + i)).c_str());
    }

    // Valores grandes
    check(sqrt(int128_t(1000000)) == int128_t(1000), "sqrt(1000000) == 1000");
}

// =============================================================================
// SECTION: min / max
// =============================================================================

void test_min_max()
{
    // uint128_t
    check(min(uint128_t(5), uint128_t(10)) == uint128_t(5), "min(5, 10) == 5 (uint)");
    check(max(uint128_t(5), uint128_t(10)) == uint128_t(10), "max(5, 10) == 10 (uint)");

    // int128_t basicos
    check(min(int128_t(3), int128_t(5)) == int128_t(3), "min(3, 5) == 3");
    check(min(int128_t(5), int128_t(3)) == int128_t(3), "min(5, 3) == 3");
    check(max(int128_t(3), int128_t(5)) == int128_t(5), "max(3, 5) == 5");
    check(max(int128_t(5), int128_t(3)) == int128_t(5), "max(5, 3) == 5");

    // Con negativos
    check(min(int128_t(-3), int128_t(5)) == int128_t(-3), "min(-3, 5) == -3");
    check(min(int128_t(-5), int128_t(-3)) == int128_t(-5), "min(-5, -3) == -5");
    check(max(int128_t(-3), int128_t(5)) == int128_t(5), "max(-3, 5) == 5");
    check(max(int128_t(-5), int128_t(-3)) == int128_t(-3), "max(-5, -3) == -3");

    // Iguales
    check(min(int128_t(5), int128_t(5)) == int128_t(5), "min(5, 5) == 5");
    check(max(int128_t(5), int128_t(5)) == int128_t(5), "max(5, 5) == 5");

    // Valores grandes
    int128_t large1(1000000000LL);
    int128_t large2(-1000000000LL);
    check(min(large1, large2) == large2, "min(1e9, -1e9) == -1e9");
    check(max(large1, large2) == large1, "max(1e9, -1e9) == 1e9");

    // Mixed types
    check(min(int128_t(3), 5) == int128_t(3), "min(int128_t, int) works");
    check(max(3, int128_t(5)) == int128_t(5), "max(int, int128_t) works");
}

// =============================================================================
// SECTION: clamp
// =============================================================================

void test_clamp()
{
    // uint128_t
    check(clamp(uint128_t(15), uint128_t(0), uint128_t(10)) == uint128_t(10), "clamp(15, 0, 10) == 10 (uint)");

    // Dentro del rango
    check(clamp(int128_t(5), int128_t(0), int128_t(10)) == int128_t(5), "clamp(5, 0, 10) == 5");

    // Debajo del minimo
    check(clamp(int128_t(-5), int128_t(0), int128_t(10)) == int128_t(0), "clamp(-5, 0, 10) == 0");

    // Encima del maximo
    check(clamp(int128_t(15), int128_t(0), int128_t(10)) == int128_t(10), "clamp(15, 0, 10) == 10");

    // En los limites
    check(clamp(int128_t(0), int128_t(0), int128_t(10)) == int128_t(0), "clamp(0, 0, 10) == 0");
    check(clamp(int128_t(10), int128_t(0), int128_t(10)) == int128_t(10), "clamp(10, 0, 10) == 10");

    // Rangos negativos
    check(clamp(int128_t(-5), int128_t(-10), int128_t(-1)) == int128_t(-5), "clamp(-5, -10, -1) == -5");
    check(clamp(int128_t(-15), int128_t(-10), int128_t(-1)) == int128_t(-10), "clamp(-15, -10, -1) == -10");
    check(clamp(int128_t(5), int128_t(-10), int128_t(-1)) == int128_t(-1), "clamp(5, -10, -1) == -1");
}

// =============================================================================
// SECTION: midpoint
// =============================================================================

void test_midpoint()
{
    // uint128_t
    check(midpoint(uint128_t(10), uint128_t(20)) == uint128_t(15), "midpoint(10, 20) == 15 (uint)");

    // int128_t basicos
    check(midpoint(int128_t(0), int128_t(10)) == int128_t(5), "midpoint(0, 10) == 5");
    check(midpoint(int128_t(10), int128_t(20)) == int128_t(15), "midpoint(10, 20) == 15");
    check(midpoint(int128_t(-10), int128_t(10)) == int128_t(0), "midpoint(-10, 10) == 0");

    // Negativos
    check(midpoint(int128_t(-20), int128_t(-10)) == int128_t(-15), "midpoint(-20, -10) == -15");
    check(midpoint(int128_t(-5), int128_t(-1)) == int128_t(-3), "midpoint(-5, -1) == -3");

    // Iguales
    check(midpoint(int128_t(5), int128_t(5)) == int128_t(5), "midpoint(5, 5) == 5");

    // Suma impar (redondea hacia a)
    check(midpoint(int128_t(1), int128_t(2)) == int128_t(1), "midpoint(1, 2) == 1");
    check(midpoint(int128_t(2), int128_t(1)) == int128_t(2), "midpoint(2, 1) == 2");

    // Valores grandes
    int128_t large1(1000000000LL);
    int128_t large2(2000000000LL);
    check(midpoint(large1, large2) == int128_t(1500000000LL), "midpoint(1e9, 2e9) == 1.5e9");
}

// =============================================================================
// SECTION: divmod
// =============================================================================

void test_divmod()
{
    // uint128_t
    {
        auto [q, r] = divmod(uint128_t(17), uint128_t(5));
        check(q == uint128_t(3) && r == uint128_t(2), "divmod(17, 5) == (3, 2) (uint)");
    }

    // int128_t
    {
        auto [q, r] = divmod(int128_t(17), int128_t(5));
        check(q == int128_t(3) && r == int128_t(2), "divmod(17, 5) == (3, 2) (signed)");
    }

    // Division exacta
    {
        auto [q, r] = divmod(uint128_t(100), uint128_t(10));
        check(q == uint128_t(10) && r == uint128_t(0), "divmod(100, 10) == (10, 0)");
    }
}

// =============================================================================
// SECTION: bezout_coeffs (Extended Euclidean Algorithm)
// =============================================================================

void test_bezout_coeffs()
{
    // Caso basico: 48*x + 18*y = gcd(48,18) = 6
    {
        auto [x, y] = bezout_coeffs(uint128_t(48), uint128_t(18));
        uint128_t ax = uint128_t(48) * x.magnitude;
        uint128_t by = uint128_t(18) * y.magnitude;

        uint128_t result;
        if (!x.is_negative && !y.is_negative)
        {
            result = ax + by;
        }
        else if (x.is_negative && !y.is_negative)
        {
            result = by - ax;
        }
        else if (!x.is_negative && y.is_negative)
        {
            result = ax - by;
        }
        else
        {
            result = uint128_t(0);
        }
        check(result == uint128_t(6), "bezout_coeffs(48, 18) satisfies 48x + 18y = 6");
    }

    // Con cero
    {
        auto [x, y] = bezout_coeffs(uint128_t(0), uint128_t(5));
        check(x.magnitude == uint128_t(0) && y.magnitude == uint128_t(1), "bezout_coeffs(0, 5) == (0, 1)");
    }

    // Primos: gcd(17, 13) = 1
    {
        auto [x, y] = bezout_coeffs(uint128_t(17), uint128_t(13));
        uint128_t ax = uint128_t(17) * x.magnitude;
        uint128_t by = uint128_t(13) * y.magnitude;

        uint128_t result;
        if (!x.is_negative && !y.is_negative)
        {
            result = ax + by;
        }
        else if (x.is_negative && !y.is_negative)
        {
            result = by - ax;
        }
        else if (!x.is_negative && y.is_negative)
        {
            result = ax - by;
        }
        else
        {
            result = uint128_t(0);
        }
        check(result == uint128_t(1), "bezout_coeffs(17, 13) satisfies 17x + 13y = 1");
    }
}

// =============================================================================
// SECTION: edge cases
// =============================================================================

void test_edge_cases()
{
    // Operaciones con cero
    check(abs(int128_t(0)) == int128_t(0), "abs(0) == 0");
    check(sign(int128_t(0)) == int128_t(0), "sign(0) == 0");
    check(sqrt(int128_t(0)) == int128_t(0), "sqrt(0) == 0");

    // Operaciones de identidad
    check(gcd(int128_t(42), int128_t(1)) == int128_t(1), "gcd(n, 1) == 1");
    check(lcm(int128_t(42), int128_t(1)) == int128_t(42), "lcm(n, 1) == n");
    check(pow(int128_t(42), int128_t(1)) == int128_t(42), "pow(n, 1) == n");

    // Self operations
    check(min(int128_t(42), int128_t(42)) == int128_t(42), "min(n, n) == n");
    check(max(int128_t(42), int128_t(42)) == int128_t(42), "max(n, n) == n");
    check(midpoint(int128_t(42), int128_t(42)) == int128_t(42), "midpoint(n, n) == n");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_cmath.hpp tests ===\n\n";

    std::cout << "--- gcd ---\n";
    test_gcd();

    std::cout << "\n--- lcm ---\n";
    test_lcm();

    std::cout << "\n--- abs ---\n";
    test_abs();

    std::cout << "\n--- sign ---\n";
    test_sign();

    std::cout << "\n--- pow ---\n";
    test_pow();

    std::cout << "\n--- sqrt ---\n";
    test_sqrt();

    std::cout << "\n--- min / max ---\n";
    test_min_max();

    std::cout << "\n--- clamp ---\n";
    test_clamp();

    std::cout << "\n--- midpoint ---\n";
    test_midpoint();

    std::cout << "\n--- divmod ---\n";
    test_divmod();

    std::cout << "\n--- bezout_coeffs ---\n";
    test_bezout_coeffs();

    std::cout << "\n--- edge cases ---\n";
    test_edge_cases();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
