// Comprehensive tests for int128_cmath.hpp
// Mathematical functions with signed 128-bit integers
// Tests: gcd, lcm, abs, pow, sqrt, sign, max, min, clamp, midpoint

#include "int128/int128_cmath.hpp"
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace nstd;

// Test counter
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Test macro
#define TEST_ASSERT(condition, message)                                                            \
    do {                                                                                           \
        total_tests++;                                                                             \
        if (condition) {                                                                           \
            passed_tests++;                                                                        \
            std::cout << "  [OK] " << message << std::endl;                                        \
        } else {                                                                                   \
            failed_tests++;                                                                        \
            std::cout << "  [ERROR] FAILED: " << message << std::endl;                             \
        }                                                                                          \
    } while (0)

// Print section header
void print_section(const std::string& title)
{
    std::cout << "\n=== " << title << " ===" << std::endl;
}

// Test std::gcd
void test_gcd_basic()
{
    using namespace std;

    // Basic cases
    TEST_ASSERT(gcd(int128_t(12), int128_t(18)) == int128_t(6), "gcd(12, 18) == 6");
    TEST_ASSERT(gcd(int128_t(48), int128_t(18)) == int128_t(6), "gcd(48, 18) == 6");
    TEST_ASSERT(gcd(int128_t(100), int128_t(35)) == int128_t(5), "gcd(100, 35) == 5");

    // With negative numbers
    TEST_ASSERT(gcd(int128_t(-12), int128_t(18)) == int128_t(6), "gcd(-12, 18) == 6");
    TEST_ASSERT(gcd(int128_t(12), int128_t(-18)) == int128_t(6), "gcd(12, -18) == 6");
    TEST_ASSERT(gcd(int128_t(-12), int128_t(-18)) == int128_t(6), "gcd(-12, -18) == 6");

    // Edge cases
    TEST_ASSERT(gcd(int128_t(0), int128_t(5)) == int128_t(5), "gcd(0, 5) == 5");
    TEST_ASSERT(gcd(int128_t(5), int128_t(0)) == int128_t(5), "gcd(5, 0) == 5");
    TEST_ASSERT(gcd(int128_t(0), int128_t(0)) == int128_t(0), "gcd(0, 0) == 0");

    // Large values
    int128_t large1(1000000000LL);
    int128_t large2(500000000LL);
    TEST_ASSERT(gcd(large1, large2) == int128_t(500000000LL), "gcd(1e9, 5e8) == 5e8");

    // Powers of 2
    TEST_ASSERT(gcd(int128_t(64), int128_t(128)) == int128_t(64), "gcd(64, 128) == 64");
    TEST_ASSERT(gcd(int128_t(1024), int128_t(2048)) == int128_t(1024), "gcd(1024, 2048) == 1024");

    // Co-primes
    TEST_ASSERT(gcd(int128_t(17), int128_t(19)) == int128_t(1), "gcd(17, 19) == 1");
    TEST_ASSERT(gcd(int128_t(101), int128_t(103)) == int128_t(1), "gcd(101, 103) == 1");

    // Mixed types
    TEST_ASSERT(gcd(int128_t(12), 18) == int128_t(6), "gcd(int128_t, int) works");
    TEST_ASSERT(gcd(12, int128_t(18)) == int128_t(6), "gcd(int, int128_t) works");
}

