#include "../include/int128/int128_t.hpp"
#include <cassert>
#include <iostream>

using namespace int128_literals;

void test_divrem_basic()
{
    std::cout << "Testing divrem basic..." << std::endl;
    int128_t a = 100_i128;
    int128_t b = 7_i128;

    auto res = a.divrem(b);
    assert(res.has_value());
    assert(res->first == 14_i128);
    assert(res->second == 2_i128);
    std::cout << "  ✅ Basic OK" << std::endl;
}

void test_divrem_signs()
{
    std::cout << "Testing divrem signs..." << std::endl;
    int128_t a = 100_i128;
    int128_t b = 7_i128;
    int128_t neg_a = -100_i128;
    int128_t neg_b = -7_i128;

    // Regla C++11 para división entera: truncamiento hacia cero.
    // El signo del resto es el mismo que el del dividendo.

    // Caso 1: + / +  -> q=+, r=+
    // 100 / 7 = 14, rem 2
    auto r1 = a.divrem(b);
    assert(r1->first == 14_i128 && r1->second == 2_i128);

    // Caso 2: + / -  -> q=-, r=+
    // 100 / -7 = -14, rem 2
    auto r2 = a.divrem(neg_b);
    assert(r2->first == -14_i128 && r2->second == 2_i128);

    // Caso 3: - / +  -> q=-, r=-
    // -100 / 7 = -14, rem -2
    auto r3 = neg_a.divrem(b);
    assert(r3->first == -14_i128 && r3->second == -2_i128);

    // Caso 4: - / -  -> q=+, r=-
    // -100 / -7 = 14, rem -2
    auto r4 = neg_a.divrem(neg_b);
    assert(r4->first == 14_i128 && r4->second == -2_i128);

    std::cout << "  ✅ Signs OK" << std::endl;
}

void test_divrem_zero()
{
    std::cout << "Testing divrem zero..." << std::endl;
    int128_t a = 100_i128;
    auto res = a.divrem(0_i128);
    assert(!res.has_value());
    std::cout << "  ✅ Zero OK" << std::endl;
}

void test_divrem_large()
{
    std::cout << "Testing divrem large..." << std::endl;
    // 2^100
    int128_t large = 1_i128 << 100;
    int128_t divisor = 3_i128;

    auto res = large.divrem(divisor);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res->first * divisor + res->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res->second.abs() < divisor.abs());

    std::cout << "  ✅ Large OK" << std::endl;
}

int main()
{
    std::cout << "=== Test int128_t::divrem ===" << std::endl;
    test_divrem_basic();
    test_divrem_signs();
    test_divrem_zero();
    test_divrem_large();
    std::cout << "\n🎉 All tests passed!" << std::endl;
    return 0;
}