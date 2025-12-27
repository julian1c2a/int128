/*
 * Test file for int128_t (signed 128-bit integer)
 * Adapted from uint128_extracted_tests.cpp with signed-specific tests
 */

#include "../include/int128/int128_t.hpp"
#include "test_iostream_helpers.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <type_traits>
#include <vector>

// Helper for random generation
std::mt19937_64 rng(std::random_device{}());

// ================== BASIC STRUCTURE TESTS ==================

void test_int128_sizeof_is_16_bytes()
{
    assert(sizeof(int128_t) == 16);
    std::cout << "test_int128_sizeof_is_16_bytes passed" << std::endl;
}

void test_int128_default_constructor()
{
    [[maybe_unused]] int128_t val;
    assert(val.low() == 0 && val.high() == 0);
    std::cout << "test_int128_default_constructor passed" << std::endl;
}

void test_int128_positive_integral_constructor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t rand_val = rng();

        // Unsigned types - should be positive
        int128_t val_u8(static_cast<uint8_t>(rand_val));
        assert(val_u8.low() == static_cast<uint8_t>(rand_val));
        assert(val_u8.high() == 0);

        int128_t val_u64(static_cast<uint64_t>(rand_val));
        assert(val_u64.low() == rand_val);
        assert(val_u64.high() == 0);

        // Positive signed types - high should be 0
        int8_t v_i8_pos = static_cast<int8_t>(rand_val & 0x7F);
        int128_t val_i8_pos(v_i8_pos);
        assert(val_i8_pos.low() == static_cast<uint64_t>(v_i8_pos));
        assert(val_i8_pos.high() == 0);

        int64_t v_i64_pos = static_cast<int64_t>(rand_val & 0x7FFFFFFFFFFFFFFFULL);
        int128_t val_i64_pos(v_i64_pos);
        assert(val_i64_pos.low() == static_cast<uint64_t>(v_i64_pos));
        assert(val_i64_pos.high() == 0);
    }
    std::cout << "test_int128_positive_integral_constructor passed" << std::endl;
}

void test_int128_negative_integral_constructor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t rand_val = rng();

        // Negative signed types - high should be ~0 (sign extension)
        int8_t v_i8_neg = static_cast<int8_t>(rand_val | 0x80);
        if (v_i8_neg >= 0)
            v_i8_neg = -1;
        int128_t val_i8_neg(v_i8_neg);
        assert(static_cast<int8_t>(val_i8_neg.low()) == v_i8_neg);
        assert(val_i8_neg.high() == UINT64_MAX); // Sign extension

        int16_t v_i16_neg = static_cast<int16_t>(rand_val | 0x8000);
        if (v_i16_neg >= 0)
            v_i16_neg = -1;
        int128_t val_i16_neg(v_i16_neg);
        assert(static_cast<int16_t>(val_i16_neg.low()) == v_i16_neg);
        assert(val_i16_neg.high() == UINT64_MAX);

        int32_t v_i32_neg = static_cast<int32_t>(rand_val | 0x80000000);
        if (v_i32_neg >= 0)
            v_i32_neg = -1;
        int128_t val_i32_neg(v_i32_neg);
        assert(static_cast<int32_t>(val_i32_neg.low()) == v_i32_neg);
        assert(val_i32_neg.high() == UINT64_MAX);

        int64_t v_i64_neg = static_cast<int64_t>(rand_val | 0x8000000000000000ULL);
        if (v_i64_neg >= 0)
            v_i64_neg = -1;
        int128_t val_i64_neg(v_i64_neg);
        assert(static_cast<int64_t>(val_i64_neg.low()) == v_i64_neg);
        assert(val_i64_neg.high() == UINT64_MAX);
    }
    std::cout << "test_int128_negative_integral_constructor passed" << std::endl;
}

