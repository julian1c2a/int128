/*
 * Benchmarks extraídos para int128_concepts.hpp
 * Mediciones de rendimiento de conceptos C++20 para int128_t
 */

#include "../include/int128/int128_algorithm.hpp"
#include "../include/int128/int128_concepts.hpp"
#include "../include/int128/int128_t.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace nstd;
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
    std::cout << std::left << std::setw(45) << name;

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
// BENCHMARK: Verificación de conceptos en tiempo de compilación
// =============================================================================
void benchmark_concept_verification()
{
    std::cout << "\n=== BENCHMARK: Concept Verification (compile-time) ===\n";

    // Los conceptos se verifican en tiempo de compilación, así que el overhead
    // en tiempo de ejecución debe ser cercano a cero.
    constexpr uint64_t iterations = 100'000'000;
    volatile bool sink = false;

    auto result = benchmark_cycles(
        [&sink]() {
            // Estas evaluaciones deberían optimizarse completamente
            sink = int128_concepts::int128_convertible<int32_t>;
            sink = int128_concepts::int128_compatible<double>;
            sink = int128_concepts::int128_bitwise_compatible<int64_t>;
            sink = int128_concepts::int128_signed_compatible<int32_t>;
            sink = int128_concepts::valid_shift_type<int>;
            sink = int128_concepts::bit_index_type<unsigned int>;
        },
        iterations);

    print_result("Concept verification (6 concepts)", result);
}

// =============================================================================
// BENCHMARK: Uso de int128_function
// =============================================================================
void benchmark_int128_function()
{
    std::cout << "\n=== BENCHMARK: int128_function ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Función que satisface el concepto
    auto func = [](int128_t i) constexpr { return i + 1; };

    int128_t value = 42;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = func(value);
            value = sink;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Function call with int128_t", result);
}

// =============================================================================
// BENCHMARK: Uso de int128_predicate
// =============================================================================
void benchmark_int128_predicate()
{
    std::cout << "\n=== BENCHMARK: int128_predicate ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Predicado simple - verificar si es positivo
    auto is_positive = [](int128_t i) constexpr { return i > 0; };

    int128_t value = 42;
    volatile bool sink = false;

    auto result = benchmark_cycles(
        [&]() {
            sink = is_positive(value);
            value = -value; // Alternar entre positivo y negativo
        },
        iterations);

    print_result("Predicate evaluation (is_positive)", result);
}

// =============================================================================
// BENCHMARK: Uso de int128_binary_operation
// =============================================================================
void benchmark_int128_binary_operation()
{
    std::cout << "\n=== BENCHMARK: int128_binary_operation ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Operación binaria
    auto add_op = [](int128_t a, int128_t b) constexpr { return a + b; };

    int128_t a = 100;
    int128_t b = -200;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = add_op(a, b);
            a = sink + 1;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Binary operation (add)", result);
}

// =============================================================================
// BENCHMARK: Uso de int128_container con std::vector
// =============================================================================
void benchmark_int128_container()
{
    std::cout << "\n=== BENCHMARK: int128_container ===\n";

    constexpr uint64_t iterations = 1'000'000;

    std::vector<int128_t> vec;
    vec.reserve(100);

    auto result = benchmark_cycles(
        [&]() {
            vec.clear();
            for (int i = -50; i < 50; ++i) {
                vec.push_back(int128_t(i));
            }
        },
        iterations);

    print_result("Vector operations (100 elements)", result);
}

// =============================================================================
// BENCHMARK: Uso de std::find_if con predicado
// =============================================================================
void benchmark_find_with_predicate()
{
    std::cout << "\n=== BENCHMARK: std::find_if with predicate ===\n";

    constexpr uint64_t iterations = 1'000'000;

    std::vector<int128_t> vec;
    for (int i = -500; i < 500; ++i) {
        vec.push_back(int128_t(i));
    }

    auto is_target = [](int128_t i) { return i == int128_t(0); };

    auto result = benchmark_cycles(
        [&]() {
            auto it = std::find_if(vec.begin(), vec.end(), is_target);
            volatile bool found = (it != vec.end());
            (void)found;
        },
        iterations);

    print_result("find_if (1000 elements)", result);
}

// =============================================================================
// BENCHMARK: Uso de std::transform con función
// =============================================================================
void benchmark_transform_with_function()
{
    std::cout << "\n=== BENCHMARK: std::transform with function ===\n";

    constexpr uint64_t iterations = 100'000;

    std::vector<int128_t> input;
    std::vector<int128_t> output;

    for (int i = -500; i < 500; ++i) {
        input.push_back(int128_t(i));
    }
    output.resize(1000);

    auto negate_value = [](int128_t i) { return -i; };

    auto result = benchmark_cycles(
        [&]() { std::transform(input.begin(), input.end(), output.begin(), negate_value); },
        iterations);

    print_result("transform (1000 elements)", result);
}

