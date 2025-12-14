#include <type_traits>

// Forward declare uint128_t
struct uint128_t;

namespace std
{
// Especialización para is_integral
template <> struct is_integral<uint128_t> : std::true_type {
};
} // namespace std

#include "../include/uint128_t.hpp"
#include <iostream>

int main()
{
    std::cout << "1. is_integral<uint128_t>: " << std::is_integral_v<uint128_t> << "\n";
    return 0;
}