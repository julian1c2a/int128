/**
 * @file int128_cmath_extracted_benchs.cpp
 * @brief Benchmarks unificados para funciones matemáticas de int128_base_t
 *
 * Testea funciones de <cmath> style para uint128_t e int128_t:
 * - gcd, lcm (greatest common divisor, least common multiple)
 * - sqrt, cbrt (raíces)
 * - pow (exponenciación)
 * - abs, sign
 * - min, max
 */

#include "int128_base_cmath.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>

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

void benchmark_gcd()
{
    std::cout << "\n=== GCD ===\n";

    BENCHMARK("gcd", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        auto result = nstd::gcd(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("gcd", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        auto result = nstd::gcd(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("std::gcd", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        volatile auto result = std::gcd(a, b);
        (void)result;
    });
}

void benchmark_lcm()
{
    std::cout << "\n=== LCM ===\n";

    // Usamos valores pequeños para evitar overflow
    BENCHMARK("lcm", "uint128_t", ITERATIONS, {
        uint128_t a{random_u64() % 10000};
        uint128_t b{random_u64() % 10000};
        auto result = nstd::lcm(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("lcm", "int128_t", ITERATIONS, {
        int128_t a{static_cast<int64_t>(random_u64() % 10000)};
        int128_t b{static_cast<int64_t>(random_u64() % 10000)};
        auto result = nstd::lcm(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("std::lcm", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64() % 10000;
        uint64_t b = random_u64() % 10000;
        volatile auto result = std::lcm(a, b);
        (void)result;
    });
}

void benchmark_sqrt()
{
    std::cout << "\n=== SQRT ===\n";

    BENCHMARK("sqrt", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        auto result = nstd::sqrt(x);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("sqrt", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        if (x.is_negative())
            x = -x;
        auto result = nstd::sqrt(x);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("sqrt (double)", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile auto result = static_cast<uint64_t>(std::sqrt(static_cast<double>(x)));
        (void)result;
    });
}

void benchmark_pow()
{
    std::cout << "\n=== POW ===\n";

    BENCHMARK("pow", "uint128_t", ITERATIONS, {
        uint128_t base{random_u64() % 100};
        unsigned exp = rng() % 10;
        auto result = nstd::pow(base, exp);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("pow", "int128_t", ITERATIONS, {
        int128_t base{static_cast<int64_t>(random_u64() % 100) - 50};
        unsigned exp = rng() % 10;
        auto result = nstd::pow(base, exp);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("pow (double)", "uint64_t", ITERATIONS, {
        uint64_t base = random_u64() % 100;
        unsigned exp = rng() % 10;
        volatile auto result = static_cast<uint64_t>(std::pow(static_cast<double>(base), exp));
        (void)result;
    });
}

void benchmark_abs()
{
    std::cout << "\n=== ABS ===\n";

    BENCHMARK("abs", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        auto result = x.abs();
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("abs", "int64_t", ITERATIONS, {
        int64_t x = static_cast<int64_t>(random_u64());
        volatile auto result = std::abs(x);
        (void)result;
    });
}

void benchmark_min_max()
{
    std::cout << "\n=== MIN/MAX ===\n";

    BENCHMARK("min", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        auto result = nstd::min(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("max", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        auto result = nstd::max(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("min", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        auto result = nstd::min(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("max", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        auto result = nstd::max(a, b);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("std::min", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        volatile auto result = std::min(a, b);
        (void)result;
    });

    BENCHMARK("std::max", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        volatile auto result = std::max(a, b);
        (void)result;
    });
}

void benchmark_clamp()
{
    std::cout << "\n=== CLAMP ===\n";

    BENCHMARK("clamp", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t lo{1000};
        uint128_t hi{1000000};
        auto result = nstd::clamp(x, lo, hi);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("clamp", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t lo{-1000000};
        int128_t hi{1000000};
        auto result = nstd::clamp(x, lo, hi);
        volatile auto sink = result.low();
        (void)sink;
    });

    BENCHMARK("std::clamp", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        uint64_t lo = 1000;
        uint64_t hi = 1000000;
        volatile auto result = std::clamp(x, lo, hi);
        (void)result;
    });
}

int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 CMath Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Iterations: " << ITERATIONS << "\n";

    benchmark_gcd();
    benchmark_lcm();
    benchmark_sqrt();
    benchmark_pow();
    benchmark_abs();
    benchmark_min_max();
    benchmark_clamp();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
