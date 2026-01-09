/**
 * @file test_algorithm_template.cpp
 * @brief Tests para int128_base_algorithm.hpp
 * @date 2026-01-07
 */

#include "../include_new/int128_base_algorithm.hpp"
#include <iostream>
#include <vector>

using namespace nstd;

int tests_run = 0;
int tests_passed = 0;

#define TEST(name, expr)                                                                           \
    do {                                                                                           \
        ++tests_run;                                                                               \
        if (expr) {                                                                                \
            ++tests_passed;                                                                        \
        } else {                                                                                   \
            std::cerr << "FAIL: " << name << " at line " << __LINE__ << "\n";                      \
        }                                                                                          \
    } while (0)

// =============================================================================
// TEST: Búsqueda
// =============================================================================

void test_search_algorithms()
{
    std::cout << "=== Search Algorithms ===\n";

    // uint128_t tests
    {
        std::vector<uint128_t> sorted = {uint128_t(1), uint128_t(3), uint128_t(5), uint128_t(7),
                                         uint128_t(9)};

        TEST("binary_search_u128_found", binary_search_128<signedness::unsigned_type>(
                                             sorted.begin(), sorted.end(), uint128_t(5)));
        TEST("binary_search_u128_not_found", !binary_search_128<signedness::unsigned_type>(
                                                 sorted.begin(), sorted.end(), uint128_t(4)));

        auto it =
            lower_bound_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(5));
        TEST("lower_bound_u128", it != sorted.end() && *it == uint128_t(5));

        it = upper_bound_128<signedness::unsigned_type>(sorted.begin(), sorted.end(), uint128_t(5));
        TEST("upper_bound_u128", it != sorted.end() && *it == uint128_t(7));

        auto count = count_if_128<signedness::unsigned_type>(
            sorted.begin(), sorted.end(), [](const uint128_t& x) { return x > uint128_t(4); });
        TEST("count_if_u128_greater_4", count == 3);

        it = find_if_128<signedness::unsigned_type>(
            sorted.begin(), sorted.end(), [](const uint128_t& x) { return x == uint128_t(7); });
        TEST("find_if_u128", it != sorted.end() && *it == uint128_t(7));
    }

    // int128_t tests
    {
        std::vector<int128_t> sorted = {int128_t(-5), int128_t(-1), int128_t(0), int128_t(3),
                                        int128_t(7)};

        TEST("binary_search_i128_found",
             binary_search_128<signedness::signed_type>(sorted.begin(), sorted.end(), int128_t(0)));
        TEST("binary_search_i128_negative", binary_search_128<signedness::signed_type>(
                                                sorted.begin(), sorted.end(), int128_t(-5)));
        TEST("binary_search_i128_not_found", !binary_search_128<signedness::signed_type>(
                                                 sorted.begin(), sorted.end(), int128_t(2)));

        auto count = count_if_128<signedness::signed_type>(
            sorted.begin(), sorted.end(), [](const int128_t& x) { return x < int128_t(0); });
        TEST("count_if_i128_negative", count == 2);
    }
}

// =============================================================================
// TEST: Transformación
// =============================================================================

void test_transform_algorithms()
{
    std::cout << "=== Transform Algorithms ===\n";

    // uint128_t
    {
        std::vector<uint128_t> src = {uint128_t(1), uint128_t(2), uint128_t(3)};
        std::vector<uint128_t> dst(3);

        transform_128<signedness::unsigned_type>(
            src.begin(), src.end(), dst.begin(),
            [](const uint128_t& x) { return x * uint128_t(2); });
        TEST("transform_u128_double",
             dst[0] == uint128_t(2) && dst[1] == uint128_t(4) && dst[2] == uint128_t(6));

        uint128_t sum(0);
        for_each_128<signedness::unsigned_type>(src.begin(), src.end(),
                                                [&sum](const uint128_t& x) { sum += x; });
        TEST("for_each_u128_sum", sum == uint128_t(6));
    }

    // int128_t
    {
        std::vector<int128_t> src = {int128_t(-2), int128_t(0), int128_t(3)};
        std::vector<int128_t> dst(3);

        transform_128<signedness::signed_type>(src.begin(), src.end(), dst.begin(),
                                               [](const int128_t& x) { return abs(x); });
        TEST("transform_i128_abs",
             dst[0] == int128_t(2) && dst[1] == int128_t(0) && dst[2] == int128_t(3));
    }
}

// =============================================================================
// TEST: Reducción
// =============================================================================

void test_reduction_algorithms()
{
    std::cout << "=== Reduction Algorithms ===\n";

    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(2), uint128_t(3), uint128_t(5)};

        auto sum = sum_128<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("sum_u128", sum == uint128_t(10));

        auto prod = product_128<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("product_u128", prod == uint128_t(30));

        auto acc = accumulate_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), uint128_t(0),
            [](const uint128_t& a, const uint128_t& b) { return a + b; });
        TEST("accumulate_u128_sum", acc == uint128_t(10));

        acc = accumulate_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), uint128_t(1),
            [](const uint128_t& a, const uint128_t& b) { return a * b; });
        TEST("accumulate_u128_product", acc == uint128_t(30));
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(-2), int128_t(3), int128_t(5)};

        auto sum = sum_128<signedness::signed_type>(vals.begin(), vals.end());
        TEST("sum_i128", sum == int128_t(6));

        auto prod = product_128<signedness::signed_type>(vals.begin(), vals.end());
        TEST("product_i128_negative", prod == int128_t(-30));
    }
}

