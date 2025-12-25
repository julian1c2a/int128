#include "../include/uint128/uint128_t.hpp"
#include "../include/uint128/uint128_traits_specializations.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "is_integral<uint128_t>: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "UINT128_USING_LIBCPP: " << UINT128_USING_LIBCPP << "\n";
    return 0;
}
