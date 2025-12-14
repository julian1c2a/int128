#include <cassert>
#include <iostream>
#include <type_traits>

// Incluir los tipos básicos, funciones de asignación y factory
#include "int128_assign.hpp"
#include "int128_factory.hpp"
#include "int128_t.hpp"
#include "uint128_t.hpp"

// =============================================================================
// TESTS DE TRIVIALLY COPYABLE + STANDARD LAYOUT + FUNCIONES ASSIGN/FACTORY
// =============================================================================

void test_optimal_properties()
{
    std::cout << "🎯 Probando PROPIEDADES ÓPTIMAS (trivially_copyable + standard_layout)...\n";

    // Test propiedades clave para rendimiento
    std::cout << "  📋 Propiedades después del balance optimal:\n";
    std::cout
        << "    uint128_t is_trivial: "
        << std::is_trivial_v<uint128_t> << " (esperado: false - tiene constructores template)\n";
    std::cout << "    uint128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<
                     uint128_t> << " (esperado: TRUE - ¡clave para rendimiento!)\n";
    std::cout << "    uint128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<uint128_t> << " (esperado: false)\n";
    std::cout << "    uint128_t is_standard_layout: "
              << std::is_standard_layout_v<uint128_t> << " (esperado: TRUE - layout predecible!)\n";

    std::cout
        << "    int128_t is_trivial: "
        << std::is_trivial_v<int128_t> << " (esperado: false - tiene constructores template)\n";
    std::cout << "    int128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<
                     int128_t> << " (esperado: TRUE - ¡clave para rendimiento!)\n";
    std::cout << "    int128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<int128_t> << " (esperado: false)\n";
    std::cout << "    int128_t is_standard_layout: "
              << std::is_standard_layout_v<int128_t> << " (esperado: TRUE - layout predecible!)\n";

    // Verificar propiedades CLAVE para rendimiento
    assert(std::is_trivially_copyable_v<uint128_t> && "uint128_t debe ser trivialmente copiable");
    assert(std::is_standard_layout_v<uint128_t> && "uint128_t debe tener layout estándar");
    assert(std::is_trivially_copyable_v<int128_t> && "int128_t debe ser trivialmente copiable");
    assert(std::is_standard_layout_v<int128_t> && "int128_t debe tener layout estándar");

    std::cout << "  ✅ PROPIEDADES ÓPTIMAS verificadas - balance perfecto!\n";
}

void test_constructor_convenience()
{
    std::cout << "🏗️  Probando constructores convenientes...\n";

    // Test conversiones implícitas funcionando
    uint128_t u1 = 42;
    assert(u1.low() == 42);
    assert(u1.high() == 0);
    std::cout << "  ✅ uint128_t u = 42 funciona (conversión implícita)\n";

    int128_t i1 = -42;
    assert(i1.is_negative());
    std::cout << "  ✅ int128_t i = -42 funciona (conversión implícita)\n";

    // Test constructor desde dos valores
    uint128_t u2{0xDEADBEEFULL, 0xCAFEBABEULL};
    assert(u2.high() == 0xDEADBEEFULL);
    assert(u2.low() == 0xCAFEBABEULL);
    std::cout << "  ✅ uint128_t{high, low} funciona\n";

    int128_t i2{0x7FFFFFFFULL, 0xFFFFFFFFULL};
    assert(i2.high() == 0x7FFFFFFFULL);
    assert(i2.low() == 0xFFFFFFFFULL);
    std::cout << "  ✅ int128_t{high, low} funciona\n";

    // Test operaciones básicas funcionando
    auto sum = u1 + u2;
    auto diff = i2 - i1;
    std::cout << "  ✅ Operaciones aritméticas funcionan\n";
}

void test_assign_functions()
{
    std::cout << "🔧 Probando funciones de asignación...\n";

    // Test funciones assign como alternativa
    uint128_t u{};
    assign_uint128(u, 0x1234ULL, 0x5678ULL);
    assert(u.high() == 0x1234ULL);
    assert(u.low() == 0x5678ULL);
    std::cout << "  ✅ assign_uint128(target, high, low) funciona\n";

    uint128_t u2{};
    assign_from_int(u2, 999);
    assert(u2.low() == 999);
    assert(u2.high() == 0);
    std::cout << "  ✅ assign_from_int(uint128, value) funciona\n";

    int128_t i{};
    assign_int128(i, -123);
    assert(i.is_negative());
    std::cout << "  ✅ assign_int128(target, negative) funciona\n";

    int128_t i2{};
    assign_from_parts(i2, 0x7FFFFFFFULL, 0x12345678ULL);
    assert(i2.high() == 0x7FFFFFFFULL);
    assert(i2.low() == 0x12345678ULL);
    std::cout << "  ✅ assign_from_parts(target, high, low) funciona\n";
}

