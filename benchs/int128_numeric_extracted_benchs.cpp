/**
 * @file int128_numeric_extracted_benchs.cpp
 * @brief Benchmarks unificados para operaciones numéricas de int128_base_t
 *
 * Testea funciones de <numeric> style:
 * - midpoint, iota
 * - inner_product, reduce
 */

#include "int128_base_numeric.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

using namespace nstd;
using namespace std::chrono;

inline uint64_t rdtsc()
{
#if defined(_MSC_VER)
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
inline uint128_t random_uint128() { return uint128_t(random_u64(), random_u64()); }
inline int128_t random_int128() { return int128_t(static_cast<uint64_t>(static_cast<int64_t>(random_u64())), random_u64()); }

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
        std::cout << "  " << std::setw(20) << std::left << name                                     \
                  << " [" << std::setw(12) << type_name << "]"                                      \
                  << std::fixed << std::setprecision(2)                                             \
                  << std::setw(12) << ns << " ns/op"                                                \
                  << std::setw(14) << cycles << " cycles/op\n";                                     \
    }

constexpr size_t ITERATIONS = 100000;
constexpr size_t VECTOR_SIZE = 1000;

void benchmark_midpoint()
{
    std::cout << "\n=== MIDPOINT ===\n";

    BENCHMARK("midpoint", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        auto result = nstd::midpoint(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("midpoint", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        auto result = nstd::midpoint(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("std::midpoint", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        volatile auto result = std::midpoint(a, b);
        (void)result;
    });
}

void benchmark_iota()
{
    std::cout << "\n=== IOTA ===\n";

    std::vector<uint128_t> vec_u(VECTOR_SIZE);
    BENCHMARK("std::iota", "uint128_t", ITERATIONS / 10, {
        std::iota(vec_u.begin(), vec_u.end(), uint128_t{0});
    });

    std::vector<int128_t> vec_s(VECTOR_SIZE);
    BENCHMARK("std::iota", "int128_t", ITERATIONS / 10, {
        std::iota(vec_s.begin(), vec_s.end(), int128_t{-500});
    });

    std::vector<uint64_t> vec_64(VECTOR_SIZE);
    BENCHMARK("std::iota", "uint64_t", ITERATIONS / 10, {
        std::iota(vec_64.begin(), vec_64.end(), uint64_t{0});
    });
}

void benchmark_inner_product()
{
    std::cout << "\n=== INNER PRODUCT ===\n";

    std::vector<uint128_t> vec1_u(VECTOR_SIZE), vec2_u(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
    {
        vec1_u[i] = uint128_t{i + 1};
        vec2_u[i] = uint128_t{i + 1};
    }

    BENCHMARK("std::inner_product", "uint128_t", ITERATIONS / 10, {
        auto result = std::inner_product(vec1_u.begin(), vec1_u.end(), vec2_u.begin(), uint128_t{0});
        volatile auto sink = result.low();
        (void)sink;
    });

    std::vector<uint64_t> vec1_64(VECTOR_SIZE), vec2_64(VECTOR_SIZE);
    for (size_t i = 0; i < VECTOR_SIZE; ++i)
    {
        vec1_64[i] = i + 1;
        vec2_64[i] = i + 1;
    }

    BENCHMARK("std::inner_product", "uint64_t", ITERATIONS / 10, {
        volatile auto result = std::inner_product(vec1_64.begin(), vec1_64.end(), vec2_64.begin(), uint64_t{0});
        (void)result;
    });
}

int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 Numeric Benchmarks\n";
    std::cout << "========================================\n";

    benchmark_midpoint();
    benchmark_iota();
    benchmark_inner_product();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
