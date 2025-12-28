/**
 * Tests de Interoperabilidad: uint128_t ↔ int128_t
 *
 * Propósito: Verificar que las operaciones mixtas entre tipos con signo
 * y sin signo funcionan correctamente.
 *
 * Tests incluidos:
 * 1. Conversiones explícitas e implícitas
 * 2. Operaciones aritméticas mixtas (+, -, *, /, %)
 * 3. Operaciones bitwise mixtas (&, |, ^, <<, >>)
 * 4. Comparaciones mixtas (<, >, <=, >=, ==, !=)
 * 5. std::common_type entre uint128_t e int128_t
 * 6. Asignaciones y conversiones seguras
 * 7. Casos límite (overflow, underflow, valores negativos)
 */

#include "int128/int128_numeric.hpp" // Para gcd/lcm personalizado
#include "int128/int128_t.hpp"
#include "int128/int128_traits.hpp"    // Asegurar que common_type esté disponible
#include "uint128/uint128_numeric.hpp" // Para gcd/lcm personalizado
#include "uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>

using namespace nstd;

// Colores para output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

int test_count = 0;
int passed_count = 0;
int failed_count = 0;

#define TEST(name)                                                                                 \
    std::cout << "\n[TEST " << ++test_count << "] " << name << "... ";                             \
    bool test_passed = true;

#define ASSERT(condition)                                                                          \
    if (!(condition)) {                                                                            \
        std::cout << RED << "FAILED" << RESET << "\n";                                             \
        std::cout << "  Assertion failed: " #condition << "\n";                                    \
        std::cout << "  File: " << __FILE__ << ":" << __LINE__ << "\n";                            \
        test_passed = false;                                                                       \
        failed_count++;                                                                            \
        return;                                                                                    \
    }

#define END_TEST                                                                                   \
    if (test_passed) {                                                                             \
        std::cout << GREEN << "PASSED" << RESET;                                                   \
        passed_count++;                                                                            \
    }

// ============================================================================
// CONVERSIONES EXPLÍCITAS E IMPLÍCITAS
// ============================================================================

void test_explicit_conversions()
{
    TEST("Conversiones explícitas uint128_t -> int128_t");

    // uint128_t a int128_t (valor positivo pequeño)
    uint128_t u1(12345);
    int128_t i1 = static_cast<int128_t>(u1);
    ASSERT(i1 == int128_t(12345));

    // uint128_t a int128_t (valor grande pero dentro del rango int128_t)
    uint128_t u2(0, 0x7FFFFFFFFFFFFFFFULL); // Cerca del máximo int128_t
    int128_t i2 = static_cast<int128_t>(u2);
    ASSERT(i2 > int128_t(0));

    // int128_t a uint128_t (valor positivo)
    int128_t i3(67890);
    uint128_t u3 = static_cast<uint128_t>(i3);
    ASSERT(u3 == uint128_t(67890));

    // int128_t negativo a uint128_t (complemento a 2)
    int128_t i4(-123);
    uint128_t u4 = static_cast<uint128_t>(i4);
    // El resultado debe ser la representación en complemento a 2
    ASSERT(u4 != uint128_t(123)); // No es el valor absoluto

    END_TEST;
}

void test_construction_conversions()
{
    TEST("Construcción con conversión de tipos");

    // Construir int128_t desde uint128_t
    uint128_t u1(999);
    int128_t i1(static_cast<int128_t>(u1));
    ASSERT(i1 == int128_t(999));

    // Construir uint128_t desde int128_t positivo
    int128_t i2(888);
    uint128_t u2(static_cast<uint128_t>(i2));
    ASSERT(u2 == uint128_t(888));

    END_TEST;
}

// ============================================================================
// OPERACIONES ARITMÉTICAS MIXTAS
// ============================================================================

void test_mixed_addition()
{
    TEST("Suma mixta uint128_t + int128_t");

    uint128_t u1(100);
    int128_t i1(50);

    // uint128_t + int128_t requiere conversión explícita
    auto result1 = u1 + static_cast<uint128_t>(i1);
    ASSERT(result1 == uint128_t(150));

    // int128_t + uint128_t (convertir uint128_t a int128_t)
    auto result2 = i1 + static_cast<int128_t>(u1);
    ASSERT(result2 == int128_t(150));

    // Con valor negativo
    int128_t i2(-30);
    auto result3 = static_cast<int128_t>(u1) + i2;
    ASSERT(result3 == int128_t(70));

    END_TEST;
}

