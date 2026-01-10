/**
 * @file int128_concepts_extracted_tests.cpp
 * @brief Tests completos de int128_base_concepts.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_concepts.hpp tests (16 tests)
 * - test_concepts_template.cpp (50+ tests)
 *
 * Total: 55+ tests cubriendo:
 * - Type traits de detección
 * - Concepts básicos
 * - Concepts de conversión y compatibilidad
 * - Concepts de signedness
 * - Concepts de operaciones
 * - Concepts de metaprogramación
 * - Concepts de algoritmos
 * - Uso práctico con templates
 */

#include "int128_base_concepts.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

using namespace nstd;

static int tests_passed = 0;
static int tests_failed = 0;

void check(bool condition, const char *test_name)
{
    if (condition)
    {
        std::cout << "[OK]   " << test_name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "[FAIL] " << test_name << std::endl;
        tests_failed++;
    }
}

// =============================================================================
// SECTION: Type traits de detección
// =============================================================================

void test_type_traits_detection()
{
    check(is_128bit_type_v<uint128_t>, "is_128bit_type_v<uint128_t>");
    check(is_128bit_type_v<int128_t>, "is_128bit_type_v<int128_t>");
    check(!is_128bit_type_v<int>, "!is_128bit_type_v<int>");
    check(!is_128bit_type_v<uint64_t>, "!is_128bit_type_v<uint64_t>");

    check(is_uint128_v<uint128_t>, "is_uint128_v<uint128_t>");
    check(!is_uint128_v<int128_t>, "!is_uint128_v<int128_t>");

    check(is_int128_v<int128_t>, "is_int128_v<int128_t>");
    check(!is_int128_v<uint128_t>, "!is_int128_v<uint128_t>");
}

// =============================================================================
// SECTION: Concepts básicos
// =============================================================================

void test_basic_concepts()
{
    check(int128_type<uint128_t>, "int128_type<uint128_t>");
    check(int128_type<int128_t>, "int128_type<int128_t>");
    check(!int128_type<int>, "!int128_type<int>");

    check(uint128_type<uint128_t>, "uint128_type<uint128_t>");
    check(!uint128_type<int128_t>, "!uint128_type<int128_t>");

    check(signed_int128_type<int128_t>, "signed_int128_type<int128_t>");
    check(!signed_int128_type<uint128_t>, "!signed_int128_type<uint128_t>");
}

// =============================================================================
// SECTION: Concepts de conversión (int128_convertible)
// =============================================================================

void test_int128_convertible()
{
    // Tipos que deben satisfacer el concepto
    check(int128_convertible<int8_t>, "int128_convertible<int8_t>");
    check(int128_convertible<int16_t>, "int128_convertible<int16_t>");
    check(int128_convertible<int32_t>, "int128_convertible<int32_t>");
    check(int128_convertible<int64_t>, "int128_convertible<int64_t>");
    check(int128_convertible<uint8_t>, "int128_convertible<uint8_t>");
    check(int128_convertible<uint16_t>, "int128_convertible<uint16_t>");
    check(int128_convertible<uint32_t>, "int128_convertible<uint32_t>");
    check(int128_convertible<uint64_t>, "int128_convertible<uint64_t>");
    check(int128_convertible<uint128_t>, "int128_convertible<uint128_t>");
    check(int128_convertible<int128_t>, "int128_convertible<int128_t>");

    // Tipos que no deben satisfacer
    check(!int128_convertible<float>, "!int128_convertible<float>");
    check(!int128_convertible<double>, "!int128_convertible<double>");
    check(!int128_convertible<long double>, "!int128_convertible<long double>");
}

// =============================================================================
// SECTION: Concepts de compatibilidad (int128_compatible)
// =============================================================================

void test_int128_compatible()
{
    check(int128_compatible<int>, "int128_compatible<int>");
    check(int128_compatible<float>, "int128_compatible<float>");
    check(int128_compatible<double>, "int128_compatible<double>");
    check(int128_compatible<uint128_t>, "int128_compatible<uint128_t>");
    check(int128_compatible<int128_t>, "int128_compatible<int128_t>");
}

// =============================================================================
// SECTION: Concepts bitwise (int128_bitwise_compatible)
// =============================================================================

void test_int128_bitwise_compatible()
{
    check(int128_bitwise_compatible<int>, "int128_bitwise_compatible<int>");
    check(int128_bitwise_compatible<int32_t>, "int128_bitwise_compatible<int32_t>");
    check(int128_bitwise_compatible<int64_t>, "int128_bitwise_compatible<int64_t>");
    check(int128_bitwise_compatible<uint128_t>, "int128_bitwise_compatible<uint128_t>");
    check(int128_bitwise_compatible<int128_t>, "int128_bitwise_compatible<int128_t>");

    check(!int128_bitwise_compatible<float>, "!int128_bitwise_compatible<float>");
    check(!int128_bitwise_compatible<double>, "!int128_bitwise_compatible<double>");
}

