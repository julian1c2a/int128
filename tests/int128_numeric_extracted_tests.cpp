/*
 * Test para int128_numeric.hpp
 */

#include "../include/int128/int128_numeric.hpp"
#include "../include/int128/int128_t.hpp"
#include "test_iostream_helpers.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;


using namespace int128_literals;

void test_arithmetic_functions()
{
    std::cout << "=== Testing ARITHMETIC FUNCTIONS ===" << std::endl;

    // Test midpoint
    std::cout << "\n[INFO] Testing midpoint:" << std::endl;
    auto mid1 = std::midpoint(10_i128, 20_i128);
    auto mid2 = std::midpoint(-10_i128, 10_i128);
    auto mid3 = std::midpoint(-20_i128, -10_i128);

    std::cout << "midpoint(10, 20) = " << mid1 << std::endl;
    std::cout << "midpoint(-10, 10) = " << mid2 << std::endl;
    std::cout << "midpoint(-20, -10) = " << mid3 << std::endl;

    assert(mid1 == 15_i128);
    assert(mid2 == 0_i128);
    assert(mid3 == -15_i128);

    // Test clamp
    std::cout << "\n[INFO] Testing clamp:" << std::endl;
    auto clamp1 = std::clamp(5_i128, 10_i128, 20_i128);
    auto clamp2 = std::clamp(15_i128, 10_i128, 20_i128);
    auto clamp3 = std::clamp(25_i128, 10_i128, 20_i128);
    auto clamp4 = std::clamp(-25_i128, -10_i128, 10_i128);

    std::cout << "clamp(5, 10, 20) = " << clamp1 << std::endl;
    std::cout << "clamp(15, 10, 20) = " << clamp2 << std::endl;
    std::cout << "clamp(25, 10, 20) = " << clamp3 << std::endl;
    std::cout << "clamp(-25, -10, 10) = " << clamp4 << std::endl;

    assert(clamp1 == 10_i128);
    assert(clamp2 == 15_i128);
    assert(clamp3 == 20_i128);
    assert(clamp4 == -10_i128);

    // Test abs
    std::cout << "\n[INFO] Testing abs:" << std::endl;
    auto abs1 = std::abs(42_i128);
    auto abs2 = std::abs(-42_i128);
    auto abs3 = std::abs(0_i128);
    auto abs_min = std::abs(int128_t_MIN);

    std::cout << "abs(42) = " << abs1 << std::endl;
    std::cout << "abs(-42) = " << abs2 << std::endl;
    std::cout << "abs(0) = " << abs3 << std::endl;
    std::cout << "abs(INT128_MIN) = " << abs_min << " (overflow case)" << std::endl;

    assert(abs1 == 42_i128);
    assert(abs2 == 42_i128);
    assert(abs3 == 0_i128);
    assert(abs_min == int128_t_MIN); // Caso especial de overflow

    std::cout << "[OK] Arithmetic functions OK" << std::endl;
}

