#include "benchmark_suite.hpp"
#include "include/uint128_t.hpp"
#include <iostream>

// Versión rápida para testing
int main()
{
    std::cout << "🧪 QUICK BENCHMARK TEST" << std::endl;
    std::cout << "======================" << std::endl;

    BenchmarkRunner runner;

    // Usar menos iteraciones para prueba rápida
    const size_t quick_iterations = 100'000;

    std::cout << "Ejecutando con " << quick_iterations << " iteraciones..." << std::endl;

    // Test aritmético básico
    auto pair64 = runner.random_pair<uint64_t>();
    uint64_t a64 = pair64.first, b64 = pair64.second;

    auto pair128 = runner.random_pair<uint128_t>();
    uint128_t a128 = pair128.first, b128 = pair128.second;

    // Benchmarks de suma
    runner.run_benchmark(
        "Addition", "uint64_t",
        [&]() {
            volatile auto result = a64 + b64;
            (void)result;
        },
        quick_iterations);

    runner.run_benchmark(
        "Addition", "uint128_t",
        [&]() {
            volatile auto result = a128 + b128;
            (void)result;
        },
        quick_iterations);

    // Benchmarks de división
    if (b64 == 0)
        b64 = 1;
    if (b128 == uint128_t(0, 0))
        b128 = uint128_t(0, 1);

    runner.run_benchmark(
        "Division", "uint64_t",
        [&]() {
            volatile auto result = a64 / b64;
            (void)result;
        },
        quick_iterations);

    runner.run_benchmark(
        "Division", "uint128_t",
        [&]() {
            volatile auto result = a128 / b128;
            (void)result;
        },
        quick_iterations);

    // Test específico uint128_t
    runner.run_benchmark(
        "DivRem", "uint128_t",
        [&]() {
            volatile auto result = a128.divrem(b128);
            (void)result;
        },
        quick_iterations);

    runner.run_benchmark(
        "ToString", "uint128_t",
        [&]() {
            volatile auto result = a128.to_string();
            (void)result;
        },
        quick_iterations);

    // Mostrar resultados
    runner.print_results();

    // Comparación rápida
    runner.compare_performance("Addition", {"uint64_t", "uint128_t"});
    runner.compare_performance("Division", {"uint64_t", "uint128_t"});

    // Exportar resultados
    runner.export_to_csv("quick_benchmark.csv");

    std::cout << "\n🎉 Quick benchmark completado!" << std::endl;
    return 0;
}