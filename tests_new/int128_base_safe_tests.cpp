/**
 * @file int128_base_safe_tests.cpp
 * @brief Tests for int128_base_safe.hpp - Safe arithmetic operations
 */

#include "int128_base_safe.hpp"
#include <cassert>
#include <iostream>
#include <limits>

using namespace nstd;
using namespace nstd::int128_safe;

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

void test_passed(const char* name)
{
    std::cout << "[PASS] " << name << std::endl;
    tests_passed++;
}

void test_failed(const char* name, const char* reason)
{
    std::cout << "[FAIL] " << name << ": " << reason << std::endl;
    tests_failed++;
}

// =============================================================================
// TEST safe_cast TO SMALLER TYPES
// =============================================================================

void test_safe_cast_to_uint64()
{
    // Value fits
    uint128_t small(12345);
    auto result = safe_cast<signedness::unsigned_type, uint64_t>(small);
    assert(result.is_valid());
    assert(result.value == 12345);

    // Value overflows
    uint128_t big(uint64_t(1), uint64_t(0)); // 2^64
    auto overflow_result = safe_cast<signedness::unsigned_type, uint64_t>(big);
    assert(!overflow_result.is_valid());
    assert(overflow_result.status == conversion_result::overflow);

    test_passed("safe_cast to uint64_t");
}

void test_safe_cast_to_int64()
{
    // Positive value fits
    uint128_t small(1000);
    auto result = safe_cast<signedness::unsigned_type, int64_t>(small);
    assert(result.is_valid());
    assert(result.value == 1000);

    // Value exceeds INT64_MAX
    uint128_t big(uint64_t(0x8000000000000000ULL)); // > INT64_MAX
    auto overflow_result = safe_cast<signedness::unsigned_type, int64_t>(big);
    assert(!overflow_result.is_valid());
    assert(overflow_result.status == conversion_result::overflow);

    test_passed("safe_cast to int64_t");
}

void test_safe_cast_to_uint32()
{
    // Value fits
    uint128_t small(0xFFFFFFFF); // UINT32_MAX
    auto result = safe_cast<signedness::unsigned_type, uint32_t>(small);
    assert(result.is_valid());
    assert(result.value == 0xFFFFFFFF);

    // Value overflows
    uint128_t big(0x100000000ULL); // UINT32_MAX + 1
    auto overflow_result = safe_cast<signedness::unsigned_type, uint32_t>(big);
    assert(!overflow_result.is_valid());

    test_passed("safe_cast to uint32_t");
}

void test_safe_cast_int128()
{
    // Signed value fits in int64_t
    int128_t small(-12345);
    auto result = safe_cast<signedness::signed_type, int64_t>(small);
    assert(result.is_valid());
    assert(result.value == -12345);

    // Positive signed value
    int128_t positive(9999);
    auto pos_result = safe_cast<signedness::signed_type, int64_t>(positive);
    assert(pos_result.is_valid());
    assert(pos_result.value == 9999);

    test_passed("safe_cast int128_t to int64_t");
}

// =============================================================================
// TEST try_cast (optional-based)
// =============================================================================

void test_try_cast()
{
    uint128_t small(42);
    auto opt = try_cast<signedness::unsigned_type, int32_t>(small);
    assert(opt.has_value());
    assert(*opt == 42);

    uint128_t big(uint64_t(1) << 40);
    auto no_opt = try_cast<signedness::unsigned_type, int32_t>(big);
    assert(!no_opt.has_value());

    test_passed("try_cast returns optional");
}

// =============================================================================
// TEST safe_add
// =============================================================================

void test_safe_add_no_overflow()
{
    uint128_t a(100);
    uint128_t b(200);
    auto result = safe_add(a, b);
    assert(result.is_valid());
    assert(result.value == uint128_t(300));

    test_passed("safe_add no overflow");
}

void test_safe_add_overflow_unsigned()
{
    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    uint128_t one(1);
    auto result = safe_add(max_val, one);
    assert(!result.is_valid());
    assert(result.status == conversion_result::overflow);

    test_passed("safe_add overflow detection (unsigned)");
}

void test_safe_add_signed()
{
    int128_t a(100);
    int128_t b(-50);
    auto result = safe_add(a, b);
    assert(result.is_valid());
    assert(result.value == int128_t(50));

    test_passed("safe_add with signed values");
}

// =============================================================================
// TEST safe_sub
// =============================================================================

void test_safe_sub_no_underflow()
{
    uint128_t a(300);
    uint128_t b(100);
    auto result = safe_sub(a, b);
    assert(result.is_valid());
    assert(result.value == uint128_t(200));

    test_passed("safe_sub no underflow");
}

