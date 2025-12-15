#include "include/uint128_safe.hpp"
#include "include/uint128_t.hpp"
#include <iostream>
#include <limits>

using namespace uint128_literals;

int main()
{
    auto max_val = std::numeric_limits<uint128_t>::max();
    std::cout << "max_val = " << max_val << std::endl;

    auto safe_add_overflow = uint128_safe::safe_add(max_val, 1_u128);
    std::cout << "safe_add_overflow.is_valid() = " << safe_add_overflow.is_valid() << std::endl;

    // Test with smaller value that should work
    auto safe_add_ok = uint128_safe::safe_add(max_val - 10_u128, 5_u128);
    std::cout << "safe_add_ok.is_valid() = " << safe_add_ok.is_valid() << std::endl;

    return 0;
}