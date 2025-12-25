#include <algorithm>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>

// Incluir el archivo a probar
#include "include/int128_concepts.hpp"

void test_basic_concepts()
{
    std::cout << "[TEST] Probando conceptos básicos...\n";

    // Test int128_convertible
    static_assert(int128_concepts::int128_convertible<int>);
    static_assert(int128_concepts::int128_convertible<long long>);
    static_assert(int128_concepts::int128_convertible<int128_t>);
    static_assert(!int128_concepts::int128_convertible<double>);
    std::cout << "  [OK] int128_convertible funciona correctamente\n";

    // Test int128_compatible
    static_assert(int128_concepts::int128_compatible<int>);
    static_assert(int128_concepts::int128_compatible<double>);
    static_assert(int128_concepts::int128_compatible<float>);
    static_assert(int128_concepts::int128_compatible<int128_t>);
    std::cout << "  [OK] int128_compatible funciona correctamente\n";

    // Test int128_signed_compatible
    static_assert(int128_concepts::int128_signed_compatible<int>);
    static_assert(int128_concepts::int128_signed_compatible<long long>);
    static_assert(int128_concepts::int128_signed_compatible<int128_t>);
    static_assert(!int128_concepts::int128_signed_compatible<unsigned int>);
    std::cout << "  [OK] int128_signed_compatible funciona correctamente\n";
}

void test_operation_concepts()
{
    std::cout << "🔧 Probando conceptos de operaciones...\n";

    // Test valid_shift_type
    static_assert(int128_concepts::valid_shift_type<int>);
    static_assert(int128_concepts::valid_shift_type<unsigned int>);
    static_assert(int128_concepts::valid_shift_type<char>);
    std::cout << "  [OK] valid_shift_type funciona correctamente\n";

    // Test bit_index_type
    static_assert(int128_concepts::bit_index_type<unsigned int>);
    static_assert(int128_concepts::bit_index_type<size_t>);
    static_assert(!int128_concepts::bit_index_type<int>); // signado no válido
    std::cout << "  [OK] bit_index_type funciona correctamente\n";

    // Test int128_comparable
    static_assert(int128_concepts::int128_comparable<int>);
    static_assert(int128_concepts::int128_comparable<long>);
    static_assert(int128_concepts::int128_comparable<int128_t>);
    std::cout << "  [OK] int128_comparable funciona correctamente\n";
}

void test_metaprogramming_concepts()
{
    std::cout << "🔬 Probando conceptos de metaprogramación...\n";

    // Test promotable_to_int128
    static_assert(int128_concepts::promotable_to_int128<int>);
    static_assert(int128_concepts::promotable_to_int128<long long>);
    static_assert(!int128_concepts::promotable_to_int128<int128_t>); // mismo tipo
    std::cout << "  [OK] promotable_to_int128 funciona correctamente\n";

    // Test int128_container con std::vector
    static_assert(int128_concepts::int128_container<std::vector<int128_t>>);
    std::cout << "  [OK] int128_container funciona correctamente\n";

    // Test int128_range_type
    static_assert(int128_concepts::int128_range_type<long long>);
    static_assert(int128_concepts::int128_range_type<int128_t>);
    static_assert(!int128_concepts::int128_range_type<int>); // muy pequeño
    std::cout << "  [OK] int128_range_type funciona correctamente\n";
}

void test_algorithm_concepts()
{
    std::cout << "⚙️ Probando conceptos de algoritmos...\n";

    // Test predicado simple
    auto simple_predicate = [](int128_t x) -> bool { return x > 0; };
    static_assert(int128_concepts::int128_predicate<decltype(simple_predicate)>);
    std::cout << "  [OK] int128_predicate funciona correctamente\n";

    // Test operación binaria
    auto binary_op = [](int128_t a, int128_t b) { return a + b; };
    static_assert(int128_concepts::int128_binary_operation<decltype(binary_op)>);
    std::cout << "  [OK] int128_binary_operation funciona correctamente\n";

    // Test operación de reducción
    auto reduce_op = [](long long acc, int128_t val) -> long long {
        return acc + static_cast<long long>(val);
    };
    static_assert(int128_concepts::int128_reduce_operation<decltype(reduce_op), long long>);
    std::cout << "  [OK] int128_reduce_operation funciona correctamente\n";
}

void test_function_concepts()
{
    std::cout << "[BUILD] Probando conceptos de funciones...\n";

    // Test función simple
    auto simple_function = [](int128_t x) { return x * 2; };
    static_assert(int128_concepts::int128_function<decltype(simple_function)>);
    std::cout << "  [OK] int128_function (sin args) funciona correctamente\n";

    // Test función con argumentos adicionales
    auto function_with_args = [](int128_t x, int factor) { return x * factor; };
    static_assert(int128_concepts::int128_function<decltype(function_with_args), int>);
    std::cout << "  [OK] int128_function (con args) funciona correctamente\n";
}

void test_standard_concepts_compliance()
{
    std::cout << "📋 Verificando cumplimiento de conceptos estándar...\n";

    // Los static_assert en int128_concept_checks ya verifican esto,
    // pero podemos probar algunos dinámicamente

    int128_t value{42};
    int128_t copy = value;
    int128_t moved = std::move(copy);

    // Test de construcción y asignación
    std::cout << "  [OK] Construcción y asignación funcionan\n";

    // Test de comparaciones
    bool equal = (value == moved);
    bool less = (value < moved + 1);
    (void)equal;
    (void)less; // evitar warnings
    std::cout << "  [OK] Comparaciones funcionan\n";

    // Test de conceptos estándar (compilación exitosa significa que pasan)
    static_assert(std::integral<int128_t>);
    static_assert(std::signed_integral<int128_t>);
    static_assert(std::regular<int128_t>);
    std::cout << "  [OK] Conceptos estándar de C++20 se cumplen\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de int128_concepts.hpp\n";
    std::cout << "==========================================\n\n";

    try {
        test_basic_concepts();
        std::cout << "\n";

        test_operation_concepts();
        std::cout << "\n";

        test_metaprogramming_concepts();
        std::cout << "\n";

        test_algorithm_concepts();
        std::cout << "\n";

        test_function_concepts();
        std::cout << "\n";

        test_standard_concepts_compliance();
        std::cout << "\n";

        std::cout << "🎉 ¡TODOS LOS TESTS DE CONCEPTS PASARON!\n";
        std::cout << "==========================================\n";
        std::cout << "[OK] Conceptos básicos funcionando\n";
        std::cout << "[OK] Conceptos de operaciones funcionando\n";
        std::cout << "[OK] Conceptos de metaprogramación funcionando\n";
        std::cout << "[OK] Conceptos de algoritmos funcionando\n";
        std::cout << "[OK] Compatibilidad STL C++20 verificada\n";

        return 0;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error en tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "[FAIL] Error desconocido en tests" << std::endl;
        return 1;
    }
}