/**
 * @file test_uint128_cmath.cpp
 * @brief Comprehensive tests for uint128_cmath.hpp functions
 *
 * Tests all mathematical functions from std namespace:
 * - std::gcd (Greatest Common Divisor)
 * - std::lcm (Least Common Multiple)
 * - std::pow (Power function)
 * - std::sqrt (Square root)
 * - std::min / std::max (Min/Max functions)
 * - std::bezout_coeffs (Bézout coefficients - Extended Euclidean Algorithm)
 */

#include "../include/uint128/uint128_cmath.hpp"
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

// ========================= TEST UTILITIES =========================

uint64_t test_count = 0;
uint64_t test_passed = 0;
uint64_t test_failed = 0;

#define TEST_ASSERT(condition, msg)                                                                \
    do {                                                                                           \
        ++test_count;                                                                              \
        if (condition) {                                                                           \
            ++test_passed;                                                                         \
        } else {                                                                                   \
            ++test_failed;                                                                         \
            std::cerr << "FAIL: " << msg << " (line " << __LINE__ << ")" << std::endl;             \
        }                                                                                          \
    } while (0)

void print_test_summary(const std::string& section)
{
    std::cout << "\n[" << section << "] Tests: " << test_count << " | Passed: " << test_passed
              << " | Failed: " << test_failed << std::endl;
}

// ========================= TEST GCD =========================

