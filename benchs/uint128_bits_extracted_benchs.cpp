/**
 * @file uint128_bits_benchmarks.cpp
 * @brief Benchmarks para funciones de uint128_bits.hpp
 */

#include "uint128/uint128_bits.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

using namespace uint128_bits;
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
uint128_t random_uint128(uint64_t max_high = UINT64_MAX, uint64_t max_low = UINT64_MAX)
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
// BENCHMARKS std NAMESPACE
// ===============================================================================

void benchmark_popcount()
{
    std::cout << "\n[Benchmark] std::popcount\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("popcount", ITERATIONS, {
        volatile int result = nstd::popcount(value);
        (void)result;
    });
}

void benchmark_countl_zero()
{
    std::cout << "\n[Benchmark] std::countl_zero\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("countl_zero", ITERATIONS, {
        volatile int result = nstd::countl_zero(value);
        (void)result;
    });
}

void benchmark_countr_zero()
{
    std::cout << "\n[Benchmark] std::countr_zero\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("countr_zero", ITERATIONS, {
        volatile int result = nstd::countr_zero(value);
        (void)result;
    });
}

void benchmark_countl_one()
{
    std::cout << "\n[Benchmark] std::countl_one\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("countl_one", ITERATIONS, {
        volatile int result = nstd::countl_one(value);
        (void)result;
    });
}

void benchmark_countr_one()
{
    std::cout << "\n[Benchmark] std::countr_one\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("countr_one", ITERATIONS, {
        volatile int result = nstd::countr_one(value);
        (void)result;
    });
}

void benchmark_bit_width()
{
    std::cout << "\n[Benchmark] std::bit_width\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("bit_width", ITERATIONS, {
        volatile int result = nstd::bit_width(value);
        (void)result;
    });
}

void benchmark_has_single_bit()
{
    std::cout << "\n[Benchmark] std::has_single_bit\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("has_single_bit", ITERATIONS, {
        volatile bool result = nstd::has_single_bit(value);
        (void)result;
    });
}

void benchmark_bit_floor()
{
    std::cout << "\n[Benchmark] std::bit_floor\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();

    BENCHMARK("bit_floor", ITERATIONS, {
        volatile uint128_t result = nstd::bit_floor(value);
        (void)result;
    });
}

void benchmark_bit_ceil()
{
    std::cout << "\n[Benchmark] std::bit_ceil\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();

    BENCHMARK("bit_ceil", ITERATIONS, {
        volatile uint128_t result = nstd::bit_ceil(value);
        (void)result;
    });
}

// ===============================================================================
// BENCHMARKS uint128_bits NAMESPACE
// ===============================================================================

void benchmark_rotl()
{
    std::cout << "\n[Benchmark] rotl\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();
    int shift = std::uniform_int_distribution<int>(1, 127)(rng);

    BENCHMARK("rotl", ITERATIONS, {
        volatile uint128_t result = rotl(value, shift);
        (void)result;
    });
}

void benchmark_rotr()
{
    std::cout << "\n[Benchmark] rotr\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();
    int shift = std::uniform_int_distribution<int>(1, 127)(rng);

    BENCHMARK("rotr", ITERATIONS, {
        volatile uint128_t result = rotr(value, shift);
        (void)result;
    });
}

void benchmark_reverse_bits()
{
    std::cout << "\n[Benchmark] reverse_bits\n";

    const size_t ITERATIONS = 100000;
    uint128_t value = random_uint128();

    BENCHMARK("reverse_bits", ITERATIONS, {
        volatile uint128_t result = reverse_bits(value);
        (void)result;
    });
}

void benchmark_byteswap()
{
    std::cout << "\n[Benchmark] byteswap\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();

    BENCHMARK("byteswap", ITERATIONS, {
        volatile uint128_t result = byteswap(value);
        (void)result;
    });
}