void test_bit_functions()
{
    std::cout << "\n=== Testing BIT FUNCTIONS ===" << std::endl;

    // Test popcount
    std::cout << "\n[INFO] Testing popcount:" << std::endl;
    auto pop1 = std::popcount(7_i128);  // 0111 = 3 bits
    auto pop2 = std::popcount(15_i128); // 1111 = 4 bits
    auto pop3 = std::popcount(-1_i128); // Todos 1s = 128 bits
    auto pop4 = std::popcount(0_i128);  // 0000 = 0 bits

    std::cout << "popcount(7) = " << pop1 << std::endl;
    std::cout << "popcount(15) = " << pop2 << std::endl;
    std::cout << "popcount(-1) = " << pop3 << std::endl;
    std::cout << "popcount(0) = " << pop4 << std::endl;

    assert(pop1 == 3);
    assert(pop2 == 4);
    assert(pop3 == 128);
    assert(pop4 == 0);

    // Test countl_zero
    std::cout << "\n[INFO] Testing countl_zero:" << std::endl;
    auto clz1 = std::countl_zero(1_i128);  // 000...0001
    auto clz2 = std::countl_zero(0_i128);  // Todo ceros
    auto clz3 = std::countl_zero(-1_i128); // Empieza con 1

    std::cout << "countl_zero(1) = " << clz1 << std::endl;
    std::cout << "countl_zero(0) = " << clz2 << std::endl;
    std::cout << "countl_zero(-1) = " << clz3 << std::endl;

    assert(clz1 == 127); // 127 ceros antes del último bit
    assert(clz2 == 128); // Todos ceros
    assert(clz3 == 0);   // Empieza con 1 (signo)

    // Test countr_zero
    std::cout << "\n[INFO] Testing countr_zero:" << std::endl;
    auto ctz1 = std::countr_zero(8_i128); // 1000 = 3 ceros al final
    auto ctz2 = std::countr_zero(1_i128); // 0001 = 0 ceros al final
    auto ctz3 = std::countr_zero(0_i128); // Todo ceros

    std::cout << "countr_zero(8) = " << ctz1 << std::endl;
    std::cout << "countr_zero(1) = " << ctz2 << std::endl;
    std::cout << "countr_zero(0) = " << ctz3 << std::endl;

    assert(ctz1 == 3);
    assert(ctz2 == 0);
    assert(ctz3 == 128);

    // Test bit_width
    std::cout << "\n[INFO] Testing bit_width:" << std::endl;
    auto bw1 = std::bit_width(7_i128);  // 111 = necesita 3 bits
    auto bw2 = std::bit_width(15_i128); // 1111 = necesita 4 bits
    auto bw3 = std::bit_width(0_i128);  // 0 = necesita 0 bits
    auto bw4 = std::bit_width(-1_i128); // negativo = usa todos

    std::cout << "bit_width(7) = " << bw1 << std::endl;
    std::cout << "bit_width(15) = " << bw2 << std::endl;
    std::cout << "bit_width(0) = " << bw3 << std::endl;
    std::cout << "bit_width(-1) = " << bw4 << std::endl;

    assert(bw1 == 3);
    assert(bw2 == 4);
    assert(bw3 == 0);
    assert(bw4 == 128);

    std::cout << "[OK] Bit functions OK" << std::endl;
}

void test_power_of_2_functions()
{
    std::cout << "\n=== Testing POWER OF 2 FUNCTIONS ===" << std::endl;

    // Test has_single_bit
    std::cout << "\n[INFO] Testing has_single_bit:" << std::endl;
    bool hsb1 = std::has_single_bit(8_i128);  // 1000 = potencia de 2
    bool hsb2 = std::has_single_bit(7_i128);  // 0111 = no es potencia de 2
    bool hsb3 = std::has_single_bit(1_i128);  // 0001 = potencia de 2
    bool hsb4 = std::has_single_bit(-8_i128); // negativo = no es potencia de 2

    std::cout << "has_single_bit(8) = " << hsb1 << std::endl;
    std::cout << "has_single_bit(7) = " << hsb2 << std::endl;
    std::cout << "has_single_bit(1) = " << hsb3 << std::endl;
    std::cout << "has_single_bit(-8) = " << hsb4 << std::endl;

    assert(hsb1 == true);
    assert(hsb2 == false);
    assert(hsb3 == true);
    assert(hsb4 == false);

    // Test bit_floor
    std::cout << "\n[INFO] Testing bit_floor:" << std::endl;
    auto bf1 = std::bit_floor(15_i128); // 15 -> 8 (mayor potencia de 2 <= 15)
    auto bf2 = std::bit_floor(16_i128); // 16 -> 16 (es potencia de 2)
    auto bf3 = std::bit_floor(1_i128);  // 1 -> 1
    auto bf4 = std::bit_floor(0_i128);  // 0 -> 0

    std::cout << "bit_floor(15) = " << bf1 << std::endl;
    std::cout << "bit_floor(16) = " << bf2 << std::endl;
    std::cout << "bit_floor(1) = " << bf3 << std::endl;
    std::cout << "bit_floor(0) = " << bf4 << std::endl;

    assert(bf1 == 8_i128);
    assert(bf2 == 16_i128);
    assert(bf3 == 1_i128);
    assert(bf4 == 0_i128);

    std::cout << "[OK] Power of 2 functions OK" << std::endl;
}

