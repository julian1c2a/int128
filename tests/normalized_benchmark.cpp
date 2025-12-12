#include "../include/uint128_t.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace std::chrono;

struct SystemInfo
{
    std::string cpu_info;
    std::string compiler_info;
    std::string build_config;
    std::string timestamp;
};

struct BenchmarkResult
{
    std::string operation;
    double median_ns;
    double mean_ns;
    double stddev_ns;
    double min_ns;
    double max_ns;
    size_t iterations;
    double relative_to_baseline; // Relativo a suma de uint64_t
    double ops_per_second;
};

SystemInfo getSystemInfo()
{
    SystemInfo info;

    // Get timestamp
    auto now = system_clock::now();
    auto time_t = system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d_%H-%M-%S");
    info.timestamp = ss.str();

    // Compiler info
#ifdef _MSC_VER
    info.compiler_info = "MSVC_" + std::to_string(_MSC_VER);
#elif defined(__GNUC__)
    info.compiler_info = "GCC_" + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#elif defined(__clang__)
    info.compiler_info = "Clang_" + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#else
    info.compiler_info = "Unknown";
#endif

    // Build config
#ifdef NDEBUG
    info.build_config = "Release";
#else
    info.build_config = "Debug";
#endif

    // CPU info (simplified)
    info.cpu_info = "x64"; // Podríamos extender esto con CPUID

    return info;
}

template <typename Func>
BenchmarkResult benchmarkOperation(const std::string &name, Func func, size_t iterations = 100000)
{
    std::vector<double> times;
    times.reserve(iterations);

    // Warm up
    for (int i = 0; i < 1000; ++i)
    {
        func();
    }

    // Actual benchmark
    for (size_t i = 0; i < iterations; ++i)
    {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<nanoseconds>(end - start);
        times.push_back(static_cast<double>(duration.count()));
    }

    // Calculate statistics
    std::sort(times.begin(), times.end());

    double median_ns = times[times.size() / 2];
    double mean_ns = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    double min_ns = times.front();
    double max_ns = times.back();

    // Calculate standard deviation
    double variance = 0.0;
    for (auto time : times)
    {
        variance += (time - mean_ns) * (time - mean_ns);
    }
    double stddev_ns = std::sqrt(variance / times.size());

    double ops_per_second = 1000000000.0 / median_ns;

    return {name, median_ns, mean_ns, stddev_ns, min_ns, max_ns, iterations, 0.0, ops_per_second};
}

void writeCSV(const std::vector<BenchmarkResult> &results, const SystemInfo &sysInfo, const std::string &filename)
{
    std::ofstream file("../documentation/benchmarks/" + filename);

    // Header with system info
    file << "# uint128_t Benchmark Results\n";
    file << "# Timestamp: " << sysInfo.timestamp << "\n";
    file << "# Compiler: " << sysInfo.compiler_info << "\n";
    file << "# Build: " << sysInfo.build_config << "\n";
    file << "# CPU: " << sysInfo.cpu_info << "\n";
    file << "#\n";

    // CSV headers
    file << "Operation,Median_ns,Mean_ns,StdDev_ns,Min_ns,Max_ns,Iterations,Relative_to_Baseline,Ops_per_second\n";

    for (const auto &result : results)
    {
        file << result.operation << ","
             << std::fixed << std::setprecision(3) << result.median_ns << ","
             << result.mean_ns << ","
             << result.stddev_ns << ","
             << result.min_ns << ","
             << result.max_ns << ","
             << result.iterations << ","
             << std::setprecision(2) << result.relative_to_baseline << ","
             << std::scientific << result.ops_per_second << "\n";
    }
}

