#pragma once

#include "int128_t.hpp"
#include <cmath>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>

// =============================================================================
// Safe conversion functions for int128_t
// =============================================================================

namespace int128_safe
{

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
// Safe conversions FROM int128_t to other types
// =============================================================================

/**
 * @brief Safe conversion from int128_t to integral types
 */
template <typename T> constexpr safe_result<T> safe_cast(const int128_t& value) noexcept
{
    static_assert(std::is_integral_v<T>, "Target type must be integral");

    if constexpr (std::is_signed_v<T>) {
        // For signed types, check against both positive and negative limits
        constexpr auto min_val = static_cast<int64_t>(std::numeric_limits<T>::min());
        constexpr auto max_val = static_cast<int64_t>(std::numeric_limits<T>::max());

        // Convert int128_t to int64_t for comparison (this may lose precision for very large
        // values)
        bool is_negative = value < int128_t(0);
        int128_t abs_value = value.abs();

        if (abs_value.high() > 0) {
            // Value too large to fit in any standard integral type
            return {T(0), is_negative ? conversion_result::underflow : conversion_result::overflow};
        }

        int64_t int64_val = is_negative ? -static_cast<int64_t>(abs_value.low())
                                        : static_cast<int64_t>(abs_value.low());

        if (int64_val < min_val) {
            return {T(0), conversion_result::underflow};
        }
        if (int64_val > max_val) {
            return {T(0), conversion_result::overflow};
        }

        return {static_cast<T>(int64_val), conversion_result::success};

    } else {
        // For unsigned types, negative values are underflow
        if (value < int128_t(0)) {
            return {T(0), conversion_result::underflow};
        }

        // Convert to absolute value for range checking
        int128_t abs_value = value.abs();

        if constexpr (sizeof(T) >= 8) {
            // 64-bit or larger unsigned types
            if (abs_value.high() > 0) {
                return {T(0), conversion_result::overflow};
            }
            return {static_cast<T>(abs_value.low()), conversion_result::success};

        } else {
            // Less than 64-bit unsigned types
            constexpr auto max_val = static_cast<uint64_t>(std::numeric_limits<T>::max());

            if (abs_value.high() > 0 || abs_value.low() > max_val) {
                return {T(0), conversion_result::overflow};
            }

            return {static_cast<T>(abs_value.low()), conversion_result::success};
        }
    }
}

/**
 * @brief Safe conversion to floating point types
 */
template <typename T> constexpr safe_result<T> safe_cast_float(const int128_t& value) noexcept
{
    static_assert(std::is_floating_point_v<T>, "Target type must be floating point");

    // Handle negative values
    bool is_negative = value < int128_t(0);
    int128_t abs_value = value.abs();

    // Convert high and low parts separately to avoid precision loss
    T high_part = static_cast<T>(abs_value.high());
    T low_part = static_cast<T>(abs_value.low());

    // Combine: high * 2^64 + low
    T result = high_part * static_cast<T>(18446744073709551616.0) + low_part;

    // Apply sign
    if (is_negative) {
        result = -result;
    }

    // Check for infinity (overflow)
    if (!std::isfinite(result)) {
        return {T(0), is_negative ? conversion_result::underflow : conversion_result::overflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Checked conversion that throws on failure
 */
template <typename T> constexpr T checked_cast(const int128_t& value)
{
    if constexpr (std::is_floating_point_v<T>) {
        auto result = safe_cast_float<T>(value);
        if (!result.is_valid()) {
            if (result.status == conversion_result::overflow) {
                throw std::overflow_error(
                    "int128_t value too large for target floating point type");
            } else {
                throw std::underflow_error(
                    "int128_t value too small for target floating point type");
            }
        }
        return result.value;
    } else {
        auto result = safe_cast<T>(value);
        if (result.status == conversion_result::overflow) {
            throw std::overflow_error("int128_t value too large for target type");
        } else if (result.status == conversion_result::underflow) {
            throw std::underflow_error("int128_t value too small for target type");
        }
        return result.value;
    }
}

/**
 * @brief Optional-based conversion
 */
template <typename T> constexpr std::optional<T> try_cast(const int128_t& value) noexcept
{
    if constexpr (std::is_floating_point_v<T>) {
        auto result = safe_cast_float<T>(value);
        return result.is_valid() ? std::make_optional(result.value) : std::nullopt;
    } else {
        auto result = safe_cast<T>(value);
        return result.is_valid() ? std::make_optional(result.value) : std::nullopt;
    }
}

// =============================================================================
// Safe conversions TO int128_t from other types
// =============================================================================

/**
 * @brief Safe construction from integral types
 */
template <typename T> constexpr safe_result<int128_t> safe_make_int128(T value) noexcept
{
    static_assert(std::is_integral_v<T>, "Source type must be integral");

    // For all standard integral types, conversion to int128_t should be safe
    return {int128_t(value), conversion_result::success};
}

/**
 * @brief Safe construction from floating point types
 */
template <typename T> constexpr safe_result<int128_t> safe_make_int128_float(T value) noexcept
{
    static_assert(std::is_floating_point_v<T>, "Source type must be floating point");

    if (!std::isfinite(value)) {
        return {int128_t(0), conversion_result::invalid_input};
    }

    // Maximum and minimum values representable in int128_t
    constexpr T max_int128 = static_cast<T>(std::numeric_limits<int64_t>::max()) *
                                 static_cast<T>(18446744073709551616.0) +
                             static_cast<T>(std::numeric_limits<uint64_t>::max());
    constexpr T min_int128 = -max_int128 - 1.0; // -2^127

    if (value > max_int128) {
        return {int128_t(0), conversion_result::overflow};
    }

    if (value < min_int128) {
        return {int128_t(0), conversion_result::underflow};
    }

    // Handle negative values
    bool is_negative = value < 0.0;
    T abs_value = is_negative ? -value : value;

    // Extract high and low parts
    T high_part = std::floor(abs_value / static_cast<T>(18446744073709551616.0));
    T low_part = abs_value - high_part * static_cast<T>(18446744073709551616.0);

    uint64_t high_int = static_cast<uint64_t>(high_part);
    uint64_t low_int = static_cast<uint64_t>(low_part);

    int128_t result(high_int, low_int);
    if (is_negative) {
        result = -result;
    }

    return {result, conversion_result::success};
}

// =============================================================================
// Safe arithmetic operations
// =============================================================================

/**
 * @brief Safe addition
 */
constexpr safe_result<int128_t> safe_add(const int128_t& a, const int128_t& b) noexcept
{
    int128_t result = a + b;

    // Check for overflow/underflow
    // If both operands have the same sign but result has different sign, overflow occurred
    bool a_positive = a >= int128_t(0);
    bool b_positive = b >= int128_t(0);
    bool result_positive = result >= int128_t(0);

    if (a_positive && b_positive && !result_positive) {
        // Positive overflow
        return {int128_t(0), conversion_result::overflow};
    }

    if (!a_positive && !b_positive && result_positive) {
        // Negative overflow (underflow)
        return {int128_t(0), conversion_result::underflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe subtraction
 */
constexpr safe_result<int128_t> safe_sub(const int128_t& a, const int128_t& b) noexcept
{
    int128_t result = a - b;

    // Check for overflow/underflow
    // If operands have different signs but result has wrong sign, overflow occurred
    bool a_positive = a >= int128_t(0);
    bool b_positive = b >= int128_t(0);
    bool result_positive = result >= int128_t(0);

    if (a_positive && !b_positive && !result_positive) {
        // Positive - negative = should be positive, but got negative (overflow)
        return {int128_t(0), conversion_result::overflow};
    }

    if (!a_positive && b_positive && result_positive) {
        // Negative - positive = should be negative, but got positive (underflow)
        return {int128_t(0), conversion_result::underflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe multiplication
 */
constexpr safe_result<int128_t> safe_mul(const int128_t& a, const int128_t& b) noexcept
{
    if (a == int128_t(0) || b == int128_t(0)) {
        return {int128_t(0), conversion_result::success};
    }

    int128_t result = a * b;

    // Check for overflow by dividing back
    if (result / a != b) {
        // Determine if it's overflow or underflow based on signs
        bool a_positive = a >= int128_t(0);
        bool b_positive = b >= int128_t(0);
        bool should_be_positive = a_positive == b_positive;

        return {int128_t(0),
                should_be_positive ? conversion_result::overflow : conversion_result::underflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe division
 */
constexpr safe_result<int128_t> safe_div(const int128_t& a, const int128_t& b) noexcept
{
    if (b == int128_t(0)) {
        return {int128_t(0), conversion_result::invalid_input};
    }

    // Check for the special case: most negative / -1 = overflow
    // This is because -(-2^127) = +2^127, which doesn't fit in int128_t
    if (a == std::numeric_limits<int128_t>::min() && b == int128_t(-1)) {
        return {int128_t(0), conversion_result::overflow};
    }

    return {a / b, conversion_result::success};
}

/**
 * @brief Safe modulo
 */
constexpr safe_result<int128_t> safe_mod(const int128_t& a, const int128_t& b) noexcept
{
    if (b == int128_t(0)) {
        return {int128_t(0), conversion_result::invalid_input};
    }

    // For the special case a = min(), b = -1, result is 0 (no remainder)
    if (a == std::numeric_limits<int128_t>::min() && b == int128_t(-1)) {
        return {int128_t(0), conversion_result::success};
    }

    return {a % b, conversion_result::success};
}

/**
 * @brief Safe left shift
 */
constexpr safe_result<int128_t> safe_shl(const int128_t& value, int shift) noexcept
{
    if (shift < 0 || shift >= 128) {
        return {int128_t(0), conversion_result::invalid_input};
    }

    if (shift == 0) {
        return {value, conversion_result::success};
    }

    // For signed integers, left shift can cause overflow if sign changes
    int128_t result = value << shift;

    // Check if we can recover original value by right shifting
    if ((result >> shift) != value) {
        bool is_positive = value >= int128_t(0);
        return {int128_t(0),
                is_positive ? conversion_result::overflow : conversion_result::underflow};
    }

    return {result, conversion_result::success};
}

/**
 * @brief Safe right shift (for signed integers, this is arithmetic shift)
 */
constexpr safe_result<int128_t> safe_shr(const int128_t& value, int shift) noexcept
{
    if (shift < 0 || shift >= 128) {
        return {int128_t(0), conversion_result::invalid_input};
    }

    return {value >> shift, conversion_result::success};
}

// =============================================================================
// Range checking utilities
// =============================================================================

/**
 * @brief Check if value is in range [min, max]
 */
constexpr bool in_range(const int128_t& value, const int128_t& min, const int128_t& max) noexcept
{
    return value >= min && value <= max;
}

/**
 * @brief Clamp value to range [min, max]
 */
constexpr int128_t clamp(const int128_t& value, const int128_t& min, const int128_t& max) noexcept
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

/**
 * @brief Saturating addition (clamps to min/max value on overflow/underflow)
 */
constexpr int128_t saturating_add(const int128_t& a, const int128_t& b) noexcept
{
    auto result = safe_add(a, b);
    if (result.is_valid()) {
        return result.value;
    } else if (result.status == conversion_result::overflow) {
        // Return explicit maximum value
        return int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    } else {
        // Return explicit minimum value
        return int128_t(0x8000000000000000ULL, 0x0000000000000000ULL);
    }
}

/**
 * @brief Saturating subtraction (clamps to min/max value on underflow/overflow)
 */
constexpr int128_t saturating_sub(const int128_t& a, const int128_t& b) noexcept
{
    auto result = safe_sub(a, b);
    if (result.is_valid()) {
        return result.value;
    } else if (result.status == conversion_result::overflow) {
        // Return explicit maximum value
        return int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    } else {
        // Return explicit minimum value
        return int128_t(0x8000000000000000ULL, 0x0000000000000000ULL);
    }
}

/**
 * @brief Saturating multiplication (clamps to min/max value on overflow/underflow)
 */
constexpr int128_t saturating_mul(const int128_t& a, const int128_t& b) noexcept
{
    auto result = safe_mul(a, b);
    if (result.is_valid()) {
        return result.value;
    } else if (result.status == conversion_result::overflow) {
        // Return explicit maximum value
        return int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    } else {
        // Return explicit minimum value
        return int128_t(0x8000000000000000ULL, 0x0000000000000000ULL);
    }
}

/**
 * @brief Absolute value with overflow detection
 */
constexpr safe_result<int128_t> safe_abs(const int128_t& value) noexcept
{
    // Special case: abs(min()) would overflow
    // Check explicitly for min value: -2^127
    if (value == int128_t(0x8000000000000000ULL, 0x0000000000000000ULL)) {
        return {int128_t(0), conversion_result::overflow};
    }

    return {value.abs(), conversion_result::success};
}

/**
 * @brief Saturating absolute value (returns max on overflow)
 */
constexpr int128_t saturating_abs(const int128_t& value) noexcept
{
    auto result = safe_abs(value);
    return result.is_valid() ? result.value
                             : int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
}

} // namespace int128_safe