void test_int128_two_part_constructor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();
        int128_t val(h, l);
        assert(val.high() == h);
        assert(val.low() == l);
    }
    std::cout << "test_int128_two_part_constructor passed" << std::endl;
}

// ================== SIGN DETECTION TESTS ==================

void test_int128_is_negative()
{
    // Positive values
    [[maybe_unused]] int128_t pos_small(0, 100);
    assert(!pos_small.is_negative());

    [[maybe_unused]] int128_t pos_large(0x7FFFFFFFFFFFFFFFULL, UINT64_MAX);
    assert(!pos_large.is_negative());

    // Negative values (MSB of high part set)
    [[maybe_unused]] int128_t neg_small(UINT64_MAX, static_cast<uint64_t>(-100));
    assert(neg_small.is_negative());

    [[maybe_unused]] int128_t neg_large(0x8000000000000000ULL, 0);
    assert(neg_large.is_negative());

    std::cout << "test_int128_is_negative passed" << std::endl;
}

// ================== ARITHMETIC TESTS ==================

void test_int128_addition_positive()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = static_cast<int64_t>(rng() & 0x7FFFFFFF);
        int64_t b = static_cast<int64_t>(rng() & 0x7FFFFFFF);

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va + vb;

        [[maybe_unused]] int64_t expected = a + b;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_addition_positive passed" << std::endl;
}

void test_int128_addition_negative()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = -static_cast<int64_t>(rng() & 0x7FFFFFFF);
        int64_t b = -static_cast<int64_t>(rng() & 0x7FFFFFFF);

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va + vb;

        [[maybe_unused]] int64_t expected = a + b;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_addition_negative passed" << std::endl;
}

void test_int128_subtraction()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = static_cast<int64_t>(rng());
        int64_t b = static_cast<int64_t>(rng());

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va - vb;

        [[maybe_unused]] int64_t expected = a - b;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_subtraction passed" << std::endl;
}

void test_int128_multiplication()
{
    for (int i = 0; i < 1000; ++i) {
        int32_t a = static_cast<int32_t>(rng());
        int32_t b = static_cast<int32_t>(rng());

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va * vb;

        [[maybe_unused]] int64_t expected = static_cast<int64_t>(a) * static_cast<int64_t>(b);
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_multiplication passed" << std::endl;
}

void test_int128_division()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = static_cast<int64_t>(rng());
        int64_t b = static_cast<int64_t>(rng() | 1); // Ensure b != 0

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va / vb;

        [[maybe_unused]] int64_t expected = a / b;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_division passed" << std::endl;
}

void test_int128_modulo()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = static_cast<int64_t>(rng());
        int64_t b = static_cast<int64_t>(rng() | 1); // Ensure b != 0

        int128_t va(a), vb(b);
        [[maybe_unused]] int128_t result = va % vb;

        [[maybe_unused]] int64_t expected = a % b;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_modulo passed" << std::endl;
}

void test_int128_negation()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t a = static_cast<int64_t>(rng());
        int128_t va(a);
        [[maybe_unused]] int128_t result = -va;

        [[maybe_unused]] int64_t expected = -a;
        assert(static_cast<int64_t>(result.low()) == expected);
    }
    std::cout << "test_int128_negation passed" << std::endl;
}

// ================== COMPARISON TESTS ==================

void test_int128_equality()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t val = static_cast<int64_t>(rng());
        int128_t a(val), b(val);
        assert(a == b);

        [[maybe_unused]] int128_t c(val + 1);
        assert(!(a == c));
    }
    std::cout << "test_int128_equality passed" << std::endl;
}

void test_int128_less_than()
{
    // Positive vs positive
    [[maybe_unused]] int128_t a(10), b(20);
    assert(a < b);
    assert(!(b < a));

    // Negative vs negative
    [[maybe_unused]] int128_t c(-20), d(-10);
    assert(c < d);
    assert(!(d < c));

    // Negative vs positive
    [[maybe_unused]] int128_t e(-10), f(10);
    assert(e < f);
    assert(!(f < e));

    std::cout << "test_int128_less_than passed" << std::endl;
}

