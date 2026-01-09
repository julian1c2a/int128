/**
 * @file uint128_safe_bridge.hpp
 * @brief Puente de compatibilidad: API antigua -> Headers unificados (Fase 1.75)
 *
 * Este header proporciona compatibilidad hacia atrás con la API original
 * de uint128_safe mientras usa internamente los headers unificados.
 */

#pragma once

// Incluir el header unificado
#include "../../include_new/int128_base_safe.hpp"

namespace nstd
{
namespace uint128_safe
{

// Re-exportar tipos desde el header unificado
using conversion_result = ::nstd::int128_safe::conversion_result;

template <typename T> using safe_result = ::nstd::int128_safe::safe_result<T>;

// =============================================================================
// Wrapper functions para compatibilidad con API antigua
// =============================================================================

/**
 * @brief Conversion segura de uint128_t a tipo T (API antigua)
 */
template <typename T> constexpr safe_result<T> safe_cast(const uint128_t& value) noexcept
{
    return ::nstd::int128_safe::safe_cast<signedness::unsigned_type, T>(value);
}

/**
 * @brief Conversion segura de uint128_t a float/double (API antigua)
 */
template <typename T> constexpr safe_result<T> safe_cast_float(const uint128_t& value) noexcept
{
    return ::nstd::int128_safe::safe_cast_float<signedness::unsigned_type, T>(value);
}

/**
 * @brief try_cast con std::optional (API antigua)
 */
template <typename T> constexpr std::optional<T> try_cast(const uint128_t& value) noexcept
{
    return ::nstd::int128_safe::try_cast<signedness::unsigned_type, T>(value);
}

/**
 * @brief Suma segura (API antigua)
 */
inline constexpr safe_result<uint128_t> safe_add(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_add(a, b);
}

/**
 * @brief Resta segura (API antigua)
 */
inline constexpr safe_result<uint128_t> safe_sub(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_sub(a, b);
}

/**
 * @brief Multiplicacion segura (API antigua)
 */
inline constexpr safe_result<uint128_t> safe_mul(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_mul(a, b);
}

/**
 * @brief Division segura (API antigua)
 */
inline constexpr safe_result<uint128_t> safe_div(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_div(a, b);
}

/**
 * @brief Modulo seguro (API antigua)
 */
inline constexpr safe_result<uint128_t> safe_mod(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::safe_mod(a, b);
}

/**
 * @brief Suma con saturacion (API antigua)
 */
inline constexpr uint128_t saturating_add(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_add(a, b);
}

/**
 * @brief Resta con saturacion (API antigua)
 */
inline constexpr uint128_t saturating_sub(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_sub(a, b);
}

/**
 * @brief Multiplicacion con saturacion (API antigua)
 */
inline constexpr uint128_t saturating_mul(const uint128_t& a, const uint128_t& b) noexcept
{
    return ::nstd::int128_safe::saturating_mul(a, b);
}

/**
 * @brief Verificar si valor esta en rango (API antigua)
 */
inline constexpr bool in_range(const uint128_t& value, const uint128_t& min_val,
                               const uint128_t& max_val) noexcept
{
    return ::nstd::int128_safe::in_range(value, min_val, max_val);
}

/**
 * @brief Clamp a rango (API antigua)
 */
inline constexpr uint128_t clamp(const uint128_t& value, const uint128_t& min_val,
                                 const uint128_t& max_val) noexcept
{
    return ::nstd::int128_safe::clamp(value, min_val, max_val);
}

} // namespace uint128_safe
} // namespace nstd
