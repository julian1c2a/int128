#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#include <cstdint>
#include <climits>
#include <type_traits>
#include <utility>
#include <concepts>
#include <stdexcept>

#ifdef _MSC_VER
#include <intrin.h>
#endif




struct uint128_t {
    uint64_t low;
    uint64_t high;

    // CONSTRUCTORS
    constexpr uint128_t() noexcept : low(0), high(0) {}
    constexpr uint128_t(uint64_t h, uint64_t l) noexcept : low(l), high(h) {}
    
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
    constexpr uint128_t(T value) noexcept : low(static_cast<uint64_t>(value)), high(0) {}

#if defined(__GNUC__) || defined(__clang__)
    constexpr uint128_t(unsigned __int128 value) noexcept
        : low(static_cast<uint64_t>(value)), high(static_cast<uint64_t>(value >> 64)) {}
#endif
    
    // ASSIGNMENT OPERATORS
    constexpr uint128_t& operator=(const uint128_t& other) noexcept = default;
    
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
    constexpr uint128_t& operator=(T value) noexcept {
        low = static_cast<uint64_t>(value);
        high = 0;
        return *this;
    }

    // CONVERSIONS
    explicit constexpr operator bool() const noexcept { return (high != 0) || (low != 0); }

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    explicit constexpr operator T() const noexcept {
        return static_cast<T>(low);
    }
    
#if defined(__GNUC__) || defined(__clang__)
    explicit constexpr operator unsigned __int128() const noexcept {
        return (static_cast<unsigned __int128>(high) << 64) | low;
    }
#endif

    // UNARY OPERATORS
    uint128_t& operator++() noexcept {
        if (++low == 0) {
            ++high;
        }
        return *this;
    }

