#include "uint128/uint128_safe.hpp"
#include <cassert>
#include <iostream>
#include <limits>

// =============================================================================
// Tests para safe conversions
// =============================================================================

void test_safe_cast_to_uint64()
{
    uint128_t small(0, 100);
    auto result = uint128_safe::safe_cast<uint64_t>(small);
    assert(result.is_valid() && result.value == 100);

    uint128_t large(1, 0); // high=1, overflow para uint64
    result = uint128_safe::safe_cast<uint64_t>(large);
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::overflow);

    std::cout << "test_safe_cast_to_uint64: passed" << std::endl;
}

void test_safe_cast_to_int32()
{
    uint128_t valid(0, 1000);
    auto result = uint128_safe::safe_cast<int32_t>(valid);
    assert(result.is_valid() && result.value == 1000);

    uint128_t too_large(0, 0x80000000ULL); // > INT32_MAX
    result = uint128_safe::safe_cast<int32_t>(too_large);
    assert(!result.is_valid());

    std::cout << "test_safe_cast_to_int32: passed" << std::endl;
}

void test_safe_cast_float()
{
    uint128_t value(0, 12345);
    auto result = uint128_safe::safe_cast_float<double>(value);
    assert(result.is_valid() && result.value == 12345.0);

    uint128_t large(1, 0);
    result = uint128_safe::safe_cast_float<double>(large);
    assert(result.is_valid()); // double puede representar valores grandes

    std::cout << "test_safe_cast_float: passed" << std::endl;
}

void test_safe_make_uint128()
{
    auto result = uint128_safe::safe_make_uint128(42);
    assert(result.is_valid() && result.value == uint128_t(0, 42));

    auto result_neg = uint128_safe::safe_make_uint128(-1);
    assert(!result_neg.is_valid() &&
           result_neg.status == uint128_safe::conversion_result::underflow);

    std::cout << "test_safe_make_uint128: passed" << std::endl;
}

void test_safe_add()
{
    uint128_t a(0, 100);
    uint128_t b(0, 200);
    auto result = uint128_safe::safe_add(a, b);
    assert(result.is_valid() && result.value == uint128_t(0, 300));

    uint128_t max(~0ULL, ~0ULL);
    uint128_t one(0, 1);
    result = uint128_safe::safe_add(max, one);
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::overflow);

    std::cout << "test_safe_add: passed" << std::endl;
}

void test_safe_sub()
{
    uint128_t a(0, 300);
    uint128_t b(0, 100);
    auto result = uint128_safe::safe_sub(a, b);
    assert(result.is_valid() && result.value == uint128_t(0, 200));

    uint128_t small(0, 5);
    uint128_t large(0, 10);
    result = uint128_safe::safe_sub(small, large);
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::underflow);

    std::cout << "test_safe_sub: passed" << std::endl;
}

void test_safe_mul()
{
    uint128_t a(0, 100);
    uint128_t b(0, 200);
    auto result = uint128_safe::safe_mul(a, b);
    assert(result.is_valid() && result.value == uint128_t(0, 20000));

    uint128_t large1(0x8000000000000000ULL, 0);
    uint128_t large2(0, 3);
    result = uint128_safe::safe_mul(large1, large2);
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::overflow);

    std::cout << "test_safe_mul: passed" << std::endl;
}

void test_safe_div()
{
    uint128_t a(0, 100);
    uint128_t b(0, 10);
    auto result = uint128_safe::safe_div(a, b);
    assert(result.is_valid() && result.value == uint128_t(0, 10));

    uint128_t zero(0, 0);
    result = uint128_safe::safe_div(a, zero);
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::invalid_input);

    std::cout << "test_safe_div: passed" << std::endl;
}

void test_safe_shl()
{
    uint128_t value(0, 1);
    auto result = uint128_safe::safe_shl(value, 10);
    assert(result.is_valid() && result.value == uint128_t(0, 1024));

    result = uint128_safe::safe_shl(value, 130); // shift > 128
    assert(!result.is_valid() && result.status == uint128_safe::conversion_result::invalid_input);

    uint128_t large(1, 0);
    result = uint128_safe::safe_shl(large, 64); // overflow
    assert(!result.is_valid());

    std::cout << "test_safe_shl: passed" << std::endl;
}

void test_value_or()
{
    uint128_t a(0, 100);
    uint128_t b(0, 200);
    auto result = uint128_safe::safe_add(a, b);
    assert(result.value_or(uint128_t(0, 999)) == uint128_t(0, 300));

    uint128_t max(~0ULL, ~0ULL);
    auto result_overflow = uint128_safe::safe_add(max, uint128_t(0, 1));
    assert(result_overflow.value_or(uint128_t(0, 999)) == uint128_t(0, 999));

    std::cout << "test_value_or: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== uint128_t safe operations tests ===" << std::endl;

    test_safe_cast_to_uint64();
    test_safe_cast_to_int32();
    test_safe_cast_float();
    test_safe_make_uint128();
    test_safe_add();
    test_safe_sub();
    test_safe_mul();
    test_safe_div();
    test_safe_shl();
    test_value_or();

    std::cout << "\n✓ All tests passed!" << std::endl;
    return 0;
}
