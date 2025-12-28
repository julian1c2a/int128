/**
 * @file int128_numeric_benchmarks.cpp
 * @brief Performance benchmarks for int128_numeric.hpp functions
 *
 * Benchmarks C++20 numeric functions for signed int128:
 * - std::midpoint, std::clamp, std::abs
 * - std::popcount, countl_zero, countr_zero (bit manipulation)
 * - std::bit_width, has_single_bit, bit_ceil, bit_floor
 */

#include "../include/int128/int128_numeric.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace nstd;
// ========================= RDTSC for CPU Cycles =========================

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#elif defined(__INTEL_COMPILER)
#include <ia32intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <x86intrin.h>
#endif

inline uint64_t rdtsc()
{
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    return __rdtsc();
#else
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return (static_cast<uint64_t>(hi) << 32) | lo;
#endif
}
#else
inline uint64_t rdtsc()
{
    return 0;
}
#endif

// ========================= Benchmark Infrastructure =========================

struct BenchmarkResult {
    std::string name;
    double time_ns;
    uint64_t cycles;
    size_t iterations;
};

constexpr size_t ITERATIONS = 100000;
constexpr size_t WARMUP = 1000;

// ========================= Benchmark: midpoint =========================

BenchmarkResult bench_midpoint()
{
    std::vector<int128_t> values_a(ITERATIONS);
    std::vector<int128_t> values_b(ITERATIONS);
    std::vector<int128_t> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(-1000000, 1000000);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values_a[i] = int128_t(dist(rng));
        values_b[i] = int128_t(dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::midpoint(values_a[i], values_b[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::midpoint(values_a[i], values_b[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int128_t sink = results[ITERATIONS / 2];
    (void)sink;

    return {"midpoint", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: clamp =========================

BenchmarkResult bench_clamp()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int128_t> results(ITERATIONS);
    int128_t lo = int128_t(-1000);
    int128_t hi = int128_t(1000);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(-2000, 2000);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = std::clamp(values[i], lo, hi);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = std::clamp(values[i], lo, hi);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int128_t sink = results[ITERATIONS / 2];
    (void)sink;

    return {"clamp", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// ========================= Benchmark: abs =========================

BenchmarkResult bench_abs()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int128_t> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::abs(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::abs(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int128_t sink = results[ITERATIONS / 2];
    (void)sink;

    return {"abs", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// ========================= Benchmark: popcount =========================

BenchmarkResult bench_popcount()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist;

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(dist(rng), dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::popcount(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::popcount(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int sink = results[ITERATIONS / 2];
    (void)sink;

    return {"popcount", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: countl_zero =========================

BenchmarkResult bench_countl_zero()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(0, dist(rng)); // Positive values only for countl_zero
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::countl_zero(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::countl_zero(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int sink = results[ITERATIONS / 2];
    (void)sink;

    return {"countl_zero", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: countr_zero =========================

BenchmarkResult bench_countr_zero()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT32_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(0, dist(rng)) << (i % 64);
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::countr_zero(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::countr_zero(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int sink = results[ITERATIONS / 2];
    (void)sink;

    return {"countr_zero", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: bit_width =========================

BenchmarkResult bench_bit_width()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(0, dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::bit_width(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::bit_width(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int sink = results[ITERATIONS / 2];
    (void)sink;

    return {"bit_width", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: has_single_bit =========================

BenchmarkResult bench_has_single_bit()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<bool> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist;

    for (size_t i = 0; i < ITERATIONS; ++i) {
        if (i % 2 == 0) {
            values[i] = int128_t(0, uint64_t(1) << (i % 63)); // Power of 2
        } else {
            values[i] = int128_t(0, dist(rng));
        }
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::has_single_bit(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::has_single_bit(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile bool sink = results[ITERATIONS / 2];
    (void)sink;

    return {"has_single_bit", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: bit_ceil =========================

BenchmarkResult bench_bit_ceil()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int128_t> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT32_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(0, dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::bit_ceil(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::bit_ceil(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int128_t sink = results[ITERATIONS / 2];
    (void)sink;

    return {"bit_ceil", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Benchmark: bit_floor =========================

BenchmarkResult bench_bit_floor()
{
    std::vector<int128_t> values(ITERATIONS);
    std::vector<int128_t> results(ITERATIONS);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT32_MAX);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        values[i] = int128_t(0, dist(rng));
    }

    // Warmup
    for (size_t i = 0; i < WARMUP; ++i) {
        results[i] = nstd::bit_floor(values[i]);
    }

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t cycles_start = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        results[i] = nstd::bit_floor(values[i]);
    }

    uint64_t cycles_end = rdtsc();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    uint64_t cycles = cycles_end - cycles_start;

    volatile int128_t sink = results[ITERATIONS / 2];
    (void)sink;

    return {"bit_floor", static_cast<double>(elapsed_ns) / ITERATIONS, cycles / ITERATIONS,
            ITERATIONS};
}

// ========================= Display Results =========================

void print_result(const BenchmarkResult& result)
{
    std::cout << std::left << std::setw(20) << result.name << " | " << std::right << std::setw(10)
              << std::fixed << std::setprecision(2) << result.time_ns << " ns/op | "
              << std::setw(10) << result.cycles << " cycles/op | " << std::setw(10)
              << result.iterations << " iterations" << std::endl;
}

// ========================= Main =========================

int main()
{
    std::cout << "========================================================================"
              << std::endl;
    std::cout << "    INT128_T NUMERIC FUNCTIONS BENCHMARK" << std::endl;
    std::cout << "========================================================================"
              << std::endl;
    std::cout << "\nIterations per benchmark: " << ITERATIONS << std::endl;
    std::cout << "Warmup iterations: " << WARMUP << std::endl;
    std::cout << "\n" << std::endl;

    std::vector<BenchmarkResult> results;

    std::cout << "[1/10] Benchmarking midpoint..." << std::endl;
    results.push_back(bench_midpoint());

    std::cout << "[2/10] Benchmarking clamp..." << std::endl;
    results.push_back(bench_clamp());

    std::cout << "[3/10] Benchmarking abs..." << std::endl;
    results.push_back(bench_abs());

    std::cout << "[4/10] Benchmarking popcount..." << std::endl;
    results.push_back(bench_popcount());

    std::cout << "[5/10] Benchmarking countl_zero..." << std::endl;
    results.push_back(bench_countl_zero());

    std::cout << "[6/10] Benchmarking countr_zero..." << std::endl;
    results.push_back(bench_countr_zero());

    std::cout << "[7/10] Benchmarking bit_width..." << std::endl;
    results.push_back(bench_bit_width());

    std::cout << "[8/10] Benchmarking has_single_bit..." << std::endl;
    results.push_back(bench_has_single_bit());

    std::cout << "[9/10] Benchmarking bit_ceil..." << std::endl;
    results.push_back(bench_bit_ceil());

    std::cout << "[10/10] Benchmarking bit_floor..." << std::endl;
    results.push_back(bench_bit_floor());

    std::cout << "\n" << std::endl;
    std::cout << "========================================================================"
              << std::endl;
    std::cout << "    RESULTS" << std::endl;
    std::cout << "========================================================================"
              << std::endl;
    std::cout << std::left << std::setw(20) << "Function" << " | " << std::right << std::setw(10)
              << "Time" << "        | " << std::setw(10) << "Cycles" << "           | "
              << std::setw(10) << "Iterations" << std::endl;
    std::cout << "------------------------------------------------------------------------"
              << std::endl;

    for (const auto& result : results) {
        print_result(result);
    }

    std::cout << "========================================================================"
              << std::endl;

    return 0;
}
