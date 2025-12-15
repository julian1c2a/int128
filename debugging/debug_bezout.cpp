#include "include/uint128_cmath.hpp"
#include "include/uint128_t.hpp"
#include <iostream>

using namespace uint128_literals;

int main()
{
    uint128_t gcd_result = std::gcd(48_u128, 18_u128);
    std::cout << "GCD(48, 18) = " << gcd_result << std::endl;

    // Manually verify: 48 = 16 * 3, 18 = 6 * 3, GCD = 6
    std::cout << "Expected GCD: 6" << std::endl;

    auto [x, y] = std::bezout_coeffs(48_u128, 18_u128);
    uint128_t result = x * 48_u128 + y * 18_u128;
    std::cout << "Bezout coeffs: x = " << x << ", y = " << y << std::endl;
    std::cout << "x*48 + y*18 = " << result << std::endl;
    std::cout << "Should equal GCD: " << gcd_result << std::endl;

    return 0;
}