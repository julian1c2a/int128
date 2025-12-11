#include "uint128_t.hpp"
#include <cassert>
#include <iostream>

void test_constructor_and_equality() {
    std::cout << "Running test: test_constructor_and_equality\n";
    append::uint128_t num1(0x1111222233334444, 0x5555666677778888);
    append::uint128_t num2(0x1111222233334444, 0x5555666677778888);
    assert(num1 == num2);

    append::uint128_t num3(0, 0);
    append::uint128_t num4;
    assert(num3 == num4);

    append::uint128_t num5(123);
    assert(num5.high == 0 && num5.low == 123);
    std::cout << "Test passed: test_constructor_and_equality\n";
}

int main() {
    test_constructor_and_equality();
    return 0;
}

