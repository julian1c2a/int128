/*
 * Benchmarks unificados para int128_base_concepts.hpp
 * Prueba conceptos C++20 para AMBOS tipos: uint128_t e int128_t
 */

#include "int128_base_concepts.hpp"
#include "int128_base_tt.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace nstd;

// =============================================================================
// UTILITIES
// =============================================================================

#if defined(__x86_64__) || defined(_M_X64)
inline uint64_t RDTSC()
{
#ifdef _MSC_VER
    return __rdtsc();
#else
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#endif
}
#else
inline uint64_t RDTSC()
{
    return 0;
}
#endif

struct BenchResult
{
    uint64_t cycles;
    double elapsed_ms;
    uint64_t iterations;
};

template <typename Func>
BenchResult run_bench(Func &&func, uint64_t iterations)
{
    BenchResult result{};
    result.iterations = iterations;

    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start = RDTSC();

    for (uint64_t i = 0; i < iterations; ++i)
    {
        func();
    }

    uint64_t end = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();

    result.cycles = end - start;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    result.elapsed_ms = duration.count() / 1000.0;

    return result;
}

void print_result(const std::string &name, const BenchResult &result)
{
    std::cout << std::left << std::setw(50) << name;

    if (result.iterations == 0)
    {
        std::cout << "SKIP\n";
        return;
    }

    double cycles_per_iter = static_cast<double>(result.cycles) / result.iterations;
    double ns_per_iter = (result.elapsed_ms * 1'000'000.0) / result.iterations;

    std::cout << std::fixed << std::setprecision(2)
              << std::setw(10) << cycles_per_iter << " cyc/op, "
              << std::setw(10) << ns_per_iter << " ns/op\n";
}

// =============================================================================
// BENCHMARK: Concept Verification (compile-time, runtime overhead ~0)
// =============================================================================
void bench_concept_verification()
{
    std::cout << "\n=== Concept Verification (compile-time) ===\n";
    constexpr uint64_t iterations = 100'000'000;
    volatile bool sink = false;

    // uint128_t concepts
    auto result_u128 = run_bench(
        [&sink]()
        {
            sink = nstd::is_uint128_v<uint128_t>;
            sink = nstd::is_128bit_type_v<uint128_t>;
            sink = nstd::int128_convertible<uint64_t>;
            sink = nstd::int128_bitwise_compatible<uint32_t>;
        },
        iterations);
    print_result("uint128_t concept checks (4 concepts)", result_u128);

    // int128_t concepts
    auto result_i128 = run_bench(
        [&sink]()
        {
            sink = nstd::is_int128_v<int128_t>;
            sink = nstd::is_128bit_type_v<int128_t>;
            sink = nstd::signed_int128_type<int128_t>;
            sink = nstd::int128_signed_compatible<int64_t>;
        },
        iterations);
    print_result("int128_t concept checks (4 concepts)", result_i128);
}

// =============================================================================
// BENCHMARK: Function with constrained template
// =============================================================================
template <nstd::int128_type T>
constexpr T increment_value(T value)
{
    return value + T(1);
}

void bench_constrained_function()
{
    std::cout << "\n=== Constrained Template Function ===\n";
    constexpr uint64_t iterations = 50'000'000;

    // uint128_t
    uint128_t u_val{42};
    uint128_t u_sink{0};
    auto result_u128 = run_bench(
        [&]()
        {
            u_sink = increment_value(u_val);
            u_val = u_sink;
        },
        iterations);
    print_result("increment<uint128_t>", result_u128);

    // int128_t
    int128_t i_val{-42};
    int128_t i_sink{0};
    auto result_i128 = run_bench(
        [&]()
        {
            i_sink = increment_value(i_val);
            i_val = i_sink;
        },
        iterations);
    print_result("increment<int128_t>", result_i128);
}

// =============================================================================
// BENCHMARK: Predicate concept
// =============================================================================
template <nstd::int128_type T>
constexpr bool is_even(T value)
{
    return (value & T(1)) == T(0);
}

