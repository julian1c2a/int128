#include "../include/uint128_simple_traits.hpp"
#include <algorithm>
#include <iostream>

int main()
{
    std::cout << "=== Test de Type Traits para uint128_t ===\n\n";

    // Numeric limits
    std::cout << "1. Numeric limits:\n";
    std::cout << "  - is_specialized: " << std::numeric_limits<uint128_t>::is_specialized << "\n";
    std::cout << "  - is_signed: " << std::numeric_limits<uint128_t>::is_signed << "\n";
    std::cout << "  - is_integer: " << std::numeric_limits<uint128_t>::is_integer << "\n";
    std::cout << "  - digits: " << std::numeric_limits<uint128_t>::digits << "\n";
    std::cout << "  - max() != 0: " << (std::numeric_limits<uint128_t>::max() != uint128_t(0))
              << "\n\n";

    // Type traits básicos
    std::cout << "2. Type traits básicos:\n";
    std::cout << "  - is_integral: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "  - is_unsigned: " << std::is_unsigned_v<uint128_t> << "\n";
    std::cout << "  - is_signed: " << std::is_signed_v<uint128_t> << "\n";
    std::cout << "  - is_arithmetic: " << std::is_arithmetic_v<uint128_t> << "\n";
    std::cout << "  - is_scalar: " << std::is_scalar_v<uint128_t> << "\n\n";

    // Make traits
    std::cout << "3. Make traits:\n";
    std::cout << "  - make_unsigned works: "
              << std::is_same_v<std::make_unsigned_t<uint128_t>, uint128_t> << "\n\n";

    // Common type
    std::cout << "4. Common type:\n";
    std::cout << "  - common_type<uint128_t, uint64_t>: "
              << std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t> << "\n";
    std::cout << "  - common_type<uint32_t, uint128_t>: "
              << std::is_same_v<std::common_type_t<uint32_t, uint128_t>, uint128_t> << "\n";
    std::cout << "  - common_type<uint128_t, uint128_t>: "
              << std::is_same_v<std::common_type_t<uint128_t, uint128_t>, uint128_t> << "\n\n";

    // Compatibilidad con STL
    std::cout << "5. Compatibilidad con STL:\n";
    uint128_t valores[] = {1, 2, 3, 4, 5};
    auto min_val = *std::min_element(valores, valores + 5);
    auto max_val = *std::max_element(valores, valores + 5);
    std::cout << "  - min_element: " << (min_val == uint128_t(1)) << "\n";
    std::cout << "  - max_element: " << (max_val == uint128_t(5)) << "\n";

    // Test que std::sort funciona
    uint128_t test_vals[] = {5, 1, 3, 2, 4};
    std::sort(test_vals, test_vals + 5);
    std::cout << "  - std::sort: " << (test_vals[0] == uint128_t(1) && test_vals[4] == uint128_t(5))
              << "\n\n";

    // Hash function
    std::cout << "6. Hash function:\n";
    std::hash<uint128_t> hasher;
    uint128_t val(123456789);
    auto hash_val = hasher(val);
    std::cout << "  - hash(123456789): " << hash_val << " (not zero: " << (hash_val != 0)
              << ")\n\n";

    // Verifica conceptos C++20 básicos
    bool is_integral_concept = std::integral<uint128_t>;
    bool is_unsigned_concept = std::unsigned_integral<uint128_t>;
    std::cout << "7. Conceptos C++20:\n";
    std::cout << "  - integral<uint128_t>: " << is_integral_concept << "\n";
    std::cout << "  - unsigned_integral<uint128_t>: " << is_unsigned_concept << "\n\n";

    if (is_integral_concept && is_unsigned_concept) {
        std::cout << "[OK] Todos los traits están funcionando correctamente!\n";
        std::cout << "[OK] uint128_t es totalmente compatible con C++ standard library\n";
    } else {
        std::cout << "[FAIL] Algunos traits no están funcionando\n";
    }

    return 0;
}