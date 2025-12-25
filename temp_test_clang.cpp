#include "include/uint128/uint128_limits.hpp"
#include "include/uint128/uint128_t.hpp"
#include "include/uint128/uint128_traits.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "Testing uint128_t traits with Clang:\n";
    std::cout << "UINT128_USING_LIBCPP: " << UINT128_USING_LIBCPP << "\n";
    std::cout << "std::is_integral_v<uint128_t>: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "std::is_arithmetic_v<uint128_t>: " << std::is_arithmetic_v<uint128_t> << "\n";
    std::cout << "std::is_unsigned_v<uint128_t>: " << std::is_unsigned_v<uint128_t> << "\n";

    // Verificar si la especialización existe
    std::cout << "\nVerificando especializaciones:\n";
    std::cout << "is_integral<int>: " << std::is_integral_v<int> << "\n";
    std::cout << "is_integral<uint64_t>: " << std::is_integral_v<uint64_t> << "\n";

    return 0;
}
