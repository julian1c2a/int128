/**
 * @file test_ranges_template.cpp
 * @brief Tests para int128_base_ranges.hpp
 *
 * Tests: 20 total
 * - Generadores de secuencias
 * - Estadísticas de rangos
 * - Funciones de búsqueda
 * - Transformaciones y reducciones
 */

#include "../include_new/int128_base_ranges.hpp"
#include <array>
#include <iostream>
#include <vector>

using namespace nstd;
using namespace nstd::int128_ranges;

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)                                                                      \
    do {                                                                                           \
        if (condition) {                                                                           \
            std::cout << "[PASS] " << name << std::endl;                                           \
            tests_passed++;                                                                        \
        } else {                                                                                   \
            std::cout << "[FAIL] " << name << std::endl;                                           \
            tests_failed++;                                                                        \
        }                                                                                          \
    } while (0)

// =============================================================================
// Test: Generadores de secuencias
// =============================================================================

void test_generate_arithmetic_sequence()
{
    // Unsigned
    std::array<uint128_t, 5> arr_u{};
    generate_arithmetic_sequence<signedness::unsigned_type>(arr_u.begin(), arr_u.end(),
                                                            uint128_t(10), uint128_t(5));
    TEST("arithmetic_seq unsigned [10,15,20,25,30]",
         arr_u[0] == uint128_t(10) && arr_u[1] == uint128_t(15) && arr_u[2] == uint128_t(20) &&
             arr_u[3] == uint128_t(25) && arr_u[4] == uint128_t(30));

    // Signed con step negativo
    std::array<int128_t, 4> arr_s{};
    generate_arithmetic_sequence<signedness::signed_type>(arr_s.begin(), arr_s.end(), int128_t(100),
                                                          int128_t(-25));
    TEST("arithmetic_seq signed [100,75,50,25]",
         arr_s[0] == int128_t(100) && arr_s[1] == int128_t(75) && arr_s[2] == int128_t(50) &&
             arr_s[3] == int128_t(25));
}

void test_iota()
{
    std::array<uint128_t, 5> arr{};
    iota<signedness::unsigned_type>(arr.begin(), arr.end(), uint128_t(100));
    TEST("iota unsigned [100,101,102,103,104]",
         arr[0] == uint128_t(100) && arr[1] == uint128_t(101) && arr[4] == uint128_t(104));

    std::array<int128_t, 3> arr_s{};
    iota<signedness::signed_type>(arr_s.begin(), arr_s.end(), int128_t(-1));
    TEST("iota signed [-1,0,1]",
         arr_s[0] == int128_t(-1) && arr_s[1] == int128_t(0) && arr_s[2] == int128_t(1));
}

void test_generate_geometric_sequence()
{
    std::array<uint128_t, 5> arr{};
    generate_geometric_sequence<signedness::unsigned_type>(arr.begin(), arr.end(), uint128_t(1),
                                                           uint128_t(3));
    TEST("geometric_seq unsigned [1,3,9,27,81]",
         arr[0] == uint128_t(1) && arr[1] == uint128_t(3) && arr[2] == uint128_t(9) &&
             arr[3] == uint128_t(27) && arr[4] == uint128_t(81));
}

void test_generate_powers_of_2()
{
    std::array<uint128_t, 4> arr{};
    generate_powers_of_2<signedness::unsigned_type>(arr.begin(), arr.end(), 0);
    TEST("powers_of_2 [1,2,4,8]", arr[0] == uint128_t(1) && arr[1] == uint128_t(2) &&
                                      arr[2] == uint128_t(4) && arr[3] == uint128_t(8));

    std::array<uint128_t, 3> arr2{};
    generate_powers_of_2<signedness::unsigned_type>(arr2.begin(), arr2.end(), 10);
    TEST("powers_of_2 from 2^10 [1024,2048,4096]",
         arr2[0] == uint128_t(1024) && arr2[1] == uint128_t(2048) && arr2[2] == uint128_t(4096));
}

// =============================================================================
// Test: Estadísticas de rangos
// =============================================================================

void test_calculate_stats()
{
    std::array<uint128_t, 5> arr = {uint128_t(10), uint128_t(20), uint128_t(5), uint128_t(15),
                                    uint128_t(30)};
    auto stats = calculate_stats<signedness::unsigned_type>(arr.begin(), arr.end());

    TEST("stats.valid", stats.valid);
    TEST("stats.count == 5", stats.count == 5);
    TEST("stats.sum == 80", stats.sum == uint128_t(80));
    TEST("stats.min_val == 5", stats.min_val == uint128_t(5));
    TEST("stats.max_val == 30", stats.max_val == uint128_t(30));
    TEST("stats.average() == 16", stats.average() == uint128_t(16));
    TEST("stats.range() == 25", stats.range() == uint128_t(25));

    // Empty range
    std::array<uint128_t, 0> empty{};
    auto empty_stats = calculate_stats<signedness::unsigned_type>(empty.begin(), empty.end());
    TEST("empty stats.valid == false", !empty_stats.valid);
}