// Test std::lcm
void test_lcm_basic()
{
    using namespace std;

    // Basic cases
    TEST_ASSERT(lcm(int128_t(4), int128_t(6)) == int128_t(12), "lcm(4, 6) == 12");
    TEST_ASSERT(lcm(int128_t(12), int128_t(18)) == int128_t(36), "lcm(12, 18) == 36");
    TEST_ASSERT(lcm(int128_t(21), int128_t(6)) == int128_t(42), "lcm(21, 6) == 42");

    // With negative numbers
    TEST_ASSERT(lcm(int128_t(-4), int128_t(6)) == int128_t(12), "lcm(-4, 6) == 12");
    TEST_ASSERT(lcm(int128_t(4), int128_t(-6)) == int128_t(12), "lcm(4, -6) == 12");
    TEST_ASSERT(lcm(int128_t(-4), int128_t(-6)) == int128_t(12), "lcm(-4, -6) == 12");

    // Edge cases
    TEST_ASSERT(lcm(int128_t(0), int128_t(5)) == int128_t(0), "lcm(0, 5) == 0");
    TEST_ASSERT(lcm(int128_t(5), int128_t(0)) == int128_t(0), "lcm(5, 0) == 0");
    TEST_ASSERT(lcm(int128_t(1), int128_t(5)) == int128_t(5), "lcm(1, 5) == 5");

    // Large values
    TEST_ASSERT(lcm(int128_t(100), int128_t(150)) == int128_t(300), "lcm(100, 150) == 300");
    TEST_ASSERT(lcm(int128_t(1000), int128_t(2000)) == int128_t(2000), "lcm(1000, 2000) == 2000");

    // Mixed types
    TEST_ASSERT(lcm(int128_t(4), 6) == int128_t(12), "lcm(int128_t, int) works");
    TEST_ASSERT(lcm(4, int128_t(6)) == int128_t(12), "lcm(int, int128_t) works");
}

// Test std::abs
void test_abs()
{
    using namespace std;

    // Positive numbers
    TEST_ASSERT(abs(int128_t(42)) == int128_t(42), "abs(42) == 42");
    TEST_ASSERT(abs(int128_t(12345)) == int128_t(12345), "abs(12345) == 12345");

    // Negative numbers
    TEST_ASSERT(abs(int128_t(-42)) == int128_t(42), "abs(-42) == 42");
    TEST_ASSERT(abs(int128_t(-12345)) == int128_t(12345), "abs(-12345) == 12345");

    // Zero
    TEST_ASSERT(abs(int128_t(0)) == int128_t(0), "abs(0) == 0");

    // Large values
    int128_t large(-1000000000LL);
    TEST_ASSERT(abs(large) == int128_t(1000000000LL), "abs(-1e9) == 1e9");
}

// Test std::pow
void test_pow_basic()
{
    using namespace std;

    // Basic cases
    TEST_ASSERT(pow(int128_t(2), int128_t(3)) == int128_t(8), "pow(2, 3) == 8");
    TEST_ASSERT(pow(int128_t(3), int128_t(4)) == int128_t(81), "pow(3, 4) == 81");
    TEST_ASSERT(pow(int128_t(5), int128_t(2)) == int128_t(25), "pow(5, 2) == 25");

    // Fast paths
    TEST_ASSERT(pow(int128_t(123), int128_t(0)) == int128_t(1), "pow(123, 0) == 1");
    TEST_ASSERT(pow(int128_t(123), int128_t(1)) == int128_t(123), "pow(123, 1) == 123");
    TEST_ASSERT(pow(int128_t(0), int128_t(5)) == int128_t(0), "pow(0, 5) == 0");
    TEST_ASSERT(pow(int128_t(1), int128_t(100)) == int128_t(1), "pow(1, 100) == 1");

    // Negative base
    TEST_ASSERT(pow(int128_t(-2), int128_t(3)) == int128_t(-8), "pow(-2, 3) == -8");
    TEST_ASSERT(pow(int128_t(-2), int128_t(4)) == int128_t(16), "pow(-2, 4) == 16");
    TEST_ASSERT(pow(int128_t(-3), int128_t(2)) == int128_t(9), "pow(-3, 2) == 9");
    TEST_ASSERT(pow(int128_t(-1), int128_t(5)) == int128_t(-1), "pow(-1, 5) == -1");
    TEST_ASSERT(pow(int128_t(-1), int128_t(6)) == int128_t(1), "pow(-1, 6) == 1");

    // Larger exponents
    TEST_ASSERT(pow(int128_t(2), int128_t(10)) == int128_t(1024), "pow(2, 10) == 1024");
    TEST_ASSERT(pow(int128_t(10), int128_t(5)) == int128_t(100000), "pow(10, 5) == 100000");

    // Mixed types
    TEST_ASSERT(pow(int128_t(2), 3) == int128_t(8), "pow(int128_t, int) works");
}

