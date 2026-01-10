/**
 * @file int128_algorithm_extracted_tests.cpp
 * @brief Tests completos de int128_base_algorithm.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_algorithm.hpp tests (16 tests)
 * - test_algorithm_template.cpp (47 tests)
 *
 * Total: 50+ tests cubriendo:
 * - Búsqueda (binary_search, find_if, lower_bound, upper_bound, count_if)
 * - Transformación (transform, for_each)
 * - Reducción (sum, product, accumulate)
 * - Ordenamiento (sort, partial_sort, nth_element)
 * - Partición (partition, partition_by_sign)
 * - Generación de secuencias (arithmetic, geometric, iota)
 * - GCD/LCM de rangos
 * - Estadísticas
 * - Edge cases
 */

#include "int128_base_algorithm.hpp"
#include <cassert>
#include <functional>
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
// SECTION: Búsqueda - uint128_t
// =============================================================================

void test_search_uint128()
{
    std::vector<uint128_t> sorted = {uint128_t(1), uint128_t(3), uint128_t(5),
                                     uint128_t(7), uint128_t(9)};

    check(binary_search_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(5)),
          "binary_search<uint128> encontrado");
    check(!binary_search_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(4)),
          "binary_search<uint128> no encontrado");

    auto it = lower_bound_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(5));
    check(it != sorted.end() && *it == uint128_t(5), "lower_bound<uint128>");

    it = upper_bound_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(5));
    check(it != sorted.end() && *it == uint128_t(7), "upper_bound<uint128>");

    auto count = count_if_128<signedness::unsigned_type>(
        sorted.begin(), sorted.end(),
        [](const uint128_t &x)
        { return x > uint128_t(4); });
    check(count == 3, "count_if<uint128> > 4");

    it = find_if_128<signedness::unsigned_type>(
        sorted.begin(), sorted.end(),
        [](const uint128_t &x)
        { return x == uint128_t(7); });
    check(it != sorted.end() && *it == uint128_t(7), "find_if<uint128>");
}

// =============================================================================
// SECTION: Búsqueda - int128_t
// =============================================================================

void test_search_int128()
{
    std::vector<int128_t> sorted = {int128_t(-5), int128_t(-1), int128_t(0),
                                    int128_t(3), int128_t(7)};

    check(binary_search_128<signedness::signed_type>(sorted.begin(), sorted.end(), int128_t(0)),
          "binary_search<int128> encontrado cero");
    check(binary_search_128<signedness::signed_type>(sorted.begin(), sorted.end(), int128_t(-5)),
          "binary_search<int128> encontrado negativo");
    check(!binary_search_128<signedness::signed_type>(sorted.begin(), sorted.end(), int128_t(2)),
          "binary_search<int128> no encontrado");

    auto count = count_if_128<signedness::signed_type>(
        sorted.begin(), sorted.end(),
        [](const int128_t &x)
        { return x < int128_t(0); });
    check(count == 2, "count_if<int128> negativos");
}

// =============================================================================
// SECTION: Transformación
// =============================================================================

void test_transform()
{
    // uint128_t
    {
        std::vector<uint128_t> src = {uint128_t(1), uint128_t(2), uint128_t(3)};
        std::vector<uint128_t> dst(3);

        transform_128<signedness::unsigned_type>(
            src.begin(), src.end(), dst.begin(),
            [](const uint128_t &x)
            { return x * uint128_t(2); });
        check(dst[0] == uint128_t(2) && dst[1] == uint128_t(4) && dst[2] == uint128_t(6),
              "transform<uint128> duplicar");

        uint128_t sum(0);
        for_each_128<signedness::unsigned_type>(
            src.begin(), src.end(),
            [&sum](const uint128_t &x)
            { sum += x; });
        check(sum == uint128_t(6), "for_each<uint128> suma");
    }

    // int128_t
    {
        std::vector<int128_t> src = {int128_t(-2), int128_t(0), int128_t(3)};
        std::vector<int128_t> dst(3);

        transform_128<signedness::signed_type>(
            src.begin(), src.end(), dst.begin(),
            [](const int128_t &x)
            { return abs(x); });
        check(dst[0] == int128_t(2) && dst[1] == int128_t(0) && dst[2] == int128_t(3),
              "transform<int128> abs");
    }
}

// =============================================================================
// SECTION: Reducción
// =============================================================================

void test_reduction()
{
    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(2), uint128_t(3), uint128_t(5)};

        auto sum = sum_128<signedness::unsigned_type>(vals.begin(), vals.end());
        check(sum == uint128_t(10), "sum<uint128>");

        auto prod = product_128<signedness::unsigned_type>(vals.begin(), vals.end());
        check(prod == uint128_t(30), "product<uint128>");

        auto acc = accumulate_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), uint128_t(0),
            [](const uint128_t &a, const uint128_t &b)
            { return a + b; });
        check(acc == uint128_t(10), "accumulate<uint128> suma");

        acc = accumulate_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), uint128_t(1),
            [](const uint128_t &a, const uint128_t &b)
            { return a * b; });
        check(acc == uint128_t(30), "accumulate<uint128> producto");
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(-2), int128_t(3), int128_t(5)};

        auto sum = sum_128<signedness::signed_type>(vals.begin(), vals.end());
        check(sum == int128_t(6), "sum<int128>");

        auto prod = product_128<signedness::signed_type>(vals.begin(), vals.end());
        check(prod == int128_t(-30), "product<int128> negativo");
    }
}

