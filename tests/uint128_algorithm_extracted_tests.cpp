/*
 * Tests para uint128_algorithm.hpp
 * Verifica todos los algoritmos especializados para uint128_t
 */

#include "../include/uint128/uint128_algorithm.hpp"
#include "../include/uint128/uint128_t.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace nstd;

// Para que std::traits funcionen con 'uint128_t' a secas
using uint128_t = nstd::uint128_t;
using int128_t = nstd::int128_t;

using namespace uint128_algorithm;

// ===============================================================================
// TESTS DE BÚSQUEDA
// ===============================================================================

void test_binary_search_uint128()
{
    std::cout << "Test: binary_search_uint128...";

    std::vector<uint128_t> vec = {uint128_t(10), uint128_t(20), uint128_t(30), uint128_t(40),
                                  uint128_t(50)};

    // Búsqueda de valores existentes
    assert(binary_search_uint128(vec.begin(), vec.end(), uint128_t(30)));
    assert(binary_search_uint128(vec.begin(), vec.end(), uint128_t(10)));
    assert(binary_search_uint128(vec.begin(), vec.end(), uint128_t(50)));

    // Búsqueda de valores inexistentes
    assert(!binary_search_uint128(vec.begin(), vec.end(), uint128_t(15)));
    assert(!binary_search_uint128(vec.begin(), vec.end(), uint128_t(100)));
    assert(!binary_search_uint128(vec.begin(), vec.end(), uint128_t(0)));

    // Rango vacío
    std::vector<uint128_t> empty;
    assert(!binary_search_uint128(empty.begin(), empty.end(), uint128_t(10)));

    std::cout << " [PASS]\n";
}

