/*
 * Test file generated from Doxygen @test and @code{.cpp} tags in uint128_t.hpp
 * Plus additional tests for robustness (copy/move, UDL, streams, edge cases, constexpr)
 */

#include "../include/uint128/uint128_iostreams.hpp"
#include "../include/uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <type_traits>
#include <vector>

using namespace nstd;


// Helper for random generation
std::mt19937_64 rng(std::random_device{}());

void test_sizeof_is_16_bytes()
{
    assert(sizeof(uint128_t) == 16);
    std::cout << "test_sizeof_is_16_bytes passed" << std::endl;
}

void test_bits_is_128()
{
    assert(uint128_t::UINT128_BITS == 128);
    std::cout << "test_bits_is_128 passed" << std::endl;
}

void test_sizeof_high_is_8_bytes()
{
    [[maybe_unused]] uint128_t val;
    assert(sizeof(val.high()) == 8);
    std::cout << "test_sizeof_high_is_8_bytes passed" << std::endl;
}

void test_self_div_2_64_equ_high()
{
    // HACER ESTO CON VALORES ALEATORIOS DE uint64_t UN MONTÓN DE VECES
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();
        const uint128_t val(h, l);
        assert((val >> 64).low() == val.high());
    }
    std::cout << "test_self_div_2_64_equ_high passed" << std::endl;
}

void test_sizeof_low_is_8_bytes()
{
    [[maybe_unused]] uint128_t val;
    assert(sizeof(val.low()) == 8);
    std::cout << "test_sizeof_low_is_8_bytes passed" << std::endl;
}

void test_self_rem_2_64_equ_low()
{
    //   Se implementa la verificación lógica: low() debe corresponder a
    // los 64 bits bajos.
    for (int i = 0; i < 1000; ++i) {
        const uint64_t h = rng();
        const uint64_t l = rng();
        const uint128_t val(h, l);
        assert(val.low() == l);
    }
    std::cout << "test_self_rem_2_64_equ_low passed" << std::endl;
}

void test_set_high()
{
    // HACER ESTO CON VALORES ALEATORIOS DE uint64_t UN MONTÓN DE VECES
    for (int i = 0; i < 1000; ++i) {
        [[maybe_unused]] uint128_t val;
        uint64_t v = rng();
        val.set_high(v);
        assert(val.high() == v);
    }
    std::cout << "test_set_high passed" << std::endl;
}

void test_set_low()
{
    // HACER ESTO CON VALORES ALEATORIOS DE uint64_t UN MONTÓN DE VECES
    for (int i = 0; i < 1000; ++i) {
        [[maybe_unused]] uint128_t val;
        uint64_t v = rng();
        val.set_low(v);
        assert(val.low() == v);
    }
    std::cout << "test_set_low passed" << std::endl;
}

void test_default_constructor()
{
    [[maybe_unused]] uint128_t val;
    assert(val.low() == 0 && val.high() == 0);
    std::cout << "test_default_constructor passed" << std::endl;
}

void test_integral_constructor()
{
    // Test for various integral types as specified in documentation
    for (int i = 0; i < 1000; ++i) {
        uint64_t rand_val = rng();

        // --- UNSIGNED TYPES (Siempre high=0) ---

        // uint8_t
        uint128_t val_u8(static_cast<uint8_t>(rand_val));
        assert(static_cast<uint8_t>(val_u8.low()) == static_cast<uint8_t>(rand_val));
        assert(val_u8.high() == 0ull);

        // uint16_t
        uint128_t val_u16(static_cast<uint16_t>(rand_val));
        assert(static_cast<uint16_t>(val_u16.low()) == static_cast<uint16_t>(rand_val));
        assert(val_u16.high() == 0ull);

        // uint32_t
        uint128_t val_u32(static_cast<uint32_t>(rand_val));
        assert(static_cast<uint32_t>(val_u32.low()) == static_cast<uint32_t>(rand_val));
        assert(val_u32.high() == 0ull);

        // uint64_t
        uint128_t val_u64(static_cast<uint64_t>(rand_val));
        assert(static_cast<uint64_t>(val_u64.low()) == static_cast<uint64_t>(rand_val));
        assert(val_u64.high() == 0ull);

        // --- SIGNED TYPES (Positivos: high=0) ---
        // Enmascaramos el bit de signo para asegurar que sean positivos

        // int8_t
        int8_t v_i8_pos = static_cast<int8_t>(rand_val & 0x7F);
        [[maybe_unused]] uint128_t val_i8_pos(v_i8_pos);
        assert(static_cast<int8_t>(val_i8_pos.low()) == v_i8_pos);
        assert(val_i8_pos.high() == 0ull);

        // int16_t
        int16_t v_i16_pos = static_cast<int16_t>(rand_val & 0x7FFF);
        [[maybe_unused]] uint128_t val_i16_pos(v_i16_pos);
        assert(static_cast<int16_t>(val_i16_pos.low()) == v_i16_pos);
        assert(val_i16_pos.high() == 0ull);

        // int32_t
        int32_t v_i32_pos = static_cast<int32_t>(rand_val & 0x7FFFFFFF);
        [[maybe_unused]] uint128_t val_i32_pos(v_i32_pos);
        assert(static_cast<int32_t>(val_i32_pos.low()) == v_i32_pos);
        assert(val_i32_pos.high() == 0ull);

        // int64_t
        int64_t v_i64_pos = static_cast<int64_t>(rand_val & 0x7FFFFFFFFFFFFFFFULL);
        [[maybe_unused]] uint128_t val_i64_pos(v_i64_pos);
        assert(static_cast<int64_t>(val_i64_pos.low()) == v_i64_pos);
        assert(val_i64_pos.high() == 0ull);

        // --- SIGNED TYPES (Negativos: high=~0) ---
        // Forzamos el bit de signo y aseguramos que sea negativo

        // int8_t
        int8_t v_i8_neg = static_cast<int8_t>(rand_val | 0x80);
        if (v_i8_neg >= 0)
            v_i8_neg = -1;
        [[maybe_unused]] uint128_t val_i8_neg(v_i8_neg);
        assert(static_cast<int8_t>(val_i8_neg.low()) == v_i8_neg);
        assert(val_i8_neg.high() == ~0ull);

        // int16_t
        int16_t v_i16_neg = static_cast<int16_t>(rand_val | 0x8000);
        if (v_i16_neg >= 0)
            v_i16_neg = -1;
        [[maybe_unused]] uint128_t val_i16_neg(v_i16_neg);
        assert(static_cast<int16_t>(val_i16_neg.low()) == v_i16_neg);
        assert(val_i16_neg.high() == ~0ull);

        // int32_t
        int32_t v_i32_neg = static_cast<int32_t>(rand_val | 0x80000000);
        if (v_i32_neg >= 0)
            v_i32_neg = -1;
        [[maybe_unused]] uint128_t val_i32_neg(v_i32_neg);
        assert(static_cast<int32_t>(val_i32_neg.low()) == v_i32_neg);
        assert(val_i32_neg.high() == ~0ull);

        // int64_t
        int64_t v_i64_neg = static_cast<int64_t>(rand_val | 0x8000000000000000ULL);
        if (v_i64_neg >= 0)
            v_i64_neg = -1;
        [[maybe_unused]] uint128_t val_i64_neg(v_i64_neg);
        assert(static_cast<int64_t>(val_i64_neg.low()) == v_i64_neg);
        assert(val_i64_neg.high() == ~0ull);
    }
    std::cout << "test_integral_constructor passed" << std::endl;
}

void test_integral_assignment()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t rand_val = rng();
        [[maybe_unused]] uint128_t val;

        // --- UNSIGNED TYPES (Siempre high=0) ---

        // uint8_t
        val = uint128_t(rng(), rng()); // Ensuciar valor previo
        val = static_cast<uint8_t>(rand_val);
        assert(static_cast<uint8_t>(val.low()) == static_cast<uint8_t>(rand_val));
        assert(val.high() == 0ull);

        // uint16_t
        val = uint128_t(rng(), rng());
        val = static_cast<uint16_t>(rand_val);
        assert(static_cast<uint16_t>(val.low()) == static_cast<uint16_t>(rand_val));
        assert(val.high() == 0ull);

        // uint32_t
        val = uint128_t(rng(), rng());
        val = static_cast<uint32_t>(rand_val);
        assert(static_cast<uint32_t>(val.low()) == static_cast<uint32_t>(rand_val));
        assert(val.high() == 0ull);

        // uint64_t
        val = uint128_t(rng(), rng());
        val = static_cast<uint64_t>(rand_val);
        assert(static_cast<uint64_t>(val.low()) == static_cast<uint64_t>(rand_val));
        assert(val.high() == 0ull);

        // --- SIGNED TYPES (Positivos: high=0) ---

        // int8_t
        int8_t v_i8_pos = static_cast<int8_t>(rand_val & 0x7F);
        val = uint128_t(rng(), rng());
        val = v_i8_pos;
        assert(static_cast<int8_t>(val.low()) == v_i8_pos);
        assert(val.high() == 0ull);

        // int16_t
        int16_t v_i16_pos = static_cast<int16_t>(rand_val & 0x7FFF);
        val = uint128_t(rng(), rng());
        val = v_i16_pos;
        assert(static_cast<int16_t>(val.low()) == v_i16_pos);
        assert(val.high() == 0ull);

        // int32_t
        int32_t v_i32_pos = static_cast<int32_t>(rand_val & 0x7FFFFFFF);
        val = uint128_t(rng(), rng());
        val = v_i32_pos;
        assert(static_cast<int32_t>(val.low()) == v_i32_pos);
        assert(val.high() == 0ull);

        // int64_t
        int64_t v_i64_pos = static_cast<int64_t>(rand_val & 0x7FFFFFFFFFFFFFFFULL);
        val = uint128_t(rng(), rng());
        val = v_i64_pos;
        assert(static_cast<int64_t>(val.low()) == v_i64_pos);
        assert(val.high() == 0ull);

        // --- SIGNED TYPES (Negativos: high=~0) ---

        // int8_t
        int8_t v_i8_neg = static_cast<int8_t>(rand_val | 0x80);
        if (v_i8_neg >= 0)
            v_i8_neg = -1;
        val = uint128_t(rng(), rng());
        val = v_i8_neg;
        assert(static_cast<int8_t>(val.low()) == v_i8_neg);
        assert(val.high() == ~0ull);

        // int16_t
        int16_t v_i16_neg = static_cast<int16_t>(rand_val | 0x8000);
        if (v_i16_neg >= 0)
            v_i16_neg = -1;
        val = uint128_t(rng(), rng());
        val = v_i16_neg;
        assert(static_cast<int16_t>(val.low()) == v_i16_neg);
        assert(val.high() == ~0ull);

        // int32_t
        int32_t v_i32_neg = static_cast<int32_t>(rand_val | 0x80000000);
        if (v_i32_neg >= 0)
            v_i32_neg = -1;
        val = uint128_t(rng(), rng());
        val = v_i32_neg;
        assert(static_cast<int32_t>(val.low()) == v_i32_neg);
        assert(val.high() == ~0ull);

        // int64_t
        int64_t v_i64_neg = static_cast<int64_t>(rand_val | 0x8000000000000000ULL);
        if (v_i64_neg >= 0)
            v_i64_neg = -1;
        val = uint128_t(rng(), rng());
        val = v_i64_neg;
        assert(static_cast<int64_t>(val.low()) == v_i64_neg);
        assert(val.high() == ~0ull);
    }
    std::cout << "test_integral_assignment passed" << std::endl;
}