// Test std::sqrt
void test_sqrt_basic()
{
    using namespace std;

    // Perfect squares
    TEST_ASSERT(sqrt(int128_t(0)) == int128_t(0), "sqrt(0) == 0");
    TEST_ASSERT(sqrt(int128_t(1)) == int128_t(1), "sqrt(1) == 1");
    TEST_ASSERT(sqrt(int128_t(4)) == int128_t(2), "sqrt(4) == 2");
    TEST_ASSERT(sqrt(int128_t(9)) == int128_t(3), "sqrt(9) == 3");
    TEST_ASSERT(sqrt(int128_t(16)) == int128_t(4), "sqrt(16) == 4");
    TEST_ASSERT(sqrt(int128_t(25)) == int128_t(5), "sqrt(25) == 5");
    TEST_ASSERT(sqrt(int128_t(100)) == int128_t(10), "sqrt(100) == 10");
    TEST_ASSERT(sqrt(int128_t(144)) == int128_t(12), "sqrt(144) == 12");
    TEST_ASSERT(sqrt(int128_t(10000)) == int128_t(100), "sqrt(10000) == 100");

    // Non-perfect squares (floor)
    TEST_ASSERT(sqrt(int128_t(2)) == int128_t(1), "sqrt(2) == 1");
    TEST_ASSERT(sqrt(int128_t(3)) == int128_t(1), "sqrt(3) == 1");
    TEST_ASSERT(sqrt(int128_t(5)) == int128_t(2), "sqrt(5) == 2");
    TEST_ASSERT(sqrt(int128_t(8)) == int128_t(2), "sqrt(8) == 2");
    TEST_ASSERT(sqrt(int128_t(15)) == int128_t(3), "sqrt(15) == 3");
    TEST_ASSERT(sqrt(int128_t(99)) == int128_t(9), "sqrt(99) == 9");

    // Verify property: sqrt(n)^2 <= n < (sqrt(n)+1)^2
    for (int i = 1; i <= 20; ++i) {
        int128_t n(i * i + i); // Non-perfect square
        int128_t root = sqrt(n);
        int128_t root_sq = root * root;
        int128_t next_sq = (root + int128_t(1)) * (root + int128_t(1));
        TEST_ASSERT(root_sq <= n && n < next_sq,
                    "sqrt property holds for " + std::to_string(i * i + i));
    }

    // Large values
    int128_t large(1000000);
    TEST_ASSERT(sqrt(large) == int128_t(1000), "sqrt(1000000) == 1000");
}

// Test std::sign
void test_sign()
{
    using namespace std;

    // Positive
    TEST_ASSERT(sign(int128_t(1)) == int128_t(1), "sign(1) == 1");
    TEST_ASSERT(sign(int128_t(42)) == int128_t(1), "sign(42) == 1");
    TEST_ASSERT(sign(int128_t(12345)) == int128_t(1), "sign(12345) == 1");

    // Negative
    TEST_ASSERT(sign(int128_t(-1)) == int128_t(-1), "sign(-1) == -1");
    TEST_ASSERT(sign(int128_t(-42)) == int128_t(-1), "sign(-42) == -1");
    TEST_ASSERT(sign(int128_t(-12345)) == int128_t(-1), "sign(-12345) == -1");

    // Zero
    TEST_ASSERT(sign(int128_t(0)) == int128_t(0), "sign(0) == 0");
}

