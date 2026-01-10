// =============================================================================
// int128 Library - 128-bit Integer Types for C++20
// =============================================================================
//
// SPDX-License-Identifier: BSL-1.0
//
// Copyright (c) 2024-2026 Julian Calderon Almendros
// Email: julian.calderon.almendros@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//  https://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
// @file       int128_base_tt_extracted_tests.cpp
// @brief      Comprehensive tests for int128_base_t&lt;S&gt; unified template
// @author     Julian Calderon Almendros
// @date       2026-01-10
// @version    1.0.0
// =============================================================================
//
// Compile with:
//   g++ -std=c++20 -O2 -I../include int128_base_tt_extracted_tests.cpp -o test
//   clang++ -std=c++20 -O2 -I../include int128_base_tt_extracted_tests.cpp -o test
//
// =============================================================================

#include "../include/int128_base_tt.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

using namespace nstd;

// =============================================================================
// Test Infrastructure
// =============================================================================

static std::mt19937_64 rng(std::random_device{}());

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)                      \
  do                                               \
  {                                                \
    if (condition)                                 \
    {                                              \
      std::cout << "[OK]   " << name << std::endl; \
      tests_passed++;                              \
    }                                              \
    else                                           \
    {                                              \
      std::cout << "[FAIL] " << name << std::endl; \
      tests_failed++;                              \
    }                                              \
  } while (0)

// =============================================================================
// SECTION 1: Size, Layout and Type Traits
// =============================================================================

void test_sizeof_is_16_bytes()
{
  TEST("sizeof(uint128_t) == 16", sizeof(uint128_t) == 16);
  TEST("sizeof(int128_t) == 16", sizeof(int128_t) == 16);
}

void test_alignment()
{
  TEST("alignof(uint128_t) >= 8", alignof(uint128_t) >= 8);
  TEST("alignof(int128_t) >= 8", alignof(int128_t) >= 8);
}

void test_trivially_copyable()
{
  TEST("uint128_t is trivially copyable",
       std::is_trivially_copyable_v<uint128_t>);
  TEST("int128_t is trivially copyable",
       std::is_trivially_copyable_v<int128_t>);
}

void test_standard_layout()
{
  TEST("uint128_t is standard layout", std::is_standard_layout_v<uint128_t>);
  TEST("int128_t is standard layout", std::is_standard_layout_v<int128_t>);
}

// =============================================================================
// SECTION 2: Constructor Tests
// =============================================================================

void test_default_constructor()
{
  uint128_t u;
  int128_t i;
  TEST("uint128_t default == 0", u.high() == 0 && u.low() == 0);
  TEST("int128_t default == 0", i.high() == 0 && i.low() == 0);
}

void test_integral_constructor_basic()
{
  uint128_t u1(42);
  TEST("uint128_t(42) low", u1.low() == 42);
  TEST("uint128_t(42) high", u1.high() == 0);

  int128_t i1(42);
  TEST("int128_t(42) low", i1.low() == 42);
  TEST("int128_t(42) high", i1.high() == 0);

  int128_t i2(-1);
  TEST("int128_t(-1) low", i2.low() == ~0ULL);
  TEST("int128_t(-1) high (sign extended)", i2.high() == ~0ULL);

  int128_t i3(-42);
  TEST("int128_t(-42) is negative", i3.is_negative());
}

void test_integral_constructor_positive_random()
{
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t rand_val = rng();

    // Unsigned types
    const int128_t val_u8(static_cast<uint8_t>(rand_val));
    if (val_u8.low() != static_cast<uint8_t>(rand_val) || val_u8.high() != 0)
    {
      all_passed = false;
      break;
    }

    const int128_t val_u64(static_cast<uint64_t>(rand_val));
    if (val_u64.low() != rand_val || val_u64.high() != 0)
    {
      all_passed = false;
      break;
    }

    // Positive signed types
    const int8_t v_i8_pos = static_cast<int8_t>(rand_val & 0x7F);
    const int128_t val_i8_pos(v_i8_pos);
    if (val_i8_pos.low() != static_cast<uint64_t>(v_i8_pos) || val_i8_pos.high() != 0)
    {
      all_passed = false;
      break;
    }

    const int64_t v_i64_pos = static_cast<int64_t>(rand_val & 0x7FFFFFFFFFFFFFFFULL);
    const int128_t val_i64_pos(v_i64_pos);
    if (val_i64_pos.low() != static_cast<uint64_t>(v_i64_pos) || val_i64_pos.high() != 0)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t positive constructor (1000 random)", all_passed);
}

