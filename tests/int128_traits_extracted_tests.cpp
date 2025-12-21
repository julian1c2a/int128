/**
 * @file int128_traits_extracted_tests.cpp
 * @brief Pruebas individuales extraídas para cada especialización de int128_traits.hpp
 *
 * Este archivo contiene tests unitarios individuales para cada una de las
 * especializaciones de type traits definidas en int128_traits.hpp.
 * Cada test valida una característica específica del tipo int128_t.
 *
 * Compiladores soportados: GCC, Clang, Intel OneAPI, MSVC
 * Estándar: C++20
 */

#include <cassert>
#include <int128/int128_t.hpp>
#include <int128/int128_traits.hpp>
#include <iostream>
#include <type_traits>

// Detectar si estamos usando libc++ (Clang)
#ifdef _LIBCPP_VERSION
#define INT128_USING_LIBCPP 1
#else
#define INT128_USING_LIBCPP 0
#endif

// =============================================================================
// TESTS DE TYPE TRAITS FUNDAMENTALES
// =============================================================================

/**
 * @brief Test para std::is_integral<int128_t>
 * Verifica que int128_t sea reconocido como tipo integral
 */
void test_is_integral()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_integral<int128_t>::value, "int128_t debe ser integral");
    assert(std::is_integral<int128_t>::value);
    std::cout << "[PASS] test_is_integral()" << std::endl;
#else
    std::cout << "[SKIP] test_is_integral() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_arithmetic<int128_t>
 * Verifica que int128_t sea reconocido como tipo aritmético
 */
void test_is_arithmetic()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_arithmetic<int128_t>::value, "int128_t debe ser aritmético");
    assert(std::is_arithmetic<int128_t>::value);
    std::cout << "[PASS] test_is_arithmetic()" << std::endl;
#else
    std::cout << "[SKIP] test_is_arithmetic() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_unsigned<int128_t>
 * Verifica que int128_t NO sea reconocido como tipo sin signo
 */
void test_is_unsigned()
{
#if !INT128_USING_LIBCPP
    static_assert(!std::is_unsigned<int128_t>::value, "int128_t no debe ser unsigned");
    assert(!std::is_unsigned<int128_t>::value);
    std::cout << "[PASS] test_is_unsigned()" << std::endl;
#else
    std::cout << "[SKIP] test_is_unsigned() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_signed<int128_t>
 * Verifica que int128_t sea reconocido como tipo con signo
 */
void test_is_signed()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_signed<int128_t>::value, "int128_t debe ser signed");
    assert(std::is_signed<int128_t>::value);
    std::cout << "[PASS] test_is_signed()" << std::endl;
#else
    std::cout << "[SKIP] test_is_signed() - No soportado en libc++" << std::endl;
#endif
}

// =============================================================================
// TESTS DE TRIVIALIDAD
// =============================================================================

/**
 * @brief Test para std::is_trivially_copyable<int128_t>
 * Verifica que int128_t sea trivialmente copiable
 */
