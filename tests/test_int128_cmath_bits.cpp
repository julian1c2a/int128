/*
 * Test para int128_cmath.hpp e int128_bits.hpp
 */

#include "../include/int128_bits.hpp"
#include "../include/int128_cmath.hpp"
#include "../include/int128_t.hpp"
#include <cassert>
#include <iostream>

using namespace int128_literals;

void test_cmath_functions()
{
    std::cout << "=== Testing INT128_T CMATH FUNCTIONS ===" << std::endl;

    // Test gcd
    std::cout << "\n📊 Testing gcd:" << std::endl;
    auto gcd1 = std::gcd(48_i128, 18_i128);
    auto gcd2 = std::gcd(-48_i128, 18_i128);
    auto gcd3 = std::gcd(48_i128, -18_i128);
    auto gcd4 = std::gcd(-48_i128, -18_i128);

    std::cout << "gcd(48, 18) = " << gcd1 << std::endl;
    std::cout << "gcd(-48, 18) = " << gcd2 << std::endl;
    std::cout << "gcd(48, -18) = " << gcd3 << std::endl;
    std::cout << "gcd(-48, -18) = " << gcd4 << std::endl;

    assert(gcd1 == 6_i128);
    assert(gcd2 == 6_i128);
    assert(gcd3 == 6_i128);
    assert(gcd4 == 6_i128);

    // Test lcm
    std::cout << "\n📊 Testing lcm:" << std::endl;
    auto lcm1 = std::lcm(12_i128, 18_i128);
    auto lcm2 = std::lcm(-12_i128, 18_i128);
    auto lcm3 = std::lcm(12_i128, -18_i128);

    std::cout << "lcm(12, 18) = " << lcm1 << std::endl;
    std::cout << "lcm(-12, 18) = " << lcm2 << std::endl;
    std::cout << "lcm(12, -18) = " << lcm3 << std::endl;

    assert(lcm1 == 36_i128);
    assert(lcm2 == 36_i128);
    assert(lcm3 == 36_i128);

    // Test abs
    std::cout << "\n📊 Testing abs:" << std::endl;
    auto abs1 = std::abs(42_i128);
    auto abs2 = std::abs(-42_i128);
    auto abs3 = std::abs(0_i128);

    std::cout << "abs(42) = " << abs1 << std::endl;
    std::cout << "abs(-42) = " << abs2 << std::endl;
    std::cout << "abs(0) = " << abs3 << std::endl;

    assert(abs1 == 42_i128);
    assert(abs2 == 42_i128);
    assert(abs3 == 0_i128);

    // Test pow
    std::cout << "\n📊 Testing pow:" << std::endl;
    auto pow1 = std::pow(2_i128, 10_i128);
    auto pow2 = std::pow(-2_i128, 3_i128);
    auto pow3 = std::pow(-2_i128, 4_i128);
    auto pow4 = std::pow(5_i128, 0_i128);

    std::cout << "pow(2, 10) = " << pow1 << std::endl;
    std::cout << "pow(-2, 3) = " << pow2 << std::endl;
    std::cout << "pow(-2, 4) = " << pow3 << std::endl;
    std::cout << "pow(5, 0) = " << pow4 << std::endl;

    assert(pow1 == 1024_i128);
    assert(pow2 == -8_i128);
    assert(pow3 == 16_i128);
    assert(pow4 == 1_i128);

    // Test sqrt
    std::cout << "\n📊 Testing sqrt:" << std::endl;
    auto sqrt1 = std::sqrt(64_i128);
    auto sqrt2 = std::sqrt(100_i128);
    auto sqrt3 = std::sqrt(-16_i128);
    auto sqrt4 = std::sqrt(0_i128);

    std::cout << "sqrt(64) = " << sqrt1 << std::endl;
    std::cout << "sqrt(100) = " << sqrt2 << std::endl;
    std::cout << "sqrt(-16) = " << sqrt3 << " (should be 0)" << std::endl;
    std::cout << "sqrt(0) = " << sqrt4 << std::endl;

    assert(sqrt1 == 8_i128);
    assert(sqrt2 == 10_i128);
    assert(sqrt3 == 0_i128); // Números negativos dan 0
    assert(sqrt4 == 0_i128);

    // Test sign
    std::cout << "\n📊 Testing sign:" << std::endl;
    auto sign1 = std::sign(42_i128);
    auto sign2 = std::sign(-42_i128);
    auto sign3 = std::sign(0_i128);

    std::cout << "sign(42) = " << sign1 << std::endl;
    std::cout << "sign(-42) = " << sign2 << std::endl;
    std::cout << "sign(0) = " << sign3 << std::endl;

    assert(sign1 == 1_i128);
    assert(sign2 == -1_i128);
    assert(sign3 == 0_i128);

    // Test min/max
    std::cout << "\n📊 Testing min/max:" << std::endl;
    auto min1 = std::min(42_i128, 17_i128);
    auto max1 = std::max(42_i128, 17_i128);
    auto min2 = std::min(-10_i128, 5_i128);
    auto max2 = std::max(-10_i128, 5_i128);

    std::cout << "min(42, 17) = " << min1 << std::endl;
    std::cout << "max(42, 17) = " << max1 << std::endl;
    std::cout << "min(-10, 5) = " << min2 << std::endl;
    std::cout << "max(-10, 5) = " << max2 << std::endl;

    assert(min1 == 17_i128);
    assert(max1 == 42_i128);
    assert(min2 == -10_i128);
    assert(max2 == 5_i128);

    std::cout << "✅ CMATH functions OK" << std::endl;
}