// =============================================================================
// SECTION: Concepts de signedness
// =============================================================================

void test_signedness_concepts()
{
    // Signed compatible
    check(int128_signed_compatible<int>, "int128_signed_compatible<int>");
    check(int128_signed_compatible<int8_t>, "int128_signed_compatible<int8_t>");
    check(int128_signed_compatible<int64_t>, "int128_signed_compatible<int64_t>");
    check(int128_signed_compatible<int128_t>, "int128_signed_compatible<int128_t>");
    check(!int128_signed_compatible<unsigned>, "!int128_signed_compatible<unsigned>");
    check(!int128_signed_compatible<uint64_t>, "!int128_signed_compatible<uint64_t>");

    // Unsigned compatible
    check(int128_unsigned_compatible<unsigned>, "int128_unsigned_compatible<unsigned>");
    check(int128_unsigned_compatible<uint8_t>, "int128_unsigned_compatible<uint8_t>");
    check(int128_unsigned_compatible<uint64_t>, "int128_unsigned_compatible<uint64_t>");
    check(int128_unsigned_compatible<uint128_t>, "int128_unsigned_compatible<uint128_t>");
    check(!int128_unsigned_compatible<int>, "!int128_unsigned_compatible<int>");
    check(!int128_unsigned_compatible<int64_t>, "!int128_unsigned_compatible<int64_t>");
}

// =============================================================================
// SECTION: Concepts de operaciones
// =============================================================================

void test_operation_concepts()
{
    // valid_shift_type
    check(valid_shift_type<int>, "valid_shift_type<int>");
    check(valid_shift_type<unsigned int>, "valid_shift_type<unsigned int>");
    check(valid_shift_type<size_t>, "valid_shift_type<size_t>");
    check(valid_shift_type<uint64_t>, "valid_shift_type<uint64_t>");

    // bit_index_type
    check(bit_index_type<unsigned int>, "bit_index_type<unsigned int>");
    check(bit_index_type<size_t>, "bit_index_type<size_t>");
    check(bit_index_type<uint8_t>, "bit_index_type<uint8_t>");
    check(!bit_index_type<int>, "!bit_index_type<int> (signed)");
    check(!bit_index_type<int64_t>, "!bit_index_type<int64_t>");

    // int128_comparable
    check(int128_comparable<int>, "int128_comparable<int>");
    check(int128_comparable<int32_t>, "int128_comparable<int32_t>");
    check(int128_comparable<int64_t>, "int128_comparable<int64_t>");
    check(int128_comparable<uint128_t>, "int128_comparable<uint128_t>");
    check(int128_comparable<int128_t>, "int128_comparable<int128_t>");
}

// =============================================================================
// SECTION: Concepts de metaprogramación
// =============================================================================

void test_promotable_to_int128()
{
    check(promotable_to_int128<int8_t>, "promotable_to_int128<int8_t>");
    check(promotable_to_int128<int16_t>, "promotable_to_int128<int16_t>");
    check(promotable_to_int128<int32_t>, "promotable_to_int128<int32_t>");
    check(promotable_to_int128<int64_t>, "promotable_to_int128<int64_t>");
    check(promotable_to_int128<uint64_t>, "promotable_to_int128<uint64_t>");

    // int128_t/uint128_t no pueden ser promovidos (ya son el tipo destino)
    check(!promotable_to_int128<uint128_t>, "!promotable_to_int128<uint128_t>");
    check(!promotable_to_int128<int128_t>, "!promotable_to_int128<int128_t>");

    check(!promotable_to_int128<float>, "!promotable_to_int128<float>");
}

// =============================================================================
// SECTION: Concepts de algoritmos (predicate, binary_operation)
// =============================================================================

void test_algorithm_concepts()
{
    // Predicado lambda
    auto is_positive = [](auto x)
    { return x > decltype(x)(0); };
    constexpr bool pred_uint128 = int128_predicate<decltype(is_positive), uint128_t>;
    constexpr bool pred_int128 = int128_predicate<decltype(is_positive), int128_t>;
    check(pred_uint128, "int128_predicate<lambda, uint128_t>");
    check(pred_int128, "int128_predicate<lambda, int128_t>");

    // Operación binaria lambda
    auto add_op = [](auto a, auto b)
    { return a + b; };
    constexpr bool binop_uint128 = int128_binary_operation<decltype(add_op), uint128_t>;
    constexpr bool binop_int128 = int128_binary_operation<decltype(add_op), int128_t>;
    check(binop_uint128, "int128_binary_operation<lambda, uint128_t>");
    check(binop_int128, "int128_binary_operation<lambda, int128_t>");
}

