/**
 * @file int128_traits_extracted_benchs.cpp
 * @brief Benchmarks unificados para type_traits de int128_base_t
 *
 * Testea que los type_traits funcionen correctamente:
 * - nstd::is_integral, nstd::is_arithmetic, nstd::is_unsigned, nstd::is_signed
 * - Comparación con tipos builtin (usando std:: para builtin)
 *
 * NOTA: Los traits de int128_t/uint128_t están en namespace nstd::, NO en std::
 * Esto es intencional porque el estándar C++ no permite especializar std::
 * para tipos definidos por el usuario en todas las implementaciones (ej: MSVC).
 *
 * Nota: Los traits son constexpr, por lo que el compilador optimiza
 * la mayoría a constantes. Este benchmark verifica compatibilidad.
 */

#include "int128_base_traits.hpp"
#include "int128_base_traits_specializations.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <type_traits>

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

void benchmark_is_integral()
{
    std::cout << "\n=== nstd::is_integral_v (int128) / std::is_integral_v (builtin) ===\n";

    // int128 types use nstd::
    BENCHMARK("is_integral_v", "uint128_t", ITERATIONS, {
        volatile bool result = nstd::is_integral_v<uint128_t>;
        (void)result;
    });

    BENCHMARK("is_integral_v", "int128_t", ITERATIONS, {
        volatile bool result = nstd::is_integral_v<int128_t>;
        (void)result;
    });

    // builtin types use std::
    BENCHMARK("is_integral_v", "uint64_t", ITERATIONS, {
        volatile bool result = std::is_integral_v<uint64_t>;
        (void)result;
    });

    BENCHMARK("is_integral_v", "int64_t", ITERATIONS, {
        volatile bool result = std::is_integral_v<int64_t>;
        (void)result;
    });
}

void benchmark_is_unsigned()
{
    std::cout << "\n=== nstd::is_unsigned_v (int128) / std::is_unsigned_v (builtin) ===\n";

    // int128 types use nstd::
    BENCHMARK("is_unsigned_v", "uint128_t", ITERATIONS, {
        volatile bool result = nstd::is_unsigned_v<uint128_t>;
        (void)result;
    });

    BENCHMARK("is_unsigned_v", "int128_t", ITERATIONS, {
        volatile bool result = nstd::is_unsigned_v<int128_t>;
        (void)result;
    });

    // builtin types use std::
    BENCHMARK("is_unsigned_v", "uint64_t", ITERATIONS, {
        volatile bool result = std::is_unsigned_v<uint64_t>;
        (void)result;
    });

    BENCHMARK("is_unsigned_v", "int64_t", ITERATIONS, {
        volatile bool result = std::is_unsigned_v<int64_t>;
        (void)result;
    });
}

void benchmark_is_signed()
{
    std::cout << "\n=== nstd::is_signed_v (int128) / std::is_signed_v (builtin) ===\n";

    // int128 types use nstd::
    BENCHMARK("is_signed_v", "uint128_t", ITERATIONS, {
        volatile bool result = nstd::is_signed_v<uint128_t>;
        (void)result;
    });

    BENCHMARK("is_signed_v", "int128_t", ITERATIONS, {
        volatile bool result = nstd::is_signed_v<int128_t>;
        (void)result;
    });

    // builtin types use std::
    BENCHMARK("is_signed_v", "uint64_t", ITERATIONS, {
        volatile bool result = std::is_signed_v<uint64_t>;
        (void)result;
    });

    BENCHMARK("is_signed_v", "int64_t", ITERATIONS, {
        volatile bool result = std::is_signed_v<int64_t>;
        (void)result;
    });
}

void benchmark_is_arithmetic()
{
    std::cout << "\n=== nstd::is_arithmetic_v (int128) / std::is_arithmetic_v (builtin) ===\n";

    // int128 types use nstd::
    BENCHMARK("is_arithmetic_v", "uint128_t", ITERATIONS, {
        volatile bool result = nstd::is_arithmetic_v<uint128_t>;
        (void)result;
    });

    BENCHMARK("is_arithmetic_v", "int128_t", ITERATIONS, {
        volatile bool result = nstd::is_arithmetic_v<int128_t>;
        (void)result;
    });

    // builtin types use std::
    BENCHMARK("is_arithmetic_v", "uint64_t", ITERATIONS, {
        volatile bool result = std::is_arithmetic_v<uint64_t>;
        (void)result;
    });

    BENCHMARK("is_arithmetic_v", "int64_t", ITERATIONS, {
        volatile bool result = std::is_arithmetic_v<int64_t>;
        (void)result;
    });
}

void verify_traits()
{
    std::cout << "\n=== Verification (using nstd:: for int128 types) ===\n";

    std::cout << "uint128_t (nstd::):\n";
    std::cout << "  is_integral   = " << std::boolalpha << nstd::is_integral_v<uint128_t> << "\n";
    std::cout << "  is_arithmetic = " << nstd::is_arithmetic_v<uint128_t> << "\n";
    std::cout << "  is_unsigned   = " << nstd::is_unsigned_v<uint128_t> << "\n";
    std::cout << "  is_signed     = " << nstd::is_signed_v<uint128_t> << "\n";

    std::cout << "int128_t (nstd::):\n";
    std::cout << "  is_integral   = " << nstd::is_integral_v<int128_t> << "\n";
    std::cout << "  is_arithmetic = " << nstd::is_arithmetic_v<int128_t> << "\n";
    std::cout << "  is_unsigned   = " << nstd::is_unsigned_v<int128_t> << "\n";
    std::cout << "  is_signed     = " << nstd::is_signed_v<int128_t> << "\n";

    // make_unsigned / make_signed
    std::cout << "\nnstd::make_unsigned_t<int128_t> == uint128_t: "
              << std::is_same_v<nstd::make_unsigned_t<int128_t>, uint128_t> << "\n";
    std::cout << "nstd::make_signed_t<uint128_t> == int128_t: "
              << std::is_same_v<nstd::make_signed_t<uint128_t>, int128_t> << "\n";

    // Builtin types (std:: works for these)
    std::cout << "\nuint64_t (std::):\n";
    std::cout << "  is_integral   = " << std::is_integral_v<uint64_t> << "\n";
    std::cout << "  is_arithmetic = " << std::is_arithmetic_v<uint64_t> << "\n";
    std::cout << "  is_unsigned   = " << std::is_unsigned_v<uint64_t> << "\n";

    std::cout << "int64_t (std::):\n";
    std::cout << "  is_integral   = " << std::is_integral_v<int64_t> << "\n";
    std::cout << "  is_arithmetic = " << std::is_arithmetic_v<int64_t> << "\n";
    std::cout << "  is_signed     = " << std::is_signed_v<int64_t> << "\n";
}

int main()
{
    std::cout << "========================================\n";
    std::cout << " int128 Type Traits Benchmarks\n";
    std::cout << "========================================\n";

    benchmark_is_integral();
    benchmark_is_unsigned();
    benchmark_is_signed();
    benchmark_is_arithmetic();
    verify_traits();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
