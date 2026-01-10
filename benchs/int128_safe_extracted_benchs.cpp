#include "int128_base_safe.hpp"
#include <chrono>
#include <iostream>

using namespace nstd;
using namespace std::chrono;

constexpr int ITERATIONS = 100000;

void benchmark_safe_cast()
{
    int128_t value(12345678);
    int64_t sum = 0;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto result = int128_safe::safe_cast<signedness::signed_type, int64_t>(value);
        if (result.is_valid())
        {
            sum += result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe cast: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_add()
{
    int128_t a(1000);
    int128_t b(2000);
    int128_t sum(0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto result = int128_safe::safe_add(a, b);
        if (result.is_valid())
        {
            sum = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe add: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_mul()
{
    int128_t a(100);
    int128_t b(200);
    int128_t product(0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto result = int128_safe::safe_mul(a, b);
        if (result.is_valid())
        {
            product = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe mul: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_div()
{
    int128_t a(1000000);
    int128_t b(100);
    int128_t quotient(0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto result = int128_safe::safe_div(a, b);
        if (result.is_valid())
        {
            quotient = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe div: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_abs()
{
    int128_t negative(-12345);
    int128_t result_val(0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto result = int128_safe::safe_abs(negative);
        if (result.is_valid())
        {
            result_val = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe abs: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

int main()
{
    std::cout << "int128_t safe operations benchmarks" << std::endl;
    std::cout << "===================================" << std::endl;

    benchmark_safe_cast();
    benchmark_safe_add();
    benchmark_safe_mul();
    benchmark_safe_div();
    benchmark_safe_abs();

    std::cout << "\n[OK] Benchmarks completed" << std::endl;
    return 0;
}
