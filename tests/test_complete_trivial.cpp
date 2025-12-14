#include <cassert>
#include <iostream>
#include <type_traits>

// Incluir los tipos básicos y las funciones de asignación
#include "int128_assign.hpp"
#include "int128_factory.hpp"
#include "int128_t.hpp"
#include "uint128_t.hpp"

// =============================================================================
// TESTS DE TRIVIALIDAD COMPLETA
// =============================================================================

void test_complete_triviality()
{
    std::cout << "🎯 Probando TRIVIALIDAD COMPLETA...\n";

    // Test todas las propiedades triviales
    std::cout << "  📋 Propiedades después de eliminar constructores user-defined:\n";
    std::cout
        << "    uint128_t is_trivial: "
        << std::is_trivial_v<uint128_t> << " (esperado: TRUE - ¡SIN constructores user-defined!)\n";
    std::cout << "    uint128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<uint128_t> << " (esperado: true)\n";
    std::cout << "    uint128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     uint128_t> << " (esperado: TRUE - constructor default trivial!)\n";
    std::cout << "    uint128_t is_standard_layout: "
              << std::is_standard_layout_v<uint128_t> << " (esperado: true)\n";

    std::cout
        << "    int128_t is_trivial: "
        << std::is_trivial_v<int128_t> << " (esperado: TRUE - ¡SIN constructores user-defined!)\n";
    std::cout << "    int128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<int128_t> << " (esperado: true)\n";
    std::cout << "    int128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     int128_t> << " (esperado: TRUE - constructor default trivial!)\n";
    std::cout << "    int128_t is_standard_layout: "
              << std::is_standard_layout_v<int128_t> << " (esperado: true)\n";

    // Verificar TRIVIALIDAD COMPLETA
    assert(std::is_trivial_v<uint128_t> && "uint128_t debe ser COMPLETAMENTE TRIVIAL");
    assert(std::is_trivially_copyable_v<uint128_t> && "uint128_t debe ser trivialmente copiable");
    assert(std::is_trivially_default_constructible_v<uint128_t> &&
           "uint128_t debe tener constructor default trivial");
    assert(std::is_standard_layout_v<uint128_t> && "uint128_t debe tener layout estándar");

    assert(std::is_trivial_v<int128_t> && "int128_t debe ser COMPLETAMENTE TRIVIAL");
    assert(std::is_trivially_copyable_v<int128_t> && "int128_t debe ser trivialmente copiable");
    assert(std::is_trivially_default_constructible_v<int128_t> &&
           "int128_t debe tener constructor default trivial");
    assert(std::is_standard_layout_v<int128_t> && "int128_t debe tener layout estándar");

    std::cout << "  🎉 ¡TRIVIALIDAD COMPLETA LOGRADA!\n";
}

void test_assign_functions()
{
    std::cout << "🔧 Probando funciones de asignación...\n";

    // Test uint128_t asignaciones
    uint128_t u1{};
    assign_uint128(u1, 0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    assert(u1.high() == 0x1234567890ABCDEFULL);
    assert(u1.low() == 0xFEDCBA0987654321ULL);
    std::cout << "  ✅ assign_uint128(target, high, low) funciona\n";

    uint128_t u2{};
    assign_uint128(u2, 42);
    assert(u2.low() == 42);
    assert(u2.high() == 0);
    std::cout << "  ✅ assign_uint128(target, value) funciona\n";

    uint128_t u3{};
    assign_from_int(u3, 123456);
    assert(u3.low() == 123456);
    assert(u3.high() == 0);
    std::cout << "  ✅ assign_from_int(uint128, value) funciona\n";

    // Test int128_t asignaciones
    int128_t i1{};
    assign_int128(i1, 0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    assert(i1.high() == 0x7FFFFFFFFFFFFFFFULL);
    assert(i1.low() == 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "  ✅ assign_int128(target, high, low) funciona\n";

    int128_t i2{};
    assign_int128(i2, -42);
    assert(i2.is_negative());
    std::cout << "  ✅ assign_int128(target, negative) funciona\n";

    int128_t i3{};
    assign_from_int(i3, 789);
    assert(i3.low() == 789);
    assert(i3.high() == 0);
    assert(!i3.is_negative());
    std::cout << "  ✅ assign_from_int(int128, value) funciona\n";

    // Test asignación desde uint128_t
    int128_t i4{};
    assign_int128(i4, u1);
    assert(i4.high() == u1.high());
    assert(i4.low() == u1.low());
    std::cout << "  ✅ assign_int128(target, uint128) funciona\n";
}

void test_factory_vs_assign()
{
    std::cout << "⚖️  Comparando factory functions vs assign functions...\n";

    // Crear con factory
    auto u_factory = make_uint128(0xDEADBEEFULL, 0xCAFEBABEULL);

    // Crear con assign
    uint128_t u_assign{}; // Constructor trivial
    assign_uint128(u_assign, 0xDEADBEEFULL, 0xCAFEBABEULL);

    assert(u_factory == u_assign);
    std::cout << "  ✅ Factory y assign producen resultados idénticos\n";

    // Verificar que ambas approaches funcionan
    auto i_factory = make_int128(-999);

    int128_t i_assign{}; // Constructor trivial
    assign_int128(i_assign, -999);

    assert(i_factory == i_assign);
    std::cout << "  ✅ Factory y assign compatibles para negativos\n";
}

void test_memory_layout()
{
    std::cout << "💾 Verificando layout de memoria con tipos triviales...\n";

    std::cout << "  📏 Tamaños: uint128_t=" << sizeof(uint128_t)
              << ", int128_t=" << sizeof(int128_t) << " bytes\n";
    std::cout << "  📐 Alineación: uint128_t=" << alignof(uint128_t)
              << ", int128_t=" << alignof(int128_t) << " bytes\n";

    assert(sizeof(uint128_t) == 16);
    assert(sizeof(int128_t) == 16);
    assert(alignof(uint128_t) == 8);
    assert(alignof(int128_t) == 8);

    std::cout << "  ✅ Layout de memoria correcto para tipos triviales\n";
}

int main()
{
    std::cout << "🚀 Tests de TRIVIALIDAD COMPLETA\n";
    std::cout << "======================================\n";

    test_complete_triviality();
    test_assign_functions();
    test_factory_vs_assign();
    test_memory_layout();

    std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!\n";
    std::cout << "======================================\n";
    std::cout << "✅ TRIVIALIDAD COMPLETA lograda\n";
    std::cout << "✅ Funciones assign funcionando\n";
    std::cout << "✅ Compatibilidad con factory functions\n";
    std::cout << "✅ Optimizaciones del compilador habilitadas\n";

    return 0;
}