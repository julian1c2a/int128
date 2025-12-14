#include "../include/uint128_t.hpp"
#include <iostream>

int main()
{
    std::cout << "=== Test básico de Type Traits para uint128_t ===\n\n";

    std::cout << "1. is_integral<uint128_t>: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "2. is_unsigned<uint128_t>: " << std::is_unsigned_v<uint128_t> << "\n";
    std::cout << "3. is_arithmetic<uint128_t>: " << std::is_arithmetic_v<uint128_t> << "\n";
    std::cout << "4. Numeric limits specialized: " << std::numeric_limits<uint128_t>::is_specialized
              << "\n";
    std::cout << "5. Numeric limits max != 0: "
              << (std::numeric_limits<uint128_t>::max() != uint128_t(0)) << "\n";

    // Test específico de nuestras especializaciones directamente
    std::cout << "\n=== Tests directos de especializaciones ===\n";
    std::cout << "1. std::is_integral<uint128_t>::value: " << std::is_integral<uint128_t>::value
              << "\n";
    std::cout << "2. std::is_unsigned<uint128_t>::value: " << std::is_unsigned<uint128_t>::value
              << "\n";
    std::cout << "3. std::is_arithmetic<uint128_t>::value: " << std::is_arithmetic<uint128_t>::value
              << "\n";

    return 0;
}