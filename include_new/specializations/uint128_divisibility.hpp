#ifndef UINT128_DIVISIBILITY_HPP
#define UINT128_DIVISIBILITY_HPP

#include <cstdint>

/**
 * @file uint128_divisibility.hpp
 * @brief Métodos para detectar si un uint128_t es múltiplo de una constante en compile-time
 *
 * Este header proporciona funciones constexpr para detectar si un número uint128_t
 * es divisible por constantes conocidas en tiempo de compilación.
 *
 * Funciones principales:
 * - is_multiple_of<N>(): Template genérico para detectar divisibilidad por N
 * - is_even(), is_odd(): Detecta paridad
 * - is_multiple_of_3(), is_multiple_of_5(), etc.: Especializaciones optimizadas
 *
 * Todas las funciones son métodos constexpr y noexcept de uint128_t.
 */

// Forward declaration
namespace nstd
{
class uint128_t;
}

namespace uint128_divisibility_details
{

using uint128_t = nstd::uint128_t;

/**
 * @brief Helper para calcular 2^64 mod M en tiempo de compilación
 * @param M El módulo
 * @return 2^64 mod M
 * @note Usado para reducción modular de uint128_t
 */
static constexpr std::uint64_t compute_2_64_mod(std::uint64_t M) noexcept
{
    std::uint64_t result = 1;
    std::uint64_t base = 2 % M;
    int exp = 64;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % M;
        }
        base = (base * base) % M;
        exp >>= 1;
    }
    return result;
}

} // namespace uint128_divisibility_details

// ============================================================================
// Métodos privados de uint128_t para detección de divisibilidad
// ============================================================================

/**
 * @brief Macro que define métodos privados de divisibilidad para uint128_t
 *
 * Esta macro debe ser incluida en la sección privada de la clase uint128_t.
 * Define todos los métodos helper para detectar divisibilidad.
 */
