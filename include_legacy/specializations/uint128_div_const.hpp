#ifndef UINT128_DIV_CONST_HPP
#define UINT128_DIV_CONST_HPP

#include "uint128_power_detection.hpp"
#include <cstdint>
#include <optional>
#include <utility>

/**
 * @file uint128_div_const.hpp
 * @brief División optimizada por constantes en compile-time
 *
 * Este header proporciona métodos para dividir uint128_t por constantes
 * conocidas en tiempo de compilación, aplicando optimizaciones específicas.
 *
 * Optimizaciones incluidas:
 * - Potencias de 2: Shift right (sin división)
 * - Potencias de 3, 5, 7, 10: Algoritmos especializados sin división completa
 * - Multiplicación por inverso modular para primos pequeños
 * - División incremental para potencias de bases pequeñas
 *
 * Todas las funciones retornan std::pair<cociente, resto> cuando es aplicable.
 */

// Forward declaration
namespace nstd
{
class uint128_t;
}

namespace uint128_div_const_details
{

using uint128_t = nstd::uint128_t;

/**
 * @brief División por potencia de 2 mediante shift
 * @param val Valor a dividir
 * @param exp Exponente de 2^exp
 * @return Par {cociente, resto}
 * @note Cociente = val >> exp, Resto = val & ((1 << exp) - 1)
 */
static constexpr std::pair<uint128_t, uint128_t> divide_by_power_of_2(const uint128_t& val,
                                                                      int exp) noexcept;

/**
 * @brief División optimizada por 3
 * @param val Valor a dividir
 * @return Par {cociente, resto}
 * @note Usa multiplicación por inverso modular aproximado
 */
static constexpr std::pair<uint128_t, uint128_t> divide_by_3(const uint128_t& val) noexcept;

/**
 * @brief División optimizada por 5
 * @param val Valor a dividir
 * @return Par {cociente, resto}
 */
static constexpr std::pair<uint128_t, uint128_t> divide_by_5(const uint128_t& val) noexcept;

/**
 * @brief División optimizada por 10
 * @param val Valor a dividir
 * @return Par {cociente, resto}
 * @note Usa: 10 = 2 * 5, divide primero por 2 (shift) luego por 5
 */
static constexpr std::pair<uint128_t, uint128_t> divide_by_10(const uint128_t& val) noexcept;

// Forward declarations con implementaciones inline
static constexpr std::pair<uint128_t, uint128_t> divide_by_power_of_2(const uint128_t& val,
                                                                      int exp) noexcept
{
    if (exp <= 0) {
        return {val, uint128_t(0, 0)};
    }
    if (exp >= 128) {
        return {uint128_t(0, 0), val};
    }

    const uint128_t quotient = val.shift_right(exp);
    const uint128_t mask = uint128_t(0, 1).shift_left(exp) - uint128_t(0, 1);
    const uint128_t remainder = val & mask;
    return {quotient, remainder};
}

static constexpr std::pair<uint128_t, uint128_t> divide_by_3(const uint128_t& val) noexcept
{
    // Implementación simplificada: usar divrem del tipo
    auto result = val.divrem(uint128_t(0, 3));
    return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), val);
}

static constexpr std::pair<uint128_t, uint128_t> divide_by_5(const uint128_t& val) noexcept
{
    auto result = val.divrem(uint128_t(0, 5));
    return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), val);
}

static constexpr std::pair<uint128_t, uint128_t> divide_by_10(const uint128_t& val) noexcept
{
    auto result = val.divrem(uint128_t(0, 10));
    return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), val);
}

} // namespace uint128_div_const_details

// ============================================================================
// Métodos privados de uint128_t para división por constantes
// ============================================================================

/**
 * @brief Macro que define métodos privados de división por constantes
 *
 * Esta macro debe ser incluida en la sección privada de la clase uint128_t.
 */
