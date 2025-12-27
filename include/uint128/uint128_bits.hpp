#pragma once

#include "uint128_t.hpp"
#include <bitset>
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#endif

// =============================================================================
// Bit manipulation functions for uint128_t
// =============================================================================

namespace nstd
{

/**
 * @brief Count the number of set bits (population count)
 */
inline int popcount(const uint128_t& value) noexcept
{
#ifdef _MSC_VER
    return static_cast<int>(__popcnt64(value.high()) + __popcnt64(value.low()));
#else
    return __builtin_popcountll(value.high()) + __builtin_popcountll(value.low());
#endif
}

/**
 * @brief Count leading zeros
 */
inline int countl_zero(const uint128_t& value) noexcept
{
    if (value.high() != 0) {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanReverse64(&index, value.high());
        return static_cast<int>(63 - index);
#else
        return __builtin_clzll(value.high());
#endif
    } else if (value.low() != 0) {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanReverse64(&index, value.low());
        return static_cast<int>(127 - index);
#else
        return 64 + __builtin_clzll(value.low());
#endif
    } else {
        return 128;
    }
}

/**
 * @brief Count trailing zeros
 */
inline int countr_zero(const uint128_t& value) noexcept
{
    if (value.low() != 0) {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanForward64(&index, value.low());
        return static_cast<int>(index);
#else
        return __builtin_ctzll(value.low());
#endif
    } else if (value.high() != 0) {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanForward64(&index, value.high());
        return static_cast<int>(64 + index);
#else
        return 64 + __builtin_ctzll(value.high());
#endif
    } else {
        return 128;
    }
}

/**
 * @brief Count leading ones
 */
inline int countl_one(const uint128_t& value) noexcept
{
    if (value.high() == UINT64_MAX) {
        if (value.low() == UINT64_MAX) {
            return 128;
        } else {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanReverse64(&index, ~value.low());
            return static_cast<int>(63 - index);
#else
            return __builtin_clzll(~value.low());
#endif
        }
    } else {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanReverse64(&index, ~value.high());
        return static_cast<int>(63 - index);
#else
        return __builtin_clzll(~value.high());
#endif
    }
}

/**
 * @brief Count trailing ones
 */
inline int countr_one(const uint128_t& value) noexcept
{
    if (value.low() == UINT64_MAX) {
        if (value.high() == UINT64_MAX) {
            return 128;
        } else {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward64(&index, ~value.high());
            return static_cast<int>(64 + index);
#else
            return 64 + __builtin_ctzll(~value.high());
#endif
        }
    } else {
#ifdef _MSC_VER
        unsigned long index;
        _BitScanForward64(&index, ~value.low());
        return static_cast<int>(index);
#else
        return __builtin_ctzll(~value.low());
#endif
    }
}

/**
 * @brief Bit width (number of bits needed to represent the value)
 */
inline int bit_width(const uint128_t& value) noexcept
{
    return 128 - countl_zero(value);
}

/**
 * @brief Check if value has only a single bit set
 */
inline bool has_single_bit(const uint128_t& value) noexcept
{
    return value != uint128_t(0) && popcount(value) == 1;
}

/**
 * @brief Find the largest power of 2 not greater than value
 */
inline uint128_t bit_floor(const uint128_t& value) noexcept
{
    if (value == uint128_t(0)) {
        return uint128_t(0);
    }
    int width = bit_width(value);
    return uint128_t(1) << (width - 1);
}

/**
 * @brief Find the smallest power of 2 not less than value
 */
inline uint128_t bit_ceil(const uint128_t& value) noexcept
{
    if (value <= uint128_t(1)) {
        return uint128_t(1);
    }
    int width = bit_width(value - uint128_t(1));
    return uint128_t(1) << width;
}

} // namespace nstd
// =============================================================================
// Extended bit manipulation functions
// =============================================================================

