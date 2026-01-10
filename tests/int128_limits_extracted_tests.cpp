/**
 * @file int128_limits_extracted_tests.cpp
 * @brief Tests completos de int128_base_limits.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_limits.hpp tests (22 tests)
 * - test_limits_template.cpp (10 tests)
 *
 * Total: 30+ tests cubriendo:
 * - is_specialized, is_signed, is_integer, is_exact
 * - is_bounded, is_modulo
 * - digits, digits10, radix
 * - min(), lowest(), max()
 * - epsilon(), round_error()
 * - infinity(), quiet_NaN(), signaling_NaN()
 * - denorm_min()
 */

#include "int128_base_limits.hpp"
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
// SECTION: Boolean properties
// =============================================================================

void test_boolean_properties()
{
    // uint128_t
    check(numeric_limits<uint128_t>::is_specialized, "uint128_t::is_specialized == true");
    check(numeric_limits<uint128_t>::is_signed == false, "uint128_t::is_signed == false");
    check(numeric_limits<uint128_t>::is_integer, "uint128_t::is_integer == true");
    check(numeric_limits<uint128_t>::is_exact, "uint128_t::is_exact == true");
    check(numeric_limits<uint128_t>::is_bounded, "uint128_t::is_bounded == true");
    check(numeric_limits<uint128_t>::is_modulo == true, "uint128_t::is_modulo == true");
    check(!numeric_limits<uint128_t>::has_infinity, "uint128_t::has_infinity == false");
    check(!numeric_limits<uint128_t>::has_quiet_NaN, "uint128_t::has_quiet_NaN == false");
    check(!numeric_limits<uint128_t>::has_signaling_NaN, "uint128_t::has_signaling_NaN == false");

    // int128_t
    check(numeric_limits<int128_t>::is_specialized, "int128_t::is_specialized == true");
    check(numeric_limits<int128_t>::is_signed == true, "int128_t::is_signed == true");
    check(numeric_limits<int128_t>::is_integer, "int128_t::is_integer == true");
    check(numeric_limits<int128_t>::is_exact, "int128_t::is_exact == true");
    check(numeric_limits<int128_t>::is_bounded, "int128_t::is_bounded == true");
    check(numeric_limits<int128_t>::is_modulo == false, "int128_t::is_modulo == false");
    check(!numeric_limits<int128_t>::has_infinity, "int128_t::has_infinity == false");
    check(!numeric_limits<int128_t>::has_quiet_NaN, "int128_t::has_quiet_NaN == false");
}

// =============================================================================
// SECTION: Numeric constants (digits, radix)
// =============================================================================

void test_numeric_constants()
{
    // uint128_t
    check(numeric_limits<uint128_t>::digits == 128, "uint128_t::digits == 128");
    check(numeric_limits<uint128_t>::digits10 == 38, "uint128_t::digits10 == 38");
    check(numeric_limits<uint128_t>::radix == 2, "uint128_t::radix == 2");

    // int128_t
    check(numeric_limits<int128_t>::digits == 127, "int128_t::digits == 127");
    check(numeric_limits<int128_t>::digits10 == 38, "int128_t::digits10 == 38");
    check(numeric_limits<int128_t>::radix == 2, "int128_t::radix == 2");
}

// =============================================================================
// SECTION: min/max/lowest
// =============================================================================

void test_min_max_lowest()
{
    // uint128_t
    {
        auto min_val = numeric_limits<uint128_t>::min();
        check(min_val.high() == 0 && min_val.low() == 0, "uint128_t::min() == 0");

        auto max_val = numeric_limits<uint128_t>::max();
        check(max_val.high() == UINT64_MAX && max_val.low() == UINT64_MAX, "uint128_t::max() == 2^128-1");

        auto lowest_val = numeric_limits<uint128_t>::lowest();
        check(lowest_val == min_val, "uint128_t::lowest() == min()");
    }

    // int128_t
    {
        auto min_val = numeric_limits<int128_t>::min();
        // -2^127 tiene high = 0x8000000000000000, low = 0
        check(min_val.high() == 0x8000000000000000ULL && min_val.low() == 0, "int128_t::min() == -2^127");
        check(min_val < int128_t(0), "int128_t::min() < 0");

        auto max_val = numeric_limits<int128_t>::max();
        // 2^127-1 tiene high = 0x7FFFFFFFFFFFFFFF, low = 0xFFFFFFFFFFFFFFFF
        check(max_val.high() == 0x7FFFFFFFFFFFFFFFULL && max_val.low() == UINT64_MAX, "int128_t::max() == 2^127-1");
        check(max_val > int128_t(0), "int128_t::max() > 0");

        auto lowest_val = numeric_limits<int128_t>::lowest();
        check(lowest_val == min_val, "int128_t::lowest() == min()");

        // Verificar overflow
        int128_t max_plus_one = max_val + int128_t(1);
        check(max_plus_one < int128_t(0), "int128_t::max() + 1 overflows to negative");
    }
}

// =============================================================================
// SECTION: Special values (epsilon, infinity, NaN)
// =============================================================================

void test_special_values()
{
    // uint128_t
    check(numeric_limits<uint128_t>::epsilon() == uint128_t(0), "uint128_t::epsilon() == 0");
    check(numeric_limits<uint128_t>::round_error() == uint128_t(0), "uint128_t::round_error() == 0");
    check(numeric_limits<uint128_t>::infinity() == uint128_t(0), "uint128_t::infinity() == 0");
    check(numeric_limits<uint128_t>::quiet_NaN() == uint128_t(0), "uint128_t::quiet_NaN() == 0");
    check(numeric_limits<uint128_t>::signaling_NaN() == uint128_t(0), "uint128_t::signaling_NaN() == 0");

    // int128_t
    check(numeric_limits<int128_t>::epsilon() == int128_t(0), "int128_t::epsilon() == 0");
    check(numeric_limits<int128_t>::round_error() == int128_t(0), "int128_t::round_error() == 0");
    check(numeric_limits<int128_t>::infinity() == int128_t(0), "int128_t::infinity() == 0");
    check(numeric_limits<int128_t>::quiet_NaN() == int128_t(0), "int128_t::quiet_NaN() == 0");
    check(numeric_limits<int128_t>::signaling_NaN() == int128_t(0), "int128_t::signaling_NaN() == 0");
}

// =============================================================================
// SECTION: denorm_min
// =============================================================================

void test_denorm_min()
{
    auto uint_denorm = numeric_limits<uint128_t>::denorm_min();
    check(uint_denorm == numeric_limits<uint128_t>::min(), "uint128_t::denorm_min() == min()");

    auto int_denorm = numeric_limits<int128_t>::denorm_min();
    check(int_denorm == numeric_limits<int128_t>::min(), "int128_t::denorm_min() == min()");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_limits.hpp tests ===\n\n";

    std::cout << "--- boolean properties ---\n";
    test_boolean_properties();

    std::cout << "\n--- numeric constants ---\n";
    test_numeric_constants();

    std::cout << "\n--- min/max/lowest ---\n";
    test_min_max_lowest();

    std::cout << "\n--- special values ---\n";
    test_special_values();

    std::cout << "\n--- denorm_min ---\n";
    test_denorm_min();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
