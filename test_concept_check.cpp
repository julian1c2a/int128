#include "include/uint128/uint128_t.hpp"
#include <iostream>
#include <type_traits>

using namespace nstd;

int main()
{
    std::cout << "sizeof(uint128_t) = " << sizeof(uint128_t) << std::endl;
    std::cout << "std::is_integral_v<uint128_t> = " << std::is_integral_v<uint128_t> << std::endl;
    std::cout << "sizeof(uint64_t) = " << sizeof(uint64_t) << std::endl;
    std::cout << "sizeof(uint32_t) = " << sizeof(uint32_t) << std::endl;

    // Verificar si uint128_t satisface integral_builtin
    constexpr bool is_builtin = std::is_integral_v<uint128_t> && (sizeof(uint128_t) < 16);
    std::cout << "uint128_t satisfies integral_builtin: " << is_builtin << std::endl;

    return 0;
}
