#include "../include/uint128_traits.hpp"
#include <iostream>
#include <type_traits>
#include <concepts>
#include <limits>
#include <unordered_map>
#include <optional>
#include <cassert>

int main()
{
    std::cout << "Probando extensiones de traits y concepts...\n\n";

    // ===============================================================================
    // TEST NUMERIC_LIMITS
    // ===============================================================================
    std::cout << "=== NUMERIC_LIMITS ===\n";

    using limits = std::numeric_limits<uint128_t>;
    std::cout << "is_specialized: " << limits::is_specialized << "\n";
    std::cout << "is_integer: " << limits::is_integer << "\n";
    std::cout << "is_signed: " << limits::is_signed << "\n";
    std::cout << "digits: " << limits::digits << "\n";
    std::cout << "digits10: " << limits::digits10 << "\n";

    auto min_val = limits::min();
    auto max_val = limits::max();
    std::cout << "min(): " << min_val.high() << ", " << min_val.low() << "\n";
    std::cout << "max(): " << std::hex << max_val.high() << ", " << max_val.low() << std::dec << "\n\n";

    // ===============================================================================
    // TEST TYPE_TRAITS
    // ===============================================================================
    std::cout << "=== TYPE_TRAITS ===\n";

    std::cout << "is_integral<uint128_t>: " << std::is_integral_v<uint128_t> << "\n";
    std::cout << "is_arithmetic<uint128_t>: " << std::is_arithmetic_v<uint128_t> << "\n";
    std::cout << "is_unsigned<uint128_t>: " << std::is_unsigned_v<uint128_t> << "\n";
    std::cout << "is_signed<uint128_t>: " << std::is_signed_v<uint128_t> << "\n";

    static_assert(std::is_integral_v<uint128_t>, "uint128_t debe ser integral");
    static_assert(std::is_arithmetic_v<uint128_t>, "uint128_t debe ser arithmetic");
    static_assert(std::is_unsigned_v<uint128_t>, "uint128_t debe ser unsigned");
    static_assert(!std::is_signed_v<uint128_t>, "uint128_t no debe ser signed");

    std::cout << "[OK] Todas las especializaciones de type_traits funcionan\n\n";

    // ===============================================================================
    // TEST CONCEPTS PERSONALIZADOS
    // ===============================================================================
    std::cout << "=== CONCEPTS PERSONALIZADOS ===\n";

    using namespace uint128_concepts;

    static_assert(uint128_convertible<int>, "int debe ser convertible");
    static_assert(uint128_convertible<uint64_t>, "uint64_t debe ser convertible");
    static_assert(uint128_convertible<uint128_t>, "uint128_t debe ser convertible a sí mismo");

    static_assert(uint128_arithmetic<int>, "int debe ser arithmetic compatible");
    static_assert(uint128_comparable<uint64_t>, "uint64_t debe ser comparable");
    static_assert(uint128_bitwise<unsigned>, "unsigned debe ser bitwise compatible");

    std::cout << "[OK] Todos los concepts personalizados funcionan\n\n";

    // ===============================================================================
    // TEST HASH (para unordered containers)
    // ===============================================================================
    std::cout << "=== HASH SUPPORT ===\n";

    std::unordered_map<uint128_t, std::string> map;
    uint128_t key1(0x1234, 0x5678);
    uint128_t key2(0xABCD, 0xEF12);

    map[key1] = "First value";
    map[key2] = "Second value";

    assert(map[key1] == "First value");
    assert(map[key2] == "Second value");
    assert(map.size() == 2);

    std::cout << "[OK] Hash support funciona con unordered_map\n\n";

    // ===============================================================================
    // TEST UTILITIES
    // ===============================================================================
    std::cout << "=== UTILIDADES ===\n";

    using namespace uint128_utils;

    // Test safe_cast
    uint128_t large_value(0, UINT32_MAX + 1ULL);
    auto safe_int = safe_cast<int>(large_value);
    auto safe_uint64 = safe_cast<uint64_t>(large_value);

    std::cout << "safe_cast a int (debería fallar): " << safe_int.has_value() << "\n";
    std::cout << "safe_cast a uint64_t (debería funcionar): " << safe_uint64.has_value() << "\n";

    if (safe_uint64)
    {
        std::cout << "Valor convertido: " << *safe_uint64 << "\n";
    }

    // Test constexpr helpers
    static_assert(!can_hold_uint128_v<uint32_t>, "uint32_t no puede contener uint128_t");
    static_assert(may_overflow_from_uint128_v<uint16_t>, "uint16_t puede overflow desde uint128_t");

    std::cout << "[OK] Utilidades funcionan correctamente\n\n";

    // ===============================================================================
    // TEST INTEGRACIÓN CON STL
    // ===============================================================================
    std::cout << "=== INTEGRACIÓN STL ===\n";

    // Verificar que funciona con template functions de STL
    auto min_128 = std::numeric_limits<uint128_t>::min();
    auto max_128 = std::numeric_limits<uint128_t>::max();

    // Test que compile con funciones template genéricas
    auto clamped = std::clamp(uint128_t(500), min_128, uint128_t(1000));
    std::cout << "clamp(500, min, 1000) = " << clamped.low() << "\n";

    std::cout << "[OK] Integración con STL funciona\n\n";

    std::cout << "¡Todas las extensiones funcionan perfectamente! 🎉\n";
    return 0;
}