void test_gcd_basic()
{
    std::cout << "\n=== Testing std::gcd (Greatest Common Divisor) ===" << std::endl;

    // Test cases básicos
    TEST_ASSERT(std::gcd(uint128_t(0), uint128_t(5)) == uint128_t(5), "gcd(0, 5) = 5");
    TEST_ASSERT(std::gcd(uint128_t(5), uint128_t(0)) == uint128_t(5), "gcd(5, 0) = 5");
    TEST_ASSERT(std::gcd(uint128_t(1), uint128_t(1)) == uint128_t(1), "gcd(1, 1) = 1");

    // Test cases clásicos
    TEST_ASSERT(std::gcd(uint128_t(48), uint128_t(18)) == uint128_t(6), "gcd(48, 18) = 6");
    TEST_ASSERT(std::gcd(uint128_t(54), uint128_t(24)) == uint128_t(6), "gcd(54, 24) = 6");
    TEST_ASSERT(std::gcd(uint128_t(100), uint128_t(50)) == uint128_t(50), "gcd(100, 50) = 50");

    // Números primos (gcd = 1)
    TEST_ASSERT(std::gcd(uint128_t(17), uint128_t(19)) == uint128_t(1), "gcd(17, 19) = 1");
    TEST_ASSERT(std::gcd(uint128_t(13), uint128_t(97)) == uint128_t(1), "gcd(13, 97) = 1");

    // Números grandes (64-bit)
    TEST_ASSERT(std::gcd(uint128_t(0, 1000000007), uint128_t(0, 1000000009)) == uint128_t(1),
                "gcd(1000000007, 1000000009) = 1 (primos grandes)");
    TEST_ASSERT(std::gcd(uint128_t(0, 123456789), uint128_t(0, 987654321)) == uint128_t(9),
                "gcd(123456789, 987654321) = 9");

    // Potencias de 2
    TEST_ASSERT(std::gcd(uint128_t(256), uint128_t(128)) == uint128_t(128), "gcd(256, 128) = 128");
    TEST_ASSERT(std::gcd(uint128_t(1024), uint128_t(512)) == uint128_t(512),
                "gcd(1024, 512) = 512");

    // Test con números 128-bit completos
    uint128_t large1(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    uint128_t large2(0x0FEDCBA987654321ULL, 0x123456789ABCDEFULL);
    uint128_t gcd_result = std::gcd(large1, large2);
    TEST_ASSERT(gcd_result > uint128_t(0), "gcd(large128, large128) > 0");

    // Test gcd con tipos mixtos
    TEST_ASSERT(std::gcd(uint128_t(48), 18) == uint128_t(6), "gcd(uint128_t, int) = 6");
    TEST_ASSERT(std::gcd(48, uint128_t(18)) == uint128_t(6), "gcd(int, uint128_t) = 6");
    TEST_ASSERT(std::gcd(uint128_t(100), 50ULL) == uint128_t(50), "gcd(uint128_t, uint64_t) = 50");

    print_test_summary("GCD");
}

// ========================= TEST LCM =========================

void test_lcm_basic()
{
    std::cout << "\n=== Testing std::lcm (Least Common Multiple) ===" << std::endl;

    // Test cases básicos
    TEST_ASSERT(std::lcm(uint128_t(0), uint128_t(5)) == uint128_t(0), "lcm(0, 5) = 0");
    TEST_ASSERT(std::lcm(uint128_t(5), uint128_t(0)) == uint128_t(0), "lcm(5, 0) = 0");
    TEST_ASSERT(std::lcm(uint128_t(1), uint128_t(1)) == uint128_t(1), "lcm(1, 1) = 1");

    // Test cases clásicos
    TEST_ASSERT(std::lcm(uint128_t(4), uint128_t(6)) == uint128_t(12), "lcm(4, 6) = 12");
    TEST_ASSERT(std::lcm(uint128_t(12), uint128_t(18)) == uint128_t(36), "lcm(12, 18) = 36");
    TEST_ASSERT(std::lcm(uint128_t(21), uint128_t(6)) == uint128_t(42), "lcm(21, 6) = 42");

    // Números primos (lcm = producto)
    TEST_ASSERT(std::lcm(uint128_t(7), uint128_t(13)) == uint128_t(91), "lcm(7, 13) = 91");
    TEST_ASSERT(std::lcm(uint128_t(11), uint128_t(17)) == uint128_t(187), "lcm(11, 17) = 187");

    // Múltiplos exactos
    TEST_ASSERT(std::lcm(uint128_t(10), uint128_t(5)) == uint128_t(10), "lcm(10, 5) = 10");
    TEST_ASSERT(std::lcm(uint128_t(100), uint128_t(25)) == uint128_t(100), "lcm(100, 25) = 100");

    // Test con tipos mixtos
    TEST_ASSERT(std::lcm(uint128_t(4), 6) == uint128_t(12), "lcm(uint128_t, int) = 12");
    TEST_ASSERT(std::lcm(12, uint128_t(18)) == uint128_t(36), "lcm(int, uint128_t) = 36");

    print_test_summary("LCM");
}

// ========================= TEST POW =========================

void test_pow_basic()
{
    std::cout << "\n=== Testing std::pow (Power Function) ===" << std::endl;

    // Test cases básicos
    TEST_ASSERT(std::pow(uint128_t(2), uint128_t(0)) == uint128_t(1), "2^0 = 1");
    TEST_ASSERT(std::pow(uint128_t(2), uint128_t(1)) == uint128_t(2), "2^1 = 2");
    TEST_ASSERT(std::pow(uint128_t(0), uint128_t(5)) == uint128_t(0), "0^5 = 0");
    TEST_ASSERT(std::pow(uint128_t(1), uint128_t(100)) == uint128_t(1), "1^100 = 1");

    // Potencias de 2
    TEST_ASSERT(std::pow(uint128_t(2), uint128_t(10)) == uint128_t(1024), "2^10 = 1024");
    TEST_ASSERT(std::pow(uint128_t(2), uint128_t(20)) == uint128_t(1048576), "2^20 = 1048576");

    // Potencias pequeñas
    TEST_ASSERT(std::pow(uint128_t(3), uint128_t(3)) == uint128_t(27), "3^3 = 27");
    TEST_ASSERT(std::pow(uint128_t(5), uint128_t(4)) == uint128_t(625), "5^4 = 625");
    TEST_ASSERT(std::pow(uint128_t(10), uint128_t(6)) == uint128_t(1000000), "10^6 = 1000000");

    // Potencias grandes
    uint128_t pow2_63 = std::pow(uint128_t(2), uint128_t(63));
    TEST_ASSERT(pow2_63 == uint128_t(0, 0x8000000000000000ULL), "2^63 correcto");

    uint128_t pow2_64 = std::pow(uint128_t(2), uint128_t(64));
    TEST_ASSERT(pow2_64 == uint128_t(1, 0), "2^64 correcto");

    // Test con tipo mixto
    TEST_ASSERT(std::pow(uint128_t(2), 10) == uint128_t(1024), "pow(uint128_t, int) = 1024");

    print_test_summary("POW");
}

// ========================= TEST SQRT =========================

void test_sqrt_basic()
{
    std::cout << "\n=== Testing std::sqrt (Square Root) ===" << std::endl;

    // Test cases básicos
    TEST_ASSERT(std::sqrt(uint128_t(0)) == uint128_t(0), "sqrt(0) = 0");
    TEST_ASSERT(std::sqrt(uint128_t(1)) == uint128_t(1), "sqrt(1) = 1");
    TEST_ASSERT(std::sqrt(uint128_t(4)) == uint128_t(2), "sqrt(4) = 2");
    TEST_ASSERT(std::sqrt(uint128_t(9)) == uint128_t(3), "sqrt(9) = 3");
    TEST_ASSERT(std::sqrt(uint128_t(16)) == uint128_t(4), "sqrt(16) = 4");
    TEST_ASSERT(std::sqrt(uint128_t(25)) == uint128_t(5), "sqrt(25) = 5");

    // Cuadrados perfectos
    TEST_ASSERT(std::sqrt(uint128_t(100)) == uint128_t(10), "sqrt(100) = 10");
    TEST_ASSERT(std::sqrt(uint128_t(144)) == uint128_t(12), "sqrt(144) = 12");
    TEST_ASSERT(std::sqrt(uint128_t(10000)) == uint128_t(100), "sqrt(10000) = 100");

    // Números no cuadrados perfectos (parte entera)
    TEST_ASSERT(std::sqrt(uint128_t(2)) == uint128_t(1), "sqrt(2) = 1 (truncado)");
    TEST_ASSERT(std::sqrt(uint128_t(5)) == uint128_t(2), "sqrt(5) = 2 (truncado)");
    TEST_ASSERT(std::sqrt(uint128_t(10)) == uint128_t(3), "sqrt(10) = 3 (truncado)");

    // Números grandes
    TEST_ASSERT(std::sqrt(uint128_t(0, 100000000)) == uint128_t(10000), "sqrt(100000000) = 10000");

    uint128_t large = uint128_t(1) << 64;
    uint128_t sqrt_large = std::sqrt(large);
    TEST_ASSERT(sqrt_large == (uint128_t(1) << 32), "sqrt(2^64) = 2^32");

    // Verificar que sqrt(n)^2 <= n < (sqrt(n)+1)^2
    for (uint64_t n = 50; n < 60; ++n) {
        uint128_t val(n);
        uint128_t s = std::sqrt(val);
        uint128_t s_squared = s * s;
        uint128_t s_plus_1_squared = (s + uint128_t(1)) * (s + uint128_t(1));
        TEST_ASSERT(s_squared <= val && val < s_plus_1_squared,
                    "sqrt(" + std::to_string(n) + ") correcto");
    }

    print_test_summary("SQRT");
}

// ========================= TEST MIN/MAX =========================

void test_min_max()
{
    std::cout << "\n=== Testing std::min and std::max ===" << std::endl;

    // Test min
    TEST_ASSERT(std::min(uint128_t(5), uint128_t(10)) == uint128_t(5), "min(5, 10) = 5");
    TEST_ASSERT(std::min(uint128_t(100), uint128_t(50)) == uint128_t(50), "min(100, 50) = 50");
    TEST_ASSERT(std::min(uint128_t(7), uint128_t(7)) == uint128_t(7), "min(7, 7) = 7");

    // Test max
    TEST_ASSERT(std::max(uint128_t(5), uint128_t(10)) == uint128_t(10), "max(5, 10) = 10");
    TEST_ASSERT(std::max(uint128_t(100), uint128_t(50)) == uint128_t(100), "max(100, 50) = 100");
    TEST_ASSERT(std::max(uint128_t(7), uint128_t(7)) == uint128_t(7), "max(7, 7) = 7");

    // Test con tipos mixtos - min
    TEST_ASSERT(std::min(uint128_t(5), 10) == uint128_t(5), "min(uint128_t, int) = 5");
    TEST_ASSERT(std::min(100, uint128_t(50)) == uint128_t(50), "min(int, uint128_t) = 50");
    TEST_ASSERT(std::min(uint128_t(7), 7ULL) == uint128_t(7), "min(uint128_t, uint64_t) = 7");

    // Test con tipos mixtos - max
    TEST_ASSERT(std::max(uint128_t(5), 10) == uint128_t(10), "max(uint128_t, int) = 10");
    TEST_ASSERT(std::max(100, uint128_t(50)) == uint128_t(100), "max(int, uint128_t) = 100");
    TEST_ASSERT(std::max(uint128_t(7), 7ULL) == uint128_t(7), "max(uint128_t, uint64_t) = 7");

    // Test con números grandes
    uint128_t large1(1, 0);
    uint128_t large2(0, 0xFFFFFFFFFFFFFFFFULL);
    TEST_ASSERT(std::max(large1, large2) == large1, "max con 128-bit");
    TEST_ASSERT(std::min(large1, large2) == large2, "min con 128-bit");

    print_test_summary("MIN/MAX");
}

// ========================= TEST BEZOUT COEFFICIENTS =========================

void test_bezout_coeffs()
{
    std::cout << "\n=== Testing std::bezout_coeffs (Extended Euclidean Algorithm) ===" << std::endl;

    // Helper para verificar la identidad de Bézout: a*x + b*y = gcd(a,b)
    auto verify_bezout = [](uint128_t a, uint128_t b, std::bezout_coeff x, std::bezout_coeff y) {
        uint128_t g = std::gcd(a, b);

        // Calcular a*x (con signo)
        uint128_t ax = a * x.magnitude;

        // Calcular b*y (con signo)
        uint128_t by = b * y.magnitude;

        // Verificar la identidad según los signos
        uint128_t result;
        if (!x.is_negative && !y.is_negative) {
            // Ambos positivos: a*x + b*y
            result = ax + by;
        } else if (x.is_negative && !y.is_negative) {
            // x negativo: -a*x + b*y = b*y - a*x
            if (by >= ax) {
                result = by - ax;
            } else {
                return false; // No puede ser negativo
            }
        } else if (!x.is_negative && y.is_negative) {
            // y negativo: a*x - b*y
            if (ax >= by) {
                result = ax - by;
            } else {
                return false;
            }
        } else {
            // Ambos negativos: -a*x - b*y (resultado negativo, no válido para unsigned)
            return false;
        }

        return result == g;
    };

    // Test case clásico: gcd(48, 18) = 6
    // Una solución conocida: x=-1, y=3: 48*(-1) + 18*3 = -48 + 54 = 6
    auto [x1, y1] = std::bezout_coeffs(uint128_t(48), uint128_t(18));
    TEST_ASSERT(verify_bezout(uint128_t(48), uint128_t(18), x1, y1),
                "bezout_coeffs(48, 18) satisface identidad");

    // Test case: gcd(54, 24) = 6
    auto [x2, y2] = std::bezout_coeffs(uint128_t(54), uint128_t(24));
    TEST_ASSERT(verify_bezout(uint128_t(54), uint128_t(24), x2, y2),
                "bezout_coeffs(54, 24) satisface identidad");

    // Test case: números coprimos (gcd = 1)
    auto [x3, y3] = std::bezout_coeffs(uint128_t(17), uint128_t(19));
    TEST_ASSERT(verify_bezout(uint128_t(17), uint128_t(19), x3, y3),
                "bezout_coeffs(17, 19) satisface identidad");

    // Edge cases
    auto [x4, y4] = std::bezout_coeffs(uint128_t(0), uint128_t(5));
    TEST_ASSERT(x4.magnitude == uint128_t(0) && y4.magnitude == uint128_t(1),
                "bezout_coeffs(0, 5) = (0, 1)");

    auto [x5, y5] = std::bezout_coeffs(uint128_t(5), uint128_t(0));
    TEST_ASSERT(x5.magnitude == uint128_t(1) && y5.magnitude == uint128_t(0),
                "bezout_coeffs(5, 0) = (1, 0)");

    // Verificar estructura bezout_coeff
    std::bezout_coeff c1(10, false);
    std::bezout_coeff c2(10, false);
    std::bezout_coeff c3(10, true);
    TEST_ASSERT(c1 == c2, "bezout_coeff operator== funciona (iguales)");
    TEST_ASSERT(!(c1 == c3), "bezout_coeff operator== funciona (diferentes signos)");

    print_test_summary("BEZOUT");
}

// ========================= EDGE CASES & STRESS TESTS =========================

void test_edge_cases()
{
    std::cout << "\n=== Testing Edge Cases ===" << std::endl;

    // GCD con valores máximos
    uint128_t max_val = std::numeric_limits<uint128_t>::max();
    TEST_ASSERT(std::gcd(max_val, max_val) == max_val, "gcd(max, max) = max");
    TEST_ASSERT(std::gcd(max_val, uint128_t(1)) == uint128_t(1), "gcd(max, 1) = 1");

    // LCM overflow prevention
    uint128_t big1 = uint128_t(1) << 63;
    uint128_t big2 = uint128_t(1) << 62;
    uint128_t lcm_big = std::lcm(big1, big2);
    TEST_ASSERT(lcm_big == big1, "lcm(2^63, 2^62) = 2^63");

    // POW con exponente grande pero resultado pequeño
    TEST_ASSERT(std::pow(uint128_t(1), uint128_t(127)) == uint128_t(1),
                "1^127 = 1 (exponente grande)");

    // SQRT de números muy grandes
    uint128_t very_large(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    uint128_t sqrt_very_large = std::sqrt(very_large);
    uint128_t sqrt_squared = sqrt_very_large * sqrt_very_large;
    TEST_ASSERT(sqrt_squared <= very_large, "sqrt(max)^2 <= max");

    print_test_summary("EDGE CASES");
}

// ========================= CONSTEXPR TESTS =========================

void test_constexpr()
{
    std::cout << "\n=== Testing Constexpr Compatibility ===" << std::endl;

#if defined(__GNUC__) && !defined(__INTEL_COMPILER) && !defined(_MSC_VER)
    // Test que las funciones sean constexpr (solo GCC/Clang puro)
    // MSVC/Intel tienen intrínsecos no-constexpr en trailing_zeros usado por división
    constexpr uint128_t gcd_result = std::gcd(uint128_t(48), uint128_t(18));
    TEST_ASSERT(gcd_result == uint128_t(6), "gcd es constexpr");

    constexpr uint128_t lcm_result = std::lcm(uint128_t(4), uint128_t(6));
    TEST_ASSERT(lcm_result == uint128_t(12), "lcm es constexpr");

    constexpr uint128_t pow_result = std::pow(uint128_t(2), uint128_t(10));
    TEST_ASSERT(pow_result == uint128_t(1024), "pow es constexpr");

    constexpr uint128_t sqrt_result = std::sqrt(uint128_t(144));
    TEST_ASSERT(sqrt_result == uint128_t(12), "sqrt es constexpr");

    constexpr uint128_t min_result = std::min(uint128_t(5), uint128_t(10));
    TEST_ASSERT(min_result == uint128_t(5), "min es constexpr");

    constexpr uint128_t max_result = std::max(uint128_t(5), uint128_t(10));
    TEST_ASSERT(max_result == uint128_t(10), "max es constexpr");
#else
    // Runtime tests para MSVC/Intel
    uint128_t gcd_result = std::gcd(uint128_t(48), uint128_t(18));
    TEST_ASSERT(gcd_result == uint128_t(6), "gcd funciona (runtime)");

    uint128_t lcm_result = std::lcm(uint128_t(4), uint128_t(6));
    TEST_ASSERT(lcm_result == uint128_t(12), "lcm funciona (runtime)");

    uint128_t pow_result = std::pow(uint128_t(2), uint128_t(10));
    TEST_ASSERT(pow_result == uint128_t(1024), "pow funciona (runtime)");

    uint128_t sqrt_result = std::sqrt(uint128_t(144));
    TEST_ASSERT(sqrt_result == uint128_t(12), "sqrt funciona (runtime)");

    uint128_t min_result = std::min(uint128_t(5), uint128_t(10));
    TEST_ASSERT(min_result == uint128_t(5), "min funciona (runtime)");

    uint128_t max_result = std::max(uint128_t(5), uint128_t(10));
    TEST_ASSERT(max_result == uint128_t(10), "max funciona (runtime)");

    std::cout << "Note: Constexpr tests skipped (MSVC/Intel intrinsics not constexpr)" << std::endl;
#endif

    print_test_summary("CONSTEXPR");
}

// ========================= PERFORMANCE NOTES =========================

void print_performance_notes()
{
    std::cout << "\n=== Performance Notes ===" << std::endl;
    std::cout << "• std::gcd usa algoritmo binario (Stein's) - O(log(min(a,b)))" << std::endl;
    std::cout << "• std::lcm optimizado: lcm(a,b) = (a/gcd(a,b))*b para evitar overflow"
              << std::endl;
    std::cout << "• std::pow usa exponenciación rápida - O(log(exp))" << std::endl;
    std::cout << "• std::sqrt usa método de Newton - convergencia cuadrática" << std::endl;
    std::cout << "• Todas las funciones son constexpr y compatible con compile-time" << std::endl;
}

// ========================= MAIN =========================

int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  UINT128_CMATH.HPP - COMPREHENSIVE TEST SUITE               ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;

    test_gcd_basic();
    test_lcm_basic();
    test_pow_basic();
    test_sqrt_basic();
    test_min_max();
    test_bezout_coeffs();
    test_edge_cases();
    test_constexpr();

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  FINAL SUMMARY                                               ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "Total Tests:  " << test_count << std::endl;
    std::cout << "✓ Passed:     " << test_passed << std::endl;
    std::cout << "✗ Failed:     " << test_failed << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1)
              << (100.0 * test_passed / test_count) << "%" << std::endl;

    print_performance_notes();

    return (test_failed == 0) ? 0 : 1;
}
