/**
 * @file int128_bits_extracted_benchs.cpp
 * @brief Benchmarks unificados para operaciones de bits en int128_base_t
 *
 * Testea operaciones de bits (<bit> style) para uint128_t e int128_t:
 * - popcount, countl_zero, countr_zero, countl_one, countr_one
 * - bit_width, has_single_bit, bit_floor, bit_ceil
 * - rotl, rotr (rotaciones)
 * - byteswap
 */

#include "int128_base_bits.hpp"
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

#ifdef __SIZEOF_INT128__
#define HAS_NATIVE_INT128 1
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
    return 0;
#endif
}

// Generadores aleatorios
std::mt19937_64 rng(std::random_device{}());

inline uint64_t random_u64()
{
    return rng();
}

inline uint128_t random_uint128()
{
    return uint128_t(random_u64(), random_u64());
}

inline int128_t random_int128()
{
    return int128_t(static_cast<uint64_t>(static_cast<int64_t>(random_u64())), random_u64());
}

// Macro BENCHMARK
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
                  << std::setw(10) << ns << " ns/op"                                                \
                  << std::setw(12) << cycles << " cycles/op\n";                                     \
    }

constexpr size_t ITERATIONS = 1000000;

// ============= Popcount Benchmarks =============
void benchmark_popcount()
{
    std::cout << "\n=== POPCOUNT ===\n";

    BENCHMARK("popcount", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::popcount(x);
        (void)r;
    });

    BENCHMARK("popcount", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::popcount(x);
        (void)r;
    });

    BENCHMARK("popcount", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile int r = std::popcount(x);
        (void)r;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("popcount", "__uint128_t", ITERATIONS, {
        __uint128_t x = ((__uint128_t)random_u64() << 64) | random_u64();
        volatile int r = __builtin_popcountll((uint64_t)x) + __builtin_popcountll((uint64_t)(x >> 64));
        (void)r;
    });
#endif
}

// ============= Count Leading Zeros =============
void benchmark_countl_zero()
{
    std::cout << "\n=== COUNTL_ZERO ===\n";

    BENCHMARK("countl_zero", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::countl_zero(x);
        (void)r;
    });

    BENCHMARK("countl_zero", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::countl_zero(x);
        (void)r;
    });

    BENCHMARK("countl_zero", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile int r = std::countl_zero(x);
        (void)r;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("countl_zero", "__uint128_t", ITERATIONS, {
        __uint128_t x = ((__uint128_t)random_u64() << 64) | random_u64();
        uint64_t hi = (uint64_t)(x >> 64);
        volatile int r = (hi == 0) ? 64 + __builtin_clzll((uint64_t)x) : __builtin_clzll(hi);
        (void)r;
    });
#endif
}

// ============= Count Trailing Zeros =============
void benchmark_countr_zero()
{
    std::cout << "\n=== COUNTR_ZERO ===\n";

    BENCHMARK("countr_zero", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::countr_zero(x);
        (void)r;
    });

    BENCHMARK("countr_zero", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::countr_zero(x);
        (void)r;
    });

    BENCHMARK("countr_zero", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile int r = std::countr_zero(x);
        (void)r;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("countr_zero", "__uint128_t", ITERATIONS, {
        __uint128_t x = ((__uint128_t)random_u64() << 64) | random_u64();
        uint64_t lo = (uint64_t)x;
        volatile int r = (lo == 0) ? 64 + __builtin_ctzll((uint64_t)(x >> 64)) : __builtin_ctzll(lo);
        (void)r;
    });
#endif
}

// ============= Count Leading Ones =============
void benchmark_countl_one()
{
    std::cout << "\n=== COUNTL_ONE ===\n";

    BENCHMARK("countl_one", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::countl_one(x);
        (void)r;
    });

    BENCHMARK("countl_one", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::countl_one(x);
        (void)r;
    });
}

// ============= Count Trailing Ones =============
void benchmark_countr_one()
{
    std::cout << "\n=== COUNTR_ONE ===\n";

    BENCHMARK("countr_one", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::countr_one(x);
        (void)r;
    });

    BENCHMARK("countr_one", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::countr_one(x);
        (void)r;
    });
}

