#ifndef UINT128_MOD_HELPERS_HPP
#define UINT128_MOD_HELPERS_HPP

#include <cstdint>

// Forward declaration
namespace nstd
{
class uint128_t;
}

/**
 * @file uint128_mod_helpers.hpp
 * @brief Helpers y especializaciones para operaciones de módulo optimizadas en uint128_t
 *
 * Este archivo contiene todas las funciones auxiliares y especializaciones
 * para calcular módulos de forma eficiente sin usar divisiones completas de 128 bits.
 *
 * Optimizaciones incluidas:
 * - Potencias de 2: Máscaras de bits
 * - Potencias de 3, 5, 7, 10: Reducción modular genérica
 * - Primos pequeños (3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61)
 * - Compuestos comunes (6, 9, 10, 12, 14, 15, 18, 20)
 */

namespace uint128_mod_details
{

using uint128_t = nstd::uint128_t;
using int128_t = nstd::int128_t;

// ============================================================================
// Funciones auxiliares para detección de patrones
// ============================================================================

static constexpr bool is_power_of_2(std::uint64_t n) noexcept
{
    return n > 0 && (n & (n - 1)) == 0;
}

static constexpr int log2_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n >>= 1;
        ++result;
    }
    return result;
}

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

static constexpr int log3_uint64(std::uint64_t n) noexcept
{
    int result = 0;
    while (n > 1) {
        n /= 3;
        ++result;
    }
    return result;
}

// ============================================================================
// Funciones para calcular potencias en tiempo de compilación
// ============================================================================

static constexpr std::uint64_t pow3(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 3;
    }
    return result;
}

static constexpr std::uint64_t pow5(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 5;
    }
    return result;
}

static constexpr std::uint64_t pow7(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 7;
    }
    return result;
}

static constexpr std::uint64_t pow10(int exp) noexcept
{
    std::uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}

// ============================================================================
// Helper genérico para calcular 2^64 mod M en tiempo de compilación
// ============================================================================

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

} // namespace uint128_mod_details

// ============================================================================
// Extensión de la clase uint128_t con helpers privados
// ============================================================================

// Este archivo debe ser incluido dentro de la definición de la clase uint128_t
// en la sección privada. Los siguientes métodos son helpers privados.

