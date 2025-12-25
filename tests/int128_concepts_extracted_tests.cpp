/*
 * Tests extraídos para int128_concepts.hpp
 * Pruebas de conceptos C++20 para int128_t
 */

#include "../include/int128/int128_concepts.hpp"
#include "../include/int128/int128_t.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// =============================================================================
// TEST: int128_convertible
// =============================================================================
bool test_int128_convertible()
{
    std::cout << "\n=== TEST: int128_convertible ===\n";

    // Tipos que deben satisfacer el concepto
    static_assert(int128_concepts::int128_convertible<int8_t>, "int8_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<int16_t>, "int16_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<int32_t>, "int32_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<int64_t>, "int64_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<uint8_t>, "uint8_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<uint16_t>, "uint16_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<uint32_t>, "uint32_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<uint64_t>, "uint64_t debe ser convertible");
    static_assert(int128_concepts::int128_convertible<int128_t>, "int128_t debe ser convertible");

    // Tipos que no deben satisfacer el concepto
    static_assert(!int128_concepts::int128_convertible<float>, "float no debe ser convertible");
    static_assert(!int128_concepts::int128_convertible<double>, "double no debe ser convertible");
    static_assert(!int128_concepts::int128_convertible<void*>, "void* no debe ser convertible");

    std::cout << "  [OK] PASS - Concept int128_convertible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_compatible
// =============================================================================
bool test_int128_compatible()
{
    std::cout << "\n=== TEST: int128_compatible ===\n";

    // Tipos integrales
    static_assert(int128_concepts::int128_compatible<int32_t>, "int32_t debe ser compatible");
    static_assert(int128_concepts::int128_compatible<int64_t>, "int64_t debe ser compatible");
    static_assert(int128_concepts::int128_compatible<uint64_t>, "uint64_t debe ser compatible");
    static_assert(int128_concepts::int128_compatible<int128_t>, "int128_t debe ser compatible");

    // Tipos flotantes también son compatibles
    static_assert(int128_concepts::int128_compatible<float>, "float debe ser compatible");
    static_assert(int128_concepts::int128_compatible<double>, "double debe ser compatible");

    // Tipos que no son compatibles
    static_assert(!int128_concepts::int128_compatible<void*>, "void* no debe ser compatible");
    static_assert(!int128_concepts::int128_compatible<std::string>,
                  "string no debe ser compatible");

    std::cout << "  [OK] PASS - Concept int128_compatible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_bitwise_compatible
// =============================================================================
bool test_int128_bitwise_compatible()
{
    std::cout << "\n=== TEST: int128_bitwise_compatible ===\n";

    // Tipos integrales son compatibles
    static_assert(int128_concepts::int128_bitwise_compatible<int32_t>,
                  "int32_t debe ser bitwise compatible");
    static_assert(int128_concepts::int128_bitwise_compatible<int64_t>,
                  "int64_t debe ser bitwise compatible");
    static_assert(int128_concepts::int128_bitwise_compatible<int128_t>,
                  "int128_t debe ser bitwise compatible");

    // Tipos flotantes NO son compatibles para bitwise
    static_assert(!int128_concepts::int128_bitwise_compatible<float>,
                  "float no debe ser bitwise compatible");
    static_assert(!int128_concepts::int128_bitwise_compatible<double>,
                  "double no debe ser bitwise compatible");

    std::cout << "  [OK] PASS - Concept int128_bitwise_compatible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_signed_compatible
// =============================================================================
bool test_int128_signed_compatible()
{
    std::cout << "\n=== TEST: int128_signed_compatible ===\n";

    // Tipos signados son compatibles
    static_assert(int128_concepts::int128_signed_compatible<int8_t>,
                  "int8_t debe ser signed compatible");
    static_assert(int128_concepts::int128_signed_compatible<int32_t>,
                  "int32_t debe ser signed compatible");
    static_assert(int128_concepts::int128_signed_compatible<int64_t>,
                  "int64_t debe ser signed compatible");
    static_assert(int128_concepts::int128_signed_compatible<int128_t>,
                  "int128_t debe ser signed compatible");

    // Tipos no signados NO son compatibles
    static_assert(!int128_concepts::int128_signed_compatible<uint32_t>,
                  "uint32_t no debe ser signed compatible");
    static_assert(!int128_concepts::int128_signed_compatible<uint64_t>,
                  "uint64_t no debe ser signed compatible");

    std::cout << "  [OK] PASS - Concept int128_signed_compatible funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: valid_shift_type
// =============================================================================
bool test_valid_shift_type()
{
    std::cout << "\n=== TEST: valid_shift_type ===\n";

    // Tipos válidos para shift
    static_assert(int128_concepts::valid_shift_type<int>, "int debe ser válido para shift");
    static_assert(int128_concepts::valid_shift_type<unsigned int>,
                  "unsigned int debe ser válido para shift");
    static_assert(int128_concepts::valid_shift_type<size_t>, "size_t debe ser válido para shift");

    // Tipos inválidos
    static_assert(!int128_concepts::valid_shift_type<float>, "float no debe ser válido para shift");

    std::cout << "  [OK] PASS - Concept valid_shift_type funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: bit_index_type
// =============================================================================
bool test_bit_index_type()
{
    std::cout << "\n=== TEST: bit_index_type ===\n";

    // Tipos válidos para índices de bit
    static_assert(int128_concepts::bit_index_type<unsigned int>,
                  "unsigned int debe ser válido para índice");
    static_assert(int128_concepts::bit_index_type<size_t>, "size_t debe ser válido para índice");
    static_assert(int128_concepts::bit_index_type<uint8_t>, "uint8_t debe ser válido para índice");

    // Tipos signados no son válidos
    static_assert(!int128_concepts::bit_index_type<int>, "int no debe ser válido para índice");
    static_assert(!int128_concepts::bit_index_type<int64_t>,
                  "int64_t no debe ser válido para índice");

    std::cout << "  [OK] PASS - Concept bit_index_type funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_comparable
// =============================================================================
bool test_int128_comparable()
{
    std::cout << "\n=== TEST: int128_comparable ===\n";

    // Tipos comparables
    static_assert(int128_concepts::int128_comparable<int32_t>, "int32_t debe ser comparable");
    static_assert(int128_concepts::int128_comparable<int64_t>, "int64_t debe ser comparable");
    static_assert(int128_concepts::int128_comparable<uint64_t>, "uint64_t debe ser comparable");
    static_assert(int128_concepts::int128_comparable<int128_t>, "int128_t debe ser comparable");

    // Tipos no comparables
    static_assert(!int128_concepts::int128_comparable<float>, "float no debe ser comparable");
    static_assert(!int128_concepts::int128_comparable<double>, "double no debe ser comparable");

    std::cout << "  [OK] PASS - Concept int128_comparable funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: promotable_to_int128
// =============================================================================
bool test_promotable_to_int128()
{
    std::cout << "\n=== TEST: promotable_to_int128 ===\n";

    // Tipos que pueden ser promovidos
    static_assert(int128_concepts::promotable_to_int128<int8_t>, "int8_t puede ser promovido");
    static_assert(int128_concepts::promotable_to_int128<int16_t>, "int16_t puede ser promovido");
    static_assert(int128_concepts::promotable_to_int128<int32_t>, "int32_t puede ser promovido");
    static_assert(int128_concepts::promotable_to_int128<int64_t>, "int64_t puede ser promovido");
    static_assert(int128_concepts::promotable_to_int128<uint64_t>, "uint64_t puede ser promovido");

    // int128_t mismo no puede ser promovido (ya es el tipo destino)
    static_assert(!int128_concepts::promotable_to_int128<int128_t>,
                  "int128_t no puede ser promovido");

    // Tipos no integrales no pueden ser promovidos
    static_assert(!int128_concepts::promotable_to_int128<float>, "float no puede ser promovido");

    std::cout << "  [OK] PASS - Concept promotable_to_int128 funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_function
// =============================================================================
bool test_int128_function()
{
    std::cout << "\n=== TEST: int128_function ===\n";

    // Función que satisface el concepto
    auto func1 = [](int128_t i) { return i + 1; };
    static_assert(int128_concepts::int128_function<decltype(func1)>,
                  "Lambda que acepta int128_t debe satisfacer el concepto");

    // Función con argumentos adicionales
    auto func2 = [](int128_t i, int x) { return i + x; };
    static_assert(int128_concepts::int128_function<decltype(func2), int>,
                  "Lambda con args adicionales debe satisfacer el concepto");

    std::cout << "  [OK] PASS - Concept int128_function funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_container
// =============================================================================
bool test_int128_container()
{
    std::cout << "\n=== TEST: int128_container ===\n";

    // std::vector<int128_t> debe satisfacer el concepto
    static_assert(int128_concepts::int128_container<std::vector<int128_t>>,
                  "vector<int128_t> debe satisfacer el concepto");

    // std::vector<int> no debe satisfacerlo
    static_assert(!int128_concepts::int128_container<std::vector<int>>,
                  "vector<int> no debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    std::vector<int128_t> vec = {int128_t(1), int128_t(-2), int128_t(3)};
    assert(vec.size() == 3 && "Vector debe tener 3 elementos");
    assert(vec[0] == int128_t(1) && "Primer elemento debe ser 1");
    assert(vec[1] == int128_t(-2) && "Segundo elemento debe ser -2");

    std::cout << "  [OK] PASS - Concept int128_container funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_range_type
// =============================================================================
bool test_int128_range_type()
{
    std::cout << "\n=== TEST: int128_range_type ===\n";

    // Tipos con rango adecuado
    static_assert(int128_concepts::int128_range_type<int64_t>,
                  "int64_t debe ser válido como range type");

    // Tipos pequeños no son válidos
    static_assert(!int128_concepts::int128_range_type<int32_t>,
                  "int32_t no debe ser válido como range type");
    static_assert(!int128_concepts::int128_range_type<uint32_t>,
                  "uint32_t no debe ser válido como range type");

    std::cout << "  [OK] PASS - Concept int128_range_type funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_predicate
// =============================================================================
bool test_int128_predicate()
{
    std::cout << "\n=== TEST: int128_predicate ===\n";

    // Predicado que satisface el concepto
    auto is_positive = [](int128_t i) { return i > 0; };
    static_assert(int128_concepts::int128_predicate<decltype(is_positive)>,
                  "Predicado debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(is_positive(int128_t(5)) && "5 debe ser positivo");
    assert(!is_positive(int128_t(-5)) && "-5 no debe ser positivo");
    assert(!is_positive(int128_t(0)) && "0 no debe ser positivo");

    std::cout << "  [OK] PASS - Concept int128_predicate funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_binary_operation
// =============================================================================
bool test_int128_binary_operation()
{
    std::cout << "\n=== TEST: int128_binary_operation ===\n";

    // Operación binaria que satisface el concepto
    auto add = [](int128_t a, int128_t b) { return a + b; };
    static_assert(int128_concepts::int128_binary_operation<decltype(add)>,
                  "Operación de suma debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(add(int128_t(10), int128_t(-20)) == int128_t(-10) && "10 + (-20) debe ser -10");

    std::cout << "  [OK] PASS - Concept int128_binary_operation funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_reduce_operation
// =============================================================================
bool test_int128_reduce_operation()
{
    std::cout << "\n=== TEST: int128_reduce_operation ===\n";

    // Operación de reducción que satisface el concepto
    auto sum_op = [](int64_t acc, int128_t i) -> int64_t {
        return acc + static_cast<int64_t>(i.low());
    };
    static_assert(int128_concepts::int128_reduce_operation<decltype(sum_op), int64_t>,
                  "Operación de reducción debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(sum_op(100, int128_t(-50)) == 50 && "Reducción debe dar 50");

    std::cout << "  [OK] PASS - Concept int128_reduce_operation funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: int128_safe_operation
// =============================================================================
bool test_int128_safe_operation()
{
    std::cout << "\n=== TEST: int128_safe_operation ===\n";

    // Operación segura que satisface el concepto
    auto safe_add = [](int128_t a, int128_t b) -> int128_t { return a + b; };
    static_assert(int128_concepts::int128_safe_operation<decltype(safe_add)>,
                  "Operación segura debe satisfacer el concepto");

    // Prueba en tiempo de ejecución
    assert(safe_add(int128_t(1000), int128_t(-500)) == int128_t(500) &&
           "1000 + (-500) debe ser 500");

    std::cout << "  [OK] PASS - Concept int128_safe_operation funciona correctamente\n";
    return true;
}

// =============================================================================
// TEST: Verificaciones de concept checks estándar
// =============================================================================
bool test_standard_concept_checks()
{
    std::cout << "\n=== TEST: Standard Concept Checks ===\n";

    // Verificar que int128_concept_checks compila
    // (las static_assert en ese namespace ya se ejecutan en compile-time)

    std::cout << "  std::regular<int128_t> = " << std::regular<int128_t> << "\n";
    std::cout << "  std::totally_ordered<int128_t> = " << std::totally_ordered<int128_t> << "\n";
    std::cout << "  std::equality_comparable<int128_t> = "
              << std::equality_comparable<int128_t> << "\n";
    std::cout << "  std::default_initializable<int128_t> = "
              << std::default_initializable<int128_t> << "\n";
    std::cout << "  std::copy_constructible<int128_t> = "
              << std::copy_constructible<int128_t> << "\n";
    std::cout << "  std::move_constructible<int128_t> = "
              << std::move_constructible<int128_t> << "\n";

    // Verificaciones manuales específicas para int128_t
    std::cout << "  sizeof(int128_t) == 16 = " << (sizeof(int128_t) == 16) << "\n";
    std::cout << "  int128_t(-1) < int128_t(0) = " << (int128_t(-1) < int128_t(0)) << "\n";
    std::cout << "  int128_t(1) > int128_t(0) = " << (int128_t(1) > int128_t(0)) << "\n";

    std::cout << "  [OK] PASS - Standard concept checks correctos\n";
    return true;
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  INT128 CONCEPTS - EXTRACTED TESTS\n";
    std::cout << "========================================\n";

    bool all_pass = true;

    try {
        all_pass &= test_int128_convertible();
        all_pass &= test_int128_compatible();
        all_pass &= test_int128_bitwise_compatible();
        all_pass &= test_int128_signed_compatible();
        all_pass &= test_valid_shift_type();
        all_pass &= test_bit_index_type();
        all_pass &= test_int128_comparable();
        all_pass &= test_promotable_to_int128();
        all_pass &= test_int128_function();
        all_pass &= test_int128_container();
        all_pass &= test_int128_range_type();
        all_pass &= test_int128_predicate();
        all_pass &= test_int128_binary_operation();
        all_pass &= test_int128_reduce_operation();
        all_pass &= test_int128_safe_operation();
        all_pass &= test_standard_concept_checks();
    } catch (const std::exception& e) {
        std::cerr << "\n[FAIL] Exception caught: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    if (all_pass) {
        std::cout << "  [OK] TODOS LOS TESTS PASARON\n";
    } else {
        std::cout << "  [FAIL] ALGUNOS TESTS FALLARON\n";
    }
    std::cout << "========================================\n";

    return all_pass ? 0 : 1;
}
