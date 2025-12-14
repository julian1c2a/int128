#include "../include/int128.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std::chrono;

struct BenchmarkResult {
    std::string operation;
    double time_ns;
    size_t iterations;
    double ops_per_second;
};

void writeCSV(const std::vector<BenchmarkResult>& results, const std::string& filename)
{
    std::ofstream file("../documentation/benchmarks/" + filename);
    file << "Operation,Time_ns,Iterations,Ops_per_second\n";
    for (const auto& result : results) {
        file << result.operation << "," << result.time_ns << "," << result.iterations << ","
             << result.ops_per_second << "\n";
    }
}

template <typename Func>
BenchmarkResult benchmark(const std::string& name, Func func, size_t iterations = 1000000)
{
    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        func();
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);

    double time_ns = static_cast<double>(duration.count()) / iterations;
    double ops_per_second = 1000000000.0 / time_ns;

    return {name, time_ns, iterations, ops_per_second};
}

int main()
{
    std::cout << "=== uint128_t Benchmarks ===\n\n";

    // Setup random numbers for testing
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    uint128_t a(dist(gen), dist(gen));
    uint128_t b(dist(gen), dist(gen));

    std::vector<BenchmarkResult> arithmetic_results;
    std::vector<BenchmarkResult> comparison_results;

    std::cout << "Running arithmetic benchmarks...\n";

    // Addition
    arithmetic_results.push_back(benchmark("Addition", [&]() { volatile auto result = a + b; }));

    // Subtraction
    arithmetic_results.push_back(benchmark("Subtraction", [&]() { volatile auto result = a - b; }));

    // Multiplication
    arithmetic_results.push_back(benchmark(
        "Multiplication", [&]() { volatile auto result = a * b; },
        100000)); // Fewer iterations for slower operations

    // Bitwise AND
    arithmetic_results.push_back(benchmark("Bitwise_AND", [&]() { volatile auto result = a & b; }));

    // Left shift
    arithmetic_results.push_back(benchmark("Left_Shift", [&]() { volatile auto result = a << 1; }));

    std::cout << "Running comparison benchmarks...\n";

    // Equality
    comparison_results.push_back(benchmark("Equality", [&]() { volatile bool result = a == b; }));

    // Less than
    comparison_results.push_back(benchmark("Less_Than", [&]() { volatile bool result = a < b; }));

    // Write results to CSV
    writeCSV(arithmetic_results, "arithmetic_operations.csv");
    writeCSV(comparison_results, "comparison_operations.csv");

    std::cout << "\nArithmetic Operations Results:\n";
    std::cout << "Operation\t\tTime (ns)\tOps/sec\n";
    std::cout << "----------------------------------------\n";
    for (const auto& result : arithmetic_results) {
        std::cout << result.operation << "\t\t" << std::fixed << std::setprecision(2)
                  << result.time_ns << "\t\t" << std::scientific << result.ops_per_second << "\n";
    }

    std::cout << "\nComparison Operations Results:\n";
    std::cout << "Operation\t\tTime (ns)\tOps/sec\n";
    std::cout << "----------------------------------------\n";
    for (const auto& result : comparison_results) {
        std::cout << result.operation << "\t\t" << std::fixed << std::setprecision(2)
                  << result.time_ns << "\t\t" << std::scientific << result.ops_per_second << "\n";
    }

    std::cout << "\nCSV files saved to documentation/benchmarks/\n";

    return 0;
}