void test_high_low_constructor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();

        // Test with uint64_t
        [[maybe_unused]] uint128_t val(h, l);
        assert(val.high() == h);
        assert(val.low() == l);

        // Test with mixed types (e.g. uint32_t)
        uint32_t h32 = static_cast<uint32_t>(h);
        uint32_t l32 = static_cast<uint32_t>(l);
        uint128_t val32(h32, l32);
        assert(val32.high() == h32);
        assert(val32.low() == l32);

        // Test with signed types (random values which might be negative)
        int64_t h_signed = static_cast<int64_t>(h);
        int64_t l_signed = static_cast<int64_t>(l);

        [[maybe_unused]] uint128_t val_signed(h_signed, l_signed);

        // Verification: The constructor casts to uint64_t, which for negative
        // numbers behaves as 2's complement (standard behavior for unsigned
        // cast).
        assert(val_signed.high() == static_cast<uint64_t>(h_signed));
        assert(val_signed.low() == static_cast<uint64_t>(l_signed));

        // Test with mixed signed/unsigned
        [[maybe_unused]] uint128_t val_mixed1(h_signed, l);
        assert(val_mixed1.high() == static_cast<uint64_t>(h_signed));
        assert(val_mixed1.low() == l);

        [[maybe_unused]] uint128_t val_mixed2(h, l_signed);
        assert(val_mixed2.high() == h);
        assert(val_mixed2.low() == static_cast<uint64_t>(l_signed));

        // Test with smaller signed types (to check sign extension behavior)
        int8_t h_i8 = static_cast<int8_t>(h);    // Random 8-bit signed
        int16_t l_i16 = static_cast<int16_t>(l); // Random 16-bit signed

        [[maybe_unused]] uint128_t val_small_signed(h_i8, l_i16);

        // Sign extension is expected here because the constructor takes T1, T2
        // and casts to uint64_t. static_cast<uint64_t>(int8_t) extends sign.
        assert(val_small_signed.high() == static_cast<uint64_t>(h_i8));
        assert(val_small_signed.low() == static_cast<uint64_t>(l_i16));
    }
    std::cout << "test_high_low_constructor passed" << std::endl;
}

void test_cstr_constructor()
{
    // Test hex parsing
    [[maybe_unused]] uint128_t val_hex("0x123456789ABCDEF0");
    assert(val_hex.low() == 0x123456789ABCDEF0ULL);
    assert(val_hex.high() == 0);

    // Test large hex parsing (spanning into high bits)
    // 0x10000000000000000 (1 followed by 16 zeros) = 2^64
    [[maybe_unused]] uint128_t val_large("0x10000000000000000");
    assert(val_large.high() == 1);
    assert(val_large.low() == 0);

    // Test decimal parsing
    [[maybe_unused]] uint128_t val_dec("12345");
    assert(val_dec.low() == 12345);
    assert(val_dec.high() == 0);

    // Test octal parsing (prefix 0)
    [[maybe_unused]] uint128_t val_oct("010"); // Octal 10 = Decimal 8
    assert(val_oct.low() == 8);

    // Test binary parsing (prefix 0b)
    [[maybe_unused]] uint128_t val_bin("0b1010"); // Binary 1010 = Decimal 10
    assert(val_bin.low() == 10);

    std::cout << "test_cstr_constructor passed" << std::endl;
}

void test_cstr_assignment()
{
    [[maybe_unused]] uint128_t val;

    // Test hex assignment
    val = uint128_t(rng(), rng()); // Ensuciar valor previo
    val = "0x123456789ABCDEF0";
    assert(val.low() == 0x123456789ABCDEF0ULL);
    assert(val.high() == 0);

    // Test large hex assignment
    val = uint128_t(rng(), rng());
    val = "0x10000000000000000";
    assert(val.high() == 1);
    assert(val.low() == 0);

    // Test decimal assignment
    val = uint128_t(rng(), rng());
    val = "12345";
    assert(val.low() == 12345);
    assert(val.high() == 0);

    // Test octal assignment
    val = uint128_t(rng(), rng());
    val = "010";
    assert(val.low() == 8);

    // Test binary assignment
    val = uint128_t(rng(), rng());
    val = "0b1010";
    assert(val.low() == 10);

    std::cout << "test_cstr_assignment passed" << std::endl;
}

void test_bool_conversion()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();
        if (h == 0 && l == 0)
            l = 1; // Asegurar valor no-cero

        [[maybe_unused]] uint128_t val(h, l);
        assert(static_cast<bool>(val) == true);
        assert(val); // Conversión contextual

        val = uint128_t(0);
        assert(static_cast<bool>(val) == false);
        assert(!val); // Conversión contextual
    }
    std::cout << "test_bool_conversion passed" << std::endl;
}

void test_integral_conversion()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t rand_val = rng();
        [[maybe_unused]] uint128_t val(rand_val);

        // Unsigned types
        assert(static_cast<uint8_t>(val) == static_cast<uint8_t>(rand_val));
        assert(static_cast<uint16_t>(val) == static_cast<uint16_t>(rand_val));
        assert(static_cast<uint32_t>(val) == static_cast<uint32_t>(rand_val));
        assert(static_cast<uint64_t>(val) == static_cast<uint64_t>(rand_val));

        // Signed types
        assert(static_cast<int8_t>(val) == static_cast<int8_t>(rand_val));
        assert(static_cast<int16_t>(val) == static_cast<int16_t>(rand_val));
        assert(static_cast<int32_t>(val) == static_cast<int32_t>(rand_val));
        assert(static_cast<int64_t>(val) == static_cast<int64_t>(rand_val));
    }

    // Test specific value from docs
    [[maybe_unused]] uint128_t val(123);
    assert(static_cast<uint8_t>(val) == 123);
    assert(static_cast<uint16_t>(val) == 123);
    assert(static_cast<uint32_t>(val) == 123);
    assert(static_cast<uint64_t>(val) == 123);
    assert(static_cast<int8_t>(val) == 123);
    assert(static_cast<int16_t>(val) == 123);
    assert(static_cast<int32_t>(val) == 123);
    assert(static_cast<int64_t>(val) == 123);

    std::cout << "test_integral_conversion passed" << std::endl;
}

void test___uint128_conversion()
{
#if defined(__SIZEOF_INT128__)
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();
        [[maybe_unused]] uint128_t val(h, l);
        [[maybe_unused]] __uint128_t native = static_cast<__uint128_t>(val);

        assert((static_cast<uint64_t>(native >> 64)) == h);
        assert(static_cast<uint64_t>(native) == l);
    }
    std::cout << "test___uint128_conversion passed" << std::endl;
#else
    std::cout << "test___uint128_conversion skipped" << std::endl;
#endif
}

void test___int128_conversion()
{
#if defined(__SIZEOF_INT128__)
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();
        [[maybe_unused]] uint128_t val(h, l);
        __int128_t native = static_cast<__int128_t>(val);

        [[maybe_unused]] __uint128_t native_u = static_cast<__uint128_t>(native);
        assert((static_cast<uint64_t>(native_u >> 64)) == h);
        assert(static_cast<uint64_t>(native_u) == l);
    }
    std::cout << "test___int128_conversion passed" << std::endl;
#else
    std::cout << "test___int128_conversion skipped" << std::endl;
#endif
}

void test_addition_assignment()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        // Identidad: a += 0 no cambia a
        uint128_t a_copy = a;
        a_copy += uint128_t(0);
        assert(a_copy == a);

        // Conmutatividad: a + b == b + a
        uint128_t sum1 = a;
        sum1 += b;
        uint128_t sum2 = b;
        sum2 += a;
        assert(sum1 == sum2);

        // Asociatividad: (a + b) + c == a + (b + c)
        uint128_t c(rng(), rng());
        uint128_t s1 = sum1; // a + b
        s1 += c;

        uint128_t bc = b;
        bc += c;
        uint128_t s2 = a;
        s2 += bc;
        assert(s1 == s2);
    }

    // Overflow tests
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    max_val += 1;
    assert(max_val == uint128_t(0));

    uint128_t low_max(0, UINT64_MAX);
    low_max += 1;
    assert(low_max == uint128_t(1, 0));

    std::cout << "test_addition_assignment passed" << std::endl;
}

void test_subtraction_assignment()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        // Identidad: a -= 0 no cambia a
        uint128_t a_copy = a;
        a_copy -= uint128_t(0);
        assert(a_copy == a);

        // Propiedad inversa: (a - b) + b == a
        uint128_t a_copy_inv = a;
        a_copy_inv -= b;
        a_copy_inv += b;
        assert(a_copy_inv == a);

        // Otra verificación: c - c = 0
        uint128_t c(rng(), rng());
        uint128_t c_copy = c;
        c -= c_copy;
        assert(c == uint128_t(0));

        // Test a' -= b vs decrementing b veces
        uint128_t a_prime = a;
        uint128_t a_double_prime = a;
        // Usar un valor pequeño para b para que el bucle sea factible
        uint16_t small_b_val = static_cast<uint16_t>(rng() % 1001); // 0 to 1000
        uint128_t small_b(small_b_val);

        a_prime -= small_b;
        for (uint16_t j = 0; j < small_b_val; ++j) {
            --a_double_prime;
        }
        assert(a_prime == a_double_prime);

        // Test a' != a, excepto para b == 0
        if (b != uint128_t(0)) {
            uint128_t a_copy_neq = a;
            a_copy_neq -= b;
            assert(a_copy_neq != a);
        }
    }

    // Underflow tests
    uint128_t zero_val(0);
    zero_val -= 1;
    assert(zero_val == uint128_t(UINT64_MAX, UINT64_MAX));

    uint128_t high_one(1, 0);
    high_one -= 1;
    assert(high_one == uint128_t(0, UINT64_MAX));

    std::cout << "test_subtraction_assignment passed" << std::endl;
}

void test_add_operator()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());
        uint128_t c(rng(), rng());

        // Conmutatividad
        assert(a + b == b + a);

        // Asociatividad
        assert((a + b) + c == a + (b + c));

        // Identidad
        assert(a + uint128_t(0) == a);

        // Consistencia con +=
        uint128_t sum = a;
        sum += b;
        assert(sum == a + b);
    }

    // Overflow tests
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(max_val + uint128_t(1) == uint128_t(0));

    std::cout << "test_add_operator passed" << std::endl;
}

void test_sub_operator()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        // Identidad
        assert(a - uint128_t(0) == a);

        // Inversa
        assert((a - b) + b == a);

        // Consistencia con -=
        uint128_t diff = a;
        diff -= b;
        assert(diff == a - b);
    }

    // Underflow tests
    [[maybe_unused]] uint128_t zero(0);
    assert(zero - uint128_t(1) == uint128_t(UINT64_MAX, UINT64_MAX));

    std::cout << "test_sub_operator passed" << std::endl;
}

void test_pre_increment()
{
    // Simple increment
    [[maybe_unused]] uint128_t val(0, 5);
    ++val;
    assert(val == uint128_t(0, 6));

    // Overflow low part
    [[maybe_unused]] uint128_t val_overflow_low(0, UINT64_MAX);
    ++val_overflow_low;
    assert(val_overflow_low == uint128_t(1, 0));

    // Overflow full 128-bit
    [[maybe_unused]] uint128_t val_max(UINT64_MAX, UINT64_MAX);
    ++val_max;
    assert(val_max == uint128_t(0, 0));

    std::cout << "test_pre_increment passed" << std::endl;
}

void test_post_increment()
{
    // Simple increment
    [[maybe_unused]] uint128_t val(0, 5);
    uint128_t old = val++;
    assert(old == uint128_t(0, 5));
    assert(val == uint128_t(0, 6));

    // Overflow low part
    [[maybe_unused]] uint128_t val_overflow_low(0, UINT64_MAX);
    old = val_overflow_low++;
    assert(old == uint128_t(0, UINT64_MAX));
    assert(val_overflow_low == uint128_t(1, 0));

    // Overflow full 128-bit
    [[maybe_unused]] uint128_t val_max(UINT64_MAX, UINT64_MAX);
    old = val_max++;
    assert(old == uint128_t(UINT64_MAX, UINT64_MAX));
    assert(val_max == uint128_t(0, 0));

    std::cout << "test_post_increment passed" << std::endl;
}

void test_pre_decrement()
{
    // Simple decrement
    [[maybe_unused]] uint128_t val(0, 6);
    --val;
    assert(val == uint128_t(0, 5));

    // Underflow low part (borrow from high)
    [[maybe_unused]] uint128_t val_underflow_low(1, 0);
    --val_underflow_low;
    assert(val_underflow_low == uint128_t(0, UINT64_MAX));

    // Underflow full 128-bit
    [[maybe_unused]] uint128_t val_zero(0, 0);
    --val_zero;
    assert(val_zero == uint128_t(UINT64_MAX, UINT64_MAX));

    std::cout << "test_pre_decrement passed" << std::endl;
}

