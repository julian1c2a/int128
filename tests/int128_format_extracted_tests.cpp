#include "int128_base_format.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

// =============================================================================
// Tests para format functions
// =============================================================================

void test_format_decimal()
{
    int128_t value(12345);

    std::string result = int128_format::dec(value);
    assert(result == "12345");

    result = int128_format::dec(value, 10, ' ', false);
    assert(result == "     12345");

    std::cout << "test_format_decimal: passed" << std::endl;
}

void test_format_negative()
{
    int128_t value(-12345);

    std::string result = int128_format::dec(value);
    assert(result == "-12345");

    result = int128_format::dec(value, 10, ' ', false);
    assert(result == "    -12345");

    std::cout << "test_format_negative: passed" << std::endl;
}

void test_format_hexadecimal()
{
    int128_t value(0xFF);

    std::string result = int128_format::hex(value);
    assert(result == "FF" || result == "ff");

    result = int128_format::hex(value, 0, true, false);
    assert(result == "0xff" || result == "0xFF");

    std::cout << "test_format_hexadecimal: passed" << std::endl;
}

void test_format_octal()
{
    int128_t value(64);

    std::string result = int128_format::oct(value);
    assert(result == "100");

    result = int128_format::oct(value, 0, true);
    assert(result == "0100");

    std::cout << "test_format_octal: passed" << std::endl;
}

void test_format_with_width()
{
    int128_t value(42);

    std::string result = int128_format::format(value, 10, 10, '*');
    assert(result == "********42");

    result = int128_format::format(value, 10, 10, ' ', false, false, false, true);
    assert(result == "42        ");

    std::cout << "test_format_with_width: passed" << std::endl;
}

void test_format_showpos()
{
    int128_t value(100);

    std::string result = int128_format::format(value, 10, 0, ' ', false, true, false, false, false);
    assert(result == "+100");

    std::cout << "test_format_showpos: passed" << std::endl;
}

void test_format_internal_align()
{
    int128_t value(0x100);

    std::string result = int128_format::format(value, 16, 10, '0', true, false, false, false, true);
    // Debería tener formato "0x00000100" o similar
    assert(result.length() == 10);
    assert(result.find("0x") != std::string::npos || result.find("0X") != std::string::npos);

    std::cout << "test_format_internal_align: passed" << std::endl;
}

void test_format_zero()
{
    int128_t zero(0);

    assert(int128_format::dec(zero) == "0");
    assert(int128_format::hex(zero) == "0" || int128_format::hex(zero) == "0");
    assert(int128_format::oct(zero) == "0");

    std::cout << "test_format_zero: passed" << std::endl;
}

void test_format_large_values()
{
    int128_t large(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);

    std::string dec_result = int128_format::dec(large);
    assert(dec_result.length() > 10);

    std::cout << "test_format_large_values: passed" << std::endl;
}

void test_format_negative_hex()
{
    int128_t negative(-100);

    // Para negativos en hex, debería mostrar representación two's complement
    std::string hex_result = int128_format::hex(negative);
    assert(hex_result.length() > 1);

    std::cout << "test_format_negative_hex: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== int128_t format tests ===" << std::endl;

    test_format_decimal();
    test_format_negative();
    test_format_hexadecimal();
    test_format_octal();
    test_format_with_width();
    test_format_showpos();
    test_format_internal_align();
    test_format_zero();
    test_format_large_values();
    test_format_negative_hex();

    std::cout << "\n[OK] All tests passed!" << std::endl;
    return 0;
}
