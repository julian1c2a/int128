#include "int128/int128_format.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;

constexpr int ITERATIONS = 50000;

void benchmark_format_decimal()
{
    int128_t value(123456789);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = int128_format::dec(value);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format decimal: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_format_negative()
{
    int128_t value(-123456789);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = int128_format::dec(value);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format negative: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_format_hexadecimal()
{
    int128_t value(0xABCDEF123456ULL);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = int128_format::hex(value, 0, true, true);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format hexadecimal: " << duration << " µs (" << ITERATIONS << " ops)"
              << std::endl;
}

void benchmark_format_with_width()
{
    int128_t value(42);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = int128_format::format(value, 10, 20, '*', false, false, false, false, true);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format with width: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_format_showpos()
{
    int128_t value(100);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = int128_format::format(value, 10, 0, ' ', false, true, false, false, false);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format showpos: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

int main()
{
    std::cout << "int128_t format benchmarks" << std::endl;
    std::cout << "==========================" << std::endl;

    benchmark_format_decimal();
    benchmark_format_negative();
    benchmark_format_hexadecimal();
    benchmark_format_with_width();
    benchmark_format_showpos();

    std::cout << "\n[OK] Benchmarks completed" << std::endl;
    return 0;
}
