/*
 * Benchmarks para int128_traits.hpp
 * Mide el rendimiento de operaciones que usan traits
 */

#include "../include/int128/int128_limits.hpp"
#include "../include/int128/int128_t.hpp"
#include "../include/int128/int128_traits.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// Detectar si las especializaciones de traits están disponibles
#if defined(_LIBCPP_VERSION)
#define INT128_TRAITS_NOT_AVAILABLE 1
#elif defined(__INTEL_LLVM_COMPILER) && defined(_MSC_VER)
#define INT128_TRAITS_NOT_AVAILABLE 1
#else
#define INT128_TRAITS_NOT_AVAILABLE 0
#endif

// =============================================================================
// UTILIDADES DE BENCHMARK
// =============================================================================

struct BenchmarkResult {
    std::string name;
    double ns_per_op;
    double cycles_per_op;
    uint64_t ops_per_sec;
};

std::vector<BenchmarkResult> g_results;

// Obtener frecuencia de CPU aproximada (en GHz)
double get_cpu_freq_ghz()
{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    return 3.1; // Aproximación (ajustar según CPU)
#else
    return 2.5;
#endif
}

// Timestamp
std::string get_timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
#ifdef _WIN32
    struct tm tm_buf;
    localtime_s(&tm_buf, &time_t_now);
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
#else
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
#endif
    return ss.str();
}

// Obtener compilador
std::string get_compiler()
{
#if defined(__clang__)
    return "Clang-" + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#elif defined(__GNUC__) && !defined(__clang__)
    return "GCC-" + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#elif defined(__INTEL_LLVM_COMPILER)
    return "Clang-" + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#elif defined(_MSC_VER)
    return "MSVC-" + std::to_string(_MSC_VER);
#else
    return "Unknown";
#endif
}

// Obtener nivel de optimización
std::string get_optimization()
{
#if defined(__OPTIMIZE__)
#if __OPTIMIZE__ >= 3
    return "O3";
#elif __OPTIMIZE__ >= 2
    return "O2";
#elif __OPTIMIZE__ >= 1
    return "O1";
#else
    return "O0";
#endif
#elif defined(_DEBUG) || defined(DEBUG)
    return "O0";
#else
    return "O1";
#endif
}

template <typename Func>
BenchmarkResult benchmark(const std::string& name, Func func, uint64_t iterations = 1000000)
{
    // Warmup
    for (uint64_t i = 0; i < iterations / 10; ++i) {
        func();
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double ns_per_op = static_cast<double>(duration.count()) / iterations;
    double cycles_per_op = ns_per_op * get_cpu_freq_ghz();
    uint64_t ops_per_sec = static_cast<uint64_t>(1e9 / ns_per_op);

    BenchmarkResult result{name, ns_per_op, cycles_per_op, ops_per_sec};
    g_results.push_back(result);

    std::cout << name << ": " << std::fixed << std::setprecision(3) << ns_per_op << " ns, "
              << std::setprecision(2) << cycles_per_op << " cycles, " << ops_per_sec
              << " ops/sec\n";

    return result;
}

// =============================================================================
// BENCHMARKS DE TRAITS
// =============================================================================

void benchmark_traits_queries()
{
    std::cout << "\nBenchmarking Traits Queries...\n";

#if !INT128_TRAITS_NOT_AVAILABLE
    // is_integral
    benchmark("is_integral<int128_t>",
              []() { [[maybe_unused]] bool v = std::is_integral_v<int128_t>; });

    // is_signed
    benchmark("is_signed<int128_t>",
              []() { [[maybe_unused]] bool v = std::is_signed_v<int128_t>; });

    // is_arithmetic
    benchmark("is_arithmetic<int128_t>",
              []() { [[maybe_unused]] bool v = std::is_arithmetic_v<int128_t>; });

    // make_unsigned
    benchmark("make_unsigned<int128_t>",
              []() { [[maybe_unused]] typename std::make_unsigned<int128_t>::type v; });

    // make_signed
    benchmark("make_signed<int128_t>",
              []() { [[maybe_unused]] typename std::make_signed<int128_t>::type v; });

    // numeric_limits::max
    benchmark("numeric_limits<int128_t>::max",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::max(); });

    // numeric_limits::min
    benchmark("numeric_limits<int128_t>::min",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::min(); });

    // numeric_limits::lowest
    benchmark("numeric_limits<int128_t>::lowest",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::lowest(); });

    // numeric_limits::digits
    benchmark("numeric_limits<int128_t>::digits",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::digits; });
#else
    std::cout << "  [SKIP] Traits no soportados en esta configuración STL\n";
#endif
}

void benchmark_hash_operations()
{
    std::cout << "\nBenchmarking Hash Operations...\n";

    std::hash<int128_t> hasher;
    int128_t val(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    benchmark("hash<int128_t>", [&]() { [[maybe_unused]] auto h = hasher(val); });
}

void benchmark_container_usage()
{
    std::cout << "\nBenchmarking Container Usage...\n";

#if !INT128_TRAITS_NOT_AVAILABLE
    std::vector<int128_t> vec;
    vec.reserve(1000);

    benchmark(
        "vector<int128_t>::push_back",
        [&]() {
            vec.clear();
            for (int i = 0; i < 100; ++i) {
                vec.push_back(int128_t(i));
            }
        },
        10000);

    benchmark(
        "vector<int128_t>::accumulate",
        [&]() { [[maybe_unused]] auto sum = std::accumulate(vec.begin(), vec.end(), int128_t(0)); },
        10000);
#else
    std::cout << "  [SKIP] Traits no soportados en esta configuración STL\n";
#endif
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "int128_t Traits Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Compiler: " << get_compiler() << "\n";
    std::cout << "Optimization: " << get_optimization() << "\n";
    std::cout << "Timestamp: " << get_timestamp() << "\n";

#if INT128_TRAITS_NOT_AVAILABLE
    std::cout << "\n[WARN]  ADVERTENCIA: Traits no soportados en esta configuración\n";
    std::cout << "   (libc++ o Intel ICX/Windows)\n";
#endif

    std::cout << "========================================\n";

    benchmark_traits_queries();
    benchmark_hash_operations();
    benchmark_container_usage();

    std::cout << "\n========================================\n";
    std::cout << "BENCHMARK SUMMARY\n";
    std::cout << "========================================\n";
    std::cout << "Compiler: " << get_compiler() << "\n";
    std::cout << "Optimization: " << get_optimization() << "\n";
    std::cout << "Total operations benchmarked: " << g_results.size() << "\n";
    std::cout << "========================================\n";

    std::cout << "\nBenchmarks completed successfully!\n";

    return 0;
}
