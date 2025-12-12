#include "benchmark_suite.hpp"
#include "include/uint128_t.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

/// Benchmarks para operaciones aritméticas básicas
class ArithmeticBenchmarks
{
  private:
    BenchmarkRunner& runner;

  public:
    ArithmeticBenchmarks(BenchmarkRunner& r) : runner(r) {}

    /// Benchmark de suma
    template <typename T> void benchmark_addition()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("Addition", typeid(T).name(), [&]() {
            volatile auto result = a + b;
            (void)result; // Evitar optimización
        });
    }

    /// Benchmark de resta
    template <typename T> void benchmark_subtraction()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("Subtraction", typeid(T).name(), [&]() {
            volatile auto result = a - b;
            (void)result;
        });
    }

    /// Benchmark de multiplicación
    template <typename T> void benchmark_multiplication()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("Multiplication", typeid(T).name(), [&]() {
            volatile auto result = a * b;
            (void)result;
        });
    }

    /// Benchmark de división
    template <typename T> void benchmark_division()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;
        if constexpr (std::is_same_v<T, uint128_t>) {
            if (b == uint128_t(0, 0))
                b = uint128_t(0, 1);
        } else {
            if (b == 0)
                b = 1;
        }

        runner.run_benchmark("Division", typeid(T).name(), [&]() {
            volatile auto result = a / b;
            (void)result;
        });
    }

    /// Benchmark de módulo
    template <typename T> void benchmark_modulo()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;
        if constexpr (std::is_same_v<T, uint128_t>) {
            if (b == uint128_t(0, 0))
                b = uint128_t(0, 1);
        } else {
            if (b == 0)
                b = 1;
        }

        runner.run_benchmark("Modulo", typeid(T).name(), [&]() {
            volatile auto result = a % b;
            (void)result;
        });
    }

    /// Benchmark de incremento
    template <typename T> void benchmark_increment()
    {
        T a = runner.random_value<T>();

        runner.run_benchmark("Increment", typeid(T).name(), [&]() { ++a; });
    }

    /// Benchmark de decremento
    template <typename T> void benchmark_decrement()
    {
        T a = runner.random_value<T>();

        runner.run_benchmark("Decrement", typeid(T).name(), [&]() { --a; });
    }

    /// Ejecuta todos los benchmarks aritméticos
    template <typename T> void run_all()
    {
        std::cout << "\n🧮 Benchmarks Aritméticos para " << typeid(T).name() << std::endl;
        benchmark_addition<T>();
        benchmark_subtraction<T>();
        benchmark_multiplication<T>();
        benchmark_division<T>();
        benchmark_modulo<T>();
        benchmark_increment<T>();
        benchmark_decrement<T>();
    }
};

/// Benchmarks para operaciones de comparación
class ComparisonBenchmarks
{
  private:
    BenchmarkRunner& runner;

  public:
    ComparisonBenchmarks(BenchmarkRunner& r) : runner(r) {}

    template <typename T> void benchmark_equality()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("Equality", typeid(T).name(), [&]() {
            volatile bool result = (a == b);
            (void)result;
        });
    }

    template <typename T> void benchmark_less_than()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("LessThan", typeid(T).name(), [&]() {
            volatile bool result = (a < b);
            (void)result;
        });
    }

    template <typename T> void benchmark_greater_than()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("GreaterThan", typeid(T).name(), [&]() {
            volatile bool result = (a > b);
            (void)result;
        });
    }

    template <typename T> void run_all()
    {
        std::cout << "\n🔍 Benchmarks de Comparación para " << typeid(T).name() << std::endl;
        benchmark_equality<T>();
        benchmark_less_than<T>();
        benchmark_greater_than<T>();
    }
};

/// Benchmarks para operaciones bitwise
class BitwiseBenchmarks
{
  private:
    BenchmarkRunner& runner;

  public:
    BitwiseBenchmarks(BenchmarkRunner& r) : runner(r) {}

