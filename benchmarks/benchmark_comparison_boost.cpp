/**
 * Benchmark Comparativo: uint128_t vs Boost.Multiprecision (128-bit fixed)
 *
 * Compara:
 * - uint128_t (nuestra implementación)
 * - boost::multiprecision::uint128_t (cpp_int 128-bit)
 * - boost::multiprecision::checked_uint128_t (con overflow checking)
 * - boost::multiprecision::number<gmp_int> (GMP backend)
 * - boost::multiprecision::number<tommath_int> (tommath backend)
 *
 * Operaciones benchmarked:
 * - Construcción y asignación
 * - Aritmética básica (+, -, *, /, %)
 * - Operaciones bit a bit (&, |, ^, <<, >>)
 * - Conversiones string
 * - Hash computation
 * - Funciones matemáticas (gcd, lcm, pow, sqrt)
 */

#include "uint128/uint128_cmath.hpp"
#include "uint128/uint128_t.hpp"
#include <boost/multiprecision/cpp_int.hpp>

// Opcional: Descomentar si tienes GMP/tommath instalados
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/tommath.hpp>

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std::chrono;

// Tipos de Boost.Multiprecision (128-bit fixed)
namespace mp = boost::multiprecision;
using boost_uint128_t = mp::uint128_t;
using boost_checked_uint128_t = mp::checked_uint128_t;

// Opcional: GMP y tommath backends
using gmp_uint128_t = mp::number<mp::gmp_int, mp::et_off>;
using tom_uint128_t = mp::number<mp::tommath_int, mp::et_off>;

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
    auto time_our_default = measure_time("uint128_t default ctor", []() {
        uint128_t val;
        (void)val;
    });

    auto time_our_uint64 = measure_time("uint128_t from uint64_t", []() {
        uint128_t val(0x123456789ABCDEF0ULL);
        (void)val;
    });

    auto time_our_copy = measure_time("uint128_t copy", []() {
        static uint128_t src(0x123456789ABCDEF0ULL);
        uint128_t val(src);
        (void)val;
    });

    // Boost cpp_int
    auto time_boost_default = measure_time("boost_uint128_t default ctor", []() {
        boost_uint128_t val;
        (void)val;
    });

    auto time_boost_uint64 = measure_time("boost_uint128_t from uint64_t", []() {
        boost_uint128_t val(0x123456789ABCDEF0ULL);
        (void)val;
    });

    auto time_boost_copy = measure_time("boost_uint128_t copy", []() {
        static boost_uint128_t src(0x123456789ABCDEF0ULL);
        boost_uint128_t val(src);
        (void)val;
    });

    std::cout << "Default constructor:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_default << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_default << " ns  ";
    std::cout << "(speedup: " << time_boost_default / time_our_default << "x)" << std::endl;

    std::cout << "From uint64_t:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_uint64 << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_uint64 << " ns  ";
    std::cout << "(speedup: " << time_boost_uint64 / time_our_uint64 << "x)" << std::endl;

    std::cout << "Copy constructor:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_copy << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_copy << " ns  ";
    std::cout << "(speedup: " << time_boost_copy / time_our_copy << "x)" << std::endl;
}

// ============================================================================
// BENCHMARKS: ARITMÉTICA BÁSICA
// ============================================================================

