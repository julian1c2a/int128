/**
 * @file int128_numeric_extracted_tests.cpp
 * @brief Tests completos de int128_base_numeric.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_numeric.hpp tests (40+ tests)
 * - test_numeric_template.cpp (87 tests)
 *
 * Total: 90+ tests cubriendo:
 * - midpoint, clamp
 * - abs, sign
 * - gcd, lcm
 * - power, isqrt, ilog2
 * - is_even, is_odd
 * - abs_diff
 * - factorial
 * - min, max (variadic)
 * - divmod
 */

#include "int128_base_numeric.hpp"
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
// SECTION: midpoint
// =============================================================================

void test_midpoint()
{
    // uint128_t
    check(midpoint(uint128_t(100), uint128_t(200)) == uint128_t(150), "midpoint(100, 200) == 150 (uint)");
    check(midpoint(uint128_t(0), uint128_t(100)) == uint128_t(50), "midpoint(0, 100) == 50 (uint)");

    // int128_t
    check(midpoint(int128_t(-100), int128_t(100)) == int128_t(0), "midpoint(-100, 100) == 0 (signed)");
    check(midpoint(int128_t(-200), int128_t(-100)) == int128_t(-150), "midpoint(-200, -100) == -150 (signed)");
    check(midpoint(int128_t(10), int128_t(20)) == int128_t(15), "midpoint(10, 20) == 15 (signed)");
}

// =============================================================================
// SECTION: clamp
// =============================================================================

void test_clamp()
{
    // uint128_t
    uint128_t ulo(10), uhi(100);
    check(clamp(uint128_t(50), ulo, uhi) == uint128_t(50), "clamp(50, 10, 100) == 50 (uint)");
    check(clamp(uint128_t(5), ulo, uhi) == ulo, "clamp(5, 10, 100) == 10 (uint)");
    check(clamp(uint128_t(150), ulo, uhi) == uhi, "clamp(150, 10, 100) == 100 (uint)");

    // int128_t
    int128_t ilo(-50), ihi(50);
    check(clamp(int128_t(-100), ilo, ihi) == ilo, "clamp(-100, -50, 50) == -50 (signed)");
    check(clamp(int128_t(100), ilo, ihi) == ihi, "clamp(100, -50, 50) == 50 (signed)");
    check(clamp(int128_t(0), ilo, ihi) == int128_t(0), "clamp(0, -50, 50) == 0 (signed)");
}

// =============================================================================
// SECTION: abs
// =============================================================================

void test_abs()
{
    // uint128_t (identidad)
    check(abs(uint128_t(42)) == uint128_t(42), "abs(uint128(42)) == 42");

    // int128_t
    check(abs(int128_t(42)) == int128_t(42), "abs(int128(42)) == 42");
    check(abs(int128_t(-42)) == int128_t(42), "abs(int128(-42)) == 42");
    check(abs(int128_t(0)) == int128_t(0), "abs(int128(0)) == 0");
}

// =============================================================================
// SECTION: sign
// =============================================================================

void test_sign()
{
    // uint128_t
    check(sign(uint128_t(0)) == 0, "sign(uint128(0)) == 0");
    check(sign(uint128_t(42)) == 1, "sign(uint128(42)) == 1");

    // int128_t
    check(sign(int128_t(0)) == 0, "sign(int128(0)) == 0");
    check(sign(int128_t(42)) == 1, "sign(int128(42)) == 1");
    check(sign(int128_t(-42)) == -1, "sign(int128(-42)) == -1");
}

// =============================================================================
// SECTION: gcd
// =============================================================================

void test_gcd()
{
    // uint128_t
    check(gcd(uint128_t(12), uint128_t(8)) == uint128_t(4), "gcd(12, 8) == 4 (uint)");
    check(gcd(uint128_t(100), uint128_t(25)) == uint128_t(25), "gcd(100, 25) == 25 (uint)");
    check(gcd(uint128_t(17), uint128_t(13)) == uint128_t(1), "gcd(17, 13) == 1 coprimos");
    check(gcd(uint128_t(0), uint128_t(5)) == uint128_t(5), "gcd(0, 5) == 5 (uint)");
    check(gcd(uint128_t(5), uint128_t(0)) == uint128_t(5), "gcd(5, 0) == 5 (uint)");

    // int128_t (maneja negativos)
    check(gcd(int128_t(-12), int128_t(8)) == int128_t(4), "gcd(-12, 8) == 4 (signed)");
    check(gcd(int128_t(12), int128_t(-8)) == int128_t(4), "gcd(12, -8) == 4 (signed)");
    check(gcd(int128_t(-12), int128_t(-8)) == int128_t(4), "gcd(-12, -8) == 4 (signed)");
}