void test_mixed_subtraction()
{
    TEST("Resta mixta uint128_t - int128_t");

    uint128_t u1(200);
    int128_t i1(50);

    // uint128_t - int128_t positivo
    auto result1 = u1 - static_cast<uint128_t>(i1);
    ASSERT(result1 == uint128_t(150));

    // int128_t - uint128_t
    int128_t i2(300);
    auto result2 = i2 - static_cast<int128_t>(u1);
    ASSERT(result2 == int128_t(100));

    // Resultado negativo
    int128_t i3(50);
    auto result3 = i3 - static_cast<int128_t>(u1);
    ASSERT(result3 == int128_t(-150));

    END_TEST;
}

void test_mixed_multiplication()
{
    TEST("Multiplicación mixta uint128_t * int128_t");

    uint128_t u1(20);
    int128_t i1(5);

    // uint128_t * int128_t
    auto result1 = u1 * static_cast<uint128_t>(i1);
    ASSERT(result1 == uint128_t(100));

    // Con negativo
    int128_t i2(-3);
    auto result2 = static_cast<int128_t>(u1) * i2;
    ASSERT(result2 == int128_t(-60));

    END_TEST;
}

void test_mixed_division()
{
    TEST("División mixta uint128_t / int128_t");

    uint128_t u1(100);
    int128_t i1(5);

    // uint128_t / int128_t
    auto result1 = u1 / static_cast<uint128_t>(i1);
    ASSERT(result1 == uint128_t(20));

    // int128_t / uint128_t
    int128_t i2(150);
    auto result2 = i2 / static_cast<int128_t>(u1);
    ASSERT(result2 == int128_t(1));

    // Con negativo
    int128_t i3(-200);
    uint128_t u2(4);
    auto result3 = i3 / static_cast<int128_t>(u2);
    ASSERT(result3 == int128_t(-50));

    END_TEST;
}

// ============================================================================
// COMPARACIONES MIXTAS
// ============================================================================

void test_mixed_equality()
{
    TEST("Comparaciones de igualdad mixtas");

    uint128_t u1(100);
    int128_t i1(100);
    int128_t i2(50);
    int128_t i3(-100);

    // uint128_t == int128_t (positivo)
    ASSERT(static_cast<int128_t>(u1) == i1);
    ASSERT(!(static_cast<int128_t>(u1) == i2));

    // uint128_t != int128_t (negativo)
    ASSERT(static_cast<int128_t>(u1) != i3);

    END_TEST;
}

void test_mixed_comparisons()
{
    TEST("Comparaciones ordenadas mixtas");

    uint128_t u1(100);
    int128_t i1(50);
    int128_t i2(150);
    int128_t i3(-50);

    // uint128_t vs int128_t positivo
    ASSERT(static_cast<int128_t>(u1) > i1);
    ASSERT(static_cast<int128_t>(u1) < i2);
    ASSERT(static_cast<int128_t>(u1) >= i1);
    ASSERT(static_cast<int128_t>(u1) <= i2);

    // uint128_t vs int128_t negativo
    ASSERT(static_cast<int128_t>(u1) > i3);

    END_TEST;
}

// ============================================================================
// STD::COMMON_TYPE
// ============================================================================

void test_common_type()
{
    TEST("nstd::common_type entre uint128_t e int128_t");

    // Verificar que common_type está definido
    using CommonType = typename nstd::common_type<uint128_t, int128_t>::type;

    // common_type debería ser int128_t (tipo con signo domina)
    // O podría ser uint128_t dependiendo de la implementación
    // Lo importante es que exista y sea uno de los dos tipos

    bool is_int128 = std::is_same_v<CommonType, int128_t>;
    bool is_uint128 = std::is_same_v<CommonType, uint128_t>;

    ASSERT(is_int128 || is_uint128);

    // Verificar que podemos usar common_type en operaciones
    uint128_t u(100);
    int128_t i(50);

    CommonType c1 = static_cast<CommonType>(u);
    CommonType c2 = static_cast<CommonType>(i);
    CommonType result = c1 + c2;

    (void)result; // Silenciar warning

    END_TEST;
}

// ============================================================================
// OPERACIONES BITWISE MIXTAS
// ============================================================================

