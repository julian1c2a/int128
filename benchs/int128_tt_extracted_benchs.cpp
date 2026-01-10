/**
 * @file int128_tt_extracted_benchs.cpp
 * @brief Benchmarks unificados para int128_base_t (uint128_t e int128_t)
 *
 * Este archivo contiene benchmarks del template unificado int128_base_t<S>
 * comparando rendimiento entre:
 * - nstd::uint128_t / nstd::int128_t (nuestra implementación)
 * - __uint128_t / __int128_t (GCC/Clang nativo)
 * - boost::multiprecision con 3 backends:
 *   - cpp_int (pure C++, header-only)
 *   - gmp_int (GNU MP backend, requiere libgmp)
 *   - tommath_int (libtommath backend, requiere libtommath)
 */

#include "int128_base_tt.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

// Boost.Multiprecision backends
#include <boost/multiprecision/cpp_int.hpp>

// NOTA: gmp_int y tom_int son tipos de precisión ARBITRARIA, no 128-bit fijos.
// Esto los hace más lentos y no comparables directamente con uint128_t.
// Deshabilitados para benchmarks válidos - usamos solo cpp_int que es header-only.

// #define HAS_BOOST_GMP 1
// #define HAS_BOOST_TOMMATH 1

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

// Native 128-bit types (GCC/Clang only)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
#define HAS_NATIVE_INT128 1
#endif

using namespace nstd;
using namespace std::chrono;
namespace mp = boost::multiprecision;

// Boost type aliases
using boost_cpp_uint128 = mp::uint128_t; // cpp_int backend (Boost's default uint128_t)
using boost_cpp_int128 = mp::int128_t;   // cpp_int backend signed

#ifdef HAS_BOOST_GMP
using boost_gmp_uint128 = mp::number<mp::gmp_int, mp::et_off>;
using boost_gmp_int128 = mp::number<mp::gmp_int, mp::et_off>;
#endif

#ifdef HAS_BOOST_TOMMATH
using boost_tom_uint128 = mp::tom_int;
using boost_tom_int128 = mp::tom_int;
#endif

// Función para leer ciclos de CPU (rdtsc)
inline uint64_t rdtsc()
{
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    return __rdtsc();
#elif defined(__x86_64__) || defined(__i386__)
    uint32_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#else
    return 0;
#endif
}

// Generadores aleatorios
std::mt19937_64 rng(std::random_device{}());

inline uint64_t random_u64()
{
    return rng();
}

inline int64_t random_i64()
{
    return static_cast<int64_t>(rng());
}

inline uint128_t random_uint128()
{
    return uint128_t(random_u64(), random_u64());
}

inline int128_t random_int128()
{
    return int128_t(static_cast<uint64_t>(random_i64()), random_u64());
}

inline boost_cpp_uint128 random_boost_cpp_uint128()
{
    boost_cpp_uint128 result = random_u64();
    result <<= 64;
    result |= random_u64();
    return result;
}

inline boost_cpp_int128 random_boost_cpp_int128()
{
    boost_cpp_int128 result = random_i64();
    result <<= 64;
    result |= random_u64();
    return result;
}

#ifdef HAS_BOOST_GMP
inline boost_gmp_uint128 random_boost_gmp_uint128()
{
    boost_gmp_uint128 result = random_u64();
    result <<= 64;
    result |= random_u64();
    return result;
}
#endif

#ifdef HAS_BOOST_TOMMATH
inline boost_tom_uint128 random_boost_tom_uint128()
{
    boost_tom_uint128 result = random_u64();
    result <<= 64;
    result |= random_u64();
    return result;
}
#endif

// Macro BENCHMARK simple - el código debe ser self-contained
#define BENCHMARK(name, type_name, iterations, code)                                                \
    {                                                                                               \
        auto start_time = high_resolution_clock::now();                                             \
        uint64_t start_cycles = rdtsc();                                                            \
        for (size_t _i = 0; _i < iterations; ++_i)                                                  \
        {                                                                                           \
            code;                                                                                   \
        }                                                                                           \
        uint64_t end_cycles = rdtsc();                                                              \
        auto end_time = high_resolution_clock::now();                                               \
        double ns = duration_cast<nanoseconds>(end_time - start_time).count() / double(iterations); \
        double cycles = double(end_cycles - start_cycles) / double(iterations);                     \
        std::cout << "  " << std::setw(25) << std::left << name                                     \
                  << " [" << std::setw(14) << type_name << "]"                                      \
                  << std::fixed << std::setprecision(2)                                             \
                  << std::setw(10) << ns << " ns/op"                                                \
                  << std::setw(12) << cycles << " cycles/op\n";                                     \
    }

constexpr size_t ITERATIONS = 1000000;
constexpr size_t STR_ITERATIONS = 100000;
constexpr size_t DIV_ITERATIONS = ITERATIONS / 10;

