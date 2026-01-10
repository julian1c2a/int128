/**
 * @file uint128_cmath_benchmarks.cpp
 * @brief Performance benchmarks for uint128_cmath.hpp mathematical functions
 *
 * Benchmarks:
 * - std::gcd (Greatest Common Divisor - Binary GCD/Stein's algorithm)
 * - std::lcm (Least Common Multiple)
 * - std::pow (Power function - Fast exponentiation)
 * - std::sqrt (Square root - Newton's method)
 * - std::min / std::max
 * - nstd::bezout_coeffs (Bézout coefficients)
 *
 * Compares performance with built-in types and measures CPU cycles
 */

#include "int128_base_cmath.hpp"
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
    return 0; // Fallback para arquitecturas no-x86
}
#endif

// ========================= BENCHMARK UTILITIES =========================

std::mt19937_64 rng(std::random_device{}());

template <typename Func>
void benchmark(const std::string& name, Func&& func, size_t iterations = 100000)
{
    // Warm-up
    for (size_t i = 0; i < iterations / 10; ++i) {
        func();
    }

    // Benchmark tiempo
    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < iterations; ++i) {
        func();
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double time_per_op = static_cast<double>(duration) / iterations;
    double cycles_per_op = static_cast<double>(end_cycles - start_cycles) / iterations;

    std::cout << std::left << std::setw(40) << name << std::right << std::fixed
              << std::setprecision(3) << std::setw(12) << time_per_op << " ns/op" << std::setw(12)
              << std::setprecision(1) << cycles_per_op << " cycles/op" << std::endl;
}

// ========================= BENCHMARK GCD =========================

void benchmark_gcd()
{
    using std::gcd; // Permite ADL para encontrar la sobrecarga de uint128_t
    std::cout << "\n=== std::gcd Benchmarks ===" << std::endl;

    // Test datos
    std::vector<std::pair<uint64_t, uint64_t>> test_pairs = {
        {48, 18}, {1000000007, 1000000009}, {123456789, 987654321}, {0xFFFFFFFF, 0x7FFFFFFF}};

    // Benchmark uint64_t (baseline)
    benchmark(
        "gcd(uint64_t, uint64_t)",
        [&]() {
            volatile uint64_t result = std::gcd(test_pairs[rng() % test_pairs.size()].first,
                                                test_pairs[rng() % test_pairs.size()].second);
            (void)result;
        },
        1000000);

    // Benchmark uint128_t (low values)
    benchmark(
        "gcd(uint128_t, uint128_t) - small",
        [&]() {
            auto& p = test_pairs[rng() % test_pairs.size()];
            volatile auto result = gcd(uint128_t(0, p.first), uint128_t(0, p.second));
            (void)result;
        },
        1000000);

    // Benchmark uint128_t (large values)
    uint128_t large1(0x123456789ABCDEFULL, 0xFEDCBA987654321ULL);
    uint128_t large2(0x0FEDCBA987654321ULL, 0x123456789ABCDEFULL);
    benchmark(
        "gcd(uint128_t, uint128_t) - large",
        [&]() {
            volatile auto result = nstd::gcd(large1, large2);
            (void)result;
        },
        100000);

    // Benchmark casos especiales
    benchmark(
        "gcd(uint128_t, uint128_t) - potencias 2",
        [&]() {
            volatile auto result = gcd(uint128_t(1024), uint128_t(512));
            (void)result;
        },
        1000000);

    benchmark(
        "gcd(uint128_t, uint128_t) - primos",
        [&]() {
            volatile auto result = gcd(uint128_t(1000000007), uint128_t(1000000009));
            (void)result;
        },
        500000);
}

// ========================= BENCHMARK LCM =========================

void benchmark_lcm()
{
    using std::lcm; // Permite ADL para encontrar la sobrecarga de uint128_t
    std::cout << "\n=== std::lcm Benchmarks ===" << std::endl;

    // Benchmark uint64_t (baseline)
    benchmark(
        "lcm(uint64_t, uint64_t)",
        [&]() {
            volatile uint64_t result = std::lcm(48ULL, 18ULL);
            (void)result;
        },
        1000000);

    // Benchmark uint128_t
    benchmark(
        "lcm(uint128_t, uint128_t) - small",
        [&]() {
            volatile auto result = lcm(uint128_t(48), uint128_t(18));
            (void)result;
        },
        1000000);

    benchmark(
        "lcm(uint128_t, uint128_t) - medium",
        [&]() {
            volatile auto result = lcm(uint128_t(123456), uint128_t(987654));
            (void)result;
        },
        500000);

    benchmark(
        "lcm(uint128_t, uint128_t) - large",
        [&]() {
            volatile auto result = lcm(uint128_t(0, 0xFFFFFFFF), uint128_t(0, 0x7FFFFFFF));
            (void)result;
        },
        100000);
}