    template <typename T> void benchmark_and()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("BitwiseAND", typeid(T).name(), [&]() {
            volatile auto result = a & b;
            (void)result;
        });
    }

    template <typename T> void benchmark_or()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("BitwiseOR", typeid(T).name(), [&]() {
            volatile auto result = a | b;
            (void)result;
        });
    }

    template <typename T> void benchmark_xor()
    {
        auto pair = runner.random_pair<T>();
        T a = pair.first, b = pair.second;

        runner.run_benchmark("BitwiseXOR", typeid(T).name(), [&]() {
            volatile auto result = a ^ b;
            (void)result;
        });
    }

    template <typename T> void benchmark_not()
    {
        T a = runner.random_value<T>();

        runner.run_benchmark("BitwiseNOT", typeid(T).name(), [&]() {
            volatile auto result = ~a;
            (void)result;
        });
    }

    template <typename T> void benchmark_shift_left()
    {
        T a = runner.random_value<T>();

        runner.run_benchmark("ShiftLeft", typeid(T).name(), [&]() {
            volatile auto result = a << 5;
            (void)result;
        });
    }

    template <typename T> void benchmark_shift_right()
    {
        T a = runner.random_value<T>();

        runner.run_benchmark("ShiftRight", typeid(T).name(), [&]() {
            volatile auto result = a >> 5;
            (void)result;
        });
    }

    template <typename T> void run_all()
    {
        std::cout << "\n🔧 Benchmarks Bitwise para " << typeid(T).name() << std::endl;
        benchmark_and<T>();
        benchmark_or<T>();
        benchmark_xor<T>();
        benchmark_not<T>();
        benchmark_shift_left<T>();
        benchmark_shift_right<T>();
    }
};

/// Benchmarks específicos para uint128_t
class Uint128Benchmarks
{
  private:
    BenchmarkRunner& runner;

  public:
    Uint128Benchmarks(BenchmarkRunner& r) : runner(r) {}

    void benchmark_divrem()
    {
        auto pair = runner.random_pair<uint128_t>();
        uint128_t a = pair.first, b = pair.second;
        if (b == uint128_t(0, 0))
            b = uint128_t(0, 1);

        runner.run_benchmark("DivRem", "uint128_t", [&]() {
            volatile auto result = a.divrem(b);
            (void)result;
        });
    }

    void benchmark_leading_zeros()
    {
        uint128_t a = runner.random_value<uint128_t>();

        runner.run_benchmark("LeadingZeros", "uint128_t", [&]() {
            volatile auto result = a.leading_zeros();
            (void)result;
        });
    }

    void benchmark_to_string()
    {
        uint128_t a = runner.random_value<uint128_t>();

        runner.run_benchmark("ToString", "uint128_t", [&]() {
            volatile auto result = a.to_string();
            (void)result;
        });
    }

    void benchmark_from_string()
    {
        std::string test_string = "123456789012345678901234567890";

        runner.run_benchmark("FromString", "uint128_t", [&]() {
            volatile auto result = uint128_t::from_string(test_string);
            (void)result;
        });
    }

    void benchmark_shift_left_custom()
    {
        uint128_t a = runner.random_value<uint128_t>();
        int shift = 32;

        runner.run_benchmark("ShiftLeftCustom", "uint128_t", [&]() {
            volatile auto result = a.shift_left(shift);
            (void)result;
        });
    }

    void benchmark_shift_right_custom()
    {
        uint128_t a = runner.random_value<uint128_t>();
        int shift = 32;

        runner.run_benchmark("ShiftRightCustom", "uint128_t", [&]() {
            volatile auto result = a.shift_right(shift);
            (void)result;
        });
    }

    void run_all()
    {
        std::cout << "\n🎯 Benchmarks Específicos uint128_t" << std::endl;
        benchmark_divrem();
        benchmark_leading_zeros();
        benchmark_to_string();
        benchmark_from_string();
        benchmark_shift_left_custom();
        benchmark_shift_right_custom();
    }
};

