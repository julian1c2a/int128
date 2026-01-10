/**
 * @file uint128_extracted_benchmarks.cpp
 * @brief Comprehensive benchmarks for uint128_t comparing with built-in types and Boost
 *
 * This file benchmarks all major operations of uint128_t and compares with:
 * - Built-in types: uint64_t, uint32_t, __uint128_t (GCC/Clang)
 * - Boost multiprecision: cpp_int (pure C++), cpp_int with GMP, cpp_int with tommath
 *
 * Results are exported in CSV and JSON format for analysis
 */

#include "int128_base_tt.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace nstd;
// Para medir ciclos de CPU
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

// __uint128_t nativo (GCC/Clang)
#if defined(__GNUC__) || defined(__clang__)
#ifndef _MSC_VER
#define HAS_UINT128_T 1
#endif
#endif

// Optional: Boost multiprecision for comparison
#ifdef HAVE_BOOST
#include <boost/multiprecision/cpp_int.hpp>
using boost_uint128 = boost::multiprecision::uint128_t;

// Boost con GMP backend
#ifdef HAVE_BOOST_GMP
#include <boost/multiprecision/gmp.hpp>
using boost_uint128_gmp =
    boost::multiprecision::number<boost::multiprecision::gmp_int, boost::multiprecision::et_off>;
#endif

// Boost con tommath backend
#ifdef HAVE_BOOST_TOMMATH
#include <boost/multiprecision/tommath.hpp>
using boost_uint128_tommath = boost::multiprecision::number<boost::multiprecision::tommath_int,
                                                            boost::multiprecision::et_off>;
#endif
#endif

// ========================= BENCHMARK INFRASTRUCTURE =========================

struct BenchmarkResult {
    std::string operation;
    std::string type;
    std::string compiler;
    std::string optimization;
    double time_ns; // Tiempo promedio por operación en nanosegundos
    double cycles;  // Ciclos de CPU promedio por operación
    size_t iterations;
    double ops_per_sec; // Operaciones por segundo
    std::string timestamp;
};

std::vector<BenchmarkResult> all_results;

// Generador de números aleatorios
std::mt19937_64 rng(std::random_device{}());

// Obtener timestamp
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
#elif defined(__GNUC__)
    return "GCC-" + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
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
#else
#ifdef NDEBUG
    return "O2"; // Asumido
#else
    return "O0";
#endif
#endif
}

// Leer ciclos de CPU
inline uint64_t read_cpu_cycles()
{
#ifdef HAS_RDTSC
    return __rdtsc();
#else
    // Fallback: convertir tiempo a ciclos estimados (asumiendo 2.5 GHz)
    auto now = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return static_cast<uint64_t>(ns * 2.5); // 2.5 ciclos por nanosegundo
#endif
}

// Template para benchmarks genéricos
template <typename Func>
double benchmark_operation(const std::string& op_name, const std::string& type_name, Func&& func,
                           size_t iterations = 1000000)
{
    // Warm-up
    for (size_t i = 0; i < iterations / 10; ++i) {
        func();
    }

    // Benchmark real - Tiempo
    auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        func();
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    // Benchmark real - Ciclos (segunda ejecución para minimizar interferencia)
    uint64_t start_cycles = read_cpu_cycles();
    for (size_t i = 0; i < iterations; ++i) {
        func();
    }
    uint64_t end_cycles = read_cpu_cycles();

    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double time_per_op = static_cast<double>(duration) / iterations;
    double cycles_per_op = static_cast<double>(end_cycles - start_cycles) / iterations;
    double ops_per_sec = 1e9 / time_per_op;

    BenchmarkResult result;
    result.operation = op_name;
    result.type = type_name;
    result.compiler = get_compiler();
    result.optimization = get_optimization();
    result.time_ns = time_per_op;
    result.cycles = cycles_per_op;
    result.iterations = iterations;
    result.ops_per_sec = ops_per_sec;
    result.timestamp = get_timestamp();

    all_results.push_back(result);

    return time_per_op;
}

