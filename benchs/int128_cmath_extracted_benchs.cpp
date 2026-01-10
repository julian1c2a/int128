// Performance benchmarks for int128_cmath.hpp
// Measures time (nanoseconds) and CPU cycles (rdtsc) for all mathematical functions
// Tests with positive, negative, and mixed-sign values

#include "int128_base_cmath.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace nstd;

// CPU cycle counter (rdtsc)
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#include <intrin.h>
inline uint64_t rdtsc()
{
    return __rdtsc();
}
#elif defined(__GNUC__)
inline uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#else
inline uint64_t rdtsc()
{
    return 0;
}
#endif

// Benchmark result structure
struct BenchResult {
    std::string name;
    double time_ns;
    uint64_t cycles;
    std::string category;
};

std::vector<BenchResult> results;

// Benchmark template
template <typename Func>
BenchResult benchmark(const std::string& name, const std::string& category, Func f,
                      int iterations = 10000)
{
    // Warmup
    for (int i = 0; i < 100; ++i)
        f();

    // Time measurement
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    double time_ns =
        std::chrono::duration<double, std::nano>(end_time - start_time).count() / iterations;

    // CPU cycles measurement
    uint64_t start_cycles = rdtsc();
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    uint64_t end_cycles = rdtsc();
    uint64_t cycles = (end_cycles - start_cycles) / iterations;

    return {name, time_ns, cycles, category};
}

// Global sink to prevent optimization (not volatile - C++20 respects side effects)
int128_t sink;

void print_header()
{
    std::cout << "\n╔==========================================================================╗\n";
    std::cout << "║  INT128_CMATH.HPP - PERFORMANCE BENCHMARKS                               ║\n";
    std::cout << "╚==========================================================================╝\n";

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    std::cout << "Compiler: GCC " << __VERSION__ << "\n";
#elif defined(__clang__)
    std::cout << "Compiler: Clang " << __clang_version__ << "\n";
#elif defined(__INTEL_COMPILER)
    std::cout << "Compiler: Intel " << __INTEL_COMPILER << "\n";
#elif defined(_MSC_VER)
    std::cout << "Compiler: MSVC " << _MSC_VER << "\n";
#endif

    std::cout << "CPU Cycles measured with RDTSC\n";
    std::cout << "Time measured with std::chrono::high_resolution_clock\n\n";
}

void print_results()
{
    std::cout << "\n===========================================================================\n";
    std::cout << std::left << std::setw(50) << "Operation" << std::right << std::setw(12)
              << "Time (ns)" << std::setw(12) << "CPU Cycles" << "\n";
    std::cout << "===========================================================================\n";

    std::string last_category;
    for (const auto& r : results) {
        if (r.category != last_category) {
            std::cout << "\n--- " << r.category << " ---\n";
            last_category = r.category;
        }
        std::cout << std::left << std::setw(50) << r.name << std::right << std::setw(12)
                  << std::fixed << std::setprecision(1) << r.time_ns << std::setw(12) << r.cycles
                  << "\n";
    }
    std::cout << "===========================================================================\n";
}

// ============================================================================
// GCD Benchmarks
// ============================================================================
void benchmark_gcd()
{
    // Small positive values
    results.push_back(benchmark("gcd(12, 18) - small positive", "GCD",
                                []() { sink = nstd::gcd(int128_t(12), int128_t(18)); }));

    // Large positive values
    results.push_back(benchmark("gcd(1000000000, 500000000) - large positive", "GCD", []() {
        sink = nstd::gcd(int128_t(1000000000LL), int128_t(500000000LL));
    }));

    // Negative inputs (various combinations)
    results.push_back(benchmark("gcd(-48, 18) - negative first", "GCD",
                                []() { sink = nstd::gcd(int128_t(-48), int128_t(18)); }));

    results.push_back(benchmark("gcd(48, -18) - negative second", "GCD",
                                []() { sink = nstd::gcd(int128_t(48), int128_t(-18)); }));

    results.push_back(benchmark("gcd(-48, -18) - both negative", "GCD",
                                []() { sink = nstd::gcd(int128_t(-48), int128_t(-18)); }));

    // Co-primes (worst case for GCD)
    results.push_back(benchmark("gcd(999983, 1000003) - co-primes", "GCD",
                                []() { sink = nstd::gcd(int128_t(999983), int128_t(1000003)); }));

    // Baseline: int64_t for comparison
    int64_t sink64 = 0;
    results.push_back(benchmark("gcd(12, 18) - int64_t baseline", "GCD",
                                [&sink64]() { sink64 = std::gcd(int64_t(12), int64_t(18)); }));
}

