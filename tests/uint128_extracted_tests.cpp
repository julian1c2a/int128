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
        // GENERAR VALOR NEGATIVO PARA TIPOS CON SIGNO
        int64_t valsignedlow =
            std::uniform_int_distribution<int64_t>(std::numeric_limits<int64_t>::min(), -1)(rng);
        uint64_t vallow = std::abs(valsignedlow);

        // uint8_t
        uint128_t val_1(static_cast<uint8_t>(vallow));
        assert(static_cast<uint8_t>(val_1.low()) == static_cast<uint8_t>(vallow));
        assert(val_1.high() == 0ull);

        // uint16_t
        uint128_t val_2(static_cast<uint16_t>(vallow));
        assert(static_cast<uint16_t>(val_2.low()) == static_cast<uint16_t>(vallow));
        assert(val_2.high() == 0ull);

        // uint32_t
        uint128_t val_3(static_cast<uint32_t>(vallow));
        assert(static_cast<uint32_t>(val_3.low()) == static_cast<uint32_t>(vallow));
        assert(val_3.high() == 0ull);

        // uint64_t
        uint128_t val_4(static_cast<uint64_t>(vallow));
        assert(static_cast<uint64_t>(val_4.low()) == static_cast<uint64_t>(vallow));
        assert(val_4.high() == 0ull);

        // int8_t
        uint128_t val_5(static_cast<int8_t>(vallow));
        assert(static_cast<int8_t>(val_5.low()) == static_cast<int8_t>(vallow));
        assert(val_5.high() == 0ull);

        // int16_t
        uint128_t val_6(static_cast<int16_t>(vallow));
        assert(static_cast<int16_t>(val_6.low()) == static_cast<int16_t>(vallow));
        assert(val_6.high() == 0ull);

        // int32_t
        uint128_t val_7(static_cast<int32_t>(vallow));
        assert(static_cast<int32_t>(val_7.low()) == static_cast<int32_t>(vallow));
        assert(val_7.high() == 0ull);

        // int64_t
        uint128_t val_8(static_cast<int64_t>(vallow));
        assert(static_cast<int64_t>(val_8.low()) == static_cast<int64_t>(vallow));
        assert(val_8.high() == 0ull);

        // int8_t
        uint128_t val_9(static_cast<int8_t>(valsignedlow));
        assert(static_cast<int8_t>(val_9.low()) == static_cast<int8_t>(valsignedlow));
        assert(val_9.high() == ~0ull);

        // int16_t
        uint128_t val_A(static_cast<int16_t>(valsignedlow));
        assert(static_cast<int16_t>(val_A.low()) == static_cast<int16_t>(valsignedlow));
        assert(val_A.high() == ~0ull);

        // int32_t
        uint128_t val_B(static_cast<int32_t>(valsignedlow));
        assert(static_cast<int32_t>(val_B.low()) == static_cast<int32_t>(valsignedlow));
        assert(val_B.high() == ~0ull);

        // int64_t
        uint128_t val_C(static_cast<int64_t>(valsignedlow));
        assert(static_cast<int64_t>(val_C.low()) == static_cast<int64_t>(valsignedlow));
        assert(val_C.high() == ~0ull);
    }
    std::cout << "test_integral_constructor passed" << std::endl;
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

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}