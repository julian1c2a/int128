/**
 * @file uint128_algorithm_benchmarks.cpp
 * @brief Benchmarks para funciones de uint128_algorithm.hpp
 */

#include "int128_base_algorithm.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace nstd;
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

using namespace nstd;
using namespace std::chrono;

// Función para leer ciclos de CPU (rdtsc)
inline uint64_t rdtsc()
{
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    return __rdtsc();
#elif defined(__x86_64__) || defined(__i386__)
    uint32_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#else
    return 0; // Fallback para arquitecturas no soportadas
#endif
}

// Generador de numeros aleatorios
std::mt19937_64 rng(std::random_device{}());

// Generador de valores uint128_t aleatorios
uint128_t random_uint128(uint64_t max_high = 1000000, uint64_t max_low = UINT64_MAX)
{
    uint64_t high = std::uniform_int_distribution<uint64_t>(0, max_high)(rng);
    uint64_t low = std::uniform_int_distribution<uint64_t>(0, max_low)(rng);
    return uint128_t(high, low);
}

// Macro para benchmark con tiempo y ciclos de CPU
#define BENCHMARK(name, iterations, code)                                                          \
    {                                                                                              \
        auto start_time = high_resolution_clock::now();                                            \
        uint64_t start_cycles = rdtsc();                                                           \
        for (size_t i = 0; i < iterations; ++i) {                                                  \
            code;                                                                                  \
        }                                                                                          \
        uint64_t end_cycles = rdtsc();                                                             \
        auto end_time = high_resolution_clock::now();                                              \
        auto duration = duration_cast<microseconds>(end_time - start_time).count();                \
        double avg_us = static_cast<double>(duration) / iterations;                                \
        uint64_t total_cycles = end_cycles - start_cycles;                                         \
        double avg_cycles = static_cast<double>(total_cycles) / iterations;                        \
        std::cout << "  " << name << ": " << avg_us << " us/op, " << avg_cycles << " cycles/op ("  \
                  << iterations << " ops)\n";                                                      \
    }

// ===============================================================================
// BENCHMARKS DE BUSQUEDA
// ===============================================================================

void benchmark_binary_search()
{
    std::cout << "\n[Benchmark] binary_search_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t(i * 100));
    }

    uint128_t target = uint128_t(500000);

    BENCHMARK("binary_search (sorted)", ITERATIONS, {
        volatile bool found = binary_search_uint128(vec.begin(), vec.end(), target);
        (void)found;
    });
}

void benchmark_find_if()
{
    std::cout << "\n[Benchmark] find_if_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_uint128(1000));
    }

    auto predicate = [](const uint128_t& x) { return x > uint128_t(500000); };

    BENCHMARK("find_if (predicate)", ITERATIONS, {
        volatile auto it = find_if_uint128(vec.begin(), vec.end(), predicate);
        (void)it;
    });
}

// ===============================================================================
// BENCHMARKS DE TRANSFORMACION
// ===============================================================================

void benchmark_transform()
{
    std::cout << "\n[Benchmark] transform_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> input;
    std::vector<uint128_t> output(SIZE);
    input.reserve(SIZE);

    for (size_t i = 0; i < SIZE; ++i) {
        input.push_back(random_uint128(1000));
    }

    auto doubler = [](const uint128_t& x) { return x * uint128_t(2); };

    BENCHMARK("transform (double)", ITERATIONS,
              { transform_uint128(input.begin(), input.end(), output.begin(), doubler); });
}

void benchmark_for_each()
{
    std::cout << "\n[Benchmark] for_each_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_uint128(1000));
    }

    uint128_t sum = 0;
    auto accumulator = [&sum](const uint128_t& x) { sum += x; };

    BENCHMARK("for_each (accumulate)", ITERATIONS, {
        sum = 0;
        for_each_uint128(vec.begin(), vec.end(), accumulator);
    });
}

// ===============================================================================
// BENCHMARKS DE REDUCCION
// ===============================================================================

void benchmark_accumulate()
{
    std::cout << "\n[Benchmark] accumulate_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t(i + 1));
    }

    BENCHMARK("accumulate (sum)", ITERATIONS, {
        volatile auto result =
            accumulate_uint128(vec.begin(), vec.end(), uint128_t(0), std::plus<uint128_t>());
        (void)result;
    });
}

