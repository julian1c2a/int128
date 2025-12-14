#include "include/uint128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

class QuickTimer
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

struct QuickTestCase {
    uint128_t dividend;
    uint128_t divisor;
    std::string name;

    QuickTestCase(uint128_t div, uint128_t dis, const std::string& n)
        : dividend(div), divisor(dis), name(n)
    {
    }
};

// Variable para evitar optimizaciones
volatile uint64_t anti_optimize = 0;

void consume_result(const std::optional<std::pair<uint128_t, uint128_t>>& result)
{
    if (result.has_value()) {
        auto [q, r] = result.value();
        anti_optimize += q.low() ^ r.high();
    }
}

int main()
{
    std::cout << "=== BENCHMARK RÁPIDO: divrem() vs knuth_D_divrem() ===" << std::endl;

    std::vector<QuickTestCase> cases;

    // Solo algunos casos representativos
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0, 0x123456789ABCDEFULL), "Divisor 64-bit");

    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x123456789ABCDEFULL, 0x0ULL), "Divisor 128-bit pequeño");

    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x8000000000000000ULL, 0x0ULL), "Divisor 128-bit grande");

    std::cout << "\n1. Verificando correctness..." << std::endl;
    bool all_correct = true;
    for (const auto& test_case : cases) {
        auto r1 = test_case.dividend.divrem(test_case.divisor);
        auto r2 = test_case.dividend.knuth_D_divrem(test_case.divisor);

        if (r1.has_value() && r2.has_value()) {
            auto [q1, rem1] = r1.value();
            auto [q2, rem2] = r2.value();
            if (q1 == q2 && rem1 == rem2) {
                std::cout << "✅ " << test_case.name << std::endl;
            } else {
                std::cout << "❌ " << test_case.name << " - DIFERENTE!" << std::endl;
                all_correct = false;
            }
        }
    }

    if (!all_correct) {
        std::cout << "❌ Error: Los métodos no coinciden!" << std::endl;
        return 1;
    }

    const int QUICK_ITERATIONS = 1000; // Mucho menos iteraciones
    std::cout << "\n2. Benchmarking (" << QUICK_ITERATIONS << " iteraciones cada uno)..."
              << std::endl;

    QuickTimer timer;

    std::cout << std::setw(25) << "Caso" << std::setw(15) << "divrem (ns)" << std::setw(18)
              << "knuth_D (ns)" << std::setw(12) << "Speedup" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    double total_divrem = 0, total_knuth = 0;

    for (const auto& test_case : cases) {
        // Benchmark divrem()
        timer.start();
        for (int i = 0; i < QUICK_ITERATIONS; ++i) {
            auto result = test_case.dividend.divrem(test_case.divisor);
            consume_result(result);
        }
        double divrem_time = timer.stop_ns() / QUICK_ITERATIONS;

        // Benchmark knuth_D_divrem()
        timer.start();
        for (int i = 0; i < QUICK_ITERATIONS; ++i) {
            auto result = test_case.dividend.knuth_D_divrem(test_case.divisor);
            consume_result(result);
        }
        double knuth_time = timer.stop_ns() / QUICK_ITERATIONS;

        total_divrem += divrem_time;
        total_knuth += knuth_time;

        double speedup = divrem_time / knuth_time;

        std::cout << std::setw(25) << test_case.name << std::setw(15) << std::fixed
                  << std::setprecision(1) << divrem_time << std::setw(18) << std::fixed
                  << std::setprecision(1) << knuth_time << std::setw(11) << std::fixed
                  << std::setprecision(1) << speedup << "x" << std::endl;
    }

    std::cout << std::string(70, '-') << std::endl;
    double avg_divrem = total_divrem / cases.size();
    double avg_knuth = total_knuth / cases.size();
    double overall_speedup = avg_divrem / avg_knuth;

    std::cout << std::setw(25) << "PROMEDIO" << std::setw(15) << std::fixed << std::setprecision(1)
              << avg_divrem << std::setw(18) << std::fixed << std::setprecision(1) << avg_knuth
              << std::setw(11) << std::fixed << std::setprecision(1) << overall_speedup << "x"
              << std::endl;

    std::cout << "\n3. Conclusión:" << std::endl;
    if (avg_knuth < avg_divrem) {
        std::cout << "🏆 knuth_D_divrem es " << std::fixed << std::setprecision(1)
                  << overall_speedup << "x más rápido en promedio" << std::endl;
        std::cout << "   - Optimizado para divisores de 64-bit con __uint128_t" << std::endl;
        std::cout << "   - Algoritmo D de Knuth para divisores de 128-bit" << std::endl;
    } else {
        std::cout << "🏆 divrem es " << std::fixed << std::setprecision(1)
                  << (avg_knuth / avg_divrem) << "x más rápido en promedio" << std::endl;
    }

    std::cout << "\nAnti-optimización: " << anti_optimize << std::endl;
    return 0;
}