void test_mathematical_functions()
{
    std::cout << "\n=== Testing MATHEMATICAL FUNCTIONS ===" << std::endl;

    // Test gcd
    std::cout << "\n[INFO] Testing gcd:" << std::endl;
    auto gcd1 = std::gcd(48_i128, 18_i128);  // gcd(48, 18) = 6
    auto gcd2 = std::gcd(-48_i128, 18_i128); // gcd(|-48|, |18|) = 6
    auto gcd3 = std::gcd(17_i128, 19_i128);  // gcd(17, 19) = 1 (primos)
    auto gcd4 = std::gcd(0_i128, 42_i128);   // gcd(0, 42) = 42

    std::cout << "gcd(48, 18) = " << gcd1 << std::endl;
    std::cout << "gcd(-48, 18) = " << gcd2 << std::endl;
    std::cout << "gcd(17, 19) = " << gcd3 << std::endl;
    std::cout << "gcd(0, 42) = " << gcd4 << std::endl;

    assert(gcd1 == 6_i128);
    assert(gcd2 == 6_i128);
    assert(gcd3 == 1_i128);
    assert(gcd4 == 42_i128);

    // Test lcm
    std::cout << "\n[INFO] Testing lcm:" << std::endl;
    auto lcm1 = std::lcm(4_i128, 6_i128);  // lcm(4, 6) = 12
    auto lcm2 = std::lcm(-4_i128, 6_i128); // lcm(|-4|, |6|) = 12
    auto lcm3 = std::lcm(7_i128, 11_i128); // lcm(7, 11) = 77 (primos)
    auto lcm4 = std::lcm(0_i128, 42_i128); // lcm(0, anything) = 0

    std::cout << "lcm(4, 6) = " << lcm1 << std::endl;
    std::cout << "lcm(-4, 6) = " << lcm2 << std::endl;
    std::cout << "lcm(7, 11) = " << lcm3 << std::endl;
    std::cout << "lcm(0, 42) = " << lcm4 << std::endl;

    assert(lcm1 == 12_i128);
    assert(lcm2 == 12_i128);
    assert(lcm3 == 77_i128);
    assert(lcm4 == 0_i128);

    std::cout << "[OK] Mathematical functions OK" << std::endl;
}

int main()
{
    std::cout << "🧮 COMPREHENSIVE INT128_T NUMERIC TESTING" << std::endl;
    std::cout << "==========================================" << std::endl;

    try {
        test_arithmetic_functions();
        test_bit_functions();
        test_power_of_2_functions();
        test_mathematical_functions();

        std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!" << std::endl;
        std::cout << "\n🔧 FUNCIONES CONFIRMADAS:" << std::endl;
        std::cout << "[OK] std::midpoint(int128_t, int128_t)" << std::endl;
        std::cout << "[OK] std::clamp(int128_t, int128_t, int128_t)" << std::endl;
        std::cout << "[OK] std::abs(int128_t)" << std::endl;
        std::cout << "[OK] std::popcount(int128_t)" << std::endl;
        std::cout << "[OK] std::countl_zero(int128_t)" << std::endl;
        std::cout << "[OK] std::countr_zero(int128_t)" << std::endl;
        std::cout << "[OK] std::bit_width(int128_t)" << std::endl;
        std::cout << "[OK] std::has_single_bit(int128_t)" << std::endl;
        std::cout << "[OK] std::bit_floor(int128_t)" << std::endl;
        std::cout << "[OK] std::gcd(int128_t, int128_t)" << std::endl;
        std::cout << "[OK] std::lcm(int128_t, int128_t)" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}