/**
 * @file int128_base_t_extracted_tests.cpp
 * @brief Tests canónicos para int128_base_t<S> (template unificado)
 *
 * Este archivo contiene tests extraídos del template unificado
 * int128_base_tt.hpp Tests para ambos tipos: uint128_t (unsigned) e int128_t
 * (signed)
 *
 * Compilar:
 *   g++ -std=c++20 -O2 -I../include tests/int128_base_t_extracted_tests.cpp -o
 * test
 *
 * @date 2026-01-08
 */

#include "../include/int128_base_tt.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <type_traits>
#include <vector>

using namespace nstd;

// Helper for random generation
static std::mt19937_64 rng(std::random_device{}());

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)                                                  \
  do {                                                                         \
    if (condition) {                                                           \
      std::cout << "[PASS] " << name << std::endl;                             \
      tests_passed++;                                                          \
    } else {                                                                   \
      std::cout << "[FAIL] " << name << std::endl;                             \
      tests_failed++;                                                          \
    }                                                                          \
  } while (0)

// =============================================================================
// SECTION 1: Size and Layout Tests
// =============================================================================

void test_sizeof_is_16_bytes() {
  TEST("sizeof(uint128_t) == 16", sizeof(uint128_t) == 16);
  TEST("sizeof(int128_t) == 16", sizeof(int128_t) == 16);
}

void test_alignment() {
  TEST("alignof(uint128_t) >= 8", alignof(uint128_t) >= 8);
  TEST("alignof(int128_t) >= 8", alignof(int128_t) >= 8);
}

void test_trivially_copyable() {
  TEST("uint128_t is trivially copyable",
       std::is_trivially_copyable_v<uint128_t>);
  TEST("int128_t is trivially copyable",
       std::is_trivially_copyable_v<int128_t>);
}

// =============================================================================
// SECTION 2: Constructor Tests
// =============================================================================

void test_default_constructor() {
  uint128_t u;
  int128_t i;
  TEST("uint128_t default == 0", u.high() == 0 && u.low() == 0);
  TEST("int128_t default == 0", i.high() == 0 && i.low() == 0);
}

void test_integral_constructor() {
  uint128_t u1(42);
  TEST("uint128_t(42) low", u1.low() == 42);
  TEST("uint128_t(42) high", u1.high() == 0);

  int128_t i1(42);
  TEST("int128_t(42) low", i1.low() == 42);
  TEST("int128_t(42) high", i1.high() == 0);

  // Negative for signed
  int128_t i2(-1);
  TEST("int128_t(-1) low", i2.low() == ~0ULL);
  TEST("int128_t(-1) high (sign extended)", i2.high() == ~0ULL);

  int128_t i3(-42);
  TEST("int128_t(-42) is negative", i3.is_negative());
}

