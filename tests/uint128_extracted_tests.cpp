/*
 * Test file generated from Doxygen @test and @code{.cpp} tags in uint128_t.hpp
 */

#include "../include/uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <type_traits>

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
    uint128_t val;
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
    uint128_t val;
    assert(sizeof(val.low()) == 8);
    std::cout << "test_sizeof_low_is_8_bytes passed" << std::endl;
}

void test_self_rem_2_64_equ_low()
{
    // HACER ESTO CON VALORES ALEATORIOS DE uint64_t UN MONTÓN DE VECES
    // Nota: La documentación original tenía un error de copy-paste en el bloque
    // de código. Se implementa la verificación lógica: low() debe corresponder a
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
        uint128_t val;
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
        uint128_t val;
        uint64_t v = rng();
        val.set_low(v);
        assert(val.low() == v);
    }
    std::cout << "test_set_low passed" << std::endl;
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
        uint128_t val_i8_pos(v_i8_pos);
        assert(static_cast<int8_t>(val_i8_pos.low()) == v_i8_pos);
        assert(val_i8_pos.high() == 0ull);

        // int16_t
        int16_t v_i16_pos = static_cast<int16_t>(rand_val & 0x7FFF);
        uint128_t val_i16_pos(v_i16_pos);
        assert(static_cast<int16_t>(val_i16_pos.low()) == v_i16_pos);
        assert(val_i16_pos.high() == 0ull);

        // int32_t
        int32_t v_i32_pos = static_cast<int32_t>(rand_val & 0x7FFFFFFF);
        uint128_t val_i32_pos(v_i32_pos);
        assert(static_cast<int32_t>(val_i32_pos.low()) == v_i32_pos);
        assert(val_i32_pos.high() == 0ull);

        // int64_t
        int64_t v_i64_pos = static_cast<int64_t>(rand_val & 0x7FFFFFFFFFFFFFFFULL);
        uint128_t val_i64_pos(v_i64_pos);
        assert(static_cast<int64_t>(val_i64_pos.low()) == v_i64_pos);
        assert(val_i64_pos.high() == 0ull);

        // --- SIGNED TYPES (Negativos: high=~0) ---
        // Forzamos el bit de signo y aseguramos que sea negativo

        // int8_t
        int8_t v_i8_neg = static_cast<int8_t>(rand_val | 0x80);
        if (v_i8_neg >= 0)
            v_i8_neg = -1;
        uint128_t val_i8_neg(v_i8_neg);
        assert(static_cast<int8_t>(val_i8_neg.low()) == v_i8_neg);
        assert(val_i8_neg.high() == ~0ull);

        // int16_t
        int16_t v_i16_neg = static_cast<int16_t>(rand_val | 0x8000);
        if (v_i16_neg >= 0)
            v_i16_neg = -1;
        uint128_t val_i16_neg(v_i16_neg);
        assert(static_cast<int16_t>(val_i16_neg.low()) == v_i16_neg);
        assert(val_i16_neg.high() == ~0ull);

        // int32_t
        int32_t v_i32_neg = static_cast<int32_t>(rand_val | 0x80000000);
        if (v_i32_neg >= 0)
            v_i32_neg = -1;
        uint128_t val_i32_neg(v_i32_neg);
        assert(static_cast<int32_t>(val_i32_neg.low()) == v_i32_neg);
        assert(val_i32_neg.high() == ~0ull);

        // int64_t
        int64_t v_i64_neg = static_cast<int64_t>(rand_val | 0x8000000000000000ULL);
        if (v_i64_neg >= 0)
            v_i64_neg = -1;
        uint128_t val_i64_neg(v_i64_neg);
        assert(static_cast<int64_t>(val_i64_neg.low()) == v_i64_neg);
        assert(val_i64_neg.high() == ~0ull);
    }
    std::cout << "test_integral_constructor passed" << std::endl;
}

void test_high_low_constructor()
{
    for (int i = 0; i < 1000; ++i) {
        uint64_t h = rng();
        uint64_t l = rng();

        // Test with uint64_t
        uint128_t val(h, l);
        assert(val.high() == h);
        assert(val.low() == l);

        // Test with mixed types (e.g. uint32_t)
        uint32_t h32 = static_cast<uint32_t>(h);
        uint32_t l32 = static_cast<uint32_t>(l);
        uint128_t val32(h32, l32);
        assert(val32.high() == h32);
        assert(val32.low() == l32);
    }
    std::cout << "test_high_low_constructor passed" << std::endl;
}

int main()
{
    std::cout << "Running extracted tests for uint128_t..." << std::endl;

    test_sizeof_is_16_bytes();
    test_bits_is_128();
    test_sizeof_high_is_8_bytes();
    test_self_div_2_64_equ_high();
    test_sizeof_low_is_8_bytes();
    test_self_rem_2_64_equ_low();
    test_set_high();
    test_set_low();
    test_integral_constructor();
    test_high_low_constructor();

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}