void test_post_decrement()
{
    // Simple decrement
    [[maybe_unused]] uint128_t val(0, 6);
    uint128_t old = val--;
    assert(old == uint128_t(0, 6));
    assert(val == uint128_t(0, 5));

    // Underflow low part
    [[maybe_unused]] uint128_t val_underflow_low(1, 0);
    old = val_underflow_low--;
    assert(old == uint128_t(1, 0));
    assert(val_underflow_low == uint128_t(0, UINT64_MAX));

    // Underflow full 128-bit
    [[maybe_unused]] uint128_t val_zero(0, 0);
    old = val_zero--;
    assert(old == uint128_t(0, 0));
    assert(val_zero == uint128_t(UINT64_MAX, UINT64_MAX));

    std::cout << "test_post_decrement passed" << std::endl;
}

void test_leading_zeros()
{
    // Test 0
    [[maybe_unused]] uint128_t zero(0, 0);
    assert(zero.leading_zeros() == 128);

    // Test 1
    [[maybe_unused]] uint128_t one(0, 1);
    assert(one.leading_zeros() == 127);

    // Test powers of 2
    for (int i = 0; i < 128; ++i) {
        [[maybe_unused]] uint128_t val = uint128_t(1) << i;
        assert(val.leading_zeros() == 127 - i);
    }

    // Test max value
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(max_val.leading_zeros() == 0);

    std::cout << "test_leading_zeros passed" << std::endl;
}

void test_trailing_zeros()
{
    // Test 0
    [[maybe_unused]] uint128_t zero(0, 0);
    assert(zero.trailing_zeros() == 128);

    // Test 1
    [[maybe_unused]] uint128_t one(0, 1);
    assert(one.trailing_zeros() == 0);

    // Test powers of 2
    for (int i = 0; i < 128; ++i) {
        [[maybe_unused]] uint128_t val = uint128_t(1) << i;
        assert(val.trailing_zeros() == i);
    }

    // Test max value
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(max_val.trailing_zeros() == 0);

    std::cout << "test_trailing_zeros passed" << std::endl;
}

void test_bitwise_operators()
{
    // Static tests
    [[maybe_unused]] uint128_t a(0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA);
    [[maybe_unused]] uint128_t b(0x5555555555555555, 0x5555555555555555);

    // AND
    assert((a & b) == uint128_t(0));

    // OR
    assert((a | b) == uint128_t(UINT64_MAX, UINT64_MAX));

    // XOR
    assert((a ^ b) == uint128_t(UINT64_MAX, UINT64_MAX));
    assert((a ^ a) == uint128_t(0));

    // NOT
    assert(~a == b);
    assert(~b == a);

    // Random tests
    for (int i = 0; i < 1000; ++i) {
        uint128_t r1(rng(), rng());
        uint128_t r2(rng(), rng());
        uint128_t r3(rng(), rng());

        // Commutativity
        assert((r1 & r2) == (r2 & r1));
        assert((r1 | r2) == (r2 | r1));
        assert((r1 ^ r2) == (r2 ^ r1));

        // Associativity
        assert(((r1 & r2) & r3) == (r1 & (r2 & r3)));
        assert(((r1 | r2) | r3) == (r1 | (r2 | r3)));
        assert(((r1 ^ r2) ^ r3) == (r1 ^ (r2 ^ r3)));

        // Distributivity
        assert((r1 & (r2 | r3)) == ((r1 & r2) | (r1 & r3)));
        assert((r1 | (r2 & r3)) == ((r1 | r2) & (r1 | r3)));

        // Identity / Annihilator
        assert((r1 & uint128_t(0)) == uint128_t(0));
        assert((r1 | uint128_t(0)) == r1);
        assert((r1 ^ uint128_t(0)) == r1);
        assert((r1 & uint128_t(UINT64_MAX, UINT64_MAX)) == r1);
        assert((r1 | uint128_t(UINT64_MAX, UINT64_MAX)) == uint128_t(UINT64_MAX, UINT64_MAX));

        // Self
        assert((r1 & r1) == r1);
        assert((r1 | r1) == r1);
        assert((r1 ^ r1) == uint128_t(0));

        // Double negation
        assert(~~r1 == r1);

        // De Morgan's laws
        assert(~(r1 & r2) == (~r1 | ~r2));
        assert(~(r1 | r2) == (~r1 & ~r2));
    }

    std::cout << "test_bitwise_operators passed" << std::endl;
}

void test_bitwise_assignment_operators()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        // AND assignment
        uint128_t a_and = a;
        a_and &= b;
        assert(a_and == (a & b));

        // OR assignment
        uint128_t a_or = a;
        a_or |= b;
        assert(a_or == (a | b));

        // XOR assignment
        uint128_t a_xor = a;
        a_xor ^= b;
        assert(a_xor == (a ^ b));
    }
    std::cout << "test_bitwise_assignment_operators passed" << std::endl;
}

void test_shift_left()
{
    // Test basic left shift
    [[maybe_unused]] uint128_t val(0, 1);
    assert((val << 1) == uint128_t(0, 2));
    assert((val << 63) == uint128_t(0, 1ULL << 63));

    // Left shift
    assert((val << 1) == uint128_t(0, 2));
    assert((val << 63) == uint128_t(0, 0x8000000000000000ULL));
    // Test shift across boundary
    assert((val << 64) == uint128_t(1, 0));
    assert((val << 127) == uint128_t(0x8000000000000000ULL, 0));
    assert((val << 127) == uint128_t(1ULL << 63, 0));

    // Test shift by 0
    assert((val << 0) == val);

    // Test shift by >= 128
    assert((val << 128) == uint128_t(0, 0));
    assert((val << 200) == uint128_t(0, 0));

    // Test assignment operator
    [[maybe_unused]] uint128_t val_assign(0, 1);
    val_assign <<= 10;
    assert(val_assign == uint128_t(0, 1024));
    val_assign <<= 60; // total shift 70
    assert(val_assign == (uint128_t(1) << 70));

    std::cout << "test_shift_left passed" << std::endl;
}

void test_shift_right()
{
    // Test basic right shift
    [[maybe_unused]] uint128_t val(1ULL << 63, 0); // MSB of high part is 1
    assert((val >> 1) == uint128_t(1ULL << 62, 0));
    assert((val >> 63) == uint128_t(1, 0));

    // Test shift across boundary
    assert((val >> 64) == uint128_t(0, 1ULL << 63));
    assert((val >> 127) == uint128_t(0, 1));

    // Test shift by 0
    assert((val >> 0) == val);

    // Test shift by >= 128
    assert((val >> 128) == uint128_t(0, 0));
    assert((val >> 200) == uint128_t(0, 0));

    // Test assignment operator
    [[maybe_unused]] uint128_t val_assign(1ULL << 63, 0);
    val_assign >>= 10;
    assert(val_assign == (uint128_t(1ULL << 63, 0) >> 10));
    val_assign >>= 60; // total shift 70
    assert(val_assign == (uint128_t(1ULL << 63, 0) >> 70));

    std::cout << "test_shift_right passed" << std::endl;
}

void test_effective_length()
{
    // Test 0
    [[maybe_unused]] uint128_t zero(0, 0);
    assert(zero.effective_length() == 0);

    // Test 1
    [[maybe_unused]] uint128_t one(0, 1);
    assert(one.effective_length() == 1);

    // Test powers of 2
    for (int i = 0; i < 128; ++i) {
        [[maybe_unused]] uint128_t val = uint128_t(1) << i;
        assert(val.effective_length() == i + 1);
    }

    // Test max value
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(max_val.effective_length() == 128);

    std::cout << "test_effective_length passed" << std::endl;
}

void test_is_power_of_2()
{
    // Test 0
    [[maybe_unused]] uint128_t zero(0, 0);
    assert(!zero.is_power_of_2());

    // Test powers of 2
    for (int i = 0; i < 128; ++i) {
        [[maybe_unused]] uint128_t val = uint128_t(1) << i;
        assert(val.is_power_of_2());
    }

    // Test non-powers of 2
    for (int i = 2; i < 128; ++i) {
        [[maybe_unused]] uint128_t val = (uint128_t(1) << i) + uint128_t(1);
        assert(!val.is_power_of_2());
    }

    // Test max value (all ones)
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(!max_val.is_power_of_2());

    std::cout << "test_is_power_of_2 passed" << std::endl;
}

void test_comparison_operators()
{
    // Equality / Inequality
    [[maybe_unused]] uint128_t zero(0);
    [[maybe_unused]] uint128_t one(1);
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);

    assert(zero == zero);
    assert(one == one);
    assert(max_val == max_val);
    assert(!(zero == one));
    assert(zero != one);
    assert(!(zero != zero));

    // Less than / Greater than
    assert(zero < one);
    assert(one > zero);
    assert(!(one < zero));
    assert(!(zero > one));

    // High part comparison
    [[maybe_unused]] uint128_t h1(1, 0);
    [[maybe_unused]] uint128_t h2(2, 0);
    assert(h1 < h2);
    assert(h2 > h1);

    // Low part comparison with equal high
    [[maybe_unused]] uint128_t hl1(1, 10);
    [[maybe_unused]] uint128_t hl2(1, 20);
    assert(hl1 < hl2);
    assert(hl2 > hl1);

    // Mixed comparison (high part dominates)
    [[maybe_unused]] uint128_t m1(1, UINT64_MAX);
    [[maybe_unused]] uint128_t m2(2, 0);
    assert(m1 < m2);
    assert(m2 > m1);

    // <= and >=
    assert(zero <= zero);
    assert(zero <= one);
    assert(one >= one);
    assert(one >= zero);
    assert(!(one <= zero));
    assert(!(zero >= one));

    // Edge cases
    [[maybe_unused]] uint128_t max_low(0, UINT64_MAX);
    [[maybe_unused]] uint128_t min_high(1, 0);
    assert(max_low < min_high);
    assert(min_high > max_low);

    [[maybe_unused]] uint128_t max_u128(UINT64_MAX, UINT64_MAX);
    [[maybe_unused]] uint128_t almost_max(UINT64_MAX, UINT64_MAX - 1);
    assert(almost_max < max_u128);
    assert(max_u128 > almost_max);

    // Random tests
    for (int i = 0; i < 1000; ++i) {
        uint64_t h1_r = rng();
        uint64_t l1_r = rng();
        uint64_t h2_r = rng();
        uint64_t l2_r = rng();

        uint128_t u1(h1_r, l1_r);
        uint128_t u2(h2_r, l2_r);

        [[maybe_unused]] bool real_less = (h1_r < h2_r) || (h1_r == h2_r && l1_r < l2_r);
        [[maybe_unused]] bool real_eq = (h1_r == h2_r) && (l1_r == l2_r);

        assert((u1 < u2) == real_less);
        assert((u1 > u2) == (!real_less && !real_eq));
        assert((u1 == u2) == real_eq);
        assert((u1 != u2) == !real_eq);
        assert((u1 <= u2) == (real_less || real_eq));
        assert((u1 >= u2) == (!real_less));
    }

    std::cout << "test_comparison_operators passed" << std::endl;
}

using namespace uint128_literals;

void test_divrem_basic()
{
    std::cout << "test_divrem: test_divrem_basic ......" << std::endl;
    uint128_t a = 100_u128;
    uint128_t b = 7_u128;

    [[maybe_unused]] auto res = a.divrem(b);
    assert(res.has_value());
    assert(res->first == 14_u128);
    assert(res->second == 2_u128);

    // Check operators
    uint128_t a_div = a;
    a_div /= b;
    assert(a_div == 14_u128);

    uint128_t a_mod = a;
    a_mod %= b;
    assert(a_mod == 2_u128);
    std::cout << "test_divrem: test_divrem_basic passed" << std::endl;
}

void test_divrem_zero()
{
    std::cout << "test_divrem: test_divrem_zero ......" << std::endl;
    uint128_t a = 100_u128;
    [[maybe_unused]] auto res = a.divrem(0_u128);
    assert(!res.has_value());

    // Check operators (division by zero results in 0 per implementation)
    uint128_t a_div = a;
    a_div /= 0_u128;
    assert(a_div == 0_u128);

    uint128_t a_mod = a;
    a_mod %= 0_u128;
    assert(a_mod == 0_u128);
    std::cout << "test_divrem: test_divrem_zero passed" << std::endl;
}

