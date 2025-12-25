#include "int128/int128_ranges.hpp"
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// =============================================================================
// Tests para range functions
// =============================================================================

void test_iota()
{
    std::vector<int128_t> vec(5);
    std::iota(vec.begin(), vec.end(), int128_t(10));

    assert(vec[0] == int128_t(10));
    assert(vec[1] == int128_t(11));
    assert(vec[2] == int128_t(12));
    assert(vec[3] == int128_t(13));
    assert(vec[4] == int128_t(14));

    std::cout << "test_iota: passed" << std::endl;
}

void test_iota_negative()
{
    std::vector<int128_t> vec(5);
    std::iota(vec.begin(), vec.end(), int128_t(-2));

    assert(vec[0] == int128_t(-2));
    assert(vec[1] == int128_t(-1));
    assert(vec[2] == int128_t(0));
    assert(vec[3] == int128_t(1));
    assert(vec[4] == int128_t(2));

    std::cout << "test_iota_negative: passed" << std::endl;
}

void test_accumulate()
{
    std::vector<int128_t> vec = {int128_t(1), int128_t(2), int128_t(3), int128_t(4), int128_t(5)};

    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    assert(sum == int128_t(15));

    std::cout << "test_accumulate: passed" << std::endl;
}

void test_accumulate_negative()
{
    std::vector<int128_t> vec = {int128_t(10), int128_t(-5), int128_t(3), int128_t(-2)};

    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    assert(sum == int128_t(6)); // 10 - 5 + 3 - 2 = 6

    std::cout << "test_accumulate_negative: passed" << std::endl;
}

void test_accumulate_with_op()
{
    std::vector<int128_t> vec = {int128_t(2), int128_t(3), int128_t(4)};

    auto product = std::accumulate(vec.begin(), vec.end(), int128_t(1),
                                   [](const int128_t& a, const int128_t& b) { return a * b; });
    assert(product == int128_t(24)); // 2 * 3 * 4 = 24

    std::cout << "test_accumulate_with_op: passed" << std::endl;
}

void test_inner_product()
{
    std::vector<int128_t> vec1 = {int128_t(1), int128_t(2), int128_t(3)};

    std::vector<int128_t> vec2 = {int128_t(4), int128_t(5), int128_t(6)};

    auto result = std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), int128_t(0));
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    assert(result == int128_t(32));

    std::cout << "test_inner_product: passed" << std::endl;
}

void test_partial_sum()
{
    std::vector<int128_t> vec = {int128_t(1), int128_t(2), int128_t(3), int128_t(4)};
    std::vector<int128_t> result(4);

    std::partial_sum(vec.begin(), vec.end(), result.begin());

    assert(result[0] == int128_t(1));  // 1
    assert(result[1] == int128_t(3));  // 1+2
    assert(result[2] == int128_t(6));  // 1+2+3
    assert(result[3] == int128_t(10)); // 1+2+3+4

    std::cout << "test_partial_sum: passed" << std::endl;
}

void test_adjacent_difference()
{
    std::vector<int128_t> vec = {int128_t(10), int128_t(15), int128_t(22), int128_t(30)};
    std::vector<int128_t> result(4);

    std::adjacent_difference(vec.begin(), vec.end(), result.begin());

    assert(result[0] == int128_t(10)); // 10
    assert(result[1] == int128_t(5));  // 15-10
    assert(result[2] == int128_t(7));  // 22-15
    assert(result[3] == int128_t(8));  // 30-22

    std::cout << "test_adjacent_difference: passed" << std::endl;
}

void test_empty_range()
{
    std::vector<int128_t> empty;

    auto sum = std::accumulate(empty.begin(), empty.end(), int128_t(0));
    assert(sum == int128_t(0));

    std::vector<int128_t> result;
    std::partial_sum(empty.begin(), empty.end(), std::back_inserter(result));
    assert(result.empty());

    std::cout << "test_empty_range: passed" << std::endl;
}

void test_large_values()
{
    std::vector<int128_t> vec = {int128_t(0x1000000000000000ULL, 0),
                                 int128_t(0x2000000000000000ULL, 0),
                                 int128_t(0x3000000000000000ULL, 0)};

    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    assert(sum == int128_t(0x6000000000000000ULL, 0));

    std::cout << "test_large_values: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== int128_t ranges tests ===" << std::endl;

    test_iota();
    test_iota_negative();
    test_accumulate();
    test_accumulate_negative();
    test_accumulate_with_op();
    test_inner_product();
    test_partial_sum();
    test_adjacent_difference();
    test_empty_range();
    test_large_values();

    std::cout << "\n✓ All tests passed!" << std::endl;
    return 0;
}