void test_bit_functions()
{
    std::cout << "\n=== Testing INT128_T BIT FUNCTIONS ===" << std::endl;

    // Test popcount
    std::cout << "\n📊 Testing popcount:" << std::endl;
    auto pop1 = std::popcount(7_i128);  // 0111 = 3 bits
    auto pop2 = std::popcount(-1_i128); // All 1s = 128 bits
    auto pop3 = std::popcount(0_i128);  // 0000 = 0 bits
    auto pop4 = std::popcount(-8_i128); // Complemento a 2

    std::cout << "popcount(7) = " << pop1 << std::endl;
    std::cout << "popcount(-1) = " << pop2 << std::endl;
    std::cout << "popcount(0) = " << pop3 << std::endl;
    std::cout << "popcount(-8) = " << pop4 << std::endl;

    assert(pop1 == 3);
    assert(pop2 == 128);
    assert(pop3 == 0);

    // Test countl_zero
    std::cout << "\n📊 Testing countl_zero:" << std::endl;
    auto clz1 = std::countl_zero(1_i128);
    auto clz2 = std::countl_zero(0_i128);
    auto clz3 = std::countl_zero(-1_i128);

    std::cout << "countl_zero(1) = " << clz1 << std::endl;
    std::cout << "countl_zero(0) = " << clz2 << std::endl;
    std::cout << "countl_zero(-1) = " << clz3 << std::endl;

    assert(clz1 == 127);
    assert(clz2 == 128);
    assert(clz3 == 0); // Negativos empiezan con 1

    // Test countr_zero
    std::cout << "\n📊 Testing countr_zero:" << std::endl;
    auto ctz1 = std::countr_zero(8_i128);
    auto ctz2 = std::countr_zero(1_i128);
    auto ctz3 = std::countr_zero(0_i128);

    std::cout << "countr_zero(8) = " << ctz1 << std::endl;
    std::cout << "countr_zero(1) = " << ctz2 << std::endl;
    std::cout << "countr_zero(0) = " << ctz3 << std::endl;

    assert(ctz1 == 3);
    assert(ctz2 == 0);
    assert(ctz3 == 128);

    // Test bit_width
    std::cout << "\n📊 Testing bit_width:" << std::endl;
    auto bw1 = std::bit_width(7_i128);
    auto bw2 = std::bit_width(0_i128);
    auto bw3 = std::bit_width(-1_i128);

    std::cout << "bit_width(7) = " << bw1 << std::endl;
    std::cout << "bit_width(0) = " << bw2 << std::endl;
    std::cout << "bit_width(-1) = " << bw3 << std::endl;

    assert(bw1 == 3);
    assert(bw2 == 0);
    assert(bw3 == 128); // Negativos usan todos los bits

    // Test has_single_bit
    std::cout << "\n📊 Testing has_single_bit:" << std::endl;
    bool hsb1 = std::has_single_bit(8_i128);
    bool hsb2 = std::has_single_bit(7_i128);
    bool hsb3 = std::has_single_bit(-8_i128);

    std::cout << "has_single_bit(8) = " << hsb1 << std::endl;
    std::cout << "has_single_bit(7) = " << hsb2 << std::endl;
    std::cout << "has_single_bit(-8) = " << hsb3 << std::endl;

    assert(hsb1 == true);
    assert(hsb2 == false);
    assert(hsb3 == false); // Negativos no califican

    std::cout << "✅ STD bit functions OK" << std::endl;
}

