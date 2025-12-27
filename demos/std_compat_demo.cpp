/**
 * @file std_compat_demo.cpp
 * @brief Demo de std_compat.hpp - Polyfills C++20/C++23
 */

#include "uint128/std_compat.hpp"
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== std_compat.hpp Feature Detection ===\n\n";

    // Feature test macros
    std::cout << "Feature Test Macros:\n";
    std::cout << "  UINT128_HAS_CPP23_BYTESWAP: " << UINT128_HAS_CPP23_BYTESWAP << "\n";
    std::cout << "  UINT128_HAS_CPP20_BITOPS: " << UINT128_HAS_CPP20_BITOPS << "\n";
    std::cout << "  UINT128_HAS_CPP23_CONSTEXPR_CMATH: " << UINT128_HAS_CPP23_CONSTEXPR_CMATH
              << "\n";
    std::cout << "  UINT128_HAS_IS_CONSTANT_EVALUATED: " << UINT128_HAS_IS_CONSTANT_EVALUATED
              << "\n\n";

    // Byteswap demo
    std::cout << "=== std_compat::byteswap ===\n";
    {
        uint16_t x16 = 0x1234;
        uint32_t x32 = 0x12345678;
        uint64_t x64 = 0x123456789ABCDEF0ULL;

        std::cout << std::hex << std::setfill('0');
        std::cout << "  uint16: 0x" << std::setw(4) << x16 << " -> 0x" << std::setw(4)
                  << std_compat::byteswap(x16) << "\n";
        std::cout << "  uint32: 0x" << std::setw(8) << x32 << " -> 0x" << std::setw(8)
                  << std_compat::byteswap(x32) << "\n";
        std::cout << "  uint64: 0x" << std::setw(16) << x64 << " -> 0x" << std::setw(16)
                  << std_compat::byteswap(x64) << "\n";
        std::cout << std::dec << "\n";
    }

    // Constexpr abs demo
    std::cout << "=== std_compat::abs (constexpr) ===\n";
    {
        constexpr int neg = -42;
        constexpr int pos = 42;
        constexpr auto abs_neg = std_compat::abs(neg);
        constexpr auto abs_pos = std_compat::abs(pos);

        std::cout << "  abs(-42) = " << abs_neg << " (constexpr: OK)\n";
        std::cout << "  abs(42) = " << abs_pos << " (constexpr: OK)\n\n";
    }

    // to_underlying demo
    std::cout << "=== std_compat::to_underlying ===\n";
    {
        enum class Color : uint8_t { Red = 1, Green = 2, Blue = 3 };
        constexpr auto red_value = std_compat::to_underlying(Color::Red);

        std::cout << "  Color::Red -> " << static_cast<int>(red_value) << "\n";
        std::cout << "  Color::Green -> "
                  << static_cast<int>(std_compat::to_underlying(Color::Green)) << "\n";
        std::cout << "  Color::Blue -> " << static_cast<int>(std_compat::to_underlying(Color::Blue))
                  << "\n\n";
    }

    // is_constant_evaluated demo
    std::cout << "=== std_compat::is_constant_evaluated ===\n";
    {
        auto check_context = []() constexpr {
            return std_compat::is_constant_evaluated() ? "compile-time" : "runtime";
        };

        constexpr auto compile_time = check_context();
        auto runtime = check_context();

        std::cout << "  Constexpr context: " << compile_time << "\n";
        std::cout << "  Runtime context: " << runtime << "\n\n";
    }

    std::cout << "=== All Demos Completed Successfully ===\n";
    return 0;
}
