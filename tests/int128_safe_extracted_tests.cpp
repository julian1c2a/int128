/**
 * @file int128_safe_extracted_tests.cpp
 * @brief Tests completos de int128_base_safe.hpp (template unificado)
 *
 * Fusion de:
 * - int128/int128_safe.hpp tests (legacy)
 * - test_safe_template.cpp (25 tests)
 * - int128_base_safe_tests.cpp (33 tests)
 *
 * Total: 45+ tests cubriendo:
 * - Conversiones seguras (safe_cast, try_cast, checked_cast)
 * - Aritmetica segura (add, sub, mul, div, mod)
 * - Shift seguro (shl, shr)
 * - Aritmetica con saturacion
 * - safe_abs
 * - Range utilities (in_range, clamp)
 */

#include "int128_base_safe.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace nstd;
using namespace nstd::int128_safe;

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
// SECTION: safe_cast TO SMALLER TYPES (uint128_t source)
// =============================================================================

void test_safe_cast_uint128_to_uint64()
{
    uint128_t small(12345ULL);
    auto result = safe_cast<signedness::unsigned_type, uint64_t>(small);
    check(result.is_valid() && result.value == 12345ULL, "safe_cast uint128->uint64 (small)");

    uint128_t big(1ULL, 0ULL); // 2^64
    auto overflow = safe_cast<signedness::unsigned_type, uint64_t>(big);
    check(!overflow.is_valid() && overflow.status == conversion_result::overflow,
          "safe_cast uint128->uint64 (overflow)");
}

void test_safe_cast_uint128_to_int64()
{
    uint128_t small(1000ULL);
    auto result = safe_cast<signedness::unsigned_type, int64_t>(small);
    check(result.is_valid() && result.value == 1000LL, "safe_cast uint128->int64 (small)");

    uint128_t big(0x8000000000000000ULL);
    auto overflow = safe_cast<signedness::unsigned_type, int64_t>(big);
    check(!overflow.is_valid() && overflow.status == conversion_result::overflow,
          "safe_cast uint128->int64 (overflow)");
}

void test_safe_cast_uint128_to_uint32()
{
    uint128_t max32(0xFFFFFFFFULL);
    auto result = safe_cast<signedness::unsigned_type, uint32_t>(max32);
    check(result.is_valid() && result.value == 0xFFFFFFFFU, "safe_cast uint128->uint32 (max)");

    uint128_t big(0x100000000ULL);
    auto overflow = safe_cast<signedness::unsigned_type, uint32_t>(big);
    check(!overflow.is_valid(), "safe_cast uint128->uint32 (overflow)");
}

void test_safe_cast_uint128_to_int32()
{
    uint128_t small(1000ULL);
    auto result = safe_cast<signedness::unsigned_type, int32_t>(small);
    check(result.is_valid() && result.value == 1000, "safe_cast uint128->int32 (small)");

    uint128_t max_int32(static_cast<uint64_t>(std::numeric_limits<int32_t>::max()));
    auto result2 = safe_cast<signedness::unsigned_type, int32_t>(max_int32);
    check(result2.is_valid(), "safe_cast uint128->int32 (INT32_MAX)");

    uint128_t too_large(static_cast<uint64_t>(std::numeric_limits<int32_t>::max()) + 1ULL);
    auto result3 = safe_cast<signedness::unsigned_type, int32_t>(too_large);
    check(result3.status == conversion_result::overflow, "safe_cast uint128->int32 (overflow)");
}

// =============================================================================
// SECTION: safe_cast FROM int128_t
// =============================================================================

void test_safe_cast_int128_to_int64()
{
    int128_t positive(12345LL);
    auto result = safe_cast<signedness::signed_type, int64_t>(positive);
    check(result.is_valid() && result.value == 12345LL, "safe_cast int128->int64 (positive)");

    int128_t negative(-9999LL);
    auto result2 = safe_cast<signedness::signed_type, int64_t>(negative);
    check(result2.is_valid() && result2.value == -9999LL, "safe_cast int128->int64 (negative)");
}

void test_safe_cast_int128_to_uint64()
{
    int128_t positive(12345LL);
    auto result = safe_cast<signedness::signed_type, uint64_t>(positive);
    check(result.is_valid() && result.value == 12345ULL, "safe_cast int128->uint64 (positive)");

    int128_t negative(-100LL);
    auto result2 = safe_cast<signedness::signed_type, uint64_t>(negative);
    check(result2.status == conversion_result::underflow, "safe_cast int128->uint64 (underflow)");
}