void test_safe_sub_underflow_unsigned()
{
    uint128_t a(100);
    uint128_t b(200);
    auto result = safe_sub(a, b);
    assert(!result.is_valid());
    assert(result.status == conversion_result::underflow);

    test_passed("safe_sub underflow detection (unsigned)");
}

void test_safe_sub_signed()
{
    int128_t a(-100);
    int128_t b(50);
    auto result = safe_sub(a, b);
    assert(result.is_valid());
    assert(result.value == int128_t(-150));

    test_passed("safe_sub with signed values");
}

// =============================================================================
// TEST safe_mul
// =============================================================================

void test_safe_mul_no_overflow()
{
    uint128_t a(1000);
    uint128_t b(1000);
    auto result = safe_mul(a, b);
    assert(result.is_valid());
    assert(result.value == uint128_t(1000000));

    test_passed("safe_mul no overflow");
}

void test_safe_mul_overflow()
{
    uint128_t big(uint64_t(1) << 63);
    uint128_t four(4);
    auto result = safe_mul(big, four);
    // This should overflow (2^63 * 4 = 2^65 which fits in 128 bits)
    // Let's use a bigger value
    uint128_t huge(uint64_t(1), uint64_t(0)); // 2^64
    auto overflow_result = safe_mul(huge, huge);
    assert(!overflow_result.is_valid());
    assert(overflow_result.status == conversion_result::overflow);

    test_passed("safe_mul overflow detection");
}

void test_safe_mul_by_zero()
{
    uint128_t a(12345678901234567890ULL);
    uint128_t zero(0);
    auto result = safe_mul(a, zero);
    assert(result.is_valid());
    assert(result.value == uint128_t(0));

    test_passed("safe_mul by zero");
}

// =============================================================================
// TEST safe_div
// =============================================================================

void test_safe_div_normal()
{
    uint128_t a(1000);
    uint128_t b(10);
    auto result = safe_div(a, b);
    assert(result.is_valid());
    assert(result.value == uint128_t(100));

    test_passed("safe_div normal");
}

void test_safe_div_by_zero()
{
    uint128_t a(100);
    uint128_t zero(0);
    auto result = safe_div(a, zero);
    assert(!result.is_valid());
    assert(result.status == conversion_result::invalid_input);

    test_passed("safe_div by zero detection");
}

// =============================================================================
// TEST safe_mod
// =============================================================================

void test_safe_mod_normal()
{
    uint128_t a(17);
    uint128_t b(5);
    auto result = safe_mod(a, b);
    assert(result.is_valid());
    assert(result.value == uint128_t(2));

    test_passed("safe_mod normal");
}

void test_safe_mod_by_zero()
{
    uint128_t a(100);
    uint128_t zero(0);
    auto result = safe_mod(a, zero);
    assert(!result.is_valid());
    assert(result.status == conversion_result::invalid_input);

    test_passed("safe_mod by zero detection");
}

// =============================================================================
// TEST safe_shl / safe_shr
// =============================================================================

void test_safe_shl()
{
    uint128_t a(1);
    auto result = safe_shl(a, 10);
    assert(result.is_valid());
    assert(result.value == uint128_t(1024));

    // Overflow case
    uint128_t big(uint64_t(1) << 60);
    auto overflow = safe_shl(big, 70);
    assert(!overflow.is_valid());

    test_passed("safe_shl");
}

void test_safe_shr()
{
    uint128_t a(1024);
    auto result = safe_shr(a, 5);
    assert(result.is_valid());
    assert(result.value == uint128_t(32));

    // Negative shift
    auto invalid = safe_shr(a, -1);
    assert(!invalid.is_valid());

    test_passed("safe_shr");
}

// =============================================================================
// TEST saturating operations
// =============================================================================

void test_saturating_add()
{
    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    uint128_t one(1);
    auto result = saturating_add(max_val, one);
    assert(result == max_val); // Saturates at max

    uint128_t a(100);
    uint128_t b(200);
    auto normal = saturating_add(a, b);
    assert(normal == uint128_t(300));

    test_passed("saturating_add");
}

void test_saturating_sub()
{
    uint128_t a(100);
    uint128_t b(200);
    auto result = saturating_sub(a, b);
    assert(result == uint128_t(0)); // Saturates at min (0 for unsigned)

    uint128_t c(500);
    uint128_t d(200);
    auto normal = saturating_sub(c, d);
    assert(normal == uint128_t(300));

    test_passed("saturating_sub");
}

