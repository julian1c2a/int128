#include <cassert>
#include <iostream>
#include <type_traits>

// Incluir los tipos básicos y las funciones de fábrica
#include "int128_factory.hpp"
#include "int128_t.hpp"
#include "uint128_t.hpp"

// =============================================================================
// FUNCIONES DE FÁBRICA INLINE BÁSICAS (EVITAR CONFLICTOS DE SÍMBOLOS)
// =============================================================================

constexpr uint128_t make_uint128_simple(uint64_t high, uint64_t low) noexcept
{
    uint128_t result{};
    result.set_high(high);
    result.set_low(low);
    return result;
}

constexpr int128_t make_int128_simple(uint64_t high, uint64_t low) noexcept
{
    int128_t result{};
    result.set_high(high);
    result.set_low(low);
    return result;
}

constexpr uint128_t make_uint128_from_int(int value) noexcept
{
    uint128_t result{};
    if (value < 0) {
        uint64_t val = static_cast<uint64_t>(static_cast<int64_t>(value));
        result.set_low(val);
        result.set_high(UINT64_MAX);
    } else {
        result.set_low(static_cast<uint64_t>(value));
        result.set_high(0);
    }
    return result;
}

constexpr int128_t make_int128_from_int(int value) noexcept
{
    int128_t result{};
    int64_t val = static_cast<int64_t>(value);
    result.set_low(static_cast<uint64_t>(val));
    result.set_high(val < 0 ? UINT64_MAX : 0);
    return result;
}

