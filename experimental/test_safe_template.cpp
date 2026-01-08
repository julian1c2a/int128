/**
 * @file test_safe_template.cpp
 * @brief Tests para int128_base_safe.hpp - Operaciones seguras
 *
 * Tests: 25 total
 * - Conversion segura a tipos pequenos
 * - Deteccion de overflow/underflow
 * - Aritmetica segura (add, sub, mul, div, mod)
 * - Aritmetica con saturacion
 * - Range checking
 */

#include "../include_new/int128_base_safe.hpp"
#include <cmath>
#include <iostream>
#include <limits>

using namespace nstd;
using namespace nstd::int128_safe;

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)                                                                      \
    do {                                                                                           \
        if (condition) {                                                                           \
            std::cout << "[PASS] " << name << std::endl;                                           \
            tests_passed++;                                                                        \
        } else {                                                                                   \
            std::cout << "[FAIL] " << name << std::endl;                                           \
            tests_failed++;                                                                        \
        }                                                                                          \
    } while (0)

// =============================================================================
// Test: Conversiones seguras FROM uint128_t
// =============================================================================

void test_safe_cast_uint128_to_uint64()
{
    uint128_t small(12345ULL);
    auto result = safe_cast<signedness::unsigned_type, uint64_t>(small);
    TEST("safe_cast uint128->uint64 (small)", result.is_valid() && result.value == 12345ULL);

    uint128_t large(1ULL, 0ULL); // > 2^64
    auto result2 = safe_cast<signedness::unsigned_type, uint64_t>(large);
    TEST("safe_cast uint128->uint64 (overflow)", result2.status == conversion_result::overflow);
}

void test_safe_cast_uint128_to_int32()
{
    uint128_t small(1000ULL);
    auto result = safe_cast<signedness::unsigned_type, int32_t>(small);
    TEST("safe_cast uint128->int32 (small)", result.is_valid() && result.value == 1000);

    uint128_t max_int32(static_cast<uint64_t>(std::numeric_limits<int32_t>::max()));
    auto result2 = safe_cast<signedness::unsigned_type, int32_t>(max_int32);
    TEST("safe_cast uint128->int32 (max)", result2.is_valid());

    uint128_t too_large(static_cast<uint64_t>(std::numeric_limits<int32_t>::max()) + 1ULL);
    auto result3 = safe_cast<signedness::unsigned_type, int32_t>(too_large);
    TEST("safe_cast uint128->int32 (overflow)", result3.status == conversion_result::overflow);
}

// =============================================================================
// Test: Conversiones seguras FROM int128_t
// =============================================================================

void test_safe_cast_int128_to_int64()
{
    int128_t positive(12345LL);
    auto result = safe_cast<signedness::signed_type, int64_t>(positive);
    TEST("safe_cast int128->int64 (positive)", result.is_valid() && result.value == 12345LL);

    int128_t negative(-9999LL);
    auto result2 = safe_cast<signedness::signed_type, int64_t>(negative);
    TEST("safe_cast int128->int64 (negative)", result2.is_valid() && result2.value == -9999LL);
}

void test_safe_cast_int128_to_uint64()
{
    int128_t positive(12345LL);
    auto result = safe_cast<signedness::signed_type, uint64_t>(positive);
    TEST("safe_cast int128->uint64 (positive)", result.is_valid() && result.value == 12345ULL);

    int128_t negative(-100LL);
    auto result2 = safe_cast<signedness::signed_type, uint64_t>(negative);
    TEST("safe_cast int128->uint64 (underflow)", result2.status == conversion_result::underflow);
}

// =============================================================================
// Test: try_cast con std::optional
// =============================================================================

void test_try_cast()
{
    uint128_t small(42ULL);
    auto opt = try_cast<signedness::unsigned_type, int32_t>(small);
    TEST("try_cast success", opt.has_value() && *opt == 42);

    uint128_t large(1ULL, 0ULL);
    auto opt2 = try_cast<signedness::unsigned_type, int32_t>(large);
    TEST("try_cast failure (nullopt)", !opt2.has_value());
}

// =============================================================================
// Test: checked_cast con excepciones
// =============================================================================

void test_checked_cast()
{
    uint128_t small(100ULL);
    bool ok = false;
    try {
        auto val = checked_cast<signedness::unsigned_type, int32_t>(small);
        ok = (val == 100);
    } catch (...) {
        ok = false;
    }
    TEST("checked_cast success", ok);

    uint128_t large(1ULL, 0ULL);
    bool threw = false;
    try {
        [[maybe_unused]] auto val = checked_cast<signedness::unsigned_type, int32_t>(large);
    } catch (const std::overflow_error&) {
        threw = true;
    }
    TEST("checked_cast throws overflow_error", threw);
}

// =============================================================================
// Test: Aritmetica segura unsigned
// =============================================================================

void test_safe_add_unsigned()
{
    uint128_t a(100ULL);
    uint128_t b(200ULL);
    auto result = safe_add(a, b);
    TEST("safe_add unsigned (normal)", result.is_valid() && result.value == uint128_t(300ULL));

    uint128_t max = std::numeric_limits<uint128_t>::max();
    auto result2 = safe_add(max, uint128_t(1ULL));
    TEST("safe_add unsigned (overflow)", result2.status == conversion_result::overflow);
}

void test_safe_sub_unsigned()
{
    uint128_t a(500ULL);
    uint128_t b(200ULL);
    auto result = safe_sub(a, b);
    TEST("safe_sub unsigned (normal)", result.is_valid() && result.value == uint128_t(300ULL));

    auto result2 = safe_sub(b, a); // 200 - 500
    TEST("safe_sub unsigned (underflow)", result2.status == conversion_result::underflow);
}

