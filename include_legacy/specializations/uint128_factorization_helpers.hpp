#ifndef UINT128_FACTORIZATION_HELPERS_HPP
#define UINT128_FACTORIZATION_HELPERS_HPP

#include <cstdint>
#include <utility>
#include <vector>

/**
 * @file uint128_factorization_helpers.hpp
 * @brief Helpers para encontrar factores específicos mediante examen rápido del número
 *
 * Este header proporciona métodos para detectar y extraer factores pequeños
 * de un uint128_t mediante tests de divisibilidad rápidos.
 *
 * Funciones principales:
 * - extract_power_of_2(): Extrae la potencia de 2 máxima que divide al número
 * - extract_power_of<N>(): Extrae la potencia de N máxima que divide al número
 * - find_small_factor(): Busca el factor primo más pequeño (≤ 128)
 * - is_prime_candidate(): Test rápido de primalidad para números pequeños
 *
 * Todas las funciones son constexpr y noexcept cuando es posible.
 */

// Forward declaration
namespace nstd
{
class uint128_t;
}

namespace uint128_factorization_details
{

using uint128_t = nstd::uint128_t;

/**
 * @brief Lista de primos menores que 128 (2 a 127)
 * @note Usada para test de divisibilidad rápido
 */
static constexpr std::uint64_t small_primes[] = {2,  3,  5,  7,   11,  13,  17,  19,  23, 29, 31,
                                                 37, 41, 43, 47,  53,  59,  61,  67,  71, 73, 79,
                                                 83, 89, 97, 101, 103, 107, 109, 113, 127};
static constexpr int num_small_primes = sizeof(small_primes) / sizeof(small_primes[0]);

} // namespace uint128_factorization_details

// ============================================================================
// Métodos privados de uint128_t para factorización
// ============================================================================

/**
 * @brief Macro que define métodos privados de factorización para uint128_t
 *
 * Esta macro debe ser incluida en la sección privada de la clase uint128_t.
 */