// =============================================================================
// TEST: Ordenamiento
// =============================================================================

void test_sort_algorithms()
{
    std::cout << "=== Sort Algorithms ===\n";

    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(5), uint128_t(2), uint128_t(8), uint128_t(1),
                                       uint128_t(9)};
        sort_128<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("sort_u128_ascending",
             vals[0] == uint128_t(1) && vals[1] == uint128_t(2) && vals[4] == uint128_t(9));

        sort_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), [](const uint128_t& a, const uint128_t& b) { return a > b; });
        TEST("sort_u128_descending", vals[0] == uint128_t(9) && vals[4] == uint128_t(1));

        std::vector<uint128_t> partial = {uint128_t(5), uint128_t(2), uint128_t(8), uint128_t(1)};
        partial_sort_128<signedness::unsigned_type>(partial.begin(), partial.begin() + 2,
                                                    partial.end());
        TEST("partial_sort_u128", partial[0] == uint128_t(1) && partial[1] == uint128_t(2));

        std::vector<uint128_t> nth = {uint128_t(5), uint128_t(2), uint128_t(8), uint128_t(1),
                                      uint128_t(3)};
        nth_element_128<signedness::unsigned_type>(nth.begin(), nth.begin() + 2, nth.end());
        TEST("nth_element_u128", nth[2] == uint128_t(3)); // Median
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(5), int128_t(-2), int128_t(0), int128_t(-7),
                                      int128_t(3)};
        sort_128<signedness::signed_type>(vals.begin(), vals.end());
        TEST("sort_i128_with_negatives", vals[0] == int128_t(-7) && vals[1] == int128_t(-2) &&
                                             vals[2] == int128_t(0) && vals[4] == int128_t(5));
    }
}

// =============================================================================
// TEST: Partición
// =============================================================================

void test_partition_algorithms()
{
    std::cout << "=== Partition Algorithms ===\n";

    // uint128_t
    {
        std::vector<uint128_t> vals = {uint128_t(1), uint128_t(4), uint128_t(3), uint128_t(6),
                                       uint128_t(2)};
        auto it = partition_128<signedness::unsigned_type>(
            vals.begin(), vals.end(), [](const uint128_t& x) { return x % uint128_t(2) == 0; });

        // Contar pares antes del pivote
        int even_count = 0;
        for (auto i = vals.begin(); i != it; ++i) {
            if (*i % uint128_t(2) == 0)
                even_count++;
        }
        TEST("partition_u128_evens", even_count == 3); // 4, 6, 2
    }

    // int128_t
    {
        std::vector<int128_t> vals = {int128_t(-3), int128_t(5), int128_t(-1), int128_t(7),
                                      int128_t(-8)};
        auto it = partition_128<signedness::signed_type>(
            vals.begin(), vals.end(), [](const int128_t& x) { return x < int128_t(0); });

        int neg_count = 0;
        for (auto i = vals.begin(); i != it; ++i) {
            if (*i < int128_t(0))
                neg_count++;
        }
        TEST("partition_i128_negatives", neg_count == 3);
    }
}

// =============================================================================
// TEST: Algoritmos especializados
// =============================================================================