void test_two_part_constructor() {
  uint128_t u(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
  TEST("uint128_t(high, low) high", u.high() == 0x1234567890ABCDEFULL);
  TEST("uint128_t(high, low) low", u.low() == 0xFEDCBA0987654321ULL);

  int128_t i(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
  TEST("int128_t(high, low) high", i.high() == 0x1234567890ABCDEFULL);
  TEST("int128_t(high, low) low", i.low() == 0xFEDCBA0987654321ULL);
}

void test_copy_move_constructors() {
  uint128_t original(0x1234, 0x5678);
  uint128_t copy(original);
  TEST("uint128_t copy constructor", copy == original);

  uint128_t moved(std::move(copy));
  TEST("uint128_t move constructor", moved == original);

  int128_t i_original(-12345);
  int128_t i_copy(i_original);
  TEST("int128_t copy constructor", i_copy == i_original);
}

// =============================================================================
// SECTION 3: Accessor Tests
// =============================================================================

void test_high_low_accessors() {
  for (int i = 0; i < 100; ++i) {
    uint64_t h = rng();
    uint64_t l = rng();
    uint128_t val(h, l);
    assert(val.high() == h);
    assert(val.low() == l);
  }
  TEST("high()/low() random (100 iterations)", true);
}

void test_set_high_low() {
  uint128_t val;
  val.set_high(0xABCD);
  val.set_low(0x1234);
  TEST("set_high()", val.high() == 0xABCD);
  TEST("set_low()", val.low() == 0x1234);
}

// =============================================================================
// SECTION 4: Comparison Tests
// =============================================================================

void test_equality() {
  uint128_t a(100, 200);
  uint128_t b(100, 200);
  uint128_t c(100, 201);
  TEST("uint128_t == (equal)", a == b);
  TEST("uint128_t != (not equal)", a != c);

  int128_t ia(-42);
  int128_t ib(-42);
  int128_t ic(42);
  TEST("int128_t == (equal negative)", ia == ib);
  TEST("int128_t != (neg vs pos)", ia != ic);
}

void test_ordering_unsigned() {
  uint128_t small(0, 100);
  uint128_t large(1, 0);
  TEST("uint128_t < (low vs high)", small < large);
  TEST("uint128_t > (high vs low)", large > small);
  TEST("uint128_t <=", small <= large);
  TEST("uint128_t >=", large >= small);
}

void test_ordering_signed() {
  int128_t negative(-100);
  int128_t positive(100);
  int128_t zero(0);

  TEST("int128_t: negative < zero", negative < zero);
  TEST("int128_t: negative < positive", negative < positive);
  TEST("int128_t: zero < positive", zero < positive);
  TEST("int128_t: positive > negative", positive > negative);
}

// =============================================================================
// SECTION 5: Arithmetic Tests
// =============================================================================

void test_addition() {
  uint128_t a(0, 100);
  uint128_t b(0, 50);
  TEST("uint128_t addition", (a + b).low() == 150);

  // Carry test
  uint128_t c(0, ~0ULL);
  uint128_t d(0, 1);
  uint128_t sum = c + d;
  TEST("uint128_t addition with carry", sum.high() == 1 && sum.low() == 0);

  int128_t ia(-50);
  int128_t ib(100);
  TEST("int128_t: -50 + 100 = 50", (ia + ib) == int128_t(50));
}

void test_subtraction() {
  uint128_t a(0, 100);
  uint128_t b(0, 30);
  TEST("uint128_t subtraction", (a - b).low() == 70);

  // Borrow test
  uint128_t c(1, 0);
  uint128_t d(0, 1);
  uint128_t diff = c - d;
  TEST("uint128_t subtraction with borrow",
       diff.high() == 0 && diff.low() == ~0ULL);

  int128_t ia(50);
  int128_t ib(100);
  TEST("int128_t: 50 - 100 = -50", (ia - ib) == int128_t(-50));
}

void test_multiplication() {
  uint128_t a(0, 1000);
  uint128_t b(0, 2000);
  TEST("uint128_t multiplication", (a * b).low() == 2000000);

  int128_t ia(-10);
  int128_t ib(20);
  TEST("int128_t: -10 * 20 = -200", (ia * ib) == int128_t(-200));

  int128_t ic(-10);
  int128_t id(-20);
  TEST("int128_t: -10 * -20 = 200", (ic * id) == int128_t(200));
}

void test_division() {
  uint128_t a(0, 1000);
  uint128_t b(0, 10);
  TEST("uint128_t division", (a / b).low() == 100);

  int128_t ia(-100);
  int128_t ib(10);
  TEST("int128_t: -100 / 10 = -10", (ia / ib) == int128_t(-10));

  int128_t ic(-100);
  int128_t id(-10);
  TEST("int128_t: -100 / -10 = 10", (ic / id) == int128_t(10));
}

void test_modulo() {
  uint128_t a(0, 107);
  uint128_t b(0, 10);
  TEST("uint128_t modulo", (a % b).low() == 7);

  int128_t ia(-107);
  int128_t ib(10);
  // C++ truncation toward zero: -107 % 10 = -7
  TEST("int128_t: -107 % 10 = -7", (ia % ib) == int128_t(-7));
}

void test_increment_decrement() {
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
}

// =============================================================================
// SECTION 6: Bitwise Tests
// =============================================================================

void test_bitwise_and() {
  uint128_t a(0xFF00FF00, 0x00FF00FF);
  uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  uint128_t result = a & b;
  TEST("uint128_t AND high", result.high() == (0xFF00FF00ULL & 0xF0F0F0F0ULL));
  TEST("uint128_t AND low", result.low() == (0x00FF00FFULL & 0x0F0F0F0FULL));
}

void test_bitwise_or() {
  uint128_t a(0xFF00FF00, 0x00FF00FF);
  uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  uint128_t result = a | b;
  TEST("uint128_t OR high", result.high() == (0xFF00FF00ULL | 0xF0F0F0F0ULL));
  TEST("uint128_t OR low", result.low() == (0x00FF00FFULL | 0x0F0F0F0FULL));
}

void test_bitwise_xor() {
  uint128_t a(0xFF00FF00, 0x00FF00FF);
  uint128_t b(0xF0F0F0F0, 0x0F0F0F0F);
  uint128_t result = a ^ b;
  TEST("uint128_t XOR high", result.high() == (0xFF00FF00ULL ^ 0xF0F0F0F0ULL));
  TEST("uint128_t XOR low", result.low() == (0x00FF00FFULL ^ 0x0F0F0F0FULL));
}

void test_bitwise_not() {
  uint128_t a(0, 0);
  uint128_t result = ~a;
  TEST("uint128_t NOT high", result.high() == ~0ULL);
  TEST("uint128_t NOT low", result.low() == ~0ULL);
}

void test_shift_left() {
  uint128_t a(0, 1);
  TEST("uint128_t << 0", (a << 0).low() == 1);
  TEST("uint128_t << 1", (a << 1).low() == 2);
  TEST("uint128_t << 64 high", (a << 64).high() == 1);
  TEST("uint128_t << 64 low", (a << 64).low() == 0);
  TEST("uint128_t << 127", (a << 127).high() == (1ULL << 63));
}

void test_shift_right() {
  uint128_t a(1ULL << 63, 0);
  // a = 0x8000000000000000'0000000000000000
  // a >> 63 shifts bit 127 to bit 64
  // Result: high = 1 (bit 64), low = 0
  TEST("uint128_t >> 63", (a >> 63).low() == 0 && (a >> 63).high() == 1);

  // Arithmetic shift for signed
  int128_t neg(-1);
  TEST("int128_t >> 1 (arithmetic)", (neg >> 1) == int128_t(-1));
}

// =============================================================================
// SECTION 7: String Conversion Tests
// =============================================================================

void test_to_string() {
  uint128_t u(0, 12345);
  TEST("uint128_t to_string()", u.to_string() == "12345");

  uint128_t zero(0, 0);
  TEST("uint128_t(0) to_string()", zero.to_string() == "0");

  int128_t neg(-12345);
  TEST("int128_t(-12345) to_string()", neg.to_string() == "-12345");
}

void test_from_string() {
  auto [err1, val1] = uint128_t::parse("12345");
  TEST("uint128_t parse success",
       err1 == parse_error::success && val1.low() == 12345);

  auto [err2, val2] = uint128_t::parse("0xABCD");
  TEST("uint128_t parse hex",
       err2 == parse_error::success && val2.low() == 0xABCD);

  auto [err3, val3] = int128_t::parse("-12345");
  TEST("int128_t parse negative",
       err3 == parse_error::success && val3 == int128_t(-12345));
}

// =============================================================================
// SECTION 8: Special Value Tests
// =============================================================================

void test_min_max() {
  auto umin = uint128_t::min();
  auto umax = uint128_t::max();
  TEST("uint128_t::min() == 0", umin.high() == 0 && umin.low() == 0);
  TEST("uint128_t::max() high", umax.high() == ~0ULL);
  TEST("uint128_t::max() low", umax.low() == ~0ULL);

  auto imin = int128_t::min();
  auto imax = int128_t::max();
  TEST("int128_t::min() is negative", imin.is_negative());
  TEST("int128_t::max() is positive", !imax.is_negative());
  TEST("int128_t::min() high bit", imin.high() == (1ULL << 63));
}

void test_is_negative() {
  int128_t pos(100);
  int128_t neg(-100);
  int128_t zero(0);

  TEST("int128_t positive is_negative()", !pos.is_negative());
  TEST("int128_t negative is_negative()", neg.is_negative());
  TEST("int128_t zero is_negative()", !zero.is_negative());

  // uint128_t siempre false
  uint128_t u(~0ULL, ~0ULL);
  TEST("uint128_t is_negative() always false", !u.is_negative());
}

void test_abs() {
  int128_t pos(42);
  int128_t neg(-42);

  TEST("int128_t abs(42) = 42", pos.abs() == int128_t(42));
  TEST("int128_t abs(-42) = 42", neg.abs() == int128_t(42));

  // uint128_t abs es identidad
  uint128_t u(12345);
  TEST("uint128_t abs() = identity", u.abs() == u);
}

// =============================================================================
// SECTION 9: Conversion Tests
// =============================================================================

void test_bool_conversion() {
  uint128_t zero(0, 0);
  uint128_t nonzero(0, 1);
  TEST("uint128_t(0) to bool", !static_cast<bool>(zero));
  TEST("uint128_t(1) to bool", static_cast<bool>(nonzero));
}

void test_integral_conversion() {
  uint128_t u(0, 12345);
  TEST("uint128_t to uint64_t", static_cast<uint64_t>(u) == 12345);

  int128_t i(-42);
  TEST("int128_t to int64_t", static_cast<int64_t>(i) == -42);
}

// =============================================================================
// SECTION 10: Edge Cases
// =============================================================================

void test_overflow_wrap() {
  uint128_t max = uint128_t::max();
  uint128_t one(0, 1);
  uint128_t result = max + one;
  TEST("uint128_t overflow wraps to 0",
       result.high() == 0 && result.low() == 0);

  uint128_t zero(0, 0);
  result = zero - one;
  TEST("uint128_t underflow wraps to max", result == max);
}

void test_signed_overflow_wrap() {
  int128_t max = int128_t::max();
  int128_t one(1);
  int128_t result = max + one;
  TEST("int128_t MAX + 1 wraps to MIN", result == int128_t::min());

  int128_t min = int128_t::min();
  result = min - one;
  TEST("int128_t MIN - 1 wraps to MAX", result == int128_t::max());
}

// =============================================================================
// Main
// =============================================================================

int main() {
  std::cout << "=== int128_base_t<S> Canonical Tests ===" << std::endl
            << std::endl;

  // Section 1: Size and Layout
  std::cout << "--- Size and Layout ---" << std::endl;
  test_sizeof_is_16_bytes();
  test_alignment();
  test_trivially_copyable();

  // Section 2: Constructors
  std::cout << std::endl << "--- Constructors ---" << std::endl;
  test_default_constructor();
  test_integral_constructor();
  test_two_part_constructor();
  test_copy_move_constructors();

  // Section 3: Accessors
  std::cout << std::endl << "--- Accessors ---" << std::endl;
  test_high_low_accessors();
  test_set_high_low();

  // Section 4: Comparisons
  std::cout << std::endl << "--- Comparisons ---" << std::endl;
  test_equality();
  test_ordering_unsigned();
  test_ordering_signed();

  // Section 5: Arithmetic
  std::cout << std::endl << "--- Arithmetic ---" << std::endl;
  test_addition();
  test_subtraction();
  test_multiplication();
  test_division();
  test_modulo();
  test_increment_decrement();

  // Section 6: Bitwise
  std::cout << std::endl << "--- Bitwise ---" << std::endl;
  test_bitwise_and();
  test_bitwise_or();
  test_bitwise_xor();
  test_bitwise_not();
  test_shift_left();
  test_shift_right();

  // Section 7: String Conversion
  std::cout << std::endl << "--- String Conversion ---" << std::endl;
  test_to_string();
  test_from_string();

  // Section 8: Special Values
  std::cout << std::endl << "--- Special Values ---" << std::endl;
  test_min_max();
  test_is_negative();
  test_abs();

  // Section 9: Conversions
  std::cout << std::endl << "--- Type Conversions ---" << std::endl;
  test_bool_conversion();
  test_integral_conversion();

  // Section 10: Edge Cases
  std::cout << std::endl << "--- Edge Cases ---" << std::endl;
  test_overflow_wrap();
  test_signed_overflow_wrap();

  // Summary
  std::cout << std::endl;
  std::cout << "==========================================" << std::endl;
  std::cout << "Passed: " << tests_passed << "/"
            << (tests_passed + tests_failed) << std::endl;
  std::cout << "==========================================" << std::endl;

  return tests_failed > 0 ? 1 : 0;
}