// =============================================================================
// BENCHMARK: Comparación con diferentes tipos compatibles (signados)
// =============================================================================
void benchmark_signed_compatible_operations()
{
    std::cout << "\n=== BENCHMARK: Signed compatible operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    int128_t i128 = 1000;
    int64_t i64 = -500;
    int32_t i32 = 250;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = i128 + i64;
            sink = sink + i32;
            i128 = sink - 1;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Mixed signed type operations", result);
}

// =============================================================================
// BENCHMARK: Operaciones bitwise con tipos compatibles
// =============================================================================
void benchmark_bitwise_operations()
{
    std::cout << "\n=== BENCHMARK: Bitwise operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    int128_t i128 = int128_t(0xFFFFFFFFFFFFFFFFull);
    int64_t mask = 0xFF00FF00FF00FF00ll;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = i128 & mask;
            sink = sink | int128_t(0x0F);
            sink = sink ^ int128_t(0xF0);
            i128 = sink + 1;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Bitwise operations (AND, OR, XOR)", result);
}

// =============================================================================
// BENCHMARK: Operaciones de shift (con manejo de signo)
// =============================================================================
void benchmark_shift_operations()
{
    std::cout << "\n=== BENCHMARK: Shift operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    int128_t value = int128_t(1) << 64;
    int shift = 8;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = value << shift;
            sink = sink >> shift; // Arithmetic shift para tipos signados
            value = sink + 1;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Shift operations (left and right)", result);
}

// =============================================================================
// BENCHMARK: Uso de std::accumulate con reduce operation
// =============================================================================
void benchmark_accumulate_with_reduce()
{
    std::cout << "\n=== BENCHMARK: std::accumulate with reduce ===\n";

    constexpr uint64_t iterations = 100'000;

    std::vector<int128_t> vec;
    for (int i = -500; i < 500; ++i) {
        vec.push_back(int128_t(i));
    }

    auto result = benchmark_cycles(
        [&]() {
            int128_t sum = std::accumulate(vec.begin(), vec.end(), int128_t(0));
            volatile int128_t dummy = sum;
            (void)dummy;
        },
        iterations);

    print_result("accumulate (1000 elements)", result);
}

// =============================================================================
// BENCHMARK: Operaciones con valores negativos
// =============================================================================
void benchmark_negative_value_operations()
{
    std::cout << "\n=== BENCHMARK: Negative value operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    int128_t positive = 1000;
    int128_t negative = -1000;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = positive + negative;
            sink = -sink;
            positive = sink > 0 ? sink : -sink;
            negative = -positive;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Operations with negative values", result);
}

// =============================================================================
// BENCHMARK: Operación segura (safe_operation)
// =============================================================================
void benchmark_safe_operation()
{
    std::cout << "\n=== BENCHMARK: Safe operation ===\n";

    constexpr uint64_t iterations = 50'000'000;

    auto safe_multiply = [](int128_t a, int128_t b) -> int128_t { return a * b; };

    int128_t a = 1000;
    int128_t b = -2;
    int128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = safe_multiply(a, b);
            a = (sink / 1000) + 1000;
        },
        iterations);

    volatile int128_t dummy = sink;
    (void)dummy;
    print_result("Safe multiply operation", result);
}

// =============================================================================
// BENCHMARK: Comparaciones (respetando signo)
// =============================================================================
void benchmark_comparison_operations()
{
    std::cout << "\n=== BENCHMARK: Comparison operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    int128_t positive = 100;
    int128_t negative = -100;
    int128_t zero = 0;
    volatile bool sink = false;

    auto result = benchmark_cycles(
        [&]() {
            sink = positive > negative;
            sink = negative < zero;
            sink = zero == int128_t(0);
            sink = positive >= zero;
            sink = negative <= zero;
            positive++;
            negative--;
        },
        iterations);

    print_result("Comparison operations (5 comparisons)", result);
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  INT128 CONCEPTS - BENCHMARKS\n";
    std::cout << "========================================\n";

    try {
        benchmark_concept_verification();
        benchmark_int128_function();
        benchmark_int128_predicate();
        benchmark_int128_binary_operation();
        benchmark_int128_container();
        benchmark_find_with_predicate();
        benchmark_transform_with_function();
        benchmark_signed_compatible_operations();
        benchmark_bitwise_operations();
        benchmark_shift_operations();
        benchmark_accumulate_with_reduce();
        benchmark_negative_value_operations();
        benchmark_safe_operation();
        benchmark_comparison_operations();
    } catch (const std::exception& e) {
        std::cerr << "\n[FAIL] Exception caught: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << "  [OK] BENCHMARKS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}
