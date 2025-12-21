/**
 * @file int128_traits_extracted_benchs.cpp
 * @brief Benchmarks individuales para cada especialización de int128_traits.hpp
 *
 * Este archivo contiene benchmarks detallados para medir el rendimiento de:
 * - Type traits (is_integral, is_arithmetic, etc.)
 * - Función hash
 * - Operaciones de copia y movimiento
 *
 * Mide tanto tiempo (nanosegundos) como ciclos de CPU (RDTSC)
 *
 * Compiladores: GCC, Clang, Intel OneAPI, MSVC
 * Estándar: C++20
 */

#include <chrono>
#include <cstdint>
#include <functional>
#include <int128/int128_t.hpp>
#include <int128/int128_traits.hpp>
#include <iostream>
#include <type_traits>

// Detectar si estamos usando libc++ (Clang)
#ifdef _LIBCPP_VERSION
#define INT128_USING_LIBCPP 1
#else
#define INT128_USING_LIBCPP 0
#endif

// =============================================================================
// MEDICIÓN DE CICLOS CON RDTSC
// =============================================================================

#if defined(_MSC_VER)
#include <intrin.h>
inline uint64_t rdtsc()
{
    return __rdtsc();
}
#elif defined(__GNUC__) || defined(__clang__)
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

// =============================================================================
// UTILIDADES DE BENCHMARK
// =============================================================================

constexpr size_t ITERATIONS = 10'000'000;
constexpr size_t WARMUP_ITERATIONS = 1000;

struct BenchmarkResult {
    double time_ns;
    uint64_t cycles;
    const char* name;
};

void print_header()
{
    std::cout << "=============================================================\n";
    std::cout << "   Benchmarks de int128_traits.hpp\n";
    std::cout << "=============================================================\n";
    std::cout << "Iteraciones: " << ITERATIONS << "\n";
    std::cout << "Formato: [Benchmark] | Tiempo (ns) | Ciclos CPU\n";
    std::cout << "-------------------------------------------------------------\n\n";
}

void print_result(const BenchmarkResult& result)
{
    std::cout << "[" << result.name << "]\n";
    std::cout << "  Tiempo promedio: " << result.time_ns << " ns\n";
    std::cout << "  Ciclos promedio: " << result.cycles << " ciclos\n\n";
}

// =============================================================================
// BENCHMARKS DE TYPE TRAITS
// =============================================================================

/**
 * @brief Benchmark de is_integral en tiempo de compilación
 * Este benchmark simula el uso de is_integral en código genérico
 */
BenchmarkResult bench_is_integral()
{
    bool result = false;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = std::is_integral<int128_t>::value;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_integral<int128_t>::value;
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory"); // Evitar optimización

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "is_integral"};
}

/**
 * @brief Benchmark de is_arithmetic
 */
BenchmarkResult bench_is_arithmetic()
{
    bool result = false;

    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = std::is_arithmetic<int128_t>::value;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_arithmetic<int128_t>::value;
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "is_arithmetic"};
}

/**
 * @brief Benchmark de is_signed
 */
BenchmarkResult bench_is_signed()
{
    bool result = false;

    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = std::is_signed<int128_t>::value;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_signed<int128_t>::value;
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "is_signed"};
}

/**
 * @brief Benchmark de is_trivially_copyable
 */
BenchmarkResult bench_is_trivially_copyable()
{
    bool result = false;

    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = std::is_trivially_copyable<int128_t>::value;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = std::is_trivially_copyable<int128_t>::value;
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "is_trivially_copyable"};
}

// =============================================================================
// BENCHMARKS DE HASH
// =============================================================================

/**
 * @brief Benchmark de std::hash<int128_t>
 * Mide el rendimiento del cálculo de hash
 */
BenchmarkResult bench_hash_computation()
{
    std::hash<int128_t> hasher;
    int128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    size_t result = 0;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = hasher(value);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = hasher(value);
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "hash_computation"};
}

/**
 * @brief Benchmark de hash con valores variables
 */
BenchmarkResult bench_hash_varying_values()
{
    std::hash<int128_t> hasher;
    size_t result = 0;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        int128_t value(i, i * 2);
        result = hasher(value);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t value(i, i * 2);
        result = hasher(value);
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "hash_varying_values"};
}

// =============================================================================
// BENCHMARKS DE OPERACIONES DE COPIA/MOVIMIENTO
// =============================================================================

/**
 * @brief Benchmark de copia de int128_t
 * Como int128_t es trivialmente copiable, esta operación debe ser muy rápida
 */
BenchmarkResult bench_copy_int128()
{
    int128_t src(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    int128_t dst;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        dst = src;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        dst = src;
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    // Prevent optimization
    asm volatile("" : : "r,m"(dst) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "copy_assignment"};
}

/**
 * @brief Benchmark de movimiento de int128_t
 */
BenchmarkResult bench_move_int128()
{
    int128_t dst;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        int128_t src(i, i * 2);
        dst = std::move(src);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        int128_t src(i, i * 2);
        dst = std::move(src);
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(dst) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "move_assignment"};
}

// =============================================================================
// BENCHMARKS DE COMMON_TYPE
// =============================================================================

/**
 * @brief Benchmark de evaluación de common_type
 * Aunque es compiletime, medimos su uso en contextos genéricos
 */
BenchmarkResult bench_common_type_usage()
{
    using common_t = std::common_type<int128_t, int64_t>::type;
    common_t result;

    int128_t a(1000, 2000);
    int64_t b = 42;

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        result = static_cast<common_t>(a) + static_cast<common_t>(b);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = static_cast<common_t>(a) + static_cast<common_t>(b);
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "common_type_usage"};
}

/**
 * @brief Benchmark de make_unsigned usage
 */
BenchmarkResult bench_make_unsigned_usage()
{
#if !INT128_USING_LIBCPP
    using unsigned_t = std::make_unsigned<int128_t>::type;
    unsigned_t result;

    int128_t src(-1000, 2000);

    // Warmup
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        // Conversión bit a bit de int128_t a uint128_t
        result = unsigned_t(src.low(), src.high());
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = rdtsc();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        result = unsigned_t(src.low(), src.high());
    }

    uint64_t end_cycles = rdtsc();
    auto end_time = std::chrono::high_resolution_clock::now();

    asm volatile("" : : "r,m"(result) : "memory");

    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();

    return BenchmarkResult{elapsed_ns / ITERATIONS, (end_cycles - start_cycles) / ITERATIONS,
                           "make_unsigned_usage"};
#else
    return BenchmarkResult{0.0, 0, "make_unsigned_usage [SKIP - libc++]"};
#endif
}

// =============================================================================
// FUNCIÓN PRINCIPAL
// =============================================================================

int main()
{
    print_header();

    // Type traits benchmarks
    print_result(bench_is_integral());
    print_result(bench_is_arithmetic());
    print_result(bench_is_signed());
    print_result(bench_is_trivially_copyable());

    // Hash benchmarks
    print_result(bench_hash_computation());
    print_result(bench_hash_varying_values());

    // Copy/move benchmarks
    print_result(bench_copy_int128());
    print_result(bench_move_int128());

    // Type transformation benchmarks
    print_result(bench_common_type_usage());
    print_result(bench_make_unsigned_usage());

    std::cout << "=============================================================\n";
    std::cout << "   Benchmarks Completados\n";
    std::cout << "=============================================================\n";

    return 0;
}
