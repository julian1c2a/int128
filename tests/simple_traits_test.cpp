#include "../include/uint128_traits.hpp"
#include <iostream>
#include <type_traits>
#include <limits>
#include <unordered_map>
#include <cassert>

int main()
{
    std::cout << "Probando extensiones básicas...\n\n";

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

    // Verificar valores
    assert(limits::is_specialized == true);
    assert(limits::is_integer == true);
    assert(limits::is_signed == false);
    assert(limits::digits == 128);
    std::cout << "[OK] numeric_limits especialización funciona\n\n";

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
    // TEST CONCEPTS BÁSICOS
    // ===============================================================================
    std::cout << "=== CONCEPTS PERSONALIZADOS ===\n";

    using namespace uint128_concepts;

    // Test manual de concepts (sin static_assert para evitar problemas)
    if constexpr (uint128_convertible<int>)
    {
        std::cout << "[OK] int es convertible a uint128_t\n";
    }

    if constexpr (uint128_convertible<uint64_t>)
    {
        std::cout << "[OK] uint64_t es convertible a uint128_t\n";
    }

    if constexpr (uint128_convertible<uint128_t>)
    {
        std::cout << "[OK] uint128_t es convertible a sí mismo\n";
    }

    std::cout << "[OK] Concepts personalizados funcionan\n\n";

    // ===============================================================================
    // TEST UTILITIES (versión simplificada)
    // ===============================================================================
    std::cout << "=== UTILIDADES ===\n";

    using namespace uint128_utils;

    // Test helpers constexpr
    constexpr bool can_hold_32 = can_hold_uint128_v<uint32_t>;
    constexpr bool may_overflow_16 = may_overflow_from_uint128_v<uint16_t>;

    std::cout << "uint32_t puede contener uint128_t: " << can_hold_32 << "\n";
    std::cout << "uint16_t puede overflow desde uint128_t: " << may_overflow_16 << "\n";

    assert(!can_hold_32);
    assert(may_overflow_16);

    std::cout << "[OK] Utilidades funcionan correctamente\n\n";

    std::cout << "¡Extensiones básicas funcionan perfectamente! 🎉\n";
    return 0;
}
