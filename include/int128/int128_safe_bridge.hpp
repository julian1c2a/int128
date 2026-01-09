/**
 * @file int128_safe_bridge.hpp
 * @brief Puente de compatibilidad: API antigua -> Headers unificados (Fase 1.75)
 *
 * Este header proporciona compatibilidad hacia atrás con la API original
 * de int128_safe mientras usa internamente los headers unificados.
 */

#pragma once

// Incluir el header unificado
#include "../../include_new/int128_base_safe.hpp"

namespace nstd
{
namespace int128_safe
{

// Re-exportar tipos desde el header unificado
using conversion_result = ::nstd::int128_safe::conversion_result;

template <typename T> using safe_result = ::nstd::int128_safe::safe_result<T>;

// =============================================================================
// Wrapper functions para compatibilidad con API antigua
// =============================================================================

/**
 * @brief Conversion segura de int128_t a tipo T (API antigua)
 */
template <typename T> constexpr safe_result<T> safe_cast(const int128_t& value) noexcept
{
    return ::nstd::int128_safe::safe_cast<signedness::signed_type, T>(value);
}

/**
 * @brief Conversion segura de int128_t a float/double (API antigua)
 */
template <typename T> constexpr safe_result<T> safe_cast_float(const int128_t& value) noexcept
{
    return ::nstd::int128_safe::safe_cast_float<signedness::signed_type, T>(value);
}

/**
 * @brief try_cast con std::optional (API antigua)
 */
template <typename T> constexpr std::optional<T> try_cast(const int128_t& value) noexcept
{
    return ::nstd::int128_safe::try_cast<signedness::signed_type, T>(value);
}

/**
 * @brief Suma segura (API antigua)
 */
inline constexpr safe_result<int128_t> safe_add(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_add(a, b);
}

/**
 * @brief Resta segura (API antigua)
 */
inline constexpr safe_result<int128_t> safe_sub(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_sub(a, b);
}

/**
 * @brief Multiplicacion segura (API antigua)
 */
inline constexpr safe_result<int128_t> safe_mul(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_mul(a, b);
}

/**
 * @brief Division segura (API antigua)
 */
inline constexpr safe_result<int128_t> safe_div(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_div(a, b);
}

/**
 * @brief Modulo seguro (API antigua)
 */
inline constexpr safe_result<int128_t> safe_mod(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_mod(a, b);
}

/**
 * @brief Valor absoluto seguro (API antigua)
 */
inline constexpr safe_result<int128_t> safe_abs(const int128_t& value) noexcept
{
    return ::nstd::int128_safe::safe_abs(value);
}

/**
 * @brief Suma con saturacion (API antigua)
 */
inline constexpr int128_t saturating_add(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_add(a, b);
}

/**
 * @brief Resta con saturacion (API antigua)
 */
inline constexpr int128_t saturating_sub(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_sub(a, b);
}

/**
 * @brief Multiplicacion con saturacion (API antigua)
 */
inline constexpr int128_t saturating_mul(const int128_t& a, const int128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_mul(a, b);
}

/**
 * @brief Verificar si valor esta en rango (API antigua)
 */
inline constexpr bool in_range(const int128_t& value, const int128_t& min_val,
                               const int128_t& max_val) noexcept
{
    return ::nstd::int128_safe::in_range(value, min_val, max_val);
}

/**
 * @brief Clamp a rango (API antigua)
 */
inline constexpr int128_t clamp(const int128_t& value, const int128_t& min_val,
                                const int128_t& max_val) noexcept
{
    return ::nstd::int128_safe::clamp(value, min_val, max_val);
}

/**
 * @brief Factory function segura (API antigua)
 */
inline constexpr safe_result<int128_t> safe_make_int128(int64_t high, uint64_t low) noexcept
{
    return {int128_t(high, low), conversion_result::success};
}

} // namespace int128_safe
} // namespace nstd
