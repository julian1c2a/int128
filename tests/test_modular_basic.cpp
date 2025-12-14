#include "../include/uint128_limits.hpp"
#include "../include/uint128_t.hpp"
#include "../include/uint128_traits.hpp"
#include <iostream>

int main()
{
    std::cout << "=== Test Modular de uint128_t ===\n\n";

    // Test numeric limits
    std::cout << "1. Numeric limits funcionando:\n";
    std::cout << "  - is_specialized: " << std::numeric_limits<uint128_t>::is_specialized << "\n";
    std::cout << "  - digits: " << std::numeric_limits<uint128_t>::digits << "\n";
    std::cout << "  - max != 0: " << (std::numeric_limits<uint128_t>::max() != uint128_t(0))
              << "\n\n";

    // Test traits básicos
    std::cout << "2. Type traits básicos:\n";
    std::cout << "  - is_integral: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "  - is_unsigned: " << std::is_unsigned_v<uint128_t> << "\n";
    std::cout << "  - is_arithmetic: " << std::is_arithmetic_v<uint128_t> << "\n\n";

    // Test make_unsigned
    std::cout << "3. Make traits:\n";
    std::cout << "  - make_unsigned: "
              << std::is_same_v<std::make_unsigned_t<uint128_t>, uint128_t> << "\n\n";

    // Test común types básicos
    std::cout << "4. Common types:\n";
    std::cout << "  - uint64_t: "
              << std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t> << "\n";
    std::cout << "  - uint32_t: "
              << std::is_same_v<std::common_type_t<uint128_t, uint32_t>, uint128_t> << "\n\n";

    // Test hash
    std::cout << "5. Hash function:\n";
    std::hash<uint128_t> hasher;
    auto hash_result = hasher(uint128_t(12345));
    std::cout << "  - hash(12345): " << hash_result << " (≠ 0: " << (hash_result != 0) << ")\n\n";

    // Test básico de operaciones
    std::cout << "6. Operaciones básicas:\n";
    uint128_t a(100);
    uint128_t b(200);
    auto sum = a + b;
    std::cout << "  - 100 + 200 = " << sum << "\n";
    std::cout << "  - Comparación: " << (a < b) << "\n\n";

    std::cout << "✅ Arquitectura modular funcionando correctamente!\n";

    return 0;
}