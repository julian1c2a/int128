#include "include/uint128_t.hpp"
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

class HighPrecisionTimer
{
  private:
    std::chrono::high_resolution_clock::time_point start_time;

  public:
    void start()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop_ns()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        return static_cast<double>(duration.count());
    }
};

struct DivisionTestCase {
    uint128_t dividend;
    uint128_t divisor;
    std::string name;

    DivisionTestCase(uint128_t div, uint128_t dis, const std::string& n)
        : dividend(div), divisor(dis), name(n)
    {
    }
};

// Variable global para evitar optimizaciones del compilador
volatile uint64_t prevent_optimization = 0;

void force_computation(const std::optional<std::pair<uint128_t, uint128_t>>& result)
{
    if (result.has_value()) {
        auto [q, r] = result.value();
        prevent_optimization += q.low() ^ r.low();
    }
}

std::vector<DivisionTestCase> create_comprehensive_test_cases()
{
    std::vector<DivisionTestCase> cases;

    // Caso 1: Divisor pequeño de 64 bits
    cases.emplace_back(uint128_t(0x123456789ABCDEFULL, 0xFEDCBA0987654321ULL),
                       uint128_t(0, 0x12345ULL), "Pequeño 64-bit");

    // Caso 2: Divisor mediano de 64 bits
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0, 0x123456789ABCDEFULL), "Mediano 64-bit");

    // Caso 3: Divisor grande de 64 bits
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0, 0xF000000000000000ULL), "Grande 64-bit");

    // Caso 4: Divisor pequeño de 128 bits
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x123ULL, 0x456789ABCDEF0123ULL), "Pequeño 128-bit");

    // Caso 5: Divisor mediano de 128 bits
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x123456789ABCDEFULL, 0x0ULL), "Mediano 128-bit");

    // Caso 6: Divisor grande de 128 bits
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x8000000000000000ULL, 0x0ULL), "Grande 128-bit");

    return cases;
}

void benchmark_comparison(const std::vector<DivisionTestCase>& test_cases, int iterations)
{
    std::cout << "\n=== BENCHMARK DETALLADO (iteraciones: " << iterations << ") ===" << std::endl;
    std::cout << std::setw(20) << "Caso" << std::setw(15) << "divrem (ns)" << std::setw(18)
              << "knuth_D (ns)" << std::setw(12) << "Speedup" << std::setw(15) << "Ganador"
              << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    HighPrecisionTimer timer;
    double total_divrem_time = 0.0;
    double total_knuth_time = 0.0;

    for (const auto& test_case : test_cases) {
        // Warm-up
        for (int i = 0; i < 1000; ++i) {
            auto result1 = test_case.dividend.divrem(test_case.divisor);
            auto result2 = test_case.dividend.knuth_D_divrem(test_case.divisor);
            force_computation(result1);
            force_computation(result2);
        }

        // Benchmark divrem()
        timer.start();
        for (int i = 0; i < iterations; ++i) {
            auto result = test_case.dividend.divrem(test_case.divisor);
            force_computation(result);
        }
        double divrem_total_ns = timer.stop_ns();
        double divrem_avg_ns = divrem_total_ns / iterations;

        // Benchmark knuth_D_divrem()
        timer.start();
        for (int i = 0; i < iterations; ++i) {
            auto result = test_case.dividend.knuth_D_divrem(test_case.divisor);
            force_computation(result);
        }
        double knuth_total_ns = timer.stop_ns();
        double knuth_avg_ns = knuth_total_ns / iterations;

        total_divrem_time += divrem_avg_ns;
        total_knuth_time += knuth_avg_ns;

        double speedup = divrem_avg_ns / knuth_avg_ns;
        std::string winner = (knuth_avg_ns < divrem_avg_ns) ? "knuth_D_divrem" : "divrem";

        std::cout << std::setw(20) << test_case.name << std::setw(15) << std::fixed
                  << std::setprecision(2) << divrem_avg_ns << std::setw(18) << std::fixed
                  << std::setprecision(2) << knuth_avg_ns << std::setw(12) << std::fixed
                  << std::setprecision(2) << speedup << "x" << std::setw(15) << winner << std::endl;
    }

    std::cout << std::string(80, '-') << std::endl;
    double avg_divrem = total_divrem_time / test_cases.size();
    double avg_knuth = total_knuth_time / test_cases.size();
    double overall_speedup = avg_divrem / avg_knuth;

    std::cout << std::setw(20) << "PROMEDIO" << std::setw(15) << std::fixed << std::setprecision(2)
              << avg_divrem << std::setw(18) << std::fixed << std::setprecision(2) << avg_knuth
              << std::setw(12) << std::fixed << std::setprecision(2) << overall_speedup << "x"
              << std::setw(15) << (avg_knuth < avg_divrem ? "knuth_D_divrem" : "divrem")
              << std::endl;
}

void verify_methods(const std::vector<DivisionTestCase>& test_cases)
{
    std::cout << "=== VERIFICACIÓN DE CORRECTNESS ===" << std::endl;

    bool all_match = true;
    for (const auto& test_case : test_cases) {
        auto result_divrem = test_case.dividend.divrem(test_case.divisor);
        auto result_knuth = test_case.dividend.knuth_D_divrem(test_case.divisor);

        if (!result_divrem.has_value() || !result_knuth.has_value()) {
            std::cout << "❌ " << test_case.name << " - Error en cálculo" << std::endl;
            all_match = false;
            continue;
        }

        auto [q1, r1] = result_divrem.value();
        auto [q2, r2] = result_knuth.value();

        if (q1 != q2 || r1 != r2) {
            std::cout << "❌ " << test_case.name << " - Resultados diferentes" << std::endl;
            all_match = false;
        } else {
            std::cout << "✅ " << test_case.name << " - Idénticos" << std::endl;
        }
    }

    if (all_match) {
        std::cout << "\n🎉 Todos los métodos producen resultados idénticos!" << std::endl;
    } else {
        std::cout << "\n⚠️ Hay discrepancias entre los métodos!" << std::endl;
    }
}

int main()
{
    std::cout << "=== BENCHMARK: divrem() vs knuth_D_divrem() ===" << std::endl;
    std::cout << "Compilador: " << __VERSION__ << std::endl;

#ifdef __SIZEOF_INT128__
    std::cout << "Soporte __uint128_t: Habilitado" << std::endl;
#else
    std::cout << "Soporte __uint128_t: Deshabilitado" << std::endl;
#endif

#ifdef _MSC_VER
    std::cout << "Intrínsecos MSVC: Habilitado" << std::endl;
#else
    std::cout << "Intrínsecos MSVC: Deshabilitado (usando GCC/Clang)" << std::endl;
#endif

    auto test_cases = create_comprehensive_test_cases();

    // Verificar correctness
    verify_methods(test_cases);

    // Benchmark con diferentes números de iteraciones
    std::vector<int> iteration_counts = {10000, 50000};

    for (int iterations : iteration_counts) {
        benchmark_comparison(test_cases, iterations);
    }

    std::cout << "\n=== RESUMEN ===" << std::endl;
    std::cout << "knuth_D_divrem usa rutas optimizadas:" << std::endl;
    std::cout << "- Divisores 64-bit: __uint128_t nativo" << std::endl;
    std::cout << "- Divisores 128-bit: Algoritmo D de Knuth" << std::endl;
    std::cout << "divrem usa: División binaria larga genérica" << std::endl;
    std::cout << "\nVariable anti-optimización final: " << prevent_optimization << std::endl;

    return 0;
}