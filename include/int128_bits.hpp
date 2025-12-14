#pragma once
#ifndef INT128_BITS_HPP
#define INT128_BITS_HPP

#include "int128_t.hpp"
#include <bitset>
#include <type_traits>

// =============================================================================
// Bit manipulation functions for int128_t
// =============================================================================

namespace std
{

/**
 * @brief Count the number of set bits (population count)
 * Para números con signo, cuenta todos los bits establecidos
 */
constexpr int popcount(const int128_t& value) noexcept
{
    return __builtin_popcountll(value.high()) + __builtin_popcountll(value.low());
}

/**
 * @brief Count leading zeros
 * Para int128_t, cuenta ceros a la izquierda considerando el bit de signo
 */
constexpr int countl_zero(const int128_t& value) noexcept
{
    if (value.is_negative()) {
        // Números negativos empiezan con 1 (bit de signo)
        return 0;
    }

    if (value.high() != 0) {
        return __builtin_clzll(value.high());
    } else if (value.low() != 0) {
        return 64 + __builtin_clzll(value.low());
    } else {
        return 128;
    }
}

/**
 * @brief Count trailing zeros
 */
constexpr int countr_zero(const int128_t& value) noexcept
{
    if (value.low() != 0) {
        return __builtin_ctzll(value.low());
    } else if (value.high() != 0) {
        return 64 + __builtin_ctzll(value.high());
    } else {
        return 128;
    }
}

/**
 * @brief Count leading ones
 * Para números negativos, cuenta los 1s consecutivos desde el MSB
 */
constexpr int countl_one(const int128_t& value) noexcept
{
    if (!value.is_negative()) {
        // Números positivos/cero empiezan con 0
        return 0;
    }

    if (value.high() == UINT64_MAX) {
        if (value.low() == UINT64_MAX) {
            return 128; // Todo unos (-1)
        } else {
            return 64 + __builtin_clzll(~value.low());
        }
    } else {
        return __builtin_clzll(~value.high());
    }
}

/**
 * @brief Count trailing ones
 */
constexpr int countr_one(const int128_t& value) noexcept
{
    if (value.low() == UINT64_MAX) {
        if (value.high() == UINT64_MAX) {
            return 128;
        } else {
            return 64 + __builtin_ctzll(~value.high());
        }
    } else {
        return __builtin_ctzll(~value.low());
    }
}

/**
 * @brief Bit width (número de bits necesarios para representar el valor)
 * Para números negativos, usa todos los 128 bits
 */
constexpr int bit_width(const int128_t& value) noexcept
{
    if (value.is_negative()) {
        return 128; // Los números negativos usan todos los bits
    }
    return 128 - countl_zero(value);
}

/**
 * @brief Check if value has only a single bit set
 * Solo válido para números positivos
 */
constexpr bool has_single_bit(const int128_t& value) noexcept
{
    if (value.is_negative() || value.is_zero())
        return false;

    return popcount(value) == 1;
}

/**
 * @brief Find the largest power of 2 not greater than value
 * Solo válido para números positivos
 */
constexpr int128_t bit_floor(const int128_t& value) noexcept
{
    if (value.is_negative() || value.is_zero()) {
        return int128_t(0);
    }

    int width = bit_width(value);
    if (width == 0)
        return int128_t(0);
    return int128_t(1) << (width - 1);
}

/**
 * @brief Find the smallest power of 2 not less than value
 * Solo válido para números positivos
 */
constexpr int128_t bit_ceil(const int128_t& value) noexcept
{
    if (value.is_negative()) {
        return int128_t(0); // No definido para negativos
    }

    if (value <= int128_t(1)) {
        return int128_t(1);
    }

    int width = bit_width(value - int128_t(1));
    return int128_t(1) << width;
}

} // namespace std

// =============================================================================
// Extended bit manipulation functions
// =============================================================================

namespace int128_bits
{

/**
 * @brief Rotate left (rotación lógica, no aritmética)
 */
constexpr int128_t rotl(const int128_t& value, int shift) noexcept
{
    shift %= 128;
    if (shift == 0)
        return value;
    if (shift < 0)
        shift += 128;

    // Trabajar con representación unsigned para rotación
    uint128_t uval = value.to_uint128();
    uint128_t result = (uval << shift) | (uval >> (128 - shift));
    return int128_t(result);
}

/**
 * @brief Rotate right (rotación lógica, no aritmética)
 */
constexpr int128_t rotr(const int128_t& value, int shift) noexcept
{
    shift %= 128;
    if (shift == 0)
        return value;
    if (shift < 0)
        shift += 128;

    // Trabajar con representación unsigned para rotación
    uint128_t uval = value.to_uint128();
    uint128_t result = (uval >> shift) | (uval << (128 - shift));
    return int128_t(result);
}

/**
 * @brief Reverse all bits
 */
constexpr int128_t reverse_bits(const int128_t& value) noexcept
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