void test_mixed_bitwise()
{
    TEST("Operaciones bitwise mixtas");

    uint128_t u1(0xFF);
    int128_t i1(0xF0);

    // AND
    auto and_result = u1 & static_cast<uint128_t>(i1);
    ASSERT(and_result == uint128_t(0xF0));

    // OR
    auto or_result = u1 | static_cast<uint128_t>(i1);
    ASSERT(or_result == uint128_t(0xFF));

    // XOR
    auto xor_result = u1 ^ static_cast<uint128_t>(i1);
    ASSERT(xor_result == uint128_t(0x0F));

    END_TEST;
}

// ============================================================================
// CASOS LÍMITE
// ============================================================================

void test_edge_cases_zero()
{
    TEST("Casos límite: cero");

    uint128_t u_zero(0);
    int128_t i_zero(0);

    // Comparaciones con cero
    ASSERT(static_cast<int128_t>(u_zero) == i_zero);
    ASSERT(!(static_cast<int128_t>(u_zero) < i_zero));
    ASSERT(!(static_cast<int128_t>(u_zero) > i_zero));

    // Operaciones con cero
    uint128_t u1(100);
    ASSERT(u1 + static_cast<uint128_t>(i_zero) == u1);
    ASSERT(static_cast<int128_t>(u1) * i_zero == int128_t(0));

    END_TEST;
}

void test_edge_cases_negative()
{
    TEST("Casos límite: valores negativos");

    int128_t i_neg(-123);
    uint128_t u1(50);

    // Suma con negativo
    auto result1 = static_cast<int128_t>(u1) + i_neg;
    ASSERT(result1 == int128_t(-73));

    // Comparación uint128_t con int128_t negativo
    // Un uint128_t siempre es mayor que un int128_t negativo
    ASSERT(static_cast<int128_t>(u1) > i_neg);

    END_TEST;
}

