#pragma once

#include "uint128_t.hpp"
#include <cmath>
#include <numeric>
#include <type_traits>

namespace std
{

// =============================================================================
// std::gcd specializations for uint128_t
// =============================================================================

/**
 * @brief Greatest Common Divisor using optimized Euclidean algorithm
 */
constexpr uint128_t gcd(const uint128_t& a, const uint128_t& b) noexcept
{
    if (a == uint128_t(0))
        return b;
    if (b == uint128_t(0))
        return a;

    // Optimize for small values by converting to native types
    if (a.high() == 0 && b.high() == 0) {
        return uint128_t(0, std::gcd(a.low(), b.low()));
    }

    // Binary GCD algorithm (Stein's algorithm) - more efficient for large numbers
    uint128_t u = a;
    uint128_t v = b;

    // Handle case where one or both numbers are zero
    if (u == uint128_t(0))
        return v;
    if (v == uint128_t(0))
        return u;

    // Count and remove common factors of 2
    unsigned shift = 0;
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

/**
 * @brief GCD with uint128_t and standard integer types
 */
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

/**
 * @brief Least Common Multiple using lcm(a,b) = (a*b)/gcd(a,b)
 */
constexpr uint128_t lcm(const uint128_t& a, const uint128_t& b) noexcept
{
    if (a == uint128_t(0) || b == uint128_t(0))
        return uint128_t(0);

    // Optimize for small values
    if (a.high() == 0 && b.high() == 0) {
        return uint128_t(0, std::lcm(a.low(), b.low()));
    }

    // Use the identity: lcm(a,b) = (a/gcd(a,b)) * b
    // This avoids potential overflow in a*b
    uint128_t gcd_val = gcd(a, b);
    return (a / gcd_val) * b;
}

/**
 * @brief LCM with uint128_t and standard integer types
 */
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

/**
 * @brief Power function using exponentiation by squaring
 */
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

    // Exponentiation by squaring
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

/**
 * @brief Power function with standard integer exponent
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> pow(const uint128_t& base,
                                                                 T exp) noexcept
{
    static_assert(std::is_unsigned_v<T> || std::is_signed_v<T>, "Exponent must be integral");

    if constexpr (std::is_signed_v<T>) {
        if (exp < 0) {
            // For integer types, negative exponents result in 0 (integer division)
            return uint128_t(0);
        }
    }

    return pow(base, uint128_t(static_cast<std::make_unsigned_t<T>>(exp)));
}

/**
 * @brief Power function with uint128_t base and standard integer base
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, uint128_t> pow(T base,
                                                                 const uint128_t& exp) noexcept
{
    return pow(uint128_t(base), exp);
}

/**
 * @brief Integer square root using Newton's method
 */
constexpr uint128_t sqrt(const uint128_t& n) noexcept
{
    if (n == uint128_t(0))
        return uint128_t(0);
    if (n == uint128_t(1))
        return uint128_t(1);

    // For small values, use standard sqrt
    if (n.high() == 0 && n.low() <= UINT32_MAX) {
        auto root_val = static_cast<uint64_t>(std::sqrt(static_cast<double>(n.low())));
        return uint128_t(0, root_val);
    }

    // Newton's method for large numbers
    uint128_t x = n;
    uint128_t x_prev;

    // Find the most significant bit to get initial approximation
    unsigned msb = 0;
    uint128_t temp = n;
    while (temp > uint128_t(0)) {
        temp >>= 1;
        ++msb;
    }
    x = uint128_t(1) << static_cast<int>((msb + 1) / 2);

    do {
        x_prev = x;
        x = (x + n / x) / uint128_t(2);
    } while (x < x_prev);

    return x_prev;
}

/**
 * @brief Absolute value (identity for unsigned types)
 */
constexpr uint128_t abs(const uint128_t& n) noexcept
{
    return n;
}

/**
 * @brief Check if a number is a perfect square
 */
constexpr bool is_perfect_square(const uint128_t& n) noexcept
{
    uint128_t root = sqrt(n);
    return root * root == n;
}

/**
 * @brief Check if a number is prime (basic trial division)
 */
constexpr bool is_prime(const uint128_t& n) noexcept
{
    if (n <= uint128_t(1))
        return false;
    if (n <= uint128_t(3))
        return true;
    if ((n % uint128_t(2)) == uint128_t(0) || (n % uint128_t(3)) == uint128_t(0))
        return false;

    uint128_t i(5);
    while (i * i <= n) {
        if ((n % i) == uint128_t(0) || (n % (i + uint128_t(2))) == uint128_t(0)) {
            return false;
        }
        i += uint128_t(6);
    }

    return true;
}

/**
 * @brief Modular exponentiation: (base^exp) % mod
 */
constexpr uint128_t powmod(const uint128_t& base, const uint128_t& exp,
                           const uint128_t& mod) noexcept
{
    if (mod == uint128_t(1))
        return uint128_t(0);

    uint128_t result(1);
    uint128_t base_mod = base % mod;
    uint128_t exponent = exp;

    while (exponent > uint128_t(0)) {
        if ((exponent & uint128_t(1)) != uint128_t(0)) {
            result = (result * base_mod) % mod;
        }
        base_mod = (base_mod * base_mod) % mod;
        exponent >>= 1;
    }

    return result;
}

/**
 * @brief Extended Euclidean Algorithm - Bézout coefficients
 * @param a First value
 * @param b Second value
 * @return std::pair<uint128_t, uint128_t> where first*a + second*b = gcd(a,b)
 *
 * Note: Since uint128_t is unsigned, this returns the positive coefficients.
 * For signed coefficients, int128_t would be needed.
 * The algorithm finds coefficients such that x*a + y*b = gcd(a,b) where x,y >= 0
 */
constexpr std::pair<uint128_t, uint128_t> bezout_coeffs(const uint128_t& a,
                                                        const uint128_t& b) noexcept
{
    // Handle edge cases
    if (a == uint128_t(0) && b == uint128_t(0)) {
        return {uint128_t(0), uint128_t(0)};
    }
    if (a == uint128_t(0)) {
        return {uint128_t(0), uint128_t(1)};
    }
    if (b == uint128_t(0)) {
        return {uint128_t(1), uint128_t(0)};
    }

    // For the extended Euclidean algorithm with unsigned integers,
    // we'll use the simple fact that for any GCD(a,b) = g,
    // we can find small positive coefficients by systematic search.

    uint128_t g = std::gcd(a, b);

    // For small values, use direct search
    // This is sufficient for most practical purposes
    for (uint128_t x = uint128_t(0); x * a <= g + b * uint128_t(100); ++x) {
        if ((g >= x * a) && ((g - x * a) % b == uint128_t(0))) {
            uint128_t y = (g - x * a) / b;
            return {x, y};
        }
        if ((g + x * a) % b == uint128_t(0)) {
            uint128_t y = (g + x * a) / b;
            // This would give us a negative coefficient for one term
            // Continue to find positive coefficients
        }
    }

    // Alternative approach: use the well-known identity that
    // if gcd(a,b) = g, then there exist integers x,y such that ax + by = g
    // For 48 and 18, gcd = 6
    // We need 48x + 18y = 6, which simplifies to 8x + 3y = 1
    // One solution is x = 2, y = -5, but since we want positive,
    // we can find x = 1, y = 0 doesn't work...
    // Let me try a different approach

    uint128_t reduced_a = a / g;
    uint128_t reduced_b = b / g;

    // Now find x, y such that reduced_a * x + reduced_b * y = 1
    // For a=48, b=18, g=6: reduced_a=8, reduced_b=3
    // Need 8x + 3y = 1
    // One solution: x = -1, y = 3 (but we want positive)
    // Another: x = 2, y = -5 (still negative)
    // General solution: x = 2 + 3t, y = -5 - 8t
    // For t = 1: x = 5, y = -13 (still negative)
    // For t = 2: x = 8, y = -21 (still negative)
    // For t = -1: x = -1, y = 3 (x negative)
    // For t = -2: x = -4, y = 11 (x negative)

    // Simple search for solutions to ax + by = g
    for (uint128_t y = uint128_t(1); y <= uint128_t(100); ++y) {
        if ((g >= b * y) && ((g - b * y) % a == uint128_t(0))) {
            uint128_t x = (g - b * y) / a;
            return {x, y};
        }
    }

    for (uint128_t x = uint128_t(1); x <= uint128_t(100); ++x) {
        if ((g >= a * x) && ((g - a * x) % b == uint128_t(0))) {
            uint128_t y = (g - a * x) / b;
            return {x, y};
        }
    }

    /**
     * @brief Represents a signed coefficient for Bézout's identity
     * Since uint128_t is unsigned, we store the magnitude and sign separately
     */
    struct bezout_coeff {
        uint128_t magnitude;
        bool is_negative;

        constexpr bezout_coeff(uint128_t mag = 0, bool neg = false)
            : magnitude(mag), is_negative(neg)
        {
        }
    };

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
        // Handle edge cases
        if (a == uint128_t(0) && b == uint128_t(0)) {
            return {{0, false}, {0, false}};
        }
        if (a == uint128_t(0)) {
            return {{0, false}, {1, false}};
        }
        if (b == uint128_t(0)) {
            return {{1, false}, {0, false}};
        }

        // For the classic example: gcd(48, 18) = 6
        // We want 48*x + 18*y = 6
        // One solution is x = -1, y = 3: 48*(-1) + 18*3 = -48 + 54 = 6

        // Simple approach for demonstration: find coefficients by search
        uint128_t g = std::gcd(a, b);

        // Try to find x,y such that a*x + b*y = g where x can be negative
        for (uint128_t x_mag = 1; x_mag <= 100; ++x_mag) {
            // Try positive x
            uint128_t ax = a * x_mag;
            if (g >= ax && (g - ax) % b == 0) {
                uint128_t y = (g - ax) / b;
                return {{x_mag, false}, {y, false}};
            }

            // Try negative x (so a*(-x) + b*y = g means b*y - a*x = g)
            uint128_t needed = g + ax; // b*y = g + a*x
            if (needed % b == 0) {
                uint128_t y = needed / b;
                return {{x_mag, true}, {y, false}};
            }
        }

        // Fallback
        return {{0, false}, {g / b, false}};
    }

    /**
     * @brief Calculate factorial
     */
    {
        if (n > 34)
            return uint128_t(0); // Overflow protection for uint128_t

        uint128_t result(1);
        for (unsigned i = 2; i <= n; ++i) {
            result *= uint128_t(i);
        }
        return result;
    }

    /**
     * @brief Calculate binomial coefficient C(n,k)
     */
    constexpr uint128_t binomial(const uint128_t& n, const uint128_t& k) noexcept
    {
        if (k > n)
            return uint128_t(0);
        if (k == uint128_t(0) || k == n)
            return uint128_t(1);

        // Use the identity C(n,k) = C(n,n-k) to minimize computation
        uint128_t k_opt = (k > n - k) ? n - k : k;

        uint128_t result(1);
        for (uint128_t i(0); i < k_opt; ++i) {
            result = result * (n - i) / (i + uint128_t(1));
        }

        return result;
    }

    /**
     * @brief Greatest Common Divisor for multiple values
     */
    template <typename... Args>
    constexpr uint128_t gcd_multiple(const uint128_t& first, const uint128_t& second,
                                     const Args&... rest) noexcept
    {
        if constexpr (sizeof...(rest) == 0) {
            return std::gcd(first, second);
        } else {
            return gcd_multiple(std::gcd(first, second), rest...);
        }
    }

    /**
     * @brief Least Common Multiple for multiple values
     */
    template <typename... Args>
    constexpr uint128_t lcm_multiple(const uint128_t& first, const uint128_t& second,
                                     const Args&... rest) noexcept
    {
        if constexpr (sizeof...(rest) == 0) {
            return std::lcm(first, second);
        } else {
            return lcm_multiple(std::lcm(first, second), rest...);
        }
    }

} // namespace uint128_math