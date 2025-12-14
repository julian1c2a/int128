#pragma once

#include "int128_t.hpp"
#include <cmath>
#include <limits>
#include <numeric>
#include <type_traits>

namespace std
{

// =============================================================================
// std::gcd specializations for int128_t
// =============================================================================

/**
 * @brief Greatest Common Divisor for signed 128-bit integers
 * Uses binary GCD algorithm with sign handling
 */
constexpr int128_t gcd(const int128_t& a, const int128_t& b) noexcept
{
    // gcd(-a, b) = gcd(a, -b) = gcd(-a, -b) = gcd(a, b)
    int128_t abs_a = a.abs();
    int128_t abs_b = b.abs();

    if (abs_a.is_zero())
        return abs_b;
    if (abs_b.is_zero())
        return abs_a;

    // Convert to uint128_t for the algorithm
    uint128_t ua = abs_a.to_uint128();
    uint128_t ub = abs_b.to_uint128();

    // Handle small values efficiently
    if (ua.high() == 0 && ub.high() == 0) {
        return int128_t(std::gcd(ua.low(), ub.low()));
    }

    // Binary GCD algorithm (Stein's algorithm)
    uint128_t u = ua, v = ub;

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

    return int128_t((u << shift));
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, int128_t> gcd(const int128_t& a, T b) noexcept
{
    return gcd(a, int128_t(b));
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, int128_t> gcd(T a, const int128_t& b) noexcept
{
    return gcd(int128_t(a), b);
}

// =============================================================================
// std::lcm specializations for int128_t
// =============================================================================

constexpr int128_t lcm(const int128_t& a, const int128_t& b) noexcept
{
    int128_t abs_a = a.abs();
    int128_t abs_b = b.abs();

    if (abs_a.is_zero() || abs_b.is_zero())
        return int128_t(0);

    int128_t gcd_val = gcd(abs_a, abs_b);
    return (abs_a / gcd_val) * abs_b;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, int128_t> lcm(const int128_t& a, T b) noexcept
{
    return lcm(a, int128_t(b));
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, int128_t> lcm(T a, const int128_t& b) noexcept
{
    return lcm(int128_t(a), b);
}

// =============================================================================
// std::abs specialization for int128_t
// =============================================================================

constexpr int128_t abs(const int128_t& x) noexcept
{
    return x.abs();
}

// =============================================================================
// std::pow specializations for int128_t
// =============================================================================

constexpr int128_t pow(const int128_t& base, const int128_t& exp) noexcept
{
    // Para exponentes negativos, el resultado es 0 (división entera)
    if (exp.is_negative())
        return int128_t(0);

    if (exp.is_zero())
        return int128_t(1);
    if (exp == int128_t(1))
        return base;
    if (base.is_zero())
        return int128_t(0);
    if (base == int128_t(1))
        return int128_t(1);
    if (base == int128_t(-1)) {
        // (-1)^n = 1 if n is even, -1 if n is odd
        return ((exp.low() & 1) == 0) ? int128_t(1) : int128_t(-1);
    }

    // Exponenciación binaria
    int128_t result(1);
    int128_t base_power = base;
    int128_t exponent = exp;

    while (exponent > int128_t(0)) {
        if ((exponent.low() & 1) != 0) {
            result *= base_power;
        }
        base_power *= base_power;
        exponent >>= 1;
    }

    return result;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, int128_t> pow(const int128_t& base,
                                                                T exp) noexcept
{
    return pow(base, int128_t(exp));
}

// =============================================================================
// Funciones matemáticas adicionales
// =============================================================================

constexpr int128_t sqrt(const int128_t& n) noexcept
{
    if (n.is_negative())
        return int128_t(0); // No hay raíz cuadrada real de números negativos

    if (n.is_zero())
        return int128_t(0);
    if (n == int128_t(1))
        return int128_t(1);

    // Convertir a uint128_t para usar su algoritmo de raíz cuadrada
    uint128_t n_unsigned = n.to_uint128();

    uint128_t x = n_unsigned;
    uint128_t x_prev;

    // Find initial approximation
    unsigned msb = 0;
    uint128_t temp = n_unsigned;
    while (temp > uint128_t(0)) {
        temp >>= 1;
        ++msb;
    }
    x = uint128_t(1) << static_cast<int>((msb + 1) / 2);

    // Newton's method
    do {
        x_prev = x;
        x = (x + n_unsigned / x) / uint128_t(2);
    } while (x < x_prev);

    return int128_t(x_prev);
}

// =============================================================================
// Funciones trigonométricas básicas (solo enteros)
// =============================================================================

/**
 * @brief Signo de un número
 * @return -1 si negativo, 0 si cero, 1 si positivo
 */
constexpr int128_t sign(const int128_t& x) noexcept
{
    if (x.is_zero())
        return int128_t(0);
    return x.is_negative() ? int128_t(-1) : int128_t(1);
}

/**
 * @brief Valor máximo entre dos números
 */
constexpr int128_t max(const int128_t& a, const int128_t& b) noexcept
{
    return (a > b) ? a : b;
}

/**
 * @brief Valor mínimo entre dos números
 */
constexpr int128_t min(const int128_t& a, const int128_t& b) noexcept
{
    return (a < b) ? a : b;
}

/**
 * @brief Clamp - restringe un valor a un rango
 */
constexpr int128_t clamp(const int128_t& value, const int128_t& min_val,
                         const int128_t& max_val) noexcept
{
    return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
}

/**
 * @brief Punto medio sin overflow
 */
constexpr int128_t midpoint(const int128_t& a, const int128_t& b) noexcept
{
    // Evitar overflow: usar a + (b - a) / 2
    if (a <= b) {
        int128_t diff = b - a;
        return a + diff / int128_t(2);
    } else {
        int128_t diff = a - b;
        return a - diff / int128_t(2);
    }
}

// =============================================================================
// División con módulo (divmod)
// =============================================================================

/**
 * @brief División y módulo simultáneos
 * @param dividend Dividendo
 * @param divisor Divisor (no puede ser 0)
 * @return Par (cociente, resto)
 */
constexpr std::pair<int128_t, int128_t> divmod(const int128_t& dividend, const int128_t& divisor)
{
    int128_t quotient = dividend / divisor;
    int128_t remainder = dividend % divisor;
    return {quotient, remainder};
}

} // namespace std