// ============================================================================
// LCM Benchmarks
// ============================================================================
void benchmark_lcm()
{
    // Small values
    results.push_back(benchmark("lcm(12, 18) - small positive", "LCM",
                                []() { sink = nstd::lcm(int128_t(12), int128_t(18)); }));

    // Large values
    results.push_back(benchmark("lcm(1000, 2000) - medium", "LCM",
                                []() { sink = nstd::lcm(int128_t(1000), int128_t(2000)); }));

    // Negative inputs
    results.push_back(benchmark("lcm(-12, 18) - negative first", "LCM",
                                []() { sink = nstd::lcm(int128_t(-12), int128_t(18)); }));

    results.push_back(benchmark("lcm(-12, -18) - both negative", "LCM",
                                []() { sink = nstd::lcm(int128_t(-12), int128_t(-18)); }));

    // Baseline: int64_t
    int64_t sink64 = 0;
    results.push_back(benchmark("lcm(12, 18) - int64_t baseline", "LCM",
                                [&sink64]() { sink64 = std::lcm(int64_t(12), int64_t(18)); }));
}

// ============================================================================
// ABS Benchmarks
// ============================================================================
void benchmark_abs()
{
    // Positive value (no-op)
    results.push_back(
        benchmark("abs(42) - positive", "ABS", []() { sink = nstd::abs(int128_t(42)); }));

    // Negative value (requires negation)
    results.push_back(
        benchmark("abs(-42) - negative", "ABS", []() { sink = nstd::abs(int128_t(-42)); }));

    // Large negative
    results.push_back(benchmark("abs(-1000000000) - large negative", "ABS",
                                []() { sink = nstd::abs(int128_t(-1000000000LL)); }));

    // Zero
    results.push_back(benchmark("abs(0) - zero", "ABS", []() { sink = nstd::abs(int128_t(0)); }));

    // Baseline: int64_t
    int64_t sink64 = 0;
    results.push_back(benchmark("abs(-42) - int64_t baseline", "ABS",
                                [&sink64]() { sink64 = std::abs(int64_t(-42)); }));
}

// ============================================================================
// POW Benchmarks
// ============================================================================
void benchmark_pow()
{
    // Small exponents
    results.push_back(benchmark("pow(2, 3) - small positive base", "POW",
                                []() { sink = nstd::pow(int128_t(2), int128_t(3)); }));

    results.push_back(benchmark("pow(2, 10) - medium exponent", "POW",
                                []() { sink = nstd::pow(int128_t(2), int128_t(10)); }));

    // Negative base (odd exponent)
    results.push_back(benchmark("pow(-2, 3) - negative base, odd exp", "POW",
                                []() { sink = nstd::pow(int128_t(-2), int128_t(3)); }));

    // Negative base (even exponent)
    results.push_back(benchmark("pow(-2, 4) - negative base, even exp", "POW",
                                []() { sink = nstd::pow(int128_t(-2), int128_t(4)); }));

    // Fast paths
    results.push_back(benchmark("pow(n, 0) - exponent zero", "POW",
                                []() { sink = nstd::pow(int128_t(123), int128_t(0)); }));

    results.push_back(benchmark("pow(n, 1) - exponent one", "POW",
                                []() { sink = nstd::pow(int128_t(123), int128_t(1)); }));

    results.push_back(benchmark("pow(1, 100) - base one", "POW",
                                []() { sink = nstd::pow(int128_t(1), int128_t(100)); }));
}

