/**
 * @file int128_ranges_extracted_tests.cpp
 * @brief Tests completos de int128_base_ranges.hpp (template unificado)
 *
 * Fusion de tests:
 * - STL numeric algorithms (iota, accumulate, inner_product, partial_sum, adjacent_difference)
 * - int128_ranges namespace functions (generate_*, calculate_stats, find_*, transform, reduce)
 *
 * Total: 30+ tests
 */

#include "int128_base_ranges.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace nstd;
using namespace nstd::int128_ranges;

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
// SECTION: STL std::iota
// =============================================================================

void test_std_iota()
{
    std::vector<int128_t> vec(5);
    std::iota(vec.begin(), vec.end(), int128_t(10));
    check(vec[0] == int128_t(10) && vec[1] == int128_t(11) && vec[4] == int128_t(14),
          "std::iota [10,11,12,13,14]");
}

void test_std_iota_negative()
{
    std::vector<int128_t> vec(5);
    std::iota(vec.begin(), vec.end(), int128_t(-2));
    check(vec[0] == int128_t(-2) && vec[2] == int128_t(0) && vec[4] == int128_t(2),
          "std::iota [-2,-1,0,1,2]");
}

// =============================================================================
// SECTION: STL std::accumulate
// =============================================================================

void test_std_accumulate()
{
    std::vector<int128_t> vec = {int128_t(1), int128_t(2), int128_t(3), int128_t(4), int128_t(5)};
    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    check(sum == int128_t(15), "std::accumulate sum=15");
}

void test_std_accumulate_negative()
{
    std::vector<int128_t> vec = {int128_t(10), int128_t(-5), int128_t(3), int128_t(-2)};
    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    check(sum == int128_t(6), "std::accumulate with negatives sum=6");
}

void test_std_accumulate_with_op()
{
    std::vector<int128_t> vec = {int128_t(2), int128_t(3), int128_t(4)};
    auto product = std::accumulate(vec.begin(), vec.end(), int128_t(1),
                                   [](const int128_t &a, const int128_t &b)
                                   { return a * b; });
    check(product == int128_t(24), "std::accumulate product=24");
}

// =============================================================================
// SECTION: STL std::inner_product
// =============================================================================

void test_std_inner_product()
{
    std::vector<int128_t> vec1 = {int128_t(1), int128_t(2), int128_t(3)};
    std::vector<int128_t> vec2 = {int128_t(4), int128_t(5), int128_t(6)};
    auto result = std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), int128_t(0));
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    check(result == int128_t(32), "std::inner_product=32");
}

// =============================================================================
// SECTION: STL std::partial_sum
// =============================================================================

void test_std_partial_sum()
{
    std::vector<int128_t> vec = {int128_t(1), int128_t(2), int128_t(3), int128_t(4)};
    std::vector<int128_t> result(4);
    std::partial_sum(vec.begin(), vec.end(), result.begin());
    check(result[0] == int128_t(1) && result[1] == int128_t(3) && result[2] == int128_t(6) &&
              result[3] == int128_t(10),
          "std::partial_sum [1,3,6,10]");
}

// =============================================================================
// SECTION: STL std::adjacent_difference
// =============================================================================

void test_std_adjacent_difference()
{
    std::vector<int128_t> vec = {int128_t(10), int128_t(15), int128_t(22), int128_t(30)};
    std::vector<int128_t> result(4);
    std::adjacent_difference(vec.begin(), vec.end(), result.begin());
    check(result[0] == int128_t(10) && result[1] == int128_t(5) && result[2] == int128_t(7) &&
              result[3] == int128_t(8),
          "std::adjacent_difference [10,5,7,8]");
}

// =============================================================================
// SECTION: Empty and large ranges
// =============================================================================

void test_empty_range()
{
    std::vector<int128_t> empty;
    auto sum = std::accumulate(empty.begin(), empty.end(), int128_t(0));
    check(sum == int128_t(0), "empty range accumulate=0");

    std::vector<int128_t> result;
    std::partial_sum(empty.begin(), empty.end(), std::back_inserter(result));
    check(result.empty(), "empty range partial_sum is empty");
}