// =============================================================================
// SECTION: try_cast (optional-based)
// =============================================================================

void test_try_cast()
{
    uint128_t small(42ULL);
    auto opt = try_cast<signedness::unsigned_type, int32_t>(small);
    check(opt.has_value() && *opt == 42, "try_cast success -> has_value");

    uint128_t large(1ULL, 0ULL);
    auto opt2 = try_cast<signedness::unsigned_type, int32_t>(large);
    check(!opt2.has_value(), "try_cast failure -> nullopt");
}

// =============================================================================
// SECTION: checked_cast (exception-based)
// =============================================================================

void test_checked_cast()
{
    uint128_t small(100ULL);
    bool ok = false;
    try
    {
        auto val = checked_cast<signedness::unsigned_type, int32_t>(small);
        ok = (val == 100);
    }
    catch (...)
    {
        ok = false;
    }
    check(ok, "checked_cast success");

    uint128_t large(1ULL, 0ULL);
    bool threw = false;
    try
    {
        [[maybe_unused]] auto val = checked_cast<signedness::unsigned_type, int32_t>(large);
    }
    catch (const std::overflow_error &)
    {
        threw = true;
    }
    check(threw, "checked_cast throws overflow_error");
}

// =============================================================================
// SECTION: safe_add
// =============================================================================

void test_safe_add_unsigned()
{
    uint128_t a(100ULL);
    uint128_t b(200ULL);
    auto result = safe_add(a, b);
    check(result.is_valid() && result.value == uint128_t(300ULL), "safe_add unsigned (normal)");

    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    auto overflow = safe_add(max_val, uint128_t(1ULL));
    check(overflow.status == conversion_result::overflow, "safe_add unsigned (overflow)");
}

void test_safe_add_signed()
{
    int128_t a(100LL);
    int128_t b(200LL);
    auto result = safe_add(a, b);
    check(result.is_valid() && result.value == int128_t(300LL), "safe_add signed (positive)");

    int128_t neg_a(-100LL);
    int128_t neg_b(-50LL);
    auto result2 = safe_add(neg_a, neg_b);
    check(result2.is_valid() && result2.value == int128_t(-150LL), "safe_add signed (negative)");

    int128_t max_val = nstd::numeric_limits<int128_t>::max();
    auto overflow = safe_add(max_val, int128_t(1LL));
    check(overflow.status == conversion_result::overflow, "safe_add signed (overflow)");

    int128_t mixed_a(100LL);
    int128_t mixed_b(-50LL);
    auto mixed = safe_add(mixed_a, mixed_b);
    check(mixed.is_valid() && mixed.value == int128_t(50LL), "safe_add signed (mixed)");
}

// =============================================================================
// SECTION: safe_sub
// =============================================================================

void test_safe_sub_unsigned()
{
    uint128_t a(500ULL);
    uint128_t b(200ULL);
    auto result = safe_sub(a, b);
    check(result.is_valid() && result.value == uint128_t(300ULL), "safe_sub unsigned (normal)");

    auto underflow = safe_sub(b, a); // 200 - 500
    check(underflow.status == conversion_result::underflow, "safe_sub unsigned (underflow)");
}

void test_safe_sub_signed()
{
    int128_t a(-100LL);
    int128_t b(50LL);
    auto result = safe_sub(a, b);
    check(result.is_valid() && result.value == int128_t(-150LL), "safe_sub signed (normal)");

    int128_t min_val = nstd::numeric_limits<int128_t>::min();
    auto underflow = safe_sub(min_val, int128_t(1LL));
    check(underflow.status == conversion_result::underflow, "safe_sub signed (underflow)");
}

// =============================================================================
// SECTION: safe_mul
// =============================================================================

void test_safe_mul_unsigned()
{
    uint128_t a(1000ULL);
    uint128_t b(2000ULL);
    auto result = safe_mul(a, b);
    check(result.is_valid() && result.value == uint128_t(2000000ULL), "safe_mul unsigned (normal)");

    uint128_t large(1ULL, 0ULL);            // 2^64
    auto overflow = safe_mul(large, large); // 2^128 overflow
    check(overflow.status == conversion_result::overflow, "safe_mul unsigned (overflow)");

    auto zero_result = safe_mul(a, uint128_t(0ULL));
    check(zero_result.is_valid() && zero_result.value == uint128_t(0ULL), "safe_mul unsigned (by zero)");
}