void test_integral_constructor_negative_random()
{
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t rand_val = rng();

    // Negative int8_t
    int8_t v_i8_neg = static_cast<int8_t>(rand_val | 0x80);
    if (v_i8_neg >= 0)
    {
      v_i8_neg = -1;
    }
    const int128_t val_i8_neg(v_i8_neg);
    if (static_cast<int8_t>(val_i8_neg.low()) != v_i8_neg ||
        val_i8_neg.high() != UINT64_MAX)
    {
      all_passed = false;
      break;
    }

    // Negative int16_t
    int16_t v_i16_neg = static_cast<int16_t>(rand_val | 0x8000);
    if (v_i16_neg >= 0)
    {
      v_i16_neg = -1;
    }
    const int128_t val_i16_neg(v_i16_neg);
    if (static_cast<int16_t>(val_i16_neg.low()) != v_i16_neg ||
        val_i16_neg.high() != UINT64_MAX)
    {
      all_passed = false;
      break;
    }

    // Negative int32_t
    int32_t v_i32_neg = static_cast<int32_t>(rand_val | 0x80000000);
    if (v_i32_neg >= 0)
    {
      v_i32_neg = -1;
    }
    const int128_t val_i32_neg(v_i32_neg);
    if (static_cast<int32_t>(val_i32_neg.low()) != v_i32_neg ||
        val_i32_neg.high() != UINT64_MAX)
    {
      all_passed = false;
      break;
    }

    // Negative int64_t
    int64_t v_i64_neg = static_cast<int64_t>(rand_val | 0x8000000000000000ULL);
    if (v_i64_neg >= 0)
    {
      v_i64_neg = -1;
    }
    const int128_t val_i64_neg(v_i64_neg);
    if (static_cast<int64_t>(val_i64_neg.low()) != v_i64_neg ||
        val_i64_neg.high() != UINT64_MAX)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t negative constructor with sign extension (1000 random)", all_passed);
}

