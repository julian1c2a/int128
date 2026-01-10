/**
 * Tutorial 07: Formateo Avanzado
 *
 * Aprenderás:
 * - Conversiones a hex, oct, bin
 * - Formateo con width y padding
 * - User-defined literals (UDL)
 */

#include <int128_simple.hpp>
#include <iomanip>
#include <iostream>

using namespace nstd;

// Usar los literales definidos por el usuario
using namespace nstd::int128_literals;
using namespace nstd::int128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 07: Formateo Avanzado\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. USER-DEFINED LITERALS (UDL)
    // ============================================================
    std::cout << "--- 1. User-Defined Literals ---\n";

    auto a = 1000_u128; // uint128_t
    auto b = -500_i128; // int128_t

    std::cout << "1000_u128 = " << a << "\n";
    std::cout << "-500_i128 = " << b << "\n\n";

    // ============================================================
    // 2. HEXADECIMAL
    // ============================================================
    std::cout << "--- 2. Hexadecimal ---\n";

    auto hex = 0xDEADBEEF_u128;

    std::cout << std::hex << std::showbase << std::uppercase;
    std::cout << "0xDEADBEEF_u128 = " << hex << "\n";
    std::cout << std::dec << std::noshowbase << std::nouppercase << "\n";

    // ============================================================
    // 3. FORMATO CONWIDTH Y PADDING
    // ============================================================
    std::cout << "--- 3. Width y padding ---\n";

    auto values = {10_u128, 1000_u128, 100000_u128};

    for (const auto& v : values) {
        std::cout << std::setw(10) << std::setfill('0') << v << "\n";
    }
    std::cout << std::setfill(' ') << "\n";

    // ============================================================
    // 4. MÚLTIPLES BASES
    // ============================================================
    std::cout << "--- 4. Múltiples bases ---\n";

    auto num = 255_u128;

    std::cout << "Decimal: " << std::dec << num << "\n";
    std::cout << "Hex:     " << std::hex << std::showbase << num << "\n";
    std::cout << "Oct:     " << std::oct << num << "\n";
    std::cout << std::dec << std::noshowbase << "\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 07 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 08_cmath_functions.cpp\n";

    return 0;
}