void test_find_if_uint128()
{
    std::cout << "Test: find_if_uint128...";

    std::vector<uint128_t> vec = {uint128_t(5), uint128_t(10), uint128_t(15), uint128_t(20),
                                  uint128_t(25)};

    // Encontrar primer número mayor que 12
    auto it = find_if_uint128(vec.begin(), vec.end(),
                              [](const uint128_t& x) { return x > uint128_t(12); });
    assert(it != vec.end());
    assert(*it == uint128_t(15));

    // Encontrar primer número par (divisible por 2)
    auto it2 = find_if_uint128(vec.begin(), vec.end(), [](const uint128_t& x) {
        return (x % uint128_t(2)) == uint128_t(0);
    });
    assert(it2 != vec.end());
    assert(*it2 == uint128_t(10));

    // No encontrar número mayor que 100
    auto it3 = find_if_uint128(vec.begin(), vec.end(),
                               [](const uint128_t& x) { return x > uint128_t(100); });
    assert(it3 == vec.end());

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE TRANSFORMACIÓN
// ===============================================================================

void test_transform_uint128()
{
    std::cout << "Test: transform_uint128...";

    std::vector<uint128_t> input = {uint128_t(1), uint128_t(2), uint128_t(3), uint128_t(4),
                                    uint128_t(5)};
    std::vector<uint128_t> output(5);

    // Duplicar cada elemento
    transform_uint128(input.begin(), input.end(), output.begin(),
                      [](const uint128_t& x) { return x * uint128_t(2); });

    assert(output[0] == uint128_t(2));
    assert(output[1] == uint128_t(4));
    assert(output[2] == uint128_t(6));
    assert(output[3] == uint128_t(8));
    assert(output[4] == uint128_t(10));

    // Elevar al cuadrado
    transform_uint128(input.begin(), input.end(), output.begin(),
                      [](const uint128_t& x) { return x * x; });

    assert(output[0] == uint128_t(1));
    assert(output[1] == uint128_t(4));
    assert(output[2] == uint128_t(9));
    assert(output[3] == uint128_t(16));
    assert(output[4] == uint128_t(25));

    std::cout << " [PASS]\n";
}

void test_for_each_uint128()
{
    std::cout << "Test: for_each_uint128...";

    std::vector<uint128_t> vec = {uint128_t(1), uint128_t(2), uint128_t(3)};

    // Contar elementos
    size_t count = 0;
    for_each_uint128(vec.begin(), vec.end(), [&count](const uint128_t&) { ++count; });
    assert(count == 3);

    // Sumar todos los elementos
    uint128_t sum = 0;
    for_each_uint128(vec.begin(), vec.end(), [&sum](const uint128_t& x) { sum += x; });
    assert(sum == uint128_t(6));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE REDUCCIÓN
// ===============================================================================

void test_accumulate_uint128()
{
    std::cout << "Test: accumulate_uint128...";

    std::vector<uint128_t> vec = {uint128_t(10), uint128_t(20), uint128_t(30)};

    // Suma con valor inicial 0
    auto sum = accumulate_uint128(vec.begin(), vec.end(), uint128_t(0), std::plus<uint128_t>());
    assert(sum == uint128_t(60));

    // Suma con valor inicial 100
    auto sum2 = accumulate_uint128(vec.begin(), vec.end(), uint128_t(100), std::plus<uint128_t>());
    assert(sum2 == uint128_t(160));

    // Producto
    auto product =
        accumulate_uint128(vec.begin(), vec.end(), uint128_t(1), std::multiplies<uint128_t>());
    assert(product == uint128_t(6000));

    std::cout << " [PASS]\n";
}

void test_sum_uint128()
{
    std::cout << "Test: sum_uint128...";

    std::vector<uint128_t> vec = {uint128_t(5), uint128_t(10), uint128_t(15), uint128_t(20)};

    auto sum = sum_uint128(vec.begin(), vec.end());
    assert(sum == uint128_t(50));

    // Rango vacío
    std::vector<uint128_t> empty;
    auto sum_empty = sum_uint128(empty.begin(), empty.end());
    assert(sum_empty == uint128_t(0));

    // Un solo elemento
    std::vector<uint128_t> single = {uint128_t(42)};
    auto sum_single = sum_uint128(single.begin(), single.end());
    assert(sum_single == uint128_t(42));

    std::cout << " [PASS]\n";
}

void test_product_uint128()
{
    std::cout << "Test: product_uint128...";

    std::vector<uint128_t> vec = {uint128_t(2), uint128_t(3), uint128_t(4)};

    auto product = product_uint128(vec.begin(), vec.end());
    assert(product == uint128_t(24));

    // Con un cero
    std::vector<uint128_t> with_zero = {uint128_t(2), uint128_t(0), uint128_t(4)};
    auto product_zero = product_uint128(with_zero.begin(), with_zero.end());
    assert(product_zero == uint128_t(0));

    // Rango vacío (producto identidad = 1)
    std::vector<uint128_t> empty;
    auto product_empty = product_uint128(empty.begin(), empty.end());
    assert(product_empty == uint128_t(1));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE PARTICIÓN Y ORDENAMIENTO
// ===============================================================================

void test_partition_uint128()
{
    std::cout << "Test: partition_uint128...";

    std::vector<uint128_t> vec = {uint128_t(1), uint128_t(2), uint128_t(3),
                                  uint128_t(4), uint128_t(5), uint128_t(6)};

    // Particionar por números pares
    auto middle = partition_uint128(vec.begin(), vec.end(), [](const uint128_t& x) {
        return (x % uint128_t(2)) == uint128_t(0);
    });

    // Verificar que todos los elementos antes de middle son pares
    for (auto it = vec.begin(); it != middle; ++it) {
        assert((*it % uint128_t(2)) == uint128_t(0));
    }

    // Verificar que todos los elementos desde middle son impares
    for (auto it = middle; it != vec.end(); ++it) {
        assert((*it % uint128_t(2)) != uint128_t(0));
    }

    std::cout << " [PASS]\n";
}

void test_sort_uint128()
{
    std::cout << "Test: sort_uint128...";

    std::vector<uint128_t> vec = {uint128_t(50), uint128_t(20), uint128_t(80), uint128_t(10),
                                  uint128_t(30)};

    // Ordenar ascendente
    sort_uint128(vec.begin(), vec.end());
    assert(vec[0] == uint128_t(10));
    assert(vec[1] == uint128_t(20));
    assert(vec[2] == uint128_t(30));
    assert(vec[3] == uint128_t(50));
    assert(vec[4] == uint128_t(80));

    // Ordenar descendente
    sort_uint128(vec.begin(), vec.end(), std::greater<uint128_t>());
    assert(vec[0] == uint128_t(80));
    assert(vec[1] == uint128_t(50));
    assert(vec[2] == uint128_t(30));
    assert(vec[3] == uint128_t(20));
    assert(vec[4] == uint128_t(10));

    std::cout << " [PASS]\n";
}

// ===============================================================================
// TESTS DE ALGORITMOS ESPECIALIZADOS
// ===============================================================================

void test_gcd_range()
{
    std::cout << "Test: gcd_range...";

    std::vector<uint128_t> vec = {uint128_t(12), uint128_t(18), uint128_t(24)};

    auto gcd = gcd_range(vec.begin(), vec.end());
    assert(gcd == uint128_t(6));

    // GCD de números coprimos
    std::vector<uint128_t> coprime = {uint128_t(7), uint128_t(11), uint128_t(13)};
    auto gcd_coprime = gcd_range(coprime.begin(), coprime.end());
    assert(gcd_coprime == uint128_t(1));

    // Rango vacío
    std::vector<uint128_t> empty;
    auto gcd_empty = gcd_range(empty.begin(), empty.end());
    assert(gcd_empty == uint128_t(0));

    std::cout << " [PASS]\n";
}

void test_lcm_range()
{
    std::cout << "Test: lcm_range...";

    std::vector<uint128_t> vec = {uint128_t(4), uint128_t(6), uint128_t(8)};

    auto lcm = lcm_range(vec.begin(), vec.end());
    assert(lcm == uint128_t(24));

    // Con un cero (lcm debe ser 0)
    std::vector<uint128_t> with_zero = {uint128_t(4), uint128_t(0), uint128_t(8)};
    auto lcm_zero = lcm_range(with_zero.begin(), with_zero.end());
    assert(lcm_zero == uint128_t(0));

    // Rango vacío
    std::vector<uint128_t> empty;
    auto lcm_empty = lcm_range(empty.begin(), empty.end());
    assert(lcm_empty == uint128_t(0));

    std::cout << " [PASS]\n";
}

void test_generate_arithmetic_sequence()
{
    std::cout << "Test: generate_arithmetic_sequence...";

    std::vector<uint128_t> seq(5);

    // Secuencia: 10, 15, 20, 25, 30
    generate_arithmetic_sequence(seq.begin(), 5, uint128_t(10), uint128_t(5));

    assert(seq[0] == uint128_t(10));
    assert(seq[1] == uint128_t(15));
    assert(seq[2] == uint128_t(20));
    assert(seq[3] == uint128_t(25));
    assert(seq[4] == uint128_t(30));

    // Secuencia descendente: 100, 90, 80, 70, 60
    std::vector<uint128_t> seq_desc(5);
    generate_arithmetic_sequence(seq_desc.begin(), 5, uint128_t(100), uint128_t(-10));

    assert(seq_desc[0] == uint128_t(100));
    assert(seq_desc[1] == uint128_t(90));
    assert(seq_desc[2] == uint128_t(80));
    assert(seq_desc[3] == uint128_t(70));
    assert(seq_desc[4] == uint128_t(60));

    std::cout << " [PASS]\n";
}

void test_generate_geometric_sequence()
{
    std::cout << "Test: generate_geometric_sequence...";

    std::vector<uint128_t> seq(5);

    // Secuencia: 2, 4, 8, 16, 32
    generate_geometric_sequence(seq.begin(), 5, uint128_t(2), uint128_t(2));

    assert(seq[0] == uint128_t(2));
    assert(seq[1] == uint128_t(4));
    assert(seq[2] == uint128_t(8));
    assert(seq[3] == uint128_t(16));
    assert(seq[4] == uint128_t(32));

    // Secuencia: 5, 15, 45, 135, 405
    std::vector<uint128_t> seq2(5);
    generate_geometric_sequence(seq2.begin(), 5, uint128_t(5), uint128_t(3));

    assert(seq2[0] == uint128_t(5));
    assert(seq2[1] == uint128_t(15));
    assert(seq2[2] == uint128_t(45));
    assert(seq2[3] == uint128_t(135));
    assert(seq2[4] == uint128_t(405));

    std::cout << " [PASS]\n";
}

void test_calculate_stats()
{
    std::cout << "Test: calculate_stats...";

    std::vector<uint128_t> vec = {uint128_t(10), uint128_t(20), uint128_t(30), uint128_t(40),
                                  uint128_t(50)};

    auto stats = calculate_stats(vec.begin(), vec.end());

    assert(stats.min_value == uint128_t(10));
    assert(stats.max_value == uint128_t(50));
    assert(stats.sum == uint128_t(150));
    assert(stats.count == 5);
    assert(stats.mean() == 30.0);

    // Un solo elemento
    std::vector<uint128_t> single = {uint128_t(42)};
    auto stats_single = calculate_stats(single.begin(), single.end());
    assert(stats_single.min_value == uint128_t(42));
    assert(stats_single.max_value == uint128_t(42));
    assert(stats_single.sum == uint128_t(42));
    assert(stats_single.count == 1);

    // Rango vacío
    std::vector<uint128_t> empty;
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
    std::cout << "  Tests: uint128_algorithm.hpp\n";
    std::cout << "========================================\n\n";

    // Tests de búsqueda
    test_binary_search_uint128();
    test_find_if_uint128();

    // Tests de transformación
    test_transform_uint128();
    test_for_each_uint128();

    // Tests de reducción
    test_accumulate_uint128();
    test_sum_uint128();
    test_product_uint128();

    // Tests de partición y ordenamiento
    test_partition_uint128();
    test_sort_uint128();

    // Tests de algoritmos especializados
    test_gcd_range();
    test_lcm_range();
    test_generate_arithmetic_sequence();
    test_generate_geometric_sequence();
    test_calculate_stats();

    std::cout << "\n========================================\n";
    std::cout << "  [OK] Todos los tests pasaron\n";
    std::cout << "========================================\n";

    return 0;
}