    // Para int128_t, intercambiamos high/low y revertimos cada parte
    return int128_t(reverse64(value.low()), reverse64(value.high()));
}

/**
 * @brief Reverse bytes (endianness swap)
 */
constexpr int128_t byteswap(const int128_t& value) noexcept
{
    auto swap64 = [](uint64_t v) -> uint64_t { return __builtin_bswap64(v); };

    return int128_t(swap64(value.low()), swap64(value.high()));
}

/**
 * @brief Extract bit field
 * @param value Source value
 * @param offset Starting bit position (0-based, from LSB)
 * @param width Number of bits to extract
 */
constexpr int128_t extract_bits(const int128_t& value, int offset, int width) noexcept
{
    if (width <= 0 || offset < 0 || offset >= 128) {
        return int128_t(0);
    }

    if (offset + width > 128) {
        width = 128 - offset;
    }

    // Trabajar con uint128_t para evitar problemas de signo
    uint128_t uval = value.to_uint128();

    // Shift right para mover los bits deseados a la posición LSB
    uval >>= offset;

    // Crear máscara para los bits deseados
    if (width == 128) {
        return int128_t(uval);
    }

    uint128_t mask = (uint128_t(1) << width) - uint128_t(1);
    return int128_t(uval & mask);
}

/**
 * @brief Insert bit field
 * @param target Target value
 * @param source Source bits
 * @param offset Starting bit position (0-based, from LSB)
 * @param width Number of bits to insert
 */
constexpr int128_t insert_bits(const int128_t& target, const int128_t& source, int offset,
                               int width) noexcept
{
    if (width <= 0 || offset < 0 || offset >= 128) {
        return target;
    }

    if (offset + width > 128) {
        width = 128 - offset;
    }

    uint128_t utarget = target.to_uint128();
    uint128_t usource = source.to_uint128();

    // Crear máscara para limpiar el área de destino
    uint128_t mask;
    if (width == 128) {
        mask = uint128_t(0);
    } else {
        mask = (uint128_t(1) << width) - uint128_t(1);
    }

    // Limpiar bits del target
    utarget &= ~(mask << offset);

    // Limpiar bits extra del source e insertar
    usource &= mask;
    utarget |= (usource << offset);

    return int128_t(utarget);
}

/**
 * @brief Test if a specific bit is set
 */
constexpr bool test_bit(const int128_t& value, int bit_pos) noexcept
{
    if (bit_pos < 0 || bit_pos >= 128) {
        return false;
    }

    uint128_t uval = value.to_uint128();
    return ((uval >> bit_pos) & uint128_t(1)) != uint128_t(0);
}

/**
 * @brief Set a specific bit
 */
constexpr int128_t set_bit(const int128_t& value, int bit_pos) noexcept
{
    if (bit_pos < 0 || bit_pos >= 128) {
        return value;
    }

    uint128_t uval = value.to_uint128();
    uval |= (uint128_t(1) << bit_pos);
    return int128_t(uval);
}

/**
 * @brief Clear a specific bit
 */
constexpr int128_t clear_bit(const int128_t& value, int bit_pos) noexcept
{
    if (bit_pos < 0 || bit_pos >= 128) {
        return value;
    }

    uint128_t uval = value.to_uint128();
    uval &= ~(uint128_t(1) << bit_pos);
    return int128_t(uval);
}

/**
 * @brief Flip a specific bit
 */
constexpr int128_t flip_bit(const int128_t& value, int bit_pos) noexcept
{
    if (bit_pos < 0 || bit_pos >= 128) {
        return value;
    }

    uint128_t uval = value.to_uint128();
    uval ^= (uint128_t(1) << bit_pos);
    return int128_t(uval);
}

/**
 * @brief Find first set bit (LSB to MSB scan)
 * @return Position of first set bit (0-based), or -1 if none
 */
constexpr int find_first_set(const int128_t& value) noexcept
{
    if (value.is_zero()) {
        return -1;
    }

    return std::countr_zero(value);
}

/**
 * @brief Find last set bit (MSB to LSB scan)
 * @return Position of last set bit (0-based), or -1 if none
 */
constexpr int find_last_set(const int128_t& value) noexcept
{
    if (value.is_zero()) {
        return -1;
    }

    return 127 - std::countl_zero(value.is_negative() ? ~value : value);
}

} // namespace int128_bits

#endif // INT128_BITS_HPP