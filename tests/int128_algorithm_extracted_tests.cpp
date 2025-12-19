/*
 * Tests para int128_algorithm.hpp
 * Verifica todos los algoritmos especializados para int128_t
 */

#include "int128/int128_algorithm.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace int128_algorithm;

// ===============================================================================
// TESTS DE BUSQUEDA
// ===============================================================================

void test_binary_search_int128()
{
    std::cout << "Test: binary_search_int128...";

    std::vector<int128_t> vec = {int128_t(-50), int128_t(-20), int128_t(10), int128_t(30),
                                 int128_t(50)};

    // Valores existentes
    assert(binary_search_int128(vec.begin(), vec.end(), int128_t(30)));
    assert(binary_search_int128(vec.begin(), vec.end(), int128_t(-20)));
    assert(binary_search_int128(vec.begin(), vec.end(), int128_t(-50)));

    // Valores inexistentes
    assert(!binary_search_int128(vec.begin(), vec.end(), int128_t(15)));
    assert(!binary_search_int128(vec.begin(), vec.end(), int128_t(-100)));

    // Rango vacio
    std::vector<int128_t> empty;
    assert(!binary_search_int128(empty.begin(), empty.end(), int128_t(10)));

    std::cout << " [PASS]\n";
}

void test_find_if_int128()
{
    std::cout << "Test: find_if_int128...";

    std::vector<int128_t> vec = {int128_t(-15), int128_t(-5), int128_t(5), int128_t(10),
                                 int128_t(15)};

    // Encontrar primer positivo mayor que 7
    auto it =
        find_if_int128(vec.begin(), vec.end(), [](const int128_t& x) { return x > int128_t(7); });
    assert(it != vec.end() && *it == int128_t(10));

    // Encontrar primer negativo
    auto it2 =
        find_if_int128(vec.begin(), vec.end(), [](const int128_t& x) { return x < int128_t(0); });
    assert(it2 != vec.end() && *it2 == int128_t(-15));

    // No encontrar numero mayor que 100
    auto it3 =
        find_if_int128(vec.begin(), vec.end(), [](const int128_t& x) { return x > int128_t(100); });
    assert(it3 == vec.end());

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE TRANSFORMACION
// ===============================================================================

void test_transform_int128()
{
    std::cout << "Test: transform_int128...";

    std::vector<int128_t> input = {int128_t(-5), int128_t(10), int128_t(-15)};
    std::vector<int128_t> output(input.size());

    // Duplicar valores
    transform_int128(input.begin(), input.end(), output.begin(),
                     [](const int128_t& x) { return x * int128_t(2); });
    assert(output[0] == int128_t(-10) && output[1] == int128_t(20) && output[2] == int128_t(-30));

    // Valor absoluto
    transform_int128(input.begin(), input.end(), output.begin(),
                     [](const int128_t& x) { return x.abs(); });
    assert(output[0] == int128_t(5) && output[1] == int128_t(10) && output[2] == int128_t(15));

    std::cout << " [PASS]\n";
}

void test_for_each_int128()
{
    std::cout << "Test: for_each_int128...";

    std::vector<int128_t> vec = {int128_t(-2), int128_t(1), int128_t(3)};

    // Contar elementos
    int count = 0;
    for_each_int128(vec.begin(), vec.end(), [&count](const int128_t&) { count++; });
    assert(count == 3);

    // Sumar elementos
    int128_t sum = 0;
    for_each_int128(vec.begin(), vec.end(), [&sum](const int128_t& x) { sum += x; });
    assert(sum == int128_t(2)); // -2 + 1 + 3

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE REDUCCION
// ===============================================================================

void test_accumulate_int128()
{
    std::cout << "Test: accumulate_int128...";

    std::vector<int128_t> vec = {int128_t(-10), int128_t(20), int128_t(-30), int128_t(40)};

    // Suma con acumulador inicial
    auto sum = accumulate_int128(vec.begin(), vec.end(), int128_t(100), std::plus<int128_t>());
    assert(sum == int128_t(120)); // 100 - 10 + 20 - 30 + 40

    // Producto con acumulador inicial
    std::vector<int128_t> vec2 = {int128_t(-2), int128_t(3), int128_t(-4)};
    auto product =
        accumulate_int128(vec2.begin(), vec2.end(), int128_t(1), std::multiplies<int128_t>());
    assert(product == int128_t(24)); // 1 * (-2) * 3 * (-4) = 24

    std::cout << " [PASS]\n";
}

void test_sum_int128()
{
    std::cout << "Test: sum_int128...";

    std::vector<int128_t> vec = {int128_t(-5), int128_t(10), int128_t(-15), int128_t(20)};
    auto sum = sum_int128(vec.begin(), vec.end());
    assert(sum == int128_t(10)); // -5 + 10 - 15 + 20

    // Rango vacio
    std::vector<int128_t> empty;
    auto sum_empty = sum_int128(empty.begin(), empty.end());
    assert(sum_empty == int128_t(0));

    // Solo negativos
    std::vector<int128_t> negatives = {int128_t(-1), int128_t(-2), int128_t(-3)};
    auto sum_neg = sum_int128(negatives.begin(), negatives.end());
    assert(sum_neg == int128_t(-6));

    std::cout << " [PASS]\n";
}

void test_product_int128()
{
    std::cout << "Test: product_int128...";

    std::vector<int128_t> vec = {int128_t(-2), int128_t(3), int128_t(-4)};
    auto product = product_int128(vec.begin(), vec.end());
    assert(product == int128_t(24)); // (-2) * 3 * (-4)

    // Con cero
    std::vector<int128_t> with_zero = {int128_t(2), int128_t(0), int128_t(3)};
    auto product_zero = product_int128(with_zero.begin(), with_zero.end());
    assert(product_zero == int128_t(0));

    // Rango vacio (debe ser 1)
    std::vector<int128_t> empty;
    auto product_empty = product_int128(empty.begin(), empty.end());
    assert(product_empty == int128_t(1));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE PARTICION Y ORDENAMIENTO
// ===============================================================================

void test_partition_int128()
{
    std::cout << "Test: partition_int128...";

    std::vector<int128_t> vec = {int128_t(-10), int128_t(15), int128_t(-20), int128_t(25),
                                 int128_t(30)};

    // Particionar: negativos primero
    auto middle =
        partition_int128(vec.begin(), vec.end(), [](const int128_t& x) { return x < int128_t(0); });

    // Verificar que elementos antes de middle son negativos
    for (auto it = vec.begin(); it != middle; ++it) {
        assert(*it < int128_t(0));
    }

    // Verificar que elementos despues de middle son positivos
    for (auto it = middle; it != vec.end(); ++it) {
        assert(*it >= int128_t(0));
    }

    std::cout << " [PASS]\n";
}

void test_sort_int128()
{
    std::cout << "Test: sort_int128...";

    std::vector<int128_t> vec = {int128_t(30), int128_t(-10), int128_t(50), int128_t(-20),
                                 int128_t(10)};

    // Orden ascendente (por defecto)
    sort_int128(vec.begin(), vec.end());
    assert(vec[0] == int128_t(-20) && vec[1] == int128_t(-10) && vec[2] == int128_t(10) &&
           vec[3] == int128_t(30) && vec[4] == int128_t(50));

    // Orden descendente
    sort_int128(vec.begin(), vec.end(), std::greater<int128_t>());
    assert(vec[0] == int128_t(50) && vec[1] == int128_t(30) && vec[2] == int128_t(10) &&
           vec[3] == int128_t(-10) && vec[4] == int128_t(-20));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE GCD/LCM
// ===============================================================================

void test_gcd_range()
{
    std::cout << "Test: gcd_range...";

    std::vector<int128_t> vec = {int128_t(12), int128_t(-18), int128_t(24)};
    auto gcd = gcd_range(vec.begin(), vec.end());
    assert(gcd == int128_t(6));

    // Con negativos (GCD siempre positivo)
    std::vector<int128_t> vec2 = {int128_t(-15), int128_t(-25), int128_t(-35)};
    auto gcd2 = gcd_range(vec2.begin(), vec2.end());
    assert(gcd2 == int128_t(5));

    // Coprimos
    std::vector<int128_t> coprimes = {int128_t(7), int128_t(11), int128_t(13)};
    auto gcd3 = gcd_range(coprimes.begin(), coprimes.end());
    assert(gcd3 == int128_t(1));

    std::cout << " [PASS]\n";
}

void test_lcm_range()
{
    std::cout << "Test: lcm_range...";

    std::vector<int128_t> vec = {int128_t(4), int128_t(-6), int128_t(8)};
    auto lcm = lcm_range(vec.begin(), vec.end());
    assert(lcm == int128_t(24));

    // Con negativos (LCM siempre positivo)
    std::vector<int128_t> vec2 = {int128_t(-3), int128_t(-4), int128_t(-5)};
    auto lcm2 = lcm_range(vec2.begin(), vec2.end());
    assert(lcm2 == int128_t(60));

    // Con cero (resultado es 0)
    std::vector<int128_t> with_zero = {int128_t(4), int128_t(0), int128_t(6)};
    auto lcm3 = lcm_range(with_zero.begin(), with_zero.end());
    assert(lcm3 == int128_t(0));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE GENERACION DE SECUENCIAS
// ===============================================================================

void test_generate_arithmetic_sequence()
{
    std::cout << "Test: generate_arithmetic_sequence...";

    std::vector<int128_t> seq(5);

    // Secuencia ascendente: 10, 15, 20, 25, 30
    generate_arithmetic_sequence(seq.begin(), 5, int128_t(10), int128_t(5));
    assert(seq[0] == int128_t(10) && seq[1] == int128_t(15) && seq[2] == int128_t(20) &&
           seq[3] == int128_t(25) && seq[4] == int128_t(30));

    // Secuencia descendente: 20, 10, 0, -10, -20
    generate_arithmetic_sequence(seq.begin(), 5, int128_t(20), int128_t(-10));
    assert(seq[0] == int128_t(20) && seq[1] == int128_t(10) && seq[2] == int128_t(0) &&
           seq[3] == int128_t(-10) && seq[4] == int128_t(-20));

    std::cout << " [PASS]\n";
}

void test_generate_geometric_sequence()
{
    std::cout << "Test: generate_geometric_sequence...";

    std::vector<int128_t> seq(5);

    // Secuencia: 2, 4, 8, 16, 32
    generate_geometric_sequence(seq.begin(), 5, int128_t(2), int128_t(2));
    assert(seq[0] == int128_t(2) && seq[1] == int128_t(4) && seq[2] == int128_t(8) &&
           seq[3] == int128_t(16) && seq[4] == int128_t(32));

    // Secuencia con ratio negativo: 3, -6, 12, -24, 48
    generate_geometric_sequence(seq.begin(), 5, int128_t(3), int128_t(-2));
    assert(seq[0] == int128_t(3) && seq[1] == int128_t(-6) && seq[2] == int128_t(12) &&
           seq[3] == int128_t(-24) && seq[4] == int128_t(48));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS ESPECIFICOS DE SIGNADOS
// ===============================================================================

void test_partition_by_sign()
{
    std::cout << "Test: partition_by_sign...";

    std::vector<int128_t> vec = {int128_t(-5), int128_t(10), int128_t(-15),
                                 int128_t(0),  int128_t(20), int128_t(-25)};

    auto middle = partition_by_sign(vec.begin(), vec.end());

    // Verificar negativos a la izquierda
    for (auto it = vec.begin(); it != middle; ++it) {
        assert(*it < int128_t(0));
    }

    // Verificar positivos/cero a la derecha
    for (auto it = middle; it != vec.end(); ++it) {
        assert(*it >= int128_t(0));
    }

    std::cout << " [PASS]\n";
}

void test_max_abs_value()
{
    std::cout << "Test: max_abs_value...";

    std::vector<int128_t> vec = {int128_t(10), int128_t(-50), int128_t(30), int128_t(-20)};

    auto max_abs = max_abs_value(vec.begin(), vec.end());
    assert(max_abs == int128_t(-50)); // Mantiene signo original, pero |-50| es el mayor

    // Solo positivos
    std::vector<int128_t> positives = {int128_t(5), int128_t(15), int128_t(3)};
    auto max_abs2 = max_abs_value(positives.begin(), positives.end());
    assert(max_abs2 == int128_t(15));

    // Con cero
    std::vector<int128_t> with_zero = {int128_t(0), int128_t(-1), int128_t(1)};
    auto max_abs3 = max_abs_value(with_zero.begin(), with_zero.end());
    assert(max_abs3 == int128_t(-1) || max_abs3 == int128_t(1)); // Ambos tienen |1|

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE ESTADISTICAS
// ===============================================================================

void test_calculate_stats()
{
    std::cout << "Test: calculate_stats...";

    std::vector<int128_t> vec = {int128_t(-10), int128_t(20), int128_t(0), int128_t(-30),
                                 int128_t(40)};

    auto stats = calculate_stats(vec.begin(), vec.end());

    assert(stats.min_value == int128_t(-30));
    assert(stats.max_value == int128_t(40));
    assert(stats.sum == int128_t(20)); // -10 + 20 + 0 - 30 + 40
    assert(stats.count == 5);
    assert(stats.negative_count == 2);
    assert(stats.positive_count == 2);
    assert(stats.zero_count == 1);
    assert(stats.has_negative_values());
    assert(stats.has_positive_values());
    assert(stats.has_zero_values());

    // Rango vacio
    std::vector<int128_t> empty;
    auto stats_empty = calculate_stats(empty.begin(), empty.end());
    assert(stats_empty.count == 0);

    std::cout << " [PASS]\n";
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  Test Suite: int128_algorithm.hpp\n";
    std::cout << "========================================\n\n";

    test_binary_search_int128();
    test_find_if_int128();
    test_transform_int128();
    test_for_each_int128();
    test_accumulate_int128();
    test_sum_int128();
    test_product_int128();
    test_partition_int128();
    test_sort_int128();
    test_gcd_range();
    test_lcm_range();
    test_generate_arithmetic_sequence();
    test_generate_geometric_sequence();
    test_partition_by_sign();
    test_max_abs_value();
    test_calculate_stats();

    std::cout << "\n========================================\n";
    std::cout << "  [OK] Todos los tests pasaron\n";
    std::cout << "========================================\n";

    return 0;
}