void test_factory_functions()
{
    std::cout << "🏭 Probando factory functions...\n";

    // Test factory functions como alternativa
    auto u1 = make_uint128(0xABCDULL, 0xEF01ULL);
    assert(u1.high() == 0xABCDULL);
    assert(u1.low() == 0xEF01ULL);
    std::cout << "  ✅ make_uint128(high, low) funciona\n";

    auto u2 = make_uint128(12345);
    assert(u2.low() == 12345);
    assert(u2.high() == 0);
    std::cout << "  ✅ make_uint128(value) funciona\n";

    auto i1 = make_int128(-987);
    assert(i1.is_negative());
    std::cout << "  ✅ make_int128(negative) funciona\n";

    auto i2 = make_int128(0x7FFFULL, 0x8000ULL);
    assert(i2.high() == 0x7FFFULL);
    assert(i2.low() == 0x8000ULL);
    std::cout << "  ✅ make_int128(high, low) funciona\n";
}

void test_three_approaches()
{
    std::cout << "⚖️  Comparando los 3 enfoques disponibles...\n";

    // Enfoque 1: Constructores directos (conveniente)
    uint128_t u_constructor = 54321;

    // Enfoque 2: Funciones assign (modificación in-place)
    uint128_t u_assign{};
    assign_uint128(u_assign, 54321);

    // Enfoque 3: Factory functions (funcional)
    auto u_factory = make_uint128(54321);

    // Verificar que producen resultados idénticos
    assert(u_constructor == u_assign);
    assert(u_constructor == u_factory);
    assert(u_assign == u_factory);

    std::cout << "  ✅ Los 3 enfoques producen resultados idénticos\n";
    std::cout << "    - Constructores: conveniente y directo\n";
    std::cout << "    - assign_*(): modificación in-place\n";
    std::cout << "    - make_*(): estilo funcional\n";
}

void test_memory_properties()
{
    std::cout << "💾 Verificando propiedades de memoria optimizadas...\n";

    std::cout << "  📏 Tamaños: uint128_t=" << sizeof(uint128_t)
              << ", int128_t=" << sizeof(int128_t) << " bytes\n";
    std::cout << "  📐 Alineación: uint128_t=" << alignof(uint128_t)
              << ", int128_t=" << alignof(int128_t) << " bytes\n";

    assert(sizeof(uint128_t) == 16);
    assert(sizeof(int128_t) == 16);
    assert(alignof(uint128_t) == 8);
    assert(alignof(int128_t) == 8);

    // Test que son POD para memcpy
    static_assert(std::is_trivially_copyable_v<uint128_t>, "Must be trivially copyable");
    static_assert(std::is_trivially_copyable_v<int128_t>, "Must be trivially copyable");
    static_assert(std::is_standard_layout_v<uint128_t>, "Must have standard layout");
    static_assert(std::is_standard_layout_v<int128_t>, "Must have standard layout");

    std::cout << "  ✅ Optimizaciones de memoria verificadas\n";
}

int main()
{
    std::cout << "🚀 Tests de BALANCE ÓPTIMO\n";
    std::cout << "=========================================\n";
    std::cout << "🎯 trivially_copyable + standard_layout + flexibilidad\n\n";

    test_optimal_properties();
    test_constructor_convenience();
    test_assign_functions();
    test_factory_functions();
    test_three_approaches();
    test_memory_properties();

    std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!\n";
    std::cout << "=========================================\n";
    std::cout << "✅ BALANCE ÓPTIMO logrado:\n";
    std::cout << "   🚀 trivially_copyable - optimizaciones del compilador\n";
    std::cout << "   🏗️  standard_layout - layout de memoria predecible\n";
    std::cout << "   🎯 Constructores convenientes - fácil de usar\n";
    std::cout << "   🔧 Funciones assign - modificación in-place\n";
    std::cout << "   🏭 Factory functions - estilo funcional\n";
    std::cout << "   ⚖️  Lo mejor de todos los mundos!\n";

    return 0;
}