void test_specialized_algorithms()
{
    std::cout << "=== Specialized Algorithms ===\n";

    // GCD Range
    {
        std::vector<uint128_t> vals = {uint128_t(12), uint128_t(18), uint128_t(24)};
        auto g = gcd_range<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("gcd_range_u128", g == uint128_t(6));

        std::vector<uint128_t> primes = {uint128_t(7), uint128_t(11), uint128_t(13)};
        g = gcd_range<signedness::unsigned_type>(primes.begin(), primes.end());
        TEST("gcd_range_u128_primes", g == uint128_t(1));

        // int128_t with negatives
        std::vector<int128_t> signed_vals = {int128_t(-12), int128_t(18), int128_t(-24)};
        auto gi = gcd_range<signedness::signed_type>(signed_vals.begin(), signed_vals.end());
        TEST("gcd_range_i128_negatives", gi == int128_t(6));
    }

    // LCM Range
    {
        std::vector<uint128_t> vals = {uint128_t(4), uint128_t(6), uint128_t(8)};
        auto l = lcm_range<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("lcm_range_u128", l == uint128_t(24));
    }

    // Generate Sequences
    {
        std::vector<uint128_t> arith(5);
        generate_arithmetic_sequence<signedness::unsigned_type>(arith.begin(), 5, uint128_t(10),
                                                                uint128_t(3));
        TEST("generate_arithmetic_u128",
             arith[0] == uint128_t(10) && arith[1] == uint128_t(13) && arith[4] == uint128_t(22));

        std::vector<uint128_t> geom(5);
        generate_geometric_sequence<signedness::unsigned_type>(geom.begin(), 5, uint128_t(2),
                                                               uint128_t(3));
        TEST("generate_geometric_u128",
             geom[0] == uint128_t(2) && geom[1] == uint128_t(6) && geom[2] == uint128_t(18));

        // int128_t arithmetic with negative step
        std::vector<int128_t> arith_neg(5);
        generate_arithmetic_sequence<signedness::signed_type>(arith_neg.begin(), 5, int128_t(10),
                                                              int128_t(-3));
        TEST("generate_arithmetic_i128_negative", arith_neg[0] == int128_t(10) &&
                                                      arith_neg[1] == int128_t(7) &&
                                                      arith_neg[4] == int128_t(-2));
    }

    // Iota
    {
        std::vector<uint128_t> iota_vec(5);
        iota_128<signedness::unsigned_type>(iota_vec.begin(), iota_vec.end(), uint128_t(100));
        TEST("iota_u128", iota_vec[0] == uint128_t(100) && iota_vec[4] == uint128_t(104));

        std::vector<int128_t> iota_neg(5);
        iota_128<signedness::signed_type>(iota_neg.begin(), iota_neg.end(), int128_t(-2));
        TEST("iota_i128_from_negative", iota_neg[0] == int128_t(-2) && iota_neg[2] == int128_t(0) &&
                                            iota_neg[4] == int128_t(2));
    }

    // MinMax
    {
        std::vector<uint128_t> vals = {uint128_t(5), uint128_t(2), uint128_t(9), uint128_t(1)};
        auto [min_u, max_u] = minmax_128<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("minmax_u128", min_u == uint128_t(1) && max_u == uint128_t(9));

        std::vector<int128_t> signed_vals = {int128_t(-5), int128_t(2), int128_t(-9), int128_t(1)};
        auto [min_i, max_i] =
            minmax_128<signedness::signed_type>(signed_vals.begin(), signed_vals.end());
        TEST("minmax_i128", min_i == int128_t(-9) && max_i == int128_t(2));
    }

    // Stats
    {
        std::vector<uint128_t> vals = {uint128_t(10), uint128_t(20), uint128_t(30)};
        auto stats = calculate_stats<signedness::unsigned_type>(vals.begin(), vals.end());
        TEST("stats_u128", stats.min_val == uint128_t(10) && stats.max_val == uint128_t(30) &&
                               stats.sum == uint128_t(60) && stats.count == 3);
    }
}

// =============================================================================
// TEST: Edge cases
// =============================================================================

void test_edge_cases()
{
    std::cout << "=== Edge Cases ===\n";

    // Empty ranges
    {
        std::vector<uint128_t> empty;
        TEST("binary_search_empty", !binary_search_128<signedness::unsigned_type>(
                                        empty.begin(), empty.end(), uint128_t(1)));

        auto sum = sum_128<signedness::unsigned_type>(empty.begin(), empty.end());
        TEST("sum_empty", sum == uint128_t(0));

        auto g = gcd_range<signedness::unsigned_type>(empty.begin(), empty.end());
        TEST("gcd_empty", g == uint128_t(0));

        auto [min_val, max_val] = minmax_128<signedness::unsigned_type>(empty.begin(), empty.end());
        TEST("minmax_empty", min_val == uint128_t(0) && max_val == uint128_t(0));
    }

    // Single element
    {
        std::vector<uint128_t> single = {uint128_t(42)};
        TEST("binary_search_single", binary_search_128<signedness::unsigned_type>(
                                         single.begin(), single.end(), uint128_t(42)));
        TEST("sum_single",
             sum_128<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42));
        TEST("product_single",
             product_128<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42));
        TEST("gcd_single",
             gcd_range<signedness::unsigned_type>(single.begin(), single.end()) == uint128_t(42));
    }

    // Large values
    {
        std::vector<uint128_t> large = {uint128_t::max() - uint128_t(2),
                                        uint128_t::max() - uint128_t(1), uint128_t::max()};
        sort_128<signedness::unsigned_type>(large.begin(), large.end());
        TEST("sort_large_values",
             large[0] == uint128_t::max() - uint128_t(2) && large[2] == uint128_t::max());
    }
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "Testing int128_base_algorithm.hpp\n";
    std::cout << "==================================\n\n";

    test_search_algorithms();
    test_transform_algorithms();
    test_reduction_algorithms();
    test_sort_algorithms();
    test_partition_algorithms();
    test_specialized_algorithms();
    test_edge_cases();

    std::cout << "\n==================================\n";
    std::cout << "Results: " << tests_passed << "/" << tests_run << " tests passed\n";

    if (tests_passed == tests_run) {
        std::cout << "[SUCCESS] All algorithm tests passed!\n";
        return 0;
    } else {
        std::cout << "[FAILURE] Some tests failed\n";
        return 1;
    }
}
