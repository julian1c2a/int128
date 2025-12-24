/**
 * @file int128_extracted_benchmarks.cpp
 * @brief Comprehensive benchmarks for int128_t (signed 128-bit integer)
 *
 * Benchmarks all major operations comparing with:
 * - Built-in signed types: int64_t, int32_t, __int128_t (GCC/Clang)
 * - Boost multiprecision signed integers
 *
 * Results exported in CSV and JSON format
 */

#include "../include/int128/int128_t.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// CPU cycles measurement
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#define HAS_RDTSC 1
#elif defined(__INTEL_COMPILER)
#include <ia32intrin.h>
#define HAS_RDTSC 1
#elif defined(__GNUC__) || defined(__clang__)
#include <x86intrin.h>
#define HAS_RDTSC 1
#endif
#endif

// __int128_t nativo (GCC/Clang)
#if defined(__GNUC__) || defined(__clang__)
#ifndef _MSC_VER
#define HAS_INT128_T 1
#endif
#endif

// Optional: Boost multiprecision
#ifdef HAVE_BOOST
#include <boost/multiprecision/cpp_int.hpp>
using boost_int128 = boost::multiprecision::int128_t;

#ifdef HAVE_BOOST_GMP
#include <boost/multiprecision/gmp.hpp>
using boost_int128_gmp =
    boost::multiprecision::number<boost::multiprecision::gmp_int, boost::multiprecision::et_off>;
#endif

#ifdef HAVE_BOOST_TOMMATH
#include <boost/multiprecision/tommath.hpp>
using boost_int128_tommath = boost::multiprecision::number<boost::multiprecision::tommath_int,
                                                           boost::multiprecision::et_off>;
#endif
#endif

// ========================= INFRASTRUCTURE =========================

struct BenchmarkResult {
    std::string operation;
    std::string type;
    std::string compiler;
    std::string optimization;
    double time_ns;
    double cycles;
    size_t iterations;
    double ops_per_sec;
    std::string timestamp;
};

std::vector<BenchmarkResult> all_results;
std::mt19937_64 rng(std::random_device{}());

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
#elif defined(__GNUC__)
    return "GCC-" + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#elif defined(_MSC_VER)
    return "MSVC-" + std::to_string(_MSC_VER);
#else
    return "Unknown";
#endif
}

std::string get_optimization()
{
#ifdef __OPTIMIZE__
#if __OPTIMIZE__ >= 3
    return "O3";
#elif __OPTIMIZE__ >= 2
    return "O2";
#elif __OPTIMIZE__ >= 1
    return "O1";
#else
    return "O0";
#endif
#else
#ifdef NDEBUG
    return "O2";
#else
    return "O0";
#endif
#endif
}

inline uint64_t read_cpu_cycles()
{
#ifdef HAS_RDTSC
    return __rdtsc();
#else
    auto now = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return static_cast<uint64_t>(ns * 2.5);
#endif
}

template <typename Func>
void benchmark_operation(const std::string& op_name, const std::string& type_name, Func&& func,
                         size_t iterations = 1000000)
{
    // Warm-up
    for (size_t i = 0; i < iterations / 10; ++i) {
        func();
    }

    // Benchmark - Time
    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = read_cpu_cycles();

    for (size_t i = 0; i < iterations; ++i) {
        func();
    }

    uint64_t end_cycles = read_cpu_cycles();
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate metrics
    auto duration_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double time_per_op_ns = static_cast<double>(duration_ns) / iterations;
    double cycles_per_op = static_cast<double>(end_cycles - start_cycles) / iterations;
    double ops_per_sec = 1e9 / time_per_op_ns;

    // Store result
    BenchmarkResult result;
    result.operation = op_name;
    result.type = type_name;
    result.compiler = get_compiler();
    result.optimization = get_optimization();
    result.time_ns = time_per_op_ns;
    result.cycles = cycles_per_op;
    result.iterations = iterations;
    result.ops_per_sec = ops_per_sec;
    result.timestamp = get_timestamp();

    all_results.push_back(result);

    std::cout << op_name << " (" << type_name << "): " << std::fixed << std::setprecision(3)
              << time_per_op_ns << " ns, " << std::setprecision(2) << cycles_per_op << " cycles, "
              << std::setprecision(0) << ops_per_sec << " ops/sec" << std::endl;
}

// ========================= BENCHMARKS =========================

