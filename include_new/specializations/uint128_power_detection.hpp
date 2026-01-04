#ifndef UINT128_POWER_DETECTION_HPP
#define UINT128_POWER_DETECTION_HPP

#include <cstdint>
#include <type_traits>

/**
 * @file uint128_power_detection.hpp
 * @brief Detección en tiempo de compilación de potencias de números < 64
 *
 * Este header proporciona funciones constexpr para detectar si un número
 * es una potencia de otro número base (2, 3, 5, 7, ..., 61).
 *
 * Funciones principales:
 * - is_power_of_2, is_power_of_3, ..., is_power_of_61: Detección específica por base
 * - is_power_of<Base>(n): Template genérico para cualquier base
 * - log_base<Base>(n): Calcula el logaritmo en una base específica
 * - pow<Base, Exp>: Calcula Base^Exp en tiempo de compilación
 *
 * Todas las funciones son constexpr y noexcept para uso en compile-time.
 */

namespace uint128_power_detection
{

// ============================================================================
// Funciones auxiliares de detección de potencias
// ============================================================================

/**
 * @brief Detecta si un número es potencia de 2
 * @param n Número a verificar
 * @return true si n es potencia de 2, false en caso contrario
 * @note Algoritmo: n > 0 && (n & (n-1)) == 0
 */
static constexpr bool is_power_of_2(std::uint64_t n) noexcept
{
    return n > 0 && (n & (n - 1)) == 0;
}

/**
 * @brief Detecta si un número es potencia de 3
 * @param n Número a verificar
 * @return true si n es potencia de 3, false en caso contrario
 */
static constexpr bool is_power_of_3(std::uint64_t n) noexcept
{
    if (n < 3)
        return false;
    while (n > 1) {
        if (n % 3 != 0)
            return false;
        n /= 3;
    }
    return true;
}

/**
 * @brief Detecta si un número es potencia de 5
 * @param n Número a verificar
 * @return true si n es potencia de 5, false en caso contrario
 */
static constexpr bool is_power_of_5(std::uint64_t n) noexcept
{
    if (n < 5)
        return false;
    while (n > 1) {
        if (n % 5 != 0)
            return false;
        n /= 5;
    }
    return true;
}

/**
 * @brief Detecta si un número es potencia de 7
 * @param n Número a verificar
 * @return true si n es potencia de 7, false en caso contrario
 */
static constexpr bool is_power_of_7(std::uint64_t n) noexcept
{
    if (n < 7)
        return false;
    while (n > 1) {
        if (n % 7 != 0)
            return false;
        n /= 7;
    }
    return true;
}

/**
 * @brief Detecta si un número es potencia de 10
 * @param n Número a verificar
 * @return true si n es potencia de 10, false en caso contrario
 */
static constexpr bool is_power_of_10(std::uint64_t n) noexcept
{
    if (n < 10)
        return false;
    while (n > 1) {
        if (n % 10 != 0)
            return false;
        n /= 10;
    }
    return true;
}

/**
 * @brief Template genérico para detectar potencias de cualquier base
 * @tparam Base La base a verificar (debe estar entre 2 y 63)
 * @param n Número a verificar
 * @return true si n es potencia de Base, false en caso contrario
 */
template <std::uint64_t Base>
    requires(Base >= 2 && Base <= 63)
static constexpr bool is_power_of(std::uint64_t n) noexcept
{
    if constexpr (Base == 2) {
        return is_power_of_2(n);
    } else if constexpr (Base == 3) {
        return is_power_of_3(n);
    } else if constexpr (Base == 5) {
        return is_power_of_5(n);
    } else if constexpr (Base == 7) {
        return is_power_of_7(n);
    } else if constexpr (Base == 10) {
        return is_power_of_10(n);
    } else {
        // Algoritmo genérico para otras bases
        if (n < Base)
            return false;
        while (n > 1) {
            if (n % Base != 0)
                return false;
            n /= Base;
        }
        return true;
    }
}

// ============================================================================
// Funciones de logaritmo en bases específicas
// ============================================================================

/**
 * @brief Calcula log₂(n) redondeado hacia abajo
 * @param n Número (debe ser > 0)
 * @return Logaritmo base 2 de n
 */
static constexpr int log2_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n >>= 1;
        ++result;
    }
    return result;
}

/**
 * @brief Calcula log₃(n) redondeado hacia abajo
 * @param n Número (debe ser > 0)
 * @return Logaritmo base 3 de n
 */
static constexpr int log3_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n /= 3;
        ++result;
    }
    return result;
}

/**
 * @brief Calcula log₅(n) redondeado hacia abajo
 * @param n Número (debe ser > 0)
 * @return Logaritmo base 5 de n
 */
static constexpr int log5_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n /= 5;
        ++result;
    }
    return result;
}

