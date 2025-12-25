#include "int128/int128_ranges.hpp"
#include <chrono>
#include <iostream>
#include <numeric> // para std::partial_sum, std::adjacent_difference
#include <vector>

using namespace std::chrono;

constexpr int SIZE = 10000;

void benchmark_iota()
{
    std::vector<int128_t> vec(SIZE);

    auto start = high_resolution_clock::now();
    std::iota(vec.begin(), vec.end(), int128_t(0));
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Iota: " << duration << " µs (" << SIZE << " elements)" << std::endl;
}

void benchmark_accumulate()
{
    std::vector<int128_t> vec(SIZE);
    std::iota(vec.begin(), vec.end(), int128_t(1));

    auto start = high_resolution_clock::now();
    auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Accumulate: " << duration << " µs (" << SIZE << " elements)" << std::endl;
}

void benchmark_inner_product()
{
    std::vector<int128_t> vec1(SIZE);
    std::vector<int128_t> vec2(SIZE);
    std::iota(vec1.begin(), vec1.end(), int128_t(1));
    std::iota(vec2.begin(), vec2.end(), int128_t(2));

    auto start = high_resolution_clock::now();
    auto result = std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), int128_t(0));
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Inner product: " << duration << " µs (" << SIZE << " elements)" << std::endl;
}

void benchmark_partial_sum()
{
    std::vector<int128_t> vec(SIZE);
    std::iota(vec.begin(), vec.end(), int128_t(1));
    std::vector<int128_t> result(SIZE);

    auto start = high_resolution_clock::now();
    std::partial_sum(vec.begin(), vec.end(), result.begin());
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Partial sum: " << duration << " µs (" << SIZE << " elements)" << std::endl;
}

void benchmark_adjacent_difference()
{
    std::vector<int128_t> vec(SIZE);
    std::iota(vec.begin(), vec.end(), int128_t(1000));
    std::vector<int128_t> result(SIZE);

    auto start = high_resolution_clock::now();
    std::adjacent_difference(vec.begin(), vec.end(), result.begin());
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Adjacent difference: " << duration << " µs (" << SIZE << " elements)"
              << std::endl;
}

int main()
{
    std::cout << "int128_t ranges benchmarks" << std::endl;
    std::cout << "==========================" << std::endl;

    benchmark_iota();
    benchmark_accumulate();
    benchmark_inner_product();
    benchmark_partial_sum();
    benchmark_adjacent_difference();

    std::cout << "\n✓ Benchmarks completed" << std::endl;
    return 0;
}