void test_divrem_large()
{
    std::cout << "test_divrem: test_divrem_large ......" << std::endl;
    // 2^100
    [[maybe_unused]] uint128_t large = 1_u128 << 100;
    uint128_t divisor = 3_u128;

    [[maybe_unused]] auto res = large.divrem(divisor);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res->first * divisor + res->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res->second < divisor);

    // Check operators
    uint128_t l_div = large;
    l_div /= divisor;
    assert(l_div == res->first);

    uint128_t l_mod = large;
    l_mod %= divisor;
    assert(l_mod == res->second);
    std::cout << "test_divrem: test_divrem_large passed" << std::endl;
}

void test_divrem_zero_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_zero_integral_divisor ......" << std::endl;
    uint128_t a = 100_u128;
    [[maybe_unused]] auto res_u64 = a.divrem(static_cast<uint64_t>(0U));
    assert(!res_u64.has_value());
    [[maybe_unused]] auto res_i64 = a.divrem(static_cast<int64_t>(0));
    assert(!res_i64.has_value());
    [[maybe_unused]] auto res_u32 = a.divrem(static_cast<uint32_t>(0U));
    assert(!res_u32.has_value());
    [[maybe_unused]] auto res_i32 = a.divrem(static_cast<int32_t>(0));
    assert(!res_i32.has_value());
    [[maybe_unused]] auto res_u16 = a.divrem(static_cast<uint16_t>(0U));
    assert(!res_u16.has_value());
    [[maybe_unused]] auto res_i16 = a.divrem(static_cast<int16_t>(0));
    assert(!res_i16.has_value());
    [[maybe_unused]] auto res_u8 = a.divrem(static_cast<uint8_t>(0U));
    assert(!res_u8.has_value());
    [[maybe_unused]] auto res_i8 = a.divrem(static_cast<int8_t>(0));
    assert(!res_i8.has_value());
    std::cout << "test_divrem: test_divrem_zero_integral_divisor passed" << std::endl;
}

void test_divrem_basic_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_basic_integral_divisor ......" << std::endl;
    uint128_t a = 100_u128;
    uint64_t b_u64 = 7ULL;
    [[maybe_unused]] auto res_u64 = a.divrem(b_u64);
    assert(res_u64.has_value());
    assert(res_u64->first == 14_u128);
    assert(res_u64->second == 2_u128);
    uint128_t a_div = a;
    a_div /= b_u64;
    assert(a_div == 14_u128);
    uint128_t a_mod = a;
    a_mod %= b_u64;
    assert(a_mod == 2_u128);

    int64_t b_i64 = 7LL;
    [[maybe_unused]] auto res_i64 = a.divrem(b_i64);
    assert(res_i64.has_value());
    assert(res_i64->first == 14_u128);
    assert(res_i64->second == 2_u128);
    a_div = a;
    a_div /= b_i64;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_i64;
    assert(a_mod == 2_u128);

    uint32_t b_u32 = 7U;
    [[maybe_unused]] auto res_u32 = a.divrem(b_u32);
    assert(res_u32.has_value());
    assert(res_u32->first == 14_u128);
    assert(res_u32->second == 2_u128);
    a_div = a;
    a_div /= b_u32;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_u32;
    assert(a_mod == 2_u128);

    int32_t b_i32 = 7L;
    [[maybe_unused]] auto res_i32 = a.divrem(b_i32);
    assert(res_i32.has_value());
    assert(res_i32->first == 14_u128);
    assert(res_i32->second == 2_u128);
    a_div = a;
    a_div /= b_i32;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_i32;
    assert(a_mod == 2_u128);

    uint16_t b_u16 = 7u;
    [[maybe_unused]] auto res_u16 = a.divrem(b_u16);
    assert(res_u16.has_value());
    assert(res_u16->first == 14_u128);
    assert(res_u16->second == 2_u128);
    a_div = a;
    a_div /= b_u16;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_u16;
    assert(a_mod == 2_u128);

    int16_t b_i16 = 7;
    [[maybe_unused]] auto res_i16 = a.divrem(b_i16);
    assert(res_i16.has_value());
    assert(res_i16->first == 14_u128);
    assert(res_i16->second == 2_u128);
    a_div = a;
    a_div /= b_i16;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_i16;
    assert(a_mod == 2_u128);

    uint8_t b_u8 = 7;
    [[maybe_unused]] auto res_u8 = a.divrem(b_u8);
    assert(res_u8.has_value());
    assert(res_u8->first == 14_u128);
    assert(res_u8->second == 2_u128);
    a_div = a;
    a_div /= b_u8;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_u8;
    assert(a_mod == 2_u128);

    int8_t b_i8 = 7;
    [[maybe_unused]] auto res_i8 = a.divrem(b_i8);
    assert(res_i8.has_value());
    assert(res_i8->first == 14_u128);
    assert(res_i8->second == 2_u128);
    a_div = a;
    a_div /= b_i8;
    assert(a_div == 14_u128);
    a_mod = a;
    a_mod %= b_i8;
    assert(a_mod == 2_u128);

    std::cout << "test_divrem: test_divrem_basic_integral_divisor passed" << std::endl;
}

void test_divrem_large_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_large_integral_divisor ......" << std::endl;
    // 2^100
    [[maybe_unused]] uint128_t large = 1_u128 << 100;

    // Divisor como entero de 64 bits sin signo
    uint64_t divisor_u64 = 3ULL;

    [[maybe_unused]] auto res_u64 = large.divrem(divisor_u64);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u64->first * divisor_u64 + res_u64->second == large);

    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u64->second < divisor_u64);

    uint128_t l_div = large;
    l_div /= divisor_u64;
    assert(l_div == res_u64->first);
    uint128_t l_mod = large;
    l_mod %= divisor_u64;
    assert(l_mod == res_u64->second);

    // Divisor como entero de 64 bits con signo
    int64_t divisor_i64 = 3LL;

    [[maybe_unused]] auto res_i64 = large.divrem(divisor_i64);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i64->first * divisor_i64 + res_i64->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i64->second < divisor_i64);

    l_div = large;
    l_div /= divisor_i64;
    assert(l_div == res_i64->first);
    l_mod = large;
    l_mod %= divisor_i64;
    assert(l_mod == res_i64->second);

    // Divisor como entero de 32 bits sin signo
    uint32_t divisor_u32 = 3U;

    [[maybe_unused]] auto res_u32 = large.divrem(divisor_u32);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u32->first * divisor_u32 + res_u32->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u32->second < divisor_u32);

    l_div = large;
    l_div /= divisor_u32;
    assert(l_div == res_u32->first);
    l_mod = large;
    l_mod %= divisor_u32;
    assert(l_mod == res_u32->second);

    // Divisor como entero de 32 bits con signo
    int32_t divisor_i32 = 3L;

    [[maybe_unused]] auto res_i32 = large.divrem(divisor_i32);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i32->first * divisor_i32 + res_i32->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i32->second < divisor_i32);

    l_div = large;
    l_div /= divisor_i32;
    assert(l_div == res_i32->first);
    l_mod = large;
    l_mod %= divisor_i32;
    assert(l_mod == res_i32->second);

    // Divisor como entero de 16 bits sin signo
    uint16_t divisor_u16 = 3U;

    [[maybe_unused]] auto res_u16 = large.divrem(divisor_u16);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u16->first * divisor_u16 + res_u16->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u16->second < divisor_u16);

    l_div = large;
    l_div /= divisor_u16;
    assert(l_div == res_u16->first);
    l_mod = large;
    l_mod %= divisor_u16;
    assert(l_mod == res_u16->second);

    // Divisor como entero de 16 bits con signo
    int16_t divisor_i16 = 3;

    [[maybe_unused]] auto res_i16 = large.divrem(divisor_i16);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i16->first * divisor_i16 + res_i16->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i16->second < divisor_i16);

    l_div = large;
    l_div /= divisor_i16;
    assert(l_div == res_i16->first);
    l_mod = large;
    l_mod %= divisor_i16;
    assert(l_mod == res_i16->second);

    // Divisor como entero de 8 bits sin signo
    uint8_t divisor_u8 = 3U;

    [[maybe_unused]] auto res_u8 = large.divrem(divisor_u8);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u8->first * divisor_u8 + res_u8->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u8->second < divisor_u8);

    l_div = large;
    l_div /= divisor_u8;
    assert(l_div == res_u8->first);
    l_mod = large;
    l_mod %= divisor_u8;
    assert(l_mod == res_u8->second);

    // Divisor como entero de 8 bits con signo
    int8_t divisor_i8 = 3;

    [[maybe_unused]] auto res_i8 = large.divrem(divisor_i8);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i8->first * divisor_i8 + res_i8->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i8->second < divisor_i8);

    l_div = large;
    l_div /= divisor_i8;
    assert(l_div == res_i8->first);
    l_mod = large;
    l_mod %= divisor_i8;
    assert(l_mod == res_i8->second);

    std::cout << "test_divrem: test_divrem_large_integral_divisor passed" << std::endl;
}

void test_divrem_random()
{
    std::cout << "test_divrem: test_divrem_random ......" << std::endl;
    std::mt19937_64 rng(std::random_device{}());

    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        if (b == 0_u128)
            b = 1_u128;

        [[maybe_unused]] auto res = a.divrem(b);
        assert(res.has_value());
        uint128_t q = res->first;
        uint128_t r = res->second;
        assert(r < b);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b > 1_u128 && a > 0_u128) {
            assert(q < a);
        }

        // Check operators
        uint128_t a_div = a;
        a_div /= b;
        assert(a_div == q);

        uint128_t a_mod = a;
        a_mod %= b;
        assert(a_mod == r);

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r;

        // 1. (a - r) debe ser divisible por b y dar q
        [[maybe_unused]] auto check1 = a_minus_r.divrem(b);
        assert(check1.has_value());
        assert(check1->first == q);
        assert(check1->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q > 0_u128) {
            [[maybe_unused]] auto check2 = a_minus_r.divrem(q);
            assert(check2.has_value());
            assert(check2->first == b);
            assert(check2->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            [[maybe_unused]] auto check3 = (a - 1_u128).divrem(b);
            assert(check3.has_value());
            if (r != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3->first == q);
                assert(check3->second == r - 1_u128);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                assert(check3->first == q - 1_u128);
                assert(check3->second == b - 1_u128);
            }
        }
    }
    std::cout << "test_divrem: test_divrem_random passed" << std::endl;
}

