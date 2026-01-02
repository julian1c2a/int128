#ifndef UINT128_MULTIPLY_CONST_HPP
#define UINT128_MULTIPLY_CONST_HPP

#include "uint128_power_detection.hpp"
#include <cstdint>

/**
 * @file uint128_multiply_const.hpp
 * @brief Multiplicación optimizada por constantes en compile-time
 *
 * Este header proporciona métodos para multiplicar uint128_t por constantes
 * conocidas en tiempo de compilación, aplicando optimizaciones específicas.
 *
 * Optimizaciones incluidas:
 * - Potencias de 2: Shift left (sin multiplicación)
 * - Potencias de 3, 5, 7, 10: Combinaciones de shifts y sumas
 * - Números pequeños (2-63): Algoritmos especializados
 * - Template genérico: multiply_by<N>() para cualquier constante
 *
 * Todas las funciones son constexpr y noexcept para uso en compile-time.
 */

// Forward declaration
namespace nstd
{
class uint128_t;
}

namespace uint128_multiply_const_details
{

using uint128_t = nstd::uint128_t;

/**
 * @brief Multiplicación por potencia de 2 mediante shift
 * @param val Valor a multiplicar
 * @param exp Exponente de 2^exp
 * @return val * 2^exp
 */
static constexpr uint128_t multiply_by_power_of_2(const uint128_t& val, int exp) noexcept;

/**
 * @brief Multiplicación optimizada por 3 (x + x + x o shift)
 * @param val Valor a multiplicar
 * @return val * 3
 * @note Usa: x * 3 = (x << 1) + x
 */
static constexpr uint128_t multiply_by_3(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 5
 * @param val Valor a multiplicar
 * @return val * 5
 * @note Usa: x * 5 = (x << 2) + x
 */
static constexpr uint128_t multiply_by_5(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 7
 * @param val Valor a multiplicar
 * @return val * 7
 * @note Usa: x * 7 = (x << 3) - x
 */
static constexpr uint128_t multiply_by_7(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 9
 * @param val Valor a multiplicar
 * @return val * 9
 * @note Usa: x * 9 = (x << 3) + x
 */
static constexpr uint128_t multiply_by_9(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 10
 * @param val Valor a multiplicar
 * @return val * 10
 * @note Usa: x * 10 = ((x << 2) + x) << 1 = (x << 3) + (x << 1)
 */
static constexpr uint128_t multiply_by_10(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 11
 * @param val Valor a multiplicar
 * @return val * 11
 * @note Usa: x * 11 = (x << 3) + (x << 1) + x
 */
static constexpr uint128_t multiply_by_11(const uint128_t& val) noexcept;

/**
 * @brief Multiplicación optimizada por 15
 * @param val Valor a multiplicar
 * @return val * 15
 * @note Usa: x * 15 = (x << 4) - x
 */
static constexpr uint128_t multiply_by_15(const uint128_t& val) noexcept;

// Forward declarations para que compile
static constexpr uint128_t multiply_by_power_of_2(const uint128_t& val, int exp) noexcept
{
    return val.shift_left(exp);
}

static constexpr uint128_t multiply_by_3(const uint128_t& val) noexcept
{
    return val.shift_left(1) + val; // (x << 1) + x
}

static constexpr uint128_t multiply_by_5(const uint128_t& val) noexcept
{
    return val.shift_left(2) + val; // (x << 2) + x
}

static constexpr uint128_t multiply_by_7(const uint128_t& val) noexcept
{
    return val.shift_left(3) - val; // (x << 3) - x
}

static constexpr uint128_t multiply_by_9(const uint128_t& val) noexcept
{
    return val.shift_left(3) + val; // (x << 3) + x
}

static constexpr uint128_t multiply_by_10(const uint128_t& val) noexcept
{
    return val.shift_left(3) + val.shift_left(1); // (x << 3) + (x << 1)
}

static constexpr uint128_t multiply_by_11(const uint128_t& val) noexcept
{
    return val.shift_left(3) + val.shift_left(1) + val; // (x << 3) + (x << 1) + x
}

static constexpr uint128_t multiply_by_15(const uint128_t& val) noexcept
{
    return val.shift_left(4) - val; // (x << 4) - x
}

} // namespace uint128_multiply_const_details

// ============================================================================
// Métodos privados de uint128_t para multiplicación por constantes
// ============================================================================

/**
 * @brief Macro que define métodos privados de multiplicación por constantes
 *
 * Esta macro debe ser incluida en la sección privada de la clase uint128_t.
 */
#define UINT128_MULTIPLY_CONST_PRIVATE_METHODS                                                     \
    /**                                                                                            \
     * @brief Helper para multiplicar por potencia de 2 mediante shift                             \
     * @tparam Exp Exponente de 2^Exp                                                              \
     * @return *this * 2^Exp                                                                       \
     */                                                                                            \
    template <int Exp>                                                                             \
        requires(Exp >= 0 && Exp < 128)                                                            \
    constexpr uint128_t multiply_by_power_of_2_helper() const noexcept                             \
    {                                                                                              \
        return shift_left(Exp);                                                                    \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para multiplicar por potencia de un primo pequeño                            \
     * @tparam Base Base del número (debe ser 2, 3, 5, 7, ..., 61)                                \
     * @tparam Exp Exponente                                                                       \
     * @return *this * Base^Exp                                                                    \
     */                                                                                            \
    template <std::uint64_t Base, int Exp>                                                         \
        requires(Base >= 2 && Base <= 61 && Exp >= 0)                                              \
    constexpr uint128_t multiply_by_power_helper() const noexcept                                  \
    {                                                                                              \
        if constexpr (Exp == 0) {                                                                  \
            return *this;                                                                          \
        } else if constexpr (Exp == 1) {                                                           \
            return multiply_by_const_helper<Base>();                                               \
        } else if constexpr (Base == 2) {                                                          \
            return multiply_by_power_of_2_helper<Exp>();                                           \
        } else {                                                                                   \
            /* Multiplicación iterativa: result = this * Base^Exp */                               \
            constexpr std::uint64_t power = uint128_power_detection::pow<Base, Exp>();             \
            return *this * uint128_t(0, power);                                                    \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper genérico para multiplicar por una constante pequeña                         \
     * @tparam N Constante multiplicadora (debe estar entre 2 y 63)                                \
     * @return *this * N                                                                           \
     */                                                                                            \
    template <std::uint64_t N>                                                                     \
        requires(N >= 2 && N <= 63)                                                                \
    constexpr uint128_t multiply_by_const_helper() const noexcept                                  \
    {                                                                                              \
        /* Optimizaciones para casos específicos */                                                \
        if constexpr (uint128_power_detection::is_power_of_2(N)) {                                 \
            constexpr int exp = uint128_power_detection::log2_uint64(N);                           \
            return multiply_by_power_of_2_helper<exp>();                                           \
        } else if constexpr (N == 3) {                                                             \
            return uint128_multiply_const_details::multiply_by_3(*this);                           \
        } else if constexpr (N == 5) {                                                             \
            return uint128_multiply_const_details::multiply_by_5(*this);                           \
        } else if constexpr (N == 7) {                                                             \
            return uint128_multiply_const_details::multiply_by_7(*this);                           \
        } else if constexpr (N == 9) {                                                             \
            return uint128_multiply_const_details::multiply_by_9(*this);                           \
        } else if constexpr (N == 10) {                                                            \
            return uint128_multiply_const_details::multiply_by_10(*this);                          \
        } else if constexpr (N == 11) {                                                            \
            return uint128_multiply_const_details::multiply_by_11(*this);                          \
        } else if constexpr (N == 15) {                                                            \
            return uint128_multiply_const_details::multiply_by_15(*this);                          \
        } else {                                                                                   \
            /* Multiplicación estándar para otros casos */                                         \
            return *this * uint128_t(0, N);                                                        \
        }                                                                                          \
    }

// ============================================================================
// Métodos públicos de uint128_t para multiplicación por constantes
// ============================================================================

/**
 * @brief Macro que define métodos públicos de multiplicación por constantes
 *
 * Esta macro debe ser incluida en la sección pública de la clase uint128_t.
 */
#define UINT128_MULTIPLY_CONST_PUBLIC_METHODS                                                      \
    /**                                                                                            \
     * @brief Multiplica por una constante conocida en compile-time                                \
     * @tparam N Constante multiplicadora (debe estar entre 2 y 63)                                \
     * @return *this * N                                                                           \
     * @property Es constexpr y noexcept                                                           \
     * @note Aplica optimizaciones específicas para potencias de 2 y números especiales          \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(100);                                                                         \
     * auto result = val.multiply_by<10>();  // 100 * 10 = 1000                                    \
     * assert(result == uint128_t(1000));                                                          \
     *                                                                                             \
     * uint128_t big(0x1000, 0);                                                                   \
     * auto shifted = big.multiply_by<4>();  // Optimizado como shift left 2                       \
     * assert(shifted == uint128_t(0x4000, 0));                                                    \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t N>                                                                     \
        requires(N >= 2 && N <= 63)                                                                \
    constexpr uint128_t multiply_by() const noexcept                                               \
    {                                                                                              \
        return multiply_by_const_helper<N>();                                                      \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Multiplica por una potencia de 2 (2^Exp) mediante shift                              \
     * @tparam Exp Exponente de la potencia de 2                                                   \
     * @return *this * 2^Exp                                                                       \
     * @property Es constexpr y noexcept                                                           \
     * @note Equivalente a shift_left(Exp) pero con semántica de multiplicación                  \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(5);                                                                           \
     * auto result = val.multiply_by_power_of_2<3>();  // 5 * 8 = 40                               \
     * assert(result == uint128_t(40));                                                            \
     * @endcode                                                                                    \
     */                                                                                            \
    template <int Exp>                                                                             \
        requires(Exp >= 0 && Exp < 128)                                                            \
    constexpr uint128_t multiply_by_power_of_2() const noexcept                                    \
    {                                                                                              \
        return multiply_by_power_of_2_helper<Exp>();                                               \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Multiplica por una potencia de un número primo pequeño (Base^Exp)                  \
     * @tparam Base Base del número (debe ser 2, 3, 5, 7, ..., 61)                                \
     * @tparam Exp Exponente                                                                       \
     * @return *this * Base^Exp                                                                    \
     * @property Es constexpr y noexcept                                                           \
     * @note Aplica optimizaciones específicas para Base = 2                                      \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(10);                                                                          \
     * auto result = val.multiply_by_power<3, 2>();  // 10 * 3^2 = 10 * 9 = 90                     \
     * assert(result == uint128_t(90));                                                            \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t Base, int Exp>                                                         \
        requires(Base >= 2 && Base <= 61 && Exp >= 0)                                              \
    constexpr uint128_t multiply_by_power() const noexcept                                         \
    {                                                                                              \
        return multiply_by_power_helper<Base, Exp>();                                              \
    }

#endif // UINT128_MULTIPLY_CONST_HPP
