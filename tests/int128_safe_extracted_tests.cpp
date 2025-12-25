#include "int128/int128_safe.hpp"
#include <cassert>
#include <iostream>
#include <limits>

// =============================================================================
// Tests para safe conversions
// =============================================================================

void test_safe_cast_to_int64()
{
    int128_t small(42);
    auto result = int128_safe::safe_cast<int64_t>(small);
    assert(result.is_valid() && result.value == 42);

    int128_t negative(-100);
    result = int128_safe::safe_cast<int64_t>(negative);
    assert(result.is_valid() && result.value == -100);

    int128_t large(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    result = int128_safe::safe_cast<int64_t>(large);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::overflow);

    std::cout << "test_safe_cast_to_int64: passed" << std::endl;
}

void test_safe_cast_to_uint32()
{
    int128_t valid(1000);
    auto result = int128_safe::safe_cast<uint32_t>(valid);
    assert(result.is_valid() && result.value == 1000);

    int128_t negative(-50);
    result = int128_safe::safe_cast<uint32_t>(negative);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::underflow);

    int128_t too_large(0, 0x100000000ULL); // > UINT32_MAX
    result = int128_safe::safe_cast<uint32_t>(too_large);
    assert(!result.is_valid());

    std::cout << "test_safe_cast_to_uint32: passed" << std::endl;
}

void test_safe_cast_float()
{
    int128_t positive(12345);
    auto result = int128_safe::safe_cast_float<double>(positive);
    assert(result.is_valid() && result.value == 12345.0);

    int128_t negative(-67890);
    result = int128_safe::safe_cast_float<double>(negative);
    assert(result.is_valid() && result.value == -67890.0);

    std::cout << "test_safe_cast_float: passed" << std::endl;
}

void test_safe_make_int128()
{
    auto result = int128_safe::safe_make_int128(42);
    assert(result.is_valid() && result.value == int128_t(42));

    auto result_neg = int128_safe::safe_make_int128(-100);
    assert(result_neg.is_valid() && result_neg.value == int128_t(-100));

    std::cout << "test_safe_make_int128: passed" << std::endl;
}

void test_safe_add()
{
    int128_t a(100);
    int128_t b(200);
    auto result = int128_safe::safe_add(a, b);
    assert(result.is_valid() && result.value == int128_t(300));

    int128_t max_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    int128_t one(1);
    result = int128_safe::safe_add(max_val, one);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::overflow);

    int128_t neg_a(-50);
    int128_t neg_b(-30);
    result = int128_safe::safe_add(neg_a, neg_b);
    assert(result.is_valid() && result.value == int128_t(-80));

    std::cout << "test_safe_add: passed" << std::endl;
}

void test_safe_sub()
{
    int128_t a(300);
    int128_t b(100);
    auto result = int128_safe::safe_sub(a, b);
    assert(result.is_valid() && result.value == int128_t(200));

    int128_t min_val(0x8000000000000000ULL, 0);
    int128_t one(1);
    result = int128_safe::safe_sub(min_val, one);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::underflow);

    std::cout << "test_safe_sub: passed" << std::endl;
}

void test_safe_mul()
{
    int128_t a(100);
    int128_t b(200);
    auto result = int128_safe::safe_mul(a, b);
    assert(result.is_valid() && result.value == int128_t(20000));

    // Usar valor cercano a max que cause overflow real
    int128_t large(0x4000000000000000ULL, 0); // 2^126
    int128_t mult(4);
    result = int128_safe::safe_mul(large, mult);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::overflow);

    int128_t neg_a(-10);
    int128_t neg_b(-20);
    result = int128_safe::safe_mul(neg_a, neg_b);
    assert(result.is_valid() && result.value == int128_t(200));

    std::cout << "test_safe_mul: passed" << std::endl;
}

void test_safe_div()
{
    int128_t a(100);
    int128_t b(10);
    auto result = int128_safe::safe_div(a, b);
    assert(result.is_valid() && result.value == int128_t(10));

    int128_t zero(0);
    result = int128_safe::safe_div(a, zero);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::invalid_input);

    int128_t neg_a(-100);
    int128_t pos_b(10);
    result = int128_safe::safe_div(neg_a, pos_b);
    assert(result.is_valid() && result.value == int128_t(-10));

    std::cout << "test_safe_div: passed" << std::endl;
}

void test_safe_abs()
{
    int128_t positive(100);
    auto result = int128_safe::safe_abs(positive);
    assert(result.is_valid() && result.value == int128_t(100));

    int128_t negative(-50);
    result = int128_safe::safe_abs(negative);
    assert(result.is_valid() && result.value == int128_t(50));

    int128_t min_val(0x8000000000000000ULL, 0);
    result = int128_safe::safe_abs(min_val);
    assert(!result.is_valid() && result.status == int128_safe::conversion_result::overflow);

    std::cout << "test_safe_abs: passed" << std::endl;
}

void test_value_or()
{
    int128_t a(100);
    int128_t b(200);
    auto result = int128_safe::safe_add(a, b);
    assert(result.value_or(int128_t(999)) == int128_t(300));

    int128_t max_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    auto result_overflow = int128_safe::safe_add(max_val, int128_t(1));
    assert(result_overflow.value_or(int128_t(999)) == int128_t(999));

    std::cout << "test_value_or: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== int128_t safe operations tests ===" << std::endl;

    test_safe_cast_to_int64();
    test_safe_cast_to_uint32();
    test_safe_cast_float();
    test_safe_make_int128();
    test_safe_add();
    test_safe_sub();
    test_safe_mul();
    test_safe_div();
    test_safe_abs();
    test_value_or();

    std::cout << "\n[OK] All tests passed!" << std::endl;
    return 0;
}