void test_divrem_random_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_random_integral_divisor ......" << std::endl;
    std::mt19937_64 rng(std::random_device{}());

    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());

        // Divisor como uint64_t
        {
            auto b_u64(static_cast<uint64_t>(rng()));

            if (b_u64 == 0_u128)
                b_u64 = 1ULL;

            [[maybe_unused]] auto res_u64 = a.divrem(b_u64);
            assert(res_u64.has_value());
            uint128_t q_u64 = res_u64->first;
            uint128_t r_u64 = res_u64->second;
            assert(r_u64 < b_u64);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_u64 > 1 && a > 0_u128) {
                assert(q_u64 < a);
            }

            // Check operators
            uint128_t a_div_u64 = a;
            a_div_u64 /= b_u64;
            assert(a_div_u64 == q_u64);

            uint128_t a_mod_u64 = a;
            a_mod_u64 %= b_u64;
            assert(a_mod_u64 == r_u64);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_u64;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_u64 = a_minus_r.divrem(b_u64);
            assert(check1_u64.has_value());
            assert(check1_u64->first == q_u64);
            assert(check1_u64->second == 0ULL);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_u64 > 0_u128) {
                [[maybe_unused]] auto check2_u64 = a_minus_r.divrem(q_u64);
                assert(check2_u64.has_value());
                assert(check2_u64->first == b_u64);
                assert(check2_u64->second == 0ULL);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_u64 = (a - 1_u128).divrem(b_u64);
                assert(check3_u64.has_value());
                if (r_u64 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_u64->first == q_u64);
                    assert(check3_u64->second == r_u64 - 1_u128);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    assert(check3_u64->first == q_u64 - 1_u128);
                    assert(check3_u64->second == b_u64 - 1_u128);
                }
            }
        }

        // Divisor como int64_t
        {
            auto b_i64(static_cast<int64_t>(rng()));

            if (b_i64 == 0)
                b_i64 = 1;
            [[maybe_unused]] auto res_i64 = a.divrem(b_i64);
            assert(res_i64.has_value());
            uint128_t q_i64 = res_i64->first;
            uint128_t r_i64 = res_i64->second;
            assert(r_i64 < b_i64);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_i64 > 1 && a > 0_u128) {
                assert(q_i64 < a);
            }

            // Check operators
            uint128_t a_div_i64 = a;
            a_div_i64 /= b_i64;
            assert(a_div_i64 == q_i64);

            uint128_t a_mod_i64 = a;
            a_mod_i64 %= b_i64;
            assert(a_mod_i64 == r_i64);
            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_i64;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_i64 = a_minus_r.divrem(b_i64);
            assert(check1_i64.has_value());
            assert(check1_i64->first == q_i64);
            assert(check1_i64->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_i64 > 0_u128) {
                [[maybe_unused]] auto check2_i64 = a_minus_r.divrem(q_i64);
                assert(check2_i64.has_value());
                assert(check2_i64->first == b_i64);
                assert(check2_i64->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_i64 = (a - 1_u128).divrem(b_i64);
                assert(check3_i64.has_value());
                if (r_i64 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_i64->first == q_i64);
                    assert(check3_i64->second == r_i64 - 1_u128);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_i64 = q_i64;
                    --q2_i64;
                    [[maybe_unused]] auto b2_i64 = b_i64;
                    --b2_i64;
                    assert(check3_i64->first == q2_i64);
                    assert(check3_i64->second == b2_i64);
                }
            }
        }

        // Divisor como uint32_t
        {
            auto b_u32(static_cast<uint32_t>(rng()));

            if (b_u32 == 0)
                b_u32 = 1;
            [[maybe_unused]] auto res_u32 = a.divrem(b_u32);
            assert(res_u32.has_value());
            uint128_t q_u32 = res_u32->first;
            uint128_t r_u32 = res_u32->second;
            assert(r_u32 < b_u32);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_u32 > 1 && a > 0_u128) {
                assert(q_u32 < a);
            }

            // Check operators
            uint128_t a_div_u32 = a;
            a_div_u32 /= b_u32;
            assert(a_div_u32 == q_u32);

            uint128_t a_mod_u32 = a;
            a_mod_u32 %= b_u32;
            assert(a_mod_u32 == r_u32);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_u32;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_u32 = a_minus_r.divrem(b_u32);
            assert(check1_u32.has_value());
            assert(check1_u32->first == q_u32);
            assert(check1_u32->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_u32 > 0_u128) {
                [[maybe_unused]] auto check2_u32 = a_minus_r.divrem(q_u32);
                assert(check2_u32.has_value());
                assert(check2_u32->first == b_u32);
                assert(check2_u32->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_u32 = (a - 1_u128).divrem(b_u32);
                assert(check3_u32.has_value());
                if (r_u32 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_u32->first == q_u32);
                    auto r2_u32 = r_u32;
                    --r2_u32;
                    assert(check3_u32->second == r2_u32);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_u32 = q_u32;
                    --q2_u32;
                    [[maybe_unused]] auto b2_u32 = b_u32;
                    --b2_u32;
                    assert(check3_u32->first == q2_u32);
                    assert(check3_u32->second == b2_u32);
                }
            }
        }

        // Divisor como int32_t
        {
            auto b_i32(static_cast<int32_t>(rng()));

            if (b_i32 == 0)
                b_i32 = 1;
            [[maybe_unused]] auto res_i32 = a.divrem(b_i32);
            assert(res_i32.has_value());
            uint128_t q_i32 = res_i32->first;
            uint128_t r_i32 = res_i32->second;
            assert(r_i32 < b_i32);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_i32 > 1 && a > 0_u128) {
                assert(q_i32 < a);
            }

            // Check operators
            uint128_t a_div_i32 = a;
            a_div_i32 /= b_i32;
            assert(a_div_i32 == q_i32);

            uint128_t a_mod_i32 = a;
            a_mod_i32 %= b_i32;
            assert(a_mod_i32 == r_i32);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_i32;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_i32 = a_minus_r.divrem(b_i32);
            assert(check1_i32.has_value());
            assert(check1_i32->first == q_i32);
            assert(check1_i32->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_i32 > 0_u128) {
                [[maybe_unused]] auto check2_i32 = a_minus_r.divrem(q_i32);
                assert(check2_i32.has_value());
                assert(check2_i32->first == b_i32);
                assert(check2_i32->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_i32 = (a - 1_u128).divrem(b_i32);
                assert(check3_i32.has_value());
                if (r_i32 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_i32->first == q_i32);
                    auto r2_i32 = r_i32;
                    --r2_i32;
                    assert(check3_i32->second == r2_i32);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_i32 = q_i32;
                    --q2_i32;
                    [[maybe_unused]] auto b2_i32 = b_i32;
                    --b2_i32;
                    assert(check3_i32->first == q2_i32);
                    assert(check3_i32->second == b2_i32);
                }
            }
        }

        // Divisor como uint16_t
        {
            auto b_u16(static_cast<uint16_t>(rng()));

            if (b_u16 == 0)
                b_u16 = 1;
            [[maybe_unused]] auto res_u16 = a.divrem(b_u16);
            assert(res_u16.has_value());
            uint128_t q_u16 = res_u16->first;
            uint128_t r_u16 = res_u16->second;
            assert(r_u16 < b_u16);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_u16 > 1 && a > 0_u128) {
                assert(q_u16 < a);
            }

            // Check operators
            uint128_t a_div_u16 = a;
            a_div_u16 /= b_u16;
            assert(a_div_u16 == q_u16);

            uint128_t a_mod_u16 = a;
            a_mod_u16 %= b_u16;
            assert(a_mod_u16 == r_u16);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_u16;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_u16 = a_minus_r.divrem(b_u16);
            assert(check1_u16.has_value());
            assert(check1_u16->first == q_u16);
            assert(check1_u16->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_u16 > 0_u128) {
                [[maybe_unused]] auto check2_u16 = a_minus_r.divrem(q_u16);
                assert(check2_u16.has_value());
                assert(check2_u16->first == b_u16);
                assert(check2_u16->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_u16 = (a - 1_u128).divrem(b_u16);
                assert(check3_u16.has_value());
                if (r_u16 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_u16->first == q_u16);
                    auto r2_u16 = r_u16;
                    --r2_u16;
                    assert(check3_u16->second == r2_u16);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_u16 = q_u16;
                    --q2_u16;
                    [[maybe_unused]] auto b2_u16 = b_u16;
                    --b2_u16;
                    assert(check3_u16->first == q2_u16);
                    assert(check3_u16->second == b2_u16);
                }
            }
        }

        // Divisor como int16_t
        {
            auto b_i16(static_cast<int16_t>(rng()));

            if (b_i16 == 0)
                b_i16 = 1;
            [[maybe_unused]] auto res_i16 = a.divrem(b_i16);
            assert(res_i16.has_value());
            uint128_t q_i16 = res_i16->first;
            uint128_t r_i16 = res_i16->second;
            assert(r_i16 < b_i16);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_i16 > 1 && a > 0_u128) {
                assert(q_i16 < a);
            }

            // Check operators
            uint128_t a_div_i16 = a;
            a_div_i16 /= b_i16;
            assert(a_div_i16 == q_i16);

            uint128_t a_mod_i16 = a;
            a_mod_i16 %= b_i16;
            assert(a_mod_i16 == r_i16);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_i16;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_i16 = a_minus_r.divrem(b_i16);
            assert(check1_i16.has_value());
            assert(check1_i16->first == q_i16);
            assert(check1_i16->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_i16 > 0_u128) {
                [[maybe_unused]] auto check2_i16 = a_minus_r.divrem(q_i16);
                assert(check2_i16.has_value());
                assert(check2_i16->first == b_i16);
                assert(check2_i16->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_i16 = (a - 1_u128).divrem(b_i16);
                assert(check3_i16.has_value());
                if (r_i16 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_i16->first == q_i16);
                    auto r2_i16 = r_i16;
                    --r2_i16;
                    assert(check3_i16->second == r2_i16);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_i16 = q_i16;
                    --q2_i16;
                    [[maybe_unused]] auto b2_i16 = b_i16;
                    --b2_i16;
                    assert(check3_i16->first == q2_i16);
                    assert(check3_i16->second == b2_i16);
                }
            }
        }

        // Divisor como uint8_t
        {
            auto b_u8(static_cast<uint8_t>(rng()));

            if (b_u8 == 0)
                b_u8 = 1;
            [[maybe_unused]] auto res_u8 = a.divrem(b_u8);
            assert(res_u8.has_value());
            uint128_t q_u8 = res_u8->first;
            uint128_t r_u8 = res_u8->second;
            assert(r_u8 < b_u8);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_u8 > 1 && a > 0_u128) {
                assert(q_u8 < a);
            }

            // Check operators
            uint128_t a_div_u8 = a;
            a_div_u8 /= b_u8;
            assert(a_div_u8 == q_u8);

            uint128_t a_mod_u8 = a;
            a_mod_u8 %= b_u8;
            assert(a_mod_u8 == r_u8);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_u8;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_u8 = a_minus_r.divrem(b_u8);
            assert(check1_u8.has_value());
            assert(check1_u8->first == q_u8);
            assert(check1_u8->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_u8 > 0_u128) {
                [[maybe_unused]] auto check2_u8 = a_minus_r.divrem(q_u8);
                assert(check2_u8.has_value());
                assert(check2_u8->first == b_u8);
                assert(check2_u8->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_u8 = (a - 1_u128).divrem(b_u8);
                assert(check3_u8.has_value());
                if (r_u8 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_u8->first == q_u8);
                    auto r2_u8 = r_u8;
                    --r2_u8;
                    assert(check3_u8->second == r2_u8);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_u8 = q_u8;
                    --q2_u8;
                    [[maybe_unused]] auto b2_u8 = b_u8;
                    --b2_u8;
                    assert(check3_u8->first == q2_u8);
                    assert(check3_u8->second == b2_u8);
                }
            }
        }

        // Divisor como int8_t
        {
            auto b_i8(static_cast<int8_t>(rng()));

            if (b_i8 == 0)
                b_i8 = 1;
            [[maybe_unused]] auto res_i8 = a.divrem(b_i8);
            assert(res_i8.has_value());
            uint128_t q_i8 = res_i8->first;
            uint128_t r_i8 = res_i8->second;
            assert(r_i8 < b_i8);

            // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el
            // dividendo.
            if (b_i8 > 1 && a > 0_u128) {
                assert(q_i8 < a);
            }

            // Check operators
            uint128_t a_div_i8 = a;
            a_div_i8 /= b_i8;
            assert(a_div_i8 == q_i8);

            uint128_t a_mod_i8 = a;
            a_mod_i8 %= b_i8;
            assert(a_mod_i8 == r_i8);

            // Verificación sin asumir multiplicación completa:
            // a = q * b + r  =>  a - r = q * b
            uint128_t a_minus_r = a - r_i8;

            // 1. (a - r) debe ser divisible por b y dar q
            [[maybe_unused]] auto check1_i8 = a_minus_r.divrem(b_i8);
            assert(check1_i8.has_value());
            assert(check1_i8->first == q_i8);
            assert(check1_i8->second == 0_u128);

            // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
            if (q_i8 > 0_u128) {
                [[maybe_unused]] auto check2_i8 = a_minus_r.divrem(q_i8);
                assert(check2_i8.has_value());
                assert(check2_i8->first == b_i8);
                assert(check2_i8->second == 0_u128);
            }

            // 3. Comprobación con (a-1) (check3)
            if (a > 0_u128) {
                [[maybe_unused]] auto check3_i8 = (a - 1_u128).divrem(b_i8);
                assert(check3_i8.has_value());
                if (r_i8 != 0_u128) {
                    // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                    assert(check3_i8->first == q_i8);
                    auto r2_i8 = r_i8;
                    --r2_i8;
                    assert(check3_i8->second == r2_i8);
                } else // r == 0
                {
                    // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                    auto q2_i8 = q_i8;
                    --q2_i8;
                    [[maybe_unused]] auto b2_i8 = b_i8;
                    --b2_i8;
                    assert(check3_i8->first == q2_i8);
                    assert(check3_i8->second == b2_i8);
                }
            }
        }
    }
    std::cout << "test_divrem: test_divrem_random_integral_divisor passed" << std::endl;
}

