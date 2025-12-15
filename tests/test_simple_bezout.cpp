#include "include/uint128_cmath.hpp"
#include "include/uint128_t.hpp"
#include <iostream>

using namespace uint128_literals;

int main()
{
    auto [x, y] = std::bezout_coeffs(48_u128, 18_u128);
    uint128_t result = x * 48_u128 + y * 18_u128;
    std::cout << "x = " << x << ", y = " << y << ", result = " << result << std::endl;
    return 0;
}