void bench_predicate()
{
    std::cout << "\n=== Predicate Evaluation ===\n";
    constexpr uint64_t iterations = 50'000'000;
    volatile bool sink = false;

    // uint128_t
    uint128_t u_val{42};
    auto result_u128 = run_bench(
        [&]()
        {
            sink = is_even(u_val);
            u_val = u_val + uint128_t(1);
        },
        iterations);
    print_result("is_even<uint128_t>", result_u128);

    // int128_t
    int128_t i_val{42};
    auto result_i128 = run_bench(
        [&]()
        {
            sink = is_even(i_val);
            i_val = i_val + int128_t(1);
        },
        iterations);
    print_result("is_even<int128_t>", result_i128);
}

// =============================================================================
// BENCHMARK: Binary operation concept
// =============================================================================
template <nstd::int128_type T>
constexpr T add_values(T a, T b)
{
    return a + b;
}

void bench_binary_operation()
{
    std::cout << "\n=== Binary Operation ===\n";
    constexpr uint64_t iterations = 50'000'000;

    // uint128_t
    uint128_t u_a{100}, u_b{200}, u_sink{0};
    auto result_u128 = run_bench(
        [&]()
        {
            u_sink = add_values(u_a, u_b);
            u_a = u_sink + uint128_t(1);
        },
        iterations);
    volatile auto dummy_u = u_sink.low();
    (void)dummy_u;
    print_result("add<uint128_t>", result_u128);

    // int128_t
    int128_t i_a{100}, i_b{-200}, i_sink{0};
    auto result_i128 = run_bench(
        [&]()
        {
            i_sink = add_values(i_a, i_b);
            i_a = i_sink + int128_t(1);
        },
        iterations);
    volatile auto dummy_i = i_sink.low();
    (void)dummy_i;
    print_result("add<int128_t>", result_i128);
}

// =============================================================================
// BENCHMARK: Container operations with concept-constrained type
// =============================================================================
template <nstd::int128_type T>
void fill_container(std::vector<T> &vec, int count)
{
    vec.clear();
    for (int i = 0; i < count; ++i)
    {
        vec.push_back(T(i));
    }
}

void bench_container_operations()
{
    std::cout << "\n=== Container Operations (100 elements) ===\n";
    constexpr uint64_t iterations = 1'000'000;

    // uint128_t
    std::vector<uint128_t> u_vec;
    u_vec.reserve(100);
    auto result_u128 = run_bench(
        [&]()
        { fill_container(u_vec, 100); },
        iterations);
    print_result("vector<uint128_t> fill", result_u128);

    // int128_t
    std::vector<int128_t> i_vec;
    i_vec.reserve(100);
    auto result_i128 = run_bench(
        [&]()
        { fill_container(i_vec, 100); },
        iterations);
    print_result("vector<int128_t> fill", result_i128);
}

// =============================================================================
// BENCHMARK: std::find_if with constrained predicate
// =============================================================================
void bench_find_if()
{
    std::cout << "\n=== std::find_if (1000 elements) ===\n";
    constexpr uint64_t iterations = 500'000;

    // uint128_t - target at position 500
    std::vector<uint128_t> u_vec;
    for (int i = 0; i < 1000; ++i)
    {
        u_vec.push_back(uint128_t(i));
    }
    auto result_u128 = run_bench(
        [&]()
        {
            auto it = std::find_if(u_vec.begin(), u_vec.end(),
                                   [](uint128_t u)
                                   { return u == uint128_t(500); });
            volatile bool found = (it != u_vec.end());
            (void)found;
        },
        iterations);
    print_result("find_if<uint128_t>", result_u128);

    // int128_t - target at position 500 (negative values mixed)
    std::vector<int128_t> i_vec;
    for (int i = 0; i < 1000; ++i)
    {
        i_vec.push_back(int128_t(i - 500)); // -500 to 499
    }
    auto result_i128 = run_bench(
        [&]()
        {
            auto it = std::find_if(i_vec.begin(), i_vec.end(),
                                   [](int128_t i)
                                   { return i == int128_t(0); });
            volatile bool found = (it != i_vec.end());
            (void)found;
        },
        iterations);
    print_result("find_if<int128_t>", result_i128);
}

// =============================================================================
// BENCHMARK: std::transform with concept-constrained function
// =============================================================================
template <nstd::int128_type T>
T double_value(T val)
{
    return val * T(2);
}

