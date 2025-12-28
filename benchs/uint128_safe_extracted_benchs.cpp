#include "uint128/uint128_safe.hpp"
#include <chrono>
#include <iostream>

using namespace nstd;
using namespace std::chrono;

constexpr int ITERATIONS = 100000;

void benchmark_safe_cast()
{
    uint128_t value(0, 0xFFFFFFFF);
    uint64_t sum = 0;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto result = uint128_safe::safe_cast<uint64_t>(value);
        if (result.is_valid()) {
            sum += result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe cast: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_add()
{
    uint128_t a(0, 1000);
    uint128_t b(0, 2000);
    uint128_t sum(0, 0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto result = uint128_safe::safe_add(a, b);
        if (result.is_valid()) {
            sum = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe add: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_mul()
{
    uint128_t a(0, 100);
    uint128_t b(0, 200);
    uint128_t product(0, 0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto result = uint128_safe::safe_mul(a, b);
        if (result.is_valid()) {
            product = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe mul: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_safe_div()
{
    uint128_t a(0, 1000000);
    uint128_t b(0, 100);
    uint128_t quotient(0, 0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto result = uint128_safe::safe_div(a, b);
        if (result.is_valid()) {
            quotient = result.value;
        }
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Safe div: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

int main()
{
    std::cout << "uint128_t safe operations benchmarks" << std::endl;
    std::cout << "====================================" << std::endl;

    benchmark_safe_cast();
    benchmark_safe_add();
    benchmark_safe_mul();
    benchmark_safe_div();

    std::cout << "\n[OK] Benchmarks completed" << std::endl;
    return 0;
}
