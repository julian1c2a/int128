/**
 * Tutorial 08: Funciones Matemáticas (cmath)
 *
 * Aprenderás:
 * - abs, gcd, lcm
 * - sqrt, pow
 * - Funciones numéricas
 */

#include <int128_simple.hpp>
#include <iostream>

using namespace nstd;

using namespace uint128_literals;
using namespace int128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 08: Funciones Matemáticas\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. VALOR ABSOLUTO (abs)
    // ============================================================
    std::cout << "--- 1. Valor absoluto ---\n";

    int128_t negative = -12345_i128;
    auto positive = nstd::abs(negative);

    std::cout << "abs(" << negative << ") = " << positive << "\n\n";

    // ============================================================
    // 2. MÁXIMO COMÚN DIVISOR (gcd)
    // ============================================================
    std::cout << "--- 2. GCD (Máximo Común Divisor) ---\n";

    auto a = 48_u128;
    auto b = 18_u128;
    auto result_gcd = nstd::gcd(a, b);

    std::cout << "gcd(" << a << ", " << b << ") = " << result_gcd << "\n\n";

    // ============================================================
    // 3. MÍNIMO COMÚN MÚLTIPLO (lcm)
    // ============================================================
    std::cout << "--- 3. LCM (Mínimo Común Múltiplo) ---\n";

    auto result_lcm = nstd::lcm(a, b);

    std::cout << "lcm(" << a << ", " << b << ") = " << result_lcm << "\n\n";

    // ============================================================
    // 4. POTENCIA (pow)
    // ============================================================
    std::cout << "--- 4. Potencia ---\n";

    auto base = 2_u128;
    auto exponent = 10;

    uint128_t power{1};
    for (int i = 0; i < exponent; ++i) {
        power *= base;
    }

    std::cout << base << "^" << exponent << " = " << power << "\n\n";

    // ============================================================
    // 5. RAÍZ CUADRADA (sqrt)
    // ============================================================
    std::cout << "--- 5. Raíz cuadrada ---\n";

    auto num = 144_u128;

    // Implementación simple de sqrt
    auto sqrt_approx = [](uint128_t n) -> uint128_t {
        if (n == 0)
            return 0;
        uint128_t x = n;
        uint128_t y = (x + 1) / 2;
        while (y < x) {
            x = y;
            y = (x + n / x) / 2;
        }
        return x;
    };

    auto root = sqrt_approx(num);

    std::cout << "sqrt(" << num << ") ≈ " << root << "\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 08 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 09_algorithms_stl.cpp\n";

    return 0;
}