// =============================================================================
// SECTION: lcm
// =============================================================================

void test_lcm()
{
    // uint128_t
    check(lcm(uint128_t(4), uint128_t(6)) == uint128_t(12), "lcm(4, 6) == 12 (uint)");
    check(lcm(uint128_t(3), uint128_t(5)) == uint128_t(15), "lcm(3, 5) == 15 (uint)");
    check(lcm(uint128_t(0), uint128_t(5)) == uint128_t(0), "lcm(0, 5) == 0 (uint)");

    // int128_t
    check(lcm(int128_t(-4), int128_t(6)) == int128_t(12), "lcm(-4, 6) == 12 (signed)");
    check(lcm(int128_t(4), int128_t(-6)) == int128_t(12), "lcm(4, -6) == 12 (signed)");
}

// =============================================================================
// SECTION: power
// =============================================================================

void test_power()
{
    check(power(uint128_t(2), 0u) == uint128_t(1), "power(2, 0) == 1");
    check(power(uint128_t(2), 1u) == uint128_t(2), "power(2, 1) == 2");
    check(power(uint128_t(2), 10u) == uint128_t(1024), "power(2, 10) == 1024");
    check(power(uint128_t(3), 4u) == uint128_t(81), "power(3, 4) == 81");

    check(power(int128_t(-2), 3u) == int128_t(-8), "power(-2, 3) == -8");
    check(power(int128_t(-2), 4u) == int128_t(16), "power(-2, 4) == 16");
}

// =============================================================================
// SECTION: isqrt
// =============================================================================

void test_isqrt()
{
    check(isqrt(uint128_t(0)) == uint128_t(0), "isqrt(0) == 0");
    check(isqrt(uint128_t(1)) == uint128_t(1), "isqrt(1) == 1");
    check(isqrt(uint128_t(4)) == uint128_t(2), "isqrt(4) == 2");
    check(isqrt(uint128_t(9)) == uint128_t(3), "isqrt(9) == 3");
    check(isqrt(uint128_t(10)) == uint128_t(3), "isqrt(10) == 3 (floor)");
    check(isqrt(uint128_t(100)) == uint128_t(10), "isqrt(100) == 10");
    check(isqrt(uint128_t(1000000)) == uint128_t(1000), "isqrt(1000000) == 1000");

    check(isqrt(int128_t(-1)) == int128_t(0), "isqrt(int128(-1)) == 0 (neg)");
    check(isqrt(int128_t(100)) == int128_t(10), "isqrt(int128(100)) == 10");
}

// =============================================================================
// SECTION: ilog2
// =============================================================================

void test_ilog2()
{
    check(ilog2(uint128_t(1)) == 0, "ilog2(1) == 0");
    check(ilog2(uint128_t(2)) == 1, "ilog2(2) == 1");
    check(ilog2(uint128_t(4)) == 2, "ilog2(4) == 2");
    check(ilog2(uint128_t(8)) == 3, "ilog2(8) == 3");
    check(ilog2(uint128_t(15)) == 3, "ilog2(15) == 3 (floor)");
    check(ilog2(uint128_t(16)) == 4, "ilog2(16) == 4");
    check(ilog2(uint128_t(0)) == -1, "ilog2(0) == -1");

    check(ilog2(int128_t(-1)) == -1, "ilog2(int128(-1)) == -1");
    check(ilog2(int128_t(16)) == 4, "ilog2(int128(16)) == 4");
}

// =============================================================================
// SECTION: is_even / is_odd
// =============================================================================

void test_parity()
{
    // is_even
    check(is_even(uint128_t(0)), "is_even(0)");
    check(is_even(uint128_t(2)), "is_even(2)");
    check(is_even(uint128_t(100)), "is_even(100)");
    check(!is_even(uint128_t(1)), "!is_even(1)");
    check(!is_even(uint128_t(99)), "!is_even(99)");

    // is_odd
    check(is_odd(uint128_t(1)), "is_odd(1)");
    check(is_odd(uint128_t(99)), "is_odd(99)");
    check(!is_odd(uint128_t(0)), "!is_odd(0)");
    check(!is_odd(uint128_t(100)), "!is_odd(100)");

    // signed
    check(is_even(int128_t(-2)), "is_even(int128(-2))");
    check(is_odd(int128_t(-1)), "is_odd(int128(-1))");
}

