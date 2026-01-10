/**
 * @file int128_iostreams_extracted_tests.cpp
 * @brief Tests completos de int128_base_iostreams.hpp (template unificado)
 *
 * Fusion de tests existentes + tests de test_iostreams_template.cpp
 * Total: 25+ tests cubriendo operadores << >> y funciones de formateo
 */

#include "int128_base_iostreams.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace nstd;

int passed = 0;
int failed = 0;

void check(bool condition, const char *test_name, const std::string &result = "")
{
    if (condition)
    {
        ++passed;
        std::cout << "[OK]   " << test_name;
    }
    else
    {
        ++failed;
        std::cout << "[FAIL] " << test_name;
    }
    if (!result.empty())
    {
        std::cout << " -> '" << result << "'";
    }
    std::cout << "\n";
}

// =============================================================================
// Tests de salida (operator<<)
// =============================================================================

void test_output_decimal_uint128()
{
    std::ostringstream oss;
    oss << uint128_t(12345);
    check(oss.str() == "12345", "uint128 decimal", oss.str());
}

void test_output_decimal_int128()
{
    std::ostringstream oss;
    oss << int128_t(12345);
    check(oss.str() == "12345", "int128 decimal", oss.str());
}

void test_output_decimal_negative()
{
    std::ostringstream oss;
    oss << int128_t(-12345);
    check(oss.str() == "-12345", "int128 decimal negativo", oss.str());
}

void test_output_hex_basic()
{
    std::ostringstream oss;
    oss << std::hex << uint128_t(255);
    check(oss.str() == "ff", "uint128 hex", oss.str());
}

void test_output_hex_showbase()
{
    std::ostringstream oss;
    oss << std::hex << std::showbase << uint128_t(255);
    check(oss.str() == "0xff", "uint128 hex showbase", oss.str());
}

void test_output_hex_uppercase()
{
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::showbase << uint128_t(255);
    check(oss.str() == "0XFF", "uint128 hex uppercase", oss.str());
}

void test_output_octal_basic()
{
    std::ostringstream oss;
    oss << std::oct << uint128_t(64);
    check(oss.str() == "100", "uint128 octal", oss.str());
}

void test_output_octal_showbase()
{
    std::ostringstream oss;
    oss << std::oct << std::showbase << uint128_t(64);
    check(oss.str() == "0100", "uint128 octal showbase", oss.str());
}

void test_output_setw_right()
{
    std::ostringstream oss;
    oss << std::setw(10) << uint128_t(42);
    check(oss.str() == "        42", "setw(10) right", oss.str());
}

void test_output_setw_left()
{
    std::ostringstream oss;
    oss << std::setw(10) << std::left << uint128_t(42);
    check(oss.str() == "42        ", "setw(10) left", oss.str());
}

void test_output_setfill()
{
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << uint128_t(42);
    check(oss.str() == "00000042", "setfill('0')", oss.str());
}

void test_output_showpos()
{
    std::ostringstream oss;
    oss << std::showpos << int128_t(42);
    check(oss.str() == "+42", "int128 showpos", oss.str());
}

void test_output_internal_sign()
{
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << std::internal << int128_t(-42);
    check(oss.str() == "-0000042", "internal con signo", oss.str());
}

void test_output_internal_0x()
{
    std::ostringstream oss;
    oss << std::hex << std::showbase << std::setw(10) << std::setfill('0') << std::internal << uint128_t(255);
    check(oss.str() == "0x000000ff", "internal con 0x", oss.str());
}

void test_output_zero()
{
    std::ostringstream oss;
    oss << uint128_t(0);
    check(oss.str() == "0", "uint128 cero", oss.str());
}

void test_output_large_value()
{
    std::ostringstream oss;
    uint128_t big(0x1234567890ABCDEFULL, 0xFEDCBA9876543210ULL);
    oss << std::hex << big;
    check(oss.str().length() > 16, "uint128 grande hex", oss.str());
}

// =============================================================================
// Tests de entrada (operator>>)
// =============================================================================

void test_input_decimal_uint128()
{
    std::istringstream iss("12345");
    uint128_t val;
    iss >> val;
    check(val == uint128_t(12345), "input decimal uint128");
}

void test_input_decimal_int128_negative()
{
    std::istringstream iss("-12345");
    int128_t val;
    iss >> val;
    check(val == int128_t(-12345), "input decimal int128 negativo");
}

void test_input_hex_autodetect()
{
    std::istringstream iss("0xff");
    uint128_t val;
    iss >> val;
    check(val == uint128_t(255), "input hex auto-detect");
}

void test_input_hex_manipulator()
{
    std::istringstream iss("ff");
    uint128_t val;
    iss >> std::hex >> val;
    check(val == uint128_t(255), "input hex manipulador");
}

void test_input_octal_autodetect()
{
    std::istringstream iss("0100");
    uint128_t val;
    iss >> val;
    check(val == uint128_t(64), "input octal auto-detect");
}

// =============================================================================
// Tests de funciones de formateo
// =============================================================================

void test_iostream_hex()
{
    std::string result = int128_iostream::hex(uint128_t(255), 4, true, true, '0');
    check(result == "0XFF", "iostream::hex", result);
}

void test_iostream_dec()
{
    std::string result = int128_iostream::dec(uint128_t(42), 6, ' ', false);
    check(result == "    42", "iostream::dec", result);
}

void test_iostream_oct()
{
    std::string result = int128_iostream::oct(uint128_t(64), 6, true, '0');
    check(result == "000100", "iostream::oct", result);
}

void test_iostream_dec_signed()
{
    std::string result = int128_iostream::dec_signed(int128_t(42), 0, ' ', false);
    check(result == "+42", "iostream::dec_signed", result);
}

// =============================================================================
// Tests roundtrip
// =============================================================================

void test_roundtrip_hex()
{
    uint128_t original(0x123456789ABCDEFULL);
    std::stringstream ss;
    ss << std::hex << original;
    uint128_t recovered;
    ss >> std::hex >> recovered;
    check(original == recovered, "roundtrip hex");
}

void test_roundtrip_decimal()
{
    int128_t original(-987654321);
    std::stringstream ss;
    ss << original;
    int128_t recovered;
    ss >> recovered;
    check(original == recovered, "roundtrip decimal");
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== int128_base_iostreams.hpp tests ===\n\n";

    // Output tests
    test_output_decimal_uint128();
    test_output_decimal_int128();
    test_output_decimal_negative();
    test_output_hex_basic();
    test_output_hex_showbase();
    test_output_hex_uppercase();
    test_output_octal_basic();
    test_output_octal_showbase();
    test_output_setw_right();
    test_output_setw_left();
    test_output_setfill();
    test_output_showpos();
    test_output_internal_sign();
    test_output_internal_0x();
    test_output_zero();
    test_output_large_value();

    // Input tests
    test_input_decimal_uint128();
    test_input_decimal_int128_negative();
    test_input_hex_autodetect();
    test_input_hex_manipulator();
    test_input_octal_autodetect();

    // Formatting functions
    test_iostream_hex();
    test_iostream_dec();
    test_iostream_oct();
    test_iostream_dec_signed();

    // Roundtrip
    test_roundtrip_hex();
    test_roundtrip_decimal();

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed) << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
