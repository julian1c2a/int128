/**
 * @file int128_algorithm_extracted_benchs.cpp
 * @brief Benchmarks unificados para algoritmos STL con int128_base_t
 *
 * Compara rendimiento de algoritmos STL entre uint128_t, int128_t y uint64_t
 */

#include "int128_base_algorithm.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

using namespace nstd;
using namespace std::chrono;

inline uint64_t rdtsc()
{
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    return __rdtsc();
#elif defined(__x86_64__) || defined(__i386__)
    uint32_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#else
    return 0;
#endif
}

std::mt19937_64 rng(std::random_device{}());

inline uint64_t random_u64() { return rng(); }
inline uint128_t random_uint128() { return uint128_t(random_u64() % 1000000, random_u64()); }
inline int128_t random_int128() { return int128_t(static_cast<uint64_t>(static_cast<int64_t>(random_u64()) % 1000000), random_u64()); }

#define BENCHMARK(name, type_name, iterations, code)                                                \
    {                                                                                               \
        auto start_time = high_resolution_clock::now();                                             \
        uint64_t start_cycles = rdtsc();                                                            \
        for (size_t _i = 0; _i < iterations; ++_i)                                                  \
        {                                                                                           \
            code;                                                                                   \
        }                                                                                           \
        uint64_t end_cycles = rdtsc();                                                              \
        auto end_time = high_resolution_clock::now();                                               \
        double ns = duration_cast<nanoseconds>(end_time - start_time).count() / double(iterations); \
        double cycles = double(end_cycles - start_cycles) / double(iterations);                     \
        std::cout << "  " << std::setw(25) << std::left << name                                     \
                  << " [" << std::setw(12) << type_name << "]"                                      \
                  << std::fixed << std::setprecision(2)                                             \
                  << std::setw(12) << ns << " ns/op"                                                \
                  << std::setw(14) << cycles << " cycles/op\n";                                     \
    }

constexpr size_t VECTOR_SIZE = 10000;
constexpr size_t ITERATIONS = 500;