// =============================================================================
// SECTION: abs_diff
// =============================================================================

void test_abs_diff()
{
    check(abs_diff(uint128_t(10), uint128_t(3)) == uint128_t(7), "abs_diff(10, 3) == 7");
    check(abs_diff(uint128_t(3), uint128_t(10)) == uint128_t(7), "abs_diff(3, 10) == 7");
    check(abs_diff(uint128_t(5), uint128_t(5)) == uint128_t(0), "abs_diff(5, 5) == 0");

    check(abs_diff(int128_t(-10), int128_t(10)) == int128_t(20), "abs_diff(-10, 10) == 20");
    check(abs_diff(int128_t(10), int128_t(-10)) == int128_t(20), "abs_diff(10, -10) == 20");
}

// =============================================================================
// SECTION: factorial
// =============================================================================

void test_factorial()
{
    check(factorial<signedness::unsigned_type>(0) == uint128_t(1), "factorial(0) == 1");
    check(factorial<signedness::unsigned_type>(1) == uint128_t(1), "factorial(1) == 1");
    check(factorial<signedness::unsigned_type>(5) == uint128_t(120), "factorial(5) == 120");
    check(factorial<signedness::unsigned_type>(10) == uint128_t(3628800), "factorial(10) == 3628800");
    check(factorial<signedness::unsigned_type>(20) == uint128_t(2432902008176640000ULL), "factorial(20) correct");
}

// =============================================================================
// SECTION: min / max variadic
// =============================================================================

void test_min_max_variadic()
{
    check(min(uint128_t(5), uint128_t(10)) == uint128_t(5), "min(5, 10) == 5");
    check(max(uint128_t(5), uint128_t(10)) == uint128_t(10), "max(5, 10) == 10");

    check(min(int128_t(-5), int128_t(10)) == int128_t(-5), "min(-5, 10) == -5");
    check(max(int128_t(-5), int128_t(10)) == int128_t(10), "max(-5, 10) == 10");

    // Múltiples argumentos
    check(min(uint128_t(10), uint128_t(5), uint128_t(8)) == uint128_t(5), "min(10, 5, 8) == 5");
    check(max(uint128_t(10), uint128_t(5), uint128_t(8)) == uint128_t(10), "max(10, 5, 8) == 10");
}

// =============================================================================
// SECTION: divmod
// =============================================================================

void test_divmod()
{
    {
        auto [q, r] = divmod(uint128_t(17), uint128_t(5));
        check(q == uint128_t(3), "divmod(17, 5).quotient == 3");
        check(r == uint128_t(2), "divmod(17, 5).remainder == 2");
    }
    {
        auto [q, r] = divmod(uint128_t(100), uint128_t(10));
        check(q == uint128_t(10), "divmod(100, 10).quotient == 10");
        check(r == uint128_t(0), "divmod(100, 10).remainder == 0");
    }
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_numeric.hpp tests ===\n\n";

    std::cout << "--- midpoint ---\n";
    test_midpoint();

    std::cout << "\n--- clamp ---\n";
    test_clamp();

    std::cout << "\n--- abs ---\n";
    test_abs();

    std::cout << "\n--- sign ---\n";
    test_sign();

    std::cout << "\n--- gcd ---\n";
    test_gcd();

    std::cout << "\n--- lcm ---\n";
    test_lcm();

    std::cout << "\n--- power ---\n";
    test_power();

    std::cout << "\n--- isqrt ---\n";
    test_isqrt();

    std::cout << "\n--- ilog2 ---\n";
    test_ilog2();

    std::cout << "\n--- is_even / is_odd ---\n";
    test_parity();

    std::cout << "\n--- abs_diff ---\n";
    test_abs_diff();

    std::cout << "\n--- factorial ---\n";
    test_factorial();

    std::cout << "\n--- min / max variadic ---\n";
    test_min_max_variadic();

    std::cout << "\n--- divmod ---\n";
    test_divmod();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
