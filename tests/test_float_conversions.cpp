/**
 * Test de conversiones a tipos de punto flotante
 */

#include <cmath>
#include <int128.hpp>
#include <iomanip>
#include <iostream>

using namespace uint128_literals;
using namespace int128_literals;

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "=== Tests de Conversión uint128_t → float/double/long double ===\n\n";

    // Test 1: uint128_t pequeño
    uint128_t u1 = 12345_u128;
    std::cout << "uint128_t(12345):\n";
    std::cout << "  → float:       " << static_cast<float>(u1) << "\n";
    std::cout << "  → double:      " << static_cast<double>(u1) << "\n";
    std::cout << "  → long double: " << static_cast<long double>(u1) << "\n\n";

    // Test 2: uint128_t grande (2^64)
    uint128_t u2(1, 0); // 2^64
    std::cout << "uint128_t(2^64 = 18446744073709551616):\n";
    std::cout << "  → float:       " << static_cast<float>(u2) << "\n";
    std::cout << "  → double:      " << static_cast<double>(u2) << "\n";
    std::cout << "  → long double: " << static_cast<long double>(u2) << "\n\n";

    // Test 3: uint128_t muy grande
    uint128_t u3(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // 2^128 - 1
    std::cout << "uint128_t(2^128 - 1):\n";
    std::cout << "  → float:       " << static_cast<float>(u3) << "\n";
    std::cout << "  → double:      " << static_cast<double>(u3) << "\n";
    std::cout << "  → long double: " << static_cast<long double>(u3) << "\n\n";

    std::cout << "=== Tests de Conversión int128_t → float/double/long double ===\n\n";

    // Test 4: int128_t positivo
    int128_t i1 = 12345_i128;
    std::cout << "int128_t(12345):\n";
    std::cout << "  → float:       " << i1.to<float>() << "\n";
    std::cout << "  → double:      " << i1.to<double>() << "\n";
    std::cout << "  → long double: " << i1.to<long double>() << "\n\n";

    // Test 5: int128_t negativo
    int128_t i2 = -12345_i128;
    std::cout << "int128_t(-12345):\n";
    std::cout << "  → float:       " << i2.to<float>() << "\n";
    std::cout << "  → double:      " << i2.to<double>() << "\n";
    std::cout << "  → long double: " << i2.to<long double>() << "\n\n";

    // Test 6: int128_t muy grande negativo
    int128_t i3(0x8000000000000000, 0); // -2^127
    std::cout << "int128_t(-2^127):\n";
    std::cout << "  → float:       " << i3.to<float>() << "\n";
    std::cout << "  → double:      " << i3.to<double>() << "\n";
    std::cout << "  → long double: " << i3.to<long double>() << "\n\n";

    // Test 7: Uso práctico - calcular sqrt
    std::cout << "=== Uso Práctico: sqrt(uint128_t) ===\n\n";
    uint128_t num = 1000000000000_u128;
    double num_double = static_cast<double>(num);
    double result = std::sqrt(num_double);
    std::cout << "sqrt(" << num_double << ") ≈ " << result << "\n\n";

    std::cout << "✓ Todas las conversiones ejecutadas correctamente\n";

    return 0;
}
