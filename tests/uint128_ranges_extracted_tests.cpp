#include "uint128/uint128_ranges.hpp"
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// =============================================================================
// Tests para range functions
// =============================================================================

void test_iota()
{
    std::vector<uint128_t> vec(5);
    std::iota(vec.begin(), vec.end(), uint128_t(0, 10));

    assert(vec[0] == uint128_t(0, 10));
    assert(vec[1] == uint128_t(0, 11));
    assert(vec[2] == uint128_t(0, 12));
    assert(vec[3] == uint128_t(0, 13));
    assert(vec[4] == uint128_t(0, 14));

    std::cout << "test_iota: passed" << std::endl;
}

void test_accumulate()
{
    std::vector<uint128_t> vec = {uint128_t(0, 1), uint128_t(0, 2), uint128_t(0, 3),
                                  uint128_t(0, 4), uint128_t(0, 5)};

    auto sum = std::accumulate(vec.begin(), vec.end(), uint128_t(0, 0));
    assert(sum == uint128_t(0, 15));

    std::cout << "test_accumulate: passed" << std::endl;
}

void test_accumulate_with_op()
{
    std::vector<uint128_t> vec = {uint128_t(0, 2), uint128_t(0, 3), uint128_t(0, 4)};

    auto product = std::accumulate(vec.begin(), vec.end(), uint128_t(0, 1),
                                   [](const uint128_t& a, const uint128_t& b) { return a * b; });
    assert(product == uint128_t(0, 24)); // 2 * 3 * 4 = 24

    std::cout << "test_accumulate_with_op: passed" << std::endl;
}

void test_inner_product()
{
    std::vector<uint128_t> vec1 = {uint128_t(0, 1), uint128_t(0, 2), uint128_t(0, 3)};

    std::vector<uint128_t> vec2 = {uint128_t(0, 4), uint128_t(0, 5), uint128_t(0, 6)};

    auto result = std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), uint128_t(0, 0));
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    assert(result == uint128_t(0, 32));

    std::cout << "test_inner_product: passed" << std::endl;
}

void test_partial_sum()
{
    std::vector<uint128_t> vec = {uint128_t(0, 1), uint128_t(0, 2), uint128_t(0, 3),
                                  uint128_t(0, 4)};
    std::vector<uint128_t> result(4);

    std::partial_sum(vec.begin(), vec.end(), result.begin());

    assert(result[0] == uint128_t(0, 1));  // 1
    assert(result[1] == uint128_t(0, 3));  // 1+2
    assert(result[2] == uint128_t(0, 6));  // 1+2+3
    assert(result[3] == uint128_t(0, 10)); // 1+2+3+4

    std::cout << "test_partial_sum: passed" << std::endl;
}

void test_adjacent_difference()
{
    std::vector<uint128_t> vec = {uint128_t(0, 10), uint128_t(0, 15), uint128_t(0, 22),
                                  uint128_t(0, 30)};
    std::vector<uint128_t> result(4);

    std::adjacent_difference(vec.begin(), vec.end(), result.begin());

    assert(result[0] == uint128_t(0, 10)); // 10
    assert(result[1] == uint128_t(0, 5));  // 15-10
    assert(result[2] == uint128_t(0, 7));  // 22-15
    assert(result[3] == uint128_t(0, 8));  // 30-22

    std::cout << "test_adjacent_difference: passed" << std::endl;
}

void test_empty_range()
{
    std::vector<uint128_t> empty;

    auto sum = std::accumulate(empty.begin(), empty.end(), uint128_t(0, 0));
    assert(sum == uint128_t(0, 0));

    std::vector<uint128_t> result;
    std::partial_sum(empty.begin(), empty.end(), std::back_inserter(result));
    assert(result.empty());

    std::cout << "test_empty_range: passed" << std::endl;
}

void test_large_values()
{
    std::vector<uint128_t> vec = {uint128_t(0x1000000000000000ULL, 0),
                                  uint128_t(0x2000000000000000ULL, 0),
                                  uint128_t(0x3000000000000000ULL, 0)};

    auto sum = std::accumulate(vec.begin(), vec.end(), uint128_t(0, 0));
    assert(sum == uint128_t(0x6000000000000000ULL, 0));

    std::cout << "test_large_values: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== uint128_t ranges tests ===" << std::endl;

    test_iota();
    test_accumulate();
    test_accumulate_with_op();
    test_inner_product();
    test_partial_sum();
    test_adjacent_difference();
    test_empty_range();
    test_large_values();

    std::cout << "\n[OK] All tests passed!" << std::endl;
    return 0;
}
