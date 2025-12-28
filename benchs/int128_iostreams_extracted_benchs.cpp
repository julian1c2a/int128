#include "int128/int128_iostreams.hpp"
#include "int128/int128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace nstd;
using namespace std::chrono;

void benchmark_output_decimal()
{
    int128_t value(0x1234, 0x5678ABCD);
    std::ostringstream oss;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        oss.str("");
        oss << std::dec << value;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Output decimal: " << duration << " µs (100k ops)" << std::endl;
}

void benchmark_output_negative()
{
    int128_t value = -int128_t(0x1234, 0x5678ABCD);
    std::ostringstream oss;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        oss.str("");
        oss << std::dec << value;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Output negative: " << duration << " µs (100k ops)" << std::endl;
}

void benchmark_output_hexadecimal()
{
    int128_t value(0x1234, 0x5678ABCD);
    std::ostringstream oss;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        oss.str("");
        oss << std::hex << value;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Output hexadecimal: " << duration << " µs (100k ops)" << std::endl;
}

void benchmark_input_decimal()
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        std::istringstream iss("123456789012345");
        int128_t value;
        iss >> std::dec >> value;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Input decimal: " << duration << " µs (100k ops)" << std::endl;
}

void benchmark_formatted_output()
{
    int128_t value(0x1234, 0x5678ABCD);
    std::ostringstream oss;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 50000; ++i) {
        oss.str("");
        oss << std::hex << std::uppercase << std::showbase << std::setw(40) << std::setfill('0')
            << value;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << "Formatted output: " << duration << " µs (50k ops)" << std::endl;
}

int main()
{
    std::cout << "int128_t iostreams benchmarks" << std::endl;
    std::cout << "=============================" << std::endl;

    benchmark_output_decimal();
    benchmark_output_negative();
    benchmark_output_hexadecimal();
    benchmark_input_decimal();
    benchmark_formatted_output();

    std::cout << "\n[OK] Benchmarks completed" << std::endl;
    return 0;
}
