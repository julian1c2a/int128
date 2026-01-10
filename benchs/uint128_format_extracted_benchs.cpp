#include "int128_base_format.hpp"
#include <chrono>
#include <iostream>

using namespace nstd;
using namespace std::chrono;

constexpr int ITERATIONS = 50000;

void benchmark_format_decimal()
{
    uint128_t value(0, 123456789);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = uint128_format::dec(value);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format decimal: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_format_hexadecimal()
{
    uint128_t value(0, 0xABCDEF123456ULL);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = uint128_format::hex(value, 0, true, true);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format hexadecimal: " << duration << " µs (" << ITERATIONS << " ops)"
              << std::endl;
}

void benchmark_format_with_width()
{
    uint128_t value(0, 42);
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = uint128_format::format(value, 10, 20, '*', false, false, false, true);
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format with width: " << duration << " µs (" << ITERATIONS << " ops)" << std::endl;
}

void benchmark_format_like_iostream()
{
    uint128_t value(0, 0xFF00FF);
    std::ios_base::fmtflags flags =
        std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase;
    std::string result;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        result = uint128_format::format_like_iostream(value, flags, 16, '0');
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Format like iostream: " << duration << " µs (" << ITERATIONS << " ops)"
              << std::endl;
}

int main()
{
    std::cout << "uint128_t format benchmarks" << std::endl;
    std::cout << "===========================" << std::endl;

    benchmark_format_decimal();
    benchmark_format_hexadecimal();
    benchmark_format_with_width();
    benchmark_format_like_iostream();

    std::cout << "\n[OK] Benchmarks completed" << std::endl;
    return 0;
}