// ============================================================================
// SQRT Benchmarks
// ============================================================================
void benchmark_sqrt()
{
    // Perfect squares
    results.push_back(
        benchmark("sqrt(4) - small perfect", "SQRT", []() { sink = nstd::sqrt(int128_t(4)); }));

    results.push_back(benchmark("sqrt(100) - medium perfect", "SQRT",
                                []() { sink = nstd::sqrt(int128_t(100)); }));

    results.push_back(benchmark("sqrt(10000) - large perfect", "SQRT",
                                []() { sink = nstd::sqrt(int128_t(10000)); }));

    // Non-perfect squares
    results.push_back(
        benchmark("sqrt(99) - non-perfect", "SQRT", []() { sink = nstd::sqrt(int128_t(99)); }));

    results.push_back(benchmark("sqrt(1000000) - large non-perfect", "SQRT",
                                []() { sink = nstd::sqrt(int128_t(1000000)); }));

    // Edge cases
    results.push_back(
        benchmark("sqrt(0) - zero", "SQRT", []() { sink = nstd::sqrt(int128_t(0)); }));

    results.push_back(benchmark("sqrt(1) - one", "SQRT", []() { sink = nstd::sqrt(int128_t(1)); }));

    // Baseline: int64_t
    int64_t sink64 = 0;
    results.push_back(benchmark("sqrt(100) - int64_t baseline", "SQRT",
                                [&sink64]() { sink64 = static_cast<int64_t>(std::sqrt(100)); }));
}

// ============================================================================
// SIGN Benchmarks
// ============================================================================
void benchmark_sign()
{
    // Positive
    results.push_back(
        benchmark("sign(42) - positive", "SIGN", []() { sink = nstd::sign(int128_t(42)); }));

    // Negative
    results.push_back(
        benchmark("sign(-42) - negative", "SIGN", []() { sink = nstd::sign(int128_t(-42)); }));

    // Zero
    results.push_back(
        benchmark("sign(0) - zero", "SIGN", []() { sink = nstd::sign(int128_t(0)); }));

    // Large values
    results.push_back(benchmark("sign(1000000000) - large positive", "SIGN",
                                []() { sink = nstd::sign(int128_t(1000000000LL)); }));

    results.push_back(benchmark("sign(-1000000000) - large negative", "SIGN",
                                []() { sink = nstd::sign(int128_t(-1000000000LL)); }));
}

// ============================================================================
// MIN/MAX Benchmarks
// ============================================================================
void benchmark_min_max()
{
    // Positive values
    results.push_back(benchmark("min(3, 5) - small positive", "MIN/MAX",
                                []() { sink = nstd::min(int128_t(3), int128_t(5)); }));

    results.push_back(benchmark("max(3, 5) - small positive", "MIN/MAX",
                                []() { sink = nstd::max(int128_t(3), int128_t(5)); }));

    // Negative values
    results.push_back(benchmark("min(-3, 5) - mixed signs", "MIN/MAX",
                                []() { sink = nstd::min(int128_t(-3), int128_t(5)); }));

    results.push_back(benchmark("max(-3, 5) - mixed signs", "MIN/MAX",
                                []() { sink = nstd::max(int128_t(-3), int128_t(5)); }));

    results.push_back(benchmark("min(-5, -3) - both negative", "MIN/MAX",
                                []() { sink = nstd::min(int128_t(-5), int128_t(-3)); }));

    results.push_back(benchmark("max(-5, -3) - both negative", "MIN/MAX",
                                []() { sink = nstd::max(int128_t(-5), int128_t(-3)); }));

    // Equal values
    results.push_back(benchmark("min(5, 5) - equal", "MIN/MAX",
                                []() { sink = nstd::min(int128_t(5), int128_t(5)); }));

    // Baseline: int64_t
    int64_t sink64 = 0;
    results.push_back(benchmark("min(3, 5) - int64_t baseline", "MIN/MAX",
                                [&sink64]() { sink64 = std::min(int64_t(3), int64_t(5)); }));
}

// ============================================================================
// CLAMP Benchmarks
// ============================================================================
void benchmark_clamp()
{
    // Within range
    results.push_back(benchmark("clamp(5, 0, 10) - within range", "CLAMP", []() {
        sink = std::clamp(int128_t(5), int128_t(0), int128_t(10));
    }));

    // Below minimum
    results.push_back(benchmark("clamp(-5, 0, 10) - below min", "CLAMP", []() {
        sink = std::clamp(int128_t(-5), int128_t(0), int128_t(10));
    }));

    // Above maximum
    results.push_back(benchmark("clamp(15, 0, 10) - above max", "CLAMP", []() {
        sink = std::clamp(int128_t(15), int128_t(0), int128_t(10));
    }));

    // At boundaries
    results.push_back(benchmark("clamp(0, 0, 10) - at min boundary", "CLAMP", []() {
        sink = std::clamp(int128_t(0), int128_t(0), int128_t(10));
    }));

    results.push_back(benchmark("clamp(10, 0, 10) - at max boundary", "CLAMP", []() {
        sink = std::clamp(int128_t(10), int128_t(0), int128_t(10));
    }));

    // Negative range
    results.push_back(benchmark("clamp(-5, -10, -1) - negative range", "CLAMP", []() {
        sink = std::clamp(int128_t(-5), int128_t(-10), int128_t(-1));
    }));
}