// =============================================================================
// Test: Funciones de búsqueda
// =============================================================================

void test_find_first_if()
{
    std::array<uint128_t, 5> arr = {uint128_t(10), uint128_t(25), uint128_t(30), uint128_t(15),
                                    uint128_t(40)};

    auto found = find_first_if<signedness::unsigned_type>(
        arr.begin(), arr.end(), [](const uint128_t& v) { return v > uint128_t(20); });
    TEST("find_first_if (>20) found 25", found.has_value() && *found == uint128_t(25));

    auto not_found = find_first_if<signedness::unsigned_type>(
        arr.begin(), arr.end(), [](const uint128_t& v) { return v > uint128_t(100); });
    TEST("find_first_if (>100) not found", !not_found.has_value());
}

void test_count_if()
{
    std::array<int128_t, 6> arr = {int128_t(-5), int128_t(10), int128_t(-3),
                                   int128_t(7),  int128_t(-1), int128_t(20)};

    auto neg_count = count_if<signedness::signed_type>(
        arr.begin(), arr.end(), [](const int128_t& v) { return v.is_negative(); });
    TEST("count_if (negative) == 3", neg_count == 3);

    auto pos_count = count_if<signedness::signed_type>(
        arr.begin(), arr.end(), [](const int128_t& v) { return !v.is_negative(); });
    TEST("count_if (non-negative) == 3", pos_count == 3);
}

// =============================================================================
// Test: Transformaciones
// =============================================================================

void test_transform()
{
    std::array<uint128_t, 3> src = {uint128_t(2), uint128_t(3), uint128_t(4)};
    std::array<uint128_t, 3> dst{};

    transform<signedness::unsigned_type>(src.begin(), src.end(), dst.begin(),
                                         [](const uint128_t& v) { return v * v; });
    TEST("transform (square) [4,9,16]",
         dst[0] == uint128_t(4) && dst[1] == uint128_t(9) && dst[2] == uint128_t(16));
}

void test_copy_if()
{
    std::array<int128_t, 5> src = {int128_t(1), int128_t(-2), int128_t(3), int128_t(-4),
                                   int128_t(5)};
    std::vector<int128_t> dst;
    dst.reserve(5);

    copy_if<signedness::signed_type>(src.begin(), src.end(), std::back_inserter(dst),
                                     [](const int128_t& v) { return !v.is_negative(); });
    TEST("copy_if (positive only) size==3", dst.size() == 3);
    TEST("copy_if values [1,3,5]",
         dst[0] == int128_t(1) && dst[1] == int128_t(3) && dst[2] == int128_t(5));
}

// =============================================================================
// Test: Reducciones
// =============================================================================

void test_sum()
{
    std::array<uint128_t, 4> arr = {uint128_t(10), uint128_t(20), uint128_t(30), uint128_t(40)};
    auto total = sum<signedness::unsigned_type>(arr.begin(), arr.end());
    TEST("sum [10,20,30,40] == 100", total == uint128_t(100));
}

void test_product()
{
    std::array<uint128_t, 4> arr = {uint128_t(2), uint128_t(3), uint128_t(4), uint128_t(5)};
    auto total = product<signedness::unsigned_type>(arr.begin(), arr.end());
    TEST("product [2,3,4,5] == 120", total == uint128_t(120));
}

void test_reduce()
{
    std::array<int128_t, 3> arr = {int128_t(5), int128_t(3), int128_t(2)};
    // Multiplicación acumulativa
    auto result =
        reduce<signedness::signed_type>(arr.begin(), arr.end(), int128_t(1),
                                        [](const int128_t& a, const int128_t& b) { return a * b; });
    TEST("reduce (multiply) [5,3,2] == 30", result == int128_t(30));
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== Tests: int128_base_ranges.hpp ===" << std::endl << std::endl;

    // Generadores
    test_generate_arithmetic_sequence();
    test_iota();
    test_generate_geometric_sequence();
    test_generate_powers_of_2();

    // Estadísticas
    test_calculate_stats();

    // Búsqueda
    test_find_first_if();
    test_count_if();

    // Transformaciones
    test_transform();
    test_copy_if();

    // Reducciones
    test_sum();
    test_product();
    test_reduce();

    std::cout << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Passed: " << tests_passed << "/" << (tests_passed + tests_failed) << std::endl;
    std::cout << "==================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