// ========================= CONSTRUCTION BENCHMARKS =========================

void benchmark_construction()
{
    std::cout << "Benchmarking Construction..." << std::endl;

    // uint128_t construction
    benchmark_operation("construction_default", "uint128_t", []() {
        volatile uint128_t v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "uint128_t", []() {
        volatile uint128_t v(rng());
        (void)v;
    });

    benchmark_operation("construction_from_two_uint64", "uint128_t", []() {
        volatile uint128_t v(rng(), rng());
        (void)v;
    });

    // uint64_t construction (comparison)
    benchmark_operation("construction_default", "uint64_t", []() {
        volatile uint64_t v = 0;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "uint64_t", []() {
        volatile uint64_t v = rng();
        (void)v;
    });

    // uint32_t construction (comparison)
    benchmark_operation("construction_default", "uint32_t", []() {
        volatile uint32_t v = 0;
        (void)v;
    });

#ifdef HAS_UINT128_T
    benchmark_operation("construction_default", "__uint128_t", []() {
        volatile __uint128_t v = 0;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "__uint128_t", []() {
        volatile __uint128_t v = rng();
        (void)v;
    });
#endif

#ifdef HAVE_BOOST
    benchmark_operation("construction_default", "boost_uint128", []() {
        volatile boost_uint128 v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "boost_uint128", []() {
        volatile boost_uint128 v(rng());
        (void)v;
    });

#ifdef HAVE_BOOST_GMP
    benchmark_operation("construction_default", "boost_gmp", []() {
        volatile boost_uint128_gmp v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "boost_gmp", []() {
        volatile boost_uint128_gmp v(rng());
        (void)v;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    benchmark_operation("construction_default", "boost_tommath", []() {
        volatile boost_uint128_tommath v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "boost_tommath", []() {
        volatile boost_uint128_tommath v(rng());
        (void)v;
    });
#endif
#endif
}

// ========================= ARITHMETIC BENCHMARKS =========================

void benchmark_addition()
{
    std::cout << "Benchmarking Addition..." << std::endl;

    // uint128_t
    uint128_t a(rng(), rng()), b(rng(), rng());
    benchmark_operation("addition", "uint128_t", [&]() {
        volatile uint128_t result = a + b;
        (void)result;
    });

    // uint64_t
    uint64_t a64 = rng(), b64 = rng();
    benchmark_operation("addition", "uint64_t", [&]() {
        volatile uint64_t result = a64 + b64;
        (void)result;
    });

    // uint32_t
    uint32_t a32 = rng(), b32 = rng();
    benchmark_operation("addition", "uint32_t", [&]() {
        volatile uint32_t result = a32 + b32;
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = (__uint128_t)rng() << 64 | rng();
    benchmark_operation("addition", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 + b128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("addition", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab + bb;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng());
    benchmark_operation("addition", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp + bb_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng());
    benchmark_operation("addition", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm + bb_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_subtraction()
{
    std::cout << "Benchmarking Subtraction..." << std::endl;

    uint128_t a(rng(), rng()), b(rng(), rng());
    benchmark_operation("subtraction", "uint128_t", [&]() {
        volatile uint128_t result = a - b;
        (void)result;
    });

    uint64_t a64 = rng(), b64 = rng();
    benchmark_operation("subtraction", "uint64_t", [&]() {
        volatile uint64_t result = a64 - b64;
        (void)result;
    });

    uint32_t a32 = rng(), b32 = rng();
    benchmark_operation("subtraction", "uint32_t", [&]() {
        volatile uint32_t result = a32 - b32;
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = (__uint128_t)rng() << 64 | rng();
    benchmark_operation("subtraction", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 - b128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("subtraction", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab - bb;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng());
    benchmark_operation("subtraction", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp - bb_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng());
    benchmark_operation("subtraction", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm - bb_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_multiplication()
{
    std::cout << "Benchmarking Multiplication..." << std::endl;

    uint128_t a(0, rng()), b(0, rng());
    benchmark_operation("multiplication", "uint128_t", [&]() {
        volatile uint128_t result = a * b;
        (void)result;
    });

    uint64_t a64 = rng(), b64 = rng();
    benchmark_operation("multiplication", "uint64_t", [&]() {
        volatile uint64_t result = a64 * b64;
        (void)result;
    });

    uint32_t a32 = rng(), b32 = rng();
    benchmark_operation("multiplication", "uint32_t", [&]() {
        volatile uint32_t result = a32 * b32;
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = rng();
    __uint128_t b128 = rng();
    benchmark_operation("multiplication", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 * b128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("multiplication", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab * bb;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng());
    benchmark_operation("multiplication", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp * bb_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng());
    benchmark_operation("multiplication", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm * bb_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_division()
{
    std::cout << "Benchmarking Division..." << std::endl;

    uint128_t a(rng(), rng()), b(0, rng() | 1);
    benchmark_operation(
        "division", "uint128_t",
        [&]() {
            volatile uint128_t result = a / b;
            (void)result;
        },
        100000); // Menos iteraciones (división es lenta)

    uint64_t a64 = rng(), b64 = rng() | 1;
    benchmark_operation(
        "division", "uint64_t",
        [&]() {
            volatile uint64_t result = a64 / b64;
            (void)result;
        },
        100000);

    uint32_t a32 = rng(), b32 = rng() | 1;
    benchmark_operation(
        "division", "uint32_t",
        [&]() {
            volatile uint32_t result = a32 / b32;
            (void)result;
        },
        100000);

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = rng() | 1;
    benchmark_operation(
        "division", "__uint128_t",
        [&]() {
            volatile __uint128_t result = a128 / b128;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng() | 1);
    benchmark_operation(
        "division", "boost_uint128",
        [&]() {
            volatile boost_uint128 result = ab / bb;
            (void)result;
        },
        100000);

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng() | 1);
    benchmark_operation(
        "division", "boost_gmp",
        [&]() {
            volatile boost_uint128_gmp result = ab_gmp / bb_gmp;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng() | 1);
    benchmark_operation(
        "division", "boost_tommath",
        [&]() {
            volatile boost_uint128_tommath result = ab_tm / bb_tm;
            (void)result;
        },
        100000);
#endif
#endif
}

// ========================= DIVISION ALGORITHMS COMPARISON =========================

void benchmark_division_algorithms()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Division Algorithms Comparison" << std::endl;
    std::cout << "Comparing divrem() vs knuth_D_divrem()" << std::endl;
    std::cout << "========================================\n" << std::endl;

    struct ComparisonResult {
        std::string test_name;
        double divrem_time_ns;
        double knuth_time_ns;
        double divrem_cycles;
        double knuth_cycles;
    };
    std::vector<ComparisonResult> comparisons;

    // Test Case 1: Small numbers (64-bit dividend, 64-bit divisor)
    std::cout << "Test 1: Small numbers (64-bit range)..." << std::endl;
    {
        uint128_t dividend(0, rng());
        uint128_t divisor(0, rng() | 1);

        double divrem_time = benchmark_operation(
            "division_small_divrem", "uint128_t",
            [&]() {
                volatile auto result = dividend.divrem(divisor);
                (void)result;
            },
            100000);

        double knuth_time = benchmark_operation(
            "division_small_knuth_D", "uint128_t",
            [&]() {
                volatile auto result = dividend.knuth_D_divrem(divisor);
                (void)result;
            },
            100000);

        comparisons.push_back({"Small (64-bit)", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Test Case 2: Large dividend, small divisor (common case)
    std::cout << "Test 2: Large dividend, small divisor..." << std::endl;
    {
        uint128_t dividend(rng(), rng());
        uint128_t divisor(0, rng() | 1);

        double divrem_time = benchmark_operation(
            "division_large_small_divrem", "uint128_t",
            [&]() {
                volatile auto result = dividend.divrem(divisor);
                (void)result;
            },
            100000);

        double knuth_time = benchmark_operation(
            "division_large_small_knuth_D", "uint128_t",
            [&]() {
                volatile auto result = dividend.knuth_D_divrem(divisor);
                (void)result;
            },
            100000);

        comparisons.push_back({"Large/Small", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Test Case 3: Both large (128-bit dividend, 128-bit divisor)
    std::cout << "Test 3: Both large (full 128-bit)..." << std::endl;
    {
        uint128_t dividend(rng(), rng());
        uint128_t divisor(rng() | 1, rng() | 1);

        double divrem_time = benchmark_operation(
            "division_both_large_divrem", "uint128_t",
            [&]() {
                volatile auto result = dividend.divrem(divisor);
                (void)result;
            },
            100000);

        double knuth_time = benchmark_operation(
            "division_both_large_knuth_D", "uint128_t",
            [&]() {
                volatile auto result = dividend.knuth_D_divrem(divisor);
                (void)result;
            },
            100000);

        comparisons.push_back({"Both Large (128-bit)", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Test Case 4: Power of 2 divisor (optimization path)
    std::cout << "Test 4: Power of 2 divisor (optimization)..." << std::endl;
    {
        uint128_t dividend(rng(), rng());
        uint128_t divisor(0, 1ULL << (rng() % 63)); // Random power of 2

        double divrem_time = benchmark_operation(
            "division_pow2_divrem", "uint128_t",
            [&]() {
                volatile auto result = dividend.divrem(divisor);
                (void)result;
            },
            100000);

        double knuth_time = benchmark_operation(
            "division_pow2_knuth_D", "uint128_t",
            [&]() {
                volatile auto result = dividend.knuth_D_divrem(divisor);
                (void)result;
            },
            100000);

        comparisons.push_back({"Power of 2", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Test Case 5: Worst case - requires normalization and correction
    std::cout << "Test 5: Worst case (needs normalization)..." << std::endl;
    {
        // Create numbers that require full Knuth D processing
        uint128_t dividend(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
        uint128_t divisor(0x4000000000000000ULL, 0x0000000000000001ULL);

        double divrem_time = benchmark_operation(
            "division_worst_divrem", "uint128_t",
            [&]() {
                volatile auto result = dividend.divrem(divisor);
                (void)result;
            },
            100000);

        double knuth_time = benchmark_operation(
            "division_worst_knuth_D", "uint128_t",
            [&]() {
                volatile auto result = dividend.knuth_D_divrem(divisor);
                (void)result;
            },
            100000);

        comparisons.push_back({"Worst Case", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Test Case 6: Random cases (statistical average)
    std::cout << "Test 6: Random cases (1000 samples)..." << std::endl;
    {
        std::vector<std::pair<uint128_t, uint128_t>> test_cases;
        test_cases.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            uint64_t div_hi = (rng() % 3 == 0) ? 0 : rng(); // 33% chance of 64-bit divisor
            test_cases.emplace_back(uint128_t(rng(), rng()), uint128_t(div_hi, rng() | 1));
        }

        size_t idx = 0;
        double divrem_time = benchmark_operation(
            "division_random_divrem", "uint128_t",
            [&]() {
                volatile auto result =
                    test_cases[idx % 1000].first.divrem(test_cases[idx % 1000].second);
                idx++;
                (void)result;
            },
            100000);

        idx = 0;
        double knuth_time = benchmark_operation(
            "division_random_knuth_D", "uint128_t",
            [&]() {
                volatile auto result =
                    test_cases[idx % 1000].first.knuth_D_divrem(test_cases[idx % 1000].second);
                idx++;
                (void)result;
            },
            100000);

        comparisons.push_back({"Random Mix", divrem_time, knuth_time,
                               all_results[all_results.size() - 2].cycles,
                               all_results[all_results.size() - 1].cycles});
    }

    // Print comparison table
    std::cout << "\n========================================" << std::endl;
    std::cout << "DIVISION ALGORITHMS RESULTS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::left << std::setw(25) << "Test Case" << std::right << std::setw(15)
              << "divrem (ns)" << std::setw(15) << "knuth_D (ns)" << std::setw(12) << "Ratio"
              << std::setw(15) << "divrem (cy)" << std::setw(15) << "knuth_D (cy)" << std::setw(12)
              << "Ratio" << std::endl;
    std::cout << std::string(108, '-') << std::endl;

    for (const auto& cmp : comparisons) {
        double time_ratio = cmp.knuth_time_ns / cmp.divrem_time_ns;
        double cycles_ratio = cmp.knuth_cycles / cmp.divrem_cycles;
        std::cout << std::left << std::setw(25) << cmp.test_name << std::right << std::fixed
                  << std::setprecision(2) << std::setw(15) << cmp.divrem_time_ns << std::setw(15)
                  << cmp.knuth_time_ns << std::setw(12) << time_ratio << "x" << std::setw(14)
                  << cmp.divrem_cycles << std::setw(15) << cmp.knuth_cycles << std::setw(12)
                  << cycles_ratio << "x" << std::endl;
    }
    std::cout << std::string(108, '-') << std::endl;
    std::cout << "Note: Ratio > 1.0 means knuth_D is SLOWER than divrem" << std::endl;
    std::cout << "      Ratio < 1.0 means knuth_D is FASTER than divrem" << std::endl;

    std::cout << "\nDivision algorithms comparison completed.\n" << std::endl;
}

void benchmark_modulo()
{
    std::cout << "Benchmarking Modulo..." << std::endl;

    uint128_t a(rng(), rng()), b(0, rng() | 1);
    benchmark_operation(
        "modulo", "uint128_t",
        [&]() {
            volatile uint128_t result = a % b;
            (void)result;
        },
        100000);

    uint64_t a64 = rng(), b64 = rng() | 1;
    benchmark_operation(
        "modulo", "uint64_t",
        [&]() {
            volatile uint64_t result = a64 % b64;
            (void)result;
        },
        100000);

    uint32_t a32 = rng(), b32 = rng() | 1;
    benchmark_operation(
        "modulo", "uint32_t",
        [&]() {
            volatile uint32_t result = a32 % b32;
            (void)result;
        },
        100000);

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = rng() | 1;
    benchmark_operation(
        "modulo", "__uint128_t",
        [&]() {
            volatile __uint128_t result = a128 % b128;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng() | 1);
    benchmark_operation(
        "modulo", "boost_uint128",
        [&]() {
            volatile boost_uint128 result = ab % bb;
            (void)result;
        },
        100000);

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng() | 1);
    benchmark_operation(
        "modulo", "boost_gmp",
        [&]() {
            volatile boost_uint128_gmp result = ab_gmp % bb_gmp;
            (void)result;
        },
        100000);
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng() | 1);
    benchmark_operation(
        "modulo", "boost_tommath",
        [&]() {
            volatile boost_uint128_tommath result = ab_tm % bb_tm;
            (void)result;
        },
        100000);
#endif
#endif
}

// ========================= BITWISE BENCHMARKS =========================

void benchmark_bitwise()
{
    std::cout << "Benchmarking Bitwise Operations..." << std::endl;

    uint128_t a(rng(), rng()), b(rng(), rng());

    benchmark_operation("bitwise_and", "uint128_t", [&]() {
        volatile uint128_t result = a & b;
        (void)result;
    });

    benchmark_operation("bitwise_or", "uint128_t", [&]() {
        volatile uint128_t result = a | b;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "uint128_t", [&]() {
        volatile uint128_t result = a ^ b;
        (void)result;
    });

    benchmark_operation("bitwise_not", "uint128_t", [&]() {
        volatile uint128_t result = ~a;
        (void)result;
    });

    // uint64_t comparison
    uint64_t a64 = rng(), b64 = rng();

    benchmark_operation("bitwise_and", "uint64_t", [&]() {
        volatile uint64_t result = a64 & b64;
        (void)result;
    });

    benchmark_operation("bitwise_or", "uint64_t", [&]() {
        volatile uint64_t result = a64 | b64;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "uint64_t", [&]() {
        volatile uint64_t result = a64 ^ b64;
        (void)result;
    });

    benchmark_operation("bitwise_not", "uint64_t", [&]() {
        volatile uint64_t result = ~a64;
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = (__uint128_t)rng() << 64 | rng();

    benchmark_operation("bitwise_and", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 & b128;
        (void)result;
    });

    benchmark_operation("bitwise_or", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 | b128;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 ^ b128;
        (void)result;
    });

    benchmark_operation("bitwise_not", "__uint128_t", [&]() {
        volatile __uint128_t result = ~a128;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());

    benchmark_operation("bitwise_and", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab & bb;
        (void)result;
    });

    benchmark_operation("bitwise_or", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab | bb;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab ^ bb;
        (void)result;
    });

    benchmark_operation("bitwise_not", "boost_uint128", [&]() {
        volatile boost_uint128 result = ~ab;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng());

    benchmark_operation("bitwise_and", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp & bb_gmp;
        (void)result;
    });

    benchmark_operation("bitwise_or", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp | bb_gmp;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp ^ bb_gmp;
        (void)result;
    });

    benchmark_operation("bitwise_not", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ~ab_gmp;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng());

    benchmark_operation("bitwise_and", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm & bb_tm;
        (void)result;
    });

    benchmark_operation("bitwise_or", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm | bb_tm;
        (void)result;
    });

    benchmark_operation("bitwise_xor", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm ^ bb_tm;
        (void)result;
    });

    benchmark_operation("bitwise_not", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ~ab_tm;
        (void)result;
    });
#endif
#endif
}

void benchmark_shifts()
{
    std::cout << "Benchmarking Shift Operations..." << std::endl;

    uint128_t a(rng(), rng());

    benchmark_operation("shift_left_8", "uint128_t", [&]() {
        volatile uint128_t result = a << 8;
        (void)result;
    });

    benchmark_operation("shift_left_64", "uint128_t", [&]() {
        volatile uint128_t result = a << 64;
        (void)result;
    });

    benchmark_operation("shift_right_8", "uint128_t", [&]() {
        volatile uint128_t result = a >> 8;
        (void)result;
    });

    benchmark_operation("shift_right_64", "uint128_t", [&]() {
        volatile uint128_t result = a >> 64;
        (void)result;
    });

    // uint64_t comparison
    uint64_t a64 = rng();

    benchmark_operation("shift_left_8", "uint64_t", [&]() {
        volatile uint64_t result = a64 << 8;
        (void)result;
    });

    benchmark_operation("shift_right_8", "uint64_t", [&]() {
        volatile uint64_t result = a64 >> 8;
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();

    benchmark_operation("shift_left_8", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 << 8;
        (void)result;
    });

    benchmark_operation("shift_left_64", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 << 64;
        (void)result;
    });

    benchmark_operation("shift_right_8", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 >> 8;
        (void)result;
    });

    benchmark_operation("shift_right_64", "__uint128_t", [&]() {
        volatile __uint128_t result = a128 >> 64;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng());

    benchmark_operation("shift_left_8", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab << 8;
        (void)result;
    });

    benchmark_operation("shift_left_64", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab << 64;
        (void)result;
    });

    benchmark_operation("shift_right_8", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab >> 8;
        (void)result;
    });

    benchmark_operation("shift_right_64", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab >> 64;
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng());

    benchmark_operation("shift_left_8", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp << 8;
        (void)result;
    });

    benchmark_operation("shift_left_64", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp << 64;
        (void)result;
    });

    benchmark_operation("shift_right_8", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp >> 8;
        (void)result;
    });

    benchmark_operation("shift_right_64", "boost_gmp", [&]() {
        volatile boost_uint128_gmp result = ab_gmp >> 64;
        (void)result;
    });
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath ab_tm(rng());

    benchmark_operation("shift_left_8", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm << 8;
        (void)result;
    });

    benchmark_operation("shift_left_64", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm << 64;
        (void)result;
    });

    benchmark_operation("shift_right_8", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm >> 8;
        (void)result;
    });

    benchmark_operation("shift_right_64", "boost_tommath", [&]() {
        volatile boost_uint128_tommath result = ab_tm >> 64;
        (void)result;
    });
#endif
#endif
}

// ========================= COMPARISON BENCHMARKS =========================

void benchmark_comparisons()
{
    std::cout << "Benchmarking Comparisons..." << std::endl;

    uint128_t a(rng(), rng()), b(rng(), rng());

    benchmark_operation("comparison_eq", "uint128_t", [&]() {
        volatile bool result = (a == b);
        (void)result;
    });

    benchmark_operation("comparison_ne", "uint128_t", [&]() {
        volatile bool result = (a != b);
        (void)result;
    });

    benchmark_operation("comparison_lt", "uint128_t", [&]() {
        volatile bool result = (a < b);
        (void)result;
    });

    benchmark_operation("comparison_le", "uint128_t", [&]() {
        volatile bool result = (a <= b);
        (void)result;
    });

    benchmark_operation("comparison_gt", "uint128_t", [&]() {
        volatile bool result = (a > b);
        (void)result;
    });

    benchmark_operation("comparison_ge", "uint128_t", [&]() {
        volatile bool result = (a >= b);
        (void)result;
    });

    // uint64_t comparison
    uint64_t a64 = rng(), b64 = rng();

    benchmark_operation("comparison_eq", "uint64_t", [&]() {
        volatile bool result = (a64 == b64);
        (void)result;
    });

    benchmark_operation("comparison_lt", "uint64_t", [&]() {
        volatile bool result = (a64 < b64);
        (void)result;
    });

#ifdef HAS_UINT128_T
    __uint128_t a128 = (__uint128_t)rng() << 64 | rng();
    __uint128_t b128 = (__uint128_t)rng() << 64 | rng();

    benchmark_operation("comparison_eq", "__uint128_t", [&]() {
        volatile bool result = (a128 == b128);
        (void)result;
    });

    benchmark_operation("comparison_ne", "__uint128_t", [&]() {
        volatile bool result = (a128 != b128);
        (void)result;
    });

    benchmark_operation("comparison_lt", "__uint128_t", [&]() {
        volatile bool result = (a128 < b128);
        (void)result;
    });

    benchmark_operation("comparison_le", "__uint128_t", [&]() {
        volatile bool result = (a128 <= b128);
        (void)result;
    });

    benchmark_operation("comparison_gt", "__uint128_t", [&]() {
        volatile bool result = (a128 > b128);
        (void)result;
    });

    benchmark_operation("comparison_ge", "__uint128_t", [&]() {
        volatile bool result = (a128 >= b128);
        (void)result;
    });
#endif

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());

    benchmark_operation("comparison_eq", "boost_uint128", [&]() {
        volatile bool result = (ab == bb);
        (void)result;
    });

    benchmark_operation("comparison_lt", "boost_uint128", [&]() {
        volatile bool result = (ab < bb);
        (void)result;
    });

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp ab_gmp(rng()), bb_gmp(rng());

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
    boost_uint128_tommath ab_tm(rng()), bb_tm(rng());

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

// ========================= STRING CONVERSION BENCHMARKS =========================

void benchmark_string_conversion()
{
    std::cout << "Benchmarking String Conversions..." << std::endl;

    uint128_t value(rng(), rng());

    benchmark_operation(
        "to_string_dec", "uint128_t",
        [&]() {
            volatile auto result = value.to_string();
            (void)result;
        },
        10000);

    benchmark_operation(
        "to_string_hex", "uint128_t",
        [&]() {
            volatile auto result = value.to_string_base(16);
            (void)result;
        },
        10000);

    benchmark_operation(
        "from_string_dec", "uint128_t",
        []() {
            volatile uint128_t result = uint128_t::from_string("123456789012345678901234567890");
            (void)result;
        },
        10000);

    benchmark_operation(
        "from_string_hex", "uint128_t",
        []() {
            volatile uint128_t result = uint128_t::from_string_base("FEDCBA9876543210", 16);
            (void)result;
        },
        10000);

#ifdef HAVE_BOOST
    boost_uint128 bvalue(rng());

    benchmark_operation(
        "to_string_dec", "boost_uint128",
        [&]() {
            volatile auto result = bvalue.str();
            (void)result;
        },
        10000);

#ifdef HAVE_BOOST_GMP
    boost_uint128_gmp bvalue_gmp(rng());

    benchmark_operation(
        "to_string_dec", "boost_gmp",
        [&]() {
            volatile auto result = bvalue_gmp.str();
            (void)result;
        },
        10000);
#endif

#ifdef HAVE_BOOST_TOMMATH
    boost_uint128_tommath bvalue_tm(rng());

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

// ========================= RESULTS EXPORT =========================

void export_csv(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    // Header
    file
        << "Operation,Type,Compiler,Optimization,Time_ns,Cycles,Iterations,Ops_per_sec,Timestamp\n";

    // Data
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

void print_summary()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "BENCHMARK SUMMARY" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Compiler: " << get_compiler() << std::endl;
    std::cout << "Optimization: " << get_optimization() << std::endl;
    std::cout << "Total operations benchmarked: " << all_results.size() << std::endl;
    std::cout << "========================================" << std::endl;
}

// ========================= MAIN =========================

int main(int argc, char* argv[])
{
    std::cout << "========================================" << std::endl;
    std::cout << "uint128_t Comprehensive Benchmarks" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Compiler: " << get_compiler() << std::endl;
    std::cout << "Optimization: " << get_optimization() << std::endl;
#ifdef HAVE_BOOST
    std::cout << "Boost: Enabled" << std::endl;
#else
    std::cout << "Boost: Disabled (compile with -DHAVE_BOOST to enable)" << std::endl;
#endif
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Run all benchmarks
    benchmark_construction();
    benchmark_addition();
    benchmark_subtraction();
    benchmark_multiplication();
    benchmark_division();
    benchmark_division_algorithms(); // NEW: Compare divrem vs knuth_D_divrem
    benchmark_modulo();
    benchmark_bitwise();
    benchmark_shifts();
    benchmark_comparisons();
    benchmark_string_conversion();

    // Print summary
    print_summary();

    // Export results
    std::string compiler_name = get_compiler();
    std::replace(compiler_name.begin(), compiler_name.end(), '.', '_');
    std::replace(compiler_name.begin(), compiler_name.end(), '-', '_');

    std::string csv_filename = "benchmark_results/uint128_benchmarks_" + compiler_name + ".csv";
    std::string json_filename = "benchmark_results/uint128_benchmarks_" + compiler_name + ".json";

    // Allow custom output file from command line
    if (argc > 1) {
        csv_filename = std::string(argv[1]) + ".csv";
        json_filename = std::string(argv[1]) + ".json";
    }

    export_csv(csv_filename);
    export_json(json_filename);

    std::cout << "\nBenchmarks completed successfully!" << std::endl;

    return 0;
}