#define UINT128_DIVISIBILITY_PRIVATE_METHODS                                                       \
    /**                                                                                            \
     * @brief Verifica si el número es par                                                        \
     * @return true si el número es par, false si es impar                                        \
     */                                                                                            \
    constexpr bool is_even_helper() const noexcept                                                 \
    {                                                                                              \
        return (data[0] & 1) == 0;                                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es impar                                                      \
     * @return true si el número es impar, false si es par                                        \
     */                                                                                            \
    constexpr bool is_odd_helper() const noexcept                                                  \
    {                                                                                              \
        return (data[0] & 1) == 1;                                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es múltiplo de 3                                             \
     * @return true si es múltiplo de 3, false en caso contrario                                  \
     * @note Usa reducción modular: (high * (2^64 mod 3) + low) mod 3                             \
     */                                                                                            \
    constexpr bool is_multiple_of_3_helper() const noexcept                                        \
    {                                                                                              \
        constexpr std::uint64_t pow2_64_mod_3 = uint128_divisibility_details::compute_2_64_mod(3); \
        const std::uint64_t reduced = (data[1] % 3) * pow2_64_mod_3 + (data[0] % 3);               \
        return (reduced % 3) == 0;                                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es múltiplo de 5                                             \
     * @return true si es múltiplo de 5, false en caso contrario                                  \
     * @note Usa reducción modular: (high * (2^64 mod 5) + low) mod 5                             \
     */                                                                                            \
    constexpr bool is_multiple_of_5_helper() const noexcept                                        \
    {                                                                                              \
        constexpr std::uint64_t pow2_64_mod_5 = uint128_divisibility_details::compute_2_64_mod(5); \
        const std::uint64_t reduced = (data[1] % 5) * pow2_64_mod_5 + (data[0] % 5);               \
        return (reduced % 5) == 0;                                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es múltiplo de 7                                             \
     * @return true si es múltiplo de 7, false en caso contrario                                  \
     */                                                                                            \
    constexpr bool is_multiple_of_7_helper() const noexcept                                        \
    {                                                                                              \
        constexpr std::uint64_t pow2_64_mod_7 = uint128_divisibility_details::compute_2_64_mod(7); \
        const std::uint64_t reduced = (data[1] % 7) * pow2_64_mod_7 + (data[0] % 7);               \
        return (reduced % 7) == 0;                                                                 \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es múltiplo de 10                                            \
     * @return true si es múltiplo de 10, false en caso contrario                                 \
     * @note Optimización: múltiplo de 10 ≡ múltiplo de 2 Y múltiplo de 5                    \
     */                                                                                            \
    constexpr bool is_multiple_of_10_helper() const noexcept                                       \
    {                                                                                              \
        return is_even_helper() && is_multiple_of_5_helper();                                      \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Template genérico para verificar divisibilidad por un divisor constante             \
     * @tparam Divisor El divisor a verificar (debe estar entre 2 y 63)                            \
     * @return true si es múltiplo de Divisor, false en caso contrario                            \
     */                                                                                            \
    template <std::uint64_t Divisor>                                                               \
        requires(Divisor >= 2 && Divisor <= 63)                                                    \
    constexpr bool is_multiple_of_helper() const noexcept                                          \
    {                                                                                              \
        if constexpr (Divisor == 2) {                                                              \
            return is_even_helper();                                                               \
        } else if constexpr (Divisor == 3) {                                                       \
            return is_multiple_of_3_helper();                                                      \
        } else if constexpr (Divisor == 5) {                                                       \
            return is_multiple_of_5_helper();                                                      \
        } else if constexpr (Divisor == 7) {                                                       \
            return is_multiple_of_7_helper();                                                      \
        } else if constexpr (Divisor == 10) {                                                      \
            return is_multiple_of_10_helper();                                                     \
        } else {                                                                                   \
            /* Método genérico usando reducción modular */                                         \
            constexpr std::uint64_t pow2_64_mod_D =                                                \
                uint128_divisibility_details::compute_2_64_mod(Divisor);                           \
            const std::uint64_t reduced =                                                          \
                (data[1] % Divisor) * pow2_64_mod_D + (data[0] % Divisor);                         \
            return (reduced % Divisor) == 0;                                                       \
        }                                                                                          \
    }

// ============================================================================
// Métodos públicos de uint128_t para detección de divisibilidad
// ============================================================================

/**
 * @brief Macro que define métodos públicos de divisibilidad para uint128_t
 *
 * Esta macro debe ser incluida en la sección pública de la clase uint128_t.
 * Define la API pública para detectar divisibilidad.
 */
#define UINT128_DIVISIBILITY_PUBLIC_METHODS                                                        \
    /**                                                                                            \
     * @brief Verifica si el número es par                                                        \
     * @return true si el número es par, false si es impar                                        \
     * @property Es constexpr y noexcept                                                           \
     */                                                                                            \
    constexpr bool is_even() const noexcept                                                        \
    {                                                                                              \
        return is_even_helper();                                                                   \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es impar                                                      \
     * @return true si el número es impar, false si es par                                        \
     * @property Es constexpr y noexcept                                                           \
     */                                                                                            \
    constexpr bool is_odd() const noexcept                                                         \
    {                                                                                              \
        return is_odd_helper();                                                                    \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Verifica si el número es múltiplo de un divisor constante                          \
     * @tparam Divisor El divisor a verificar (debe estar entre 2 y 63)                            \
     * @return true si es múltiplo de Divisor, false en caso contrario                            \
     * @property Es constexpr y noexcept                                                           \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(60);                                                                          \
     * assert(val.is_multiple_of<2>());  // true: 60 es par                                        \
     * assert(val.is_multiple_of<3>());  // true: 60 = 3 * 20                                      \
     * assert(val.is_multiple_of<5>());  // true: 60 = 5 * 12                                      \
     * assert(!val.is_multiple_of<7>()); // false: 60 no es múltiplo de 7                         \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t Divisor>                                                               \
        requires(Divisor >= 2 && Divisor <= 63)                                                    \
    constexpr bool is_multiple_of() const noexcept                                                 \
    {                                                                                              \
        return is_multiple_of_helper<Divisor>();                                                   \
    }

#endif // UINT128_DIVISIBILITY_HPP