void test_divrem_known_result()
{
    std::cout << "test_divrem: test_divrem_known_result ......" << std::endl;

    // 100 / 3 = 33 rem 1
    [[maybe_unused]] auto res1 = (100_u128).divrem(3_u128);
    assert(res1->first == 33_u128);
    assert(res1->second == 1_u128);
    uint128_t a1 = 100_u128;
    a1 /= 3_u128;
    assert(a1 == 33_u128);
    uint128_t m1 = 100_u128;
    m1 %= 3_u128;
    assert(m1 == 1_u128);

    // Max / 2 = (2^128 - 1) / 2 = 2^127 - 1 rem 1
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    [[maybe_unused]] auto res2 = max_val.divrem(2_u128);
    assert(res2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res2->second == 1_u128);
    uint128_t a2 = max_val;
    a2 /= 2_u128;
    assert(a2 == res2->first);
    uint128_t m2 = max_val;
    m2 %= 2_u128;
    assert(m2 == 1_u128);

    std::cout << "test_divrem: test_divrem_known_result passed" << std::endl;
}

void test_divrem_known_result_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_known_result_integral_divisor ......" << std::endl;

    // divisor como uint64_t

    // 100 / 3 (int)
    [[maybe_unused]] auto res_u64 = (100_u128).divrem(static_cast<uint64_t>(3u));
    assert(res_u64->first == 33_u128);
    assert(res_u64->second == 1_u128);
    uint128_t a1 = 100_u128;
    a1 /= static_cast<uint64_t>(3u);
    assert(a1 == 33_u128);
    uint128_t m1 = 100_u128;
    m1 %= static_cast<uint64_t>(3u);
    assert(m1 == 1_u128);

    // Max / 2 (uint64_t)
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    [[maybe_unused]] auto res_u64_2 = max_val.divrem(static_cast<uint64_t>(2u));
    assert(res_u64_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_u64_2->second == 1_u128);
    uint128_t a2 = max_val;
    a2 /= static_cast<uint64_t>(2u);
    assert(a2 == res_u64_2->first);
    uint128_t m2 = max_val;
    m2 %= static_cast<uint64_t>(2u);
    assert(m2 == 1_u128);

    // divisor como int64_t

    // 100 / 3 (int)
    [[maybe_unused]] auto res_i64 = (100_u128).divrem(static_cast<int64_t>(3));
    assert(res_i64->first == 33_u128);
    assert(res_i64->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<int64_t>(3);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<int64_t>(3);
    assert(m1 == 1_u128);

    // Max / 2 (int64_t)
    [[maybe_unused]] auto res_i64_2 = max_val.divrem(static_cast<int64_t>(2));
    assert(res_i64_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_i64_2->second == 1_u128);
    a2 = max_val;
    a2 /= static_cast<int64_t>(2);
    assert(a2 == res_i64_2->first);
    m2 = max_val;
    m2 %= static_cast<int64_t>(2);
    assert(m2 == 1_u128);

    // divisor como uint32_t
    [[maybe_unused]] auto res_u32 = (100_u128).divrem(static_cast<uint32_t>(3u));
    assert(res_u32->first == 33_u128);
    assert(res_u32->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<uint32_t>(3u);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<uint32_t>(3u);
    assert(m1 == 1_u128);

    // divisor como int32_t
    [[maybe_unused]] auto res_i32 = (100_u128).divrem(static_cast<int32_t>(3));
    assert(res_i32->first == 33_u128);
    assert(res_i32->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<int32_t>(3);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<int32_t>(3);
    assert(m1 == 1_u128);

    // divisor como uint16_t
    [[maybe_unused]] auto res_u16 = (100_u128).divrem(static_cast<uint16_t>(3u));
    assert(res_u16->first == 33_u128);
    assert(res_u16->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<uint16_t>(3u);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<uint16_t>(3u);
    assert(m1 == 1_u128);

    // divisor como int16_t
    [[maybe_unused]] auto res_i16 = (100_u128).divrem(static_cast<int16_t>(3));
    assert(res_i16->first == 33_u128);
    assert(res_i16->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<int16_t>(3);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<int16_t>(3);
    assert(m1 == 1_u128);

    // divisor como uint8_t
    [[maybe_unused]] auto res_u8 = (100_u128).divrem(static_cast<uint8_t>(3u));
    assert(res_u8->first == 33_u128);
    assert(res_u8->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<uint8_t>(3u);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<uint8_t>(3u);
    assert(m1 == 1_u128);

    // divisor como int8_t
    [[maybe_unused]] auto res_i8 = (100_u128).divrem(static_cast<int8_t>(3));
    assert(res_i8->first == 33_u128);
    assert(res_i8->second == 1_u128);
    a1 = 100_u128;
    a1 /= static_cast<int8_t>(3);
    assert(a1 == 33_u128);
    m1 = 100_u128;
    m1 %= static_cast<int8_t>(3);
    assert(m1 == 1_u128);

    assert(m2 == 1_u128);

    std::cout << "test_divrem: test_divrem_known_result_integral_divisor passed" << std::endl;
}

void test_mult_assignment_operator()
{
    std::cout << "test_mult_assignment_operator ......" << std::endl;

    // Test 1: Identidad multiplicativa (a *= 1 no cambia a)
    for (int i = 0; i < 100; ++i) {
        uint128_t a(rng(), rng());
        uint128_t a_copy = a;
        a_copy *= uint128_t(1);
        assert(a_copy == a);
    }

    // Test 2: Elemento cero (a *= 0 = 0)
    for (int i = 0; i < 100; ++i) {
        uint128_t a(rng(), rng());
        a *= uint128_t(0);
        assert(a == uint128_t(0));
    }

    // Test 3: Conmutatividad (a * b == b * a)
    for (int i = 0; i < 100; ++i) {
        // Usar valores más pequeños para evitar overflow
        uint128_t a(0, rng() % 0xFFFFFFFFULL);
        uint128_t b(0, rng() % 0xFFFFFFFFULL);

        uint128_t prod1 = a;
        prod1 *= b;

        uint128_t prod2 = b;
        prod2 *= a;

        assert(prod1 == prod2);
    }

    // Test 4: Verificación con división (estrategia de líneas 1300-1317)
    // Si a * b = c, entonces c / b = a (si b != 0)
    for (int i = 0; i < 100; ++i) {
        // Usar valores pequeños para evitar overflow y asegurar que podamos dividir
        uint64_t a_val = (rng() % 0xFFFFULL) + 1; // Evitar 0
        uint64_t b_val = (rng() % 0xFFFFULL) + 1; // Evitar 0

        [[maybe_unused]] uint128_t a(a_val);
        [[maybe_unused]] uint128_t b(b_val);

        uint128_t c = a;
        c *= b;

        // Verificación 1: c / b debe dar a
        [[maybe_unused]] auto div_result_b = c.divrem(b);
        assert(div_result_b.has_value());
        assert(div_result_b->first == a);
        assert(div_result_b->second == uint128_t(0)); // Sin resto

        // Verificación 2: c / a debe dar b
        [[maybe_unused]] auto div_result_a = c.divrem(a);
        assert(div_result_a.has_value());
        assert(div_result_a->first == b);
        assert(div_result_a->second == uint128_t(0)); // Sin resto
    }

    // Test 5: Asociatividad ((a * b) * c == a * (b * c))
    for (int i = 0; i < 100; ++i) {
        // Valores aún más pequeños para evitar overflow con 3 factores
        uint128_t a(0, (rng() % 0xFFFFULL) + 1);
        uint128_t b(0, (rng() % 0xFFFFULL) + 1);
        uint128_t c(0, (rng() % 0xFFFFULL) + 1);

        uint128_t ab = a;
        ab *= b;
        uint128_t ab_c = ab;
        ab_c *= c;

        uint128_t bc = b;
        bc *= c;
        uint128_t a_bc = a;
        a_bc *= bc;

        assert(ab_c == a_bc);
    }

    // Test 6: Casos conocidos específicos
    {
        // 2 * 3 = 6
        [[maybe_unused]] uint128_t val = uint128_t(2);
        val *= uint128_t(3);
        assert(val == uint128_t(6));

        // Verificar con división
        [[maybe_unused]] auto check = val.divrem(uint128_t(2));
        assert(check->first == uint128_t(3));
        assert(check->second == uint128_t(0));
    }

    {
        // 100 * 200 = 20000
        [[maybe_unused]] uint128_t val = uint128_t(100);
        val *= uint128_t(200);
        assert(val == uint128_t(20000));

        // Verificar con división
        [[maybe_unused]] auto check1 = val.divrem(uint128_t(100));
        assert(check1->first == uint128_t(200));
        assert(check1->second == uint128_t(0));

        [[maybe_unused]] auto check2 = val.divrem(uint128_t(200));
        assert(check2->first == uint128_t(100));
        assert(check2->second == uint128_t(0));
    }

    {
        // 2^32 * 2^32 = 2^64
        [[maybe_unused]] uint128_t val(0, 1ULL << 32);
        val *= uint128_t(0, 1ULL << 32);
        assert(val == uint128_t(1, 0)); // 2^64 = (1, 0)

        // Verificar con división
        uint128_t divisor(0, 1ULL << 32);
        [[maybe_unused]] auto check = val.divrem(divisor);
        assert(check->first == divisor);
        assert(check->second == uint128_t(0));
    }

    // Test 7: Multiplicación con tipos integrales
    for (int i = 0; i < 100; ++i) {
        uint128_t a(0, rng() % 0xFFFFFFFFULL);
        uint64_t b_u64 = (rng() % 0xFFFFULL) + 1;

        [[maybe_unused]] uint128_t result = a;
        result *= b_u64;

        // Verificar con división
        [[maybe_unused]] auto check = result.divrem(b_u64);
        assert(check.has_value());
        assert(check->first == a);
        assert(check->second == uint128_t(0));
    }

    // Test 8: Distributividad: a * (b + c) == a * b + a * c
    for (int i = 0; i < 100; ++i) {
        uint128_t a(0, (rng() % 0xFFFFULL) + 1);
        uint128_t b(0, (rng() % 0xFFFFULL) + 1);
        uint128_t c(0, (rng() % 0xFFFFULL) + 1);

        [[maybe_unused]] uint128_t left = a * (b + c);
        [[maybe_unused]] uint128_t right = a * b + a * c;

        assert(left == right);
    }

    // Test 9: Multiplicación por potencias de 2 (equivalente a shift)
    for (int shift = 0; shift < 64; ++shift) {
        [[maybe_unused]] uint128_t a(0, 12345);
        [[maybe_unused]] uint128_t b(0, 1ULL << shift);

        uint128_t prod = a;
        prod *= b;

        [[maybe_unused]] uint128_t shifted = a << shift;

        assert(prod == shifted);
    }

    // Test 10: Overflow controlado (2^127 * 2 = 2^128 = 0 con wrapping)
    {
        uint128_t val(1ULL << 63, 0); // 2^127
        val *= uint128_t(2);
        assert(val == uint128_t(0)); // Overflow completo
    }

    std::cout << "test_mult_assignment_operator passed" << std::endl;
}

void test_mult_operator()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());
        uint128_t c(rng(), rng());

        // Conmutatividad
        assert(a * b == b * a);

        // Asociatividad
        assert((a * b) * c == a * (b * c));

        // Identidad
        assert(a * uint128_t(1) == a);

        // Elemento cero
        assert(a * uint128_t(0) == uint128_t(0));

        // Consistencia con *=
        uint128_t prod = a;
        prod *= b;
        assert(prod == a * b);
    }

    std::cout << "test_mult_operator passed" << std::endl;
}

void test_div_operator()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        if (b == uint128_t(0)) {
            // Division by zero returns 0 in this implementation
            assert((a / b) == uint128_t(0));
            continue;
        }

        // Identity
        assert((a / uint128_t(1)) == a);

        // Self division
        if (a != uint128_t(0)) {
            assert((a / a) == uint128_t(1));
        }

        // Consistency with /=
        uint128_t quot = a;
        quot /= b;
        assert(quot == (a / b));
    }

    std::cout << "test_div_operator passed" << std::endl;
}