void benchmark_arithmetic()
{
    std::cout << "\n=== ARITMÉTICA BÁSICA ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Valores de prueba
    uint128_t our_a(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    uint128_t our_b(0xABCDEF0123456789ULL, 0x0123456789ABCDEFULL);

    boost_uint128_t boost_a(std::string("123456789012345678901234567890"));
    boost_uint128_t boost_b(std::string("98765432109876543210987654321"));

    // Suma
    auto time_our_add = measure_time("uint128_t addition", [&]() {
        auto result = our_a + our_b;
        (void)result;
    });

    auto time_boost_add = measure_time("boost_uint128_t addition", [&]() {
        auto result = boost_a + boost_b;
        (void)result;
    });

    // Resta
    auto time_our_sub = measure_time("uint128_t subtraction", [&]() {
        auto result = our_a - our_b;
        (void)result;
    });

    auto time_boost_sub = measure_time("boost_uint128_t subtraction", [&]() {
        auto result = boost_a - boost_b;
        (void)result;
    });

    // Multiplicación
    auto time_our_mul = measure_time("uint128_t multiplication", [&]() {
        auto result = our_a * uint128_t(12345);
        (void)result;
    });

    auto time_boost_mul = measure_time("boost_uint128_t multiplication", [&]() {
        auto result = boost_a * boost_uint128_t(12345);
        (void)result;
    });

    // División
    auto time_our_div = measure_time("uint128_t division", [&]() {
        auto result = our_a / uint128_t(12345);
        (void)result;
    });

    auto time_boost_div = measure_time("boost_uint128_t division", [&]() {
        auto result = boost_a / boost_uint128_t(12345);
        (void)result;
    });

    // Módulo
    auto time_our_mod = measure_time("uint128_t modulo", [&]() {
        auto result = our_a % uint128_t(12345);
        (void)result;
    });

    auto time_boost_mod = measure_time("boost_uint128_t modulo", [&]() {
        auto result = boost_a % boost_uint128_t(12345);
        (void)result;
    });

    std::cout << "Addition (+):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_add << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_add << " ns  ";
    std::cout << "(speedup: " << time_boost_add / time_our_add << "x)" << std::endl;

    std::cout << "Subtraction (-):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_sub << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_sub << " ns  ";
    std::cout << "(speedup: " << time_boost_sub / time_our_sub << "x)" << std::endl;

    std::cout << "Multiplication (*):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_mul << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_mul << " ns  ";
    std::cout << "(speedup: " << time_boost_mul / time_our_mul << "x)" << std::endl;

    std::cout << "Division (/):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_div << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_div << " ns  ";
    std::cout << "(speedup: " << time_boost_div / time_our_div << "x)" << std::endl;

    std::cout << "Modulo (%):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_mod << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_mod << " ns  ";
    std::cout << "(speedup: " << time_boost_mod / time_our_mod << "x)" << std::endl;
}

// ============================================================================
// BENCHMARKS: OPERACIONES BIT A BIT
// ============================================================================

void benchmark_bitwise()
{
    std::cout << "\n=== OPERACIONES BIT A BIT ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t our_a(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    uint128_t our_b(0xABCDEF0123456789ULL, 0x0123456789ABCDEFULL);

    boost_uint128_t boost_a(std::string("123456789012345678901234567890"));
    boost_uint128_t boost_b(std::string("98765432109876543210987654321"));

    // AND
    auto time_our_and = measure_time("uint128_t AND (&)", [&]() {
        auto result = our_a & our_b;
        (void)result;
    });

    auto time_boost_and = measure_time("boost_uint128_t AND (&)", [&]() {
        auto result = boost_a & boost_b;
        (void)result;
    });

    // OR
    auto time_our_or = measure_time("uint128_t OR (|)", [&]() {
        auto result = our_a | our_b;
        (void)result;
    });

    auto time_boost_or = measure_time("boost_uint128_t OR (|)", [&]() {
        auto result = boost_a | boost_b;
        (void)result;
    });

    // XOR
    auto time_our_xor = measure_time("uint128_t XOR (^)", [&]() {
        auto result = our_a ^ our_b;
        (void)result;
    });

    auto time_boost_xor = measure_time("boost_uint128_t XOR (^)", [&]() {
        auto result = boost_a ^ boost_b;
        (void)result;
    });

    // Left shift
    auto time_our_shl = measure_time("uint128_t left shift (<<)", [&]() {
        auto result = our_a << 10;
        (void)result;
    });

    auto time_boost_shl = measure_time("boost_uint128_t left shift (<<)", [&]() {
        auto result = boost_a << 10;
        (void)result;
    });

    // Right shift
    auto time_our_shr = measure_time("uint128_t right shift (>>)", [&]() {
        auto result = our_a >> 10;
        (void)result;
    });

    auto time_boost_shr = measure_time("boost_uint128_t right shift (>>)", [&]() {
        auto result = boost_a >> 10;
        (void)result;
    });

    std::cout << "Bitwise AND (&):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_and << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_and << " ns  ";
    std::cout << "(speedup: " << time_boost_and / time_our_and << "x)" << std::endl;

    std::cout << "Bitwise OR (|):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_or << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_or << " ns  ";
    std::cout << "(speedup: " << time_boost_or / time_our_or << "x)" << std::endl;

    std::cout << "Bitwise XOR (^):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_xor << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_xor << " ns  ";
    std::cout << "(speedup: " << time_boost_xor / time_our_xor << "x)" << std::endl;

    std::cout << "Left shift (<<):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_shl << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_shl << " ns  ";
    std::cout << "(speedup: " << time_boost_shl / time_our_shl << "x)" << std::endl;

    std::cout << "Right shift (>>):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_shr << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_shr << " ns  ";
    std::cout << "(speedup: " << time_boost_shr / time_our_shr << "x)" << std::endl;
}

// ============================================================================
// BENCHMARKS: CONVERSIONES STRING
// ============================================================================

void benchmark_string_conversions()
{
    std::cout << "\n=== CONVERSIONES STRING ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t our_val(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    boost_uint128_t boost_val(std::string("123456789012345678901234567890"));

    // to_string decimal
    auto time_our_to_dec = measure_time("uint128_t to_string (decimal)", [&]() {
        auto str = our_val.to_string();
        (void)str;
    });

    auto time_boost_to_dec = measure_time("boost_uint128_t to_string (decimal)", [&]() {
        auto str = boost_val.str();
        (void)str;
    });

    // to_string hex
    auto time_our_to_hex = measure_time("uint128_t to_string_hex", [&]() {
        auto str = our_val.to_string_hex();
        (void)str;
    });

    auto time_boost_to_hex = measure_time("boost_uint128_t to hex string", [&]() {
        std::stringstream ss;
        ss << std::hex << boost_val;
        auto str = ss.str();
        (void)str;
    });

    // from_string
    auto time_our_from_string = measure_time("uint128_t from_string", []() {
        auto val = uint128_t::from_string("123456789012345678901234567890");
        (void)val;
    });

    auto time_boost_from_string = measure_time("boost_uint128_t from string", []() {
        boost_uint128_t val(std::string("123456789012345678901234567890"));
        (void)val;
    });

    std::cout << "to_string (decimal):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_to_dec << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_to_dec << " ns  ";
    std::cout << "(speedup: " << time_boost_to_dec / time_our_to_dec << "x)" << std::endl;

    std::cout << "to_string (hex):" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_to_hex << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_to_hex << " ns  ";
    std::cout << "(speedup: " << time_boost_to_hex / time_our_to_hex << "x)" << std::endl;

    std::cout << "from_string:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_from_string << " ns"
              << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_from_string << " ns  ";
    std::cout << "(speedup: " << time_boost_from_string / time_our_from_string << "x)" << std::endl;
}

// ============================================================================
// BENCHMARKS: FUNCIONES MATEMÁTICAS
// ============================================================================

void benchmark_math_functions()
{
    std::cout << "\n=== FUNCIONES MATEMÁTICAS ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    uint128_t our_a(48);
    uint128_t our_b(18);

    boost_uint128_t boost_a(48);
    boost_uint128_t boost_b(18);

    // GCD
    auto time_our_gcd = measure_time("uint128_t gcd", [&]() {
        auto result = std::gcd(our_a, our_b);
        (void)result;
    });

    auto time_boost_gcd = measure_time("boost_uint128_t gcd", [&]() {
        auto result = boost::multiprecision::gcd(boost_a, boost_b);
        (void)result;
    });

    // LCM
    auto time_our_lcm = measure_time("uint128_t lcm", [&]() {
        auto result = std::lcm(our_a, our_b);
        (void)result;
    });

    auto time_boost_lcm = measure_time("boost_uint128_t lcm", [&]() {
        auto result = boost::multiprecision::lcm(boost_a, boost_b);
        (void)result;
    });

    // Pow
    auto time_our_pow = measure_time("uint128_t pow", [&]() {
        auto result = std::pow(uint128_t(2), uint128_t(10));
        (void)result;
    });

    auto time_boost_pow = measure_time("boost_uint128_t pow", [&]() {
        auto result = boost::multiprecision::pow(boost_uint128_t(2), 10);
        (void)result;
    });

    // Sqrt
    auto time_our_sqrt = measure_time("uint128_t sqrt", [&]() {
        auto result = std::sqrt(uint128_t(12345678901234567890ULL));
        (void)result;
    });

    auto time_boost_sqrt = measure_time("boost_uint128_t sqrt", [&]() {
        auto result =
            boost::multiprecision::sqrt(boost_uint128_t(std::string("12345678901234567890")));
        (void)result;
    });

    std::cout << "GCD:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_gcd << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_gcd << " ns  ";
    std::cout << "(speedup: " << time_boost_gcd / time_our_gcd << "x)" << std::endl;

    std::cout << "LCM:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_lcm << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_lcm << " ns  ";
    std::cout << "(speedup: " << time_boost_lcm / time_our_lcm << "x)" << std::endl;

    std::cout << "Pow:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_pow << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_pow << " ns  ";
    std::cout << "(speedup: " << time_boost_pow / time_our_pow << "x)" << std::endl;

    std::cout << "Sqrt:" << std::endl;
    std::cout << "  uint128_t:       " << std::setw(8) << time_our_sqrt << " ns" << std::endl;
    std::cout << "  boost_uint128_t: " << std::setw(8) << time_boost_sqrt << " ns  ";
    std::cout << "(speedup: " << time_boost_sqrt / time_our_sqrt << "x)" << std::endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "=============================================================================="
              << std::endl;
    std::cout << "BENCHMARK COMPARATIVO: uint128_t vs Boost.Multiprecision" << std::endl;
    std::cout << "=============================================================================="
              << std::endl;
    std::cout << "Iteraciones: " << ITERATIONS << " (warm-up: " << WARM_UP << ")" << std::endl;
    std::cout << "Nota: speedup > 1.0 significa uint128_t es más rápido" << std::endl;

    benchmark_construction();
    benchmark_arithmetic();
    benchmark_bitwise();
    benchmark_string_conversions();
    benchmark_math_functions();

    std::cout << "\n=============================================================================="
              << std::endl;
    std::cout << "BENCHMARK COMPLETO" << std::endl;
    std::cout << "=============================================================================="
              << std::endl;

    return 0;
}