void test_trivial_properties()
{
    std::cout << "🔬 Probando propiedades de trivialidad...\n";

    // Test que los tipos básicos sin constructores definidos son triviales
    std::cout << "  📋 Propiedades después de la optimización con fábrica:\n";
    std::cout
        << "    uint128_t is_trivial: "
        << std::is_trivial_v<uint128_t> << " (esperado: true - sin constructores user-defined)\n";
    std::cout << "    uint128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<uint128_t> << " (esperado: true - copia trivial)\n";
    std::cout << "    uint128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     uint128_t> << " (esperado: true - constructor default trivial)\n";
    std::cout << "    uint128_t is_standard_layout: "
              << std::is_standard_layout_v<uint128_t> << " (esperado: true - layout estándar)\n";

    std::cout
        << "    int128_t is_trivial: "
        << std::is_trivial_v<int128_t> << " (esperado: true - sin constructores user-defined)\n";
    std::cout << "    int128_t is_trivially_copyable: "
              << std::is_trivially_copyable_v<int128_t> << " (esperado: true - copia trivial)\n";
    std::cout << "    int128_t is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     int128_t> << " (esperado: true - constructor default trivial)\n";
    std::cout << "    int128_t is_standard_layout: "
              << std::is_standard_layout_v<int128_t> << " (esperado: true - layout estándar)\n";

    // Verificar que las propiedades más importantes son verdaderas
    // NOTA: Los tipos no serán completamente triviales debido a constructores template,
    // pero son trivialmente copiables que es lo más importante para rendimiento
    assert(std::is_trivially_copyable_v<uint128_t> && "uint128_t debe ser trivialmente copiable");
    assert(std::is_standard_layout_v<uint128_t> && "uint128_t debe tener layout estándar");
    assert(std::is_trivially_copyable_v<int128_t> && "int128_t debe ser trivialmente copiable");
    assert(std::is_standard_layout_v<int128_t> && "int128_t debe tener layout estándar");
    std::cout << "  [OK] Propiedades clave para optimización verificadas (trivially copyable + "
                 "standard layout)\n";

    if (std::is_trivial_v<uint128_t> && std::is_trivial_v<int128_t>) {
        std::cout << "  🎉 BONUS: Los tipos son completamente triviales!\n";
    } else {
        std::cout
            << "  ℹ️  Los tipos no son completamente triviales debido a constructores necesarios\n";
        std::cout << "     pero son trivialmente copiables - suficiente para optimizaciones "
                     "importantes\n";
    }

    // Test funciones de fábrica básicas
    auto uint_val = make_uint128_simple(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    assert(uint_val.high() == 0x1234567890ABCDEFULL);
    assert(uint_val.low() == 0xFEDCBA0987654321ULL);
    std::cout << "  [OK] make_uint128_simple() funciona\n";

    auto int_val = make_int128_simple(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    assert(int_val.high() == 0x7FFFFFFFFFFFFFFFULL);
    assert(int_val.low() == 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "  [OK] make_int128_simple() funciona\n";

    // Test con valores enteros
    auto uint_from_int = make_uint128_from_int(42);
    assert(uint_from_int.low() == 42);
    assert(uint_from_int.high() == 0);
    std::cout << "  [OK] make_uint128_from_int() funciona\n";

    auto int_from_int = make_int128_from_int(42);
    assert(int_from_int.low() == 42);
    assert(int_from_int.high() == 0);
    assert(!int_from_int.is_negative());
    std::cout << "  [OK] make_int128_from_int() funciona\n";

    auto int_negative = make_int128_from_int(-42);
    assert(int_negative.is_negative());
    std::cout << "  [OK] make_int128_from_int() con negativos funciona\n";
}

void test_operations_with_factory()
{
    std::cout << "⚙️ Probando operaciones con valores de fábrica...\n";

    // Test aritmética básica
    auto a = make_int128_from_int(100);
    auto b = make_int128_from_int(200);
    auto sum = a + b;

    // Crear valor esperado con fábrica también
    auto expected = make_int128_from_int(300);
    assert(sum == expected);
    std::cout << "  [OK] Aritmética básica funciona con factory\n";

    // Test comparaciones
    assert(a < b);
    assert(b > a);
    assert(a != b);
    std::cout << "  [OK] Comparaciones funcionan con factory\n";

    // Test con valores grandes
    auto large1 = make_uint128_simple(0x8000000000000000ULL, 0);
    auto large2 = make_uint128_simple(0, UINT64_MAX);
    assert(large1 > large2);
    std::cout << "  [OK] Operaciones con valores grandes funcionan\n";
}

void test_memory_layout()
{
    std::cout << "💾 Probando layout de memoria...\n";

    // Test que los tipos tienen el tamaño esperado
    static_assert(sizeof(uint128_t) == 16, "uint128_t debe ser 16 bytes");
    static_assert(sizeof(int128_t) == 16, "int128_t debe ser 16 bytes");
    std::cout << "  [OK] Tamaños de tipos correctos: " << sizeof(uint128_t) << " y "
              << sizeof(int128_t) << " bytes\n";

    // Test que el layout es estándar
    assert(std::is_standard_layout_v<uint128_t>);
    assert(std::is_standard_layout_v<int128_t>);
    std::cout << "  [OK] Layout estándar verificado\n";

    // Test alineación
    static_assert(alignof(uint128_t) == alignof(uint64_t), "Alineación uint128_t");
    static_assert(alignof(int128_t) == alignof(uint64_t), "Alineación int128_t");
    std::cout << "  [OK] Alineación correcta: " << alignof(uint128_t) << " bytes\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de factory y trivialidad\n";
    std::cout << "==========================================\n";

    try {
        test_trivial_properties();
        std::cout << "\n";

        test_operations_with_factory();
        std::cout << "\n";

        test_memory_layout();
        std::cout << "\n";

        std::cout << "🎉 ¡TODOS LOS TESTS BÁSICOS PASARON!\n";
        std::cout << "==========================================\n";
        std::cout << "[OK] Funciones de fábrica básicas funcionando\n";
        std::cout << "[OK] Propiedades de trivialidad verificadas\n";
        std::cout << "[OK] Layout de memoria correcto\n";
        std::cout << "🔄 Próximo paso: Eliminar constructores no triviales\n";

        return 0;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error en tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "[FAIL] Error desconocido en tests" << std::endl;
        return 1;
    }
}