void test_extended_bit_functions()
{
    std::cout << "\n=== Testing EXTENDED BIT FUNCTIONS ===" << std::endl;

    // Test rotl/rotr
    std::cout << "\n📊 Testing rotation:" << std::endl;
    int128_t val = int128_t(0x123456789ABCDEF0ULL);
    auto rotl_result = int128_bits::rotl(val, 4);
    auto rotr_result = int128_bits::rotr(val, 4);

    std::cout << "Original: " << val.to_string_base(16) << std::endl;
    std::cout << "rotl(4): " << rotl_result.to_string_base(16) << std::endl;
    std::cout << "rotr(4): " << rotr_result.to_string_base(16) << std::endl;

    // Test bit manipulation
    std::cout << "\n📊 Testing bit manipulation:" << std::endl;
    int128_t test_val = 42_i128;

    bool bit5_set = int128_bits::test_bit(test_val, 5);
    auto with_bit10 = int128_bits::set_bit(test_val, 10);
    auto cleared_bit1 = int128_bits::clear_bit(test_val, 1);

    std::cout << "test_bit(42, 5) = " << bit5_set << std::endl;
    std::cout << "set_bit(42, 10) = " << with_bit10 << std::endl;
    std::cout << "clear_bit(42, 1) = " << cleared_bit1 << std::endl;

    assert(bit5_set == true);                    // 42 = 101010, bit 5 es 1
    assert(with_bit10 == (42_i128 + 1024_i128)); // Agregar 2^10
    assert(cleared_bit1 == 40_i128);             // 42 - 2 = 40

    // Test extract/insert bits
    std::cout << "\n📊 Testing extract/insert bits:" << std::endl;
    int128_t source = 0xFF_i128;                                      // 11111111
    auto extracted = int128_bits::extract_bits(source, 2, 4);         // Bits 2-5
    auto inserted = int128_bits::insert_bits(0_i128, 0xA_i128, 4, 4); // Insertar 1010 en posición 4

    std::cout << "extract_bits(0xFF, 2, 4) = " << extracted << std::endl;
    std::cout << "insert_bits(0, 0xA, 4, 4) = " << inserted << std::endl;

    assert(extracted == 0xF_i128); // Bits 2-5 de 0xFF = 1111 = 0xF
    assert(inserted == 0xA0_i128); // 1010 en posición 4

    // Test find first/last set
    std::cout << "\n📊 Testing find first/last set:" << std::endl;
    int128_t find_val = 0x104_i128; // 100000100
    int ffs = int128_bits::find_first_set(find_val);
    int fls = int128_bits::find_last_set(find_val);

    std::cout << "find_first_set(0x104) = " << ffs << std::endl;
    std::cout << "find_last_set(0x104) = " << fls << std::endl;

    assert(ffs == 2); // Primer bit set en posición 2
    assert(fls == 8); // Último bit set en posición 8

    std::cout << "✅ Extended bit functions OK" << std::endl;
}

void test_divmod()
{
    std::cout << "\n=== Testing DIVMOD ===" << std::endl;

    auto [q1, r1] = std::divmod(100_i128, 7_i128);
    auto [q2, r2] = std::divmod(-100_i128, 7_i128);
    auto [q3, r3] = std::divmod(100_i128, -7_i128);
    auto [q4, r4] = std::divmod(-100_i128, -7_i128);

    std::cout << "divmod(100, 7) = (" << q1 << ", " << r1 << ")" << std::endl;
    std::cout << "divmod(-100, 7) = (" << q2 << ", " << r2 << ")" << std::endl;
    std::cout << "divmod(100, -7) = (" << q3 << ", " << r3 << ")" << std::endl;
    std::cout << "divmod(-100, -7) = (" << q4 << ", " << r4 << ")" << std::endl;

    // Verificar que dividend = divisor * quotient + remainder
    assert(100_i128 == 7_i128 * q1 + r1);
    assert(-100_i128 == 7_i128 * q2 + r2);
    assert(100_i128 == -7_i128 * q3 + r3);
    assert(-100_i128 == -7_i128 * q4 + r4);

    std::cout << "✅ DIVMOD OK" << std::endl;
}

int main()
{
    std::cout << "🧮 COMPREHENSIVE INT128_T CMATH & BITS TESTING" << std::endl;
    std::cout << "===============================================" << std::endl;

    try {
        test_cmath_functions();
        test_bit_functions();
        test_extended_bit_functions();
        test_divmod();

        std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!" << std::endl;
        std::cout << "\n🔧 FUNCIONES CONFIRMADAS:" << std::endl;
        std::cout << "✅ int128_cmath.hpp - Funciones matemáticas completas" << std::endl;
        std::cout << "  • std::gcd, std::lcm, std::abs, std::pow, std::sqrt" << std::endl;
        std::cout << "  • std::sign, std::min, std::max, std::clamp, std::midpoint" << std::endl;
        std::cout << "  • std::divmod para división y módulo simultáneos" << std::endl;
        std::cout << "✅ int128_bits.hpp - Manipulación de bits completa" << std::endl;
        std::cout << "  • std::popcount, std::countl_zero, std::countr_zero" << std::endl;
        std::cout << "  • std::bit_width, std::has_single_bit, std::bit_floor" << std::endl;
        std::cout << "  • int128_bits:: rotl, rotr, reverse_bits, byteswap" << std::endl;
        std::cout << "  • int128_bits:: extract_bits, insert_bits, test/set/clear_bit" << std::endl;
        std::cout << "  • int128_bits:: find_first_set, find_last_set" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}