#define UINT128_MOD_HELPERS_PRIVATE_METHODS                                                        \
    /* Optimización genérica para mod 2^k -> máscara de bits */                                    \
    template <std::uint64_t PowerOf2>                                                              \
    constexpr std::uint64_t mod_power_of_2_helper() const noexcept                                 \
    {                                                                                              \
        static_assert(uint128_mod_details::is_power_of_2(PowerOf2), "Must be a power of 2");       \
        constexpr int bits = uint128_mod_details::log2_uint64(PowerOf2);                           \
        constexpr std::uint64_t mask = PowerOf2 - 1;                                               \
        if constexpr (bits < 64) {                                                                 \
            return low() & mask;                                                                   \
        } else {                                                                                   \
            return low();                                                                          \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    /* Optimización genérica para mod 3^k */                                                       \
    template <std::uint64_t PowerOf3>                                                              \
    constexpr std::uint64_t mod_power_of_3_helper() const noexcept                                 \
    {                                                                                              \
        static_assert(uint128_mod_details::is_power_of_3(PowerOf3), "Must be a power of 3");       \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = h % PowerOf3;                                                                          \
        l = l % PowerOf3;                                                                          \
        constexpr std::uint64_t mod_2_64 = []() constexpr {                                        \
            std::uint64_t result = 1;                                                              \
            std::uint64_t base = 2 % PowerOf3;                                                     \
            int exp = 64;                                                                          \
            while (exp > 0) {                                                                      \
                if (exp & 1) {                                                                     \
                    result = (result * base) % PowerOf3;                                           \
                }                                                                                  \
                base = (base * base) % PowerOf3;                                                   \
                exp >>= 1;                                                                         \
            }                                                                                      \
            return result;                                                                         \
        }();                                                                                       \
        std::uint64_t result = (h * mod_2_64 + l) % PowerOf3;                                      \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización genérica para mod 5^k */                                                       \
    template <std::uint64_t PowerOf5>                                                              \
    constexpr std::uint64_t mod_power_of_5_helper() const noexcept                                 \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = h % PowerOf5;                                                                          \
        l = l % PowerOf5;                                                                          \
        constexpr std::uint64_t mod_2_64 = uint128_mod_details::compute_2_64_mod(PowerOf5);        \
        std::uint64_t result = (h * mod_2_64 + l) % PowerOf5;                                      \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización genérica para mod 7^k */                                                       \
    template <std::uint64_t PowerOf7>                                                              \
    constexpr std::uint64_t mod_power_of_7_helper() const noexcept                                 \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = h % PowerOf7;                                                                          \
        l = l % PowerOf7;                                                                          \
        constexpr std::uint64_t mod_2_64 = uint128_mod_details::compute_2_64_mod(PowerOf7);        \
        std::uint64_t result = (h * mod_2_64 + l) % PowerOf7;                                      \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización genérica para mod 10^k */                                                      \
    template <std::uint64_t PowerOf10>                                                             \
    constexpr std::uint64_t mod_power_of_10_helper() const noexcept                                \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = h % PowerOf10;                                                                         \
        l = l % PowerOf10;                                                                         \
        constexpr std::uint64_t mod_2_64 = uint128_mod_details::compute_2_64_mod(PowerOf10);       \
        std::uint64_t result = (h * mod_2_64 + l) % PowerOf10;                                     \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 3 */                                                                  \
    constexpr std::uint64_t mod3_helper() const noexcept                                           \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = (h >> 32) + (h & 0xFFFFFFFF);                                                          \
        h = (h >> 16) + (h & 0xFFFF);                                                              \
        h = (h >> 8) + (h & 0xFF);                                                                 \
        h = (h >> 4) + (h & 0xF);                                                                  \
        l = (l >> 32) + (l & 0xFFFFFFFF);                                                          \
        l = (l >> 16) + (l & 0xFFFF);                                                              \
        l = (l >> 8) + (l & 0xFF);                                                                 \
        l = (l >> 4) + (l & 0xF);                                                                  \
        std::uint64_t sum = h + l;                                                                 \
        while (sum >= 3) {                                                                         \
            sum = (sum >> 2) + (sum & 3);                                                          \
        }                                                                                          \
        return sum;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 5 */                                                                  \
    constexpr std::uint64_t mod5_helper() const noexcept                                           \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        h = (h >> 32) + (h & 0xFFFFFFFF);                                                          \
        h = (h >> 16) + (h & 0xFFFF);                                                              \
        l = (l >> 32) + (l & 0xFFFFFFFF);                                                          \
        l = (l >> 16) + (l & 0xFFFF);                                                              \
        std::uint64_t sum = h + l;                                                                 \
        while (sum >= 5) {                                                                         \
            sum = (sum >> 4) * 1 + (sum & 0xF);                                                    \
            if (sum >= 5)                                                                          \
                sum -= 5;                                                                          \
        }                                                                                          \
        return sum;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 6 */                                                                  \
    constexpr std::uint64_t mod6_helper() const noexcept                                           \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 6) * 4 + (l % 6)) % 6;                                        \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 7 */                                                                  \
    constexpr std::uint64_t mod7_helper() const noexcept                                           \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 3) {                                                          \
            result += (l >> i) & 0x7;                                                              \
            result += (h >> i) & 0x7;                                                              \
        }                                                                                          \
        while (result >= 7) {                                                                      \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 3) {                                                      \
                temp += (result >> i) & 0x7;                                                       \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 9 */                                                                  \
    constexpr std::uint64_t mod9_helper() const noexcept                                           \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 3) {                                                          \
            result += (l >> i) & 0x7;                                                              \
            result += (h >> i) & 0x7;                                                              \
        }                                                                                          \
        while (result >= 9) {                                                                      \
            result = (result >> 3) + (result & 0x7);                                               \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 10 */                                                                 \
    constexpr std::uint64_t mod10_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 10) * 6 + (l % 10)) % 10;                                     \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 11 */                                                                 \
    constexpr std::uint64_t mod11_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 10) {                                                         \
            result += (l >> i) & 0x3FF;                                                            \
            result += (h >> i) & 0x3FF;                                                            \
        }                                                                                          \
        while (result >= 11) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 10) {                                                     \
                temp += (result >> i) & 0x3FF;                                                     \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 12 */                                                                 \
    constexpr std::uint64_t mod12_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 12) * 4 + (l % 12)) % 12;                                     \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 13 */                                                                 \
    constexpr std::uint64_t mod13_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 12) {                                                         \
            result += (l >> i) & 0xFFF;                                                            \
            result += (h >> i) & 0xFFF;                                                            \
        }                                                                                          \
        while (result >= 13) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 12) {                                                     \
                temp += (result >> i) & 0xFFF;                                                     \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 14 */                                                                 \
    constexpr std::uint64_t mod14_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 14) * 4 + (l % 14)) % 14;                                     \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 15 */                                                                 \
    constexpr std::uint64_t mod15_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 4) {                                                          \
            result += (l >> i) & 0xF;                                                              \
            result += (h >> i) & 0xF;                                                              \
        }                                                                                          \
        while (result >= 15) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 4) {                                                      \
                temp += (result >> i) & 0xF;                                                       \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 17 */                                                                 \
    constexpr std::uint64_t mod17_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 8) {                                                          \
            result += (l >> i) & 0xFF;                                                             \
            result += (h >> i) & 0xFF;                                                             \
        }                                                                                          \
        while (result >= 17) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 8) {                                                      \
                temp += (result >> i) & 0xFF;                                                      \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 18 */                                                                 \
    constexpr std::uint64_t mod18_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 18) * 10 + (l % 18)) % 18;                                    \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 19 */                                                                 \
    constexpr std::uint64_t mod19_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 18) {                                                         \
            result += (l >> i) & 0x3FFFF;                                                          \
            result += (h >> i) & 0x3FFFF;                                                          \
        }                                                                                          \
        while (result >= 19) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 18) {                                                     \
                temp += (result >> i) & 0x3FFFF;                                                   \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 20 */                                                                 \
    constexpr std::uint64_t mod20_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        std::uint64_t result = ((h % 20) * 16 + (l % 20)) % 20;                                    \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 23 */                                                                 \
    constexpr std::uint64_t mod23_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 11) {                                                         \
            result += (l >> i) & 0x7FF;                                                            \
            result += (h >> i) & 0x7FF;                                                            \
        }                                                                                          \
        while (result >= 23) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 11) {                                                     \
                temp += (result >> i) & 0x7FF;                                                     \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 29 */                                                                 \
    constexpr std::uint64_t mod29_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 28) {                                                         \
            result += (l >> i) & 0xFFFFFFF;                                                        \
            result += (h >> i) & 0xFFFFFFF;                                                        \
        }                                                                                          \
        while (result >= 29) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 28) {                                                     \
                temp += (result >> i) & 0xFFFFFFF;                                                 \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 31 */                                                                 \
    constexpr std::uint64_t mod31_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 5) {                                                          \
            result += (l >> i) & 0x1F;                                                             \
            result += (h >> i) & 0x1F;                                                             \
        }                                                                                          \
        while (result >= 31) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 5) {                                                      \
                temp += (result >> i) & 0x1F;                                                      \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 37 */                                                                 \
    constexpr std::uint64_t mod37_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 12) {                                                         \
            result += (l >> i) & 0xFFF;                                                            \
            result += (h >> i) & 0xFFF;                                                            \
        }                                                                                          \
        while (result >= 37) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 12) {                                                     \
                temp += (result >> i) & 0xFFF;                                                     \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 41 */                                                                 \
    constexpr std::uint64_t mod41_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 20) {                                                         \
            result += (l >> i) & 0xFFFFF;                                                          \
            result += (h >> i) & 0xFFFFF;                                                          \
        }                                                                                          \
        while (result >= 41) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 20) {                                                     \
                temp += (result >> i) & 0xFFFFF;                                                   \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 43 */                                                                 \
    constexpr std::uint64_t mod43_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 14) {                                                         \
            result += (l >> i) & 0x3FFF;                                                           \
            result += (h >> i) & 0x3FFF;                                                           \
        }                                                                                          \
        while (result >= 43) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 14) {                                                     \
                temp += (result >> i) & 0x3FFF;                                                    \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 47 */                                                                 \
    constexpr std::uint64_t mod47_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        for (int i = 0; i < 64; i += 23) {                                                         \
            result += (l >> i) & 0x7FFFFF;                                                         \
            result += (h >> i) & 0x7FFFFF;                                                         \
        }                                                                                          \
        while (result >= 47) {                                                                     \
            std::uint64_t temp = 0;                                                                \
            for (int i = 0; i < 64; i += 23) {                                                     \
                temp += (result >> i) & 0x7FFFFF;                                                  \
            }                                                                                      \
            result = temp;                                                                         \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 53 */                                                                 \
    constexpr std::uint64_t mod53_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        result += (l & 0xFFFFFFFFFFFFF);                                                           \
        result += ((l >> 52) | (h << 12)) & 0xFFFFFFFFFFFFF;                                       \
        result += (h >> 40) & 0xFFFFFFFFFFFFF;                                                     \
        while (result >= 53) {                                                                     \
            result = (result >> 52) + (result & 0xFFFFFFFFFFFFF);                                  \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 59 */                                                                 \
    constexpr std::uint64_t mod59_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        result += (l & 0x3FFFFFFFFFFFFFF);                                                         \
        result += ((l >> 58) | (h << 6)) & 0x3FFFFFFFFFFFFFF;                                      \
        result += (h >> 52) & 0x3FFFFFFFFFFFFFF;                                                   \
        while (result >= 59) {                                                                     \
            result = (result >> 58) + (result & 0x3FFFFFFFFFFFFFF);                                \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización para mod 61 */                                                                 \
    constexpr std::uint64_t mod61_helper() const noexcept                                          \
    {                                                                                              \
        std::uint64_t result = 0;                                                                  \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        result += (l & 0xFFFFFFFFFFFFFFF);                                                         \
        result += ((l >> 60) | (h << 4)) & 0xFFFFFFFFFFFFFFF;                                      \
        result += (h >> 56) & 0xFFFFFFFFFFFFFFF;                                                   \
        while (result >= 61) {                                                                     \
            result = (result >> 60) + (result & 0xFFFFFFFFFFFFFFF);                                \
        }                                                                                          \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    /* Optimización genérica para otros módulos */                                                 \
    template <std::uint64_t M> constexpr std::uint64_t modM_generic_helper() const noexcept        \
    {                                                                                              \
        std::uint64_t h = high();                                                                  \
        std::uint64_t l = low();                                                                   \
        constexpr std::uint64_t power64_mod = (1ULL % M) * ((1ULL << 63) % M) * 2 % M;             \
        std::uint64_t h_mod = h % M;                                                               \
        std::uint64_t l_mod = l % M;                                                               \
        return (h_mod * power64_mod + l_mod) % M;                                                  \
    }

#endif // UINT128_MOD_HELPERS_HPP