void test_safe_mul_signed()
{
    int128_t a(-7LL);
    int128_t b(6LL);
    auto result = safe_mul(a, b);
    check(result.is_valid() && result.value == int128_t(-42LL), "safe_mul signed (neg*pos)");

    int128_t c(-5LL);
    int128_t d(-8LL);
    auto result2 = safe_mul(c, d);
    check(result2.is_valid() && result2.value == int128_t(40LL), "safe_mul signed (neg*neg)");
}

// =============================================================================
// SECTION: safe_div / safe_mod
// =============================================================================

void test_safe_div()
{
    uint128_t a(1000ULL);
    uint128_t b(10ULL);
    auto result = safe_div(a, b);
    check(result.is_valid() && result.value == uint128_t(100ULL), "safe_div unsigned (normal)");

    auto div_zero = safe_div(a, uint128_t(0ULL));
    check(div_zero.status == conversion_result::invalid_input, "safe_div (by zero)");

    int128_t neg_a(-100LL);
    int128_t pos_b(10LL);
    auto signed_result = safe_div(neg_a, pos_b);
    check(signed_result.is_valid() && signed_result.value == int128_t(-10LL), "safe_div signed (neg/pos)");
}

void test_safe_mod()
{
    uint128_t a(17ULL);
    uint128_t b(5ULL);
    auto result = safe_mod(a, b);
    check(result.is_valid() && result.value == uint128_t(2ULL), "safe_mod (normal)");

    auto mod_zero = safe_mod(a, uint128_t(0ULL));
    check(mod_zero.status == conversion_result::invalid_input, "safe_mod (by zero)");
}

// =============================================================================
// SECTION: safe_shl / safe_shr
// =============================================================================

void test_safe_shl()
{
    uint128_t a(1ULL);
    auto result = safe_shl(a, 10);
    check(result.is_valid() && result.value == uint128_t(1024ULL), "safe_shl (normal)");

    uint128_t big(1ULL << 60);
    auto overflow = safe_shl(big, 70);
    check(!overflow.is_valid(), "safe_shl (overflow)");
}

void test_safe_shr()
{
    uint128_t a(1024ULL);
    auto result = safe_shr(a, 5);
    check(result.is_valid() && result.value == uint128_t(32ULL), "safe_shr (normal)");

    auto invalid = safe_shr(a, -1);
    check(!invalid.is_valid(), "safe_shr (negative shift)");
}

// =============================================================================
// SECTION: Saturating arithmetic
// =============================================================================

void test_saturating_add()
{
    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    auto sat_uint = saturating_add(max_val, uint128_t(100ULL));
    check(sat_uint == max_val, "saturating_add unsigned (saturates to max)");

    int128_t int_max = nstd::numeric_limits<int128_t>::max();
    auto sat_int = saturating_add(int_max, int128_t(1LL));
    check(sat_int == int_max, "saturating_add signed (saturates to max)");

    uint128_t a(100ULL);
    uint128_t b(200ULL);
    auto normal = saturating_add(a, b);
    check(normal == uint128_t(300ULL), "saturating_add (normal)");
}

void test_saturating_sub()
{
    uint128_t a(50ULL);
    uint128_t b(100ULL);
    auto sat_uint = saturating_sub(a, b);
    check(sat_uint == uint128_t(0ULL), "saturating_sub unsigned (saturates to 0)");

    int128_t int_min = nstd::numeric_limits<int128_t>::min();
    auto sat_int = saturating_sub(int_min, int128_t(1LL));
    check(sat_int == int_min, "saturating_sub signed (saturates to min)");

    uint128_t c(500ULL);
    uint128_t d(200ULL);
    auto normal = saturating_sub(c, d);
    check(normal == uint128_t(300ULL), "saturating_sub (normal)");
}

