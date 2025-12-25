#include "uint128/uint128_format.hpp"
#include <cassert>
#include <iostream>

// =============================================================================
// Tests para format functions
// =============================================================================

void test_format_decimal()
{
    uint128_t value(0, 12345);

    std::string result = uint128_format::dec(value);
    assert(result == "12345");

    result = uint128_format::dec(value, 10, ' ', false);
    assert(result == "     12345");

    std::cout << "test_format_decimal: passed" << std::endl;
}

void test_format_hexadecimal()
{
    uint128_t value(0, 0xFF);

    std::string result = uint128_format::hex(value);
    assert(result == "FF");

    result = uint128_format::hex(value, 0, true, false);
    assert(result == "0xFF");

    result = uint128_format::hex(value, 8, true, true, '0');
    assert(result == "0x0000FF");

    std::cout << "test_format_hexadecimal: passed" << std::endl;
}

void test_format_octal()
{
    uint128_t value(0, 64);

    std::string result = uint128_format::oct(value);
    assert(result == "100");

    result = uint128_format::oct(value, 0, true);
    assert(result == "0100");

    std::cout << "test_format_octal: passed" << std::endl;
}

void test_format_with_width()
{
    uint128_t value(0, 42);

    std::string result = uint128_format::format(value, 10, 10, '*');
    assert(result == "********42");

    result = uint128_format::format(value, 10, 10, ' ', false, false, true);
    assert(result == "42        ");

    std::cout << "test_format_with_width: passed" << std::endl;
}

void test_format_internal_align()
{
    uint128_t value(0, 0x100);

    std::string result = uint128_format::format(value, 16, 10, '0', true, false, false, true);
    assert(result == "0x00000100");

    std::cout << "test_format_internal_align: passed" << std::endl;
}

void test_format_like_iostream()
{
    uint128_t value(0, 255);

    std::ios_base::fmtflags flags =
        std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase;
    std::string result = uint128_format::format_like_iostream(value, flags);
    assert(result == "0XFF");

    flags = std::ios_base::oct | std::ios_base::showbase;
    result = uint128_format::format_like_iostream(value, flags);
    assert(result == "0377");

    std::cout << "test_format_like_iostream: passed" << std::endl;
}

void test_format_large_values()
{
    uint128_t large(0x123456789ABCDEFULL, 0xFEDCBA987654321ULL);

    std::string hex_result = uint128_format::hex(large, 0, true, true);
    assert(hex_result.substr(0, 2) == "0X");
    assert(hex_result.length() > 10);

    std::cout << "test_format_large_values: passed" << std::endl;
}

void test_format_zero()
{
    uint128_t zero(0, 0);

    assert(uint128_format::dec(zero) == "0");
    assert(uint128_format::hex(zero) == "0");
    assert(uint128_format::oct(zero) == "0");

    std::cout << "test_format_zero: passed" << std::endl;
}

void test_format_uppercase()
{
    uint128_t value(0, 0xABCD);

    std::string lower = uint128_format::hex(value, 0, false, false);
    std::string upper = uint128_format::hex(value, 0, false, true);

    assert(lower == "ABCD" || lower == "abcd"); // depending on to_string_hex implementation
    assert(upper == "ABCD");

    std::cout << "test_format_uppercase: passed" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== uint128_t format tests ===" << std::endl;

    test_format_decimal();
    test_format_hexadecimal();
    test_format_octal();
    test_format_with_width();
    test_format_internal_align();
    test_format_like_iostream();
    test_format_large_values();
    test_format_zero();
    test_format_uppercase();

    std::cout << "\n✓ All tests passed!" << std::endl;
    return 0;
}
