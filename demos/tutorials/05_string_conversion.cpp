/**
 * Tutorial 05: Conversión de Strings
 *
 * Aprenderás:
 * - Conversión de uint128_t/int128_t a string
 * - Parsing desde string (decimal, hex, oct, bin)
 * - Manejo de errores
 * - Formateo personalizado
 */

#include <int128_simple.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <optional>
#include <sstream>

using namespace nstd;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 05: Conversión de Strings\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. UINT128 A STRING (DECIMAL)
    // ============================================================
    std::cout << "--- 1. uint128_t a string (decimal) ---\n";

    uint128_t num{123456789};
    std::ostringstream oss;
    oss << num;
    std::string str = oss.str();

    std::cout << "Número: " << num << "\n";
    std::cout << "String: \"" << str << "\"\n\n";

    // ============================================================
    // 2. STRING A UINT128 (DECIMAL)
    // ============================================================
    std::cout << "--- 2. String a uint128_t ---\n";

    std::string input = "987654321";
    std::istringstream iss(input);
    uint128_t parsed;
    iss >> parsed;

    std::cout << "String: \"" << input << "\"\n";
    std::cout << "Parseado: " << parsed << "\n\n";

    // ============================================================
    // 3. HEXADECIMAL
    // ============================================================
    std::cout << "--- 3. Hexadecimal ---\n";

    uint128_t hex_num = 0xDEADBEEF;

    std::cout << std::hex << std::uppercase << std::showbase;
    std::cout << "Hexadecimal: " << hex_num << "\n";

    // Parse hex
    std::string hex_str = "0xABCDEF123456";
    std::istringstream hex_iss(hex_str);
    uint128_t hex_parsed;
    hex_iss >> std::hex >> hex_parsed;

    std::cout << "Parseado de \"" << hex_str << "\": " << hex_parsed << "\n";
    std::cout << std::dec << std::nouppercase << std::noshowbase << "\n";

    // ============================================================
    // 4. OCTAL
    // ============================================================
    std::cout << "--- 4. Octal ---\n";

    uint128_t oct_num{0755};

    std::cout << std::oct << std::showbase;
    std::cout << "Octal: " << oct_num << "\n";
    std::cout << std::dec << std::noshowbase << "\n";

    // ============================================================
    // 5. INT128 CON SIGNO
    // ============================================================
    std::cout << "--- 5. int128_t (con signo) ---\n";

    int128_t positive{12345};
    int128_t negative{-67890};

    std::cout << "Positivo: " << positive << "\n";
    std::cout << "Negativo: " << negative << "\n\n";

    // ============================================================
    // 6. NÚMEROS MUY GRANDES
    // ============================================================
    std::cout << "--- 6. Números muy grandes ---\n";

    // Factorial de 30
    uint128_t factorial{1};
    for (int i = 2; i <= 30; ++i) {
        factorial *= i;
    }

    std::cout << "30! = " << factorial << "\n\n";

    // ============================================================
    // 7. FORMATO CON ANCHO Y RELLENO
    // ============================================================
    std::cout << "--- 7. Formato personalizado ---\n";

    uint128_t values[] = {1, 12, 123, 1234, 12345};

    std::cout << "Con relleno de ceros:\n";
    for (const auto& v : values) {
        std::cout << std::setfill('0') << std::setw(10) << v << "\n";
    }
    std::cout << std::setfill(' ') << "\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 05 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 06_iostream.cpp\n";

    return 0;
}