// ========================= BENCHMARK POW =========================

void benchmark_pow()
{
    using std::pow; // Permite ADL para encontrar la sobrecarga de uint128_t
    std::cout << "\n=== std::pow Benchmarks ===" << std::endl;

    // Benchmark uint64_t con pow manual
    auto pow_uint64 = [](uint64_t base, uint64_t exp) -> uint64_t {
        uint64_t result = 1;
        uint64_t b = base;
        while (exp > 0) {
            if (exp & 1)
                result *= b;
            b *= b;
            exp >>= 1;
        }
        return result;
    };

    benchmark(
        "pow(uint64_t, uint64_t) - manual",
        [&]() {
            volatile uint64_t result = pow_uint64(2, 20);
            (void)result;
        },
        1000000);

    // Benchmark uint128_t - exponentes pequeños
    benchmark(
        "pow(uint128_t, small exp)",
        [&]() {
            volatile auto result = pow(uint128_t(3), uint128_t(5));
            (void)result;
        },
        1000000);

    benchmark(
        "pow(uint128_t, medium exp)",
        [&]() {
            volatile auto result = pow(uint128_t(2), uint128_t(20));
            (void)result;
        },
        500000);

    benchmark(
        "pow(uint128_t, large exp)",
        [&]() {
            volatile auto result = pow(uint128_t(2), uint128_t(64));
            (void)result;
        },
        100000);

    // Casos especiales rápidos
    benchmark(
        "pow(uint128_t, 0) - fast path",
        [&]() {
            volatile auto result = pow(uint128_t(12345), uint128_t(0));
            (void)result;
        },
        10000000);

    benchmark(
        "pow(uint128_t, 1) - fast path",
        [&]() {
            volatile auto result = pow(uint128_t(12345), uint128_t(1));
            (void)result;
        },
        10000000);
}

// ========================= BENCHMARK SQRT =========================

void benchmark_sqrt()
{
    using std::sqrt; // Permite ADL para encontrar la sobrecarga de uint128_t
    std::cout << "\n=== std::sqrt Benchmarks ===" << std::endl;

    // Benchmark sqrt de uint64_t (usando ::sqrt de double como comparación)
    // Nota: usar :: en lugar de std:: para evitar ambigüedad con uint128_t overload
    benchmark(
        "::sqrt(double) - baseline",
        [&]() {
            volatile double result = ::sqrt(123456789.0);
            (void)result;
        },
        10000000);

    // Benchmark uint128_t - valores pequeños
    benchmark(
        "sqrt(uint128_t) - small",
        [&]() {
            volatile auto result = sqrt(uint128_t(144)); // ADL encuentra nstd::sqrt(uint128_t)
            (void)result;
        },
        1000000);

    benchmark(
        "sqrt(uint128_t) - medium",
        [&]() {
            volatile auto result = sqrt(uint128_t(0, 100000000));
            (void)result;
        },
        100000);

    benchmark(
        "sqrt(uint128_t) - large",
        [&]() {
            volatile auto result = sqrt(uint128_t(1, 0)); // 2^64
            (void)result;
        },
        50000);

    benchmark(
        "sqrt(uint128_t) - very large",
        [&]() {
            volatile auto result = sqrt(uint128_t(0x123456789ABCDEFULL, 0xFEDCBA987654321ULL));
            (void)result;
        },
        50000);

    // Casos especiales rápidos
    benchmark(
        "sqrt(uint128_t) - 0 fast path",
        [&]() {
            volatile auto result = sqrt(uint128_t(0));
            (void)result;
        },
        10000000);

    benchmark(
        "sqrt(uint128_t) - 1 fast path",
        [&]() {
            volatile auto result = sqrt(uint128_t(1));
            (void)result;
        },
        10000000);
}

// ========================= BENCHMARK MIN/MAX =========================

void benchmark_min_max()
{
    using std::max; // Permite ADL para encontrar las sobrecargas de uint128_t
    using std::min;
    std::cout << "\n=== std::min / std::max Benchmarks ===" << std::endl;

    // Benchmark uint64_t (baseline)
    benchmark(
        "std::min(uint64_t, uint64_t)",
        [&]() {
            volatile uint64_t result = std::min(123456789ULL, 987654321ULL);
            (void)result;
        },
        10000000);

    benchmark(
        "std::max(uint64_t, uint64_t)",
        [&]() {
            volatile uint64_t result = std::max(123456789ULL, 987654321ULL);
            (void)result;
        },
        10000000);

    // Benchmark uint128_t
    benchmark(
        "min(uint128_t, uint128_t)",
        [&]() {
            volatile auto result = min(uint128_t(0, 123456789), uint128_t(0, 987654321));
            (void)result;
        },
        10000000);

    benchmark(
        "max(uint128_t, uint128_t)",
        [&]() {
            volatile auto result = max(uint128_t(0, 123456789), uint128_t(0, 987654321));
            (void)result;
        },
        10000000);

    // Con valores grandes (requiere comparación de high)
    benchmark(
        "min(uint128_t, uint128_t) - large",
        [&]() {
            volatile auto result = min(uint128_t(1, 0), uint128_t(0, 0xFFFFFFFFFFFFFFFFULL));
            (void)result;
        },
        10000000);

    benchmark(
        "max(uint128_t, uint128_t) - large",
        [&]() {
            volatile auto result = max(uint128_t(1, 0), uint128_t(0, 0xFFFFFFFFFFFFFFFFULL));
            (void)result;
        },
        10000000);
}

