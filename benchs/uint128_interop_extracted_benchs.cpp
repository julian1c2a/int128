/**
 * Benchmark Comparativo: int128_t vs uint128_t
 *
 * Compara el rendimiento entre:
 * - int128_t (tipo con signo, complemento a 2)
 * - uint128_t (tipo sin signo)
 *
 * Objetivo: Medir el overhead del manejo de signo en operaciones
 *
 * Operaciones benchmarked:
 * - Construcción y asignación
 * - Aritmética básica (+, -, *, /, %)
 * - Operaciones bit a bit (&, |, ^, <<, >>)
 * - Conversiones string
 * - Funciones matemáticas (gcd, lcm, pow, sqrt, abs)
 * - Comparaciones (<, >, <=, >=, ==, !=)
 * - Operaciones específicas de signo (negación, signo)
 */

#include "int128/int128_cmath.hpp"
#include "int128/int128_t.hpp"
#include "uint128/uint128_cmath.hpp"
#include "uint128/uint128_t.hpp"

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace nstd;
using namespace std::chrono;

constexpr int ITERATIONS = 100000;
constexpr int WARM_UP = 1000;

// Helper para timing
template <typename Func>
double measure_time(const std::string& name, Func func, int iterations = ITERATIONS)
{
    // Warm-up
    for (int i = 0; i < WARM_UP; ++i) {
        func();
    }

    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = high_resolution_clock::now();

    return duration_cast<nanoseconds>(end - start).count() / static_cast<double>(iterations);
}

// ============================================================================
// BENCHMARKS: CONSTRUCCIÓN Y ASIGNACIÓN
// ============================================================================

void benchmark_construction()
{
    std::cout << "\n=== CONSTRUCCIÓN Y ASIGNACIÓN ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // uint128_t
    auto time_uint_default = measure_time("uint128_t default ctor", []() {
        uint128_t val;
        (void)val;
    });

    auto time_uint_from_uint64 = measure_time("uint128_t from uint64_t", []() {
        uint128_t val(0x123456789ABCDEF0ULL);
        (void)val;
    });

    auto time_uint_copy = measure_time("uint128_t copy", []() {
        static uint128_t src(0x123456789ABCDEF0ULL);
        uint128_t val(src);
        (void)val;
    });

    // int128_t
    auto time_int_default = measure_time("int128_t default ctor", []() {
        int128_t val;
        (void)val;
    });

    auto time_int_from_int64 = measure_time("int128_t from int64_t", []() {
        int128_t val(static_cast<int64_t>(0x123456789ABCDEF0LL));
        (void)val;
    });

    auto time_int_from_negative = measure_time("int128_t from negative", []() {
        int128_t val(-123456789);
        (void)val;
    });

    auto time_int_copy = measure_time("int128_t copy", []() {
        static int128_t src(static_cast<int64_t>(0x123456789ABCDEF0LL));
        int128_t val(src);
        (void)val;
    });

    // Resultados
    std::cout << "Default constructor:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_default << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_default << " ns  ";
    std::cout << "(overhead: " << ((time_int_default / time_uint_default - 1.0) * 100) << "%)"
              << std::endl;

    std::cout << "From 64-bit integer:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_from_uint64 << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_from_int64 << " ns  ";
    std::cout << "(overhead: " << ((time_int_from_int64 / time_uint_from_uint64 - 1.0) * 100)
              << "%)" << std::endl;

    std::cout << "From negative value (int128_t only):" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_from_negative << " ns" << std::endl;

    std::cout << "Copy constructor:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_copy << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_copy << " ns  ";
    std::cout << "(overhead: " << ((time_int_copy / time_uint_copy - 1.0) * 100) << "%)"
              << std::endl;
}

// ============================================================================
// BENCHMARKS: ARITMÉTICA BÁSICA
// ============================================================================