void test_mod_operator()
{
    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        if (b == uint128_t(0)) {
            // Modulo by zero returns 0 in this implementation
            assert((a % b) == uint128_t(0));
            continue;
        }

        // Modulo 1
        assert((a % uint128_t(1)) == uint128_t(0));

        // Consistency with %=
        uint128_t rem = a;
        rem %= b;
        assert(rem == (a % b));

        // Range check
        assert((a % b) < b);

        // Fundamental division property: a = (a/b)*b + (a%b)
        assert((a / b) * b + (a % b) == a);
    }

    std::cout << "test_mod_operator passed" << std::endl;
}

void test_fullmult_times_uint64()
{
    // Test basic multiplication
    [[maybe_unused]] uint128_t val(0, 10);
    [[maybe_unused]] uint64_t mult = 10;
    // 10 * 10 = 100. High part of 192-bit result is 0.
    assert(val.fullmult_times_uint64(mult) == 0);

    // Test with large numbers to ensure high part is used
    // (2^64 - 1) * (2^64 - 1) = 2^128 - 2^65 + 1
    // This fits in 128 bits, so high part of 192 bits (bits 128-191) is 0.
    [[maybe_unused]] uint128_t max64(0, UINT64_MAX);
    assert(max64.fullmult_times_uint64(UINT64_MAX) == 0);

    // We need a case where the result exceeds 128 bits.
    // val = 2^127 (high bit set)
    // mult = 2
    // result = 2^128.
    // 2^128 in 192 bits: bit 128 is 1.
    // So fullmult should return 1.
    [[maybe_unused]] uint128_t large(0x8000000000000000ULL, 0);
    assert(large.fullmult_times_uint64(2) == 1);

    std::cout << "test_fullmult_times_uint64 passed" << std::endl;
}

void test_knuth_D_divrem()
{
    // Basic test
    [[maybe_unused]] uint128_t a(0, 100);
    [[maybe_unused]] uint128_t b(0, 3);
    [[maybe_unused]] auto res = a.knuth_D_divrem(b);
    assert(res.has_value());
    assert(res->first == uint128_t(0, 33));
    assert(res->second == uint128_t(0, 1));

    // Test optimization paths (e.g. power of 2)
    uint128_t c(0, 100);
    uint128_t d(0, 4);
    [[maybe_unused]] auto res2 = c.knuth_D_divrem(d);
    assert(res2.has_value());
    assert(res2->first == uint128_t(0, 25));
    assert(res2->second == uint128_t(0, 0));

    std::cout << "test_knuth_D_divrem passed" << std::endl;
}

void test_knuth_D_divrem_integral()
{
    [[maybe_unused]] uint128_t a(0, 100);
    uint64_t b = 3;
    [[maybe_unused]] auto res = a.knuth_D_divrem(b);
    assert(res.has_value());
    assert(res->first == uint128_t(0, 33));
    assert(res->second == uint128_t(0, 1));
    std::cout << "test_knuth_D_divrem_integral passed" << std::endl;
}

void test_to_string()
{
    [[maybe_unused]] uint128_t val(0, 12345);
    assert(val.to_string() == "12345");
    [[maybe_unused]] uint128_t val2(1, 0); // 2^64
    assert(val2.to_string() == "18446744073709551616");
    std::cout << "test_to_string passed" << std::endl;
}

void test_to_string_base()
{
    [[maybe_unused]] uint128_t val(0, 255);
    assert(val.to_string_base(16) == "FF");
    assert(val.to_string_base(2) == "11111111");
    assert(val.to_string_base(8) == "377");
    std::cout << "test_to_string_base passed" << std::endl;
}

void test_to_string_hex()
{
    [[maybe_unused]] uint128_t val(0, 255);
    assert(val.to_string_hex() == "FF");
    assert(val.to_string_hex(true) == "0xFF");
    std::cout << "test_to_string_hex passed" << std::endl;
}

void test_to_string_bin()
{
    [[maybe_unused]] uint128_t val(0, 15);
    assert(val.to_string_bin() == "1111");
    assert(val.to_string_bin(true) == "0b1111");
    std::cout << "test_to_string_bin passed" << std::endl;
}

void test_to_string_oct()
{
    [[maybe_unused]] uint128_t val(0, 8);
    assert(val.to_string_oct() == "10");
    assert(val.to_string_oct(true) == "010");
    std::cout << "test_to_string_oct passed" << std::endl;
}

void test_from_cstr()
{
    assert(uint128_t::from_cstr("123") == uint128_t(0, 123));
    assert(uint128_t::from_cstr("0xFF") == uint128_t(0, 255));
    assert(uint128_t::from_cstr("0b101") == uint128_t(0, 5));
    std::cout << "test_from_cstr passed" << std::endl;
}

void test_from_cstr_base()
{
    assert(uint128_t::from_cstr_base("FF", 16) == uint128_t(0, 255));
    assert(uint128_t::from_cstr_base("101", 2) == uint128_t(0, 5));
    std::cout << "test_from_cstr_base passed" << std::endl;
}

void test_to_cstr()
{
    [[maybe_unused]] uint128_t val(0, 123);
    std::string s = val.to_cstr();
    assert(s == "123");
    std::cout << "test_to_cstr passed" << std::endl;
}

void test_to_cstr_base()
{
    [[maybe_unused]] uint128_t val(0, 255);
    std::string s = val.to_cstr_base(16);
    assert(s == "FF");
    std::cout << "test_to_cstr_base passed" << std::endl;
}

void test_to_cstr_hex()
{
    [[maybe_unused]] uint128_t val(0, 255);
    std::string s = val.to_cstr_hex();
    assert(s == "FF");
    std::cout << "test_to_cstr_hex passed" << std::endl;
}

void test_to_cstr_bin()
{
    [[maybe_unused]] uint128_t val(0, 5);
    std::string s = val.to_cstr_bin();
    assert(s == "101");
    std::cout << "test_to_cstr_bin passed" << std::endl;
}

void test_to_cstr_oct()
{
    [[maybe_unused]] uint128_t val(0, 8);
    std::string s = val.to_cstr_oct();
    assert(s == "10");
    std::cout << "test_to_cstr_oct passed" << std::endl;
}

void test_from_string()
{
    std::string s = "123";
    assert(uint128_t::from_string(s) == uint128_t(0, 123));
    std::cout << "test_from_string passed" << std::endl;
}

void test_from_string_base()
{
    std::string s = "FF";
    assert(uint128_t::from_string_base(s, 16) == uint128_t(0, 255));
    std::cout << "test_from_string_base passed" << std::endl;
}

// ========================= COPY AND MOVE SEMANTICS =========================

