#ifndef BENCHMARK_SUITE_HPP
#define BENCHMARK_SUITE_HPP

#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

/// Estructura para almacenar estadísticas de benchmark
struct BenchmarkStats {
    std::string operation_name;
    std::string data_type;
    std::string compiler;
    size_t iterations;
    double mean_ns;       // Tiempo promedio en nanosegundos
    double stddev_ns;     // Desviación estándar en nanosegundos
    double min_ns;        // Tiempo mínimo
    double max_ns;        // Tiempo máximo
    double total_time_ms; // Tiempo total en milisegundos

    BenchmarkStats() = default;

    BenchmarkStats(const std::string& op, const std::string& type, const std::string& comp)
        : operation_name(op), data_type(type), compiler(comp), iterations(0), mean_ns(0),
          stddev_ns(0), min_ns(0), max_ns(0), total_time_ms(0)
    {
    }
};

/// Clase base para benchmarking
class BenchmarkRunner
{
  private:
    std::vector<BenchmarkStats> results;
    std::mt19937_64 rng;

    static std::string get_compiler_name()
    {
#ifdef _MSC_VER
        return "MSVC_" + std::to_string(_MSC_VER);
#elif defined(__GNUC__) && !defined(__clang__)
        return "GCC_" + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#elif defined(__clang__)
        return "Clang_" + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#else
        return "Unknown";
#endif
    }

  public:
    static constexpr size_t DEFAULT_ITERATIONS = 10'000'000;

    BenchmarkRunner() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    /// Ejecuta un benchmark con el número especificado de iteraciones
    template <typename Func>
    BenchmarkStats run_benchmark(const std::string& operation_name, const std::string& data_type,
                                 Func&& benchmark_func, size_t iterations = DEFAULT_ITERATIONS)
    {

        std::vector<double> times;
        times.reserve(iterations);

        std::cout << "🔄 Ejecutando " << operation_name << " (" << data_type << ") - " << iterations
                  << " iteraciones..." << std::flush;

        // Warm-up
        for (size_t i = 0; i < 1000; ++i) {
            benchmark_func();
        }

        auto total_start = std::chrono::high_resolution_clock::now();

        // Medición principal
        for (size_t i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            benchmark_func();
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            times.push_back(static_cast<double>(duration.count()));
        }

        auto total_end = std::chrono::high_resolution_clock::now();
        auto total_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start);

        // Calcular estadísticas
        BenchmarkStats stats(operation_name, data_type, get_compiler_name());
        stats.iterations = iterations;
        stats.total_time_ms = static_cast<double>(total_duration.count());

        // Media
        double sum = 0.0;
        for (double time : times) {
            sum += time;
        }
        stats.mean_ns = sum / iterations;

        // Min y Max
        stats.min_ns = *std::min_element(times.begin(), times.end());
        stats.max_ns = *std::max_element(times.begin(), times.end());

        // Desviación estándar
        double variance_sum = 0.0;
        for (double time : times) {
            double diff = time - stats.mean_ns;
            variance_sum += diff * diff;
        }
        stats.stddev_ns = std::sqrt(variance_sum / iterations);

        results.push_back(stats);
        std::cout << " ✓ " << std::fixed << std::setprecision(2) << stats.mean_ns << "±"
                  << stats.stddev_ns << " ns" << std::endl;

        return stats;
    }

    /// Genera números aleatorios para testing
    template <typename T> T random_value()
    {
        if constexpr (std::is_same_v<T, uint128_t>) {
            return uint128_t(rng(), rng());
        } else {
            return static_cast<T>(rng());
        }
    }

    /// Genera un par de números aleatorios
    template <typename T> std::pair<T, T> random_pair()
    {
        return std::make_pair(random_value<T>(), random_value<T>());
    }

    /// Imprime resultados en formato tabla
    void print_results() const
    {
        std::cout << "\n" << std::string(120, '=') << std::endl;
        std::cout << "📊 RESULTADOS DE BENCHMARK" << std::endl;
        std::cout << std::string(120, '=') << std::endl;

        std::cout << std::left << std::setw(25) << "Operación" << std::setw(15) << "Tipo"
                  << std::setw(15) << "Compilador" << std::setw(12) << "Media (ns)" << std::setw(12)
                  << "StdDev (ns)" << std::setw(12) << "Min (ns)" << std::setw(12) << "Max (ns)"
                  << std::setw(15) << "Total (ms)" << std::endl;
        std::cout << std::string(120, '-') << std::endl;

        for (const auto& stat : results) {
            std::cout << std::left << std::fixed << std::setprecision(2) << std::setw(25)
                      << stat.operation_name << std::setw(15) << stat.data_type << std::setw(15)
                      << stat.compiler << std::setw(12) << stat.mean_ns << std::setw(12)
                      << stat.stddev_ns << std::setw(12) << stat.min_ns << std::setw(12)
                      << stat.max_ns << std::setw(15) << stat.total_time_ms << std::endl;
        }
    }

    /// Exporta resultados a CSV
    void export_to_csv(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "❌ Error: No se pudo abrir el archivo " << filename << std::endl;
            return;
        }

        // Header
        file << "Operacion,Tipo,Compilador,Iteraciones,Media_ns,StdDev_ns,Min_ns,Max_ns,Total_ms\n";

        // Data
        for (const auto& stat : results) {
            file << stat.operation_name << "," << stat.data_type << "," << stat.compiler << ","
                 << stat.iterations << "," << std::fixed << std::setprecision(6) << stat.mean_ns
                 << "," << stat.stddev_ns << "," << stat.min_ns << "," << stat.max_ns << ","
                 << stat.total_time_ms << "\n";
        }

        file.close();
        std::cout << "✅ Resultados exportados a: " << filename << std::endl;
    }

    /// Compara rendimiento entre tipos de datos
    void compare_performance(const std::string& operation,
                             const std::vector<std::string>& types) const
    {
        std::cout << "\n🔄 COMPARACIÓN DE RENDIMIENTO: " << operation << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        std::vector<const BenchmarkStats*> matching_stats;
        for (const auto& stat : results) {
            if (stat.operation_name == operation) {
                matching_stats.push_back(&stat);
            }
        }

        if (matching_stats.empty()) {
            std::cout << "No se encontraron resultados para: " << operation << std::endl;
            return;
        }

        // Ordenar por velocidad (menor tiempo es mejor)
        std::sort(matching_stats.begin(), matching_stats.end(),
                  [](const BenchmarkStats* a, const BenchmarkStats* b) {
                      return a->mean_ns < b->mean_ns;
                  });

        const double baseline = matching_stats[0]->mean_ns;

        for (const auto* stat : matching_stats) {
            double relative_speed = stat->mean_ns / baseline;
            std::cout << std::left << std::setw(15) << stat->data_type << std::fixed
                      << std::setprecision(2) << std::setw(12) << stat->mean_ns << " ns  "
                      << std::setprecision(2) << relative_speed << "x más lento" << std::endl;
        }
    }

    /// Obtiene todos los resultados
    const std::vector<BenchmarkStats>& get_results() const
    {
        return results;
    }

    /// Limpia los resultados
    void clear_results()
    {
        results.clear();
    }
};

#endif // BENCHMARK_SUITE_HPP