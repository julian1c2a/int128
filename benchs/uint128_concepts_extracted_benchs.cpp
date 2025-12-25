/*
 * Benchmarks extraídos para uint128_concepts.hpp
 * Mediciones de rendimiento de conceptos C++20 para uint128_t
 */

#include "../include/uint128/uint128_algorithm.hpp"
#include "../include/uint128/uint128_concepts.hpp"
#include "../include/uint128/uint128_t.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

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
            sink = uint128_concepts::uint128_convertible<uint32_t>;
            sink = uint128_concepts::uint128_compatible<double>;
            sink = uint128_concepts::uint128_bitwise_compatible<int64_t>;
            sink = uint128_concepts::valid_shift_type<int>;
            sink = uint128_concepts::bit_index_type<unsigned int>;
        },
        iterations);

    print_result("Concept verification (5 concepts)", result);
}

// =============================================================================
// BENCHMARK: Uso de uint128_function
// =============================================================================
void benchmark_uint128_function()
{
    std::cout << "\n=== BENCHMARK: uint128_function ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Función que satisface el concepto
    auto func = [](uint128_t u) constexpr { return u + 1; };

    uint128_t value = 42;
    uint128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = func(value);
            value = sink;
        },
        iterations);

    volatile uint128_t dummy = sink;
    (void)dummy;
    print_result("Function call with uint128_t", result);
}

// =============================================================================
// BENCHMARK: Uso de uint128_predicate
// =============================================================================
void benchmark_uint128_predicate()
{
    std::cout << "\n=== BENCHMARK: uint128_predicate ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Predicado simple
    auto is_even = [](uint128_t u) constexpr { return (u & 1) == 0; };

    uint128_t value = 42;
    volatile bool sink = false;

    auto result = benchmark_cycles(
        [&]() {
            sink = is_even(value);
            value++;
        },
        iterations);

    print_result("Predicate evaluation", result);
}

// =============================================================================
// BENCHMARK: Uso de uint128_binary_operation
// =============================================================================
void benchmark_uint128_binary_operation()
{
    std::cout << "\n=== BENCHMARK: uint128_binary_operation ===\n";

    constexpr uint64_t iterations = 50'000'000;

    // Operación binaria
    auto add_op = [](uint128_t a, uint128_t b) constexpr { return a + b; };

    uint128_t a = 100;
    uint128_t b = 200;
    uint128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = add_op(a, b);
            a = sink + 1;
        },
        iterations);

    volatile uint128_t dummy = sink;
    (void)dummy;
    print_result("Binary operation (add)", result);
}

// =============================================================================
// BENCHMARK: Uso de uint128_container con std::vector
// =============================================================================
void benchmark_uint128_container()
{
    std::cout << "\n=== BENCHMARK: uint128_container ===\n";

    constexpr uint64_t iterations = 1'000'000;

    std::vector<uint128_t> vec;
    vec.reserve(100);

    auto result = benchmark_cycles(
        [&]() {
            vec.clear();
            for (int i = 0; i < 100; ++i) {
                vec.push_back(uint128_t(i));
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

    std::vector<uint128_t> vec;
    for (int i = 0; i < 1000; ++i) {
        vec.push_back(uint128_t(i));
    }

    auto is_target = [](uint128_t u) { return u == uint128_t(500); };

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

    std::vector<uint128_t> input;
    std::vector<uint128_t> output;

    for (int i = 0; i < 1000; ++i) {
        input.push_back(uint128_t(i));
    }
    output.resize(1000);

    auto double_value = [](uint128_t u) { return u * 2; };

    auto result = benchmark_cycles(
        [&]() { std::transform(input.begin(), input.end(), output.begin(), double_value); },
        iterations);

    print_result("transform (1000 elements)", result);
}

// =============================================================================
// BENCHMARK: Comparación con diferentes tipos compatibles
// =============================================================================
void benchmark_compatible_type_operations()
{
    std::cout << "\n=== BENCHMARK: Compatible type operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    uint128_t u128 = 1000;
    uint64_t u64 = 500;
    uint32_t u32 = 250;
    uint128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = u128 + u64;
            sink = sink + u32;
            u128 = sink + 1;
        },
        iterations);

    volatile uint128_t dummy = sink;
    (void)dummy;
    print_result("Mixed type operations", result);
}

// =============================================================================
// BENCHMARK: Operaciones bitwise con tipos compatibles
// =============================================================================
void benchmark_bitwise_operations()
{
    std::cout << "\n=== BENCHMARK: Bitwise operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    uint128_t u128 = 0xFFFFFFFFFFFFFFFFull;
    uint64_t mask = 0xFF00FF00FF00FF00ull;
    uint128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = u128 & mask;
            sink = sink | uint128_t(0x0F);
            sink = sink ^ uint128_t(0xF0);
            u128 = sink + 1;
        },
        iterations);

    volatile uint128_t dummy = sink;
    (void)dummy;
    print_result("Bitwise operations (AND, OR, XOR)", result);
}

// =============================================================================
// BENCHMARK: Operaciones de shift
// =============================================================================
void benchmark_shift_operations()
{
    std::cout << "\n=== BENCHMARK: Shift operations ===\n";

    constexpr uint64_t iterations = 50'000'000;

    uint128_t value = uint128_t(1) << 64;
    int shift = 8;
    uint128_t sink = 0;

    auto result = benchmark_cycles(
        [&]() {
            sink = value << shift;
            sink = sink >> shift;
            value = sink + 1;
        },
        iterations);

    volatile uint128_t dummy = sink;
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

    std::vector<uint128_t> vec;
    for (int i = 0; i < 1000; ++i) {
        vec.push_back(uint128_t(i));
    }

    auto result = benchmark_cycles(
        [&]() {
            uint128_t sum = std::accumulate(vec.begin(), vec.end(), uint128_t(0));
            volatile uint128_t dummy = sum;
            (void)dummy;
        },
        iterations);

    print_result("accumulate (1000 elements)", result);
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  UINT128 CONCEPTS - BENCHMARKS\n";
    std::cout << "========================================\n";

    try {
        benchmark_concept_verification();
        benchmark_uint128_function();
        benchmark_uint128_predicate();
        benchmark_uint128_binary_operation();
        benchmark_uint128_container();
        benchmark_find_with_predicate();
        benchmark_transform_with_function();
        benchmark_compatible_type_operations();
        benchmark_bitwise_operations();
        benchmark_shift_operations();
        benchmark_accumulate_with_reduce();
    } catch (const std::exception& e) {
        std::cerr << "\n[FAIL] Exception caught: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << "  [OK] BENCHMARKS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}