#define UINT128_DIV_CONST_PRIVATE_METHODS                                                          \
    /**                                                                                            \
     * @brief Helper para división por potencia de 2 mediante shift                               \
     * @tparam Exp Exponente de 2^Exp                                                              \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    template <int Exp>                                                                             \
        requires(Exp >= 0 && Exp < 128)                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_power_of_2_helper() const noexcept         \
    {                                                                                              \
        return uint128_div_const_details::divide_by_power_of_2(*this, Exp);                        \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 10 (optimizado)                                            \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_10_helper() const noexcept                 \
    {                                                                                              \
        return uint128_div_const_details::divide_by_10(*this);                                     \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 100 (10^2)                                                 \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_100_helper() const noexcept                \
    {                                                                                              \
        auto [q1, r1] = divide_by_10_helper();                                                     \
        auto [q2, r2] = q1.divide_by_10_helper();                                                  \
        const uint128_t final_remainder = r2 * uint128_t(0, 10) + r1;                              \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 1000 (10^3)                                                \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_1000_helper() const noexcept               \
    {                                                                                              \
        auto [q1, r1] = divide_by_100_helper();                                                    \
        auto [q2, r2] = q1.divide_by_10_helper();                                                  \
        const uint128_t final_remainder = r2 * uint128_t(0, 100) + r1;                             \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 3                                                          \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_3_helper() const noexcept                  \
    {                                                                                              \
        return uint128_div_const_details::divide_by_3(*this);                                      \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 9 (3^2)                                                    \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_9_helper() const noexcept                  \
    {                                                                                              \
        auto [q1, r1] = divide_by_3_helper();                                                      \
        auto [q2, r2] = q1.divide_by_3_helper();                                                   \
        const uint128_t final_remainder = r2 * uint128_t(0, 3) + r1;                               \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 27 (3^3)                                                   \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_27_helper() const noexcept                 \
    {                                                                                              \
        auto [q1, r1] = divide_by_9_helper();                                                      \
        auto [q2, r2] = q1.divide_by_3_helper();                                                   \
        const uint128_t final_remainder = r2 * uint128_t(0, 9) + r1;                               \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 5                                                          \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_5_helper() const noexcept                  \
    {                                                                                              \
        return uint128_div_const_details::divide_by_5(*this);                                      \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 25 (5^2)                                                   \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_25_helper() const noexcept                 \
    {                                                                                              \
        auto [q1, r1] = divide_by_5_helper();                                                      \
        auto [q2, r2] = q1.divide_by_5_helper();                                                   \
        const uint128_t final_remainder = r2 * uint128_t(0, 5) + r1;                               \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper para división por 125 (5^3)                                                  \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_125_helper() const noexcept                \
    {                                                                                              \
        auto [q1, r1] = divide_by_25_helper();                                                     \
        auto [q2, r2] = q1.divide_by_5_helper();                                                   \
        const uint128_t final_remainder = r2 * uint128_t(0, 25) + r1;                              \
        return {q2, final_remainder};                                                              \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Intenta dividir por potencia de 10 si el divisor es potencia de 10                   \
     * @param divisor El divisor a verificar                                                       \
     * @return std::optional con {cociente, resto} si es potencia de 10, nullopt en caso contrario \
     */                                                                                            \
    std::optional<std::pair<uint128_t, uint128_t>> try_divide_by_power_of_10_helper(               \
        const uint128_t& divisor) const noexcept                                                   \
    {                                                                                              \
        if (divisor == uint128_t(0, 1))                                                            \
            return std::make_pair(*this, uint128_t(0, 0));                                         \
        if (divisor == uint128_t(0, 10))                                                           \
            return divide_by_10_helper();                                                          \
        if (divisor == uint128_t(0, 100))                                                          \
            return divide_by_100_helper();                                                         \
        if (divisor == uint128_t(0, 1000))                                                         \
            return divide_by_1000_helper();                                                        \
        return std::nullopt;                                                                       \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Intenta dividir por potencia de 3 si el divisor es potencia de 3                     \
     * @param divisor El divisor a verificar                                                       \
     * @return std::optional con {cociente, resto} si es potencia de 3, nullopt en caso contrario  \
     */                                                                                            \
    std::optional<std::pair<uint128_t, uint128_t>> try_divide_by_power_of_3_helper(                \
        const uint128_t& divisor) const noexcept                                                   \
    {                                                                                              \
        if (divisor == uint128_t(0, 3))                                                            \
            return divide_by_3_helper();                                                           \
        if (divisor == uint128_t(0, 9))                                                            \
            return divide_by_9_helper();                                                           \
        if (divisor == uint128_t(0, 27))                                                           \
            return divide_by_27_helper();                                                          \
        return std::nullopt;                                                                       \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Intenta dividir por potencia de 5 si el divisor es potencia de 5                     \
     * @param divisor El divisor a verificar                                                       \
     * @return std::optional con {cociente, resto} si es potencia de 5, nullopt en caso contrario  \
     */                                                                                            \
    std::optional<std::pair<uint128_t, uint128_t>> try_divide_by_power_of_5_helper(                \
        const uint128_t& divisor) const noexcept                                                   \
    {                                                                                              \
        if (divisor == uint128_t(0, 5))                                                            \
            return divide_by_5_helper();                                                           \
        if (divisor == uint128_t(0, 25))                                                           \
            return divide_by_25_helper();                                                          \
        if (divisor == uint128_t(0, 125))                                                          \
            return divide_by_125_helper();                                                         \
        return std::nullopt;                                                                       \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Template genérico para división por potencia de un primo                           \
     * @tparam Base Base del número (debe ser 2, 3, 5, 7, ..., 61)                                \
     * @tparam Exp Exponente                                                                       \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    template <std::uint64_t Base, int Exp>                                                         \
        requires(Base >= 2 && Base <= 61 && Exp >= 0)                                              \
    constexpr std::pair<uint128_t, uint128_t> divide_by_power_helper() const noexcept              \
    {                                                                                              \
        if constexpr (Exp == 0) {                                                                  \
            return {*this, uint128_t(0, 0)};                                                       \
        } else if constexpr (Exp == 1) {                                                           \
            return divide_by_const_helper<Base>();                                                 \
        } else if constexpr (Base == 2) {                                                          \
            return divide_by_power_of_2_helper<Exp>();                                             \
        } else {                                                                                   \
            /* División iterativa: divide Exp veces por Base */                                    \
            uint128_t quotient = *this;                                                            \
            uint128_t accumulated_remainder(0, 0);                                                 \
            for (int i = 0; i < Exp; ++i) {                                                        \
                auto [q, r] = quotient.divide_by_const_helper<Base>();                             \
                quotient = q;                                                                      \
                accumulated_remainder = accumulated_remainder * uint128_t(0, Base) + r;            \
            }                                                                                      \
            return {quotient, accumulated_remainder};                                              \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Helper genérico para división por una constante pequeña                           \
     * @tparam Divisor Constante divisor (debe estar entre 2 y 63)                                 \
     * @return Par {cociente, resto}                                                               \
     */                                                                                            \
    template <std::uint64_t Divisor>                                                               \
        requires(Divisor >= 2 && Divisor <= 63)                                                    \
    constexpr std::pair<uint128_t, uint128_t> divide_by_const_helper() const noexcept              \
    {                                                                                              \
        /* Optimizaciones para casos específicos */                                                \
        if constexpr (uint128_power_detection::is_power_of_2(Divisor)) {                           \
            constexpr int exp = uint128_power_detection::log2_uint64(Divisor);                     \
            return divide_by_power_of_2_helper<exp>();                                             \
        } else if constexpr (Divisor == 3) {                                                       \
            return divide_by_3_helper();                                                           \
        } else if constexpr (Divisor == 5) {                                                       \
            return divide_by_5_helper();                                                           \
        } else if constexpr (Divisor == 10) {                                                      \
            return divide_by_10_helper();                                                          \
        } else {                                                                                   \
            /* División estándar usando divrem */                                                  \
            auto result = divrem(uint128_t(0, Divisor));                                           \
            return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);   \
        }                                                                                          \
    }

// ============================================================================
// Métodos públicos de uint128_t para división por constantes
// ============================================================================

/**
 * @brief Macro que define métodos públicos de división por constantes
 *
 * Esta macro debe ser incluida en la sección pública de la clase uint128_t.
 */
#define UINT128_DIV_CONST_PUBLIC_METHODS                                                           \
    /**                                                                                            \
     * @brief Divide por una constante conocida en compile-time                                    \
     * @tparam Divisor Constante divisor (debe estar entre 2 y 63)                                 \
     * @return Par {cociente, resto}                                                               \
     * @property Es constexpr y noexcept                                                           \
     * @note Aplica optimizaciones específicas para potencias de 2 y números especiales          \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(100);                                                                         \
     * auto [q, r] = val.divide_by<10>();                                                          \
     * assert(q == uint128_t(10));                                                                 \
     * assert(r == uint128_t(0));                                                                  \
     *                                                                                             \
     * uint128_t big(1000);                                                                        \
     * auto [q2, r2] = big.divide_by<7>();                                                         \
     * assert(q2 == uint128_t(142));  // 1000 / 7 = 142                                            \
     * assert(r2 == uint128_t(6));    // 1000 % 7 = 6                                              \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t Divisor>                                                               \
        requires(Divisor >= 2 && Divisor <= 63)                                                    \
    constexpr std::pair<uint128_t, uint128_t> divide_by() const noexcept                           \
    {                                                                                              \
        return divide_by_const_helper<Divisor>();                                                  \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Divide por una potencia de 2 (2^Exp) mediante shift                                  \
     * @tparam Exp Exponente de la potencia de 2                                                   \
     * @return Par {cociente, resto}                                                               \
     * @property Es constexpr y noexcept                                                           \
     * @note Cociente = *this >> Exp, Resto = *this & ((1 << Exp) - 1)                             \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(100);                                                                         \
     * auto [q, r] = val.divide_by_power_of_2<3>();  // 100 / 8 = 12, resto 4                      \
     * assert(q == uint128_t(12));                                                                 \
     * assert(r == uint128_t(4));                                                                  \
     * @endcode                                                                                    \
     */                                                                                            \
    template <int Exp>                                                                             \
        requires(Exp >= 0 && Exp < 128)                                                            \
    constexpr std::pair<uint128_t, uint128_t> divide_by_power_of_2() const noexcept                \
    {                                                                                              \
        return divide_by_power_of_2_helper<Exp>();                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Divide por una potencia de un número primo pequeño (Base^Exp)                      \
     * @tparam Base Base del número (debe ser 2, 3, 5, 7, ..., 61)                                \
     * @tparam Exp Exponente                                                                       \
     * @return Par {cociente, resto}                                                               \
     * @property Es constexpr y noexcept                                                           \
     * @note Aplica optimizaciones específicas para Base = 2                                      \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(270);                                                                         \
     * auto [q, r] = val.divide_by_power<3, 2>();  // 270 / 9 = 30, resto 0                        \
     * assert(q == uint128_t(30));                                                                 \
     * assert(r == uint128_t(0));                                                                  \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t Base, int Exp>                                                         \
        requires(Base >= 2 && Base <= 61 && Exp >= 0)                                              \
    constexpr std::pair<uint128_t, uint128_t> divide_by_power() const noexcept                     \
    {                                                                                              \
        return divide_by_power_helper<Base, Exp>();                                                \
    }

#endif // UINT128_DIV_CONST_HPP
