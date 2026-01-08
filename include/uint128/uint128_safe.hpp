#pragma once

#include "uint128_t.hpp"
#include <cmath>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>

namespace nstd
{

// =============================================================================
// Safe conversion functions for uint128_t
// =============================================================================

namespace uint128_safe
{

using uint128_t = nstd::uint128_t;
using int128_t = nstd::int128_t;

/**
 * @brief Result type for safe conversions
 */
enum class conversion_result { success, overflow, underflow, invalid_input };

/**
 * @brief Safe conversion result with value and status
 */
template <typename T> struct safe_result {
    T value;
    conversion_result status;

    constexpr bool is_valid() const noexcept
    {
        return status == conversion_result::success;
    }

    constexpr explicit operator bool() const noexcept
    {
        return is_valid();
    }

    constexpr T value_or(const T& default_value) const noexcept
    {
        return is_valid() ? value : default_value;
    }
};

// =============================================================================
// Safe conversions FROM uint128_t to other types
// =============================================================================

/**
 * @brief Safe conversion from uint128_t to integral types
 */
template <typename T> constexpr 
safe_result<T> safe_cast(const uint128_t& value) noexcept
{
    static_assert(std::is_integral_v<T>, "Target type must be integral");

    if constexpr (std::is_signed_v<T>) {
        // For signed types, check against positive limit
        constexpr auto max_val = static_cast<uint64_t>(std::numeric_limits<T>::max());

        if (value.high() > 0 || value.low() > max_val) {
            return {T(0), conversion_result::overflow};
        }

        return {static_cast<T>(value.low()), conversion_result::success};

    } else {
        // For unsigned types
        if constexpr (sizeof(T) >= 8) {
            // 64-bit or larger unsigned types
            if (value.high() > 0) {
                return {T(0), conversion_result::overflow};
            }

            constexpr auto max_val = std::numeric_limits<T>::max();
            if constexpr (sizeof(T) == 8) {
                // Exactly 64-bit
                return {static_cast<T>(value.low()), conversion_result::success};
            } else {
                // Larger than 64-bit (shouldn't happen with standard types)
                return {static_cast<T>(value.low()), conversion_result::success};
            }

        } else {
            // Less than 64-bit unsigned types
            constexpr auto max_val = static_cast<uint64_t>(std::numeric_limits<T>::max());

            if (value.high() > 0 || value.low() > max_val) {
                return {T(0), conversion_result::overflow};
            }

            return {static_cast<T>(value.low()), conversion_result::success};
        }
    }
}

/**
 * @brief Safe conversion to floating point types
 */
template <floating_point_builtin T> constexpr 
safe_result<T> safe_cast_float(const uint128_t& value) noexcept
{
    // Convert high and low parts separately to avoid precision loss
    const T high_part = static_cast<T>(value.high());
    const T low_part = static_cast<T>(value.low());

    // Combine: high * 2^64 + low
    const T result = high_part * static_cast<T>(18446744073709551616.0) + low_part;

    // Check for infinity (overflow)
    if (!std::isfinite(result)) {
        return {T(0), conversion_result::overflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Checked conversion that throws on failure
 */
template <typename T> constexpr T checked_cast(const uint128_t& value)
{
    if constexpr (std::is_floating_point_v<T>) {
        const auto result = safe_cast_float<T>(value);
        if (!result.is_valid()) {
            throw std::overflow_error("uint128_t value too large for target floating point type");
        }
        return result.value;
    } else {
        const auto result = safe_cast<T>(value);
        if (result.status == conversion_result::overflow) {
            throw std::overflow_error("uint128_t value too large for target type");
        }
        return result.value;
    }
}

/**
 * @brief Optional-based conversion
 */
template <typename T> constexpr std::optional<T> try_cast(const uint128_t& value) noexcept
{
    if constexpr (std::is_floating_point_v<T>) {
        const auto result = safe_cast_float<T>(value);
        return result.is_valid() ? std::make_optional(result.value) : std::nullopt;
    } else {
        const auto result = safe_cast<T>(value);
        return result.is_valid() ? std::make_optional(result.value) : std::nullopt;
    }
}

// =============================================================================
// Safe conversions TO uint128_t from other types
// =============================================================================

/**
 * @brief Safe construction from integral types
 */
template <typename T> constexpr safe_result<uint128_t> safe_make_uint128(T value) noexcept
{
    static_assert(std::is_integral_v<T>, "Source type must be integral");

    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            return {uint128_t(0), conversion_result::underflow};
        }
        return {uint128_t(static_cast<std::make_unsigned_t<T>>(value)), conversion_result::success};
    } else {
        return {uint128_t(value), conversion_result::success};
    }
}

/**
 * @brief Safe construction from floating point types
 */
template <typename T> constexpr safe_result<uint128_t> safe_make_uint128_float(T value) noexcept
{
    static_assert(std::is_floating_point_v<T>, "Source type must be floating point");

    if (!std::isfinite(value)) {
        return {uint128_t(0), conversion_result::invalid_input};
    }

    if (value < 0.0) {
        return {uint128_t(0), conversion_result::underflow};
    }

    // Maximum value representable in uint128_t
    constexpr T max_uint128 = static_cast<T>(std::numeric_limits<uint64_t>::max()) *
                                  static_cast<T>(18446744073709551616.0L) +
                              static_cast<T>(std::numeric_limits<uint64_t>::max());

    if (value > max_uint128) {
        return {uint128_t(0), conversion_result::overflow};
    }

    // Extract high and low parts
    T high_part = std::floor(value / static_cast<T>(18446744073709551616.0L));
    T low_part = value - high_part * static_cast<T>(18446744073709551616.0L);

    uint64_t high_int = static_cast<uint64_t>(high_part);
    uint64_t low_int = static_cast<uint64_t>(low_part);

    return {uint128_t(high_int, low_int), conversion_result::success};
}

// =============================================================================
// Safe arithmetic operations
// =============================================================================

/**
 * @brief Safe addition
 */
constexpr safe_result<uint128_t> safe_add(const uint128_t& a, const uint128_t& b) noexcept
{
    uint128_t result = a + b;

    // Check for overflow: if result < a, then we wrapped around
    if (result < a) {
        return {uint128_t(0), conversion_result::overflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe subtraction
 */
constexpr safe_result<uint128_t> safe_sub(const uint128_t& a, const uint128_t& b) noexcept
{
    if (a < b) {
        return {uint128_t(0), conversion_result::underflow};
    }

    return {a - b, conversion_result::success};
}

/**
 * @brief Safe multiplication
 */
constexpr safe_result<uint128_t> safe_mul(const uint128_t& a, const uint128_t& b) noexcept
{
    // Simple overflow check: if a != 0 and result/a != b, then overflow occurred
    if (a == uint128_t(0) || b == uint128_t(0)) {
        return {uint128_t(0), conversion_result::success};
    }

    // For large values, do a preliminary check
    if (a.high() != 0 && b.high() != 0) {
        // Both high parts non-zero means definite overflow
        return {uint128_t(0), conversion_result::overflow};
    }

    uint128_t result = a * b;

    // Verify the result
    if (result / a != b) {
        return {uint128_t(0), conversion_result::overflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe division
 */
constexpr safe_result<uint128_t> safe_div(const uint128_t& a, const uint128_t& b) noexcept
{
    if (b == uint128_t(0)) {
        return {uint128_t(0), conversion_result::invalid_input};
    }

    return {a / b, conversion_result::success};
}

/**
 * @brief Safe modulo
 */
constexpr safe_result<uint128_t> safe_mod(const uint128_t& a, const uint128_t& b) noexcept
{
    if (b == uint128_t(0)) {
        return {uint128_t(0), conversion_result::invalid_input};
    }

    return {a % b, conversion_result::success};
}

/**
 * @brief Safe left shift
 */
constexpr safe_result<uint128_t> safe_shl(const uint128_t& value, int shift) noexcept
{
    if (shift < 0 || shift >= 128) {
        return {uint128_t(0), conversion_result::invalid_input};
    }

    if (shift == 0) {
        return {value, conversion_result::success};
    }

    // Check for overflow: if any bit would be shifted out
    uint128_t mask = (uint128_t(1) << (128 - shift)) - uint128_t(1);
    mask = ~mask; // Invert to get the bits that would be lost

    if ((value & mask) != uint128_t(0)) {
        return {uint128_t(0), conversion_result::overflow};
    }

    return {value << shift, conversion_result::success};
}

/**
 * @brief Safe right shift
 */
constexpr safe_result<uint128_t> safe_shr(const uint128_t& value, int shift) noexcept
{
    if (shift < 0 || shift >= 128) {
        return {uint128_t(0), conversion_result::invalid_input};
    }

    return {value >> shift, conversion_result::success};
}

// =============================================================================
// Range checking utilities
// =============================================================================

/**
 * @brief Check if value is in range [min, max]
 */
constexpr bool in_range(const uint128_t& value, const uint128_t& min, const uint128_t& max) noexcept
{
    return value >= min && value <= max;
}

/**
 * @brief Clamp value to range [min, max]
 */
constexpr uint128_t clamp(const uint128_t& value, const uint128_t& min,
                          const uint128_t& max) noexcept
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

/**
 * @brief Saturating addition (clamps to max value on overflow)
 */
constexpr uint128_t saturating_add(const uint128_t& a, const uint128_t& b) noexcept
{
    auto result = safe_add(a, b);
    return result.is_valid() ? result.value : ~uint128_t(0); // Usar max manual
}

/**
 * @brief Saturating subtraction (clamps to 0 on underflow)
 */
constexpr uint128_t saturating_sub(const uint128_t& a, const uint128_t& b) noexcept
{
    auto result = safe_sub(a, b);
    return result.is_valid() ? result.value : uint128_t(0);
}

/**
 * @brief Saturating multiplication (clamps to max value on overflow)
 */
constexpr uint128_t saturating_mul(const uint128_t& a, const uint128_t& b) noexcept
{
    auto result = safe_mul(a, b);
    return result.is_valid() ? result.value : ~uint128_t(0); // Usar max manual
}

} // namespace uint128_safe

} // namespace nstd
