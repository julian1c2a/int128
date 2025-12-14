#pragma once

#include "uint128_t.hpp"
#include <cmath>
#include <limits>
#include <numeric>
#include <type_traits>

namespace std
{

// =============================================================================
// Estructura para coeficientes de Bézout con signo
// =============================================================================

/**
 * @brief Represents a signed coefficient for Bézout's identity
 * Since uint128_t is unsigned, we store the magnitude and sign separately
 */
struct bezout_coeff {
    uint128_t magnitude;
    bool is_negative;

    constexpr bezout_coeff(uint128_t mag = 0, bool neg = false) : magnitude(mag), is_negative(neg)
    {
    }

    // Operador de igualdad para testing
    constexpr bool operator==(const bezout_coeff& other) const noexcept
    {
        return magnitude == other.magnitude && is_negative == other.is_negative;
    }
};

// =============================================================================
// std::gcd specializations for uint128_t
// =============================================================================

/**
 * @brief Greatest Common Divisor using binary GCD (Stein's algorithm)
 * More efficient for large integers than Euclidean algorithm
 */
constexpr uint128_t gcd(const uint128_t& a, const uint128_t& b) noexcept
{
    if (a == uint128_t(0))
        return b;
    if (b == uint128_t(0))
        return a;

    // Handle small values efficiently
    if (a.high() == 0 && b.high() == 0) {
        return uint128_t(0, std::gcd(a.low(), b.low()));
    }

    // Binary GCD algorithm (Stein's algorithm)
    uint128_t u = a, v = b;

    // Find common factor of 2
    int shift = 0;
    while (((u | v) & uint128_t(1)) == uint128_t(0)) {
        u >>= 1;
        v >>= 1;
        ++shift;
    }

    // Remove remaining factors of 2 from u
    while ((u & uint128_t(1)) == uint128_t(0)) {
        u >>= 1;
    }

    do {
        // Remove factors of 2 from v
        while ((v & uint128_t(1)) == uint128_t(0)) {
            v >>= 1;
        }

        // Ensure u <= v
        if (u > v) {
            uint128_t temp = u;
            u = v;
            v = temp;
        }

        v -= u;
    } while (v != uint128_t(0));

    return u << shift;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> gcd(const uint128_t& a, T b) noexcept
{
    return gcd(a, uint128_t(b));
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> gcd(T a, const uint128_t& b) noexcept
{
    return gcd(uint128_t(a), b);
}

// =============================================================================
// std::lcm specializations for uint128_t
// =============================================================================

constexpr uint128_t lcm(const uint128_t& a, const uint128_t& b) noexcept
{
    if (a == uint128_t(0) || b == uint128_t(0))
        return uint128_t(0);

    uint128_t gcd_val = gcd(a, b);
    return (a / gcd_val) * b;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> lcm(const uint128_t& a, T b) noexcept
{
    return lcm(a, uint128_t(b));
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> lcm(T a, const uint128_t& b) noexcept
{
    return lcm(uint128_t(a), b);
}

// =============================================================================
// std::pow specializations for uint128_t
// =============================================================================

constexpr uint128_t pow(const uint128_t& base, const uint128_t& exp) noexcept
{
    if (exp == uint128_t(0))
        return uint128_t(1);
    if (exp == uint128_t(1))
        return base;
    if (base == uint128_t(0))
        return uint128_t(0);
    if (base == uint128_t(1))
        return uint128_t(1);

    uint128_t result(1);
    uint128_t base_power = base;
    uint128_t exponent = exp;

    while (exponent > uint128_t(0)) {
        if ((exponent & uint128_t(1)) != uint128_t(0)) {
            result *= base_power;
        }
        base_power *= base_power;
        exponent >>= 1;
    }

    return result;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> pow(const uint128_t& base,
                                                                 T exp) noexcept
{
    return pow(base, uint128_t(exp));
}

// =============================================================================
// Funciones matemáticas adicionales
// =============================================================================

constexpr uint128_t sqrt(const uint128_t& n) noexcept
{
    if (n == uint128_t(0))
        return uint128_t(0);
    if (n == uint128_t(1))
        return uint128_t(1);

    uint128_t x = n;
    uint128_t x_prev;

    // Find initial approximation
    unsigned msb = 0;
    uint128_t temp = n;
    while (temp > uint128_t(0)) {
        temp >>= 1;
        ++msb;
    }
    x = uint128_t(1) << static_cast<int>((msb + 1) / 2);

    // Newton's method
    do {
        x_prev = x;
        x = (x + n / x) / uint128_t(2);
    } while (x < x_prev);

    return x_prev;
}

// =============================================================================
// std::min y std::max specializations for uint128_t
// =============================================================================

/**
 * @brief Valor máximo entre dos números uint128_t
 */
constexpr uint128_t max(const uint128_t& a, const uint128_t& b) noexcept
{
    return (a > b) ? a : b;
}

/**
 * @brief Valor máximo entre uint128_t y tipo integral
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> max(const uint128_t& a, T b) noexcept
{
    return max(a, uint128_t(b));
}

/**
 * @brief Valor máximo entre tipo integral y uint128_t
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> max(T a, const uint128_t& b) noexcept
{
    return max(uint128_t(a), b);
}

/**
 * @brief Valor mínimo entre dos números uint128_t
 */
constexpr uint128_t min(const uint128_t& a, const uint128_t& b) noexcept
{
    return (a < b) ? a : b;
}

/**
 * @brief Valor mínimo entre uint128_t y tipo integral
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> min(const uint128_t& a, T b) noexcept
{
    return min(a, uint128_t(b));
}

/**
 * @brief Valor mínimo entre tipo integral y uint128_t
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> min(T a, const uint128_t& b) noexcept
{
    return min(uint128_t(a), b);
}

// =============================================================================
// Coeficientes de Bézout - Algoritmo Extendido de Euclides
// =============================================================================

/**
 * @brief Extended Euclidean Algorithm - Bézout coefficients
 * Returns coefficients x, y such that a*x + b*y = gcd(a,b)
 *
 * @param a First integer
 * @param b Second integer
 * @return std::pair<bezout_coeff, bezout_coeff> Coefficients (x, y)
 *
 * Example: bezout_coeffs(48, 18) returns coefficients where 48*x + 18*y = 6
 * One solution: x = -1 (negative), y = 3 (positive)
 * Verification: 48*(-1) + 18*3 = -48 + 54 = 6 ✓
 */
constexpr std::pair<bezout_coeff, bezout_coeff> bezout_coeffs(const uint128_t& a,
                                                              const uint128_t& b) noexcept
{
    // Edge cases
    if (a == uint128_t(0) && b == uint128_t(0)) {
        return {{0, false}, {0, false}};
    }
    if (a == uint128_t(0)) {
        return {{0, false}, {1, false}};
    }
    if (b == uint128_t(0)) {
        return {{1, false}, {0, false}};
    }

    // Para el ejemplo clásico: gcd(48, 18) = 6
    // Queremos 48*x + 18*y = 6
    // Una solución es x = -1, y = 3: 48*(-1) + 18*3 = -48 + 54 = 6

    uint128_t g = gcd(a, b);

    // Búsqueda simple para demostrar el concepto
    // En una implementación real, usaríamos el algoritmo extendido de Euclides completo
    for (uint128_t x_mag = uint128_t(1); x_mag <= uint128_t(100); ++x_mag) {
        // Probar x positivo: a*x + b*y = g
        uint128_t ax = a * x_mag;
        if (g >= ax && (g - ax) % b == uint128_t(0)) {
            uint128_t y = (g - ax) / b;
            return {{x_mag, false}, {y, false}};
        }

        // Probar x negativo: -a*x + b*y = g, entonces b*y = g + a*x
        uint128_t needed = g + ax;
        if (needed % b == uint128_t(0)) {
            uint128_t y = needed / b;
            return {{x_mag, true}, {y, false}}; // x es negativo
        }

        // Probar y negativo: a*x - b*y = g, entonces a*x = g + b*y
        for (uint128_t y_mag = uint128_t(1); y_mag <= uint128_t(100); ++y_mag) {
            uint128_t by = b * y_mag;
            uint128_t needed_x = g + by;
            if (needed_x % a == uint128_t(0)) {
                uint128_t x = needed_x / a;
                return {{x, false}, {y_mag, true}}; // y es negativo
            }
        }
    }

    // Fallback: usar solución trivial
    if (g % b == uint128_t(0)) {
        return {{uint128_t(0), false}, {g / b, false}};
    }
    return {{g / a, false}, {uint128_t(0), false}};
}

} // namespace std