#include "uint128/uint128_iostreams.hpp"
// uint128_t.hpp is included by uint128_iostreams.hpp
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace nstd;


void test_output_decimal()
{
    std::ostringstream oss;
    uint128_t value(0, 12345);

    oss << std::dec << value;
    assert(oss.str() == "12345");

    std::cout << "test_output_decimal: passed" << std::endl;
}

void test_output_hexadecimal()
{
    std::ostringstream oss;
    uint128_t value(0, 0xFF);

    oss << std::hex << value;
    assert(oss.str() == "ff");

    oss.str("");
    oss << std::hex << std::uppercase << value;
    assert(oss.str() == "FF");

    std::cout << "test_output_hexadecimal: passed" << std::endl;
}

void test_output_octal()
{
    std::ostringstream oss;
    uint128_t value(0, 64);

    oss << std::oct << value;
    assert(oss.str() == "100");

    std::cout << "test_output_octal: passed" << std::endl;
}

void test_showbase()
{
    std::ostringstream oss;
    uint128_t value(0, 255);

    // Hex with showbase
    oss << std::hex << std::showbase << value;
    assert(oss.str() == "0xff");

    // Oct with showbase
    oss.str("");
    oss << std::oct << std::showbase << value;
    assert(oss.str() == "0377");

    std::cout << "test_showbase: passed" << std::endl;
}

void test_width_and_fill()
{
    std::ostringstream oss;
    uint128_t value(0, 42);

    oss << std::dec << std::setw(5) << std::setfill('0') << value;
    assert(oss.str() == "00042");

    oss.str("");
    oss << std::setw(5) << std::setfill('*') << value;
    assert(oss.str() == "***42");

    std::cout << "test_width_and_fill: passed" << std::endl;
}

void test_alignment()
{
    std::ostringstream oss;
    uint128_t value(0, 42);

    // Left alignment
    oss << std::dec << std::setw(5) << std::left << value;
    assert(oss.str() == "42   ");

    // Right alignment
    oss.str("");
    oss << std::setw(5) << std::right << value;
    assert(oss.str() == "   42");

    std::cout << "test_alignment: passed" << std::endl;
}

void test_input_decimal()
{
    std::istringstream iss("12345");
    uint128_t value;

    iss >> std::dec >> value;
    assert(value == uint128_t(0, 12345));

    std::cout << "test_input_decimal: passed" << std::endl;
}

void test_input_hexadecimal()
{
    std::istringstream iss("ff");
    uint128_t value;

    iss >> std::hex >> value;
    assert(value == uint128_t(0, 0xFF));

    std::cout << "test_input_hexadecimal: passed" << std::endl;
}

void test_input_octal()
{
    std::istringstream iss("100");
    uint128_t value;

    iss >> std::oct >> value;
    assert(value == uint128_t(0, 64));

    std::cout << "test_input_octal: passed" << std::endl;
}

void test_large_value()
{
    std::ostringstream oss;
    uint128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    oss << std::hex << value;
    std::string result = oss.str();
    assert(!result.empty());
    assert(result.find("123456789abcdef0") != std::string::npos ||
           result == "123456789abcdef0fedcba9876543210");

    std::cout << "test_large_value: passed" << std::endl;
}

void test_roundtrip()
{
    uint128_t original(0x1234, 0x5678);
    std::ostringstream oss;
    oss << std::dec << original;

    std::istringstream iss(oss.str());
    uint128_t result;
    iss >> std::dec >> result;

    assert(result == original);

    std::cout << "test_roundtrip: passed" << std::endl;
}

int main()
{
    std::cout << "Running uint128_t iostreams tests..." << std::endl;

    test_output_decimal();
    test_output_hexadecimal();
    test_output_octal();
    test_showbase();
    test_width_and_fill();
    test_alignment();
    test_input_decimal();
    test_input_hexadecimal();
    test_input_octal();
    test_large_value();
    test_roundtrip();

    std::cout << "\n[OK] All uint128_t iostreams tests passed!" << std::endl;
    return 0;
}
