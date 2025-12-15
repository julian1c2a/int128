#include "include/uint128_t.hpp"
#include <cassert>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

class BenchmarkTimer
{
  private:
    std::chrono::high_resolution_clock::time_point start_time;

  public:
    void start()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop_ms()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0; // Convert to milliseconds
    }

    double stop_ns()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        return static_cast<double>(duration.count());
    }
};

struct TestCase {
    uint128_t dividend;
    uint128_t divisor;
    std::string name;

    TestCase(uint128_t div, uint128_t dis, const std::string& n)
        : dividend(div), divisor(dis), name(n)
    {
    }
};

std::vector<TestCase> generate_test_cases()
{
    std::vector<TestCase> cases;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    // Caso 1: Divisor pequeño (64 bits)
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0, 0x123456789ABCDEFULL), "Divisor 64-bit pequeño");

    // Caso 2: Divisor grande (64 bits)
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0, 0xF000000000000000ULL), "Divisor 64-bit grande");

    // Caso 3: Divisor 128-bit pequeño
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x1234567890ABCDEFULL, 0x0ULL), "Divisor 128-bit pequeño");

    // Caso 4: Divisor 128-bit grande
    cases.emplace_back(uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
                       uint128_t(0x8000000000000000ULL, 0x0ULL), "Divisor 128-bit grande");

    // Caso 5: Ambos números grandes
    cases.emplace_back(uint128_t(0xFEDCBA9876543210ULL, 0x0123456789ABCDEFULL),
                       uint128_t(0x1122334455667788ULL, 0x99AABBCCDDEEFF00ULL),
                       "Ambos números grandes");

    // Casos 6-10: Números aleatorios
    for (int i = 0; i < 5; ++i) {
        cases.emplace_back(uint128_t(dis(gen), dis(gen)),
                           uint128_t(dis(gen) | 1ULL,
                                     dis(gen) | 1ULL), // Evitar división por números pares pequeños
                           "Aleatorio " + std::to_string(i + 1));
    }

    return cases;
}

void verify_correctness(const std::vector<TestCase>& test_cases)
{
    std::cout << "=== VERIFICACIÓN DE CORRECTNESS ===" << std::endl;

    bool all_correct = true;
    for (const auto& test_case : test_cases) {
        auto result_divrem = test_case.dividend.divrem(test_case.divisor);
        auto result_knuth = test_case.dividend.knuth_D_divrem(test_case.divisor);

        if (!result_divrem.has_value() || !result_knuth.has_value()) {
            std::cout << "ERROR: " << test_case.name << " - Uno o ambos métodos fallaron"
                      << std::endl;
            all_correct = false;
            continue;
        }

        auto [q1, r1] = result_divrem.value();
        auto [q2, r2] = result_knuth.value();

        if (q1 != q2 || r1 != r2) {
            std::cout << "ERROR: " << test_case.name << " - Resultados diferentes" << std::endl;
            std::cout << "  divrem():        q=" << q1 << ", r=" << r1 << std::endl;
            std::cout << "  knuth_D_divrem: q=" << q2 << ", r=" << r2 << std::endl;
            all_correct = false;
        } else {
            std::cout << "✓ " << test_case.name << " - Resultados idénticos" << std::endl;
        }
    }

    if (all_correct) {
        std::cout << "✅ Todos los tests de correctness pasaron!" << std::endl;
    } else {
        std::cout << "❌ Algunos tests de correctness fallaron!" << std::endl;
    }
    std::cout << std::endl;
}