void test_large_values()
{
    std::vector<int128_t> vec = {int128_t(0x1000000000000000ULL, 0),
                                 int128_t(0x2000000000000000ULL, 0),
                                 int128_t(0x3000000000000000ULL, 0)};
    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    check(sum == int128_t(0x6000000000000000ULL, 0), "large values sum");
}

// =============================================================================
// SECTION: int128_ranges::generate_* functions
// =============================================================================

void test_generate_arithmetic_sequence()
{
    std::array<uint128_t, 5> arr_u{};
    generate_arithmetic_sequence<signedness::unsigned_type>(arr_u.begin(), arr_u.end(),
                                                            uint128_t(10), uint128_t(5));
    check(arr_u[0] == uint128_t(10) && arr_u[1] == uint128_t(15) && arr_u[4] == uint128_t(30),
          "arithmetic_seq unsigned [10,15,20,25,30]");

    std::array<int128_t, 4> arr_s{};
    generate_arithmetic_sequence<signedness::signed_type>(arr_s.begin(), arr_s.end(), int128_t(100),
                                                          int128_t(-25));
    check(arr_s[0] == int128_t(100) && arr_s[1] == int128_t(75) && arr_s[3] == int128_t(25),
          "arithmetic_seq signed [100,75,50,25]");
}

void test_generate_geometric_sequence()
{
    std::array<uint128_t, 5> arr{};
    generate_geometric_sequence<signedness::unsigned_type>(arr.begin(), arr.end(), uint128_t(1),
                                                           uint128_t(3));
    check(arr[0] == uint128_t(1) && arr[1] == uint128_t(3) && arr[2] == uint128_t(9) &&
              arr[3] == uint128_t(27) && arr[4] == uint128_t(81),
          "geometric_seq [1,3,9,27,81]");
}

void test_generate_powers_of_2()
{
    std::array<uint128_t, 4> arr{};
    generate_powers_of_2<signedness::unsigned_type>(arr.begin(), arr.end(), 0);
    check(arr[0] == uint128_t(1) && arr[1] == uint128_t(2) && arr[2] == uint128_t(4) &&
              arr[3] == uint128_t(8),
          "powers_of_2 [1,2,4,8]");

    std::array<uint128_t, 3> arr2{};
    generate_powers_of_2<signedness::unsigned_type>(arr2.begin(), arr2.end(), 10);
    check(arr2[0] == uint128_t(1024) && arr2[1] == uint128_t(2048) && arr2[2] == uint128_t(4096),
          "powers_of_2 from 2^10 [1024,2048,4096]");
}

void test_ranges_iota()
{
    std::array<uint128_t, 5> arr{};
    iota<signedness::unsigned_type>(arr.begin(), arr.end(), uint128_t(100));
    check(arr[0] == uint128_t(100) && arr[1] == uint128_t(101) && arr[4] == uint128_t(104),
          "int128_ranges::iota [100,101,102,103,104]");
}

// =============================================================================
// SECTION: int128_ranges::calculate_stats
// =============================================================================

void test_calculate_stats()
{
    std::array<uint128_t, 5> arr = {uint128_t(10), uint128_t(20), uint128_t(5), uint128_t(15),
                                    uint128_t(30)};
    auto stats = calculate_stats<signedness::unsigned_type>(arr.begin(), arr.end());

    check(stats.valid, "stats.valid");
    check(stats.count == 5, "stats.count==5");
    check(stats.sum == uint128_t(80), "stats.sum==80");
    check(stats.min_val == uint128_t(5), "stats.min_val==5");
    check(stats.max_val == uint128_t(30), "stats.max_val==30");
    check(stats.average() == uint128_t(16), "stats.average()==16");
    check(stats.range() == uint128_t(25), "stats.range()==25");

    std::array<uint128_t, 0> empty{};
    auto empty_stats = calculate_stats<signedness::unsigned_type>(empty.begin(), empty.end());
    check(!empty_stats.valid, "empty stats.valid==false");
}

// =============================================================================
// SECTION: int128_ranges::find_first_if / count_if
// =============================================================================

void test_find_first_if()
{
    std::array<uint128_t, 5> arr = {uint128_t(10), uint128_t(25), uint128_t(30), uint128_t(15),
                                    uint128_t(40)};

    auto found = find_first_if<signedness::unsigned_type>(
        arr.begin(), arr.end(), [](const uint128_t &v)
        { return v > uint128_t(20); });
    check(found.has_value() && *found == uint128_t(25), "find_first_if (>20) found 25");

    auto not_found = find_first_if<signedness::unsigned_type>(
        arr.begin(), arr.end(), [](const uint128_t &v)
        { return v > uint128_t(100); });
    check(!not_found.has_value(), "find_first_if (>100) not found");
}

