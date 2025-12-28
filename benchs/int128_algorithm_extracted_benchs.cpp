/**
 * @file int128_algorithm_benchmarks.cpp
 * @brief Benchmarks para funciones de int128_algorithm.hpp
 */

#include "int128/int128_algorithm.hpp"
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

using namespace int128_algorithm;
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

// Generador de valores int128_t aleatorios
int128_t random_int128(int64_t min_val = INT64_MIN / 1000, int64_t max_val = INT64_MAX / 1000)
{
    int64_t value = std::uniform_int_distribution<int64_t>(min_val, max_val)(rng);
    return int128_t(value);
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
    std::cout << "\n[Benchmark] binary_search_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t(i * 100 - 50000));
    }

    int128_t target = int128_t(25000);

    BENCHMARK("binary_search (sorted)", ITERATIONS, {
        volatile bool found = binary_search_int128(vec.begin(), vec.end(), target);
        (void)found;
    });
}

void benchmark_find_if()
{
    std::cout << "\n[Benchmark] find_if_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_int128());
    }

    auto predicate = [](const int128_t& x) { return x > int128_t(50000); };

    BENCHMARK("find_if (predicate)", ITERATIONS, {
        volatile auto it = find_if_int128(vec.begin(), vec.end(), predicate);
        (void)it;
    });
}

// ===============================================================================
// BENCHMARKS DE TRANSFORMACION
// ===============================================================================

void benchmark_transform()
{
    std::cout << "\n[Benchmark] transform_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<int128_t> input;
    std::vector<int128_t> output(SIZE);
    input.reserve(SIZE);

    for (size_t i = 0; i < SIZE; ++i) {
        input.push_back(random_int128());
    }

    auto doubler = [](const int128_t& x) { return x * int128_t(2); };

    BENCHMARK("transform (double)", ITERATIONS,
              { transform_int128(input.begin(), input.end(), output.begin(), doubler); });
}

void benchmark_for_each()
{
    std::cout << "\n[Benchmark] for_each_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_int128());
    }

    int128_t sum = 0;
    auto accumulator = [&sum](const int128_t& x) { sum += x; };

    BENCHMARK("for_each (accumulate)", ITERATIONS, {
        sum = 0;
        for_each_int128(vec.begin(), vec.end(), accumulator);
    });
}

// ===============================================================================
// BENCHMARKS DE REDUCCION
// ===============================================================================

void benchmark_accumulate()
{
    std::cout << "\n[Benchmark] accumulate_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t(i - 5000));
    }

    BENCHMARK("accumulate (sum)", ITERATIONS, {
        volatile auto result =
            accumulate_int128(vec.begin(), vec.end(), int128_t(0), std::plus<int128_t>());
        (void)result;
    });
}

void benchmark_sum()
{
    std::cout << "\n[Benchmark] sum_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t(i - 5000));
    }

    BENCHMARK("sum", ITERATIONS, {
        volatile auto result = sum_int128(vec.begin(), vec.end());
        (void)result;
    });
}

void benchmark_product()
{
    std::cout << "\n[Benchmark] product_int128\n";

    const size_t SIZE = 20;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t(i - 10));
    }

    BENCHMARK("product", ITERATIONS, {
        volatile auto result = product_int128(vec.begin(), vec.end());
        (void)result;
    });
}

// ===============================================================================
// BENCHMARKS DE PARTICION Y ORDENAMIENTO
// ===============================================================================

void benchmark_partition()
{
    std::cout << "\n[Benchmark] partition_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 100;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);

    auto predicate = [](const int128_t& x) { return x < int128_t(0); };

    BENCHMARK("partition (negative/positive)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_int128());
        }
        partition_int128(vec.begin(), vec.end(), predicate);
    });
}

void benchmark_sort()
{
    std::cout << "\n[Benchmark] sort_int128\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 50;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);

    BENCHMARK("sort (ascending)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_int128());
        }
        sort_int128(vec.begin(), vec.end());
    });

    BENCHMARK("sort (descending)", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_int128());
        }
        sort_int128(vec.begin(), vec.end(), std::greater<int128_t>());
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

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t((i + 1) * 123456));
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

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (int i = 0; i < static_cast<int>(SIZE); ++i) {
        vec.push_back(int128_t(i + 2));
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

    std::vector<int128_t> seq(SIZE);

    BENCHMARK("generate_arithmetic", ITERATIONS,
              { generate_arithmetic_sequence(seq.begin(), SIZE, int128_t(-5000), int128_t(5)); });
}

void benchmark_generate_geometric()
{
    std::cout << "\n[Benchmark] generate_geometric_sequence\n";

    const size_t SIZE = 30;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> seq(SIZE);

    BENCHMARK("generate_geometric", ITERATIONS,
              { generate_geometric_sequence(seq.begin(), SIZE, int128_t(2), int128_t(2)); });
}

// ===============================================================================
// BENCHMARKS ESPECIFICOS DE SIGNADOS
// ===============================================================================

void benchmark_partition_by_sign()
{
    std::cout << "\n[Benchmark] partition_by_sign\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 100;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);

    BENCHMARK("partition_by_sign", ITERATIONS, {
        vec.clear();
        for (size_t i = 0; i < SIZE; ++i) {
            vec.push_back(random_int128());
        }
        partition_by_sign(vec.begin(), vec.end());
    });
}

void benchmark_max_abs_value()
{
    std::cout << "\n[Benchmark] max_abs_value\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 1000;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_int128());
    }

    BENCHMARK("max_abs_value", ITERATIONS, {
        volatile auto result = max_abs_value(vec.begin(), vec.end());
        (void)result;
    });
}

// ===============================================================================
// BENCHMARKS DE ESTADISTICAS
// ===============================================================================

void benchmark_calculate_stats()
{
    std::cout << "\n[Benchmark] calculate_stats\n";

    const size_t SIZE = 10000;
    const size_t ITERATIONS = 500;

    std::vector<int128_t> vec;
    vec.reserve(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        vec.push_back(random_int128());
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
    std::cout << "  int128_algorithm.hpp Benchmarks\n";
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
    benchmark_partition_by_sign();
    benchmark_max_abs_value();
    benchmark_calculate_stats();

    std::cout << "\n========================================\n";
    std::cout << "  [OK] Benchmarks completados\n";
    std::cout << "========================================\n";

    return 0;
}
