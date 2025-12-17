#include "../include/uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>

using namespace uint128_literals;
void test_divrem_basic()
{
    std::cout << "test_divrem: test_divrem_basic ......" << std::endl;
    uint128_t a = 100_u128;
    uint128_t b = 7_u128;

    auto res = a.divrem(b);
    assert(res.has_value());
    assert(res->first == 14_u128);
    assert(res->second == 2_u128);
    std::cout << "test_divrem: test_divrem_basic passed" << std::endl;
}

void test_divrem_zero()
{
    std::cout << "test_divrem: test_divrem_zero ......" << std::endl;
    uint128_t a = 100_u128;
    auto res = a.divrem(0_u128);
    assert(!res.has_value());
    std::cout << "test_divrem: test_divrem_zero passed" << std::endl;
}

void test_divrem_large()
{
    std::cout << "test_divrem: test_divrem_large ......" << std::endl;
    // 2^100
    uint128_t large = 1_u128 << 100;
    uint128_t divisor = 3_u128;

    auto res = large.divrem(divisor);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res->first * divisor + res->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res->second < divisor);

    std::cout << "test_divrem: test_divrem_large passed" << std::endl;
}

int main()
{
    std::cout << "=== Test uint128_t::divrem ===" << std::endl;
    test_divrem_basic();
    test_divrem_zero();
    test_divrem_large();
    std::cout << "\n All divrem_tests passed!" << std::endl;
    return 0;
}