void test_copy_constructor()
{
    std::cout << "test_copy_constructor: ";

    // Test basic copy
    uint128_t original(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    uint128_t copy(original);

    assert(copy.high() == original.high());
    assert(copy.low() == original.low());
    assert(copy == original);

    // Test that modifications to copy don't affect original
    copy += uint128_t(1);
    assert(copy != original);
    assert(original.low() == 0xFEDCBA0987654321ULL);

    // Test copy of zero
    [[maybe_unused]] uint128_t zero;
    [[maybe_unused]] uint128_t zero_copy(zero);
    assert(zero_copy == uint128_t(0));

    // Test copy of max value
    [[maybe_unused]] uint128_t max_val(UINT64_MAX, UINT64_MAX);
    [[maybe_unused]] uint128_t max_copy(max_val);
    assert(max_copy == max_val);

    std::cout << "passed" << std::endl;
}

void test_move_constructor()
{
    std::cout << "test_move_constructor: ";

    // Test basic move
    uint128_t original(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    [[maybe_unused]] uint64_t orig_high = original.high();
    [[maybe_unused]] uint64_t orig_low = original.low();

    [[maybe_unused]] uint128_t moved(std::move(original));

    assert(moved.high() == orig_high);
    assert(moved.low() == orig_low);

    // Test moving from temporary
    [[maybe_unused]] uint128_t from_temp(uint128_t(0xAAAA, 0xBBBB));
    assert(from_temp.high() == 0xAAAA);
    assert(from_temp.low() == 0xBBBB);

    std::cout << "passed" << std::endl;
}

void test_copy_assignment()
{
    std::cout << "test_copy_assignment: ";

    // Test basic copy assignment
    uint128_t original(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    uint128_t target;

    target = original;

    assert(target.high() == original.high());
    assert(target.low() == original.low());
    assert(target == original);

    // Test that modifications to target don't affect original
    target += uint128_t(1);
    assert(target != original);

    // Test self-assignment
    uint128_t self(0x1111, 0x2222);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#endif
    self = self;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    assert(self.high() == 0x1111);
    assert(self.low() == 0x2222);

    // Test chained assignment
    uint128_t a, b, c;
    a = b = c = uint128_t(0x9999, 0x8888);
    assert(a == uint128_t(0x9999, 0x8888));
    assert(b == uint128_t(0x9999, 0x8888));
    assert(c == uint128_t(0x9999, 0x8888));

    std::cout << "passed" << std::endl;
}

void test_move_assignment()
{
    std::cout << "test_move_assignment: ";

    // Test basic move assignment
    uint128_t original(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    [[maybe_unused]] uint64_t orig_high = original.high();
    [[maybe_unused]] uint64_t orig_low = original.low();

    uint128_t target;
    target = std::move(original);

    assert(target.high() == orig_high);
    assert(target.low() == orig_low);

    // Test moving from temporary
    [[maybe_unused]] uint128_t from_temp;
    from_temp = uint128_t(0xCCCC, 0xDDDD);
    assert(from_temp.high() == 0xCCCC);
    assert(from_temp.low() == 0xDDDD);

    std::cout << "passed" << std::endl;
}

void test_vector_of_uint128()
{
    std::cout << "test_vector_of_uint128: ";

    // Test that uint128_t works correctly with STL containers
    std::vector<uint128_t> vec;

    // Push back some values
    vec.push_back(uint128_t(1, 2));
    vec.push_back(uint128_t(3, 4));
    vec.push_back(uint128_t(5, 6));

    assert(vec.size() == 3);
    assert(vec[0] == uint128_t(1, 2));
    assert(vec[1] == uint128_t(3, 4));
    assert(vec[2] == uint128_t(5, 6));

    // Test resize
    vec.resize(10);
    assert(vec.size() == 10);
    assert(vec[9] == uint128_t(0)); // Default constructed

    // Test copy in vector
    std::vector<uint128_t> vec_copy = vec;
    assert(vec_copy.size() == vec.size());
    assert(vec_copy[0] == vec[0]);

    std::cout << "passed" << std::endl;
}

// ========================= USER-DEFINED LITERALS =========================

void test_udl_integer()
{
    std::cout << "test_udl_integer: ";

    using namespace uint128_literals;

    // Test integer literal
    [[maybe_unused]] auto val = 12345_u128;
    assert(val.low() == 12345ULL);
    assert(val.high() == 0);

    // Test zero
    [[maybe_unused]] auto zero = 0_u128;
    assert(zero == uint128_t(0));

    // Test max uint64
    [[maybe_unused]] auto max64 = 18446744073709551615_u128;
    assert(max64.low() == UINT64_MAX);
    assert(max64.high() == 0);

    std::cout << "passed" << std::endl;
}

void test_udl_string()
{
    std::cout << "test_udl_string: ";

    using namespace uint128_literals;

    // Test string literals
    [[maybe_unused]] auto hex = "0xFF"_u128;
    assert(hex.low() == 0xFF);

    [[maybe_unused]] auto dec = "12345"_u128;
    assert(dec.low() == 12345);

    [[maybe_unused]] auto bin = "0b1010"_u128;
    assert(bin.low() == 0b1010);

    [[maybe_unused]] auto oct = "0777"_u128;
    assert(oct.low() == 0777);

    std::cout << "passed" << std::endl;
}

void test_udl_specific_formats()
{
    std::cout << "test_udl_specific_formats: ";

    using namespace uint128_literals;

    // Test format-specific literals
    [[maybe_unused]] auto hex = "FF"_u128_hex;
    assert(hex.low() == 0xFF);

    [[maybe_unused]] auto bin = "1010"_u128_bin;
    assert(bin.low() == 0b1010);

    [[maybe_unused]] auto oct = "777"_u128_oct;
    assert(oct.low() == 0777);

    std::cout << "passed" << std::endl;
}

// ========================= STREAM OPERATORS =========================

void test_ostream_operator()
{
    std::cout << "test_ostream_operator: ";

    std::ostringstream oss;

    // Test zero
    [[maybe_unused]] uint128_t zero(0);
    oss << zero;
    assert(oss.str() == "0");

    // Test small value
    oss.str("");
    uint128_t small(0, 123);
    oss << small;
    assert(oss.str() == "123");

    // Test larger value
    oss.str("");
    [[maybe_unused]] uint128_t large(1, 0);
    oss << large;
    assert(oss.str() == "18446744073709551616"); // 2^64

    std::cout << "passed" << std::endl;
}

void test_istream_operator()
{
    std::cout << "test_istream_operator: ";

    // Test reading decimal
    std::istringstream iss("12345");
    [[maybe_unused]] uint128_t val;
    iss >> val;
    assert(val.low() == 12345);

    // Test reading hex
    iss.str("0xFF");
    iss.clear();
    iss >> val;
    assert(val.low() == 0xFF);

    // Test reading binary
    iss.str("0b1010");
    iss.clear();
    iss >> val;
    assert(val.low() == 0b1010);

    std::cout << "passed" << std::endl;
}

// ========================= EDGE CASES AND LIMITS =========================

void test_overflow_behavior()
{
    std::cout << "test_overflow_behavior: ";

    // Test addition overflow
    [[maybe_unused]] uint128_t max(UINT64_MAX, UINT64_MAX);
    [[maybe_unused]] uint128_t one(0, 1);
    [[maybe_unused]] uint128_t result = max + one;
    assert(result == uint128_t(0, 0)); // Should wrap to zero

    // Test increment overflow
    [[maybe_unused]] uint128_t almost_max(UINT64_MAX, UINT64_MAX);
    ++almost_max;
    assert(almost_max == uint128_t(0, 0));

    // Test multiplication overflow (silent)
    uint128_t half_max(UINT64_MAX >> 1, UINT64_MAX);
    [[maybe_unused]] uint128_t mult_result = half_max * uint128_t(0, 4);
    // Just verify it computes without crashing
    assert(mult_result.high() != 0 || mult_result.low() != 0);

    std::cout << "passed" << std::endl;
}

void test_underflow_behavior()
{
    std::cout << "test_underflow_behavior: ";

    // Test subtraction underflow
    [[maybe_unused]] uint128_t zero(0);
    [[maybe_unused]] uint128_t one(0, 1);
    [[maybe_unused]] uint128_t result = zero - one;
    assert(result == uint128_t(UINT64_MAX, UINT64_MAX)); // Should wrap to max

    // Test decrement underflow
    uint128_t min(0);
    --min;
    assert(min == uint128_t(UINT64_MAX, UINT64_MAX));

    std::cout << "passed" << std::endl;
}

void test_boundary_values()
{
    std::cout << "test_boundary_values: ";

    // Test zero
    [[maybe_unused]] uint128_t zero(0);
    assert(zero.high() == 0);
    assert(zero.low() == 0);
    assert(!static_cast<bool>(zero));

    // Test max value
    [[maybe_unused]] uint128_t max(UINT64_MAX, UINT64_MAX);
    assert(max.high() == UINT64_MAX);
    assert(max.low() == UINT64_MAX);
    assert(static_cast<bool>(max));

    // Test powers of 2
    [[maybe_unused]] uint128_t power64(1, 0); // 2^64
    assert(power64.high() == 1);
    assert(power64.low() == 0);

    // Test one less than power of 2
    [[maybe_unused]] uint128_t almost_power64(0, UINT64_MAX); // 2^64 - 1
    assert(almost_power64.high() == 0);
    assert(almost_power64.low() == UINT64_MAX);

    std::cout << "passed" << std::endl;
}

void test_division_by_zero()
{
    std::cout << "test_division_by_zero: ";

    uint128_t dividend(100, 200);
    [[maybe_unused]] uint128_t zero(0);

    // Division by zero behavior (noexcept - returns zero or max, implementation defined)
    // The operators are marked noexcept, so they don't throw
    // Just verify the operation completes without crashing
    [[maybe_unused]] uint128_t result1 = dividend / zero;
    (void)result1; // Should not crash

    [[maybe_unused]] uint128_t result2 = dividend % zero;
    (void)result2; // Should not crash

    // Division by non-zero should work
    [[maybe_unused]] uint128_t result3 = dividend / uint128_t(0, 2);
    assert(result3.high() == 50 && result3.low() == 100);

    std::cout << "passed" << std::endl;
}

// ========================= CONSTEXPR VERIFICATION =========================

void test_constexpr_operations()
{
    std::cout << "test_constexpr_operations: ";

    // These should all be evaluated at compile time
    constexpr uint128_t zero(0);
    constexpr uint128_t one(0, 1);
    constexpr uint128_t two(0, 2);

    // Test constexpr arithmetic
    constexpr uint128_t sum = one + one;
    static_assert(sum.low() == 2, "Constexpr addition failed");

    constexpr uint128_t diff = two - one;
    static_assert(diff.low() == 1, "Constexpr subtraction failed");

    // Test constexpr comparison
    static_assert(one < two, "Constexpr comparison failed");
    static_assert(zero != one, "Constexpr inequality failed");
    static_assert(one == one, "Constexpr equality failed");

    // Test constexpr bitwise
    constexpr uint128_t val(0, 0xFF);
    constexpr uint128_t shifted = val << 8;
    static_assert(shifted.low() == 0xFF00, "Constexpr shift failed");

    std::cout << "passed (compile-time)" << std::endl;
}

// ========================= TYPE TRAITS =========================

void test_type_traits()
{
    std::cout << "test_type_traits: ";

    // Test that uint128_t satisfies expected traits
    static_assert(std::is_default_constructible<uint128_t>::value,
                  "Should be default constructible");
    static_assert(std::is_copy_constructible<uint128_t>::value, "Should be copy constructible");
    static_assert(std::is_move_constructible<uint128_t>::value, "Should be move constructible");
    static_assert(std::is_copy_assignable<uint128_t>::value, "Should be copy assignable");
    static_assert(std::is_move_assignable<uint128_t>::value, "Should be move assignable");

    // Test triviality (for performance)
    // Note: These may not be trivial due to constructors, but let's check
    static_assert(std::is_trivially_copyable<uint128_t>::value, "Should be trivially copyable");

    // Test size
    static_assert(sizeof(uint128_t) == 16, "Should be 16 bytes");
    static_assert(alignof(uint128_t) <= 16, "Alignment should be reasonable");

    std::cout << "passed (compile-time)" << std::endl;
}

// ========================= ADDITIONAL EDGE CASES =========================

void test_shift_edge_cases()
{
    std::cout << "test_shift_edge_cases: ";

    [[maybe_unused]] uint128_t val(0x1234, 0x5678);

    // Test shift by 0
    [[maybe_unused]] uint128_t shift0 = val << 0;
    assert(shift0 == val);

    shift0 = val >> 0;
    assert(shift0 == val);

    // Test shift by 64 (boundary between high and low)
    [[maybe_unused]] uint128_t shift64 = uint128_t(0, 0xFF) << 64;
    assert(shift64.high() == 0xFF);
    assert(shift64.low() == 0);

    [[maybe_unused]] uint128_t shift64r = uint128_t(0xFF, 0) >> 64;
    assert(shift64r.high() == 0);
    assert(shift64r.low() == 0xFF);

    // Test shift by 128 (should zero out)
    [[maybe_unused]] uint128_t shift128 = val << 128;
    assert(shift128 == uint128_t(0));

    [[maybe_unused]] uint128_t shift128r = val >> 128;
    assert(shift128r == uint128_t(0));

    // Test shift by large value
    [[maybe_unused]] uint128_t shift200 = val << 200;
    assert(shift200 == uint128_t(0));

    std::cout << "passed" << std::endl;
}

void test_mixed_type_operations()
{
    std::cout << "test_mixed_type_operations: ";

    uint128_t small(0, 200); // Only low part, easier to test

    // Test operations with different integral types
    [[maybe_unused]] uint128_t result1 = small + 50;
    assert(result1.low() == 250);

    [[maybe_unused]] uint128_t result2 = small - 100;
    assert(result2.low() == 100);

    [[maybe_unused]] uint128_t result3 = small * 2;
    assert(result3.low() == 400);

    [[maybe_unused]] uint128_t result4 = small / 2;
    assert(result4.low() == 100);

    // Test symmetric operations
    [[maybe_unused]] uint128_t result5 = 50 + small;
    assert(result5.low() == 250);

    [[maybe_unused]] uint128_t result6 = 1000 - small;
    assert(result6.low() == 800);
    assert(result6.high() == 0);

    // Test with negative int (sign extension)
    [[maybe_unused]] uint128_t result7 = small + (-10);
    assert(result7.low() == 190);

    std::cout << "passed" << std::endl;
}

int main()
{
    std::cout << "Running comprehensive tests for uint128_t (66 original + 18 additional)..."
              << std::endl;
    std::cout << std::endl;

    test_sizeof_is_16_bytes();
    test_bits_is_128();
    test_sizeof_high_is_8_bytes();
    test_self_div_2_64_equ_high();
    test_sizeof_low_is_8_bytes();
    test_self_rem_2_64_equ_low();
    test_set_high();
    test_set_low();
    test_default_constructor();
    test_integral_constructor();
    test_integral_assignment();
    test_high_low_constructor();
    test_cstr_constructor();
    test_cstr_assignment();
    test_bool_conversion();
    test_integral_conversion();
    test___uint128_conversion();
    test___int128_conversion();
    test_addition_assignment();
    test_subtraction_assignment();
    test_add_operator();
    test_sub_operator();
    test_pre_increment();
    test_post_increment();
    test_pre_decrement();
    test_post_decrement();
    test_leading_zeros();
    test_trailing_zeros();
    test_bitwise_operators();
    test_bitwise_assignment_operators();
    test_shift_left();
    test_shift_right();
    test_effective_length();
    test_is_power_of_2();
    test_comparison_operators();
    test_divrem_basic();
    test_divrem_zero();
    test_divrem_large();
    test_divrem_zero_integral_divisor();
    test_divrem_basic_integral_divisor();
    test_divrem_large_integral_divisor();
    test_divrem_random();
    test_divrem_random_integral_divisor();
    test_divrem_known_result();
    test_divrem_known_result_integral_divisor();
    test_mult_assignment_operator();
    test_mult_operator();
    test_div_operator();
    test_mod_operator();
    test_fullmult_times_uint64();
    test_knuth_D_divrem();
    test_knuth_D_divrem_integral();
    test_to_string();
    test_to_string_base();
    test_to_string_hex();
    test_to_string_bin();
    test_to_string_oct();
    test_from_cstr();
    test_from_cstr_base();
    test_to_cstr();
    test_to_cstr_base();
    test_to_cstr_hex();
    test_to_cstr_bin();
    test_to_cstr_oct();
    test_from_string();
    test_from_string_base();

    std::cout << std::endl;
    std::cout << "=== Additional Robustness Tests ===" << std::endl;

    // Copy and Move Semantics
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_vector_of_uint128();

    // User-Defined Literals
    test_udl_integer();
    test_udl_string();
    test_udl_specific_formats();

    // Stream Operators
    test_ostream_operator();
    test_istream_operator();

    // Edge Cases and Limits
    test_overflow_behavior();
    test_underflow_behavior();
    test_boundary_values();
    test_division_by_zero();

    // Constexpr and Type Traits
    test_constexpr_operations();
    test_type_traits();

    // Additional Edge Cases
    test_shift_edge_cases();
    test_mixed_type_operations();

    std::cout << std::endl;
    std::cout << "All tests (66 original + 18 additional = 84 total) passed successfully."
              << std::endl;
    return 0;
}