void benchmark_extract_bits()
{
    std::cout << "\n[Benchmark] extract_bits\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();
    int offset = std::uniform_int_distribution<int>(0, 100)(rng);
    int width = std::uniform_int_distribution<int>(1, 28)(rng);

    BENCHMARK("extract_bits", ITERATIONS, {
        volatile uint128_t result = extract_bits(value, offset, width);
        (void)result;
    });
}

void benchmark_insert_bits()
{
    std::cout << "\n[Benchmark] insert_bits\n";

    const size_t ITERATIONS = 500000;
    uint128_t dest = random_uint128();
    uint128_t src = random_uint128();
    int offset = std::uniform_int_distribution<int>(0, 100)(rng);
    int width = std::uniform_int_distribution<int>(1, 28)(rng);

    BENCHMARK("insert_bits", ITERATIONS, {
        volatile uint128_t result = insert_bits(dest, src, offset, width);
        (void)result;
    });
}

void benchmark_parallel_deposit()
{
    std::cout << "\n[Benchmark] parallel_deposit\n";

    const size_t ITERATIONS = 100000;
    uint128_t value = random_uint128();
    uint128_t mask = random_uint128();

    BENCHMARK("parallel_deposit", ITERATIONS, {
        volatile uint128_t result = parallel_deposit(value, mask);
        (void)result;
    });
}

void benchmark_parallel_extract()
{
    std::cout << "\n[Benchmark] parallel_extract\n";

    const size_t ITERATIONS = 100000;
    uint128_t value = random_uint128();
    uint128_t mask = random_uint128();

    BENCHMARK("parallel_extract", ITERATIONS, {
        volatile uint128_t result = parallel_extract(value, mask);
        (void)result;
    });
}

void benchmark_get_bit()
{
    std::cout << "\n[Benchmark] get_bit\n";

    const size_t ITERATIONS = 1000000;
    uint128_t value = random_uint128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("get_bit", ITERATIONS, {
        volatile bool result = get_bit(value, pos);
        (void)result;
    });
}

void benchmark_set_bit()
{
    std::cout << "\n[Benchmark] set_bit\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("set_bit", ITERATIONS, {
        volatile uint128_t result = set_bit(value, pos);
        (void)result;
    });
}

void benchmark_flip_bit()
{
    std::cout << "\n[Benchmark] flip_bit\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("flip_bit", ITERATIONS, {
        volatile uint128_t result = flip_bit(value, pos);
        (void)result;
    });
}

void benchmark_find_first_set()
{
    std::cout << "\n[Benchmark] find_first_set\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();

    BENCHMARK("find_first_set", ITERATIONS, {
        volatile int result = find_first_set(value);
        (void)result;
    });
}

void benchmark_find_last_set()
{
    std::cout << "\n[Benchmark] find_last_set\n";

    const size_t ITERATIONS = 500000;
    uint128_t value = random_uint128();

    BENCHMARK("find_last_set", ITERATIONS, {
        volatile int result = find_last_set(value);
        (void)result;
    });
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  uint128_bits.hpp Benchmarks\n";
    std::cout << "========================================\n";

    // Benchmarks std namespace
    std::cout << "\n--- std namespace ---\n";
    benchmark_popcount();
    benchmark_countl_zero();
    benchmark_countr_zero();
    benchmark_countl_one();
    benchmark_countr_one();
    benchmark_bit_width();
    benchmark_has_single_bit();
    benchmark_bit_floor();
    benchmark_bit_ceil();

    // Benchmarks uint128_bits namespace
    std::cout << "\n--- uint128_bits namespace ---\n";
    benchmark_rotl();
    benchmark_rotr();
    benchmark_reverse_bits();
    benchmark_byteswap();
    benchmark_extract_bits();
    benchmark_insert_bits();
    benchmark_parallel_deposit();
    benchmark_parallel_extract();
    benchmark_get_bit();
    benchmark_set_bit();
    benchmark_flip_bit();
    benchmark_find_first_set();
    benchmark_find_last_set();

    std::cout << "\n========================================\n";
    std::cout << "[OK] Benchmarks completados\n";
    std::cout << "========================================\n";

    return 0;
}