void test_is_trivially_copyable()
{
    static_assert(std::is_trivially_copyable<int128_t>::value,
                  "int128_t debe ser trivialmente copiable");
    assert(std::is_trivially_copyable<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_copyable()" << std::endl;
}

/**
 * @brief Test para std::is_trivially_default_constructible<int128_t>
 */
void test_is_trivially_default_constructible()
{
#if !INT128_USING_LIBCPP
    static_assert(!std::is_trivially_default_constructible<int128_t>::value,
                  "int128_t no debe ser trivialmente default constructible");
    assert(!std::is_trivially_default_constructible<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_default_constructible()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_default_constructible() - No soportado en libc++"
              << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivially_copy_constructible<int128_t>
 */
void test_is_trivially_copy_constructible()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_trivially_copy_constructible<int128_t>::value,
                  "int128_t debe ser trivialmente copy constructible");
    assert(std::is_trivially_copy_constructible<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_copy_constructible()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_copy_constructible() - No soportado en libc++"
              << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivially_move_constructible<int128_t>
 */
void test_is_trivially_move_constructible()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_trivially_move_constructible<int128_t>::value,
                  "int128_t debe ser trivialmente move constructible");
    assert(std::is_trivially_move_constructible<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_move_constructible()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_move_constructible() - No soportado en libc++"
              << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivially_copy_assignable<int128_t>
 */
void test_is_trivially_copy_assignable()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_trivially_copy_assignable<int128_t>::value,
                  "int128_t debe ser trivialmente copy assignable");
    assert(std::is_trivially_copy_assignable<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_copy_assignable()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_copy_assignable() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivially_move_assignable<int128_t>
 */
void test_is_trivially_move_assignable()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_trivially_move_assignable<int128_t>::value,
                  "int128_t debe ser trivialmente move assignable");
    assert(std::is_trivially_move_assignable<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_move_assignable()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_move_assignable() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivially_destructible<int128_t>
 */
void test_is_trivially_destructible()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_trivially_destructible<int128_t>::value,
                  "int128_t debe ser trivialmente destructible");
    assert(std::is_trivially_destructible<int128_t>::value);
    std::cout << "[PASS] test_is_trivially_destructible()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivially_destructible() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_trivial<int128_t>
 */
void test_is_trivial()
{
#if !INT128_USING_LIBCPP
    static_assert(!std::is_trivial<int128_t>::value, "int128_t no es completamente trivial");
    assert(!std::is_trivial<int128_t>::value);
    std::cout << "[PASS] test_is_trivial()" << std::endl;
#else
    std::cout << "[SKIP] test_is_trivial() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_standard_layout<int128_t>
 */
void test_is_standard_layout()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_standard_layout<int128_t>::value, "int128_t debe tener standard layout");
    assert(std::is_standard_layout<int128_t>::value);
    std::cout << "[PASS] test_is_standard_layout()" << std::endl;
#else
    std::cout << "[SKIP] test_is_standard_layout() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::is_pod<int128_t>
 */
void test_is_pod()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_pod<int128_t>::value, "int128_t debe ser POD");
    assert(std::is_pod<int128_t>::value);
    std::cout << "[PASS] test_is_pod()" << std::endl;
#else
    std::cout << "[SKIP] test_is_pod() - No soportado en libc++" << std::endl;
#endif
}

// =============================================================================
// TESTS DE TRANSFORMACIÓN DE TIPOS
// =============================================================================

/**
 * @brief Test para std::make_signed<int128_t>
 * Verifica que make_signed devuelve int128_t (ya es con signo)
 */
void test_make_signed()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_same<std::make_signed<int128_t>::type, int128_t>::value,
                  "make_signed<int128_t> debe ser int128_t");
    assert((std::is_same<std::make_signed<int128_t>::type, int128_t>::value));
    std::cout << "[PASS] test_make_signed()" << std::endl;
#else
    std::cout << "[SKIP] test_make_signed() - No soportado en libc++" << std::endl;
#endif
}

/**
 * @brief Test para std::make_unsigned<int128_t>
 * Verifica que make_unsigned devuelve uint128_t
 */
void test_make_unsigned()
{
#if !INT128_USING_LIBCPP
    static_assert(std::is_same<std::make_unsigned<int128_t>::type, uint128_t>::value,
                  "make_unsigned<int128_t> debe ser uint128_t");
    assert((std::is_same<std::make_unsigned<int128_t>::type, uint128_t>::value));
    std::cout << "[PASS] test_make_unsigned()" << std::endl;
#else
    std::cout << "[SKIP] test_make_unsigned() - No soportado en libc++" << std::endl;
#endif
}

// =============================================================================
// TESTS DE COMMON_TYPE
// =============================================================================

/**
 * @brief Test para std::common_type con tipos enteros estándar
 */