void benchmark_method(const std::vector<TestCase>& test_cases, const std::string& method_name,
                      std::function<std::optional<std::pair<uint128_t, uint128_t>>(
                          const uint128_t&, const uint128_t&)>
                          method,
                      int iterations = 10000)
{

    std::cout << "=== BENCHMARK: " << method_name << " ===" << std::endl;

    BenchmarkTimer timer;
    std::vector<double> times;

    for (const auto& test_case : test_cases) {
        timer.start();

        // Ejecutar múltiples iteraciones para una medida más precisa
        for (int i = 0; i < iterations; ++i) {
            volatile auto result = method(test_case.dividend, test_case.divisor);
            (void)result; // Evitar optimizaciones que eliminen el cálculo
        }

        double total_time_ns = timer.stop_ns();
        double avg_time_ns = total_time_ns / iterations;
        times.push_back(avg_time_ns);

        std::cout << std::setw(25) << std::left << test_case.name << ": " << std::setw(8)
                  << std::fixed << std::setprecision(2) << avg_time_ns << " ns/op" << std::endl;
    }

    // Calcular estadísticas
    double total_time = 0.0;
    for (double time : times) {
        total_time += time;
    }
    double avg_time = total_time / times.size();

    std::cout << std::setw(25) << std::left << "PROMEDIO"
              << ": " << std::setw(8) << std::fixed << std::setprecision(2) << avg_time << " ns/op"
              << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "=== BENCHMARK DIVISIÓN: divrem() vs knuth_D_divrem() ===" << std::endl;
    std::cout << "Compilador: " << __VERSION__ << std::endl;

#ifdef __SIZEOF_INT128__
    std::cout << "Soporte __uint128_t: SÍ" << std::endl;
#else
    std::cout << "Soporte __uint128_t: NO" << std::endl;
#endif

#ifdef _MSC_VER
    std::cout << "Intrínsecos MSVC: SÍ" << std::endl;
#else
    std::cout << "Intrínsecos MSVC: NO" << std::endl;
#endif

    std::cout << std::endl;

    // Generar casos de prueba
    auto test_cases = generate_test_cases();

    // Verificar que ambos métodos producen los mismos resultados
    verify_correctness(test_cases);

    const int iterations = 50000; // Reducido para casos más complejos

    // Benchmark divrem()
    benchmark_method(
        test_cases, "divrem()",
        [](const uint128_t& dividend, const uint128_t& divisor) {
            return dividend.divrem(divisor);
        },
        iterations);

    // Benchmark knuth_D_divrem()
    benchmark_method(
        test_cases, "knuth_D_divrem()",
        [](const uint128_t& dividend, const uint128_t& divisor) {
            return dividend.knuth_D_divrem(divisor);
        },
        iterations);

    std::cout << "=== COMPARACIÓN DE RENDIMIENTO ===" << std::endl;
    std::cout << "Ejecutando comparación lado a lado..." << std::endl;

    // Comparación directa caso por caso
    BenchmarkTimer timer;

    for (const auto& test_case : test_cases) {
        // Medir divrem()
        timer.start();
        for (int i = 0; i < iterations; ++i) {
            volatile auto result = test_case.dividend.divrem(test_case.divisor);
            (void)result;
        }
        double divrem_time = timer.stop_ns() / iterations;

        // Medir knuth_D_divrem()
        timer.start();
        for (int i = 0; i < iterations; ++i) {
            volatile auto result = test_case.dividend.knuth_D_divrem(test_case.divisor);
            (void)result;
        }
        double knuth_time = timer.stop_ns() / iterations;

        double ratio = divrem_time / knuth_time;
        std::string winner = (knuth_time < divrem_time) ? "knuth_D_divrem" : "divrem";

        std::cout << std::setw(25) << std::left << test_case.name << ": ";
        std::cout << "divrem=" << std::setw(8) << std::fixed << std::setprecision(2) << divrem_time
                  << "ns, ";
        std::cout << "knuth=" << std::setw(8) << std::fixed << std::setprecision(2) << knuth_time
                  << "ns, ";
        std::cout << "ratio=" << std::setw(5) << std::fixed << std::setprecision(2) << ratio
                  << "x, ";
        std::cout << "ganador=" << winner << std::endl;
    }

    return 0;
}