void test_safe_mul_unsigned()
{
    uint128_t a(1000ULL);
    uint128_t b(2000ULL);
    auto result = safe_mul(a, b);
    TEST("safe_mul unsigned (normal)", result.is_valid() && result.value == uint128_t(2000000ULL));

    uint128_t large(1ULL, 0ULL);           // 2^64
    auto result2 = safe_mul(large, large); // (2^64)^2 = 2^128, overflow
    TEST("safe_mul unsigned (overflow)", result2.status == conversion_result::overflow);
}

void test_safe_div_unsigned()
{
    uint128_t a(1000ULL);
    uint128_t b(7ULL);
    auto result = safe_div(a, b);
    TEST("safe_div unsigned (normal)", result.is_valid() && result.value == uint128_t(142ULL));

    auto result2 = safe_div(a, uint128_t(0ULL));
    TEST("safe_div unsigned (div by zero)", result2.status == conversion_result::invalid_input);
}

// =============================================================================
// Test: Aritmetica segura signed
// =============================================================================

void test_safe_add_signed()
{
    int128_t a(100LL);
    int128_t b(200LL);
    auto result = safe_add(a, b);
    TEST("safe_add signed (positive)", result.is_valid() && result.value == int128_t(300LL));

    int128_t neg_a(-100LL);
    int128_t neg_b(-50LL);
    auto result2 = safe_add(neg_a, neg_b);
    TEST("safe_add signed (negative)", result2.is_valid() && result2.value == int128_t(-150LL));

    int128_t max = std::numeric_limits<int128_t>::max();
    auto result3 = safe_add(max, int128_t(1LL));
    TEST("safe_add signed (overflow)", result3.status == conversion_result::overflow);
}

void test_safe_mul_signed()
{
    int128_t a(-7LL);
    int128_t b(6LL);
    auto result = safe_mul(a, b);
    TEST("safe_mul signed (neg*pos)", result.is_valid() && result.value == int128_t(-42LL));

    int128_t c(-5LL);
    int128_t d(-8LL);
    auto result2 = safe_mul(c, d);
    TEST("safe_mul signed (neg*neg)", result2.is_valid() && result2.value == int128_t(40LL));
}

// =============================================================================
// Test: Saturating arithmetic
// =============================================================================

void test_saturating_add()
{
    uint128_t max = std::numeric_limits<uint128_t>::max();
    auto sat_result = saturating_add(max, uint128_t(100ULL));
    TEST("saturating_add unsigned (saturates to max)", sat_result == max);

    int128_t int_max = std::numeric_limits<int128_t>::max();
    auto sat_result2 = saturating_add(int_max, int128_t(1LL));
    TEST("saturating_add signed (saturates to max)", sat_result2 == int_max);
}

void test_saturating_sub()
{
    uint128_t a(50ULL);
    uint128_t b(100ULL);
    auto sat_result = saturating_sub(a, b);
    TEST("saturating_sub unsigned (saturates to 0)", sat_result == uint128_t(0ULL));

    int128_t int_min = std::numeric_limits<int128_t>::min();
    auto sat_result2 = saturating_sub(int_min, int128_t(1LL));
    TEST("saturating_sub signed (saturates to min)", sat_result2 == int_min);
}

// =============================================================================
// Test: safe_abs (signed only)
// =============================================================================

void test_safe_abs()
{
    int128_t neg(-12345LL);
    auto result = safe_abs<signedness::signed_type>(neg);
    TEST("safe_abs (normal)", result.is_valid() && result.value == int128_t(12345LL));

    int128_t min = std::numeric_limits<int128_t>::min();
    auto result2 = safe_abs<signedness::signed_type>(min);
    TEST("safe_abs (min overflow)", result2.status == conversion_result::overflow);

    auto sat = saturating_abs<signedness::signed_type>(min);
    TEST("saturating_abs (min -> max)", sat == std::numeric_limits<int128_t>::max());
}

// =============================================================================
// Test: Range utilities
// =============================================================================

void test_range_utilities()
{
    uint128_t val(50ULL);
    bool in = in_range(val, uint128_t(10ULL), uint128_t(100ULL));
    TEST("in_range (true)", in);

    bool out = in_range(val, uint128_t(60ULL), uint128_t(100ULL));
    TEST("in_range (false)", !out);

    auto clamped = clamp(uint128_t(150ULL), uint128_t(0ULL), uint128_t(100ULL));
    TEST("clamp (to max)", clamped == uint128_t(100ULL));

    auto clamped2 = clamp(int128_t(-200LL), int128_t(-100LL), int128_t(100LL));
    TEST("clamp signed (to min)", clamped2 == int128_t(-100LL));
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== Tests: int128_base_safe.hpp ===" << std::endl << std::endl;

    // Conversiones
    test_safe_cast_uint128_to_uint64();
    test_safe_cast_uint128_to_int32();
    test_safe_cast_int128_to_int64();
    test_safe_cast_int128_to_uint64();
    test_try_cast();
    test_checked_cast();

    // Aritmetica unsigned
    test_safe_add_unsigned();
    test_safe_sub_unsigned();
    test_safe_mul_unsigned();
    test_safe_div_unsigned();

    // Aritmetica signed
    test_safe_add_signed();
    test_safe_mul_signed();

    // Saturating
    test_saturating_add();
    test_saturating_sub();

    // abs
    test_safe_abs();

    // Range
    test_range_utilities();

    std::cout << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Passed: " << tests_passed << "/" << (tests_passed + tests_failed) << std::endl;
    std::cout << "==================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
