/*
 * Benchmarks extraídos para int128_limits.hpp
 * Mediciones de rendimiento de std::numeric_limits<int128_t>
 */

#include "../include/int128/int128_limits.hpp"
#include "../include/int128/int128_t.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

// =============================================================================
// UTILITIES
// =============================================================================

#if defined(__x86_64__) || defined(_M_X64)
inline uint64_t RDTSC()
{
#ifdef _MSC_VER
    return __rdtsc();
#else
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#endif
}
#else
inline uint64_t RDTSC()
{
    return 0; // No hay RDTSC en ARM
}
#endif

struct BenchmarkResult {
    uint64_t cycles;
    double elapsed_ms;
    uint64_t iterations;
};

template <typename Func> BenchmarkResult benchmark_cycles(Func&& func, uint64_t iterations)
{
    BenchmarkResult result{};
    result.iterations = iterations;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start = RDTSC();

    for (uint64_t i = 0; i < iterations; ++i) {
        func();
    }

    uint64_t end = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    result.cycles = end - start;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    result.elapsed_ms = duration.count() / 1000.0;

    return result;
}

void print_result(const std::string& name, const BenchmarkResult& result)
{
    std::cout << std::left << std::setw(40) << name;

    if (result.iterations == 0) {
        std::cout << "SKIP\n";
        return;
    }

    std::cout << std::right << std::setw(15) << result.iterations << " iters, " << std::setw(12)
              << result.cycles << " cycles";

    if (result.cycles > 0) {
        double cycles_per_iter = static_cast<double>(result.cycles) / result.iterations;
        std::cout << " (" << std::fixed << std::setprecision(2) << cycles_per_iter << " c/iter)";
    }

    std::cout << ", " << std::fixed << std::setprecision(3) << result.elapsed_ms << " ms\n";
}

// =============================================================================
// BENCHMARK: Acceso a propiedades booleanas
// =============================================================================
void benchmark_bool_properties()
{
    std::cout << "\n=== BENCHMARK: Acceso a propiedades booleanas ===\n";

    constexpr uint64_t iterations = 10'000'000;
    volatile bool sink = false;

    auto result = benchmark_cycles(
        [&sink]() {
            sink = std::numeric_limits<int128_t>::is_specialized;
            sink = std::numeric_limits<int128_t>::is_signed;
            sink = std::numeric_limits<int128_t>::is_integer;
            sink = std::numeric_limits<int128_t>::is_exact;
            sink = std::numeric_limits<int128_t>::has_infinity;
            sink = std::numeric_limits<int128_t>::has_quiet_NaN;
            sink = std::numeric_limits<int128_t>::is_bounded;
            sink = std::numeric_limits<int128_t>::is_modulo;
        },
        iterations);

    print_result("Bool properties access (8 props)", result);
}

// =============================================================================
// BENCHMARK: Acceso a propiedades numéricas
// =============================================================================
void benchmark_numeric_properties()
{
    std::cout << "\n=== BENCHMARK: Acceso a propiedades numéricas ===\n";

    constexpr uint64_t iterations = 10'000'000;
    volatile int sink = 0;

    auto result = benchmark_cycles(
        [&sink]() {
            sink = std::numeric_limits<int128_t>::digits;
            sink = std::numeric_limits<int128_t>::digits10;
            sink = std::numeric_limits<int128_t>::radix;
            sink = std::numeric_limits<int128_t>::max_digits10;
        },
        iterations);

    print_result("Numeric properties access (4 props)", result);
}