// Test std::min and std::max
void test_min_max()
{
    using namespace std;

    // Basic min
    TEST_ASSERT(min(int128_t(3), int128_t(5)) == int128_t(3), "min(3, 5) == 3");
    TEST_ASSERT(min(int128_t(5), int128_t(3)) == int128_t(3), "min(5, 3) == 3");
    TEST_ASSERT(min(int128_t(-3), int128_t(5)) == int128_t(-3), "min(-3, 5) == -3");
    TEST_ASSERT(min(int128_t(-5), int128_t(-3)) == int128_t(-5), "min(-5, -3) == -5");

    // Basic max
    TEST_ASSERT(max(int128_t(3), int128_t(5)) == int128_t(5), "max(3, 5) == 5");
    TEST_ASSERT(max(int128_t(5), int128_t(3)) == int128_t(5), "max(5, 3) == 5");
    TEST_ASSERT(max(int128_t(-3), int128_t(5)) == int128_t(5), "max(-3, 5) == 5");
    TEST_ASSERT(max(int128_t(-5), int128_t(-3)) == int128_t(-3), "max(-5, -3) == -3");

    // Equal values
    TEST_ASSERT(min(int128_t(5), int128_t(5)) == int128_t(5), "min(5, 5) == 5");
    TEST_ASSERT(max(int128_t(5), int128_t(5)) == int128_t(5), "max(5, 5) == 5");

    // Large values
    int128_t large1(1000000000LL);
    int128_t large2(-1000000000LL);
    TEST_ASSERT(min(large1, large2) == large2, "min(1e9, -1e9) == -1e9");
    TEST_ASSERT(max(large1, large2) == large1, "max(1e9, -1e9) == 1e9");

    // Mixed types
    TEST_ASSERT(min(int128_t(3), 5) == int128_t(3), "min(int128_t, int) works");
    TEST_ASSERT(max(3, int128_t(5)) == int128_t(5), "max(int, int128_t) works");
}

// Test std::clamp
void test_clamp()
{
    using namespace std;

    // Value within range
    TEST_ASSERT(clamp(int128_t(5), int128_t(0), int128_t(10)) == int128_t(5),
                "clamp(5, 0, 10) == 5");

    // Value below minimum
    TEST_ASSERT(clamp(int128_t(-5), int128_t(0), int128_t(10)) == int128_t(0),
                "clamp(-5, 0, 10) == 0");

    // Value above maximum
    TEST_ASSERT(clamp(int128_t(15), int128_t(0), int128_t(10)) == int128_t(10),
                "clamp(15, 0, 10) == 10");

    // Value at boundaries
    TEST_ASSERT(clamp(int128_t(0), int128_t(0), int128_t(10)) == int128_t(0),
                "clamp(0, 0, 10) == 0");
    TEST_ASSERT(clamp(int128_t(10), int128_t(0), int128_t(10)) == int128_t(10),
                "clamp(10, 0, 10) == 10");

    // Negative ranges
    TEST_ASSERT(clamp(int128_t(-5), int128_t(-10), int128_t(-1)) == int128_t(-5),
                "clamp(-5, -10, -1) == -5");
    TEST_ASSERT(clamp(int128_t(-15), int128_t(-10), int128_t(-1)) == int128_t(-10),
                "clamp(-15, -10, -1) == -10");
    TEST_ASSERT(clamp(int128_t(5), int128_t(-10), int128_t(-1)) == int128_t(-1),
                "clamp(5, -10, -1) == -1");
}