void test_int128_greater_than()
{
    [[maybe_unused]] int128_t a(20), b(10);
    assert(a > b);

    [[maybe_unused]] int128_t c(-10), d(-20);
    assert(c > d);

    [[maybe_unused]] int128_t e(10), f(-10);
    assert(e > f);

    std::cout << "test_int128_greater_than passed" << std::endl;
}

// ================== BITWISE TESTS ==================

void test_int128_bitwise_and()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t a = rng(), b = rng();
        int128_t va(0, a), vb(0, b);
        [[maybe_unused]] int128_t result = va & vb;
        assert(result.low() == (a & b));
    }
    std::cout << "test_int128_bitwise_and passed" << std::endl;
}

void test_int128_bitwise_or()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t a = rng(), b = rng();
        int128_t va(0, a), vb(0, b);
        [[maybe_unused]] int128_t result = va | vb;
        assert(result.low() == (a | b));
    }
    std::cout << "test_int128_bitwise_or passed" << std::endl;
}

void test_int128_bitwise_xor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t a = rng(), b = rng();
        int128_t va(0, a), vb(0, b);
        [[maybe_unused]] int128_t result = va ^ vb;
        assert(result.low() == (a ^ b));
    }
    std::cout << "test_int128_bitwise_xor passed" << std::endl;
}

void test_int128_bitwise_not()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng(), l = rng();
        int128_t val(h, l);
        [[maybe_unused]] int128_t result = ~val;
        assert(result.high() == ~h);
        assert(result.low() == ~l);
    }
    std::cout << "test_int128_bitwise_not passed" << std::endl;
}

// ================== SHIFT TESTS ==================

void test_int128_left_shift()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t val = rng() & 0xFFFFFFFF; // Keep small
        int shift = static_cast<int>(rng() % 64);

        [[maybe_unused]] int128_t v(0, val);
        [[maybe_unused]] int128_t result = v << shift;

        assert(result.low() == (val << shift));
    }
    std::cout << "test_int128_left_shift passed" << std::endl;
}

void test_int128_right_shift()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t val = rng();
        int shift = static_cast<int>(rng() % 64);

        [[maybe_unused]] int128_t v(0, val);
        [[maybe_unused]] int128_t result = v >> shift;

        // Arithmetic right shift for signed
        assert(result.low() == (val >> shift));
    }
    std::cout << "test_int128_right_shift passed" << std::endl;
}

// ================== STRING CONVERSION TESTS ==================

void test_int128_to_string_positive()
{
    int128_t val(123);
    std::string str = val.to_string();
    assert(str == "123");
    std::cout << "test_int128_to_string_positive passed" << std::endl;
}

void test_int128_to_string_negative()
{
    int128_t val(-123);
    std::string str = val.to_string();
    assert(str == "-123");
    std::cout << "test_int128_to_string_negative passed" << std::endl;
}

void test_int128_to_string_zero()
{
    int128_t val(0);
    std::string str = val.to_string();
    assert(str == "0");
    std::cout << "test_int128_to_string_zero passed" << std::endl;
}

void test_int128_from_string_positive()
{
    [[maybe_unused]] int128_t val = int128_t::from_string("12345");
    assert(val == int128_t(12345));
    std::cout << "test_int128_from_string_positive passed" << std::endl;
}

void test_int128_from_string_negative()
{
    [[maybe_unused]] int128_t val = int128_t::from_string("-12345");
    assert(val == int128_t(-12345));
    std::cout << "test_int128_from_string_negative passed" << std::endl;
}

// ================== LIMITS TESTS ==================