void test_saturating_mul()
{
    uint128_t big(uint64_t(1), uint64_t(0)); // 2^64
    uint128_t max_val = nstd::numeric_limits<uint128_t>::max();
    auto result = saturating_mul(big, big);
    assert(result == max_val); // Saturates at max

    uint128_t a(100);
    uint128_t b(10);
    auto normal = saturating_mul(a, b);
    assert(normal == uint128_t(1000));

    test_passed("saturating_mul");
}

// =============================================================================
// TEST safe_abs
// =============================================================================

void test_safe_abs()
{
    int128_t positive(12345);
    auto pos_result = safe_abs(positive);
    assert(pos_result.is_valid());
    assert(pos_result.value == int128_t(12345));

    int128_t negative(-12345);
    auto neg_result = safe_abs(negative);
    assert(neg_result.is_valid());
    assert(neg_result.value == int128_t(12345));

    test_passed("safe_abs");
}

// =============================================================================
// TEST in_range and clamp
// =============================================================================

void test_in_range()
{
    uint128_t value(50);
    assert(in_range(value, uint128_t(0), uint128_t(100)) == true);
    assert(in_range(value, uint128_t(60), uint128_t(100)) == false);
    assert(in_range(value, uint128_t(0), uint128_t(40)) == false);

    test_passed("in_range");
}

void test_clamp()
{
    uint128_t value(150);
    auto clamped = clamp(value, uint128_t(0), uint128_t(100));
    assert(clamped == uint128_t(100));

    uint128_t low_value(50);
    auto in_range_result = clamp(low_value, uint128_t(0), uint128_t(100));
    assert(in_range_result == uint128_t(50));

    test_passed("clamp");
}

// =============================================================================
// TEST value_or helper
// =============================================================================

void test_value_or()
{
    uint128_t big(uint64_t(1), uint64_t(0));
    auto result = safe_cast<signedness::unsigned_type, uint32_t>(big);
    assert(!result.is_valid());

    uint32_t fallback = result.value_or(42);
    assert(fallback == 42);

    uint128_t small(100);
    auto ok_result = safe_cast<signedness::unsigned_type, uint32_t>(small);
    assert(ok_result.is_valid());
    assert(ok_result.value_or(42) == 100);

    test_passed("value_or helper");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_safe Tests ===" << std::endl;
    std::cout << std::endl;

    std::cout << "--- safe_cast ---" << std::endl;
    test_safe_cast_to_uint64();
    test_safe_cast_to_int64();
    test_safe_cast_to_uint32();
    test_safe_cast_int128();
    test_try_cast();

    std::cout << std::endl;
    std::cout << "--- safe_add ---" << std::endl;
    test_safe_add_no_overflow();
    test_safe_add_overflow_unsigned();
    test_safe_add_signed();

    std::cout << std::endl;
    std::cout << "--- safe_sub ---" << std::endl;
    test_safe_sub_no_underflow();
    test_safe_sub_underflow_unsigned();
    test_safe_sub_signed();

    std::cout << std::endl;
    std::cout << "--- safe_mul ---" << std::endl;
    test_safe_mul_no_overflow();
    test_safe_mul_overflow();
    test_safe_mul_by_zero();

    std::cout << std::endl;
    std::cout << "--- safe_div / safe_mod ---" << std::endl;
    test_safe_div_normal();
    test_safe_div_by_zero();
    test_safe_mod_normal();
    test_safe_mod_by_zero();

    std::cout << std::endl;
    std::cout << "--- safe_shl / safe_shr ---" << std::endl;
    test_safe_shl();
    test_safe_shr();

    std::cout << std::endl;
    std::cout << "--- saturating operations ---" << std::endl;
    test_saturating_add();
    test_saturating_sub();
    test_saturating_mul();

    std::cout << std::endl;
    std::cout << "--- safe_abs ---" << std::endl;
    test_safe_abs();

    std::cout << std::endl;
    std::cout << "--- in_range / clamp ---" << std::endl;
    test_in_range();
    test_clamp();

    std::cout << std::endl;
    std::cout << "--- helpers ---" << std::endl;
    test_value_or();

    std::cout << std::endl;
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "Tests passed: " << tests_passed << std::endl;
    std::cout << "Tests failed: " << tests_failed << std::endl;
    std::cout << std::endl;

    if (tests_failed == 0) {
        std::cout << "*** ALL TESTS PASSED! ***" << std::endl;
        return 0;
    } else {
        std::cout << "*** SOME TESTS FAILED ***" << std::endl;
        return 1;
    }
}
