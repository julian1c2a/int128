/**
 * @file uint128_extracted_benchmarks.cpp
 * @brief Comprehensive benchmarks for uint128_t comparing with built-in types and Boost
 *
 * This file benchmarks all major operations of uint128_t and compares with:
 * - Built-in types: uint64_t, uint32_t
 * - Boost multiprecision: cpp_int (pure C++, no GMP/tommath backend)
 *
 * Results are exported in CSV and JSON format for analysis
 */

#include "../include/uint128/uint128_t.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Para medir ciclos de CPU
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#ifdef _MSC_VER
#include <intrin.h>
#define HAS_RDTSC 1
#elif defined(__GNUC__) || defined(__clang__)
#include <x86intrin.h>
#define HAS_RDTSC 1
#endif
#endif

// Optional: Boost multiprecision for comparison
#ifdef HAVE_BOOST
#include <boost/multiprecision/cpp_int.hpp>
using boost_uint128 = boost::multiprecision::uint128_t;
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
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
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
        volatile uint64_t v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "uint64_t", []() {
        volatile uint64_t v = rng();
        (void)v;
    });

    // uint32_t construction (comparison)
    benchmark_operation("construction_default", "uint32_t", []() {
        volatile uint32_t v;
        (void)v;
    });

#ifdef HAVE_BOOST
    benchmark_operation("construction_default", "boost_uint128", []() {
        volatile boost_uint128 v;
        (void)v;
    });

    benchmark_operation("construction_from_uint64", "boost_uint128", []() {
        volatile boost_uint128 v(rng());
        (void)v;
    });
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

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("addition", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab + bb;
        (void)result;
    });
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

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("subtraction", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab - bb;
        (void)result;
    });
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

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("multiplication", "boost_uint128", [&]() {
        volatile boost_uint128 result = ab * bb;
        (void)result;
    });
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

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng() | 1);
    benchmark_operation(
        "division", "boost_uint128",
        [&]() {
            volatile boost_uint128 result = ab / bb;
            (void)result;
        },
        100000);
#endif
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

#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng() | 1);
    benchmark_operation(
        "modulo", "boost_uint128",
        [&]() {
            volatile boost_uint128 result = ab % bb;
            (void)result;
        },
        100000);
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