int main()
{
    std::cout << "=== uint128_t Normalized Benchmarks ===\n\n";

    auto sysInfo = getSystemInfo();
    std::cout << "System Info:\n";
    std::cout << "  Timestamp: " << sysInfo.timestamp << "\n";
    std::cout << "  Compiler: " << sysInfo.compiler_info << "\n";
    std::cout << "  Build: " << sysInfo.build_config << "\n";
    std::cout << "  CPU: " << sysInfo.cpu_info << "\n\n";

    // Setup test data
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    uint128_t a(dist(gen), dist(gen));
    uint128_t b(dist(gen), dist(gen));
    uint64_t native_a = dist(gen);
    uint64_t native_b = dist(gen);

    std::vector<BenchmarkResult> results;

    std::cout << "Running baseline (uint64_t) benchmarks...\n";

    // Baseline: uint64_t addition
    auto baseline = benchmarkOperation("uint64_t_addition", [&]()
                                       { volatile auto result = native_a + native_b; });
    baseline.relative_to_baseline = 1.0;

    std::cout << "Running uint128_t benchmarks...\n";

    // uint128_t operations
    auto uint128_add = benchmarkOperation("uint128_t_addition", [&]()
                                          { volatile auto result = a + b; });
    uint128_add.relative_to_baseline = uint128_add.median_ns / baseline.median_ns;

    auto uint128_sub = benchmarkOperation("uint128_t_subtraction", [&]()
                                          { volatile auto result = a - b; });
    uint128_sub.relative_to_baseline = uint128_sub.median_ns / baseline.median_ns;

    auto uint128_mul = benchmarkOperation("uint128_t_multiplication", [&]()
                                          { volatile auto result = a * b; }, 50000); // Fewer iterations for slower ops
    uint128_mul.relative_to_baseline = uint128_mul.median_ns / baseline.median_ns;

    auto uint128_and = benchmarkOperation("uint128_t_bitwise_and", [&]()
                                          { volatile auto result = a & b; });
    uint128_and.relative_to_baseline = uint128_and.median_ns / baseline.median_ns;

    auto uint128_lshift = benchmarkOperation("uint128_t_left_shift", [&]()
                                             { volatile auto result = a << 5; });
    uint128_lshift.relative_to_baseline = uint128_lshift.median_ns / baseline.median_ns;

    auto uint128_eq = benchmarkOperation("uint128_t_equality", [&]()
                                         { volatile bool result = a == b; });
    uint128_eq.relative_to_baseline = uint128_eq.median_ns / baseline.median_ns;

    auto uint128_lt = benchmarkOperation("uint128_t_less_than", [&]()
                                         { volatile bool result = a < b; });
    uint128_lt.relative_to_baseline = uint128_lt.median_ns / baseline.median_ns;

    // Add results
    results.push_back(baseline);
    results.push_back(uint128_add);
    results.push_back(uint128_sub);
    results.push_back(uint128_mul);
    results.push_back(uint128_and);
    results.push_back(uint128_lshift);
    results.push_back(uint128_eq);
    results.push_back(uint128_lt);

    // Write to CSV
    std::string filename = "normalized_benchmark_" + sysInfo.timestamp + ".csv";
    writeCSV(results, sysInfo, filename);

    // Display results
    std::cout << "\nBenchmark Results (median times):\n";
    std::cout << std::left << std::setw(25) << "Operation"
              << std::setw(12) << "Time (ns)"
              << std::setw(10) << "Relative"
              << std::setw(15) << "Ops/sec" << "\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto &result : results)
    {
        std::cout << std::left << std::setw(25) << result.operation
                  << std::setw(12) << std::fixed << std::setprecision(2) << result.median_ns
                  << std::setw(10) << std::setprecision(2) << result.relative_to_baseline << "x"
                  << std::setw(15) << std::scientific << result.ops_per_second << "\n";
    }

    std::cout << "\nKey insights:\n";
    std::cout << "- Relative times show performance vs native uint64_t operations\n";
    std::cout << "- Lower relative values are better (closer to native performance)\n";
    std::cout << "- Median times reduce impact of outliers\n";
    std::cout << "- Results saved to: documentation/benchmarks/" << filename << "\n";

    return 0;
}