/// Función para obtener un nombre bonito del tipo
template <typename T> std::string get_type_name()
{
    if constexpr (std::is_same_v<T, uint8_t>)
        return "uint8_t";
    else if constexpr (std::is_same_v<T, uint16_t>)
        return "uint16_t";
    else if constexpr (std::is_same_v<T, uint32_t>)
        return "uint32_t";
    else if constexpr (std::is_same_v<T, uint64_t>)
        return "uint64_t";
    else if constexpr (std::is_same_v<T, uint128_t>)
        return "uint128_t";
    else
        return typeid(T).name();
}

/// Función para corregir los nombres de tipo en los resultados
void fix_type_names(BenchmarkRunner& runner)
{
    auto& results = const_cast<std::vector<BenchmarkStats>&>(runner.get_results());
    for (auto& stat : results) {
        // Reemplazar nombres de tipos generados por typeid
        if (stat.data_type.find("unsigned char") != std::string::npos) {
            stat.data_type = "uint8_t";
        } else if (stat.data_type.find("unsigned short") != std::string::npos) {
            stat.data_type = "uint16_t";
        } else if (stat.data_type.find("unsigned int") != std::string::npos) {
            stat.data_type = "uint32_t";
        } else if (stat.data_type.find("unsigned __int64") != std::string::npos ||
                   stat.data_type.find("unsigned long long") != std::string::npos) {
            stat.data_type = "uint64_t";
        } else if (stat.data_type.find("class uint128_t") != std::string::npos ||
                   stat.data_type == "uint128_t") {
            stat.data_type = "uint128_t";
        }
    }
}

int main()
{
    std::cout << "🚀 BIBLIOTECA DE BENCHMARKS UINT128_T" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Ejecutando " << BenchmarkRunner::DEFAULT_ITERATIONS
              << " iteraciones por operación..." << std::endl;

    BenchmarkRunner runner;

    // Crear los objetos de benchmark
    ArithmeticBenchmarks arith(runner);
    ComparisonBenchmarks comp(runner);
    BitwiseBenchmarks bitwise(runner);
    Uint128Benchmarks uint128_specific(runner);

    // Ejecutar benchmarks para todos los tipos unsigned
    arith.run_all<uint8_t>();
    arith.run_all<uint16_t>();
    arith.run_all<uint32_t>();
    arith.run_all<uint64_t>();
    arith.run_all<uint128_t>();

    comp.run_all<uint8_t>();
    comp.run_all<uint16_t>();
    comp.run_all<uint32_t>();
    comp.run_all<uint64_t>();
    comp.run_all<uint128_t>();

    bitwise.run_all<uint8_t>();
    bitwise.run_all<uint16_t>();
    bitwise.run_all<uint32_t>();
    bitwise.run_all<uint64_t>();
    bitwise.run_all<uint128_t>();

    // Benchmarks específicos de uint128_t
    uint128_specific.run_all();

    // Corregir nombres de tipos
    fix_type_names(runner);

    // Mostrar resultados
    runner.print_results();

    // Comparaciones de rendimiento
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "📈 ANÁLISIS COMPARATIVO" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    runner.compare_performance("Addition",
                               {"uint8_t", "uint16_t", "uint32_t", "uint64_t", "uint128_t"});
    runner.compare_performance("Division",
                               {"uint8_t", "uint16_t", "uint32_t", "uint64_t", "uint128_t"});
    runner.compare_performance("Multiplication",
                               {"uint8_t", "uint16_t", "uint32_t", "uint64_t", "uint128_t"});

    // Exportar a CSV con timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "benchmark_results_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << ".csv";
    runner.export_to_csv(ss.str());

    std::cout << "\n🎉 ¡Benchmarks completados!" << std::endl;
    return 0;
}