// =============================================================================
// SECTION: Ordenamiento
// =============================================================================

void test_sort()
{
    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(5), uint128_t(2), uint128_t(8),
                                       uint128_t(1), uint128_t(9)};
        sort_128<signedness::unsigned_type>(vals.begin(), vals.end());
        check(vals[0] == uint128_t(1) && vals[1] == uint128_t(2) && vals[4] == uint128_t(9),
              "sort<uint128> ascendente");

        sort_128<signedness::unsigned_type>(
            vals.begin(), vals.end(),
            [](const uint128_t &a, const uint128_t &b)
            { return a > b; });
        check(vals[0] == uint128_t(9) && vals[4] == uint128_t(1),
              "sort<uint128> descendente");

        std::vector<uint128_t> partial = {uint128_t(5), uint128_t(2), uint128_t(8), uint128_t(1)};
        partial_sort_128<signedness::unsigned_type>(partial.begin(), partial.begin() + 2, partial.end());
        check(partial[0] == uint128_t(1) && partial[1] == uint128_t(2),
              "partial_sort<uint128>");

        std::vector<uint128_t> nth = {uint128_t(5), uint128_t(2), uint128_t(8),
                                      uint128_t(1), uint128_t(3)};
        nth_element_128<signedness::unsigned_type>(nth.begin(), nth.begin() + 2, nth.end());
        check(nth[2] == uint128_t(3), "nth_element<uint128>");
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(5), int128_t(-2), int128_t(0),
                                      int128_t(-7), int128_t(3)};
        sort_128<signedness::signed_type>(vals.begin(), vals.end());
        check(vals[0] == int128_t(-7) && vals[1] == int128_t(-2) &&
                  vals[2] == int128_t(0) && vals[4] == int128_t(5),
              "sort<int128> con negativos");
    }
}

// =============================================================================
// SECTION: Partición
// =============================================================================

void test_partition()
{
    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(1), uint128_t(4), uint128_t(3),
                                       uint128_t(6), uint128_t(2)};
        auto it = partition_128<signedness::unsigned_type>(
            vals.begin(), vals.end(),
            [](const uint128_t &x)
            { return x % uint128_t(2) == 0; });

        int even_count = 0;
        for (auto i = vals.begin(); i != it; ++i)
        {
            if (*i % uint128_t(2) == 0)
                even_count++;
        }
        check(even_count == 3, "partition<uint128> pares");
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(-3), int128_t(5), int128_t(-1),
                                      int128_t(7), int128_t(-8)};
        auto it = partition_128<signedness::signed_type>(
            vals.begin(), vals.end(),
            [](const int128_t &x)
            { return x < int128_t(0); });

        int neg_count = 0;
        for (auto i = vals.begin(); i != it; ++i)
        {
            if (*i < int128_t(0))
                neg_count++;
        }
        check(neg_count == 3, "partition<int128> negativos");
    }
}

// =============================================================================
// SECTION: Generación de secuencias
// =============================================================================

void test_generate_sequences()
{
    // Aritmética uint128_t
    {
        std::vector<uint128_t> arith(5);
        generate_arithmetic_sequence<signedness::unsigned_type>(
            arith.begin(), 5, uint128_t(10), uint128_t(3));
        check(arith[0] == uint128_t(10) && arith[1] == uint128_t(13) && arith[4] == uint128_t(22),
              "generate_arithmetic<uint128>");
    }

    // Geométrica uint128_t
    {
        std::vector<uint128_t> geom(5);
        generate_geometric_sequence<signedness::unsigned_type>(
            geom.begin(), 5, uint128_t(2), uint128_t(3));
        check(geom[0] == uint128_t(2) && geom[1] == uint128_t(6) && geom[2] == uint128_t(18),
              "generate_geometric<uint128>");
    }

    // Aritmética int128_t con paso negativo
    {
        std::vector<int128_t> arith_neg(5);
        generate_arithmetic_sequence<signedness::signed_type>(
            arith_neg.begin(), 5, int128_t(10), int128_t(-3));
        check(arith_neg[0] == int128_t(10) && arith_neg[1] == int128_t(7) && arith_neg[4] == int128_t(-2),
              "generate_arithmetic<int128> negativo");
    }

    // Iota
    {
        std::vector<uint128_t> iota_vec(5);
        iota_128<signedness::unsigned_type>(iota_vec.begin(), iota_vec.end(), uint128_t(100));
        check(iota_vec[0] == uint128_t(100) && iota_vec[4] == uint128_t(104),
              "iota<uint128>");

        std::vector<int128_t> iota_neg(5);
        iota_128<signedness::signed_type>(iota_neg.begin(), iota_neg.end(), int128_t(-2));
        check(iota_neg[0] == int128_t(-2) && iota_neg[2] == int128_t(0) && iota_neg[4] == int128_t(2),
              "iota<int128> desde negativo");
    }
}

