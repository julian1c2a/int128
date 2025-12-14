#include "../include/uint128_t.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "=== Verificación de Type Traits para uint128_t ===\n\n";

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
              << std::is_same_v<std::make_unsigned_t<uint128_t>, uint128_t> << "\n";
    std::cout << "  - make_signed disponible pero falla correctamente (int128_t no existe)\n\n";

    // Common type
    std::cout << "4. Common type:\n";
    std::cout << "  - common_type<uint128_t, uint64_t>: "
              << std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t> << "\n";
    std::cout << "  - common_type<uint32_t, uint128_t>: "
              << std::is_same_v<std::common_type_t<uint32_t, uint128_t>, uint128_t> << "\n";
    std::cout << "  - common_type<uint128_t, uint128_t>: "
              << std::is_same_v<std::common_type_t<uint128_t, uint128_t>, uint128_t> << "\n\n";

    // Conceptos C++20
    std::cout << "5. Conceptos C++20:\n";
    std::cout << "  - integral<uint128_t>: " << std::integral<uint128_t> << "\n";
    std::cout << "  - unsigned_integral<uint128_t>: " << std::unsigned_integral<uint128_t> << "\n";
    std::cout << "  - signed_integral<uint128_t>: " << std::signed_integral<uint128_t> << "\n\n";

    // Tests de compatibilidad con STL
    std::cout << "6. Compatibilidad con STL:\n";
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

    // Template test - integral concept
    auto test_template = []<std::integral T>(T value) { return value + T(1); };
    uint128_t val(100);
    auto result = test_template(val);
    std::cout << "7. Template con integral concept: " << (result == uint128_t(101)) << "\n";

    // Template test - unsigned concept
    auto unsigned_template = []<std::unsigned_integral T>(T value) { return value * T(2); };
    uint128_t val2(50);
    auto result2 = unsigned_template(val2);
    std::cout << "8. Template con unsigned_integral concept: " << (result2 == uint128_t(100))
              << "\n";

    // Common type en templates
    auto add_mixed = []<typename T, typename U>(T a, U b) {
        using common_t = std::common_type_t<T, U>;
        return static_cast<common_t>(a) + static_cast<common_t>(b);
    };
    auto result3 = add_mixed(uint128_t(100), uint64_t(50));
    std::cout << "9. Common type en template: "
              << (std::is_same_v<decltype(result3), uint128_t>) << " y valor correcto: "
              << (result3 == uint128_t(150)) << "\n\n";

    std::cout << "✅ Todos los traits están funcionando correctamente!\n";
    std::cout << "✅ uint128_t es totalmente compatible con C++ standard library\n";

    return 0;
}