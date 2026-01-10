/**
 * @file int128_tt_extracted_benchs.cpp
 * @brief Benchmarks unificados para int128_base_t (uint128_t e int128_t)
 *
 * Este archivo contiene benchmarks del template unificado int128_base_t<S>
 * comparando rendimiento entre uint128_t, int128_t, tipos builtin y nativos.
 */

#include "int128_base_tt.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

// Native 128-bit types (GCC/Clang only)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
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

inline int64_t random_i64()
{
    return static_cast<int64_t>(rng());
}

inline uint128_t random_uint128()
{
    return uint128_t(random_u64(), random_u64());
}

inline int128_t random_int128()
{
    return int128_t(static_cast<uint64_t>(random_i64()), random_u64());
}

// Macro BENCHMARK simple - el código debe ser self-contained
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
                  << std::setw(10) << ns << " ns/op"                                                \
                  << std::setw(12) << cycles << " cycles/op\n";                                     \
    }

constexpr size_t ITERATIONS = 1000000;
constexpr size_t STR_ITERATIONS = 100000;

// ============= Construction Benchmarks =============
void benchmark_construction()
{
    std::cout << "\n=== CONSTRUCTION ===\n";

    BENCHMARK("default constructor", "uint128_t", ITERATIONS, {
        volatile uint128_t x;
        (void)x;
    });

    BENCHMARK("default constructor", "int128_t", ITERATIONS, {
        volatile int128_t x;
        (void)x;
    });

    BENCHMARK("from uint64", "uint128_t", ITERATIONS, {
        uint64_t v = random_u64();
        uint128_t x(v);
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("from int64", "int128_t", ITERATIONS, {
        int64_t v = random_i64();
        int128_t x(v);
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("from pair (h,l)", "uint128_t", ITERATIONS, {
        uint64_t h = random_u64();
        uint64_t l = random_u64();
        uint128_t x(h, l);
        volatile auto sink = x.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("from uint64", "__uint128_t", ITERATIONS, {
        uint64_t v = random_u64();
        __uint128_t x = v;
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });
#endif
}

// ============= Arithmetic Benchmarks =============
void benchmark_arithmetic()
{
    std::cout << "\n=== ARITHMETIC ===\n";

    // Addition
    BENCHMARK("addition", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a + b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("addition", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a + b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("addition", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        uint64_t c = a + b;
        volatile auto sink = c;
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("addition", "__uint128_t", ITERATIONS, {
        __uint128_t a = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t b = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t c = a + b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });
#endif

    // Subtraction
    BENCHMARK("subtraction", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a - b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("subtraction", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a - b;
        volatile auto sink = c.low();
        (void)sink;
    });

    // Multiplication
    BENCHMARK("multiplication", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a * b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("multiplication", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a * b;
        volatile auto sink = c.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("multiplication", "__uint128_t", ITERATIONS, {
        __uint128_t a = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t b = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t c = a * b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });
#endif

    // Division (smaller iteration count - expensive)
    constexpr size_t DIV_ITER = ITERATIONS / 10;

    BENCHMARK("division", "uint128_t", DIV_ITER, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if (b != uint128_t(0))
        {
            uint128_t c = a / b;
            volatile auto sink = c.low();
            (void)sink;
        }
    });

    BENCHMARK("division", "int128_t", DIV_ITER, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        if (b != int128_t(0))
        {
            int128_t c = a / b;
            volatile auto sink = c.low();
            (void)sink;
        }
    });
}

// ============= Bitwise Benchmarks =============
void benchmark_bitwise()
{
    std::cout << "\n=== BITWISE OPERATIONS ===\n";

    BENCHMARK("AND", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a & b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("OR", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a | b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("XOR", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a ^ b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("NOT", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t c = ~a;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("left shift", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        int shift = rng() % 128;
        uint128_t c = a << shift;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("right shift", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        int shift = rng() % 128;
        uint128_t c = a >> shift;
        volatile auto sink = c.low();
        (void)sink;
    });
}

// ============= Comparison Benchmarks =============
void benchmark_comparison()
{
    std::cout << "\n=== COMPARISON ===\n";

    BENCHMARK("equality", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        volatile bool r = (a == b);
        (void)r;
    });

    BENCHMARK("less than", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        volatile bool r = (a < b);
        (void)r;
    });

    BENCHMARK("less than", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        volatile bool r = (a < b);
        (void)r;
    });
}

// ============= String Conversion Benchmarks =============
void benchmark_string()
{
    std::cout << "\n=== STRING CONVERSIONS ===\n";

    BENCHMARK("to_string (dec)", "uint128_t", STR_ITERATIONS, {
        uint128_t a = random_uint128();
        std::string s = a.to_string();
        volatile auto sink = s.size();
        (void)sink;
    });

    BENCHMARK("to_string (dec)", "int128_t", STR_ITERATIONS, {
        int128_t a = random_int128();
        std::string s = a.to_string();
        volatile auto sink = s.size();
        (void)sink;
    });

    BENCHMARK("to_string (hex)", "uint128_t", STR_ITERATIONS, {
        uint128_t a = random_uint128();
        std::string s = a.to_string(16);
        volatile auto sink = s.size();
        (void)sink;
    });
}

// ============= Increment/Decrement Benchmarks =============
void benchmark_incr_decr()
{
    std::cout << "\n=== INCREMENT/DECREMENT ===\n";

    BENCHMARK("++x (pre)", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        ++x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("++x (pre)", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        ++x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("--x (pre)", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        --x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("incr()", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t y = x.incr();
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("decr()", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t y = x.decr();
        volatile auto sink = y.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("++x (pre)", "__uint128_t", ITERATIONS, {
        __uint128_t x = ((__uint128_t)random_u64() << 64) | random_u64();
        ++x;
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });
#endif
}

// ============= Signed-specific Benchmarks =============
void benchmark_signed_ops()
{
    std::cout << "\n=== SIGNED OPERATIONS ===\n";

    BENCHMARK("negate (-x)", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t y = -x;
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("abs()", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t y = x.abs();
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("is_negative()", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile bool r = x.is_negative();
        (void)r;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("negate (-x)", "__int128_t", ITERATIONS, {
        __int128_t x = ((__int128_t)random_i64() << 64) | random_u64();
        __int128_t y = -x;
        volatile auto sink = static_cast<int64_t>(y);
        (void)sink;
    });
#endif
}

// ============= Main =============
int main()
{
    std::cout << "========================================\n";
    std::cout << " int128_base_t Unified Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Iterations: " << ITERATIONS << " (string: " << STR_ITERATIONS << ")\n";

#ifdef HAS_NATIVE_INT128
    std::cout << "Native __int128: AVAILABLE\n";
#else
    std::cout << "Native __int128: NOT AVAILABLE\n";
#endif

    benchmark_construction();
    benchmark_arithmetic();
    benchmark_bitwise();
    benchmark_comparison();
    benchmark_string();
    benchmark_incr_decr();
    benchmark_signed_ops();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