void test_count_if()
{
    std::array<int128_t, 6> arr = {int128_t(-5), int128_t(10), int128_t(-3),
                                   int128_t(7), int128_t(-1), int128_t(20)};

    auto neg_count = count_if<signedness::signed_type>(
        arr.begin(), arr.end(), [](const int128_t &v)
        { return v.is_negative(); });
    check(neg_count == 3, "count_if (negative)==3");

    auto pos_count = count_if<signedness::signed_type>(
        arr.begin(), arr.end(), [](const int128_t &v)
        { return !v.is_negative(); });
    check(pos_count == 3, "count_if (non-negative)==3");
}

// =============================================================================
// SECTION: int128_ranges::transform / copy_if
// =============================================================================

void test_transform()
{
    std::array<uint128_t, 3> src = {uint128_t(2), uint128_t(3), uint128_t(4)};
    std::array<uint128_t, 3> dst{};

    transform<signedness::unsigned_type>(src.begin(), src.end(), dst.begin(),
                                         [](const uint128_t &v)
                                         { return v * v; });
    check(dst[0] == uint128_t(4) && dst[1] == uint128_t(9) && dst[2] == uint128_t(16),
          "transform (square) [4,9,16]");
}

void test_copy_if()
{
    std::array<int128_t, 5> src = {int128_t(1), int128_t(-2), int128_t(3), int128_t(-4),
                                   int128_t(5)};
    std::vector<int128_t> dst;
    dst.reserve(5);

    copy_if<signedness::signed_type>(src.begin(), src.end(), std::back_inserter(dst),
                                     [](const int128_t &v)
                                     { return !v.is_negative(); });
    check(dst.size() == 3 && dst[0] == int128_t(1) && dst[1] == int128_t(3) && dst[2] == int128_t(5),
          "copy_if (positive only) [1,3,5]");
}

// =============================================================================
// SECTION: int128_ranges::sum / product / reduce
// =============================================================================

void test_sum()
{
    std::array<uint128_t, 4> arr = {uint128_t(10), uint128_t(20), uint128_t(30), uint128_t(40)};
    auto total = sum<signedness::unsigned_type>(arr.begin(), arr.end());
    check(total == uint128_t(100), "sum [10,20,30,40]==100");
}

void test_product()
{
    std::array<uint128_t, 4> arr = {uint128_t(2), uint128_t(3), uint128_t(4), uint128_t(5)};
    auto total = product<signedness::unsigned_type>(arr.begin(), arr.end());
    check(total == uint128_t(120), "product [2,3,4,5]==120");
}

void test_reduce()
{
    std::array<int128_t, 3> arr = {int128_t(5), int128_t(3), int128_t(2)};
    auto result =
        reduce<signedness::signed_type>(arr.begin(), arr.end(), int128_t(1),
                                        [](const int128_t &a, const int128_t &b)
                                        { return a * b; });
    check(result == int128_t(30), "reduce (multiply) [5,3,2]==30");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_ranges.hpp tests ===\n\n";

    std::cout << "--- STL algorithms ---\n";
    test_std_iota();
    test_std_iota_negative();
    test_std_accumulate();
    test_std_accumulate_negative();
    test_std_accumulate_with_op();
    test_std_inner_product();
    test_std_partial_sum();
    test_std_adjacent_difference();
    test_empty_range();
    test_large_values();

    std::cout << "\n--- int128_ranges generators ---\n";
    test_generate_arithmetic_sequence();
    test_generate_geometric_sequence();
    test_generate_powers_of_2();
    test_ranges_iota();

    std::cout << "\n--- int128_ranges stats ---\n";
    test_calculate_stats();

    std::cout << "\n--- int128_ranges search ---\n";
    test_find_first_if();
    test_count_if();

    std::cout << "\n--- int128_ranges transformations ---\n";
    test_transform();
    test_copy_if();

    std::cout << "\n--- int128_ranges reductions ---\n";
    test_sum();
    test_product();
    test_reduce();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