void test_two_part_constructor()
{
  uint128_t u(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
  TEST("uint128_t(high, low) high", u.high() == 0x1234567890ABCDEFULL);
  TEST("uint128_t(high, low) low", u.low() == 0xFEDCBA0987654321ULL);

  int128_t i(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
  TEST("int128_t(high, low) high", i.high() == 0x1234567890ABCDEFULL);
  TEST("int128_t(high, low) low", i.low() == 0xFEDCBA0987654321ULL);

  // Random test
  bool all_passed = true;
  for (int j = 0; j < 1000; ++j)
  {
    const uint64_t h = rng();
    const uint64_t l = rng();
    const int128_t val(h, l);
    if (val.high() != h || val.low() != l)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t(high, low) random (1000 iterations)", all_passed);
}

void test_copy_move_constructors()
{
  const uint128_t original(0x1234, 0x5678);
  const uint128_t copy(original);
  TEST("uint128_t copy constructor", copy == original);

  uint128_t to_move(original);
  const uint128_t moved(std::move(to_move));
  TEST("uint128_t move constructor", moved == original);

  const int128_t i_original(-12345);
  const int128_t i_copy(i_original);
  TEST("int128_t copy constructor", i_copy == i_original);
}

// =============================================================================
// SECTION 3: Accessor Tests
// =============================================================================

void test_high_low_accessors()
{
  bool all_passed = true;
  for (int i = 0; i < 100; ++i)
  {
    const uint64_t h = rng();
    const uint64_t l = rng();
    const uint128_t val(h, l);
    if (val.high() != h || val.low() != l)
    {
      all_passed = false;
      break;
    }
  }
  TEST("high()/low() random (100 iterations)", all_passed);
}

void test_set_high_low()
{
  uint128_t val;
  val.set_high(0xABCD);
  val.set_low(0x1234);
  TEST("set_high()", val.high() == 0xABCD);
  TEST("set_low()", val.low() == 0x1234);
}

// =============================================================================
// SECTION 4: Sign Detection Tests
// =============================================================================

void test_is_negative()
{
  const int128_t pos(100);
  const int128_t neg(-100);
  const int128_t zero(0);

  TEST("int128_t positive is_negative()", !pos.is_negative());
  TEST("int128_t negative is_negative()", neg.is_negative());
  TEST("int128_t zero is_negative()", !zero.is_negative());

  // Boundary cases
  const int128_t pos_small(0, 100);
  TEST("int128_t(0, 100) not negative", !pos_small.is_negative());

  const int128_t pos_large(0x7FFFFFFFFFFFFFFFULL, UINT64_MAX);
  TEST("int128_t INT128_MAX not negative", !pos_large.is_negative());

  const int128_t neg_small(UINT64_MAX, static_cast<uint64_t>(-100));
  TEST("int128_t(-100) with sign extension is negative", neg_small.is_negative());

  const int128_t neg_large(0x8000000000000000ULL, 0);
  TEST("int128_t INT128_MIN is negative", neg_large.is_negative());

  // uint128_t is_negative() always false
  const uint128_t u(~0ULL, ~0ULL);
  TEST("uint128_t is_negative() always false", !u.is_negative());
}

// =============================================================================
// SECTION 5: Comparison Tests
// =============================================================================

void test_equality()
{
  const uint128_t a(100, 200);
  const uint128_t b(100, 200);
  const uint128_t c(100, 201);
  TEST("uint128_t == (equal)", a == b);
  TEST("uint128_t != (not equal)", a != c);

  const int128_t ia(-42);
  const int128_t ib(-42);
  const int128_t ic(42);
  TEST("int128_t == (equal negative)", ia == ib);
  TEST("int128_t != (neg vs pos)", ia != ic);

  // Random equality test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t val = static_cast<int64_t>(rng());
    const int128_t x(val);
    const int128_t y(val);
    if (!(x == y))
    {
      all_passed = false;
      break;
    }
    const int128_t z(val + 1);
    if (x == z)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t equality (1000 random)", all_passed);
}

void test_ordering_unsigned()
{
  const uint128_t small(0, 100);
  const uint128_t large(1, 0);
  TEST("uint128_t < (low vs high)", small < large);
  TEST("uint128_t > (high vs low)", large > small);
  TEST("uint128_t <=", small <= large);
  TEST("uint128_t >=", large >= small);
}

void test_ordering_signed()
{
  const int128_t negative(-100);
  const int128_t positive(100);
  const int128_t zero(0);

  TEST("int128_t: negative < zero", negative < zero);
  TEST("int128_t: negative < positive", negative < positive);
  TEST("int128_t: zero < positive", zero < positive);
  TEST("int128_t: positive > negative", positive > negative);

  // More detailed tests
  const int128_t a(10);
  const int128_t b(20);
  TEST("int128_t: 10 < 20", a < b);
  TEST("int128_t: !(20 < 10)", !(b < a));

  const int128_t c(-20);
  const int128_t d(-10);
  TEST("int128_t: -20 < -10", c < d);
  TEST("int128_t: !(-10 < -20)", !(d < c));

  const int128_t e(-10);
  const int128_t f(10);
  TEST("int128_t: -10 < 10", e < f);
  TEST("int128_t: !(10 < -10)", !(f < e));
}

// =============================================================================
// SECTION 6: Arithmetic Tests
// =============================================================================

void test_addition()
{
  const uint128_t a(0, 100);
  const uint128_t b(0, 50);
  TEST("uint128_t addition", (a + b).low() == 150);

  // Carry test
  const uint128_t c(0, ~0ULL);
  const uint128_t d(0, 1);
  const uint128_t sum = c + d;
  TEST("uint128_t addition with carry", sum.high() == 1 && sum.low() == 0);

  const int128_t ia(-50);
  const int128_t ib(100);
  TEST("int128_t: -50 + 100 = 50", (ia + ib) == int128_t(50));

  // Random positive addition
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = static_cast<int64_t>(rng() & 0x7FFFFFFF);
    const int64_t y = static_cast<int64_t>(rng() & 0x7FFFFFFF);
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx + vy;
    const int64_t expected = x + y;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t addition positive (1000 random)", all_passed);

  // Random negative addition
  all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = -static_cast<int64_t>(rng() & 0x7FFFFFFF);
    const int64_t y = -static_cast<int64_t>(rng() & 0x7FFFFFFF);
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx + vy;
    const int64_t expected = x + y;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t addition negative (1000 random)", all_passed);
}

void test_subtraction()
{
  const uint128_t a(0, 100);
  const uint128_t b(0, 30);
  TEST("uint128_t subtraction", (a - b).low() == 70);

  // Borrow test
  const uint128_t c(1, 0);
  const uint128_t d(0, 1);
  const uint128_t diff = c - d;
  TEST("uint128_t subtraction with borrow",
       diff.high() == 0 && diff.low() == ~0ULL);

  const int128_t ia(50);
  const int128_t ib(100);
  TEST("int128_t: 50 - 100 = -50", (ia - ib) == int128_t(-50));

  // Random subtraction
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = static_cast<int64_t>(rng());
    const int64_t y = static_cast<int64_t>(rng());
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx - vy;
    const int64_t expected = x - y;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t subtraction (1000 random)", all_passed);
}

void test_multiplication()
{
  const uint128_t a(0, 1000);
  const uint128_t b(0, 2000);
  TEST("uint128_t multiplication", (a * b).low() == 2000000);

  const int128_t ia(-10);
  const int128_t ib(20);
  TEST("int128_t: -10 * 20 = -200", (ia * ib) == int128_t(-200));

  const int128_t ic(-10);
  const int128_t id(-20);
  TEST("int128_t: -10 * -20 = 200", (ic * id) == int128_t(200));

  // Random multiplication
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int32_t x = static_cast<int32_t>(rng());
    const int32_t y = static_cast<int32_t>(rng());
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx * vy;
    const int64_t expected = static_cast<int64_t>(x) * static_cast<int64_t>(y);
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t multiplication (1000 random)", all_passed);
}

void test_division()
{
  const uint128_t a(0, 1000);
  const uint128_t b(0, 10);
  TEST("uint128_t division", (a / b).low() == 100);

  const int128_t ia(-100);
  const int128_t ib(10);
  TEST("int128_t: -100 / 10 = -10", (ia / ib) == int128_t(-10));

  const int128_t ic(-100);
  const int128_t id(-10);
  TEST("int128_t: -100 / -10 = 10", (ic / id) == int128_t(10));

  // Random division
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = static_cast<int64_t>(rng());
    const int64_t y = static_cast<int64_t>(rng() | 1); // Ensure y != 0
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx / vy;
    const int64_t expected = x / y;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t division (1000 random)", all_passed);
}

void test_modulo()
{
  const uint128_t a(0, 107);
  const uint128_t b(0, 10);
  TEST("uint128_t modulo", (a % b).low() == 7);

  const int128_t ia(-107);
  const int128_t ib(10);
  // C++ truncation toward zero: -107 % 10 = -7
  TEST("int128_t: -107 % 10 = -7", (ia % ib) == int128_t(-7));

  // Random modulo
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = static_cast<int64_t>(rng());
    const int64_t y = static_cast<int64_t>(rng() | 1); // Ensure y != 0
    const int128_t vx(x);
    const int128_t vy(y);
    const int128_t result = vx % vy;
    const int64_t expected = x % y;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t modulo (1000 random)", all_passed);
}

void test_negation()
{
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const int64_t x = static_cast<int64_t>(rng());
    const int128_t vx(x);
    const int128_t result = -vx;
    const int64_t expected = -x;
    if (static_cast<int64_t>(result.low()) != expected)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t negation (1000 random)", all_passed);
}

void test_increment_decrement()
{
  uint128_t u(0, 100);
  ++u;
  TEST("uint128_t pre-increment", u.low() == 101);

  u--;
  TEST("uint128_t post-decrement", u.low() == 100);

  int128_t i(-1);
  ++i;
  TEST("int128_t: -1 ++ = 0", i == int128_t(0));

  --i;
  TEST("int128_t: 0 -- = -1", i == int128_t(-1));

  // Pre-increment returns reference
  bool all_passed = true;
  for (int j = 0; j < 1000; ++j)
  {
    const int64_t val = static_cast<int64_t>(rng());
    int128_t v(val);
    int128_t &result = ++v;
    if (&result != &v || static_cast<int64_t>(v.low()) != val + 1)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t pre-increment returns ref (1000 random)", all_passed);

  // Post-increment returns old value
  all_passed = true;
  for (int j = 0; j < 1000; ++j)
  {
    const int64_t val = static_cast<int64_t>(rng());
    int128_t v(val);
    const int128_t old = v++;
    if (static_cast<int64_t>(old.low()) != val ||
        static_cast<int64_t>(v.low()) != val + 1)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t post-increment (1000 random)", all_passed);

  // Pre-decrement
  all_passed = true;
  for (int j = 0; j < 1000; ++j)
  {
    const int64_t val = static_cast<int64_t>(rng());
    int128_t v(val);
    int128_t &result = --v;
    if (&result != &v || static_cast<int64_t>(v.low()) != val - 1)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t pre-decrement returns ref (1000 random)", all_passed);

  // Post-decrement
  all_passed = true;
  for (int j = 0; j < 1000; ++j)
  {
    const int64_t val = static_cast<int64_t>(rng());
    int128_t v(val);
    const int128_t old = v--;
    if (static_cast<int64_t>(old.low()) != val ||
        static_cast<int64_t>(v.low()) != val - 1)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t post-decrement (1000 random)", all_passed);
}

// =============================================================================
// SECTION 7: Bitwise Tests
// =============================================================================

void test_bitwise_and()
{
  const uint128_t a(0xFF00FF00, 0x00FF00FF);
  const uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  const uint128_t result = a & b;
  TEST("uint128_t AND high", result.high() == (0xFF00FF00ULL & 0xF0F0F0F0ULL));
  TEST("uint128_t AND low", result.low() == (0x00FF00FFULL & 0x0F0F0F0FULL));

  // Random test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t x = rng();
    const uint64_t y = rng();
    const int128_t vx(0, x);
    const int128_t vy(0, y);
    const int128_t r = vx & vy;
    if (r.low() != (x & y))
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t AND (1000 random)", all_passed);
}

void test_bitwise_or()
{
  const uint128_t a(0xFF00FF00, 0x00FF00FF);
  const uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  const uint128_t result = a | b;
  TEST("uint128_t OR high", result.high() == (0xFF00FF00ULL | 0xF0F0F0F0ULL));
  TEST("uint128_t OR low", result.low() == (0x00FF00FFULL | 0x0F0F0F0FULL));

  // Random test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t x = rng();
    const uint64_t y = rng();
    const int128_t vx(0, x);
    const int128_t vy(0, y);
    const int128_t r = vx | vy;
    if (r.low() != (x | y))
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t OR (1000 random)", all_passed);
}

void test_bitwise_xor()
{
  const uint128_t a(0xFF00FF00, 0x00FF00FF);
  const uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  const uint128_t result = a ^ b;
  TEST("uint128_t XOR high", result.high() == (0xFF00FF00ULL ^ 0xF0F0F0F0ULL));
  TEST("uint128_t XOR low", result.low() == (0x00FF00FFULL ^ 0x0F0F0F0FULL));

  // Random test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t x = rng();
    const uint64_t y = rng();
    const int128_t vx(0, x);
    const int128_t vy(0, y);
    const int128_t r = vx ^ vy;
    if (r.low() != (x ^ y))
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t XOR (1000 random)", all_passed);
}

void test_bitwise_not()
{
  const uint128_t a(0, 0);
  const uint128_t result = ~a;
  TEST("uint128_t NOT high", result.high() == ~0ULL);
  TEST("uint128_t NOT low", result.low() == ~0ULL);

  // Random test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t h = rng();
    const uint64_t l = rng();
    const int128_t val(h, l);
    const int128_t r = ~val;
    if (r.high() != ~h || r.low() != ~l)
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t NOT (1000 random)", all_passed);
}

// =============================================================================
// SECTION 8: Shift Tests
// =============================================================================

void test_shift_left()
{
  const uint128_t a(0, 1);
  TEST("uint128_t << 0", (a << 0).low() == 1);
  TEST("uint128_t << 1", (a << 1).low() == 2);
  TEST("uint128_t << 64 high", (a << 64).high() == 1);
  TEST("uint128_t << 64 low", (a << 64).low() == 0);
  TEST("uint128_t << 127", (a << 127).high() == (1ULL << 63));

  // Random test (small shifts)
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t val = rng() & 0xFFFFFFFF; // Keep small
    const int shift = static_cast<int>(rng() % 64);
    const int128_t v(0, val);
    const int128_t result = v << shift;
    if (result.low() != (val << shift))
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t left shift (1000 random)", all_passed);
}

void test_shift_right()
{
  const uint128_t a(1ULL << 63, 0);
  // a >> 63: shift bit 127 to bit 64
  TEST("uint128_t >> 63", (a >> 63).low() == 0 && (a >> 63).high() == 1);

  // Arithmetic shift for signed
  const int128_t neg(-1);
  TEST("int128_t >> 1 (arithmetic)", (neg >> 1) == int128_t(-1));

  // Random test
  bool all_passed = true;
  for (int i = 0; i < 1000; ++i)
  {
    const uint64_t val = rng();
    const int shift = static_cast<int>(rng() % 64);
    const int128_t v(0, val);
    const int128_t result = v >> shift;
    if (result.low() != (val >> shift))
    {
      all_passed = false;
      break;
    }
  }
  TEST("int128_t right shift (1000 random)", all_passed);
}

// =============================================================================
// SECTION 9: String Conversion Tests
// =============================================================================

void test_to_string()
{
  const uint128_t u(0, 12345);
  TEST("uint128_t to_string()", u.to_string() == "12345");

  const uint128_t zero(0, 0);
  TEST("uint128_t(0) to_string()", zero.to_string() == "0");

  const int128_t neg(-12345);
  TEST("int128_t(-12345) to_string()", neg.to_string() == "-12345");

  const int128_t pos(123);
  TEST("int128_t(123) to_string()", pos.to_string() == "123");

  const int128_t zero_signed(0);
  TEST("int128_t(0) to_string()", zero_signed.to_string() == "0");
}

void test_from_string()
{
  const auto [err1, val1] = uint128_t::parse("12345");
  TEST("uint128_t parse success",
       err1 == parse_error::success && val1.low() == 12345);

  const auto [err2, val2] = uint128_t::parse("0xABCD");
  TEST("uint128_t parse hex",
       err2 == parse_error::success && val2.low() == 0xABCD);

  const auto [err3, val3] = int128_t::parse("-12345");
  TEST("int128_t parse negative",
       err3 == parse_error::success && val3 == int128_t(-12345));
}

// =============================================================================
// SECTION 10: Special Value Tests
// =============================================================================

void test_min_max()
{
  const auto umin = uint128_t::min();
  const auto umax = uint128_t::max();
  TEST("uint128_t::min() == 0", umin.high() == 0 && umin.low() == 0);
  TEST("uint128_t::max() high", umax.high() == ~0ULL);
  TEST("uint128_t::max() low", umax.low() == ~0ULL);

  const auto imin = int128_t::min();
  const auto imax = int128_t::max();
  TEST("int128_t::min() is negative", imin.is_negative());
  TEST("int128_t::max() is positive", !imax.is_negative());
  TEST("int128_t::min() high bit", imin.high() == (1ULL << 63));
  TEST("int128_t::max() high", imax.high() == 0x7FFFFFFFFFFFFFFFULL);
  TEST("int128_t::max() low", imax.low() == 0xFFFFFFFFFFFFFFFFULL);
}

void test_abs()
{
  const int128_t pos(42);
  const int128_t neg(-42);

  TEST("int128_t abs(42) = 42", pos.abs() == int128_t(42));
  TEST("int128_t abs(-42) = 42", neg.abs() == int128_t(42));

  // uint128_t abs is identity
  const uint128_t u(12345);
  TEST("uint128_t abs() = identity", u.abs() == u);
}

// =============================================================================
// SECTION 11: Conversion Tests
// =============================================================================

void test_bool_conversion()
{
  const uint128_t zero(0, 0);
  const uint128_t nonzero(0, 1);
  TEST("uint128_t(0) to bool", !static_cast<bool>(zero));
  TEST("uint128_t(1) to bool", static_cast<bool>(nonzero));
}

void test_integral_conversion()
{
  const uint128_t u(0, 12345);
  TEST("uint128_t to uint64_t", static_cast<uint64_t>(u) == 12345);

  const int128_t i(-42);
  TEST("int128_t to int64_t", static_cast<int64_t>(i) == -42);
}

// =============================================================================
// SECTION 12: Edge Cases and Overflow
// =============================================================================

void test_overflow_wrap()
{
  const uint128_t max_val = uint128_t::max();
  const uint128_t one(0, 1);
  const uint128_t result = max_val + one;
  TEST("uint128_t overflow wraps to 0",
       result.high() == 0 && result.low() == 0);

  const uint128_t zero(0, 0);
  const uint128_t underflow = zero - one;
  TEST("uint128_t underflow wraps to max", underflow == max_val);
}

void test_signed_overflow_wrap()
{
  const int128_t max_val = int128_t::max();
  const int128_t one(1);
  const int128_t result = max_val + one;
  TEST("int128_t MAX + 1 wraps to MIN", result == int128_t::min());

  const int128_t min_val = int128_t::min();
  const int128_t underflow = min_val - one;
  TEST("int128_t MIN - 1 wraps to MAX", underflow == int128_t::max());
}

// =============================================================================
// SECTION 13: Stream I/O Tests
// =============================================================================

void test_stream_output()
{
  std::ostringstream oss1;
  const int128_t val(42);
  oss1 << val;
  TEST("int128_t stream output positive", oss1.str() == "42");

  std::ostringstream oss2;
  const int128_t neg_val(-42);
  oss2 << neg_val;
  TEST("int128_t stream output negative", oss2.str() == "-42");

  std::ostringstream oss3;
  const uint128_t uval(12345);
  oss3 << uval;
  TEST("uint128_t stream output", oss3.str() == "12345");
}

// =============================================================================
// Main
// =============================================================================

int main()
{
  std::cout << "=== int128_base_t<S> Comprehensive Tests ===" << std::endl;
  std::cout << "Template: int128_base_tt.hpp" << std::endl;
  std::cout << std::endl;

  // Section 1: Size and Layout
  std::cout << "--- Size and Layout ---" << std::endl;
  test_sizeof_is_16_bytes();
  test_alignment();
  test_trivially_copyable();
  test_standard_layout();

  // Section 2: Constructors
  std::cout << std::endl
            << "--- Constructors ---" << std::endl;
  test_default_constructor();
  test_integral_constructor_basic();
  test_integral_constructor_positive_random();
  test_integral_constructor_negative_random();
  test_two_part_constructor();
  test_copy_move_constructors();

  // Section 3: Accessors
  std::cout << std::endl
            << "--- Accessors ---" << std::endl;
  test_high_low_accessors();
  test_set_high_low();

  // Section 4: Sign Detection
  std::cout << std::endl
            << "--- Sign Detection ---" << std::endl;
  test_is_negative();

  // Section 5: Comparisons
  std::cout << std::endl
            << "--- Comparisons ---" << std::endl;
  test_equality();
  test_ordering_unsigned();
  test_ordering_signed();

  // Section 6: Arithmetic
  std::cout << std::endl
            << "--- Arithmetic ---" << std::endl;
  test_addition();
  test_subtraction();
  test_multiplication();
  test_division();
  test_modulo();
  test_negation();
  test_increment_decrement();

  // Section 7: Bitwise
  std::cout << std::endl
            << "--- Bitwise ---" << std::endl;
  test_bitwise_and();
  test_bitwise_or();
  test_bitwise_xor();
  test_bitwise_not();

  // Section 8: Shifts
  std::cout << std::endl
            << "--- Shift Operations ---" << std::endl;
  test_shift_left();
  test_shift_right();

  // Section 9: String Conversion
  std::cout << std::endl
            << "--- String Conversion ---" << std::endl;
  test_to_string();
  test_from_string();

  // Section 10: Special Values
  std::cout << std::endl
            << "--- Special Values ---" << std::endl;
  test_min_max();
  test_abs();

  // Section 11: Conversions
  std::cout << std::endl
            << "--- Type Conversions ---" << std::endl;
  test_bool_conversion();
  test_integral_conversion();

  // Section 12: Edge Cases
  std::cout << std::endl
            << "--- Edge Cases ---" << std::endl;
  test_overflow_wrap();
  test_signed_overflow_wrap();

  // Section 13: Stream I/O
  std::cout << std::endl
            << "--- Stream I/O ---" << std::endl;
  test_stream_output();

  // Summary
  std::cout << std::endl;
  std::cout << "==========================================" << std::endl;
  std::cout << "Passed: " << tests_passed << "/" << (tests_passed + tests_failed) << std::endl;
  if (tests_failed > 0)
  {
    std::cout << "FAILED: " << tests_failed << " tests" << std::endl;
  }
  else
  {
    std::cout << "All tests passed!" << std::endl;
  }
  std::cout << "==========================================" << std::endl;

  return tests_failed > 0 ? 1 : 0;
}