// ========================= BENCHMARK BEZOUT =========================

void benchmark_bezout()
{
    using nstd::bezout_coeffs; // Permite ADL para encontrar la sobrecarga de uint128_t
    std::cout << "\n=== nstd::bezout_coeffs Benchmarks ===" << std::endl;

    benchmark(
        "bezout_coeffs(small, small)",
        [&]() {
            volatile auto result = bezout_coeffs(uint128_t(48), uint128_t(18));
            (void)result;
        },
        50000);

    benchmark(
        "bezout_coeffs(medium, medium)",
        [&]() {
            volatile auto result = bezout_coeffs(uint128_t(12345), uint128_t(67890));
            (void)result;
        },
        10000);

    benchmark(
        "bezout_coeffs(large, large)",
        [&]() {
            volatile auto result =
                bezout_coeffs(uint128_t(0, 1000000007), uint128_t(0, 1000000009));
            (void)result;
        },
        5000);

    benchmark(
        "bezout_coeffs(primes, primes)",
        [&]() {
            volatile auto result = bezout_coeffs(uint128_t(17), uint128_t(19));
            (void)result;
        },
        50000);
}

// ========================= COMBINED OPERATIONS =========================

void benchmark_combined()
{
    using std::gcd;
    using std::lcm;
    using std::max;
    using std::min;
    using std::pow;
    using std::sqrt;
    std::cout << "\n=== Combined Operations ===" << std::endl;

    // Operación típica: calcular gcd y luego lcm
    benchmark(
        "gcd + lcm sequence",
        [&]() {
            uint128_t a(123456);
            uint128_t b(789012);
            volatile auto g = nstd::gcd(a, b);
            volatile auto l = nstd::lcm(a, b);
            (void)g;
            (void)l;
        },
        500000);

    // Operación típica: pow y luego sqrt
    benchmark(
        "pow + sqrt sequence",
        [&]() {
            uint128_t base(10);
            auto powered = nstd::pow(base, uint128_t(6));
            auto rooted = sqrt(powered);
            volatile uint128_t dummy = rooted; // Prevenir optimización completa
            (void)dummy;
        },
        100000);

    // Encontrar rango usando min/max múltiples
    benchmark(
        "min/max chain (4 values)",
        [&]() {
            uint128_t a(100), b(200), c(150), d(175);
            volatile auto min_val = nstd::min(nstd::min(a, b), nstd::min(c, d));
            volatile auto max_val = nstd::max(nstd::max(a, b), nstd::max(c, d));
            (void)min_val;
            (void)max_val;
        },
        5000000);
}

// ========================= MAIN =========================

int main()
{
    std::cout << "╔================================================================╗" << std::endl;
    std::cout << "║  UINT128_CMATH.HPP - PERFORMANCE BENCHMARKS                   ║" << std::endl;
    std::cout << "╚================================================================╝" << std::endl;
    std::cout << "\nMeasuring time (nanoseconds) and CPU cycles per operation\n" << std::endl;

    benchmark_gcd();
    benchmark_lcm();
    benchmark_pow();
    benchmark_sqrt();
    benchmark_min_max();
    benchmark_bezout();
    benchmark_combined();

    std::cout << "\n╔================================================================╗"
              << std::endl;
    std::cout << "║  PERFORMANCE NOTES                                             ║" << std::endl;
    std::cout << "╚================================================================╝" << std::endl;
    std::cout << "* gcd: Binary GCD (Stein's) - O(log(min(a,b)))" << std::endl;
    std::cout << "* lcm: Optimized with gcd - O(log(min(a,b)))" << std::endl;
    std::cout << "* pow: Fast exponentiation - O(log(exp))" << std::endl;
    std::cout << "* sqrt: Newton's method - converges quadratically" << std::endl;
    std::cout << "* min/max: Single comparison - O(1)" << std::endl;
    std::cout << "* bezout_coeffs: Simple search (demo) - O(n)" << std::endl;
    std::cout << "\nNote: CPU cycles may show as 0 or very low for Intel compiler" << std::endl;
    std::cout << "      due to aggressive optimizations eliminating operations." << std::endl;

    return 0;
}