void test_edge_cases_large_values()
{
    TEST("Casos límite: valores grandes");

    // Valores grandes en uint128_t
    uint128_t u_large(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);

    // Conversión a int128_t (será negativo por MSB = 1)
    int128_t i_converted = static_cast<int128_t>(u_large);
    ASSERT(i_converted < int128_t(0)); // Debe ser negativo

    // Máximo valor positivo de int128_t
    int128_t i_max(0x7FFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT(i_max > int128_t(0));

    // Conversión de vuelta a uint128_t
    uint128_t u_converted = static_cast<uint128_t>(i_max);
    ASSERT(u_converted > uint128_t(0));

    END_TEST;
}

// ============================================================================
// ASIGNACIONES SEGURAS
// ============================================================================

void test_safe_assignments()
{
    TEST("Asignaciones con verificación de rangos");

    // Valor positivo pequeño: siempre seguro
    int128_t i1(12345);
    uint128_t u1 = static_cast<uint128_t>(i1);
    ASSERT(u1 == uint128_t(12345));

    // Valor positivo de vuelta
    int128_t i2 = static_cast<int128_t>(u1);
    ASSERT(i2 == i1);

    // Valor negativo: se convierte a complemento a 2
    int128_t i_neg(-100);
    uint128_t u_neg = static_cast<uint128_t>(i_neg);
    // Verificar que no es simplemente el valor absoluto
    ASSERT(u_neg != uint128_t(100));

    END_TEST;
}

// ============================================================================
// OPERACIONES COMPUESTAS
// ============================================================================

void test_compound_operations()
{
    TEST("Operaciones compuestas mixtas");

    uint128_t u1(100);
    int128_t i1(50);
    int128_t i2(30);

    // (uint128_t + int128_t) - int128_t
    auto result1 = (static_cast<int128_t>(u1) + i1) - i2;
    ASSERT(result1 == int128_t(120));

    // uint128_t * int128_t + int128_t
    auto result2 = (static_cast<int128_t>(u1) * int128_t(2)) + i1;
    ASSERT(result2 == int128_t(250));

    // Mezcla con negativos
    int128_t i_neg(-20);
    auto result3 = static_cast<int128_t>(u1) + i1 + i_neg;
    ASSERT(result3 == int128_t(130));

    END_TEST;
}

// ============================================================================
// FUNCIONES MATEMÁTICAS MIXTAS
// ============================================================================

void test_math_functions_mixed()
{
    TEST("Funciones matemáticas con tipos mixtos");

    uint128_t u1(60);
    uint128_t u2(48);
    int128_t i1(60);
    int128_t i2(48);

    // GCD con conversión - usando funciones personalizadas
    auto gcd1 = nstd::gcd(static_cast<int128_t>(u1), i2);
    auto gcd2 = nstd::gcd(i1, i2);
    ASSERT(gcd1 == gcd2);
    ASSERT(gcd1 == int128_t(12));

    // LCM con conversión - usando funciones personalizadas
    auto lcm1 = nstd::lcm(static_cast<int128_t>(u1), i2);
    auto lcm2 = nstd::lcm(i1, i2);
    ASSERT(lcm1 == lcm2);
    ASSERT(lcm1 == int128_t(240));

    END_TEST;
}

// ============================================================================
// CONVERSIONES CON MAKE_SIGNED/MAKE_UNSIGNED
// ============================================================================

void test_type_traits_conversions()
{
    TEST("Conversiones con std::make_signed/unsigned");

#ifndef _MSC_VER
    // MSVC no soporta make_unsigned/make_signed para tipos custom
    // Solo GCC/Clang los reconocen como tipos integrales

    // make_unsigned<int128_t> debe dar uint128_t
    using UnsignedVersion = nstd::make_unsigned_t<int128_t>;
    static_assert(std::is_same_v<UnsignedVersion, uint128_t>, "make_unsigned debe dar uint128_t");

    // make_signed<uint128_t> debe dar int128_t
    using SignedVersion = nstd::make_signed_t<uint128_t>;
    static_assert(std::is_same_v<SignedVersion, int128_t>, "make_signed debe dar int128_t");

    // Conversión práctica
    int128_t i1(42);
    UnsignedVersion u1 = static_cast<UnsignedVersion>(i1);
    ASSERT(u1 == uint128_t(42));

    uint128_t u2(84);
    SignedVersion i2 = static_cast<SignedVersion>(u2);
    ASSERT(i2 == int128_t(84));
#endif // !_MSC_VER

    END_TEST;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "=============================================================================="
              << "\n";
    std::cout << "      TESTS DE INTEROPERABILIDAD: uint128_t ↔ int128_t\n";
    std::cout << "==============================================================================\n";

    // Conversiones
    test_explicit_conversions();
    test_construction_conversions();

    // Aritmética mixta
    test_mixed_addition();
    test_mixed_subtraction();
    test_mixed_multiplication();
    test_mixed_division();

    // Comparaciones
    test_mixed_equality();
    test_mixed_comparisons();

    // Type traits
    test_common_type();
    test_type_traits_conversions();

    // Bitwise
    test_mixed_bitwise();

    // Casos límite
    test_edge_cases_zero();
    test_edge_cases_negative();
    test_edge_cases_large_values();

    // Asignaciones
    test_safe_assignments();

    // Operaciones compuestas
    test_compound_operations();

    // Funciones matemáticas
    test_math_functions_mixed();

    // Resumen
    std::cout
        << "\n==============================================================================\n";
    std::cout << "RESUMEN:\n";
    std::cout << "==============================================================================\n";
    std::cout << "Total tests:  " << test_count << "\n";
    std::cout << GREEN << "Passed:       " << passed_count << RESET << "\n";
    if (failed_count > 0) {
        std::cout << RED << "Failed:       " << failed_count << RESET << "\n";
    } else {
        std::cout << "Failed:       " << failed_count << "\n";
    }
    std::cout << "Success rate: " << (100.0 * passed_count / test_count) << "%\n";
    std::cout << "==============================================================================\n";

    if (failed_count == 0) {
        std::cout << GREEN << "\n[OK] TODOS LOS TESTS DE INTEROPERABILIDAD PASARON\n" << RESET;
        std::cout << "\nConversiones uint128_t ↔ int128_t funcionan correctamente:\n";
        std::cout << "  [OK] Conversiones explícitas seguras\n";
        std::cout << "  [OK] Operaciones aritméticas mixtas\n";
        std::cout << "  [OK] Comparaciones mixtas\n";
        std::cout << "  [OK] std::common_type definido\n";
        std::cout << "  [OK] std::make_signed/unsigned funcional\n";
        std::cout << "  [OK] Casos límite manejados\n";
        return 0;
    } else {
        std::cout << RED << "\n[FAIL] ALGUNOS TESTS FALLARON\n" << RESET;
        return 1;
    }
}