// Test std::midpoint
void test_midpoint()
{
    using namespace std;

    // Basic cases
    TEST_ASSERT(midpoint(int128_t(0), int128_t(10)) == int128_t(5), "midpoint(0, 10) == 5");
    TEST_ASSERT(midpoint(int128_t(10), int128_t(20)) == int128_t(15), "midpoint(10, 20) == 15");
    TEST_ASSERT(midpoint(int128_t(-10), int128_t(10)) == int128_t(0), "midpoint(-10, 10) == 0");

    // Negative values
    TEST_ASSERT(midpoint(int128_t(-20), int128_t(-10)) == int128_t(-15),
                "midpoint(-20, -10) == -15");
    TEST_ASSERT(midpoint(int128_t(-5), int128_t(-1)) == int128_t(-3), "midpoint(-5, -1) == -3");

    // Same values
    TEST_ASSERT(midpoint(int128_t(5), int128_t(5)) == int128_t(5), "midpoint(5, 5) == 5");

    // Odd sum (rounds towards a)
    TEST_ASSERT(midpoint(int128_t(1), int128_t(2)) == int128_t(1), "midpoint(1, 2) == 1");
    TEST_ASSERT(midpoint(int128_t(2), int128_t(1)) == int128_t(2), "midpoint(2, 1) == 2");

    // Large values
    int128_t large1(1000000000LL);
    int128_t large2(2000000000LL);
    TEST_ASSERT(midpoint(large1, large2) == int128_t(1500000000LL), "midpoint(1e9, 2e9) == 1.5e9");
}

// Test edge cases
void test_edge_cases()
{
    using namespace std;

    // Zero operations
    TEST_ASSERT(abs(int128_t(0)) == int128_t(0), "abs(0) == 0");
    TEST_ASSERT(sign(int128_t(0)) == int128_t(0), "sign(0) == 0");
    TEST_ASSERT(sqrt(int128_t(0)) == int128_t(0), "sqrt(0) == 0");

    // Identity operations
    TEST_ASSERT(gcd(int128_t(42), int128_t(1)) == int128_t(1), "gcd(n, 1) == 1");
    TEST_ASSERT(lcm(int128_t(42), int128_t(1)) == int128_t(42), "lcm(n, 1) == n");
    TEST_ASSERT(pow(int128_t(42), int128_t(1)) == int128_t(42), "pow(n, 1) == n");

    // Self operations
    TEST_ASSERT(min(int128_t(42), int128_t(42)) == int128_t(42), "min(n, n) == n");
    TEST_ASSERT(max(int128_t(42), int128_t(42)) == int128_t(42), "max(n, n) == n");
    TEST_ASSERT(midpoint(int128_t(42), int128_t(42)) == int128_t(42), "midpoint(n, n) == n");
}

// Test constexpr compatibility (GCC/Clang only)
void test_constexpr()
{
    print_section("Testing Constexpr Compatibility");

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
    // Compile-time evaluation (GCC/Clang)
    // Note: abs, pow, sqrt use intrinsics not constexpr, only constexpr functions work

    constexpr int128_t sign_result = nstd::sign(int128_t(-5));
    TEST_ASSERT(sign_result == int128_t(-1), "constexpr sign(-5) == -1");

    constexpr int128_t min_result = nstd::min(int128_t(3), int128_t(5));
    TEST_ASSERT(min_result == int128_t(3), "constexpr min(3, 5) == 3");

    constexpr int128_t max_result = nstd::max(int128_t(3), int128_t(5));
    TEST_ASSERT(max_result == int128_t(5), "constexpr max(3, 5) == 5");

    constexpr int128_t clamp_result = nstd::clamp(int128_t(15), int128_t(0), int128_t(10));
    TEST_ASSERT(clamp_result == int128_t(10), "constexpr clamp(15, 0, 10) == 10");

    // Runtime for functions with intrinsics
    int128_t abs_result = nstd::abs(int128_t(-42));
    TEST_ASSERT(abs_result == int128_t(42), "runtime abs(-42) == 42");

    int128_t pow_result = nstd::pow(int128_t(2), int128_t(3));
    TEST_ASSERT(pow_result == int128_t(8), "runtime pow(2, 3) == 8");

    int128_t sqrt_result = nstd::sqrt(int128_t(16));
    TEST_ASSERT(sqrt_result == int128_t(4), "runtime sqrt(16) == 4");

    int128_t midpoint_result = nstd::midpoint(int128_t(0), int128_t(10));
    TEST_ASSERT(midpoint_result == int128_t(5), "runtime midpoint(0, 10) == 5");
#else
    // Runtime evaluation (MSVC/Intel)
    std::cout << "Note: Constexpr tests skipped (MSVC/Intel intrinsics not constexpr)" << std::endl;

    int128_t abs_result = nstd::abs(int128_t(-42));
    TEST_ASSERT(abs_result == int128_t(42), "runtime abs(-42) == 42");

    int128_t pow_result = nstd::pow(int128_t(2), int128_t(3));
    TEST_ASSERT(pow_result == int128_t(8), "runtime pow(2, 3) == 8");

    int128_t sqrt_result = nstd::sqrt(int128_t(16));
    TEST_ASSERT(sqrt_result == int128_t(4), "runtime sqrt(16) == 4");

    int128_t sign_result = nstd::sign(int128_t(-5));
    TEST_ASSERT(sign_result == int128_t(-1), "runtime sign(-5) == -1");

    int128_t min_result = nstd::min(int128_t(3), int128_t(5));
    TEST_ASSERT(min_result == int128_t(3), "runtime min(3, 5) == 3");

    int128_t max_result = nstd::max(int128_t(3), int128_t(5));
    TEST_ASSERT(max_result == int128_t(5), "runtime max(3, 5) == 5");
#endif
}

