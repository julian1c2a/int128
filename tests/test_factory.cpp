#include <cassert>
#include <iostream>

// Solo incluir lo mínimo necesario para evitar conflictos de símbolos
#include "include/int128_t.hpp"
#include "include/uint128_t.hpp"

// Declarar solo las funciones que vamos a probar (sin incluir el header completo)
constexpr uint128_t make_uint128(uint64_t high, uint64_t low) noexcept;
constexpr int128_t make_int128(uint64_t high, uint64_t low) noexcept;

void test_uint128_factory()
{
    std::cout << "🏭 Probando funciones de fábrica uint128_t...\n";

    // Test make_uint128 desde valores uint64_t
    auto val1 = make_uint128(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    assert(val1.high() == 0x1234567890ABCDEFULL);
    assert(val1.low() == 0xFEDCBA0987654321ULL);
    std::cout << "  [OK] make_uint128(high, low) funciona\n";

    // Test make_uint128 desde enteros
    auto val2 = make_uint128(42);
    assert(val2.low() == 42);
    assert(val2.high() == 0);
    std::cout << "  [OK] make_uint128(int) funciona\n";

    // Test valores especiales
    auto zero = make_uint128_zero();
    assert(zero.low() == 0 && zero.high() == 0);
    std::cout << "  [OK] make_uint128_zero() funciona\n";

    auto one = make_uint128_one();
    assert(one.low() == 1 && one.high() == 0);
    std::cout << "  [OK] make_uint128_one() funciona\n";

    auto max_val = make_uint128_max();
    assert(max_val.low() == UINT64_MAX && max_val.high() == UINT64_MAX);
    std::cout << "  [OK] make_uint128_max() funciona\n";
}

void test_int128_factory()
{
    std::cout << "🏭 Probando funciones de fábrica int128_t...\n";

    // Test make_int128 desde valores uint64_t
    auto val1 = make_int128(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    assert(val1.high() == 0x7FFFFFFFFFFFFFFFULL);
    assert(val1.low() == 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "  [OK] make_int128(high, low) funciona\n";

    // Test make_int128 desde enteros positivos
    auto val2 = make_int128(42);
    assert(val2.low() == 42);
    assert(val2.high() == 0);
    assert(!val2.is_negative());
    std::cout << "  [OK] make_int128(int positivo) funciona\n";

    // Test make_int128 desde enteros negativos
    auto val3 = make_int128(-42);
    assert(val3.is_negative());
    std::cout << "  [OK] make_int128(int negativo) funciona\n";

    // Test valores especiales
    auto zero = make_int128_zero();
    assert(zero.low() == 0 && zero.high() == 0);
    std::cout << "  [OK] make_int128_zero() funciona\n";

    auto one = make_int128_one();
    assert(one.low() == 1 && one.high() == 0);
    std::cout << "  [OK] make_int128_one() funciona\n";

    auto minus_one = make_int128_minus_one();
    assert(minus_one.is_negative());
    std::cout << "  [OK] make_int128_minus_one() funciona\n";

    auto max_val = make_int128_max();
    assert(max_val.high() == 0x7FFFFFFFFFFFFFFFULL);
    assert(max_val.low() == UINT64_MAX);
    assert(!max_val.is_negative());
    std::cout << "  [OK] make_int128_max() funciona\n";

    auto min_val = make_int128_min();
    assert(min_val.high() == 0x8000000000000000ULL);
    assert(min_val.low() == 0);
    assert(min_val.is_negative());
    std::cout << "  [OK] make_int128_min() funciona\n";
}

void test_conversion_functions()
{
    std::cout << "🔄 Probando funciones de conversión...\n";

    // Test conversión directa int128 <-> uint128
    auto int_val = make_int128(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    auto uint_val = int128_to_uint128(int_val);
    assert(uint_val.high() == 0x1234567890ABCDEFULL);
    assert(uint_val.low() == 0xFEDCBA0987654321ULL);
    std::cout << "  [OK] int128_to_uint128() funciona\n";

    auto back_to_int = uint128_to_int128(uint_val);
    assert(back_to_int.high() == int_val.high());
    assert(back_to_int.low() == int_val.low());
    std::cout << "  [OK] uint128_to_int128() funciona\n";

    // Test conversión segura
    auto positive_int = make_int128(42);
    auto safe_uint = safe_int128_to_uint128(positive_int);
    assert(safe_uint.low() == 42);
    std::cout << "  [OK] safe_int128_to_uint128() funciona\n";

    auto safe_back = safe_uint128_to_int128(safe_uint);
    assert(safe_back.low() == 42);
    assert(!safe_back.is_negative());
    std::cout << "  [OK] safe_uint128_to_int128() funciona\n";
}

void test_alias_functions()
{
    std::cout << "🔗 Probando funciones de alias...\n";

    // Test aliases
    auto uint_alias = uint128(42);
    assert(uint_alias.low() == 42);
    std::cout << "  [OK] uint128() alias funciona\n";

    auto int_alias = int128(42);
    assert(int_alias.low() == 42);
    std::cout << "  [OK] int128() alias funciona\n";
}

void test_backward_compatibility()
{
    std::cout << "🔄 Probando compatibilidad hacia atrás...\n";

    // Test que las funciones factory crean objetos funcionalmente equivalentes
    // a los constructores originales

    // Prueba aritmética básica
    auto a = make_int128(100);
    auto b = make_int128(200);
    auto sum = a + b;

    // Verificar que la suma funciona
    assert(sum == make_int128(300));
    std::cout << "  [OK] Aritmética básica funciona\n";

    // Prueba comparaciones
    assert(a < b);
    assert(b > a);
    assert(a != b);
    std::cout << "  [OK] Comparaciones funcionan\n";

    // Prueba con valores grandes
    auto large1 = make_uint128(0x8000000000000000ULL, 0);
    auto large2 = make_uint128(0, UINT64_MAX);
    assert(large1 > large2);
    std::cout << "  [OK] Operaciones con valores grandes funcionan\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de funciones de fábrica\n";
    std::cout << "==========================================\n";

    try {
        test_uint128_factory();
        std::cout << "\n";

        test_int128_factory();
        std::cout << "\n";

        test_conversion_functions();
        std::cout << "\n";

        test_alias_functions();
        std::cout << "\n";

        test_backward_compatibility();
        std::cout << "\n";

        std::cout << "🎉 ¡TODOS LOS TESTS DE FACTORY PASARON!\n";
        std::cout << "==========================================\n";
        std::cout << "[OK] Funciones de fábrica uint128_t funcionando\n";
        std::cout << "[OK] Funciones de fábrica int128_t funcionando\n";
        std::cout << "[OK] Funciones de conversión funcionando\n";
        std::cout << "[OK] Aliases y compatibilidad funcionando\n";

        return 0;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error en tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "[FAIL] Error desconocido en tests" << std::endl;
        return 1;
    }
}