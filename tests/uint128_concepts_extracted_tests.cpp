/*
 * Tests extraídos para uint128_concepts.hpp
 * Pruebas de conceptos C++20 para uint128_t
 */

#include "../include/uint128/uint128_concepts.hpp"
#include "../include/uint128/uint128_t.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// =============================================================================
// TEST: uint128_convertible
// =============================================================================
bool test_uint128_convertible()
{
    std::cout << "\n=== TEST: uint128_convertible ===\n";

    // Tipos que deben satisfacer el concepto
    static_assert(uint128_concepts::uint128_convertible<uint8_t>, "uint8_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<uint16_t>, "uint16_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<uint32_t>, "uint32_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<uint64_t>, "uint64_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<int8_t>, "int8_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<int16_t>, "int16_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<int32_t>, "int32_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<int64_t>, "int64_t debe ser convertible");
    static_assert(uint128_concepts::uint128_convertible<uint128_t>,
                  "uint128_t debe ser convertible");

    // Tipos que no deben satisfacer el concepto
    static_assert(!uint128_concepts::uint128_convertible<float>, "float no debe ser convertible");
    static_assert(!uint128_concepts::uint128_convertible<double>, "double no debe ser convertible");
    static_assert(!uint128_concepts::uint128_convertible<void*>, "void* no debe ser convertible");

    std::cout << "  ✅ PASS - Concept uint128_convertible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_compatible
// =============================================================================
bool test_uint128_compatible()
{
    std::cout << "\n=== TEST: uint128_compatible ===\n";

    // Tipos integrales
    static_assert(uint128_concepts::uint128_compatible<uint32_t>, "uint32_t debe ser compatible");
    static_assert(uint128_concepts::uint128_compatible<int64_t>, "int64_t debe ser compatible");
    static_assert(uint128_concepts::uint128_compatible<uint128_t>, "uint128_t debe ser compatible");

    // Tipos flotantes también son compatibles
    static_assert(uint128_concepts::uint128_compatible<float>, "float debe ser compatible");
    static_assert(uint128_concepts::uint128_compatible<double>, "double debe ser compatible");

    // Tipos que no son compatibles
    static_assert(!uint128_concepts::uint128_compatible<void*>, "void* no debe ser compatible");
    static_assert(!uint128_concepts::uint128_compatible<std::string>,
                  "string no debe ser compatible");

    std::cout << "  ✅ PASS - Concept uint128_compatible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_bitwise_compatible
// =============================================================================
bool test_uint128_bitwise_compatible()
{
    std::cout << "\n=== TEST: uint128_bitwise_compatible ===\n";

    // Tipos integrales son compatibles
    static_assert(uint128_concepts::uint128_bitwise_compatible<uint32_t>,
                  "uint32_t debe ser bitwise compatible");
    static_assert(uint128_concepts::uint128_bitwise_compatible<int64_t>,
                  "int64_t debe ser bitwise compatible");
    static_assert(uint128_concepts::uint128_bitwise_compatible<uint128_t>,
                  "uint128_t debe ser bitwise compatible");

    // Tipos flotantes NO son compatibles para bitwise
    static_assert(!uint128_concepts::uint128_bitwise_compatible<float>,
                  "float no debe ser bitwise compatible");
    static_assert(!uint128_concepts::uint128_bitwise_compatible<double>,
                  "double no debe ser bitwise compatible");

    std::cout << "  ✅ PASS - Concept uint128_bitwise_compatible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: valid_shift_type
// =============================================================================
bool test_valid_shift_type()
{
    std::cout << "\n=== TEST: valid_shift_type ===\n";

    // Tipos válidos para shift
    static_assert(uint128_concepts::valid_shift_type<int>, "int debe ser válido para shift");
    static_assert(uint128_concepts::valid_shift_type<unsigned int>,
                  "unsigned int debe ser válido para shift");
    static_assert(uint128_concepts::valid_shift_type<size_t>, "size_t debe ser válido para shift");

    // Tipos inválidos
    static_assert(!uint128_concepts::valid_shift_type<float>,
                  "float no debe ser válido para shift");

    std::cout << "  ✅ PASS - Concept valid_shift_type funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: bit_index_type
// =============================================================================
bool test_bit_index_type()
{
    std::cout << "\n=== TEST: bit_index_type ===\n";

    // Tipos válidos para índices de bit
    static_assert(uint128_concepts::bit_index_type<unsigned int>,
                  "unsigned int debe ser válido para índice");
    static_assert(uint128_concepts::bit_index_type<size_t>, "size_t debe ser válido para índice");
    static_assert(uint128_concepts::bit_index_type<uint8_t>, "uint8_t debe ser válido para índice");

    // Tipos signados no son válidos
    static_assert(!uint128_concepts::bit_index_type<int>, "int no debe ser válido para índice");
    static_assert(!uint128_concepts::bit_index_type<int64_t>,
                  "int64_t no debe ser válido para índice");

    std::cout << "  ✅ PASS - Concept bit_index_type funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_comparable
// =============================================================================
bool test_uint128_comparable()
{
    std::cout << "\n=== TEST: uint128_comparable ===\n";

    // Tipos comparables
    static_assert(uint128_concepts::uint128_comparable<uint32_t>, "uint32_t debe ser comparable");
    static_assert(uint128_concepts::uint128_comparable<int64_t>, "int64_t debe ser comparable");
    static_assert(uint128_concepts::uint128_comparable<uint128_t>, "uint128_t debe ser comparable");

    // Tipos no comparables
    static_assert(!uint128_concepts::uint128_comparable<float>, "float no debe ser comparable");
    static_assert(!uint128_concepts::uint128_comparable<double>, "double no debe ser comparable");

    std::cout << "  ✅ PASS - Concept uint128_comparable funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: promotable_to_uint128
// =============================================================================
bool test_promotable_to_uint128()
{
    std::cout << "\n=== TEST: promotable_to_uint128 ===\n";

    // Tipos que pueden ser promovidos
    static_assert(uint128_concepts::promotable_to_uint128<uint8_t>, "uint8_t puede ser promovido");
    static_assert(uint128_concepts::promotable_to_uint128<uint16_t>,
                  "uint16_t puede ser promovido");
    static_assert(uint128_concepts::promotable_to_uint128<uint32_t>,
                  "uint32_t puede ser promovido");
    static_assert(uint128_concepts::promotable_to_uint128<uint64_t>,
                  "uint64_t puede ser promovido");

    // uint128_t mismo no puede ser promovido (ya es el tipo destino)
    static_assert(!uint128_concepts::promotable_to_uint128<uint128_t>,
                  "uint128_t no puede ser promovido");

    // Tipos no integrales no pueden ser promovidos
    static_assert(!uint128_concepts::promotable_to_uint128<float>, "float no puede ser promovido");

    std::cout << "  ✅ PASS - Concept promotable_to_uint128 funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_function
// =============================================================================
bool test_uint128_function()
{
    std::cout << "\n=== TEST: uint128_function ===\n";

    // Función que satisface el concepto
    auto func1 = [](uint128_t u) { return u + 1; };
    static_assert(uint128_concepts::uint128_function<decltype(func1)>,
                  "Lambda que acepta uint128_t debe satisfacer el concepto");

    // Función con argumentos adicionales
    auto func2 = [](uint128_t u, int x) { return u + x; };
    static_assert(uint128_concepts::uint128_function<decltype(func2), int>,
                  "Lambda con args adicionales debe satisfacer el concepto");

    std::cout << "  ✅ PASS - Concept uint128_function funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_container
// =============================================================================
bool test_uint128_container()
{
    std::cout << "\n=== TEST: uint128_container ===\n";

    // std::vector<uint128_t> debe satisfacer el concepto
    static_assert(uint128_concepts::uint128_container<std::vector<uint128_t>>,
                  "vector<uint128_t> debe satisfacer el concepto");

    // std::vector<int> no debe satisfacerlo
    static_assert(!uint128_concepts::uint128_container<std::vector<int>>,
                  "vector<int> no debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    std::vector<uint128_t> vec = {uint128_t(1), uint128_t(2), uint128_t(3)};
    assert(vec.size() == 3 && "Vector debe tener 3 elementos");
    assert(vec[0] == uint128_t(1) && "Primer elemento debe ser 1");

    std::cout << "  ✅ PASS - Concept uint128_container funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_predicate
// =============================================================================
bool test_uint128_predicate()
{
    std::cout << "\n=== TEST: uint128_predicate ===\n";

    // Predicado que satisface el concepto
    auto is_even = [](uint128_t u) { return (u & 1) == 0; };
    static_assert(uint128_concepts::uint128_predicate<decltype(is_even)>,
                  "Predicado debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(is_even(uint128_t(2)) && "2 debe ser par");
    assert(!is_even(uint128_t(3)) && "3 no debe ser par");

    std::cout << "  ✅ PASS - Concept uint128_predicate funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_binary_operation
// =============================================================================
bool test_uint128_binary_operation()
{
    std::cout << "\n=== TEST: uint128_binary_operation ===\n";

    // Operación binaria que satisface el concepto
    auto add = [](uint128_t a, uint128_t b) { return a + b; };
    static_assert(uint128_concepts::uint128_binary_operation<decltype(add)>,
                  "Operación de suma debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    uint128_t result = add(uint128_t(10), uint128_t(20));
    assert(result == uint128_t(30) && "10 + 20 debe ser 30");
    (void)result; // Suprimir warning de variable no usada

    std::cout << "  ✅ PASS - Concept uint128_binary_operation funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: uint128_reduce_operation
// =============================================================================
bool test_uint128_reduce_operation()
{
    std::cout << "\n=== TEST: uint128_reduce_operation ===\n";

    // Operación de reducción que satisface el concepto
    auto sum_op = [](uint64_t acc, uint128_t u) -> uint64_t {
        return acc + static_cast<uint64_t>(u.low());
    };
    static_assert(uint128_concepts::uint128_reduce_operation<decltype(sum_op), uint64_t>,
                  "Operación de reducción debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(sum_op(100, uint128_t(50)) == 150 && "Reducción debe dar 150");

    std::cout << "  ✅ PASS - Concept uint128_reduce_operation funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: Verificaciones de concept checks estándar
// =============================================================================
bool test_standard_concept_checks()
{
    std::cout << "\n=== TEST: Standard Concept Checks ===\n";

    // Verificar que uint128_concept_checks compila
    // (las static_assert en ese namespace ya se ejecutan en compile-time)

    std::cout << "  std::regular<uint128_t> = " << std::regular<uint128_t> << "\n";
    std::cout << "  std::totally_ordered<uint128_t> = " << std::totally_ordered<uint128_t> << "\n";
    std::cout << "  std::equality_comparable<uint128_t> = "
              << std::equality_comparable<uint128_t> << "\n";
    std::cout << "  std::default_initializable<uint128_t> = "
              << std::default_initializable<uint128_t> << "\n";
    std::cout << "  std::copy_constructible<uint128_t> = "
              << std::copy_constructible<uint128_t> << "\n";
    std::cout << "  std::move_constructible<uint128_t> = "
              << std::move_constructible<uint128_t> << "\n";

    std::cout << "  ✅ PASS - Standard concept checks correctos\n";
    return true;
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  UINT128 CONCEPTS - EXTRACTED TESTS\n";
    std::cout << "========================================\n";

    bool all_pass = true;

    try {
        all_pass &= test_uint128_convertible();
        all_pass &= test_uint128_compatible();
        all_pass &= test_uint128_bitwise_compatible();
        all_pass &= test_valid_shift_type();
        all_pass &= test_bit_index_type();
        all_pass &= test_uint128_comparable();
        all_pass &= test_promotable_to_uint128();
        all_pass &= test_uint128_function();
        all_pass &= test_uint128_container();
        all_pass &= test_uint128_predicate();
        all_pass &= test_uint128_binary_operation();
        all_pass &= test_uint128_reduce_operation();
        all_pass &= test_standard_concept_checks();
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Exception caught: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    if (all_pass) {
        std::cout << "  ✅ TODOS LOS TESTS PASARON\n";
    } else {
        std::cout << "  ❌ ALGUNOS TESTS FALLARON\n";
    }
    std::cout << "========================================\n";

    return all_pass ? 0 : 1;
}