// =============================================================================
// SECTION: Namespaces de compatibilidad (uint128_concepts, int128_concepts)
// =============================================================================

void test_compat_namespaces()
{
    check(uint128_concepts::uint128_convertible<int>, "uint128_concepts::uint128_convertible<int>");
    check(uint128_concepts::uint128_compatible<float>,
          "uint128_concepts::uint128_compatible<float>");

    check(int128_concepts::int128_convertible<int>, "int128_concepts::int128_convertible<int>");
    check(int128_concepts::int128_signed_compatible<int>,
          "int128_concepts::int128_signed_compatible<int>");
}

// =============================================================================
// SECTION: Uso práctico con templates
// =============================================================================

void test_practical_templates()
{
    // Función template que usa int128_type concept
    auto test_function = []<int128_type T>(T val) -> T
    { return val + T(1); };

    uint128_t u = test_function(uint128_t(100));
    int128_t i = test_function(int128_t(-50));
    check(u == uint128_t(101), "Template int128_type (uint128): 100+1=101");
    check(i == int128_t(-49), "Template int128_type (int128): -50+1=-49");

    // Función con concept de comparabilidad
    auto compare_values = []<int128_comparable T>(T a, T b) -> bool
    { return a < b; };
    check(compare_values(uint128_t(10), uint128_t(20)), "Template int128_comparable (uint128)");
    check(compare_values(5, 10), "Template int128_comparable (int)");
}

// =============================================================================
// SECTION: Standard concept checks
// =============================================================================

void test_standard_concepts()
{
    check(std::regular<int128_t>, "std::regular<int128_t>");
    check(std::regular<uint128_t>, "std::regular<uint128_t>");
    check(std::totally_ordered<int128_t>, "std::totally_ordered<int128_t>");
    check(std::totally_ordered<uint128_t>, "std::totally_ordered<uint128_t>");
    check(std::equality_comparable<int128_t>, "std::equality_comparable<int128_t>");
    check(std::default_initializable<int128_t>, "std::default_initializable<int128_t>");
    check(std::copy_constructible<int128_t>, "std::copy_constructible<int128_t>");
    check(std::move_constructible<int128_t>, "std::move_constructible<int128_t>");
}

// =============================================================================
// SECTION: Verificaciones adicionales de runtime
// =============================================================================

void test_runtime_verifications()
{
    check(sizeof(int128_t) == 16, "sizeof(int128_t)==16");
    check(sizeof(uint128_t) == 16, "sizeof(uint128_t)==16");
    check(int128_t(-1) < int128_t(0), "int128_t(-1) < int128_t(0)");
    check(int128_t(1) > int128_t(0), "int128_t(1) > int128_t(0)");

    // Container concept test
    std::vector<int128_t> vec = {int128_t(1), int128_t(-2), int128_t(3)};
    check(vec.size() == 3, "vector<int128_t> size==3");
    check(vec[0] == int128_t(1), "vector<int128_t>[0]==1");
    check(vec[1] == int128_t(-2), "vector<int128_t>[1]==-2");

    // Predicado test
    auto is_positive = [](int128_t i)
    { return !i.is_negative() && !(i == int128_t(0)); };
    check(is_positive(int128_t(5)), "is_positive(5)==true");
    check(!is_positive(int128_t(-5)), "is_positive(-5)==false");
    check(!is_positive(int128_t(0)), "is_positive(0)==false");

    // Binary operation test
    auto add = [](int128_t a, int128_t b)
    { return a + b; };
    check(add(int128_t(10), int128_t(-20)) == int128_t(-10), "add(10,-20)==-10");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_concepts.hpp tests ===\n\n";

    std::cout << "--- Type traits de detección ---\n";
    test_type_traits_detection();

    std::cout << "\n--- Concepts básicos ---\n";
    test_basic_concepts();

    std::cout << "\n--- int128_convertible ---\n";
    test_int128_convertible();

    std::cout << "\n--- int128_compatible ---\n";
    test_int128_compatible();

    std::cout << "\n--- int128_bitwise_compatible ---\n";
    test_int128_bitwise_compatible();

    std::cout << "\n--- Signedness concepts ---\n";
    test_signedness_concepts();

    std::cout << "\n--- Operation concepts ---\n";
    test_operation_concepts();

    std::cout << "\n--- promotable_to_int128 ---\n";
    test_promotable_to_int128();

    std::cout << "\n--- Algorithm concepts ---\n";
    test_algorithm_concepts();

    std::cout << "\n--- Compat namespaces ---\n";
    test_compat_namespaces();

    std::cout << "\n--- Practical templates ---\n";
    test_practical_templates();

    std::cout << "\n--- Standard concepts ---\n";
    test_standard_concepts();

    std::cout << "\n--- Runtime verifications ---\n";
    test_runtime_verifications();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