// ============= Bit Width =============
void benchmark_bit_width()
{
    std::cout << "\n=== BIT_WIDTH ===\n";

    BENCHMARK("bit_width", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile int r = nstd::bit_width(x);
        (void)r;
    });

    BENCHMARK("bit_width", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile int r = nstd::bit_width(x);
        (void)r;
    });

    BENCHMARK("bit_width", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile int r = std::bit_width(x);
        (void)r;
    });
}

// ============= Has Single Bit =============
void benchmark_has_single_bit()
{
    std::cout << "\n=== HAS_SINGLE_BIT ===\n";

    BENCHMARK("has_single_bit", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        volatile bool r = nstd::has_single_bit(x);
        (void)r;
    });

    BENCHMARK("has_single_bit", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile bool r = nstd::has_single_bit(x);
        (void)r;
    });

    BENCHMARK("has_single_bit", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile bool r = std::has_single_bit(x);
        (void)r;
    });
}

// ============= Bit Floor =============
void benchmark_bit_floor()
{
    std::cout << "\n=== BIT_FLOOR ===\n";

    BENCHMARK("bit_floor", "uint128_t", ITERATIONS / 2, {
        uint128_t x = random_uint128();
        uint128_t r = nstd::bit_floor(x);
        volatile auto sink = r.low();
        (void)sink;
    });

    BENCHMARK("bit_floor", "uint64_t", ITERATIONS / 2, {
        uint64_t x = random_u64();
        volatile uint64_t r = std::bit_floor(x);
        (void)r;
    });
}

// ============= Bit Ceil =============
void benchmark_bit_ceil()
{
    std::cout << "\n=== BIT_CEIL ===\n";

    BENCHMARK("bit_ceil", "uint128_t", ITERATIONS / 2, {
        uint128_t x = random_uint128();
        uint128_t r = nstd::bit_ceil(x);
        volatile auto sink = r.low();
        (void)sink;
    });

    BENCHMARK("bit_ceil", "uint64_t", ITERATIONS / 2, {
        uint64_t x = random_u64();
        volatile uint64_t r = std::bit_ceil(x);
        (void)r;
    });
}

// ============= Rotations =============
void benchmark_rotations()
{
    std::cout << "\n=== ROTATIONS ===\n";

    BENCHMARK("rotl", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        int shift = rng() % 128;
        uint128_t r = nstd::rotl(x, shift);
        volatile auto sink = r.low();
        (void)sink;
    });

    BENCHMARK("rotl", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        int shift = rng() % 64;
        volatile uint64_t r = std::rotl(x, shift);
        (void)r;
    });

    BENCHMARK("rotr", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        int shift = rng() % 128;
        uint128_t r = nstd::rotr(x, shift);
        volatile auto sink = r.low();
        (void)sink;
    });

    BENCHMARK("rotr", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        int shift = rng() % 64;
        volatile uint64_t r = std::rotr(x, shift);
        (void)r;
    });
}

// ============= Byteswap =============
void benchmark_byteswap()
{
    std::cout << "\n=== BYTESWAP ===\n";

    BENCHMARK("byteswap", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t r = nstd::byteswap(x);
        volatile auto sink = r.low();
        (void)sink;
    });

    BENCHMARK("byteswap", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t r = nstd::byteswap(x);
        volatile auto sink = r.low();
        (void)sink;
    });

#if __cplusplus > 202002L
    // std::byteswap es C++23
    BENCHMARK("byteswap", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile uint64_t r = std::byteswap(x);
        (void)r;
    });
#else
    // Fallback usando __builtin_bswap64
    BENCHMARK("byteswap", "uint64_t", ITERATIONS, {
        uint64_t x = random_u64();
        volatile uint64_t r = __builtin_bswap64(x);
        (void)r;
    });
#endif
}

// ============= Main =============
int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 Bit Operations Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Iterations: " << ITERATIONS << "\n";

#ifdef HAS_NATIVE_INT128
    std::cout << "Native __int128: AVAILABLE\n";
#else
    std::cout << "Native __int128: NOT available\n";
#endif

    benchmark_popcount();
    benchmark_countl_zero();
    benchmark_countr_zero();
    benchmark_countl_one();
    benchmark_countr_one();
    benchmark_bit_width();
    benchmark_has_single_bit();
    benchmark_bit_floor();
    benchmark_bit_ceil();
    benchmark_rotations();
    benchmark_byteswap();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