void benchmark_construction()
{
    std::cout << "Benchmarking Construction..." << std::endl;

    // int128_t
    benchmark_operation("construction_default", "int128_t", []() {
        volatile int128_t v;
        (void)v;
    });

    benchmark_operation("construction_from_int64", "int128_t", []() {
        volatile int128_t v(static_cast<int64_t>(rng()));
        (void)v;
    });

    // int64_t
    benchmark_operation("construction_default", "int64_t", []() {
        volatile int64_t v = 0;
        (void)v;
    });

    benchmark_operation("construction_from_int32", "int64_t", []() {
        volatile int64_t v(static_cast<int32_t>(rng()));
        (void)v;
    });

    // int32_t
    benchmark_operation("construction_default", "int32_t", []() {
        volatile int32_t v = 0;
        (void)v;
    });

#ifdef HAS_INT128_T
    benchmark_operation("construction_default", "__int128_t", []() {
        volatile __int128_t v = 0;
        (void)v;
    });

    benchmark_operation("construction_from_int64", "__int128_t", []() {
        volatile __int128_t v(static_cast<int64_t>(rng()));
        (void)v;
    });
#endif

#ifdef HAVE_BOOST
    benchmark_operation("construction_default", "boost_int128", []() {
        volatile boost_int128 v;
        (void)v;
    });

    benchmark_operation("construction_from_int64", "boost_int128", []() {
        volatile boost_int128 v(static_cast<int64_t>(rng()));
        (void)v;
    });

#ifdef HAVE_BOOST_GMP
    benchmark_operation("construction_default", "boost_gmp", []() {
        volatile boost_int128_gmp v;
        (void)v;
    });

    benchmark_operation("construction_from_int64", "boost_gmp", []() {
        volatile boost_int128_gmp v(static_cast<int64_t>(rng()));
        (void)v;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    benchmark_operation("construction_default", "boost_tommath", []() {
        volatile boost_int128_tommath v;
        (void)v;
    });

    benchmark_operation("construction_from_int64", "boost_tommath", []() {
        volatile boost_int128_tommath v(static_cast<int64_t>(rng()));
        (void)v;
    });
#endif
#endif
}

void benchmark_addition()
{
    std::cout << "Benchmarking Addition..." << std::endl;

    // int128_t
    int128_t a(static_cast<int64_t>(rng())), b(static_cast<int64_t>(rng()));
    benchmark_operation("addition", "int128_t", [&]() {
        volatile int128_t result = a + b;
        (void)result;
    });

    // int64_t
    int64_t a64 = static_cast<int64_t>(rng()), b64 = static_cast<int64_t>(rng());
    benchmark_operation("addition", "int64_t", [&]() {
        volatile int64_t result = a64 + b64;
        (void)result;
    });

    // int32_t
    int32_t a32 = static_cast<int32_t>(rng()), b32 = static_cast<int32_t>(rng());
    benchmark_operation("addition", "int32_t", [&]() {
        volatile int32_t result = a32 + b32;
        (void)result;
    });

#ifdef HAS_INT128_T
    __int128_t a128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    __int128_t b128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    benchmark_operation("addition", "__int128_t", [&]() {
        volatile __int128_t result = a128 + b128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_int128 ab(static_cast<int64_t>(rng())), bb(static_cast<int64_t>(rng()));
    benchmark_operation("addition", "boost_int128", [&]() {
        volatile boost_int128 result = ab + bb;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_int128_gmp ab_gmp(static_cast<int64_t>(rng())), bb_gmp(static_cast<int64_t>(rng()));
    benchmark_operation("addition", "boost_gmp", [&]() {
        volatile boost_int128_gmp result = ab_gmp + bb_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_int128_tommath ab_tm(static_cast<int64_t>(rng())), bb_tm(static_cast<int64_t>(rng()));
    benchmark_operation("addition", "boost_tommath", [&]() {
        volatile boost_int128_tommath result = ab_tm + bb_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_multiplication()
{
    std::cout << "Benchmarking Multiplication..." << std::endl;

    int128_t a(0, rng()), b(0, rng());
    benchmark_operation("multiplication", "int128_t", [&]() {
        volatile int128_t result = a * b;
        (void)result;
    });

    int64_t a64 = static_cast<int64_t>(rng()), b64 = static_cast<int64_t>(rng());
    benchmark_operation("multiplication", "int64_t", [&]() {
        volatile int64_t result = a64 * b64;
        (void)result;
    });

    int32_t a32 = static_cast<int32_t>(rng()), b32 = static_cast<int32_t>(rng());
    benchmark_operation("multiplication", "int32_t", [&]() {
        volatile int32_t result = a32 * b32;
        (void)result;
    });

#ifdef HAS_INT128_T
    __int128_t a128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    __int128_t b128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    benchmark_operation("multiplication", "__int128_t", [&]() {
        volatile __int128_t result = a128 * b128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_int128 ab(static_cast<int64_t>(rng())), bb(static_cast<int64_t>(rng()));
    benchmark_operation("multiplication", "boost_int128", [&]() {
        volatile boost_int128 result = ab * bb;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_int128_gmp ab_gmp(static_cast<int64_t>(rng())), bb_gmp(static_cast<int64_t>(rng()));
    benchmark_operation("multiplication", "boost_gmp", [&]() {
        volatile boost_int128_gmp result = ab_gmp * bb_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_int128_tommath ab_tm(static_cast<int64_t>(rng())), bb_tm(static_cast<int64_t>(rng()));
    benchmark_operation("multiplication", "boost_tommath", [&]() {
        volatile boost_int128_tommath result = ab_tm * bb_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_division()
{
    std::cout << "Benchmarking Division..." << std::endl;

    int128_t a(static_cast<int64_t>(rng())), b(0, static_cast<int64_t>(rng()) | 1);
    benchmark_operation(
        "division", "int128_t",
        [&]() {
            volatile int128_t result = a / b;
            (void)result;
        },
        100000);

    int64_t a64 = static_cast<int64_t>(rng()), b64 = static_cast<int64_t>(rng()) | 1;
    benchmark_operation(
        "division", "int64_t",
        [&]() {
            volatile int64_t result = a64 / b64;
            (void)result;
        },
        100000);

    int32_t a32 = static_cast<int32_t>(rng()), b32 = static_cast<int32_t>(rng()) | 1;
    benchmark_operation(
        "division", "int32_t",
        [&]() {
            volatile int32_t result = a32 / b32;
            (void)result;
        },
        100000);

#ifdef HAS_INT128_T
    __int128_t a128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    __int128_t b128 = static_cast<__int128_t>(static_cast<int64_t>(rng()) | 1);
    benchmark_operation(
        "division", "__int128_t",
        [&]() {
            volatile __int128_t result = a128 / b128;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST
    boost_int128 ab(static_cast<int64_t>(rng())), bb(static_cast<int64_t>(rng()) | 1);
    benchmark_operation(
        "division", "boost_int128",
        [&]() {
            volatile boost_int128 result = ab / bb;
            (void)result;
        },
        100000);

#ifdef HAVE_BOOST_GMP
    boost_int128_gmp ab_gmp(static_cast<int64_t>(rng())), bb_gmp(static_cast<int64_t>(rng()) | 1);
    benchmark_operation(
        "division", "boost_gmp",
        [&]() {
            volatile boost_int128_gmp result = ab_gmp / bb_gmp;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_int128_tommath ab_tm(static_cast<int64_t>(rng())), bb_tm(static_cast<int64_t>(rng()) | 1);
    benchmark_operation(
        "division", "boost_tommath",
        [&]() {
            volatile boost_int128_tommath result = ab_tm / bb_tm;
            (void)result;
        },
        100000);
#endif
#endif
}

void benchmark_comparisons()
{
    std::cout << "Benchmarking Comparisons..." << std::endl;

    int128_t a(static_cast<int64_t>(rng())), b(static_cast<int64_t>(rng()));

    benchmark_operation("comparison_eq", "int128_t", [&]() {
        volatile bool result = (a == b);
        (void)result;
    });

    benchmark_operation("comparison_lt", "int128_t", [&]() {
        volatile bool result = (a < b);
        (void)result;
    });

    int64_t a64 = static_cast<int64_t>(rng()), b64 = static_cast<int64_t>(rng());

    benchmark_operation("comparison_eq", "int64_t", [&]() {
        volatile bool result = (a64 == b64);
        (void)result;
    });

    benchmark_operation("comparison_lt", "int64_t", [&]() {
        volatile bool result = (a64 < b64);
        (void)result;
    });

#ifdef HAS_INT128_T
    __int128_t a128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));
    __int128_t b128 = static_cast<__int128_t>(static_cast<int64_t>(rng()));

    benchmark_operation("comparison_eq", "__int128_t", [&]() {
        volatile bool result = (a128 == b128);
        (void)result;
    });

    benchmark_operation("comparison_lt", "__int128_t", [&]() {
        volatile bool result = (a128 < b128);
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_int128 ab(static_cast<int64_t>(rng())), bb(static_cast<int64_t>(rng()));

    benchmark_operation("comparison_eq", "boost_int128", [&]() {
        volatile bool result = (ab == bb);
        (void)result;
    });

    benchmark_operation("comparison_lt", "boost_int128", [&]() {
        volatile bool result = (ab < bb);
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_int128_gmp ab_gmp(static_cast<int64_t>(rng())), bb_gmp(static_cast<int64_t>(rng()));

    benchmark_operation("comparison_eq", "boost_gmp", [&]() {
        volatile bool result = (ab_gmp == bb_gmp);
        (void)result;
    });

    benchmark_operation("comparison_lt", "boost_gmp", [&]() {
        volatile bool result = (ab_gmp < bb_gmp);
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_int128_tommath ab_tm(static_cast<int64_t>(rng())), bb_tm(static_cast<int64_t>(rng()));

    benchmark_operation("comparison_eq", "boost_tommath", [&]() {
        volatile bool result = (ab_tm == bb_tm);
        (void)result;
    });

    benchmark_operation("comparison_lt", "boost_tommath", [&]() {
        volatile bool result = (ab_tm < bb_tm);
        (void)result;
    });
#endif
#endif
}

void benchmark_string_conversion()
{
    std::cout << "Benchmarking String Conversions..." << std::endl;

    int128_t value(static_cast<int64_t>(rng()));

    benchmark_operation(
        "to_string_dec", "int128_t",
        [&]() {
            volatile auto result = value.to_string();
            (void)result;
        },
        10000);

    benchmark_operation(
        "from_string_dec", "int128_t",
        []() {
            volatile int128_t result = int128_t::from_string("123456789012345678901234567890");
            (void)result;
        },
        10000);

#ifdef HAVE_BOOST
    boost_int128 bvalue(static_cast<int64_t>(rng()));

    benchmark_operation(
        "to_string_dec", "boost_int128",
        [&]() {
            volatile auto result = bvalue.str();
            (void)result;
        },
        10000);

#ifdef HAVE_BOOST_GMP
    boost_int128_gmp bvalue_gmp(static_cast<int64_t>(rng()));

    benchmark_operation(
        "to_string_dec", "boost_gmp",
        [&]() {
            volatile auto result = bvalue_gmp.str();
            (void)result;
        },
        10000);
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_int128_tommath bvalue_tm(static_cast<int64_t>(rng()));

    benchmark_operation(
        "to_string_dec", "boost_tommath",
        [&]() {
            volatile auto result = bvalue_tm.str();
            (void)result;
        },
        10000);
#endif
#endif
}

// ========================= EXPORT FUNCTIONS =========================

void export_csv(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    file
        << "Operation,Type,Compiler,Optimization,Time_ns,Cycles,Iterations,Ops_per_sec,Timestamp\n";

    for (const auto& result : all_results) {
        file << result.operation << "," << result.type << "," << result.compiler << ","
             << result.optimization << "," << std::fixed << std::setprecision(3) << result.time_ns
             << "," << std::fixed << std::setprecision(2) << result.cycles << ","
             << result.iterations << "," << std::fixed << std::setprecision(0) << result.ops_per_sec
             << "," << result.timestamp << "\n";
    }

    file.close();
    std::cout << "Results exported to: " << filename << std::endl;
}

void export_json(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    file << "{\n";
    file << "  \"benchmark_results\": [\n";

    for (size_t i = 0; i < all_results.size(); ++i) {
        const auto& result = all_results[i];
        file << "    {\n";
        file << "      \"operation\": \"" << result.operation << "\",\n";
        file << "      \"type\": \"" << result.type << "\",\n";
        file << "      \"compiler\": \"" << result.compiler << "\",\n";
        file << "      \"optimization\": \"" << result.optimization << "\",\n";
        file << "      \"time_ns\": " << std::fixed << std::setprecision(3) << result.time_ns
             << ",\n";
        file << "      \"cycles\": " << std::fixed << std::setprecision(2) << result.cycles
             << ",\n";
        file << "      \"iterations\": " << result.iterations << ",\n";
        file << "      \"ops_per_sec\": " << std::fixed << std::setprecision(0)
             << result.ops_per_sec << ",\n";
        file << "      \"timestamp\": \"" << result.timestamp << "\"\n";
        file << "    }";
        if (i < all_results.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
    std::cout << "Results exported to: " << filename << std::endl;
}

// ========================= MAIN =========================

int main()
{
    std::cout << "=== int128_t Benchmarks ===" << std::endl;
    std::cout << "Compiler: " << get_compiler() << std::endl;
    std::cout << "Optimization: " << get_optimization() << std::endl;
    std::cout << std::endl;

    // Run benchmarks
    benchmark_construction();
    benchmark_addition();
    benchmark_multiplication();
    benchmark_division();
    benchmark_comparisons();
    benchmark_string_conversion();

    // Generate timestamp for filenames
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
#ifdef _WIN32
    struct tm tm_buf;
    localtime_s(&tm_buf, &time_t_now);
    ss << std::put_time(&tm_buf, "%Y%m%d_%H%M%S");
#else
    ss << std::put_time(std::localtime(&time_t_now), "%Y%m%d_%H%M%S");
#endif
    std::string timestamp = ss.str();

    // Export results
    std::string csv_file =
        "benchmark_results/int128_benchmark_" + get_compiler() + "_" + timestamp + ".csv";
    std::string json_file =
        "benchmark_results/int128_benchmark_" + get_compiler() + "_" + timestamp + ".json";

    export_csv(csv_file);
    export_json(json_file);

    std::cout << std::endl << "=== Benchmarks Complete ===" << std::endl;
    return 0;
}
