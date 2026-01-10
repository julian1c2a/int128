/**
 * @file int128_limits_extracted_benchs.cpp
 * @brief Benchmarks unificados para nstd::numeric_limits de int128_base_t
 *
 * NOTA IMPORTANTE: Para tipos int128, usar nstd::numeric_limits, NO std::numeric_limits.
 * std::numeric_limits no se puede especializar portablemente para tipos de usuario.
 *
 * Testea acceso a constantes de numeric_limits:
 * - min(), max(), lowest()
 * - digits, digits10, is_signed
 * - Comparación constexpr vs runtime
 */

#include "int128_base_limits.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

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

constexpr size_t ITERATIONS = 1000000;

void benchmark_limits_min()
{
    std::cout << "\n=== nstd::numeric_limits::min() (int128) / std::numeric_limits::min() (builtin) ===\n";

    BENCHMARK("min()", "uint128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<uint128_t>::min();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("min()", "int128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<int128_t>::min();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("min()", "uint64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<uint64_t>::min();
        (void)result;
    });

    BENCHMARK("min()", "int64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<int64_t>::min();
        (void)result;
    });
}

void benchmark_limits_max()
{
    std::cout << "\n=== nstd::numeric_limits::max() (int128) / std::numeric_limits::max() (builtin) ===\n";

    BENCHMARK("max()", "uint128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<uint128_t>::max();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("max()", "int128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<int128_t>::max();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("max()", "uint64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<uint64_t>::max();
        (void)result;
    });

    BENCHMARK("max()", "int64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<int64_t>::max();
        (void)result;
    });
}

void benchmark_limits_lowest()
{
    std::cout << "\n=== nstd::numeric_limits::lowest() (int128) / std::numeric_limits::lowest() (builtin) ===\n";

    BENCHMARK("lowest()", "uint128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<uint128_t>::lowest();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("lowest()", "int128_t", ITERATIONS, {
        auto result = nstd::numeric_limits<int128_t>::lowest();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("lowest()", "uint64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<uint64_t>::lowest();
        (void)result;
    });

    BENCHMARK("lowest()", "int64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<int64_t>::lowest();
        (void)result;
    });
}

void benchmark_limits_digits()
{
    std::cout << "\n=== nstd::numeric_limits::digits (int128) / std::numeric_limits::digits (builtin) ===\n";

    BENCHMARK("digits", "uint128_t", ITERATIONS, {
        volatile auto result = nstd::numeric_limits<uint128_t>::digits;
        (void)result;
    });

    BENCHMARK("digits", "int128_t", ITERATIONS, {
        volatile auto result = nstd::numeric_limits<int128_t>::digits;
        (void)result;
    });

    BENCHMARK("digits", "uint64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<uint64_t>::digits;
        (void)result;
    });

    BENCHMARK("digits", "int64_t", ITERATIONS, {
        volatile auto result = std::numeric_limits<int64_t>::digits;
        (void)result;
    });
}

void verify_limits()
{
    std::cout << "\n=== Verification (using nstd:: for int128 types) ===\n";

    std::cout << "uint128_t (nstd::):\n";
    std::cout << "  digits    = " << nstd::numeric_limits<uint128_t>::digits << "\n";
    std::cout << "  digits10  = " << nstd::numeric_limits<uint128_t>::digits10 << "\n";
    std::cout << "  is_signed = " << std::boolalpha << nstd::numeric_limits<uint128_t>::is_signed << "\n";

    std::cout << "int128_t (nstd::):\n";
    std::cout << "  digits    = " << nstd::numeric_limits<int128_t>::digits << "\n";
    std::cout << "  digits10  = " << nstd::numeric_limits<int128_t>::digits10 << "\n";
    std::cout << "  is_signed = " << std::boolalpha << nstd::numeric_limits<int128_t>::is_signed << "\n";

    std::cout << "uint64_t (std::):\n";
    std::cout << "  digits    = " << std::numeric_limits<uint64_t>::digits << "\n";
    std::cout << "  digits10  = " << std::numeric_limits<uint64_t>::digits10 << "\n";
    std::cout << "  is_signed = " << std::boolalpha << std::numeric_limits<uint64_t>::is_signed << "\n";
}

int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 Limits Benchmarks\n";
    std::cout << "========================================\n";

    benchmark_limits_min();
    benchmark_limits_max();
    benchmark_limits_lowest();
    benchmark_limits_digits();
    verify_limits();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
