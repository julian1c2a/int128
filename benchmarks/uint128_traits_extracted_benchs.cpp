/*
 * Benchmarks extraídos para uint128_traits.hpp
 * Pruebas de rendimiento individuales por función/especialización
 * Mide tanto tiempos como ciclos de reloj
 */

#include "../include/uint128/uint128_limits.hpp"
#include "../include/uint128/uint128_t.hpp"
#include "../include/uint128/uint128_traits.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <vector>

// =============================================================================
// UTILIDADES PARA MEDICIÓN
// =============================================================================

#ifdef _MSC_VER
#include <intrin.h>
#define RDTSC() __rdtsc()
// MSVC no soporta asm inline en x64, usamos barrera por referencia volátil
#define PREVENT_OPTIMIZE(var) (*(volatile decltype(&var))&var)
#elif defined(__GNUC__) || defined(__clang__)
static inline uint64_t RDTSC()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#define PREVENT_OPTIMIZE(var) asm volatile("" : : "r,m"(var) : "memory")
#else
#define RDTSC() 0
#define PREVENT_OPTIMIZE(var) ((void)var)
#endif

// Estructura para resultados de benchmark
struct BenchmarkResult {
    std::string name;
    double time_ns;
    uint64_t cycles;
    size_t iterations;
};

// =============================================================================
// BENCHMARK: is_integral (compile time, medimos overhead)
// =============================================================================
BenchmarkResult bench_is_integral()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_integral_v<uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_integral", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: is_arithmetic (compile time, medimos overhead)
// =============================================================================
BenchmarkResult bench_is_arithmetic()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_arithmetic_v<uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_arithmetic", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: is_unsigned (compile time, medimos overhead)
// =============================================================================
BenchmarkResult bench_is_unsigned()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_unsigned_v<uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_unsigned", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: is_trivially_copyable
// =============================================================================
BenchmarkResult bench_is_trivially_copyable()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_trivially_copyable_v<uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_trivially_copyable", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: Copia real de uint128_t (prueba de trivial copy)
// =============================================================================
BenchmarkResult bench_copy_uint128()
{
    const size_t ITERATIONS = 10'000'000;
    std::vector<uint128_t> source(100);
    std::vector<uint128_t> dest(100);

    for (size_t i = 0; i < 100; ++i) {
        source[i] = uint128_t(i, i * 2);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        dest[i % 100] = source[i % 100];
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"copy_uint128_t", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: common_type resolution (compile time)
// =============================================================================
BenchmarkResult bench_common_type()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"common_type_check", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: std::hash - cálculo de hash
// =============================================================================
BenchmarkResult bench_hash_computation()
{
    const size_t ITERATIONS = 1'000'000;
    std::vector<uint128_t> values;
    values.reserve(1000);

    for (size_t i = 0; i < 1000; ++i) {
        values.push_back(uint128_t(i, i * 2));
    }

    std::hash<uint128_t> hasher;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    volatile size_t sum = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        sum += hasher(values[i % 1000]);
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"hash_computation", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: std::hash - inserción en unordered_map
// =============================================================================
BenchmarkResult bench_hash_map_insert()
{
    const size_t ITERATIONS = 100'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    std::unordered_map<uint128_t, int> map;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        map[uint128_t(i, i * 2)] = static_cast<int>(i);
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"hash_map_insert", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: std::hash - búsqueda en unordered_map
// =============================================================================
BenchmarkResult bench_hash_map_lookup()
{
    const size_t SIZE = 10'000;
    const size_t ITERATIONS = 1'000'000;

    std::unordered_map<uint128_t, int> map;
    for (size_t i = 0; i < SIZE; ++i) {
        map[uint128_t(i, i * 2)] = static_cast<int>(i);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    volatile int sum = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        auto it = map.find(uint128_t(i % SIZE, (i % SIZE) * 2));
        if (it != map.end()) {
            sum += it->second;
        }
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"hash_map_lookup", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: numeric_limits queries (compile time)
// =============================================================================
BenchmarkResult bench_numeric_limits()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    int digits = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::numeric_limits<uint128_t>::is_specialized;
        digits = std::numeric_limits<uint128_t>::digits;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(result);
    PREVENT_OPTIMIZE(digits);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"numeric_limits_query", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: numeric_limits - obtener min/max
// =============================================================================
BenchmarkResult bench_numeric_limits_minmax()
{
    const size_t ITERATIONS = 1'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    uint128_t min_val;
    uint128_t max_val;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        min_val = std::numeric_limits<uint128_t>::min();
        max_val = std::numeric_limits<uint128_t>::max();
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    PREVENT_OPTIMIZE(min_val);
    PREVENT_OPTIMIZE(max_val);

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"numeric_limits_minmax", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// FUNCIÓN PARA IMPRIMIR RESULTADOS
// =============================================================================
void print_result(const BenchmarkResult& result)
{
    std::cout << "\n=== " << result.name << " ===\n";
    std::cout << "  Iterations: " << result.iterations << "\n";
    std::cout << "  Time:       " << result.time_ns << " ns/op\n";
    std::cout << "  Cycles:     " << result.cycles << " cycles/op\n";
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "============================================================\n";
    std::cout << "  uint128_traits.hpp - Benchmarks Extraídos Individuales\n";
    std::cout << "============================================================\n";

    std::vector<BenchmarkResult> results;

    std::cout << "\n[1/11] Benchmarking is_integral...";
    results.push_back(bench_is_integral());

    std::cout << "\n[2/11] Benchmarking is_arithmetic...";
    results.push_back(bench_is_arithmetic());

    std::cout << "\n[3/11] Benchmarking is_unsigned...";
    results.push_back(bench_is_unsigned());

    std::cout << "\n[4/11] Benchmarking is_trivially_copyable...";
    results.push_back(bench_is_trivially_copyable());

    std::cout << "\n[5/11] Benchmarking copy_uint128_t...";
    results.push_back(bench_copy_uint128());

    std::cout << "\n[6/11] Benchmarking common_type...";
    results.push_back(bench_common_type());

    std::cout << "\n[7/11] Benchmarking hash_computation...";
    results.push_back(bench_hash_computation());

    std::cout << "\n[8/11] Benchmarking hash_map_insert...";
    results.push_back(bench_hash_map_insert());

    std::cout << "\n[9/11] Benchmarking hash_map_lookup...";
    results.push_back(bench_hash_map_lookup());

    std::cout << "\n[10/11] Benchmarking numeric_limits_query...";
    results.push_back(bench_numeric_limits());

    std::cout << "\n[11/11] Benchmarking numeric_limits_minmax...";
    results.push_back(bench_numeric_limits_minmax());

    std::cout << "\n\n============================================================\n";
    std::cout << "                      RESULTADOS\n";
    std::cout << "============================================================\n";

    for (const auto& result : results) {
        print_result(result);
    }

    // Resumen en formato CSV
    std::cout << "\n============================================================\n";
    std::cout << "                    RESUMEN CSV\n";
    std::cout << "============================================================\n";
    std::cout << "Benchmark,Time(ns),Cycles,Iterations\n";
    for (const auto& result : results) {
        std::cout << result.name << "," << result.time_ns << "," << result.cycles << ","
                  << result.iterations << "\n";
    }

    return 0;
}