// ============================================================================
// MIDPOINT Benchmarks
// ============================================================================
void benchmark_midpoint()
{
    // Positive values
    results.push_back(benchmark("midpoint(0, 10) - small positive", "MIDPOINT",
                                []() { sink = nstd::midpoint(int128_t(0), int128_t(10)); }));

    results.push_back(benchmark("midpoint(100, 200) - medium positive", "MIDPOINT",
                                []() { sink = nstd::midpoint(int128_t(100), int128_t(200)); }));

    // Negative values
    results.push_back(benchmark("midpoint(-10, 10) - across zero", "MIDPOINT",
                                []() { sink = nstd::midpoint(int128_t(-10), int128_t(10)); }));

    results.push_back(benchmark("midpoint(-20, -10) - both negative", "MIDPOINT",
                                []() { sink = nstd::midpoint(int128_t(-20), int128_t(-10)); }));

    // Large values (overflow test)
    results.push_back(benchmark("midpoint(1e9, 2e9) - large values", "MIDPOINT", []() {
        sink = nstd::midpoint(int128_t(1000000000LL), int128_t(2000000000LL));
    }));

    // Same values
    results.push_back(benchmark("midpoint(5, 5) - equal values", "MIDPOINT",
                                []() { sink = nstd::midpoint(int128_t(5), int128_t(5)); }));

    // Baseline: int64_t
    int64_t sink64 = 0;
    results.push_back(benchmark("midpoint(0, 10) - int64_t baseline", "MIDPOINT",
                                [&sink64]() { sink64 = std::midpoint(int64_t(0), int64_t(10)); }));
}

// ============================================================================
// Combined Operations Benchmarks
// ============================================================================
void benchmark_combined()
{
    // Typical workflow: gcd + lcm
    results.push_back(benchmark("gcd + lcm combined", "COMBINED", []() {
        int128_t a(48), b(18);
        sink = nstd::gcd(a, b) + nstd::lcm(a, b);
    }));

    // Absolute difference
    results.push_back(benchmark("abs(a - b) - absolute difference", "COMBINED", []() {
        int128_t a(100), b(42);
        sink = nstd::abs(a - b);
    }));

    // Sign-aware comparison
    results.push_back(benchmark("sign(a) * abs(a) - reconstruction", "COMBINED", []() {
        int128_t a(-42);
        sink = int128_t(nstd::sign(a)) * nstd::abs(a);
    }));

    // Clamped midpoint
    results.push_back(benchmark("clamp(midpoint(a,b), lo, hi)", "COMBINED", []() {
        int128_t a(0), b(100);
        sink = std::clamp(nstd::midpoint(a, b), int128_t(10), int128_t(90));
    }));
}

int main()
{
    print_header();

    std::cout << "Running benchmarks...\n";

    benchmark_gcd();
    benchmark_lcm();
    benchmark_abs();
    benchmark_pow();
    benchmark_sqrt();
    benchmark_sign();
    benchmark_min_max();
    benchmark_clamp();
    benchmark_midpoint();
    benchmark_combined();

    print_results();

    std::cout << "\n=== Performance Summary ===\n";
    std::cout << "* GCD: Binary GCD (Stein's algorithm) with sign handling - O(log(min(a,b)))\n";
    std::cout << "* LCM: Optimized formula abs(a*b)/gcd(a,b) - prevents overflow\n";
    std::cout
        << "* POW: Fast exponentiation by squaring with negative base support - O(log(exp))\n";
    std::cout << "* SQRT: Newton's method for non-negative values - O(log(n))\n";
    std::cout << "* ABS/SIGN/MIN/MAX/CLAMP: Constant time operations - O(1)\n";
    std::cout << "* MIDPOINT: Overflow-safe: a + (b-a)/2 - O(1)\n";
    std::cout << "\n=== Notes ===\n";
    std::cout << "* Negative value handling adds minimal overhead (sign checks)\n";
    std::cout << "* int64_t baselines provided for direct comparison\n";
    std::cout << "* RDTSC measurements may show 0 cycles for highly optimized operations\n";
    std::cout << "* Results vary by compiler optimization levels and CPU architecture\n";

    return 0;
}