void benchmark_arithmetic()
{
    std::cout << "\n=== ARITMÉTICA BÁSICA ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Valores de prueba (positivos para comparación justa)
    uint128_t uint_a(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    uint128_t uint_b(0xABCDEF0123456789ULL, 0x0123456789ABCDEFULL);

    int128_t int_a(static_cast<int64_t>(0x0123456789ABCDEFLL),
                   static_cast<uint64_t>(0xFEDCBA9876543210ULL));
    int128_t int_b(static_cast<int64_t>(0x00CDEF0123456789LL),
                   static_cast<uint64_t>(0x0123456789ABCDEFULL));

    // Suma
    auto time_uint_add = measure_time("uint128_t addition", [&]() {
        auto result = uint_a + uint_b;
        (void)result;
    });

    auto time_int_add = measure_time("int128_t addition", [&]() {
        auto result = int_a + int_b;
        (void)result;
    });

    // Resta
    auto time_uint_sub = measure_time("uint128_t subtraction", [&]() {
        auto result = uint_a - uint_b;
        (void)result;
    });

    auto time_int_sub = measure_time("int128_t subtraction", [&]() {
        auto result = int_a - int_b;
        (void)result;
    });

    // Multiplicación
    auto time_uint_mul = measure_time("uint128_t multiplication", [&]() {
        auto result = uint_a * uint128_t(12345);
        (void)result;
    });

    auto time_int_mul = measure_time("int128_t multiplication", [&]() {
        auto result = int_a * int128_t(12345);
        (void)result;
    });

    // División
    auto time_uint_div = measure_time("uint128_t division", [&]() {
        auto result = uint_a / uint128_t(12345);
        (void)result;
    });

    auto time_int_div = measure_time("int128_t division", [&]() {
        auto result = int_a / int128_t(12345);
        (void)result;
    });

    // Módulo
    auto time_uint_mod = measure_time("uint128_t modulo", [&]() {
        auto result = uint_a % uint128_t(12345);
        (void)result;
    });

    auto time_int_mod = measure_time("int128_t modulo", [&]() {
        auto result = int_a % int128_t(12345);
        (void)result;
    });

    // Negación (int128_t específico)
    auto time_int_negate = measure_time("int128_t negation", [&]() {
        auto result = -int_a;
        (void)result;
    });

    // Resultados
    std::cout << "Addition (+):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_add << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_add << " ns  ";
    std::cout << "(overhead: " << ((time_int_add / time_uint_add - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Subtraction (-):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_sub << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_sub << " ns  ";
    std::cout << "(overhead: " << ((time_int_sub / time_uint_sub - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Multiplication (*):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_mul << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_mul << " ns  ";
    std::cout << "(overhead: " << ((time_int_mul / time_uint_mul - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Division (/):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_div << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_div << " ns  ";
    std::cout << "(overhead: " << ((time_int_div / time_uint_div - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Modulo (%):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_mod << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_mod << " ns  ";
    std::cout << "(overhead: " << ((time_int_mod / time_uint_mod - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Negation (unary -, int128_t only):" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_negate << " ns" << std::endl;
}

// ============================================================================
// BENCHMARKS: OPERACIONES BIT A BIT
// ============================================================================

void benchmark_bitwise()
{
    std::cout << "\n=== OPERACIONES BIT A BIT ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t uint_a(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    uint128_t uint_b(0xABCDEF0123456789ULL, 0x0123456789ABCDEFULL);

    int128_t int_a(static_cast<int64_t>(0x0123456789ABCDEFLL),
                   static_cast<uint64_t>(0xFEDCBA9876543210ULL));
    int128_t int_b(static_cast<int64_t>(0x00CDEF0123456789LL),
                   static_cast<uint64_t>(0x0123456789ABCDEFULL));

    // AND
    auto time_uint_and = measure_time("uint128_t AND", [&]() {
        auto result = uint_a & uint_b;
        (void)result;
    });

    auto time_int_and = measure_time("int128_t AND", [&]() {
        auto result = int_a & int_b;
        (void)result;
    });

    // OR
    auto time_uint_or = measure_time("uint128_t OR", [&]() {
        auto result = uint_a | uint_b;
        (void)result;
    });

    auto time_int_or = measure_time("int128_t OR", [&]() {
        auto result = int_a | int_b;
        (void)result;
    });

    // XOR
    auto time_uint_xor = measure_time("uint128_t XOR", [&]() {
        auto result = uint_a ^ uint_b;
        (void)result;
    });

    auto time_int_xor = measure_time("int128_t XOR", [&]() {
        auto result = int_a ^ int_b;
        (void)result;
    });

    // Shift left
    auto time_uint_shl = measure_time("uint128_t shift left", [&]() {
        auto result = uint_a << 5;
        (void)result;
    });

    auto time_int_shl = measure_time("int128_t shift left", [&]() {
        auto result = int_a << 5;
        (void)result;
    });

    // Shift right
    auto time_uint_shr = measure_time("uint128_t shift right", [&]() {
        auto result = uint_a >> 5;
        (void)result;
    });

    auto time_int_shr = measure_time("int128_t shift right (arithmetic)", [&]() {
        auto result = int_a >> 5;
        (void)result;
    });

    // Resultados
    std::cout << "AND (&):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_and << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_and << " ns  ";
    std::cout << "(overhead: " << ((time_int_and / time_uint_and - 1.0) * 100) << "%)" << std::endl;

    std::cout << "OR (|):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_or << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_or << " ns  ";
    std::cout << "(overhead: " << ((time_int_or / time_uint_or - 1.0) * 100) << "%)" << std::endl;

    std::cout << "XOR (^):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_xor << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_xor << " ns  ";
    std::cout << "(overhead: " << ((time_int_xor / time_uint_xor - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Shift left (<<):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_shl << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_shl << " ns  ";
    std::cout << "(overhead: " << ((time_int_shl / time_uint_shl - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Shift right (>>):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_shr << " ns (logical)" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_shr << " ns (arithmetic)  ";
    std::cout << "(overhead: " << ((time_int_shr / time_uint_shr - 1.0) * 100) << "%)" << std::endl;
}

// ============================================================================
// BENCHMARKS: CONVERSIONES DE STRING
// ============================================================================

void benchmark_string_conversions()
{
    std::cout << "\n=== CONVERSIONES DE STRING ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t uint_val(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    int128_t int_val_positive(static_cast<int64_t>(0x0123456789ABCDEFLL),
                              static_cast<uint64_t>(0xFEDCBA9876543210ULL));
    int128_t int_val_negative(-123456789012345LL);

    // to_string
    auto time_uint_to_string = measure_time("uint128_t to_string", [&]() {
        auto result = uint_val.to_string();
        (void)result;
    });

    auto time_int_to_string_pos = measure_time("int128_t to_string (positive)", [&]() {
        auto result = int_val_positive.to_string();
        (void)result;
    });

    auto time_int_to_string_neg = measure_time("int128_t to_string (negative)", [&]() {
        auto result = int_val_negative.to_string();
        (void)result;
    });

    // from_string
    std::string uint_str = "123456789012345678901234567890";
    std::string int_str_pos = "123456789012345678901234567890";
    std::string int_str_neg = "-123456789012345678901234567890";

    auto time_uint_from_string = measure_time("uint128_t from_string", [&]() {
        auto result = uint128_t::from_string(uint_str);
        (void)result;
    });

    auto time_int_from_string_pos = measure_time("int128_t from_string (positive)", [&]() {
        auto result = int128_t::from_string(int_str_pos);
        (void)result;
    });

    auto time_int_from_string_neg = measure_time("int128_t from_string (negative)", [&]() {
        auto result = int128_t::from_string(int_str_neg);
        (void)result;
    });

    // Resultados
    std::cout << "to_string():" << std::endl;
    std::cout << "  uint128_t:            " << std::setw(8) << time_uint_to_string << " ns"
              << std::endl;
    std::cout << "  int128_t (positive):  " << std::setw(8) << time_int_to_string_pos << " ns  ";
    std::cout << "(overhead: " << ((time_int_to_string_pos / time_uint_to_string - 1.0) * 100)
              << "%)" << std::endl;
    std::cout << "  int128_t (negative):  " << std::setw(8) << time_int_to_string_neg << " ns"
              << std::endl;

    std::cout << "from_string():" << std::endl;
    std::cout << "  uint128_t:            " << std::setw(8) << time_uint_from_string << " ns"
              << std::endl;
    std::cout << "  int128_t (positive):  " << std::setw(8) << time_int_from_string_pos << " ns  ";
    std::cout << "(overhead: " << ((time_int_from_string_pos / time_uint_from_string - 1.0) * 100)
              << "%)" << std::endl;
    std::cout << "  int128_t (negative):  " << std::setw(8) << time_int_from_string_neg << " ns"
              << std::endl;
}

// ============================================================================
// BENCHMARKS: FUNCIONES MATEMÁTICAS
// ============================================================================

void benchmark_math_functions()
{
    std::cout << "\n=== FUNCIONES MATEMÁTICAS ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t uint_a(12345678901234567890ULL);
    uint128_t uint_b(98765432109876543210ULL);

    int128_t int_a(12345678901234567890LL);
    int128_t int_b(98765432109876543210LL);
    int128_t int_neg(-12345678901234567890LL);

    // GCD
    auto time_uint_gcd = measure_time("uint128_t gcd", [&]() {
        auto result = nstd::gcd(uint_a, uint_b);
        (void)result;
    });

    auto time_int_gcd = measure_time("int128_t gcd", [&]() {
        auto result = nstd::gcd(int_a, int_b);
        (void)result;
    });

    // LCM
    auto time_uint_lcm = measure_time("uint128_t lcm", [&]() {
        auto result = nstd::lcm(uint_a, uint_b);
        (void)result;
    });

    auto time_int_lcm = measure_time("int128_t lcm", [&]() {
        auto result = nstd::lcm(int_a, int_b);
        (void)result;
    });

    // Pow
    auto time_uint_pow = measure_time("uint128_t pow", [&]() {
        auto result = nstd::pow(uint128_t(2), uint128_t(10));
        (void)result;
    });

    auto time_int_pow = measure_time("int128_t pow", [&]() {
        auto result = nstd::pow(int128_t(2), int128_t(10));
        (void)result;
    });

    // Sqrt
    auto time_uint_sqrt = measure_time("uint128_t sqrt", [&]() {
        auto result = nstd::sqrt(uint_a);
        (void)result;
    });

    auto time_int_sqrt = measure_time("int128_t sqrt", [&]() {
        auto result = nstd::sqrt(int_a);
        (void)result;
    });

    // Abs (int128_t específico)
    auto time_int_abs = measure_time("int128_t abs", [&]() {
        auto result = nstd::abs(int_neg);
        (void)result;
    });

    // Resultados
    std::cout << "GCD:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_gcd << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_gcd << " ns  ";
    std::cout << "(overhead: " << ((time_int_gcd / time_uint_gcd - 1.0) * 100) << "%)" << std::endl;

    std::cout << "LCM:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_lcm << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_lcm << " ns  ";
    std::cout << "(overhead: " << ((time_int_lcm / time_uint_lcm - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Pow:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_pow << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_pow << " ns  ";
    std::cout << "(overhead: " << ((time_int_pow / time_uint_pow - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Sqrt:" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_sqrt << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_sqrt << " ns  ";
    std::cout << "(overhead: " << ((time_int_sqrt / time_uint_sqrt - 1.0) * 100) << "%)"
              << std::endl;

    std::cout << "Abs (int128_t only):" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_abs << " ns" << std::endl;
}

// ============================================================================
// BENCHMARKS: COMPARACIONES
// ============================================================================

void benchmark_comparisons()
{
    std::cout << "\n=== OPERADORES DE COMPARACIÓN ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t uint_a(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    uint128_t uint_b(0xABCDEF0123456789ULL, 0x0123456789ABCDEFULL);

    int128_t int_a(static_cast<int64_t>(0x0123456789ABCDEFLL),
                   static_cast<uint64_t>(0xFEDCBA9876543210ULL));
    int128_t int_b(static_cast<int64_t>(0x00CDEF0123456789LL),
                   static_cast<uint64_t>(0x0123456789ABCDEFULL));

    // Igual
    auto time_uint_eq = measure_time("uint128_t ==", [&]() {
        volatile bool result = (uint_a == uint_b);
        (void)result;
    });

    auto time_int_eq = measure_time("int128_t ==", [&]() {
        volatile bool result = (int_a == int_b);
        (void)result;
    });

    // Menor que
    auto time_uint_lt = measure_time("uint128_t <", [&]() {
        volatile bool result = (uint_a < uint_b);
        (void)result;
    });

    auto time_int_lt = measure_time("int128_t <", [&]() {
        volatile bool result = (int_a < int_b);
        (void)result;
    });

    // Mayor que
    auto time_uint_gt = measure_time("uint128_t >", [&]() {
        volatile bool result = (uint_a > uint_b);
        (void)result;
    });

    auto time_int_gt = measure_time("int128_t >", [&]() {
        volatile bool result = (int_a > int_b);
        (void)result;
    });

    // Resultados
    std::cout << "Equality (==):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_eq << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_eq << " ns  ";
    std::cout << "(overhead: " << ((time_int_eq / time_uint_eq - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Less than (<):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_lt << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_lt << " ns  ";
    std::cout << "(overhead: " << ((time_int_lt / time_uint_lt - 1.0) * 100) << "%)" << std::endl;

    std::cout << "Greater than (>):" << std::endl;
    std::cout << "  uint128_t: " << std::setw(8) << time_uint_gt << " ns" << std::endl;
    std::cout << "  int128_t:  " << std::setw(8) << time_int_gt << " ns  ";
    std::cout << "(overhead: " << ((time_int_gt / time_uint_gt - 1.0) * 100) << "%)" << std::endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "=============================================================================="
              << std::endl;
    std::cout << "BENCHMARK COMPARATIVO: int128_t vs uint128_t" << std::endl;
    std::cout << "=============================================================================="
              << std::endl;
    std::cout << "Iteraciones: " << ITERATIONS << " (warm-up: " << WARM_UP << ")" << std::endl;
    std::cout << "Objetivo: Medir el overhead del manejo de signo en int128_t" << std::endl;
    std::cout << "Overhead positivo indica int128_t más lento que uint128_t" << std::endl;
    std::cout << "Overhead negativo indica int128_t más rápido que uint128_t" << std::endl;

    benchmark_construction();
    benchmark_arithmetic();
    benchmark_bitwise();
    benchmark_string_conversions();
    benchmark_math_functions();
    benchmark_comparisons();

    std::cout << "\n=============================================================================="
              << std::endl;
    std::cout << "RESUMEN:" << std::endl;
    std::cout << "=============================================================================="
              << std::endl;
    std::cout << "El overhead típico de int128_t sobre uint128_t se debe a:" << std::endl;
    std::cout << "  1. Manejo de signo en operaciones aritméticas" << std::endl;
    std::cout << "  2. Arithmetic right shift (propaga bit de signo)" << std::endl;
    std::cout << "  3. Verificaciones adicionales en división/módulo" << std::endl;
    std::cout << "  4. Procesamiento de signo en conversiones string" << std::endl;
    std::cout << "\nOperaciones sin overhead significativo:" << std::endl;
    std::cout << "  - Construcción y copia (misma estructura de datos)" << std::endl;
    std::cout << "  - Operaciones bitwise AND, OR, XOR (neutras al signo)" << std::endl;
    std::cout << "  - Comparaciones de igualdad (misma representación bits)" << std::endl;
    std::cout << "=============================================================================="
              << std::endl;

    return 0;
}