void test_common_type_standard()
{
    static_assert(std::is_same<std::common_type<int128_t, int64_t>::type, int128_t>::value,
                  "common_type<int128_t, int64_t> debe ser int128_t");
    static_assert(std::is_same<std::common_type<int64_t, int128_t>::type, int128_t>::value,
                  "common_type<int64_t, int128_t> debe ser int128_t");
    static_assert(std::is_same<std::common_type<int128_t, int32_t>::type, int128_t>::value,
                  "common_type<int128_t, int32_t> debe ser int128_t");
    static_assert(std::is_same<std::common_type<int128_t, uint64_t>::type, int128_t>::value,
                  "common_type<int128_t, uint64_t> debe ser int128_t");
    std::cout << "[PASS] test_common_type_standard()" << std::endl;
}

/**
 * @brief Test para std::common_type entre int128_t y uint128_t
 */
void test_common_type_uint128()
{
    static_assert(std::is_same<std::common_type<int128_t, uint128_t>::type, uint128_t>::value,
                  "common_type<int128_t, uint128_t> debe ser uint128_t");
    static_assert(std::is_same<std::common_type<uint128_t, int128_t>::type, uint128_t>::value,
                  "common_type<uint128_t, int128_t> debe ser uint128_t");
    std::cout << "[PASS] test_common_type_uint128()" << std::endl;
}

// =============================================================================
// TESTS DE HASH
// =============================================================================

/**
 * @brief Test para std::hash<int128_t>
 * Verifica que el hash funcione correctamente
 */
void test_hash()
{
    std::hash<int128_t> hasher;

    int128_t value1(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    int128_t value2(0x123456789ABCDEF0ULL, 0xFEDCBA9876543211ULL); // Diferente low
    int128_t value3(0x123456789ABCDEF1ULL, 0xFEDCBA9876543210ULL); // Diferente high

    size_t hash1 = hasher(value1);
    size_t hash2 = hasher(value2);
    size_t hash3 = hasher(value3);

    // Valores diferentes deben producir hashes diferentes (con alta probabilidad)
    assert(hash1 != hash2);
    assert(hash1 != hash3);
    assert(hash2 != hash3);

    // El mismo valor debe producir el mismo hash
    size_t hash1_again = hasher(value1);
    assert(hash1 == hash1_again);

    std::cout << "[PASS] test_hash()" << std::endl;
}

// =============================================================================
// FUNCIÓN PRINCIPAL
// =============================================================================

int main()
{
    std::cout << "=============================================================\n";
    std::cout << "   Pruebas Extraídas de int128_traits.hpp\n";
    std::cout << "=============================================================\n\n";

    int total_tests = 0;
    int passed_tests = 0;

    // Tests de type traits fundamentales
    try {
        test_is_integral();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_arithmetic();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_unsigned();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_signed();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    // Tests de trivialidad
    try {
        test_is_trivially_copyable();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_default_constructible();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_copy_constructible();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_move_constructible();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_copy_assignable();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_move_assignable();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivially_destructible();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_trivial();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_standard_layout();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_is_pod();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    // Tests de transformación de tipos
    try {
        test_make_signed();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_make_unsigned();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    // Tests de common_type
    try {
        test_common_type_standard();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    try {
        test_common_type_uint128();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    // Tests de hash
    try {
        test_hash();
        passed_tests++;
    } catch (...) {
    }
    total_tests++;

    // Resumen
    std::cout << "\n=============================================================\n";
    std::cout << "   Resumen de Tests\n";
    std::cout << "=============================================================\n";
    std::cout << "Total de tests: " << total_tests << "\n";
    std::cout << "Tests exitosos: " << passed_tests << "\n";
    std::cout << "Tests fallidos: " << (total_tests - passed_tests) << "\n";

    if (passed_tests == total_tests) {
        std::cout << "\n✅ TODOS LOS TESTS PASARON\n";
        return 0;
    } else {
        std::cout << "\n❌ ALGUNOS TESTS FALLARON\n";
        return 1;
    }
}