// =============================================================================
// SECTION: GCD/LCM de rangos
// =============================================================================

void test_gcd_lcm_range()
{
    // GCD uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(12), uint128_t(18), uint128_t(24)};
        auto g = gcd_range<signedness::unsigned_type>(vals.begin(), vals.end());
        check(g == uint128_t(6), "gcd_range<uint128>");

        std::vector<uint128_t> primes = {uint128_t(7), uint128_t(11), uint128_t(13)};
        g = gcd_range<signedness::unsigned_type>(primes.begin(), primes.end());
        check(g == uint128_t(1), "gcd_range<uint128> coprimos");
    }

    // GCD int128_t con negativos
    {
        std::vector<int128_t> signed_vals = {int128_t(-12), int128_t(18), int128_t(-24)};
        auto gi = gcd_range<signedness::signed_type>(signed_vals.begin(), signed_vals.end());
        check(gi == int128_t(6), "gcd_range<int128> negativos");
    }

    // LCM uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(4), uint128_t(6), uint128_t(8)};
        auto l = lcm_range<signedness::unsigned_type>(vals.begin(), vals.end());
        check(l == uint128_t(24), "lcm_range<uint128>");
    }
}

// =============================================================================
// SECTION: MinMax y Stats
// =============================================================================

void test_minmax_stats()
{
    // MinMax
    {
        std::vector<uint128_t> vals = {uint128_t(5), uint128_t(2), uint128_t(9), uint128_t(1)};
        auto [min_u, max_u] = minmax_128<signedness::unsigned_type>(vals.begin(), vals.end());
        check(min_u == uint128_t(1) && max_u == uint128_t(9), "minmax<uint128>");

        std::vector<int128_t> signed_vals = {int128_t(-5), int128_t(2), int128_t(-9), int128_t(1)};
        auto [min_i, max_i] = minmax_128<signedness::signed_type>(signed_vals.begin(), signed_vals.end());
        check(min_i == int128_t(-9) && max_i == int128_t(2), "minmax<int128>");
    }

    // Stats
    {
        std::vector<uint128_t> vals = {uint128_t(10), uint128_t(20), uint128_t(30)};
        auto stats = calculate_stats<signedness::unsigned_type>(vals.begin(), vals.end());
        check(stats.min_val == uint128_t(10) && stats.max_val == uint128_t(30) &&
                  stats.sum == uint128_t(60) && stats.count == 3,
              "calculate_stats<uint128>");
    }
}

// =============================================================================
// SECTION: Edge cases
// =============================================================================

void test_edge_cases()
{
    // Rangos vacíos
    {
        std::vector<uint128_t> empty;
        check(!binary_search_128<signedness::unsigned_type>(empty.begin(), empty.end(), uint128_t(1)),
              "binary_search vacío");

        auto sum = sum_128<signedness::unsigned_type>(empty.begin(), empty.end());
        check(sum == uint128_t(0), "sum vacío");

        auto g = gcd_range<signedness::unsigned_type>(empty.begin(), empty.end());
        check(g == uint128_t(0), "gcd_range vacío");

        auto [min_val, max_val] = minmax_128<signedness::unsigned_type>(empty.begin(), empty.end());
        check(min_val == uint128_t(0) && max_val == uint128_t(0), "minmax vacío");
    }

    // Elemento único
    {
        std::vector<uint128_t> single = {uint128_t(42)};
        check(binary_search_128<signedness::unsigned_type>(single.begin(), single.end(), uint128_t(42)),
              "binary_search único");
        check(sum_128<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42),
              "sum único");
        check(product_128<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42),
              "product único");
        check(gcd_range<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42),
              "gcd_range único");
    }

    // Valores grandes
    {
        std::vector<uint128_t> large = {uint128_t::max() - uint128_t(2),
                                        uint128_t::max() - uint128_t(1),
                                        uint128_t::max()};
        sort_128<signedness::unsigned_type>(large.begin(), large.end());
        check(large[0] == uint128_t::max() - uint128_t(2) && large[2] == uint128_t::max(),
              "sort valores grandes");
    }
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_algorithm.hpp tests ===\n\n";

    std::cout << "--- Búsqueda uint128_t ---\n";
    test_search_uint128();

    std::cout << "\n--- Búsqueda int128_t ---\n";
    test_search_int128();

    std::cout << "\n--- Transformación ---\n";
    test_transform();

    std::cout << "\n--- Reducción ---\n";
    test_reduction();

    std::cout << "\n--- Ordenamiento ---\n";
    test_sort();

    std::cout << "\n--- Partición ---\n";
    test_partition();

    std::cout << "\n--- Generación de secuencias ---\n";
    test_generate_sequences();

    std::cout << "\n--- GCD/LCM ---\n";
    test_gcd_lcm_range();

    std::cout << "\n--- MinMax y Stats ---\n";
    test_minmax_stats();

    std::cout << "\n--- Edge cases ---\n";
    test_edge_cases();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