namespace uint128_bits
{

using uint128_t = nstd::uint128_t;

/**
 * @brief Rotate left
 */
constexpr uint128_t rotl(const uint128_t& value, int shift) noexcept
{
    shift %= 128;
    if (shift == 0)
        return value;
    if (shift < 0)
        shift += 128;

    return (value << shift) | (value >> (128 - shift));
}

/**
 * @brief Rotate right
 */
constexpr uint128_t rotr(const uint128_t& value, int shift) noexcept
{
    shift %= 128;
    if (shift == 0)
        return value;
    if (shift < 0)
        shift += 128;

    return (value >> shift) | (value << (128 - shift));
}

/**
 * @brief Reverse all bits
 */
constexpr uint128_t reverse_bits(const uint128_t& value) noexcept
{
    // Helper function to reverse 64 bits
    auto reverse64 = [](uint64_t v) -> uint64_t {
        v = ((v >> 1) & 0x5555555555555555ULL) | ((v & 0x5555555555555555ULL) << 1);
        v = ((v >> 2) & 0x3333333333333333ULL) | ((v & 0x3333333333333333ULL) << 2);
        v = ((v >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((v & 0x0F0F0F0F0F0F0F0FULL) << 4);
        v = ((v >> 8) & 0x00FF00FF00FF00FFULL) | ((v & 0x00FF00FF00FF00FFULL) << 8);
        v = ((v >> 16) & 0x0000FFFF0000FFFFULL) | ((v & 0x0000FFFF0000FFFFULL) << 16);
        v = (v >> 32) | (v << 32);
        return v;
    };

    return uint128_t(reverse64(value.low()), reverse64(value.high()));
}

/**
 * @brief Reverse bytes (endianness swap)
 */
inline uint128_t byteswap(const uint128_t& value) noexcept
{
#ifdef _MSC_VER
    auto swap64 = [](uint64_t v) -> uint64_t { return _byteswap_uint64(v); };
#else
    auto swap64 = [](uint64_t v) -> uint64_t { return __builtin_bswap64(v); };
#endif

    return uint128_t(swap64(value.low()), swap64(value.high()));
}

/**
 * @brief Extract bit field
 * @param value Source value
 * @param offset Starting bit position (0-based)
 * @param width Number of bits to extract
 */
constexpr uint128_t extract_bits(const uint128_t& value, int offset, int width) noexcept
{
    if (width <= 0 || offset < 0 || offset >= 128) {
        return uint128_t(0);
    }

    if (offset + width > 128) {
        width = 128 - offset;
    }

    uint128_t mask = (uint128_t(1) << width) - uint128_t(1);
    return (value >> offset) & mask;
}

/**
 * @brief Insert bit field
 * @param dest Destination value
 * @param src Source bits to insert
 * @param offset Starting bit position (0-based)
 * @param width Number of bits to insert
 */
constexpr uint128_t insert_bits(const uint128_t& dest, const uint128_t& src, int offset,
                                int width) noexcept
{
    if (width <= 0 || offset < 0 || offset >= 128) {
        return dest;
    }

    if (offset + width > 128) {
        width = 128 - offset;
    }

    uint128_t mask = (uint128_t(1) << width) - uint128_t(1);
    uint128_t clear_mask = ~(mask << offset);

    return (dest & clear_mask) | ((src & mask) << offset);
}

/**
 * @brief Parallel bit deposit (PDEP)
 * @param value Source value
 * @param mask Bit mask indicating positions
 */
constexpr uint128_t parallel_deposit(const uint128_t& value, const uint128_t& mask) noexcept
{
    uint128_t result(0);
    uint128_t src_bit(1);

    for (int i = 0; i < 128; ++i) {
        if ((mask & (uint128_t(1) << i)) != uint128_t(0)) {
            if ((value & src_bit) != uint128_t(0)) {
                result |= (uint128_t(1) << i);
            }
            src_bit <<= 1;
        }
    }

    return result;
}

/**
 * @brief Parallel bit extract (PEXT)
 * @param value Source value
 * @param mask Bit mask indicating positions to extract
 */
constexpr uint128_t parallel_extract(const uint128_t& value, const uint128_t& mask) noexcept
{
    uint128_t result(0);
    uint128_t dst_bit(1);

    for (int i = 0; i < 128; ++i) {
        if ((mask & (uint128_t(1) << i)) != uint128_t(0)) {
            if ((value & (uint128_t(1) << i)) != uint128_t(0)) {
                result |= dst_bit;
            }
            dst_bit <<= 1;
        }
    }

    return result;
}

/**
 * @brief Get bit at specific position
 */
constexpr bool get_bit(const uint128_t& value, int position) noexcept
{
    if (position < 0 || position >= 128) {
        return false;
    }
    return (value & (uint128_t(1) << position)) != uint128_t(0);
}

/**
 * @brief Set bit at specific position
 */
constexpr uint128_t set_bit(const uint128_t& value, int position, bool bit_value = true) noexcept
{
    if (position < 0 || position >= 128) {
        return value;
    }

    if (bit_value) {
        return value | (uint128_t(1) << position);
    } else {
        return value & ~(uint128_t(1) << position);
    }
}

/**
 * @brief Flip bit at specific position
 */
constexpr uint128_t flip_bit(const uint128_t& value, int position) noexcept
{
    if (position < 0 || position >= 128) {
        return value;
    }

    return value ^ (uint128_t(1) << position);
}

/**
 * @brief Find first set bit (1-based index, 0 if none)
 */
constexpr int find_first_set(const uint128_t& value) noexcept
{
    if (value == uint128_t(0)) {
        return 0;
    }
    return std::countr_zero(value) + 1;
}

/**
 * @brief Find last set bit (1-based index, 0 if none)
 */
constexpr int find_last_set(const uint128_t& value) noexcept
{
    if (value == uint128_t(0)) {
        return 0;
    }
    return std::bit_width(value);
}

} // namespace uint128_bits

// =============================================================================
// std::bitset compatibility
// =============================================================================

namespace nstd
{

/**
 * @brief Convert uint128_t to std::bitset<128>
 */
inline std::bitset<128> to_bitset(const uint128_t& value) noexcept
{
    std::bitset<128> result;

    // Set bits for low 64 bits
    for (int i = 0; i < 64; ++i) {
        if ((value.low() & (1ULL << i)) != 0) {
            result.set(i);
        }
    }

    // Set bits for high 64 bits
    for (int i = 0; i < 64; ++i) {
        if ((value.high() & (1ULL << i)) != 0) {
            result.set(64 + i);
        }
    }

    return result;
}

/**
 * @brief Convert std::bitset<128> to uint128_t
 */
template <size_t N> inline uint128_t from_bitset(const std::bitset<N>& bs) noexcept
{
    static_assert(N <= 128, "Bitset size cannot exceed 128 bits");

    uint64_t low_part = 0;
    uint64_t high_part = 0;

    // Extract low 64 bits
    for (int i = 0; i < 64 && i < static_cast<int>(N); ++i) {
        if (bs[i]) {
            low_part |= (1ULL << i);
        }
    }

    // Extract high 64 bits
    for (int i = 64; i < 128 && i < static_cast<int>(N); ++i) {
        if (bs[i]) {
            high_part |= (1ULL << (i - 64));
        }
    }

    return uint128_t(high_part, low_part);
}

} // namespace nstd