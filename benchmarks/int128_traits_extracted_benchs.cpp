/*
 * Benchmarks extraídos para int128_traits.hpp
 * Pruebas de rendimiento individuales por función/especialización
 * Mide tanto tiempos como ciclos de reloj
 */

#include "../include/int128/int128_limits.hpp"
#include "../include/int128/int128_t.hpp"
#include "../include/int128/int128_traits.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <vector>

// Detectar si las especializaciones de traits están disponibles
// NO están disponibles en:
// - libc++ (Clang)
// - Intel ICX en Windows (usa MSVC STL)
#if defined(_LIBCPP_VERSION)
#define INT128_TRAITS_NOT_AVAILABLE 1
#elif defined(__INTEL_LLVM_COMPILER) && defined(_MSC_VER)
#define INT128_TRAITS_NOT_AVAILABLE 1
#else
#define INT128_TRAITS_NOT_AVAILABLE 0
#endif

// =============================================================================
// UTILIDADES PARA MEDICIÓN
// =============================================================================

#ifdef _MSC_VER
#include <intrin.h>
#define RDTSC() __rdtsc()
#elif defined(__GNUC__) || defined(__clang__)
static inline uint64_t RDTSC()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#else
#define RDTSC() 0
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
        result = std::is_integral_v<int128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

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
        result = std::is_arithmetic_v<int128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_arithmetic", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: is_signed
// =============================================================================
BenchmarkResult bench_is_signed()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_signed_v<int128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_signed", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: is_unsigned
// =============================================================================
BenchmarkResult bench_is_unsigned()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_unsigned_v<int128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"is_unsigned", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: hash
// =============================================================================
BenchmarkResult bench_hash()
{
#if !INT128_TRAITS_NOT_AVAILABLE
    const size_t ITERATIONS = 1'000'000;
    std::hash<int128_t> hasher;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    size_t sum = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t val = static_cast<int128_t>(i) - 500000;
        sum += hasher(val);
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(sum) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"hash", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
#else
    return {"hash [SKIP]", 0.0, 0, 0};
#endif
}

// =============================================================================
// BENCHMARK: unordered_map inserciones
// =============================================================================
BenchmarkResult bench_unordered_map_insert()
{
#if !INT128_TRAITS_NOT_AVAILABLE
    const size_t ITERATIONS = 100'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    std::unordered_map<int128_t, int> map;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t key = static_cast<int128_t>(i) - 50000;
        map[key] = static_cast<int>(i);
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(map.size()) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"unordered_map_insert", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
#else
    return {"unordered_map_insert [SKIP]", 0.0, 0, 0};
#endif
}

// =============================================================================
// BENCHMARK: unordered_map búsquedas
// =============================================================================
BenchmarkResult bench_unordered_map_lookup()
{
#if !INT128_TRAITS_NOT_AVAILABLE
    const size_t ITERATIONS = 100'000;

    // Preparar map
    std::unordered_map<int128_t, int> map;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t key = static_cast<int128_t>(i) - 50000;
        map[key] = static_cast<int>(i);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    size_t sum = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t key = static_cast<int128_t>(i) - 50000;
        auto it = map.find(key);
        if (it != map.end()) {
            sum += it->second;
        }
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(sum) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"unordered_map_lookup", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
#else
    return {"unordered_map_lookup [SKIP]", 0.0, 0, 0};
#endif
}

// =============================================================================
// BENCHMARK: vector push_back
// =============================================================================
BenchmarkResult bench_vector_push_back()
{
    const size_t ITERATIONS = 100'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    std::vector<int128_t> vec;
    vec.reserve(ITERATIONS);
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t val = static_cast<int128_t>(i) - 50000;
        vec.push_back(val);
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(vec.size()) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"vector_push_back", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: vector random access
// =============================================================================
BenchmarkResult bench_vector_access()
{
    const size_t ITERATIONS = 100'000;

    // Preparar vector
    std::vector<int128_t> vec;
    vec.reserve(ITERATIONS);
    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t val = static_cast<int128_t>(i) - 50000;
        vec.push_back(val);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    int128_t sum = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        sum += vec[i];
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(sum) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"vector_access", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// BENCHMARK: make_signed (compile time)
// =============================================================================
BenchmarkResult bench_make_signed()
{
#if !INT128_TRAITS_NOT_AVAILABLE
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_same_v<std::make_signed_t<int128_t>, int128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"make_signed", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
#else
    return {"make_signed [SKIP]", 0.0, 0, 0};
#endif
}

// =============================================================================
// BENCHMARK: make_unsigned (compile time)
// =============================================================================
BenchmarkResult bench_make_unsigned()
{
#if !INT128_TRAITS_NOT_AVAILABLE
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    bool result = false;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_same_v<std::make_unsigned_t<int128_t>, uint128_t>;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"make_unsigned", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
#else
    return {"make_unsigned [SKIP]", 0.0, 0, 0};
#endif
}

// =============================================================================
// BENCHMARK: numeric_limits acceso
// =============================================================================
BenchmarkResult bench_numeric_limits()
{
    const size_t ITERATIONS = 10'000'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();

    int count = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        if (std::numeric_limits<int128_t>::is_signed)
            count++;
        if (std::numeric_limits<int128_t>::is_integer)
            count++;
        if (std::numeric_limits<int128_t>::digits == 127)
            count++;
    }

    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(count) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;

    return {"numeric_limits", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}

// =============================================================================
// MAIN: Ejecutar todos los benchmarks
// =============================================================================
void print_result(const BenchmarkResult& res)
{
    std::cout << "  " << res.name << ":\n";
    if (res.iterations == 0) {
        std::cout << "    [SKIP] No soportado en esta configuración STL\n\n";
    } else {
        std::cout << "    Tiempo:      " << res.time_ns << " ns/op\n";
        std::cout << "    Ciclos:      " << res.cycles << " cycles/op\n";
        std::cout << "    Iteraciones: " << res.iterations << "\n\n";
    }
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "  BENCHMARKS: int128_traits.hpp\n";
    std::cout << "========================================\n\n";

    print_result(bench_is_integral());
    print_result(bench_is_arithmetic());
    print_result(bench_is_signed());
    print_result(bench_is_unsigned());
    print_result(bench_hash());
    print_result(bench_unordered_map_insert());
    print_result(bench_unordered_map_lookup());
    print_result(bench_vector_push_back());
    print_result(bench_vector_access());
    print_result(bench_make_signed());
    print_result(bench_make_unsigned());
    print_result(bench_numeric_limits());

    std::cout << "========================================\n";
    std::cout << "  BENCHMARKS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}
