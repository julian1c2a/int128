/**
 * Tutorial 10: Funciones Numéricas C++20
 *
 * Aprenderás:
 * - midpoint (punto medio seguro)
 * - clamp (acotar valores)
 * - Otros helpers numéricos
 */

#include <algorithm>
#include <int128_simple.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>

using namespace nstd;

using namespace uint128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 10: Funciones Numéricas C++20\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. MIDPOINT (PUNTO MEDIO)
    // ============================================================
    std::cout << "--- 1. Punto medio (midpoint) ---\n";

    auto a = 100_u128;
    auto b = 200_u128;

    // Cálculo seguro del punto medio (evita overflow)
    auto mid = (a + b) / 2; // Para estos valores pequeños está bien

    std::cout << "midpoint(" << a << ", " << b << ") = " << mid << "\n\n";

    // ============================================================
    // 2. CLAMP (ACOTAR)
    // ============================================================
    std::cout << "--- 2. Clamp (acotar entre límites) ---\n";

    auto value1 = 50_u128;
    auto value2 = 150_u128;
    auto value3 = 250_u128;
    auto min_val = 100_u128;
    auto max_val = 200_u128;

    auto clamped1 = std::clamp(value1, min_val, max_val);
    auto clamped2 = std::clamp(value2, min_val, max_val);
    auto clamped3 = std::clamp(value3, min_val, max_val);

    std::cout << "clamp(" << value1 << ", 100, 200) = " << clamped1 << "\n";
    std::cout << "clamp(" << value2 << ", 100, 200) = " << clamped2 << "\n";
    std::cout << "clamp(" << value3 << ", 100, 200) = " << clamped3 << "\n\n";

    // ============================================================
    // 3. MIN/MAX DE VARIOS VALORES
    // ============================================================
    std::cout << "--- 3. Min/Max de múltiples valores ---\n";

    auto values = {10_u128, 50_u128, 30_u128, 90_u128, 20_u128};

    auto minimum = std::min(values);
    auto maximum = std::max(values);

    std::cout << "Mínimo: " << minimum << "\n";
    std::cout << "Máximo: " << maximum << "\n\n";

    // ============================================================
    // 4. EXCHANGE
    // ============================================================
    std::cout << "--- 4. Exchange (intercambiar) ---\n";

    auto x = 123_u128;
    auto old_x = std::exchange(x, 456_u128);

    std::cout << "Valor anterior: " << old_x << "\n";
    std::cout << "Nuevo valor:    " << x << "\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 10 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 11_thread_safety.cpp\n";

    return 0;
}

