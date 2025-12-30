/**
 * @file type_traits.hpp
 * @brief Type traits y concepts personalizados para el proyecto int128
 * @details Define traits y concepts auxiliares usados en todo el proyecto
 *
 * @author Julián
 * @date 2025-12-30
 */

#ifndef NSTD_TYPE_TRAITS_HPP
#define NSTD_TYPE_TRAITS_HPP

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace nstd
{

// ============================================================================
// TRAIT: is_integral_builtin
// ============================================================================

/**
 * @brief Type trait para detectar tipos enteros built-in (menores que 128 bits)
 * @tparam T Tipo a verificar
 *
 * @details Un tipo es integral_builtin si:
 * - Es un tipo integral estándar (std::is_integral_v<T> == true)
 * - Su tamaño es menor que 16 bytes (sizeof(T) < 16)
 *
 * Esto incluye: bool, char, signed char, unsigned char, wchar_t, char8_t,
 * char16_t, char32_t, short, unsigned short, int, unsigned int, long,
 * unsigned long, long long, unsigned long long, int8_t, uint8_t, int16_t,
 * uint16_t, int32_t, uint32_t, int64_t, uint64_t
 *
 * Excluye: __int128, __uint128_t, y tipos personalizados como uint128_t/int128_t
 *
 * @note Útil para sobrecargas de operadores que deben funcionar con tipos built-in
 *       pero no con uint128_t/int128_t para evitar ambigüedades
 */
template <typename T>
struct is_integral_builtin : std::bool_constant<std::is_integral_v<T> && (sizeof(T) < 16)> {
};

/**
 * @brief Variable template helper para is_integral_builtin
 * @tparam T Tipo a verificar
 */
template <typename T> inline constexpr bool is_integral_builtin_v = is_integral_builtin<T>::value;

// ============================================================================
// CONCEPT: integral_builtin
// ============================================================================

/**
 * @brief Concept para tipos enteros built-in (menores que 128 bits)
 * @tparam T Tipo a verificar
 *
 * @details Un tipo satisface integral_builtin si:
 * - Es un tipo integral estándar (std::integral<T>)
 * - Su tamaño es menor que 16 bytes (sizeof(T) < 16)
 *
 * Uso típico:
 * @code{.cpp}
 * template <integral_builtin T>
 * auto foo(T value) { ... }
 *
 * // O con requires clause:
 * template <typename T>
 *     requires integral_builtin<T>
 * auto bar(T value) { ... }
 * @endcode
 *
 * @note Ventaja sobre is_integral_builtin_v: mejor diagnóstico de errores
 *       y sintaxis más clara en templates
 */
template <typename T>
concept integral_builtin = std::integral<T> && (sizeof(T) < 16);

// ============================================================================
// TRAIT: is_integral_builtin_unsigned
// ============================================================================

/**
 * @brief Type trait para detectar tipos enteros built-in sin signo
 * @tparam T Tipo a verificar
 */
template <typename T>
struct is_integral_builtin_unsigned
    : std::bool_constant<is_integral_builtin_v<T> && std::is_unsigned_v<T>> {
};

/**
 * @brief Variable template helper para is_integral_builtin_unsigned
 * @tparam T Tipo a verificar
 */
template <typename T>
inline constexpr bool is_integral_builtin_unsigned_v = is_integral_builtin_unsigned<T>::value;

// ============================================================================
// TRAIT: is_integral_builtin_signed
// ============================================================================

/**
 * @brief Type trait para detectar tipos enteros built-in con signo
 * @tparam T Tipo a verificar
 */
template <typename T>
struct is_integral_builtin_signed
    : std::bool_constant<is_integral_builtin_v<T> && std::is_signed_v<T>> {
};

/**
 * @brief Variable template helper para is_integral_builtin_signed
 * @tparam T Tipo a verificar
 */
template <typename T>
inline constexpr bool is_integral_builtin_signed_v = is_integral_builtin_signed<T>::value;

// ============================================================================
// CONCEPT: integral_builtin_unsigned
// ============================================================================

/**
 * @brief Concept para tipos enteros built-in sin signo
 * @tparam T Tipo a verificar
 */
template <typename T>
concept integral_builtin_unsigned = integral_builtin<T> && std::unsigned_integral<T>;

// ============================================================================
// CONCEPT: integral_builtin_signed
// ============================================================================

/**
 * @brief Concept para tipos enteros built-in con signo
 * @tparam T Tipo a verificar
 */
template <typename T>
concept integral_builtin_signed = integral_builtin<T> && std::signed_integral<T>;

} // namespace nstd

#endif // NSTD_TYPE_TRAITS_HPP
