/**
 * @file int128_bits_benchmarks.cpp
 * @brief Benchmarks para funciones de int128_bits.hpp
 */

#include "int128/int128_bits.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

using namespace int128_bits;
using namespace std::chrono;

// Generador de numeros aleatorios
std::mt19937_64 rng(std::random_device{}());

// Generador de valores int128_t aleatorios
int128_t random_int128(int64_t max_high = INT64_MAX, uint64_t max_low = UINT64_MAX)
{
    int64_t high = std::uniform_int_distribution<int64_t>(INT64_MIN, max_high)(rng);
    uint64_t low = std::uniform_int_distribution<uint64_t>(0, max_low)(rng);
    return int128_t(high, low);
}

// Macro para benchmark
#define BENCHMARK(name, iterations, code)                                                          \
    {                                                                                              \
        auto start = high_resolution_clock::now();                                                 \
        for (size_t i = 0; i < iterations; ++i) {                                                  \
            code;                                                                                  \
        }                                                                                          \
        auto end = high_resolution_clock::now();                                                   \
        auto duration = duration_cast<microseconds>(end - start).count();                          \
        double avg_us = static_cast<double>(duration) / iterations;                                \
        std::cout << "  " << name << ": " << avg_us << " us/op (" << iterations << " ops)\n";      \
    }

// ===============================================================================
// BENCHMARKS std NAMESPACE
// ===============================================================================

void benchmark_popcount()
{
    std::cout << "\n[Benchmark] std::popcount\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("popcount", ITERATIONS, { volatile int result = std::popcount(value); });
}

void benchmark_countl_zero()
{
    std::cout << "\n[Benchmark] std::countl_zero\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("countl_zero", ITERATIONS, { volatile int result = std::countl_zero(value); });
}

void benchmark_countr_zero()
{
    std::cout << "\n[Benchmark] std::countr_zero\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("countr_zero", ITERATIONS, { volatile int result = std::countr_zero(value); });
}

void benchmark_countl_one()
{
    std::cout << "\n[Benchmark] std::countl_one\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("countl_one", ITERATIONS, { volatile int result = std::countl_one(value); });
}

void benchmark_countr_one()
{
    std::cout << "\n[Benchmark] std::countr_one\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("countr_one", ITERATIONS, { volatile int result = std::countr_one(value); });
}

void benchmark_bit_width()
{
    std::cout << "\n[Benchmark] std::bit_width\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("bit_width", ITERATIONS, { volatile int result = std::bit_width(value); });
}

void benchmark_has_single_bit()
{
    std::cout << "\n[Benchmark] std::has_single_bit\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("has_single_bit", ITERATIONS, { volatile bool result = std::has_single_bit(value); });
}

void benchmark_bit_floor()
{
    std::cout << "\n[Benchmark] std::bit_floor\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();

    BENCHMARK("bit_floor", ITERATIONS, { volatile int128_t result = std::bit_floor(value); });
}

void benchmark_bit_ceil()
{
    std::cout << "\n[Benchmark] std::bit_ceil\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();

    BENCHMARK("bit_ceil", ITERATIONS, { volatile int128_t result = std::bit_ceil(value); });
}

// ===============================================================================
// BENCHMARKS int128_bits NAMESPACE
// ===============================================================================

void benchmark_rotl()
{
    std::cout << "\n[Benchmark] rotl\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int shift = std::uniform_int_distribution<int>(1, 127)(rng);

    BENCHMARK("rotl", ITERATIONS, { volatile int128_t result = rotl(value, shift); });
}

void benchmark_rotr()
{
    std::cout << "\n[Benchmark] rotr\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int shift = std::uniform_int_distribution<int>(1, 127)(rng);

    BENCHMARK("rotr", ITERATIONS, { volatile int128_t result = rotr(value, shift); });
}

void benchmark_reverse_bits()
{
    std::cout << "\n[Benchmark] reverse_bits\n";

    const size_t ITERATIONS = 100000;
    int128_t value = random_int128();

    BENCHMARK("reverse_bits", ITERATIONS, { volatile int128_t result = reverse_bits(value); });
}

void benchmark_byteswap()
{
    std::cout << "\n[Benchmark] byteswap\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();

    BENCHMARK("byteswap", ITERATIONS, { volatile int128_t result = byteswap(value); });
}

void benchmark_extract_bits()
{
    std::cout << "\n[Benchmark] extract_bits\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int offset = std::uniform_int_distribution<int>(0, 100)(rng);
    int width = std::uniform_int_distribution<int>(1, 28)(rng);

    BENCHMARK("extract_bits", ITERATIONS,
              { volatile int128_t result = extract_bits(value, offset, width); });
}

void benchmark_insert_bits()
{
    std::cout << "\n[Benchmark] insert_bits\n";

    const size_t ITERATIONS = 500000;
    int128_t dest = random_int128();
    int128_t src = random_int128();
    int offset = std::uniform_int_distribution<int>(0, 100)(rng);
    int width = std::uniform_int_distribution<int>(1, 28)(rng);

    BENCHMARK("insert_bits", ITERATIONS,
              { volatile int128_t result = insert_bits(dest, src, offset, width); });
}

void benchmark_test_bit()
{
    std::cout << "\n[Benchmark] test_bit\n";

    const size_t ITERATIONS = 1000000;
    int128_t value = random_int128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("test_bit", ITERATIONS, { volatile bool result = test_bit(value, pos); });
}

void benchmark_set_bit()
{
    std::cout << "\n[Benchmark] set_bit\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("set_bit", ITERATIONS, { volatile int128_t result = set_bit(value, pos); });
}

void benchmark_clear_bit()
{
    std::cout << "\n[Benchmark] clear_bit\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("clear_bit", ITERATIONS, { volatile int128_t result = clear_bit(value, pos); });
}

void benchmark_flip_bit()
{
    std::cout << "\n[Benchmark] flip_bit\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();
    int pos = std::uniform_int_distribution<int>(0, 127)(rng);

    BENCHMARK("flip_bit", ITERATIONS, { volatile int128_t result = flip_bit(value, pos); });
}

void benchmark_find_first_set()
{
    std::cout << "\n[Benchmark] find_first_set\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();

    BENCHMARK("find_first_set", ITERATIONS, { volatile int result = find_first_set(value); });
}

void benchmark_find_last_set()
{
    std::cout << "\n[Benchmark] find_last_set\n";

    const size_t ITERATIONS = 500000;
    int128_t value = random_int128();

    BENCHMARK("find_last_set", ITERATIONS, { volatile int result = find_last_set(value); });
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  int128_bits.hpp Benchmarks\n";
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

    // Benchmarks int128_bits namespace
    std::cout << "\n--- int128_bits namespace ---\n";
    benchmark_rotl();
    benchmark_rotr();
    benchmark_reverse_bits();
    benchmark_byteswap();
    benchmark_extract_bits();
    benchmark_insert_bits();
    benchmark_test_bit();
    benchmark_set_bit();
    benchmark_clear_bit();
    benchmark_flip_bit();
    benchmark_find_first_set();
    benchmark_find_last_set();

    std::cout << "\n========================================\n";
    std::cout << "[OK] Benchmarks completados\n";
    std::cout << "========================================\n";

    return 0;
}
