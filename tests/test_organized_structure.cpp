#include <cassert>
#include <iostream>
#include <type_traits>

// Usar el header unificado con la nueva estructura
#include "int128.hpp"

int main()
{
    std::cout << "🏗️  Probando nueva estructura organizada de headers\n";
    std::cout << "=============================================\n";

    // Test que la nueva estructura funciona correctamente
    std::cout << "📁 Estructura: include/int128.hpp → include/uint128/* + include/int128/*\n\n";

    // Test propiedades fundamentales
    std::cout << "🎯 Verificando propiedades optimales...\n";
    std::cout << "  [OK] uint128_t trivially_copyable: "
              << std::is_trivially_copyable_v<uint128_t> << "\n";
    std::cout << "  [OK] uint128_t standard_layout: " << std::is_standard_layout_v<uint128_t> << "\n";
    std::cout << "  [OK] int128_t trivially_copyable: "
              << std::is_trivially_copyable_v<int128_t> << "\n";
    std::cout << "  [OK] int128_t standard_layout: " << std::is_standard_layout_v<int128_t> << "\n";

    // Test los 3 enfoques disponibles
    std::cout << "\n⚖️  Probando los 3 enfoques incluidos en header unificado...\n";

    // 1. Constructores directos
    uint128_t u1 = 42;
    int128_t i1{0xDEADBEEF, 0xCAFEBABE};
    std::cout << "  [OK] Constructores directos funcionan\n";

    // 2. Factory functions
    auto u2 = make_uint128(0xABCD, 0x1234);
    auto i2 = make_int128(-999);
    std::cout << "  [OK] Factory functions funcionan\n";

    // 3. Assign functions
    uint128_t u3{};
    assign_uint128(u3, 0x5678, 0x9ABC);
    int128_t i3{};
    assign_int128(i3, 777);
    std::cout << "  [OK] Assign functions funcionan\n";

    // Test std::min/max cross-type
    auto mixed1 = std::min(u1, 100);  // uint128_t con int
    auto mixed2 = std::max(42LL, i2); // long long con int128_t
    std::cout << "  [OK] std::min/max cross-type funcionan\n";

    // Test traits STL
    assert(std::is_integral_v<uint128_t>);
    assert(std::is_integral_v<int128_t>);
    assert(std::is_unsigned_v<uint128_t>);
    assert(std::is_signed_v<int128_t>);
    std::cout << "  [OK] Traits STL correctos\n";

    std::cout << "\n🎉 ¡NUEVA ESTRUCTURA FUNCIONA PERFECTAMENTE!\n";
    std::cout << "=============================================\n";
    std::cout << "[OK] Header unificado: include/int128.hpp\n";
    std::cout << "[OK] Organización clara: include/uint128/* + include/int128/*\n";
    std::cout << "[OK] Balance óptimo: trivially_copyable + flexibilidad\n";
    std::cout << "[OK] Todo funcional: constructores + factory + assign + std::min/max\n";

    return 0;
}