// =============================================================================
// BENCHMARK: min()
// =============================================================================
void benchmark_min()
{
    std::cout << "\n=== BENCHMARK: min() ===\n";

    constexpr uint64_t iterations = 10'000'000;
    int128_t sink = 0;

    auto result =
        benchmark_cycles([&sink]() { sink = std::numeric_limits<int128_t>::min(); }, iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("min() access", result);
}

// =============================================================================
// BENCHMARK: max()
// =============================================================================
void benchmark_max()
{
    std::cout << "\n=== BENCHMARK: max() ===\n";

    constexpr uint64_t iterations = 10'000'000;
    int128_t sink = 0;

    auto result =
        benchmark_cycles([&sink]() { sink = std::numeric_limits<int128_t>::max(); }, iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("max() access", result);
}

// =============================================================================
// BENCHMARK: lowest()
// =============================================================================
void benchmark_lowest()
{
    std::cout << "\n=== BENCHMARK: lowest() ===\n";

    constexpr uint64_t iterations = 10'000'000;
    int128_t sink = 0;

    auto result =
        benchmark_cycles([&sink]() { sink = std::numeric_limits<int128_t>::lowest(); }, iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("lowest() access", result);
}

// =============================================================================
// BENCHMARK: epsilon()
// =============================================================================
void benchmark_epsilon()
{
    std::cout << "\n=== BENCHMARK: epsilon() ===\n";

    constexpr uint64_t iterations = 10'000'000;
    int128_t sink = 0;

    auto result = benchmark_cycles([&sink]() { sink = std::numeric_limits<int128_t>::epsilon(); },
                                   iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("epsilon() access", result);
}

// =============================================================================
// BENCHMARK: round_error()
// =============================================================================
void benchmark_round_error()
{
    std::cout << "\n=== BENCHMARK: round_error() ===\n";

    constexpr uint64_t iterations = 10'000'000;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&sink]() { sink = std::numeric_limits<int128_t>::round_error(); }, iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("round_error() access", result);
}

// =============================================================================
// BENCHMARK: Comparación con int64_t
// =============================================================================
void benchmark_comparison_int64()
{
    std::cout << "\n=== BENCHMARK: Comparación con int64_t ===\n";

    constexpr uint64_t iterations = 10'000'000;

    // int64_t
    {
        volatile int64_t sink = 0;
        auto result =
            benchmark_cycles([&sink]() { sink = std::numeric_limits<int64_t>::max(); }, iterations);
        print_result("int64_t::max() access", result);
    }

    // int128_t
    {
        int128_t sink = 0;
        auto result = benchmark_cycles([&sink]() { sink = std::numeric_limits<int128_t>::max(); },
                                       iterations);
        volatile int128_t dummy = sink;
        (void)dummy;
        print_result("int128_t::max() access", result);
    }
}

// =============================================================================
// BENCHMARK: Constexpr evaluation
// =============================================================================
void benchmark_constexpr_evaluation()
{
    std::cout << "\n=== BENCHMARK: Constexpr evaluation ===\n";

    constexpr uint64_t iterations = 10'000'000;

    // Evaluación en tiempo de compilación
    constexpr int128_t compile_time_max = std::numeric_limits<int128_t>::max();
    constexpr int128_t compile_time_min = std::numeric_limits<int128_t>::min();
    constexpr int compile_time_digits = std::numeric_limits<int128_t>::digits;

    // Uso en runtime (debería ser extremadamente rápido)
    int128_t sink1 = 0;
    int128_t sink2 = 0;
    int sink3 = 0;

    auto result = benchmark_cycles(
        [&sink1, &sink2, &sink3, compile_time_max, compile_time_min, compile_time_digits]() {
            sink1 = compile_time_max;
            sink2 = compile_time_min;
            sink3 = compile_time_digits;
        },
        iterations);

    volatile int128_t dummy1 = sink1;
    volatile int128_t dummy2 = sink2;
    volatile int dummy3 = sink3;
    (void)dummy1;
    (void)dummy2;
    (void)dummy3;

    print_result("Constexpr value usage (3 values)", result);
}

// =============================================================================
// BENCHMARK: Overflow detection con max()
// =============================================================================
void benchmark_overflow_detection()
{
    std::cout << "\n=== BENCHMARK: Overflow detection ===\n";

    constexpr uint64_t iterations = 1'000'000;
    volatile bool sink = false;

    int128_t test_val = std::numeric_limits<int128_t>::max() - int128_t(100);

    auto result = benchmark_cycles(
        [&sink, test_val]() {
            int128_t result = test_val + int128_t(50);
            sink = (result > test_val); // Verificar no overflow
        },
        iterations);

    print_result("Overflow check using max()", result);
}

// =============================================================================
// BENCHMARK: Range validation
// =============================================================================
void benchmark_range_validation()
{
    std::cout << "\n=== BENCHMARK: Range validation ===\n";

    constexpr uint64_t iterations = 1'000'000;
    volatile bool sink = false;

    int128_t test_val = int128_t(-12345);

    auto result = benchmark_cycles(
        [&sink, test_val]() {
            bool in_range = (test_val >= std::numeric_limits<int128_t>::min() &&
                             test_val <= std::numeric_limits<int128_t>::max());
            sink = in_range;
        },
        iterations);

    print_result("Range validation (min/max)", result);
}

// =============================================================================
// BENCHMARK: Signed-specific operations
// =============================================================================
void benchmark_signed_operations()
{
    std::cout << "\n=== BENCHMARK: Operaciones específicas de signed ===\n";

    constexpr uint64_t iterations = 1'000'000;
    volatile bool sink = false;

    int128_t max_val = std::numeric_limits<int128_t>::max();
    int128_t min_val = std::numeric_limits<int128_t>::min();

    auto result = benchmark_cycles(
        [&sink, max_val, min_val]() {
            // Verificar que min es negativo y max es positivo
            sink = (min_val < int128_t(0)) && (max_val > int128_t(0));
            // Verificar simetría aproximada
            int128_t abs_min = -min_val;
            sink = (abs_min - max_val == int128_t(1));
        },
        iterations);

    print_result("Signed property validation", result);
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  BENCHMARKS: int128_limits.hpp\n";
    std::cout << "========================================\n";

    benchmark_bool_properties();
    benchmark_numeric_properties();
    benchmark_min();
    benchmark_max();
    benchmark_lowest();
    benchmark_epsilon();
    benchmark_round_error();
    benchmark_comparison_int64();
    benchmark_constexpr_evaluation();
    benchmark_overflow_detection();
    benchmark_range_validation();
    benchmark_signed_operations();

    std::cout << "\n========================================\n";
    std::cout << "  BENCHMARKS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}