    uint128_t operator++(int) noexcept {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    uint128_t& operator--() noexcept {
        if (low-- == 0) {
            --high;
        }
        return *this;
    }

    uint128_t operator--(int) noexcept {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }
    
    uint128_t operator-() const noexcept {
        return ~(*this) + 1;
    }

    // ARITHMETIC ASSIGNMENT OPERATORS
    uint128_t& operator+=(const uint128_t& other) noexcept {
        uint64_t old_low = low;
        low += other.low;
        high += other.high;
        if (low < old_low) { // check for carry
            high++;
        }
        return *this;
    }

    uint128_t& operator-=(const uint128_t& other) noexcept {
        uint64_t old_low = low;
        low -= other.low;
        high -= other.high;
        if (low > old_low) { // check for borrow
            high--;
        }
        return *this;
    }

#ifdef _MSC_VER
    uint128_t& operator*=(const uint128_t& other) noexcept {
        uint64_t h1 = high, l1 = low;
        uint64_t h2 = other.high, l2 = other.low;
        
        uint64_t cross_product1 = h1 * l2;
        uint64_t cross_product2 = h2 * l1;

        uint64_t high_res; // will be discarded (overflow)
        uint64_t low_res = _umul128(l1, l2, &high_res);

        high = high_res + cross_product1 + cross_product2;
        low = low_res;
        return *this;
    }
    
    // Division by a 64-bit integer
    uint128_t& operator/=(uint64_t divisor) {
        if (divisor == 0) {
            throw std::domain_error("Division by zero.");
        }
        uint64_t remainder;
        low = _udiv128(high, low, divisor, &remainder);
        high = 0; // The quotient of 128/64 fits in 64 bits if high < divisor
        
        // if high part is >= divisor, the result won't fit in 64 bits.
        // _udiv128 handles this by returning the largest possible quotient.
        // We need a more complex algorithm for true 128/64 division where quotient can be > 64 bits
        // For now, this is a common implementation. A full 128/128 or 192/64 division is needed for correctness.
        // Let's stick to a simpler 128/64 that fits in a uint128_t result.

        // A better approach for 128/64:
        if (high >= divisor) {
             throw std::overflow_error("Division result overflows uint128_t.");
        }
        uint64_t rem;
        uint64_t q1 = _udiv128(0, high, divisor, &rem); // q1 = high / divisor
        uint64_t q2 = _udiv128(rem, low, divisor, &rem); // q2 = (rem:low) / divisor
        high = q1;
        low = q2;
        return *this;
    }

    // Modulo by a 64-bit integer
    uint128_t& operator%=(uint64_t divisor) {
        if (divisor == 0) {
            throw std::domain_error("Division by zero.");
        }
        uint64_t remainder;
        _udiv128(high, low, divisor, &remainder);
        low = remainder;
        high = 0;
        return *this;
    }

#else // GCC/Clang with __int128
    uint128_t& operator*=(const uint128_t& other) noexcept {
        unsigned __int128 res = static_cast<unsigned __int128>(*this) * static_cast<unsigned __int128>(other);
        high = res >> 64;
        low = static_cast<uint64_t>(res);
        return *this;
    }
    
    uint128_t& operator/=(const uint128_t& other) noexcept {
        unsigned __int128 res = static_cast<unsigned __int128>(*this) / static_cast<unsigned __int128>(other);
        high = res >> 64;
        low = static_cast<uint64_t>(res);
        return *this;
    }
    
    uint128_t& operator%=(const uint128_t& other) noexcept {
        unsigned __int128 res = static_cast<unsigned __int128>(*this) % static_cast<unsigned __int128>(other);
        high = res >> 64;
        low = static_cast<uint64_t>(res);
        return *this;
    }
#endif

    // BITWISE ASSIGNMENT OPERATORS
    uint128_t& operator&=(const uint128_t& other) noexcept { high &= other.high; low &= other.low; return *this; }
    uint128_t& operator|=(const uint128_t& other) noexcept { high |= other.high; low |= other.low; return *this; }
    uint128_t& operator^=(const uint128_t& other) noexcept { high ^= other.high; low ^= other.low; return *this; }
    uint128_t operator~() const noexcept { return uint128_t(~high, ~low); }

    // SHIFT OPERATORS
    uint128_t& operator<<=(unsigned int shift) noexcept {
        if (shift == 0) return *this;
        if (shift >= 128) { high = low = 0; return *this; }
        if (shift >= 64) {
            high = low << (shift - 64);
            low = 0;
        } else {
            high = (high << shift) | (low >> (64 - shift));
            low <<= shift;
        }
        return *this;
    }

    uint128_t& operator>>=(unsigned int shift) noexcept {
        if (shift == 0) return *this;
        if (shift >= 128) { high = low = 0; return *this; }
        if (shift >= 64) {
            low = high >> (shift - 64);
            high = 0;
        } else {
            low = (low >> shift) | (high << (64 - shift));
            high >>= shift;
        }
        return *this;
    }

    // BINARY ARITHMETIC OPERATORS
    uint128_t operator+(const uint128_t& other) const noexcept { return uint128_t(*this) += other; }
    uint128_t operator-(const uint128_t& other) const noexcept { return uint128_t(*this) -= other; }
    uint128_t operator*(const uint128_t& other) const noexcept { return uint128_t(*this) *= other; }
    
#ifdef _MSC_VER
    uint128_t operator/(uint64_t other) const { return uint128_t(*this) /= other; }
    uint128_t operator%(uint64_t other) const { return uint128_t(*this) %= other; }
#else
    uint128_t operator/(const uint128_t& other) const { return uint128_t(*this) /= other; }
    uint128_t operator%(const uint128_t& other) const { return uint128_t(*this) %= other; }
#endif

    // BINARY BITWISE OPERATORS
    uint128_t operator&(const uint128_t& other) const noexcept { return uint128_t(*this) &= other; }
    uint128_t operator|(const uint128_t& other) const noexcept { return uint128_t(*this) |= other; }
    uint128_t operator^(const uint128_t& other) const noexcept { return uint128_t(*this) ^= other; }
    uint128_t operator<<(unsigned int shift) const noexcept { return uint128_t(*this) <<= shift; }
    uint128_t operator>>(unsigned int shift) const noexcept { return uint128_t(*this) >>= shift; }

    // COMPARISON OPERATORS
    bool operator==(const uint128_t& other) const noexcept { return (high == other.high) && (low == other.low); }
    bool operator!=(const uint128_t& other) const noexcept { return !(*this == other); }
    bool operator<(const uint128_t& other) const noexcept {
        if (high < other.high) return true;
        if (high > other.high) return false;
        return low < other.low;
    }
    bool operator>(const uint128_t& other) const noexcept { return other < *this; }
    bool operator<=(const uint128_t& other) const noexcept { return !(*this > other); }
    bool operator>=(const uint128_t& other) const noexcept { return !(*this < other); }
};

// NON-MEMBER ARITHMETIC OPERATORS
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
uint128_t operator+(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) + rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
uint128_t operator-(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) - rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
uint128_t operator*(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) * rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
uint128_t operator/(T lhs, const uint128_t& rhs) {
    return uint128_t(lhs) / rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
uint128_t operator%(T lhs, const uint128_t& rhs) {
    return uint128_t(lhs) % rhs;
}


// NON-MEMBER COMPARISON OPERATORS
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator==(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) == rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator!=(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) != rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator<(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) < rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator>(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) > rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator<=(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) <= rhs;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool operator>=(T lhs, const uint128_t& rhs) noexcept {
    return uint128_t(lhs) >= rhs;
}

#endif // UINT128_T_HPP