/**
 * @brief Calcula log₇(n) redondeado hacia abajo
 * @param n Número (debe ser > 0)
 * @return Logaritmo base 7 de n
 */
static constexpr int log7_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n /= 7;
        ++result;
    }
    return result;
}

/**
 * @brief Calcula log₁₀(n) redondeado hacia abajo
 * @param n Número (debe ser > 0)
 * @return Logaritmo base 10 de n
 */
static constexpr int log10_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n /= 10;
        ++result;
    }
    return result;
}

/**
 * @brief Template genérico para calcular logaritmo en cualquier base
 * @tparam Base La base del logaritmo (debe estar entre 2 y 63)
 * @param n Número (debe ser > 0)
 * @return Logaritmo base Base de n, redondeado hacia abajo
 */
template <std::uint64_t Base>
    requires(Base >= 2 && Base <= 63)
static constexpr int log_base(std::uint64_t n) noexcept
{
    if constexpr (Base == 2) {
        return log2_uint64(n);
    } else if constexpr (Base == 3) {
        return log3_uint64(n);
    } else if constexpr (Base == 5) {
        return log5_uint64(n);
    } else if constexpr (Base == 7) {
        return log7_uint64(n);
    } else if constexpr (Base == 10) {
        return log10_uint64(n);
    } else {
        // Algoritmo genérico
        int result = 0;
        while (n > 1) {
            n /= Base;
            ++result;
        }
        return result;
    }
}

// ============================================================================
// Funciones para calcular potencias en tiempo de compilación
// ============================================================================

/**
 * @brief Calcula 2^exp en tiempo de compilación
 * @param exp Exponente
 * @return 2^exp
 */
static constexpr std::uint64_t pow2(int exp) noexcept
{
    return 1ULL << exp;
}

/**
 * @brief Calcula 3^exp en tiempo de compilación
 * @param exp Exponente
 * @return 3^exp
 */
static constexpr std::uint64_t pow3(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 3;
    }
    return result;
}

/**
 * @brief Calcula 5^exp en tiempo de compilación
 * @param exp Exponente
 * @return 5^exp
 */
static constexpr std::uint64_t pow5(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 5;
    }
    return result;
}

/**
 * @brief Calcula 7^exp en tiempo de compilación
 * @param exp Exponente
 * @return 7^exp
 */
static constexpr std::uint64_t pow7(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 7;
    }
    return result;
}

/**
 * @brief Calcula 10^exp en tiempo de compilación
 * @param exp Exponente
 * @return 10^exp
 */
static constexpr std::uint64_t pow10(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}

/**
 * @brief Template genérico para calcular Base^Exp en tiempo de compilación
 * @tparam Base La base de la potencia
 * @tparam Exp El exponente
 * @return Base^Exp
 * @note Usa exponenciación rápida para mejor rendimiento
 */
template <std::uint64_t Base, int Exp>
    requires(Base >= 2 && Base <= 63 && Exp >= 0)
static constexpr std::uint64_t pow() noexcept
{
    if constexpr (Exp == 0) {
        return 1;
    } else if constexpr (Exp == 1) {
        return Base;
    } else if constexpr (Base == 2) {
        return pow2(Exp);
    } else if constexpr (Base == 3) {
        return pow3(Exp);
    } else if constexpr (Base == 5) {
        return pow5(Exp);
    } else if constexpr (Base == 7) {
        return pow7(Exp);
    } else if constexpr (Base == 10) {
        return pow10(Exp);
    } else {
        // Exponenciación rápida genérica
        std::uint64_t result = 1;
        std::uint64_t base = Base;
        int exp = Exp;
        while (exp > 0) {
            if (exp & 1) {
                result *= base;
            }
            base *= base;
            exp >>= 1;
        }
        return result;
    }
}

/**
 * @brief Versión runtime de pow<Base, Exp>
 * @tparam Base La base de la potencia
 * @param exp El exponente (runtime)
 * @return Base^exp
 */
template <std::uint64_t Base>
    requires(Base >= 2 && Base <= 63)
static constexpr std::uint64_t pow_runtime(int exp) noexcept
{
    if constexpr (Base == 2) {
        return pow2(exp);
    } else if constexpr (Base == 3) {
        return pow3(exp);
    } else if constexpr (Base == 5) {
        return pow5(exp);
    } else if constexpr (Base == 7) {
        return pow7(exp);
    } else if constexpr (Base == 10) {
        return pow10(exp);
    } else {
        // Exponenciación rápida genérica
        std::uint64_t result = 1;
        std::uint64_t base = Base;
        while (exp > 0) {
            if (exp & 1) {
                result *= base;
            }
            base *= base;
            exp >>= 1;
        }
        return result;
    }
}

} // namespace uint128_power_detection

#endif // UINT128_POWER_DETECTION_HPP
