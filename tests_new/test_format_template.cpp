/**
 * @file test_format_template.cpp
 * @brief Test de int128_base_format.hpp
 */

#include "int128_base_format.hpp"
#include <iostream>
#include <sstream>

using namespace nstd;

int main()
{
    std::cout << "=== Test format para int128_base_t<S> ===\n\n";

    int passed = 0;
    int failed = 0;

    // Test 1: format decimal basico uint128
    {
        auto result = int128_format::format(uint128_t(12345));
        bool ok = (result == "12345");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] format uint128 decimal: '" << result
                  << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 2: format decimal basico int128
    {
        auto result = int128_format::format(int128_t(-12345));
        bool ok = (result == "-12345");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] format int128 negativo: '" << result
                  << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 3: hex basico
    {
        auto result = int128_format::hex(uint128_t(255));
        bool ok = (result == "ff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hex(255): '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 4: hex con showbase
    {
        auto result = int128_format::hex(uint128_t(255), 0, true);
        bool ok = (result == "0xff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hex showbase: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 5: hex con uppercase
    {
        auto result = int128_format::hex(uint128_t(255), 0, true, true);
        bool ok = (result == "0XFF");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hex uppercase: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 6: hex con width
    {
        auto result = int128_format::hex(uint128_t(255), 8, true, false);
        bool ok = (result == "0x0000ff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hex width=8: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 7: oct basico
    {
        auto result = int128_format::oct(uint128_t(64));
        bool ok = (result == "100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] oct(64): '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 8: oct con showbase
    {
        auto result = int128_format::oct(uint128_t(64), 0, true);
        bool ok = (result == "0100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] oct showbase: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 9: dec con width right
    {
        auto result = int128_format::dec(uint128_t(42), 10);
        bool ok = (result == "        42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] dec width=10 right: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 10: dec con width left
    {
        auto result = int128_format::dec(uint128_t(42), 10, ' ', true);
        bool ok = (result == "42        ");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] dec width=10 left: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 11: dec_signed positivo
    {
        auto result = int128_format::dec_signed(int128_t(42));
        bool ok = (result == "+42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] dec_signed positivo: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 12: dec_signed negativo
    {
        auto result = int128_format::dec_signed(int128_t(-42));
        bool ok = (result == "-42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] dec_signed negativo: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 13: internal align con signo
    {
        auto result =
            int128_format::format(int128_t(-42), 10, 8, '0', false, false, false, false, true);
        bool ok = (result == "-0000042");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] internal con signo: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 14: internal align con 0x
    {
        auto result =
            int128_format::format(uint128_t(255), 16, 10, '0', true, false, false, false, true);
        bool ok = (result == "0x000000ff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] internal con 0x: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 15: bin basico
    {
        auto result = int128_format::bin(uint128_t(10));
        bool ok = (result == "1010");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] bin(10): '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 16: bin con prefijo
    {
        auto result = int128_format::bin(uint128_t(10), 0, true);
        bool ok = (result == "0b1010");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] bin con prefijo: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 17: bin con width
    {
        auto result = int128_format::bin(uint128_t(10), 8);
        bool ok = (result == "00001010");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] bin width=8: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 18: with_separators basico
    {
        auto result = int128_format::with_separators(uint128_t(1234567890));
        bool ok = (result == "1,234,567,890");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] with_separators: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 19: with_separators negativo
    {
        auto result = int128_format::with_separators(int128_t(-1234567));
        bool ok = (result == "-1,234,567");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] with_separators negativo: '" << result
                  << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 20: with_separators custom
    {
        auto result = int128_format::with_separators(uint128_t(1234567), '.', 3);
        bool ok = (result == "1.234.567");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] with_separators custom: '" << result
                  << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 21: format_like_iostream con flags
    {
        std::ios_base::fmtflags flags =
            std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase;
        auto result = int128_format::format_like_iostream(uint128_t(255), flags, 0, ' ');
        bool ok = (result == "0XFF");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] format_like_iostream: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 22: valor cero
    {
        auto result = int128_format::format(uint128_t(0));
        bool ok = (result == "0");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] cero: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 23: valor grande hex
    {
        uint128_t big(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
        auto result = int128_format::hex(big);
        bool ok = (result == "1234567890abcdeffedcba0987654321");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] valor grande hex: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 24: format_for_stream helper
    {
        std::ostringstream oss;
        oss << std::hex << std::showbase << std::uppercase;
        auto result = format_for_stream(uint128_t(255), oss);
        bool ok = (result == "0XFF");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] format_for_stream: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 25: dec_signed para uint128_t (showpos sin efecto en unsigned negativo imposible)
    {
        auto result = int128_format::dec_signed(uint128_t(100));
        bool ok = (result == "+100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] dec_signed uint128: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