void benchmark_sum()
{
    std::cout << "\n[Benchmark] sum_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t(i + 1));
    }

    BENCHMARK("sum", ITERATIONS, {
        volatile auto result = sum_uint128(vec.begin(), vec.end());
        (void)result;
    });
}

void benchmark_product()
{
    std::cout << "\n[Benchmark] product_uint128\n";

    const size_t SIZE = 100;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t(i + 1));
    }

    BENCHMARK("product", ITERATIONS, {
        volatile auto result = product_uint128(vec.begin(), vec.end());
        (void)result;
    });
}

// ===============================================================================
// BENCHMARKS DE PARTICION Y ORDENAMIENTO
// ===============================================================================

void benchmark_partition()
{
    std::cout << "\n[Benchmark] partition_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 100;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);

    auto predicate = [](const uint128_t& x) { return (x % uint128_t(2)) == uint128_t(0); };

    BENCHMARK("partition (even/odd)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_uint128(1000));
        }
        partition_uint128(vec.begin(), vec.end(), predicate);
    });
}

void benchmark_sort()
{
    std::cout << "\n[Benchmark] sort_uint128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 50;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);

    BENCHMARK("sort (ascending)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_uint128(1000000));
        }
        sort_uint128(vec.begin(), vec.end());
    });

    BENCHMARK("sort (descending)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_uint128(1000000));
        }
        sort_uint128(vec.begin(), vec.end(), std::greater<uint128_t>());
    });
}

// ===============================================================================
// BENCHMARKS DE GCD/LCM
// ===============================================================================

void benchmark_gcd_range()
{
    std::cout << "\n[Benchmark] gcd_range\n";

    const size_t SIZE = 100;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t((i + 1) * 123456));
    }

    BENCHMARK("gcd_range", ITERATIONS, {
        volatile auto result = gcd_range(vec.begin(), vec.end());
        (void)result;
    });
}

void benchmark_lcm_range()
{
    std::cout << "\n[Benchmark] lcm_range\n";

    const size_t SIZE = 20;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(uint128_t(i + 2));
    }

    BENCHMARK("lcm_range", ITERATIONS, {
        volatile auto result = lcm_range(vec.begin(), vec.end());
        (void)result;
    });
}

// ===============================================================================
// BENCHMARKS DE GENERACION DE SECUENCIAS
// ===============================================================================

void benchmark_generate_arithmetic()
{
    std::cout << "\n[Benchmark] generate_arithmetic_sequence\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> seq(SIZE);

    BENCHMARK("generate_arithmetic", ITERATIONS,
              { generate_arithmetic_sequence(seq.begin(), SIZE, uint128_t(10), uint128_t(5)); });
}

void benchmark_generate_geometric()
{
    std::cout << "\n[Benchmark] generate_geometric_sequence\n";

    const size_t SIZE = 50;
    const size_t ITERATIONS = 1000;

    std::vector<uint128_t> seq(SIZE);

    BENCHMARK("generate_geometric", ITERATIONS,
              { generate_geometric_sequence(seq.begin(), SIZE, uint128_t(2), uint128_t(2)); });
}

// ===============================================================================
// BENCHMARKS DE ESTADISTICAS
// ===============================================================================

void benchmark_calculate_stats()
{
    std::cout << "\n[Benchmark] calculate_stats\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<uint128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_uint128(1000000));
    }

    BENCHMARK("calculate_stats", ITERATIONS, {
        volatile auto stats = calculate_stats(vec.begin(), vec.end());
        (void)stats;
    });
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  uint128_algorithm.hpp Benchmarks\n";
    std::cout << "========================================\n";

    benchmark_binary_search();
    benchmark_find_if();
    benchmark_transform();
    benchmark_for_each();
    benchmark_accumulate();
    benchmark_sum();
    benchmark_product();
    benchmark_partition();
    benchmark_sort();
    benchmark_gcd_range();
    benchmark_lcm_range();
    benchmark_generate_arithmetic();
    benchmark_generate_geometric();
    benchmark_calculate_stats();

    std::cout << "\n========================================\n";
    std::cout << "  [OK] Benchmarks completados\n";
    std::cout << "========================================\n";

    return 0;
}
