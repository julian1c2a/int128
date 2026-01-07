/**
 * @file test_numeric_template.cpp
 * @brief Test de funciones numéricas para int128_base_t<S>
 * @date 2026-01-07
 */

#include "../include_new/int128_base_numeric.hpp"
#include <iostream>

using namespace nstd;

int passed = 0;
int failed = 0;

void test(bool condition, const char* name)
{
    std::cout << "[" << (condition ? "OK" : "FAIL") << "] " << name << "\n";
    condition ? ++passed : ++failed;
}

int main()
{
    std::cout << "=== Test numeric para int128_base_t<S> ===\n\n";

    // ==========================================================================
    // MIDPOINT
    // ==========================================================================
    std::cout << "--- midpoint ---\n";
    {
        uint128_t a(100), b(200);
        test(midpoint(a, b) == uint128_t(150), "midpoint(100, 200) == 150 (unsigned)");

        uint128_t c(0), d(100);
        test(midpoint(c, d) == uint128_t(50), "midpoint(0, 100) == 50 (unsigned)");

        int128_t ia(-100), ib(100);
        test(midpoint(ia, ib) == int128_t(0), "midpoint(-100, 100) == 0 (signed)");

        int128_t ic(-200), id(-100);
        test(midpoint(ic, id) == int128_t(-150), "midpoint(-200, -100) == -150 (signed)");
    }

    // ==========================================================================
    // CLAMP
    // ==========================================================================
    std::cout << "\n--- clamp ---\n";
    {
        uint128_t lo(10), hi(100);

        test(clamp(uint128_t(50), lo, hi) == uint128_t(50), "clamp(50, 10, 100) == 50");
        test(clamp(uint128_t(5), lo, hi) == lo, "clamp(5, 10, 100) == 10");
        test(clamp(uint128_t(150), lo, hi) == hi, "clamp(150, 10, 100) == 100");

        int128_t ilo(-50), ihi(50);
        test(clamp(int128_t(-100), ilo, ihi) == ilo, "clamp(-100, -50, 50) == -50");
        test(clamp(int128_t(100), ilo, ihi) == ihi, "clamp(100, -50, 50) == 50");
        test(clamp(int128_t(0), ilo, ihi) == int128_t(0), "clamp(0, -50, 50) == 0");
    }

    // ==========================================================================
    // ABS
    // ==========================================================================
    std::cout << "\n--- abs ---\n";
    {
        uint128_t uval(42);
        test(abs(uval) == uval, "abs(uint128(42)) == 42");

        int128_t pos(42);
        test(abs(pos) == int128_t(42), "abs(int128(42)) == 42");

        int128_t neg(-42);
        test(abs(neg) == int128_t(42), "abs(int128(-42)) == 42");

        int128_t zero(0);
        test(abs(zero) == int128_t(0), "abs(int128(0)) == 0");
    }

    // ==========================================================================
    // SIGN
    // ==========================================================================
    std::cout << "\n--- sign ---\n";
    {
        uint128_t uzero(0);
        test(sign(uzero) == 0, "sign(uint128(0)) == 0");

        uint128_t upos(42);
        test(sign(upos) == 1, "sign(uint128(42)) == 1");

        int128_t zero(0);
        test(sign(zero) == 0, "sign(int128(0)) == 0");

        int128_t pos(42);
        test(sign(pos) == 1, "sign(int128(42)) == 1");

        int128_t neg(-42);
        test(sign(neg) == -1, "sign(int128(-42)) == -1");
    }

    // ==========================================================================
    // GCD
    // ==========================================================================
    std::cout << "\n--- gcd ---\n";
    {
        test(gcd(uint128_t(12), uint128_t(8)) == uint128_t(4), "gcd(12, 8) == 4");
        test(gcd(uint128_t(100), uint128_t(25)) == uint128_t(25), "gcd(100, 25) == 25");
        test(gcd(uint128_t(17), uint128_t(13)) == uint128_t(1), "gcd(17, 13) == 1 (coprimos)");
        test(gcd(uint128_t(0), uint128_t(5)) == uint128_t(5), "gcd(0, 5) == 5");
        test(gcd(uint128_t(5), uint128_t(0)) == uint128_t(5), "gcd(5, 0) == 5");

        // Signed
        test(gcd(int128_t(-12), int128_t(8)) == int128_t(4), "gcd(-12, 8) == 4");
        test(gcd(int128_t(12), int128_t(-8)) == int128_t(4), "gcd(12, -8) == 4");
        test(gcd(int128_t(-12), int128_t(-8)) == int128_t(4), "gcd(-12, -8) == 4");
    }

    // ==========================================================================
    // LCM
    // ==========================================================================
    std::cout << "\n--- lcm ---\n";
    {
        test(lcm(uint128_t(4), uint128_t(6)) == uint128_t(12), "lcm(4, 6) == 12");
        test(lcm(uint128_t(3), uint128_t(5)) == uint128_t(15), "lcm(3, 5) == 15");
        test(lcm(uint128_t(0), uint128_t(5)) == uint128_t(0), "lcm(0, 5) == 0");

        test(lcm(int128_t(-4), int128_t(6)) == int128_t(12), "lcm(-4, 6) == 12");
        test(lcm(int128_t(4), int128_t(-6)) == int128_t(12), "lcm(4, -6) == 12");
    }

    // ==========================================================================
    // POWER
    // ==========================================================================
    std::cout << "\n--- power ---\n";
    {
        test(power(uint128_t(2), 0u) == uint128_t(1), "power(2, 0) == 1");
        test(power(uint128_t(2), 1u) == uint128_t(2), "power(2, 1) == 2");
        test(power(uint128_t(2), 10u) == uint128_t(1024), "power(2, 10) == 1024");
        test(power(uint128_t(3), 4u) == uint128_t(81), "power(3, 4) == 81");

        test(power(int128_t(-2), 3u) == int128_t(-8), "power(-2, 3) == -8");
        test(power(int128_t(-2), 4u) == int128_t(16), "power(-2, 4) == 16");
    }

    // ==========================================================================
    // ISQRT
    // ==========================================================================
    std::cout << "\n--- isqrt ---\n";
    {
        test(isqrt(uint128_t(0)) == uint128_t(0), "isqrt(0) == 0");
        test(isqrt(uint128_t(1)) == uint128_t(1), "isqrt(1) == 1");
        test(isqrt(uint128_t(4)) == uint128_t(2), "isqrt(4) == 2");
        test(isqrt(uint128_t(9)) == uint128_t(3), "isqrt(9) == 3");
        test(isqrt(uint128_t(10)) == uint128_t(3), "isqrt(10) == 3 (floor)");
        test(isqrt(uint128_t(100)) == uint128_t(10), "isqrt(100) == 10");
        test(isqrt(uint128_t(1000000)) == uint128_t(1000), "isqrt(1000000) == 1000");

        test(isqrt(int128_t(-1)) == int128_t(0), "isqrt(int128(-1)) == 0 (neg)");
        test(isqrt(int128_t(100)) == int128_t(10), "isqrt(int128(100)) == 10");
    }

    // ==========================================================================
    // ILOG2
    // ==========================================================================
    std::cout << "\n--- ilog2 ---\n";
    {
        test(ilog2(uint128_t(1)) == 0, "ilog2(1) == 0");
        test(ilog2(uint128_t(2)) == 1, "ilog2(2) == 1");
        test(ilog2(uint128_t(4)) == 2, "ilog2(4) == 2");
        test(ilog2(uint128_t(8)) == 3, "ilog2(8) == 3");
        test(ilog2(uint128_t(15)) == 3, "ilog2(15) == 3 (floor)");
        test(ilog2(uint128_t(16)) == 4, "ilog2(16) == 4");
        test(ilog2(uint128_t(0)) == -1, "ilog2(0) == -1");

        test(ilog2(int128_t(-1)) == -1, "ilog2(int128(-1)) == -1");
        test(ilog2(int128_t(16)) == 4, "ilog2(int128(16)) == 4");
    }

    // ==========================================================================
    // IS_EVEN / IS_ODD
    // ==========================================================================
    std::cout << "\n--- is_even / is_odd ---\n";
    {
        test(is_even(uint128_t(0)), "is_even(0)");
        test(is_even(uint128_t(2)), "is_even(2)");
        test(is_even(uint128_t(100)), "is_even(100)");
        test(!is_even(uint128_t(1)), "!is_even(1)");
        test(!is_even(uint128_t(99)), "!is_even(99)");

        test(is_odd(uint128_t(1)), "is_odd(1)");
        test(is_odd(uint128_t(99)), "is_odd(99)");
        test(!is_odd(uint128_t(0)), "!is_odd(0)");
        test(!is_odd(uint128_t(100)), "!is_odd(100)");

        test(is_even(int128_t(-2)), "is_even(int128(-2))");
        test(is_odd(int128_t(-1)), "is_odd(int128(-1))");
    }

    // ==========================================================================
    // ABS_DIFF
    // ==========================================================================
    std::cout << "\n--- abs_diff ---\n";
    {
        test(abs_diff(uint128_t(10), uint128_t(3)) == uint128_t(7), "abs_diff(10, 3) == 7");
        test(abs_diff(uint128_t(3), uint128_t(10)) == uint128_t(7), "abs_diff(3, 10) == 7");
        test(abs_diff(uint128_t(5), uint128_t(5)) == uint128_t(0), "abs_diff(5, 5) == 0");

        test(abs_diff(int128_t(-10), int128_t(10)) == int128_t(20), "abs_diff(-10, 10) == 20");
        test(abs_diff(int128_t(10), int128_t(-10)) == int128_t(20), "abs_diff(10, -10) == 20");
    }

    // ==========================================================================
    // FACTORIAL
    // ==========================================================================
    std::cout << "\n--- factorial ---\n";
    {
        test(factorial<signedness::unsigned_type>(0) == uint128_t(1), "factorial(0) == 1");
        test(factorial<signedness::unsigned_type>(1) == uint128_t(1), "factorial(1) == 1");
        test(factorial<signedness::unsigned_type>(5) == uint128_t(120), "factorial(5) == 120");
        test(factorial<signedness::unsigned_type>(10) == uint128_t(3628800),
             "factorial(10) == 3628800");
        test(factorial<signedness::unsigned_type>(20) == uint128_t(2432902008176640000ULL),
             "factorial(20) correctness");
    }

    // ==========================================================================
    // MIN / MAX
    // ==========================================================================
    std::cout << "\n--- min / max ---\n";
    {
        test(min(uint128_t(5), uint128_t(10)) == uint128_t(5), "min(5, 10) == 5");
        test(max(uint128_t(5), uint128_t(10)) == uint128_t(10), "max(5, 10) == 10");

        test(min(int128_t(-5), int128_t(10)) == int128_t(-5), "min(-5, 10) == -5");
        test(max(int128_t(-5), int128_t(10)) == int128_t(10), "max(-5, 10) == 10");

        // Múltiples argumentos
        test(min(uint128_t(10), uint128_t(5), uint128_t(8)) == uint128_t(5), "min(10, 5, 8) == 5");
        test(max(uint128_t(10), uint128_t(5), uint128_t(8)) == uint128_t(10),
             "max(10, 5, 8) == 10");
    }

    // ==========================================================================
    // DIVMOD
    // ==========================================================================
    std::cout << "\n--- divmod ---\n";
    {
        auto [q, r] = divmod(uint128_t(17), uint128_t(5));
        test(q == uint128_t(3), "divmod(17, 5).quotient == 3");
        test(r == uint128_t(2), "divmod(17, 5).remainder == 2");

        auto [q2, r2] = divmod(uint128_t(100), uint128_t(10));
        test(q2 == uint128_t(10), "divmod(100, 10).quotient == 10");
        test(r2 == uint128_t(0), "divmod(100, 10).remainder == 0");
    }

    // ==========================================================================
    // RESULTADO FINAL
    // ==========================================================================
    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