void benchmark_binary_search()
{
    std::cout << "\n=== BINARY SEARCH ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = uint128_t(i * 100);
    uint128_t target_u{500000};

    BENCHMARK("std::binary_search", "uint128_t", ITERATIONS, {
        volatile bool found = std::binary_search(vec_u.begin(), vec_u.end(), target_u);
        (void)found;
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = int128_t(static_cast<int64_t>(i) * 100 - 500000);
    std::sort(vec_s.begin(), vec_s.end());
    int128_t target_s{0};

    BENCHMARK("std::binary_search", "int128_t", ITERATIONS, {
        volatile bool found = std::binary_search(vec_s.begin(), vec_s.end(), target_s);
        (void)found;
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = i * 100;
    uint64_t target_64{500000};

    BENCHMARK("std::binary_search", "uint64_t", ITERATIONS, {
        volatile bool found = std::binary_search(vec_64.begin(), vec_64.end(), target_64);
        (void)found;
    });
}

void benchmark_find_if()
{
    std::cout << "\n=== FIND_IF ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = random_uint128();
    auto pred_u = [](const uint128_t &x)
    { return x > uint128_t(500000); };

    BENCHMARK("std::find_if", "uint128_t", ITERATIONS, {
        volatile auto it = std::find_if(vec_u.begin(), vec_u.end(), pred_u);
        (void)it;
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = random_int128();
    auto pred_s = [](const int128_t &x)
    { return x.is_negative(); };

    BENCHMARK("std::find_if", "int128_t", ITERATIONS, {
        volatile auto it = std::find_if(vec_s.begin(), vec_s.end(), pred_s);
        (void)it;
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = random_u64();
    auto pred_64 = [](uint64_t x)
    { return x > 500000; };

    BENCHMARK("std::find_if", "uint64_t", ITERATIONS, {
        volatile auto it = std::find_if(vec_64.begin(), vec_64.end(), pred_64);
        (void)it;
    });
}

void benchmark_transform()
{
    std::cout << "\n=== TRANSFORM ===\n";

    std::vector<uint128_t> input_u(VECTOR_SIZE), output_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        input_u[i] = random_uint128();
    auto doubler_u = [](const uint128_t &x)
    { return x * uint128_t{2}; };

    BENCHMARK("std::transform (x*2)", "uint128_t", ITERATIONS, {
        std::transform(input_u.begin(), input_u.end(), output_u.begin(), doubler_u);
    });

    std::vector<int128_t> input_s(VECTOR_SIZE), output_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        input_s[i] = random_int128();
    auto doubler_s = [](const int128_t &x)
    { return x * int128_t{2}; };

    BENCHMARK("std::transform (x*2)", "int128_t", ITERATIONS, {
        std::transform(input_s.begin(), input_s.end(), output_s.begin(), doubler_s);
    });

    std::vector<uint64_t> input_64(VECTOR_SIZE), output_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        input_64[i] = random_u64();
    auto doubler_64 = [](uint64_t x)
    { return x * 2; };

    BENCHMARK("std::transform (x*2)", "uint64_t", ITERATIONS, {
        std::transform(input_64.begin(), input_64.end(), output_64.begin(), doubler_64);
    });
}

void benchmark_accumulate()
{
    std::cout << "\n=== ACCUMULATE ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = uint128_t(i + 1);

    BENCHMARK("std::accumulate (sum)", "uint128_t", ITERATIONS, {
        auto result = std::accumulate(vec_u.begin(), vec_u.end(), uint128_t{0});
        volatile auto sink = result.low();
        (void)sink;
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = int128_t(static_cast<int64_t>(i) - 5000);

    BENCHMARK("std::accumulate (sum)", "int128_t", ITERATIONS, {
        auto result = std::accumulate(vec_s.begin(), vec_s.end(), int128_t{0});
        volatile auto sink = result.low();
        (void)sink;
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = i + 1;

    BENCHMARK("std::accumulate (sum)", "uint64_t", ITERATIONS, {
        volatile auto result = std::accumulate(vec_64.begin(), vec_64.end(), uint64_t{0});
        (void)result;
    });
}

void benchmark_sort()
{
    std::cout << "\n=== SORT ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = random_uint128();

    BENCHMARK("std::sort", "uint128_t", ITERATIONS / 10, {
        auto copy = vec_u;
        std::sort(copy.begin(), copy.end());
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = random_int128();

    BENCHMARK("std::sort", "int128_t", ITERATIONS / 10, {
        auto copy = vec_s;
        std::sort(copy.begin(), copy.end());
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = random_u64();

    BENCHMARK("std::sort", "uint64_t", ITERATIONS / 10, {
        auto copy = vec_64;
        std::sort(copy.begin(), copy.end());
    });
}

void benchmark_min_max()
{
    std::cout << "\n=== MIN/MAX ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = random_uint128();

    BENCHMARK("std::min_element", "uint128_t", ITERATIONS, {
        volatile auto it = std::min_element(vec_u.begin(), vec_u.end());
        (void)it;
    });

    BENCHMARK("std::max_element", "uint128_t", ITERATIONS, {
        volatile auto it = std::max_element(vec_u.begin(), vec_u.end());
        (void)it;
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = random_int128();

    BENCHMARK("std::min_element", "int128_t", ITERATIONS, {
        volatile auto it = std::min_element(vec_s.begin(), vec_s.end());
        (void)it;
    });

    BENCHMARK("std::max_element", "int128_t", ITERATIONS, {
        volatile auto it = std::max_element(vec_s.begin(), vec_s.end());
        (void)it;
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = random_u64();

    BENCHMARK("std::min_element", "uint64_t", ITERATIONS, {
        volatile auto it = std::min_element(vec_64.begin(), vec_64.end());
        (void)it;
    });

    BENCHMARK("std::max_element", "uint64_t", ITERATIONS, {
        volatile auto it = std::max_element(vec_64.begin(), vec_64.end());
        (void)it;
    });
}

void benchmark_count_if()
{
    std::cout << "\n=== COUNT_IF ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_u[i] = random_uint128();
    auto pred_u = [](const uint128_t &x)
    { return (x.low() & 1) == 0; };

    BENCHMARK("std::count_if (even)", "uint128_t", ITERATIONS, {
        volatile auto count = std::count_if(vec_u.begin(), vec_u.end(), pred_u);
        (void)count;
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_s[i] = random_int128();
    auto pred_s = [](const int128_t &x)
    { return x.is_negative(); };

    BENCHMARK("std::count_if (neg)", "int128_t", ITERATIONS, {
        volatile auto count = std::count_if(vec_s.begin(), vec_s.end(), pred_s);
        (void)count;
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
        vec_64[i] = random_u64();
    auto pred_64 = [](uint64_t x)
    { return (x & 1) == 0; };

    BENCHMARK("std::count_if (even)", "uint64_t", ITERATIONS, {
        volatile auto count = std::count_if(vec_64.begin(), vec_64.end(), pred_64);
        (void)count;
    });
}

int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 Algorithm Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Vector size: " << VECTOR_SIZE << ", Iterations: " << ITERATIONS << "\n";

    benchmark_binary_search();
    benchmark_find_if();
    benchmark_transform();
    benchmark_accumulate();
    benchmark_sort();
    benchmark_min_max();
    benchmark_count_if();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
