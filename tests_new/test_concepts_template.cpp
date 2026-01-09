/**
 * @file test_concepts_template.cpp
 * @brief Test de concepts C++20 para int128_base_t<S> unificado
 * @date 2026-01-07
 */

#include "../include_new/int128_base_concepts.hpp"
#include <array>
#include <iostream>
#include <vector>

using namespace nstd;

// Helper para mostrar resultado
#define TEST(name, condition)                                                                      \
    do {                                                                                           \
        bool ok = (condition);                                                                     \
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] " << name << "\n";                          \
        ok ? ++passed : ++failed;                                                                  \
    } while (0)

int main()
{
    std::cout << "=== Test concepts para int128_base_t<S> ===\n\n";

    int passed = 0;
    int failed = 0;

    // =========================================================================
    // SECCIÓN 1: Type traits de detección
    // =========================================================================
    std::cout << "--- Type traits de detección ---\n";

    TEST("is_128bit_type_v<uint128_t>", is_128bit_type_v<uint128_t>);
    TEST("is_128bit_type_v<int128_t>", is_128bit_type_v<int128_t>);
    TEST("!is_128bit_type_v<int>", !is_128bit_type_v<int>);
    TEST("!is_128bit_type_v<uint64_t>", !is_128bit_type_v<uint64_t>);

    TEST("is_uint128_v<uint128_t>", is_uint128_v<uint128_t>);
    TEST("!is_uint128_v<int128_t>", !is_uint128_v<int128_t>);

    TEST("is_int128_v<int128_t>", is_int128_v<int128_t>);
    TEST("!is_int128_v<uint128_t>", !is_int128_v<uint128_t>);

    // =========================================================================
    // SECCIÓN 2: Concepts básicos
    // =========================================================================
    std::cout << "\n--- Concepts básicos ---\n";

    TEST("int128_type<uint128_t>", int128_type<uint128_t>);
    TEST("int128_type<int128_t>", int128_type<int128_t>);
    TEST("!int128_type<int>", !int128_type<int>);

    TEST("uint128_type<uint128_t>", uint128_type<uint128_t>);
    TEST("!uint128_type<int128_t>", !uint128_type<int128_t>);

    TEST("signed_int128_type<int128_t>", signed_int128_type<int128_t>);
    TEST("!signed_int128_type<uint128_t>", !signed_int128_type<uint128_t>);

    // =========================================================================
    // SECCIÓN 3: Concepts de conversión y compatibilidad
    // =========================================================================
    std::cout << "\n--- Concepts de conversión ---\n";

    TEST("int128_convertible<int>", int128_convertible<int>);
    TEST("int128_convertible<uint64_t>", int128_convertible<uint64_t>);
    TEST("int128_convertible<uint128_t>", int128_convertible<uint128_t>);
    TEST("int128_convertible<int128_t>", int128_convertible<int128_t>);
    TEST("!int128_convertible<float>", !int128_convertible<float>);

    TEST("int128_compatible<int>", int128_compatible<int>);
    TEST("int128_compatible<float>", int128_compatible<float>); // float es compatible
    TEST("int128_compatible<double>", int128_compatible<double>);

    TEST("int128_bitwise_compatible<int>", int128_bitwise_compatible<int>);
    TEST("int128_bitwise_compatible<uint128_t>", int128_bitwise_compatible<uint128_t>);
    TEST("!int128_bitwise_compatible<float>", !int128_bitwise_compatible<float>);

    // =========================================================================
    // SECCIÓN 4: Concepts de signedness
    // =========================================================================
    std::cout << "\n--- Concepts de signedness ---\n";

    TEST("int128_signed_compatible<int>", int128_signed_compatible<int>);
    TEST("int128_signed_compatible<int64_t>", int128_signed_compatible<int64_t>);
    TEST("int128_signed_compatible<int128_t>", int128_signed_compatible<int128_t>);
    TEST("!int128_signed_compatible<unsigned>", !int128_signed_compatible<unsigned>);

    TEST("int128_unsigned_compatible<unsigned>", int128_unsigned_compatible<unsigned>);
    TEST("int128_unsigned_compatible<uint64_t>", int128_unsigned_compatible<uint64_t>);
    TEST("int128_unsigned_compatible<uint128_t>", int128_unsigned_compatible<uint128_t>);
    TEST("!int128_unsigned_compatible<int>", !int128_unsigned_compatible<int>);

    // =========================================================================
    // SECCIÓN 5: Concepts de operaciones
    // =========================================================================
    std::cout << "\n--- Concepts de operaciones ---\n";

    TEST("valid_shift_type<int>", valid_shift_type<int>);
    TEST("valid_shift_type<unsigned>", valid_shift_type<unsigned>);
    TEST("valid_shift_type<uint64_t>", valid_shift_type<uint64_t>);

    TEST("bit_index_type<unsigned>", bit_index_type<unsigned>);
    TEST("bit_index_type<size_t>", bit_index_type<size_t>);
    TEST("!bit_index_type<int>", !bit_index_type<int>); // signed no es válido

    TEST("int128_comparable<int>", int128_comparable<int>);
    TEST("int128_comparable<uint128_t>", int128_comparable<uint128_t>);
    TEST("int128_comparable<int128_t>", int128_comparable<int128_t>);

    // =========================================================================
    // SECCIÓN 6: Concepts de metaprogramación
    // =========================================================================
    std::cout << "\n--- Concepts de metaprogramación ---\n";

    TEST("promotable_to_int128<int>", promotable_to_int128<int>);
    TEST("promotable_to_int128<uint64_t>", promotable_to_int128<uint64_t>);
    TEST("!promotable_to_int128<uint128_t>", !promotable_to_int128<uint128_t>);
    TEST("!promotable_to_int128<int128_t>", !promotable_to_int128<int128_t>);

    // =========================================================================
    // SECCIÓN 7: Concepts de algoritmos
    // =========================================================================
    std::cout << "\n--- Concepts de algoritmos ---\n";

    // Predicado lambda
    auto is_positive = [](auto x) { return x > decltype(x)(0); };
    constexpr bool pred_uint128 = int128_predicate<decltype(is_positive), uint128_t>;
    constexpr bool pred_int128 = int128_predicate<decltype(is_positive), int128_t>;
    TEST("int128_predicate<lambda, uint128_t>", pred_uint128);
    TEST("int128_predicate<lambda, int128_t>", pred_int128);

    // Operación binaria lambda
    auto add_op = [](auto a, auto b) { return a + b; };
    constexpr bool binop_uint128 = int128_binary_operation<decltype(add_op), uint128_t>;
    constexpr bool binop_int128 = int128_binary_operation<decltype(add_op), int128_t>;
    TEST("int128_binary_operation<lambda, uint128_t>", binop_uint128);
    TEST("int128_binary_operation<lambda, int128_t>", binop_int128);

    // =========================================================================
    // SECCIÓN 8: Namespaces de compatibilidad
    // =========================================================================
    std::cout << "\n--- Namespaces de compatibilidad ---\n";

    // uint128_concepts
    TEST("uint128_concepts::uint128_convertible<int>", uint128_concepts::uint128_convertible<int>);
    TEST("uint128_concepts::uint128_compatible<float>",
         uint128_concepts::uint128_compatible<float>);

    // int128_concepts
    TEST("int128_concepts::int128_convertible<int>", int128_concepts::int128_convertible<int>);
    TEST("int128_concepts::int128_signed_compatible<int>",
         int128_concepts::int128_signed_compatible<int>);

    // =========================================================================
    // SECCIÓN 9: Uso práctico con templates
    // =========================================================================
    std::cout << "\n--- Uso práctico con templates ---\n";

    // Función template que usa concepts
    auto test_function = []<int128_type T>(T val) -> T { return val + T(1); };

    uint128_t u = test_function(uint128_t(100));
    int128_t i = test_function(int128_t(-50));
    TEST("Template con int128_type concept (uint128)", u == uint128_t(101));
    TEST("Template con int128_type concept (int128)", i == int128_t(-49));

    // Función con concept de comparabilidad
    auto compare_values = []<int128_comparable T>(T a, T b) -> bool { return a < b; };
    TEST("Template con int128_comparable (uint128)", compare_values(uint128_t(10), uint128_t(20)));
    TEST("Template con int128_comparable (int)", compare_values(5, 10));

    // =========================================================================
    // RESULTADO FINAL
    // =========================================================================
    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