#define UINT128_FACTORIZATION_PRIVATE_METHODS                                                      \
    /**                                                                                            \
     * @brief Extrae la máxima potencia de 2 que divide al número                                \
     * @return Par {exponente, número/2^exponente}                                                \
     * @note Si el número es 0, retorna {0, 0}                                                    \
     * @note Si el número es impar, retorna {0, número}                                          \
     */                                                                                            \
    constexpr std::pair<int, uint128_t> extract_power_of_2_helper() const noexcept                 \
    {                                                                                              \
        if (*this == uint128_t(0, 0)) {                                                            \
            return {0, uint128_t(0, 0)};                                                           \
        }                                                                                          \
        const int exp = trailing_zeros();                                                          \
        if (exp == 0) {                                                                            \
            return {0, *this};                                                                     \
        }                                                                                          \
        return {exp, shift_right(exp)};                                                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Extrae la máxima potencia de un primo pequeño que divide al número                \
     * @tparam Prime El primo a extraer (debe ser 2, 3, 5, 7, ..., 61)                             \
     * @return Par {exponente, número/Prime^exponente}                                            \
     * @note Si el número no es divisible por Prime, retorna {0, número}                         \
     */                                                                                            \
    template <std::uint64_t Prime>                                                                 \
        requires(Prime >= 2 && Prime <= 61)                                                        \
    constexpr std::pair<int, uint128_t> extract_power_of_prime_helper() const noexcept             \
    {                                                                                              \
        if (*this == uint128_t(0, 0)) {                                                            \
            return {0, uint128_t(0, 0)};                                                           \
        }                                                                                          \
        if constexpr (Prime == 2) {                                                                \
            return extract_power_of_2_helper();                                                    \
        } else {                                                                                   \
            uint128_t quotient = *this;                                                            \
            int exp = 0;                                                                           \
            while (quotient.is_multiple_of<Prime>()) {                                             \
                quotient = quotient / Prime;                                                       \
                ++exp;                                                                             \
            }                                                                                      \
            return {exp, quotient};                                                                \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Busca el factor primo más pequeño (≤ 128) que divide al número                  \
     * @return El factor primo encontrado, o 0 si no hay factor pequeño                           \
     * @note Solo busca entre los primos: 2, 3, 5, 7, 11, ..., 127                                 \
     */                                                                                            \
    constexpr std::uint64_t find_small_prime_factor_helper() const noexcept                        \
    {                                                                                              \
        if (*this <= uint128_t(0, 1)) {                                                            \
            return 0;                                                                              \
        }                                                                                          \
        for (int i = 0; i < uint128_factorization_details::num_small_primes; ++i) {                \
            const std::uint64_t p = uint128_factorization_details::small_primes[i];                \
            if ((*this % p) == uint128_t(0, 0)) {                                                  \
                return p;                                                                          \
            }                                                                                      \
        }                                                                                          \
        return 0;                                                                                  \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Test rápido de primalidad para números pequeños                                   \
     * @return true si el número es probablemente primo, false si es compuesto                    \
     * @note Solo verifica divisibilidad por primos pequeños (≤ 128)                            \
     * @note NO es un test completo de primalidad para números grandes                            \
     */                                                                                            \
    constexpr bool is_prime_candidate_helper() const noexcept                                      \
    {                                                                                              \
        if (*this <= uint128_t(0, 1)) {                                                            \
            return false;                                                                          \
        }                                                                                          \
        if (*this == uint128_t(0, 2)) {                                                            \
            return true;                                                                           \
        }                                                                                          \
        for (int i = 0; i < uint128_factorization_details::num_small_primes; ++i) {                \
            const std::uint64_t p = uint128_factorization_details::small_primes[i];                \
            if (*this == uint128_t(0, p)) {                                                        \
                return true;                                                                       \
            }                                                                                      \
            if ((*this % p) == uint128_t(0, 0)) {                                                  \
                return false;                                                                      \
            }                                                                                      \
        }                                                                                          \
        /* No divisible por primos pequeños, es candidato a primo */                               \
        return true;                                                                               \
    }

// ============================================================================
// Métodos públicos de uint128_t para factorización
// ============================================================================

/**
 * @brief Macro que define métodos públicos de factorización para uint128_t
 *
 * Esta macro debe ser incluida en la sección pública de la clase uint128_t.
 */
#define UINT128_FACTORIZATION_PUBLIC_METHODS                                                       \
    /**                                                                                            \
     * @brief Extrae la máxima potencia de 2 que divide al número                                \
     * @return Par {exponente, número/2^exponente}                                                \
     * @property Es constexpr y noexcept                                                           \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(24);  // 24 = 2^3 * 3                                                         \
     * auto [exp, quotient] = val.extract_power_of_2();                                            \
     * assert(exp == 3);                                                                           \
     * assert(quotient == uint128_t(3));                                                           \
     * @endcode                                                                                    \
     */                                                                                            \
    constexpr std::pair<int, uint128_t> extract_power_of_2() const noexcept                        \
    {                                                                                              \
        return extract_power_of_2_helper();                                                        \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Extrae la máxima potencia de un primo que divide al número                         \
     * @tparam Prime El primo a extraer (debe ser 2, 3, 5, 7, ..., 61)                             \
     * @return Par {exponente, número/Prime^exponente}                                            \
     * @property Es constexpr y noexcept                                                           \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(27);  // 27 = 3^3                                                             \
     * auto [exp, quotient] = val.extract_power_of<3>();                                           \
     * assert(exp == 3);                                                                           \
     * assert(quotient == uint128_t(1));                                                           \
     * @endcode                                                                                    \
     */                                                                                            \
    template <std::uint64_t Prime>                                                                 \
        requires(Prime >= 2 && Prime <= 61)                                                        \
    constexpr std::pair<int, uint128_t> extract_power_of() const noexcept                          \
    {                                                                                              \
        return extract_power_of_prime_helper<Prime>();                                             \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Busca el factor primo más pequeño (≤ 128) que divide al número                  \
     * @return El factor primo encontrado, o 0 si no hay factor pequeño                           \
     * @property Es constexpr y noexcept                                                           \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * uint128_t val(15);                                                                          \
     * auto factor = val.find_small_prime_factor();                                                \
     * assert(factor == 3);  // 15 = 3 * 5, el menor es 3                                          \
     * @endcode                                                                                    \
     */                                                                                            \
    constexpr std::uint64_t find_small_prime_factor() const noexcept                               \
    {                                                                                              \
        return find_small_prime_factor_helper();                                                   \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * @brief Test rápido de primalidad para números pequeños                                   \
     * @return true si el número es probablemente primo, false si es compuesto                    \
     * @property Es constexpr y noexcept                                                           \
     * @warning NO es un test completo de primalidad para números grandes                         \
     * @note Solo verifica divisibilidad por primos pequeños (≤ 128)                            \
     * @example                                                                                    \
     * @code{.cpp}                                                                                 \
     * assert(uint128_t(17).is_prime_candidate());   // true                                       \
     * assert(!uint128_t(15).is_prime_candidate());  // false (15 = 3*5)                           \
     * @endcode                                                                                    \
     */                                                                                            \
    constexpr bool is_prime_candidate() const noexcept                                             \
    {                                                                                              \
        return is_prime_candidate_helper();                                                        \
    }

#endif // UINT128_FACTORIZATION_HELPERS_HPP