void bench_transform()
{
    std::cout << "\n=== std::transform (1000 elements) ===\n";
    constexpr uint64_t iterations = 100'000;

    // uint128_t
    std::vector<uint128_t> u_in, u_out;
    for (int i = 0; i < 1000; ++i)
    {
        u_in.push_back(uint128_t(i));
    }
    u_out.resize(1000);
    auto result_u128 = run_bench(
        [&]()
        {
            std::transform(u_in.begin(), u_in.end(), u_out.begin(), double_value<uint128_t>);
        },
        iterations);
    print_result("transform<uint128_t> (double)", result_u128);

    // int128_t
    std::vector<int128_t> i_in, i_out;
    for (int i = 0; i < 1000; ++i)
    {
        i_in.push_back(int128_t(i - 500));
    }
    i_out.resize(1000);
    auto result_i128 = run_bench(
        [&]()
        {
            std::transform(i_in.begin(), i_in.end(), i_out.begin(), double_value<int128_t>);
        },
        iterations);
    print_result("transform<int128_t> (double)", result_i128);
}

// =============================================================================
// BENCHMARK: Mixed type operations (int128_compatible)
// =============================================================================
void bench_mixed_types()
{
    std::cout << "\n=== Mixed Type Operations ===\n";
    constexpr uint64_t iterations = 50'000'000;

    // uint128_t + builtin types
    uint128_t u128{1000};
    uint64_t u64 = 500;
    uint32_t u32 = 250;
    uint128_t u_sink{0};
    auto result_u128 = run_bench(
        [&]()
        {
            u_sink = u128 + u64;
            u_sink = u_sink + u32;
            u128 = u_sink + uint128_t(1);
        },
        iterations);
    volatile auto dummy_u = u_sink.low();
    (void)dummy_u;
    print_result("uint128_t + uint64_t + uint32_t", result_u128);

    // int128_t + builtin signed types
    int128_t i128{1000};
    int64_t i64 = -500;
    int32_t i32 = 250;
    int128_t i_sink{0};
    auto result_i128 = run_bench(
        [&]()
        {
            i_sink = i128 + i64;
            i_sink = i_sink + i32;
            i128 = i_sink + int128_t(1);
        },
        iterations);
    volatile auto dummy_i = i_sink.low();
    (void)dummy_i;
    print_result("int128_t + int64_t + int32_t", result_i128);
}

// =============================================================================
// BENCHMARK: Bitwise operations (int128_bitwise_compatible)
// =============================================================================
void bench_bitwise()
{
    std::cout << "\n=== Bitwise Operations ===\n";
    constexpr uint64_t iterations = 50'000'000;

    // uint128_t
    uint128_t u_val{0xFFFFFFFFFFFFFFFFull};
    uint64_t mask = 0xFF00FF00FF00FF00ull;
    uint128_t u_sink{0};
    auto result_u128 = run_bench(
        [&]()
        {
            u_sink = u_val & mask;
            u_sink = u_sink | uint128_t(0x0F);
            u_sink = u_sink ^ uint128_t(0xF0);
            u_val = u_sink + uint128_t(1);
        },
        iterations);
    volatile auto dummy_u = u_sink.low();
    (void)dummy_u;
    print_result("uint128_t (AND, OR, XOR)", result_u128);

    // int128_t
    int128_t i_val{0x7FFFFFFFFFFFFFFFll};
    int64_t i_mask = 0x7F007F007F007F00ll;
    int128_t i_sink{0};
    auto result_i128 = run_bench(
        [&]()
        {
            i_sink = i_val & i_mask;
            i_sink = i_sink | int128_t(0x0F);
            i_sink = i_sink ^ int128_t(0xF0);
            i_val = i_sink + int128_t(1);
        },
        iterations);
    volatile auto dummy_i = i_sink.low();
    (void)dummy_i;
    print_result("int128_t (AND, OR, XOR)", result_i128);
}

