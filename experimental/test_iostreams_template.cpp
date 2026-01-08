/**
 * @file test_iostreams_template.cpp
 * @brief Test de operadores iostream para int128_base_t<S>
 * @date 2026-01-08
 */

#include "../include_new/int128_base_iostreams.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace nstd;

int main()
{
    std::cout << "=== Test iostreams para int128_base_t<S> ===\n\n";

    int passed = 0;
    int failed = 0;

    // =========================================================================
    // TESTS DE SALIDA (operator<<)
    // =========================================================================

    // Test 1: Salida decimal basica uint128
    {
        std::ostringstream oss;
        oss << uint128_t(12345);
        bool ok = (oss.str() == "12345");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 decimal: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 2: Salida decimal basica int128
    {
        std::ostringstream oss;
        oss << int128_t(-12345);
        bool ok = (oss.str() == "-12345");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] int128 decimal negativo: " << oss.str()
                  << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 3: Salida hexadecimal uint128
    {
        std::ostringstream oss;
        oss << std::hex << uint128_t(255);
        bool ok = (oss.str() == "ff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 hex: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 4: Salida hexadecimal con showbase
    {
        std::ostringstream oss;
        oss << std::hex << std::showbase << uint128_t(255);
        bool ok = (oss.str() == "0xff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 hex showbase: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 5: Salida hexadecimal uppercase
    {
        std::ostringstream oss;
        oss << std::hex << std::uppercase << std::showbase << uint128_t(255);
        bool ok = (oss.str() == "0XFF");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 hex uppercase: " << oss.str()
                  << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 6: Salida octal
    {
        std::ostringstream oss;
        oss << std::oct << uint128_t(64);
        bool ok = (oss.str() == "100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 octal: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 7: Salida octal con showbase
    {
        std::ostringstream oss;
        oss << std::oct << std::showbase << uint128_t(64);
        bool ok = (oss.str() == "0100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 octal showbase: " << oss.str()
                  << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 8: Ancho con setw (right align por defecto)
    {
        std::ostringstream oss;
        oss << std::setw(10) << uint128_t(42);
        bool ok = (oss.str() == "        42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] setw(10) right: '" << oss.str() << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 9: Ancho con left align
    {
        std::ostringstream oss;
        oss << std::setw(10) << std::left << uint128_t(42);
        bool ok = (oss.str() == "42        ");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] setw(10) left: '" << oss.str() << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 10: Ancho con setfill
    {
        std::ostringstream oss;
        oss << std::setw(8) << std::setfill('0') << uint128_t(42);
        bool ok = (oss.str() == "00000042");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] setfill('0'): '" << oss.str() << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 11: showpos para signed positivo
    {
        std::ostringstream oss;
        oss << std::showpos << int128_t(42);
        bool ok = (oss.str() == "+42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] int128 showpos: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 12: internal alignment con signo
    {
        std::ostringstream oss;
        oss << std::setw(8) << std::setfill('0') << std::internal << int128_t(-42);
        bool ok = (oss.str() == "-0000042");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] internal con signo: '" << oss.str() << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 13: internal alignment con 0x
    {
        std::ostringstream oss;
        oss << std::hex << std::showbase << std::setw(10) << std::setfill('0') << std::internal
            << uint128_t(255);
        bool ok = (oss.str() == "0x000000ff");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] internal con 0x: '" << oss.str() << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 14: Valor cero
    {
        std::ostringstream oss;
        oss << uint128_t(0);
        bool ok = (oss.str() == "0");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 cero: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 15: Valor grande
    {
        std::ostringstream oss;
        uint128_t big(0x1234567890ABCDEFULL, 0xFEDCBA9876543210ULL);
        oss << std::hex << big;
        bool ok = (oss.str().length() > 16); // Debe ser un string hex largo
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] uint128 grande hex: " << oss.str() << "\n";
        ok ? ++passed : ++failed;
    }

    // =========================================================================
    // TESTS DE ENTRADA (operator>>)
    // =========================================================================

    // Test 16: Entrada decimal uint128
    {
        std::istringstream iss("12345");
        uint128_t val;
        iss >> val;
        bool ok = (val == uint128_t(12345));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] input decimal uint128\n";
        ok ? ++passed : ++failed;
    }

    // Test 17: Entrada decimal int128 negativo
    {
        std::istringstream iss("-12345");
        int128_t val;
        iss >> val;
        bool ok = (val == int128_t(-12345));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] input decimal int128 negativo\n";
        ok ? ++passed : ++failed;
    }

    // Test 18: Entrada hex con auto-deteccion
    {
        std::istringstream iss("0xff");
        uint128_t val;
        iss >> val;
        bool ok = (val == uint128_t(255));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] input hex auto-detect: " << val << "\n";
        ok ? ++passed : ++failed;
    }

    // Test 19: Entrada hex con manipulador
    {
        std::istringstream iss("ff");
        uint128_t val;
        iss >> std::hex >> val;
        bool ok = (val == uint128_t(255));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] input hex manipulador\n";
        ok ? ++passed : ++failed;
    }

    // Test 20: Entrada octal con auto-deteccion
    {
        std::istringstream iss("0100");
        uint128_t val;
        iss >> val;
        bool ok = (val == uint128_t(64));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] input octal auto-detect: " << val << "\n";
        ok ? ++passed : ++failed;
    }

    // =========================================================================
    // TESTS DE FUNCIONES DE FORMATEO
    // =========================================================================

    // Test 21: int128_iostream::hex
    {
        std::string result = int128_iostream::hex(uint128_t(255), 4, true, true, '0');
        bool ok = (result == "0XFF");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] iostream::hex: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 22: int128_iostream::dec
    {
        std::string result = int128_iostream::dec(uint128_t(42), 6, ' ', false);
        bool ok = (result == "    42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] iostream::dec: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 23: int128_iostream::oct
    {
        std::string result = int128_iostream::oct(uint128_t(64), 6, true, '0');
        bool ok = (result == "000100");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] iostream::oct: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 24: int128_iostream::dec_signed
    {
        std::string result = int128_iostream::dec_signed(int128_t(42), 0, ' ', false);
        bool ok = (result == "+42");
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] iostream::dec_signed: '" << result << "'\n";
        ok ? ++passed : ++failed;
    }

    // Test 25: Roundtrip (output -> input)
    {
        uint128_t original(0x123456789ABCDEFULL);
        std::stringstream ss;
        ss << std::hex << original;
        uint128_t recovered;
        ss >> std::hex >> recovered;
        bool ok = (original == recovered);
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] roundtrip hex\n";
        ok ? ++passed : ++failed;
    }

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