// ============= Construction Benchmarks =============
void benchmark_construction()
{
    std::cout << "\n=== CONSTRUCTION ===\n";

    // nstd types
    BENCHMARK("default constructor", "uint128_t", ITERATIONS, {
        volatile uint128_t x;
        (void)x;
    });

    BENCHMARK("default constructor", "int128_t", ITERATIONS, {
        volatile int128_t x;
        (void)x;
    });

    BENCHMARK("from uint64", "uint128_t", ITERATIONS, {
        uint64_t v = random_u64();
        uint128_t x(v);
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("from int64", "int128_t", ITERATIONS, {
        int64_t v = random_i64();
        int128_t x(v);
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("from pair (h,l)", "uint128_t", ITERATIONS, {
        uint64_t h = random_u64();
        uint64_t l = random_u64();
        uint128_t x(h, l);
        volatile auto sink = x.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("from uint64", "__uint128_t", ITERATIONS, {
        uint64_t v = random_u64();
        __uint128_t x = v;
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });
#endif

    // Boost cpp_int backend
    BENCHMARK("from uint64", "boost_cpp", ITERATIONS, {
        uint64_t v = random_u64();
        boost_cpp_uint128 x(v);
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("from uint64", "boost_gmp", ITERATIONS, {
        uint64_t v = random_u64();
        boost_gmp_uint128 x(v);
        volatile auto sink = x.convert_to<uint64_t>();
        (void)sink;
    });
#endif

#ifdef HAS_BOOST_TOMMATH
    BENCHMARK("from uint64", "boost_tom", ITERATIONS, {
        uint64_t v = random_u64();
        boost_tom_uint128 x(v);
        volatile auto sink = x.convert_to<uint64_t>();
        (void)sink;
    });
#endif
}

// ============= Arithmetic Benchmarks =============
void benchmark_arithmetic()
{
    std::cout << "\n=== ARITHMETIC ===\n";

    // === ADDITION ===
    std::cout << "  --- Addition ---\n";

    BENCHMARK("addition", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a + b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("addition", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a + b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("addition", "uint64_t", ITERATIONS, {
        uint64_t a = random_u64();
        uint64_t b = random_u64();
        uint64_t c = a + b;
        volatile auto sink = c;
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("addition", "__uint128_t", ITERATIONS, {
        __uint128_t a = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t b = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t c = a + b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });
#endif

    BENCHMARK("addition", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        boost_cpp_uint128 c = a + b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("addition", "boost_gmp", ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        boost_gmp_uint128 b = random_boost_gmp_uint128();
        boost_gmp_uint128 c = a + b;
        volatile auto sink = c.convert_to<uint64_t>();
        (void)sink;
    });
#endif

#ifdef HAS_BOOST_TOMMATH
    BENCHMARK("addition", "boost_tom", ITERATIONS, {
        boost_tom_uint128 a = random_boost_tom_uint128();
        boost_tom_uint128 b = random_boost_tom_uint128();
        boost_tom_uint128 c = a + b;
        volatile auto sink = c.convert_to<uint64_t>();
        (void)sink;
    });
#endif

    // === SUBTRACTION ===
    std::cout << "  --- Subtraction ---\n";

    BENCHMARK("subtraction", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a - b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("subtraction", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a - b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("subtraction", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        boost_cpp_uint128 c = a - b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("subtraction", "boost_gmp", ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        boost_gmp_uint128 b = random_boost_gmp_uint128();
        boost_gmp_uint128 c = a - b;
        volatile auto sink = c.convert_to<uint64_t>();
        (void)sink;
    });
#endif

    // === MULTIPLICATION ===
    std::cout << "  --- Multiplication ---\n";

    BENCHMARK("multiplication", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a * b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("multiplication", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        int128_t c = a * b;
        volatile auto sink = c.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("multiplication", "__uint128_t", ITERATIONS, {
        __uint128_t a = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t b = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t c = a * b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });
#endif

    BENCHMARK("multiplication", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        boost_cpp_uint128 c = a * b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("multiplication", "boost_gmp", ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        boost_gmp_uint128 b = random_boost_gmp_uint128();
        boost_gmp_uint128 c = a * b;
        volatile auto sink = c.convert_to<uint64_t>();
        (void)sink;
    });
#endif

#ifdef HAS_BOOST_TOMMATH
    BENCHMARK("multiplication", "boost_tom", ITERATIONS, {
        boost_tom_uint128 a = random_boost_tom_uint128();
        boost_tom_uint128 b = random_boost_tom_uint128();
        boost_tom_uint128 c = a * b;
        volatile auto sink = c.convert_to<uint64_t>();
        (void)sink;
    });
#endif

    // === DIVISION ===
    std::cout << "  --- Division ---\n";

    BENCHMARK("division", "uint128_t", DIV_ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if (b != uint128_t(0))
        {
            uint128_t c = a / b;
            volatile auto sink = c.low();
            (void)sink;
        }
    });

    BENCHMARK("division", "int128_t", DIV_ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        if (b != int128_t(0))
        {
            int128_t c = a / b;
            volatile auto sink = c.low();
            (void)sink;
        }
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("division", "__uint128_t", DIV_ITERATIONS, {
        __uint128_t a = ((__uint128_t)random_u64() << 64) | random_u64();
        __uint128_t b = ((__uint128_t)random_u64() << 64) | random_u64();
        if (b != 0)
        {
            __uint128_t c = a / b;
            volatile auto sink = static_cast<uint64_t>(c);
            (void)sink;
        }
    });
#endif

    BENCHMARK("division", "boost_cpp", DIV_ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        if (b != 0)
        {
            boost_cpp_uint128 c = a / b;
            volatile auto sink = static_cast<uint64_t>(c);
            (void)sink;
        }
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("division", "boost_gmp", DIV_ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        boost_gmp_uint128 b = random_boost_gmp_uint128();
        if (b != 0)
        {
            boost_gmp_uint128 c = a / b;
            volatile auto sink = c.convert_to<uint64_t>();
            (void)sink;
        }
    });
#endif

#ifdef HAS_BOOST_TOMMATH
    BENCHMARK("division", "boost_tom", DIV_ITERATIONS, {
        boost_tom_uint128 a = random_boost_tom_uint128();
        boost_tom_uint128 b = random_boost_tom_uint128();
        if (b != 0)
        {
            boost_tom_uint128 c = a / b;
            volatile auto sink = c.convert_to<uint64_t>();
            (void)sink;
        }
    });
#endif
}

// ============= Bitwise Benchmarks =============
void benchmark_bitwise()
{
    std::cout << "\n=== BITWISE OPERATIONS ===\n";

    BENCHMARK("AND", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a & b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("AND", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        boost_cpp_uint128 c = a & b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

    BENCHMARK("OR", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a | b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("OR", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        boost_cpp_uint128 c = a | b;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

    BENCHMARK("XOR", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = a ^ b;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("NOT", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t c = ~a;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("left shift", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        int shift = rng() % 128;
        uint128_t c = a << shift;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("left shift", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        int shift = rng() % 128;
        boost_cpp_uint128 c = a << shift;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });

    BENCHMARK("right shift", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        int shift = rng() % 128;
        uint128_t c = a >> shift;
        volatile auto sink = c.low();
        (void)sink;
    });

    BENCHMARK("right shift", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        int shift = rng() % 128;
        boost_cpp_uint128 c = a >> shift;
        volatile auto sink = static_cast<uint64_t>(c);
        (void)sink;
    });
}

// ============= Comparison Benchmarks =============
void benchmark_comparison()
{
    std::cout << "\n=== COMPARISON ===\n";

    BENCHMARK("equality", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        volatile bool r = (a == b);
        (void)r;
    });

    BENCHMARK("equality", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        volatile bool r = (a == b);
        (void)r;
    });

    BENCHMARK("less than", "uint128_t", ITERATIONS, {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        volatile bool r = (a < b);
        (void)r;
    });

    BENCHMARK("less than", "int128_t", ITERATIONS, {
        int128_t a = random_int128();
        int128_t b = random_int128();
        volatile bool r = (a < b);
        (void)r;
    });

    BENCHMARK("less than", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        boost_cpp_uint128 b = random_boost_cpp_uint128();
        volatile bool r = (a < b);
        (void)r;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("less than", "boost_gmp", ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        boost_gmp_uint128 b = random_boost_gmp_uint128();
        volatile bool r = (a < b);
        (void)r;
    });
#endif
}

// ============= String Conversion Benchmarks =============
void benchmark_string()
{
    std::cout << "\n=== STRING CONVERSIONS ===\n";

    BENCHMARK("to_string (dec)", "uint128_t", STR_ITERATIONS, {
        uint128_t a = random_uint128();
        std::string s = a.to_string();
        volatile auto sink = s.size();
        (void)sink;
    });

    BENCHMARK("to_string (dec)", "int128_t", STR_ITERATIONS, {
        int128_t a = random_int128();
        std::string s = a.to_string();
        volatile auto sink = s.size();
        (void)sink;
    });

    BENCHMARK("to_string (dec)", "boost_cpp", STR_ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        std::string s = a.str();
        volatile auto sink = s.size();
        (void)sink;
    });

#ifdef HAS_BOOST_GMP
    BENCHMARK("to_string (dec)", "boost_gmp", STR_ITERATIONS, {
        boost_gmp_uint128 a = random_boost_gmp_uint128();
        std::string s = a.str();
        volatile auto sink = s.size();
        (void)sink;
    });
#endif

    BENCHMARK("to_string (hex)", "uint128_t", STR_ITERATIONS, {
        uint128_t a = random_uint128();
        std::string s = a.to_string(16);
        volatile auto sink = s.size();
        (void)sink;
    });

    BENCHMARK("to_string (hex)", "boost_cpp", STR_ITERATIONS, {
        boost_cpp_uint128 a = random_boost_cpp_uint128();
        std::string s = a.str(0, std::ios_base::hex);
        volatile auto sink = s.size();
        (void)sink;
    });
}

// ============= Increment/Decrement Benchmarks =============
void benchmark_incr_decr()
{
    std::cout << "\n=== INCREMENT/DECREMENT ===\n";

    BENCHMARK("++x (pre)", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        ++x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("++x (pre)", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        ++x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("++x (pre)", "boost_cpp", ITERATIONS, {
        boost_cpp_uint128 x = random_boost_cpp_uint128();
        ++x;
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });

    BENCHMARK("--x (pre)", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        --x;
        volatile auto sink = x.low();
        (void)sink;
    });

    BENCHMARK("incr()", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t y = x.incr();
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("decr()", "uint128_t", ITERATIONS, {
        uint128_t x = random_uint128();
        uint128_t y = x.decr();
        volatile auto sink = y.low();
        (void)sink;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("++x (pre)", "__uint128_t", ITERATIONS, {
        __uint128_t x = ((__uint128_t)random_u64() << 64) | random_u64();
        ++x;
        volatile auto sink = static_cast<uint64_t>(x);
        (void)sink;
    });
#endif
}

// ============= Signed-specific Benchmarks =============
void benchmark_signed_ops()
{
    std::cout << "\n=== SIGNED OPERATIONS ===\n";

    BENCHMARK("negate (-x)", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t y = -x;
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("negate (-x)", "boost_cpp", ITERATIONS, {
        boost_cpp_int128 x = random_boost_cpp_int128();
        boost_cpp_int128 y = -x;
        volatile auto sink = static_cast<int64_t>(y);
        (void)sink;
    });

    BENCHMARK("abs()", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        int128_t y = x.abs();
        volatile auto sink = y.low();
        (void)sink;
    });

    BENCHMARK("abs()", "boost_cpp", ITERATIONS, {
        boost_cpp_int128 x = random_boost_cpp_int128();
        boost_cpp_int128 y = mp::abs(x);
        volatile auto sink = static_cast<uint64_t>(y);
        (void)sink;
    });

    BENCHMARK("is_negative()", "int128_t", ITERATIONS, {
        int128_t x = random_int128();
        volatile bool r = x.is_negative();
        (void)r;
    });

    BENCHMARK("sign check", "boost_cpp", ITERATIONS, {
        boost_cpp_int128 x = random_boost_cpp_int128();
        volatile bool r = (x < 0);
        (void)r;
    });

#ifdef HAS_NATIVE_INT128
    BENCHMARK("negate (-x)", "__int128_t", ITERATIONS, {
        __int128_t x = ((__int128_t)random_i64() << 64) | random_u64();
        __int128_t y = -x;
        volatile auto sink = static_cast<int64_t>(y);
        (void)sink;
    });
#endif
}

// ============= Main =============
int main()
{
    std::cout << "========================================\n";
    std::cout << " int128_base_t Unified Benchmarks\n";
    std::cout << " with Boost.Multiprecision Comparison\n";
    std::cout << "========================================\n";
    std::cout << "Iterations: " << ITERATIONS << " (string: " << STR_ITERATIONS << ", div: " << DIV_ITERATIONS << ")\n\n";

    std::cout << "Backends available:\n";
#ifdef HAS_NATIVE_INT128
    std::cout << "  [x] Native __int128 (GCC/Clang)\n";
#else
    std::cout << "  [ ] Native __int128 (not available)\n";
#endif
    std::cout << "  [x] Boost cpp_int (header-only)\n";
#ifdef HAS_BOOST_GMP
    std::cout << "  [x] Boost GMP backend\n";
#else
    std::cout << "  [ ] Boost GMP backend (not available)\n";
#endif
#ifdef HAS_BOOST_TOMMATH
    std::cout << "  [x] Boost libtommath backend\n";
#else
    std::cout << "  [ ] Boost libtommath backend (not available)\n";
#endif

    benchmark_construction();
    benchmark_arithmetic();
    benchmark_bitwise();
    benchmark_comparison();
    benchmark_string();
    benchmark_incr_decr();
    benchmark_signed_ops();

    std::cout << "\n========================================\n";
    std::cout << " Benchmark complete!\n";
    std::cout << "========================================\n";

    return 0;
}