// =============================================================================
// BENCHMARK: Shift operations (valid_shift_type)
// =============================================================================
void bench_shift()
{
    std::cout << "\n=== Shift Operations ===\n";
    constexpr uint64_t iterations = 50'000'000;

    // uint128_t
    uint128_t u_val = uint128_t(1) << 64;
    int shift = 8;
    uint128_t u_sink{0};
    auto result_u128 = run_bench(
        [&]()
        {
            u_sink = u_val << shift;
            u_sink = u_sink >> shift;
            u_val = u_sink + uint128_t(1);
        },
        iterations);
    volatile auto dummy_u = u_sink.low();
    (void)dummy_u;
    print_result("uint128_t (<<, >>)", result_u128);

    // int128_t (arithmetic shift for signed)
    int128_t i_val = int128_t(1) << 64;
    int128_t i_sink{0};
    auto result_i128 = run_bench(
        [&]()
        {
            i_sink = i_val << shift;
            i_sink = i_sink >> shift; // Arithmetic shift
            i_val = i_sink + int128_t(1);
        },
        iterations);
    volatile auto dummy_i = i_sink.low();
    (void)dummy_i;
    print_result("int128_t (<<, >> arithmetic)", result_i128);
}

// =============================================================================
// BENCHMARK: std::accumulate
// =============================================================================
void bench_accumulate()
{
    std::cout << "\n=== std::accumulate (1000 elements) ===\n";
    constexpr uint64_t iterations = 100'000;

    // uint128_t
    std::vector<uint128_t> u_vec;
    for (int i = 0; i < 1000; ++i)
    {
        u_vec.push_back(uint128_t(i));
    }
    auto result_u128 = run_bench(
        [&]()
        {
            uint128_t sum = std::accumulate(u_vec.begin(), u_vec.end(), uint128_t(0));
            volatile auto dummy = sum.low();
            (void)dummy;
        },
        iterations);
    print_result("accumulate<uint128_t>", result_u128);

    // int128_t
    std::vector<int128_t> i_vec;
    for (int i = 0; i < 1000; ++i)
    {
        i_vec.push_back(int128_t(i - 500)); // Mix positive and negative
    }
    auto result_i128 = run_bench(
        [&]()
        {
            int128_t sum = std::accumulate(i_vec.begin(), i_vec.end(), int128_t(0));
            volatile auto dummy = sum.low();
            (void)dummy;
        },
        iterations);
    print_result("accumulate<int128_t>", result_i128);
}

// =============================================================================
// VERIFICATION: Concepts return correct values
// =============================================================================
void verify_concepts()
{
    std::cout << "\n=== Concept Verification ===\n";

    // uint128_t concepts
    std::cout << "is_uint128_v<uint128_t>: "
              << (nstd::is_uint128_v<uint128_t> ? "true" : "false") << "\n";
    std::cout << "is_128bit_type_v<uint128_t>: "
              << (nstd::is_128bit_type_v<uint128_t> ? "true" : "false") << "\n";
    std::cout << "uint128_type<uint128_t>: "
              << (nstd::uint128_type<uint128_t> ? "true" : "false") << "\n";

    // int128_t concepts
    std::cout << "is_int128_v<int128_t>: "
              << (nstd::is_int128_v<int128_t> ? "true" : "false") << "\n";
    std::cout << "is_128bit_type_v<int128_t>: "
              << (nstd::is_128bit_type_v<int128_t> ? "true" : "false") << "\n";
    std::cout << "signed_int128_type<int128_t>: "
              << (nstd::signed_int128_type<int128_t> ? "true" : "false") << "\n";

    // Cross-type
    std::cout << "int128_convertible<uint64_t>: "
              << (nstd::int128_convertible<uint64_t> ? "true" : "false") << "\n";
    std::cout << "int128_compatible<double>: "
              << (nstd::int128_compatible<double> ? "true" : "false") << "\n";
    std::cout << "int128_type<uint128_t>: "
              << (nstd::int128_type<uint128_t> ? "true" : "false") << "\n";
    std::cout << "int128_type<int128_t>: "
              << (nstd::int128_type<int128_t> ? "true" : "false") << "\n";
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  INT128 CONCEPTS - UNIFIED BENCHMARKS\n";
    std::cout << "  (uint128_t & int128_t)\n";
    std::cout << "========================================\n";

    try
    {
        verify_concepts();
        bench_concept_verification();
        bench_constrained_function();
        bench_predicate();
        bench_binary_operation();
        bench_container_operations();
        bench_find_if();
        bench_transform();
        bench_mixed_types();
        bench_bitwise();
        bench_shift();
        bench_accumulate();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n[FAIL] Exception: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << "  [OK] BENCHMARKS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}