int main()
{
    std::cout << "╔===========================================================╗" << std::endl;
    std::cout << "║  INT128_CMATH.HPP - COMPREHENSIVE TEST SUITE             ║" << std::endl;
    std::cout << "╚===========================================================╝" << std::endl;

    print_section("Testing std::gcd (Greatest Common Divisor)");
    test_gcd_basic();
    std::cout << "[GCD] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    int prev_total = total_tests;
    print_section("Testing std::lcm (Least Common Multiple)");
    test_lcm_basic();
    std::cout << "[LCM] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::abs (Absolute Value)");
    test_abs();
    std::cout << "[ABS] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::pow (Power Function)");
    test_pow_basic();
    std::cout << "[POW] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::sqrt (Square Root)");
    test_sqrt_basic();
    std::cout << "[SQRT] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::sign");
    test_sign();
    std::cout << "[SIGN] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::min and std::max");
    test_min_max();
    std::cout << "[MIN/MAX] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::clamp");
    test_clamp();
    std::cout << "[CLAMP] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing std::midpoint");
    test_midpoint();
    std::cout << "[MIDPOINT] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    print_section("Testing Edge Cases");
    test_edge_cases();
    std::cout << "[EDGE CASES] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    prev_total = total_tests;
    test_constexpr();
    std::cout << "[CONSTEXPR] Tests: " << total_tests << " | Passed: " << passed_tests
              << " | Failed: " << failed_tests << std::endl;

    // Summary
    std::cout << "\n╔===========================================================╗" << std::endl;
    std::cout << "║  FINAL SUMMARY                                            ║" << std::endl;
    std::cout << "╚===========================================================╝" << std::endl;
    std::cout << "Total Tests:  " << total_tests << std::endl;
    std::cout << "[OK] Passed:     " << passed_tests << std::endl;
    std::cout << "[ERROR] Failed:     " << failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1)
              << (100.0 * passed_tests / total_tests) << "%" << std::endl;

    std::cout << "\n=== Performance Notes ===" << std::endl;
    std::cout
        << "* std::gcd usa algoritmo binario (Stein's) con manejo de signos - O(log(min(a,b)))"
        << std::endl;
    std::cout << "* std::lcm optimizado: lcm(a,b) = abs(a*b)/gcd(a,b) - evita overflow"
              << std::endl;
    std::cout << "* std::pow usa exponenciación rápida con signos - O(log(exp))" << std::endl;
    std::cout << "* std::sqrt usa método de Newton para valores no-negativos" << std::endl;
    std::cout << "* std::midpoint evita overflow: a + (b-a)/2" << std::endl;
    std::cout << "* Todas las funciones constexpr son compatible con compile-time" << std::endl;

    return (failed_tests == 0) ? 0 : 1;
}