void test_int128_min_max_values()
{
    // INT128_MIN = -2^127 (MSB set, all else 0)
    [[maybe_unused]] int128_t min_val(0x8000000000000000ULL, 0);
    assert(min_val.is_negative());

    // INT128_MAX = 2^127 - 1 (MSB clear, all else 1)
    [[maybe_unused]] int128_t max_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    assert(!max_val.is_negative());

    std::cout << "test_int128_min_max_values passed" << std::endl;
}

// ================== INCREMENT/DECREMENT TESTS ==================

void test_int128_pre_increment()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t val = static_cast<int64_t>(rng());
        [[maybe_unused]] int128_t v(val);
        [[maybe_unused]] int128_t& result = ++v;
        assert(&result == &v); // Returns reference
        assert(static_cast<int64_t>(v.low()) == val + 1);
    }
    std::cout << "test_int128_pre_increment passed" << std::endl;
}

void test_int128_post_increment()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t val = static_cast<int64_t>(rng());
        [[maybe_unused]] int128_t v(val);
        [[maybe_unused]] int128_t old = v++;
        assert(static_cast<int64_t>(old.low()) == val);
        assert(static_cast<int64_t>(v.low()) == val + 1);
    }
    std::cout << "test_int128_post_increment passed" << std::endl;
}

void test_int128_pre_decrement()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t val = static_cast<int64_t>(rng());
        [[maybe_unused]] int128_t v(val);
        [[maybe_unused]] int128_t& result = --v;
        assert(&result == &v);
        assert(static_cast<int64_t>(v.low()) == val - 1);
    }
    std::cout << "test_int128_pre_decrement passed" << std::endl;
}

void test_int128_post_decrement()
{
    for (int i = 0; i < 1000; ++i) {
        int64_t val = static_cast<int64_t>(rng());
        [[maybe_unused]] int128_t v(val);
        [[maybe_unused]] int128_t old = v--;
        assert(static_cast<int64_t>(old.low()) == val);
        assert(static_cast<int64_t>(v.low()) == val - 1);
    }
    std::cout << "test_int128_post_decrement passed" << std::endl;
}

// ================== STREAM TESTS ==================

void test_int128_stream_output()
{
    std::ostringstream oss;
    int128_t val(42);
    oss << val;
    assert(oss.str() == "42");

    std::ostringstream oss2;
    [[maybe_unused]] int128_t neg_val(-42);
    oss2 << neg_val;
    assert(oss2.str() == "-42");

    std::cout << "test_int128_stream_output passed" << std::endl;
}

// ================== MAIN FUNCTION ==================

int main()
{
    std::cout << "=== int128_t Tests ===" << std::endl << std::endl;

    // Basic structure
    test_int128_sizeof_is_16_bytes();
    test_int128_default_constructor();
    test_int128_positive_integral_constructor();
    test_int128_negative_integral_constructor();
    test_int128_two_part_constructor();

    // Sign detection
    test_int128_is_negative();

    // Arithmetic
    test_int128_addition_positive();
    test_int128_addition_negative();
    test_int128_subtraction();
    test_int128_multiplication();
    test_int128_division();
    test_int128_modulo();
    test_int128_negation();

    // Comparisons
    test_int128_equality();
    test_int128_less_than();
    test_int128_greater_than();

    // Bitwise
    test_int128_bitwise_and();
    test_int128_bitwise_or();
    test_int128_bitwise_xor();
    test_int128_bitwise_not();

    // Shifts
    test_int128_left_shift();
    test_int128_right_shift();

    // String conversions
    test_int128_to_string_positive();
    test_int128_to_string_negative();
    test_int128_to_string_zero();
    test_int128_from_string_positive();
    test_int128_from_string_negative();

    // Limits
    test_int128_min_max_values();

    // Increment/Decrement
    test_int128_pre_increment();
    test_int128_post_increment();
    test_int128_pre_decrement();
    test_int128_post_decrement();

    // Streams
    test_int128_stream_output();

    std::cout << std::endl << "=== All int128_t tests passed! ===" << std::endl;
    return 0;
}
