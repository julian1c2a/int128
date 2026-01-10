/*
 * Benchmarks para int128_limits.hpp
 * Mide el rendimiento de acceso a std::numeric_limits<int128_t>
 */

#include "int128_base_limits.hpp"
#include "int128_base_tt.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace nstd;
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

double get_cpu_freq_ghz()
{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    return 3.1;
#else
    return 2.5;
#endif
}

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
BenchmarkResult benchmark(const std::string& name, Func func, uint64_t iterations = 10000000)
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
// BENCHMARKS DE LIMITS
// =============================================================================

void benchmark_limits_constants()
{
    std::cout << "\nBenchmarking Limits Constants...\n";

    benchmark("limits::max()",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::max(); });

    benchmark("limits::min()",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::min(); });

    benchmark("limits::lowest()",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::lowest(); });
}

void benchmark_limits_properties()
{
    std::cout << "\nBenchmarking Limits Properties...\n";

    benchmark("limits::digits",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::digits; });

    benchmark("limits::digits10",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::digits10; });

    benchmark("limits::max_digits10",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::max_digits10; });

    benchmark("limits::is_signed",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::is_signed; });

    benchmark("limits::is_integer",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::is_integer; });

    benchmark("limits::is_exact",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::is_exact; });

    benchmark("limits::is_bounded",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::is_bounded; });

    benchmark("limits::is_modulo",
              []() { [[maybe_unused]] auto v = std::numeric_limits<int128_t>::is_modulo; });
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "int128_t Limits Benchmarks\n";
    std::cout << "========================================\n";
    std::cout << "Compiler: " << get_compiler() << "\n";
    std::cout << "Optimization: " << get_optimization() << "\n";
    std::cout << "Timestamp: " << get_timestamp() << "\n";
    std::cout << "========================================\n";

    benchmark_limits_constants();
    benchmark_limits_properties();

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