void test_saturating_mul()
{
    uint128_t big(1ULL, 0ULL); // 2^64
    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    auto sat_result = saturating_mul(big, big);
    check(sat_result == max_val, "saturating_mul (saturates to max)");

    uint128_t a(100ULL);
    uint128_t b(10ULL);
    auto normal = saturating_mul(a, b);
    check(normal == uint128_t(1000ULL), "saturating_mul (normal)");
}

// =============================================================================
// SECTION: safe_abs
// =============================================================================

void test_safe_abs()
{
    int128_t positive(12345LL);
    auto pos_result = safe_abs<signedness::signed_type>(positive);
    check(pos_result.is_valid() && pos_result.value == int128_t(12345LL), "safe_abs (positive)");

    int128_t negative(-12345LL);
    auto neg_result = safe_abs<signedness::signed_type>(negative);
    check(neg_result.is_valid() && neg_result.value == int128_t(12345LL), "safe_abs (negative)");

    int128_t min_val = nstd::numeric_limits<int128_t>::min();
    auto overflow = safe_abs<signedness::signed_type>(min_val);
    check(overflow.status == conversion_result::overflow, "safe_abs (min overflow)");

    auto sat = saturating_abs<signedness::signed_type>(min_val);
    check(sat == nstd::numeric_limits<int128_t>::max(), "saturating_abs (min -> max)");
}

// =============================================================================
// SECTION: Range utilities
// =============================================================================

void test_in_range()
{
    uint128_t val(50ULL);
    check(in_range(val, uint128_t(10ULL), uint128_t(100ULL)), "in_range (true)");
    check(!in_range(val, uint128_t(60ULL), uint128_t(100ULL)), "in_range (false - below)");
    check(!in_range(val, uint128_t(0ULL), uint128_t(40ULL)), "in_range (false - above)");
}

void test_clamp()
{
    auto clamped_high = clamp(uint128_t(150ULL), uint128_t(0ULL), uint128_t(100ULL));
    check(clamped_high == uint128_t(100ULL), "clamp (to max)");

    auto clamped_low = clamp(int128_t(-200LL), int128_t(-100LL), int128_t(100LL));
    check(clamped_low == int128_t(-100LL), "clamp signed (to min)");

    auto in_range_val = clamp(uint128_t(50ULL), uint128_t(0ULL), uint128_t(100ULL));
    check(in_range_val == uint128_t(50ULL), "clamp (in range)");
}

// =============================================================================
// SECTION: value_or helper
// =============================================================================

void test_value_or()
{
    uint128_t big(1ULL, 0ULL);
    auto result = safe_cast<signedness::unsigned_type, uint32_t>(big);
    check(result.value_or(42) == 42, "value_or (fallback)");

    uint128_t small(100ULL);
    auto ok_result = safe_cast<signedness::unsigned_type, uint32_t>(small);
    check(ok_result.value_or(42) == 100, "value_or (original)");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_safe.hpp tests ===\n\n";

    std::cout << "--- safe_cast FROM uint128_t ---\n";
    test_safe_cast_uint128_to_uint64();
    test_safe_cast_uint128_to_int64();
    test_safe_cast_uint128_to_uint32();
    test_safe_cast_uint128_to_int32();

    std::cout << "\n--- safe_cast FROM int128_t ---\n";
    test_safe_cast_int128_to_int64();
    test_safe_cast_int128_to_uint64();

    std::cout << "\n--- try_cast / checked_cast ---\n";
    test_try_cast();
    test_checked_cast();

    std::cout << "\n--- safe_add ---\n";
    test_safe_add_unsigned();
    test_safe_add_signed();

    std::cout << "\n--- safe_sub ---\n";
    test_safe_sub_unsigned();
    test_safe_sub_signed();

    std::cout << "\n--- safe_mul ---\n";
    test_safe_mul_unsigned();
    test_safe_mul_signed();

    std::cout << "\n--- safe_div / safe_mod ---\n";
    test_safe_div();
    test_safe_mod();

    std::cout << "\n--- safe_shl / safe_shr ---\n";
    test_safe_shl();
    test_safe_shr();

    std::cout << "\n--- saturating arithmetic ---\n";
    test_saturating_add();
    test_saturating_sub();
    test_saturating_mul();

    std::cout << "\n--- safe_abs ---\n";
    test_safe_abs();

    std::cout << "\n--- range utilities ---\n";
    test_in_range();
    test_clamp();

    std::cout << "\n--- value_or ---\n";
    test_value_or();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
