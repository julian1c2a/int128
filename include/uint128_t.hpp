#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#include <cstdint>
#include <climits>
#include <type_traits>
#include <utility>
#include <concepts>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace append {

    using std::uint64_t;
    using std::uint32_t;
    using std::uint16_t;
    using std::uint8_t;
    using std::int64_t;
    using std::int32_t;
    using std::int16_t;
    using std::int8_t;
    using std::size_t;

struct uint128_t {
    static constexpr int UINT128_BITS = 128;

    uint64_t low;
    uint64_t high;

#if !defined(_MSC_VER)
    // Constructor from __int128_t (signed)
    constexpr uint128_t(__int128_t value) noexcept
        : low(static_cast<uint64_t>(static_cast<unsigned __int128>(value))),
          high(static_cast<uint64_t>(static_cast<unsigned __int128>(value) >> 64)) {}

    // Constructor from unsigned __int128
    constexpr uint128_t(unsigned __int128 value) noexcept
        : low(static_cast<uint64_t>(value)),
          high(static_cast<uint64_t>(value >> 64)) {}
#endif

    // CONSTRUCTORS

    // Default constructor
    constexpr uint128_t() noexcept : low(0ull), high(0ull) {}

    // Constructor from two uint64_t values
    constexpr uint128_t(uint64_t _high, uint64_t _low) noexcept : low(_low), high(_high) {}

    // Constructor from only one uint64_t value
    template<typename T, typename = std::enable_if_t<std::is_unsigned<T>::value && std::is_integral<T>::value>>
    constexpr uint128_t(T _low) noexcept : 
        low(static_cast<uint64_t>(_low)), high(0ull) {}

    // Copy Constructor
    constexpr uint128_t(const uint128_t& other) noexcept 
        : low(other.low), high(other.high) {}

    // Move Constructor
    constexpr uint128_t(uint128_t&& other) noexcept 
        : uint128_t(other.high, other.low) {}

    // ASSIGMENT OPERATORS
    // Copy Assignment Operator
    constexpr uint128_t& operator=(const uint128_t& other) noexcept {
        if (this == &other) {
            return (*this);
        }
        high = other.high;
        low = other.low;
        return (*this);
    }

    // CONVERSIONS
    
    // Explicit conversion to bool
    explicit constexpr operator bool() const noexcept {
        return (high != 0) || (low != 0);
    }

    // CONVERSION FROM uint128_t TO integral TYPE
    template<typename TYPE, typename = std::enable_if_t<std::is_integral<TYPE>::value>>
    explicit constexpr operator TYPE() const noexcept {
        if constexpr (std::is_same<TYPE, bool>::value) {
            return (high != 0) || (low != 0);
        } else if constexpr (std::is_same<TYPE, uint64_t>::value) {
            return low;
        } else if constexpr (std::is_same<TYPE, int32_t>::value) {
            return static_cast<int32_t>(low);
        } else if constexpr (std::is_same<TYPE, uint32_t>::value) {
            return static_cast<uint32_t>(low);
        } else if constexpr (std::is_same<TYPE, int16_t>::value) {
            return static_cast<int16_t>(low);
        } else if constexpr (std::is_same<TYPE, uint16_t>::value) {
            return static_cast<uint16_t>(low);
        } else if constexpr (std::is_same<TYPE, int8_t>::value) {
            return static_cast<int8_t>(low);
        } else if constexpr (std::is_same<TYPE, uint8_t>::value) {
            return static_cast<uint8_t>(low);
        } else if constexpr (std::is_same<TYPE, int64_t>::value) {
            return static_cast<int64_t>(low);
        }  else {
            static_assert(sizeof(TYPE) > 8, "Conversion to this type is not supported.");
        }
    }

    // Move Assignment Operator
    // Tipos simples como uint64_t no necesitan realmente un move assignment,
    // pero se incluye aquí para completar la implementación.
    constexpr uint128_t& operator=(uint128_t&& other) noexcept {
        if (this == &other) {
            return (*this);
        } 
        high = other.high;
        low = other.low;
        return (*this);
    }

    // Copy and Move Assigment Operator from uint64_t object
    template<typename T, typename = std::enable_if_t<std::is_unsigned<T>::value && std::is_integral<T>::value>>
    constexpr uint128_t& operator=(T _low) noexcept {
        high = 0ull;
        low = static_cast<uint64_t>(_low);
        return (*this);
    
    }

#ifdef _MSC_VER
    // PRE-INCREMENT OPERATOR
    uint128_t& operator++() noexcept {
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, low, 1ull, &new_low);
        low = new_low;
        if (carry) {
            high += 1ull;
        }
        return (*this);
    }

    // POST-INCREMENT OPERATOR
    uint128_t operator++(int) noexcept {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    // PRE-DECREMENT OPERATOR
    uint128_t& operator--() noexcept {
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, low, 1ull, &new_low);
        low = new_low;
        if (borrow) {
            high -= 1ull;
        }
        return (*this);
    }

    // POST-DECREMENT OPERATOR
    uint128_t operator--(int) noexcept {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    // Addition with assigment operator
    uint128_t& operator+=(const uint128_t& other) noexcept {
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, low, other.low, &new_low);
        low = new_low;
        _addcarry_u64(carry, high, other.high, &high);
        return (*this);
    }

    // Subtraction with assigment operator
    uint128_t& operator-=(const uint128_t& other) noexcept {
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, low, other.low, &new_low);
        low = new_low;
        _subborrow_u64(borrow, high, other.high, &high);
        return (*this);
    }

    // Multiplication with assignment operator
    uint128_t& operator*=(const uint128_t& other) noexcept {
        uint64_t result_high;
        low = _umul128(low, other.low, &result_high);
        high = result_high + high * other.low + low * other.high;
        return (*this);
    }

#else // GCC/Clang specific implementation

    // PRE-INCREMENT OPERATOR
    uint128_t& operator++() noexcept {
        __int128_t temp = static_cast<__int128_t>(*this);
        temp++;
        (*this) = uint128_t(temp);
        return (*this);
    }

    // POST-INCREMENT OPERATOR
    uint128_t operator++(int) noexcept {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    // PRE-DECREMENT OPERATOR
    uint128_t& operator--() noexcept {
        __int128_t temp = static_cast<__int128_t>(*this);
        temp--;
        (*this) = uint128_t(temp);
        return (*this);
    }

    // POST-DECREMENT OPERATOR
    uint128_t operator--(int) noexcept {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    // Addition with assigment operator
    uint128_t& operator+=(const uint128_t& other) noexcept {
        unsigned __int128 tmp = (static_cast<unsigned __int128>(high) << 64) | low;
        unsigned __int128 other_tmp = (static_cast<unsigned __int128>(other.high) << 64) | other.low;
        tmp += other_tmp;
        high = tmp >> 64;
        low = static_cast<uint64_t>(tmp);
        return *this;
    }
    
    // Subtraction with assigment operator
    uint128_t& operator-=(const uint128_t& other) noexcept {
        unsigned __int128 tmp = (static_cast<unsigned __int128>(high) << 64) | low;
        unsigned __int128 other_tmp = (static_cast<unsigned __int128>(other.high) << 64) | other.low;
        tmp -= other_tmp;
        high = tmp >> 64;
        low = static_cast<uint64_t>(tmp);
        return (*this);
    }
    
    // Multiplication with assignment operator
    uint128_t& operator*=(const uint128_t& other) noexcept {
        unsigned __int128 tmp = (static_cast<unsigned __int128>(high) << 64) | low;
        unsigned __int128 other_tmp = (static_cast<unsigned __int128>(other.high) << 64) | other.low;
        tmp *= other_tmp;
        high = tmp >> 64;
        low = static_cast<uint64_t>(tmp);
        return *this;
    }

#endif

    // Addition with assigment operator for integral types
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint128_t& operator+=(T other) noexcept {
        if constexpr (std::is_unsigned<T>::value) {
            // Unsigned integral type
            *this += uint128_t(0, static_cast<uint64_t>(other));
            return *this;
        } else {
            // Signed integral type
            if (other >= 0) {
                // Positive value: normal addition
                (*this) += uint128_t(0, static_cast<uint64_t>(other));
                return (*this);
            } else {
                // Negative value: subtraction of absolute value
                (*this) -= uint128_t(0, static_cast<uint64_t>(-other));
                return (*this);
            }
        }
    }

// Subtraction with assigment operator for integral types
template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
uint128_t& operator-=(T other) noexcept {
    if constexpr (std::is_unsigned<T>::value) {
        // Unsigned integral type
        (*this) -= uint128_t(0, static_cast<uint64_t>(other));
        return (*this);
    } else {
        // Signed integral type
        if (other >= 0) {
            // Positive value: normal subtraction
            (*this) -= uint128_t(0, static_cast<uint64_t>(other));
            return *this;
        } else {
            // Negative value: addition of absolute value
            (*this) += uint128_t(0, static_cast<uint64_t>(-other));
            return (*this);
        }
    }
}

    // Multiplication with assignment operator from an integral type
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint128_t& operator*=(T other) noexcept {
        if constexpr (std::is_unsigned<T>::value) {
            // Unsigned Integral Type
            (*this) *= uint128_t(0, static_cast<uint64_t>(other));
            return (*this);
        } else {
            // Signed Integral Type
            if (other >= 0) {
                // Positive value
                (*this) *= uint128_t(0, static_cast<uint64_t>(other));
                return (*this);
            } else {
                // Negative value
                // Multiplying by a negative number results in zero for uint128_t
                high = 0ull;
                low = 0ull;
                return (*this);
            }
        }
    }

    // Addition operator (uses operator+=)
    uint128_t operator+(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result += other;
        return result;
    }

    // Addition operator for integral types (uses operator+=)
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint128_t operator+(T other) const noexcept {
        uint128_t result(*this);
        result += other;
        return result;
    }

    // Subtraction operator (uses operator-=)
    uint128_t operator-(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Subtraction operator for integral types (uses operator-=)
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint128_t operator-(T other) const noexcept {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Multiplication operator (uses operator*=)
    uint128_t operator*(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // Multiplication operator (uses operator*=)
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint128_t operator*(T other) const noexcept {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // Equality operator
    bool operator==(uint128_t other) const {
        return (high == other.high) && (low == other.low);
    }

    // Equality operator for integral types
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator==(T other) const {
        if constexpr (std::is_unsigned<T>::value) {
            // Unsigned integral type
            return (high == 0) && (low == static_cast<uint64_t>(other));
        } else {
            // Signed integral type
            if (other >= 0) {
                return (high == 0) && (low == static_cast<uint64_t>(other));
            } else {
                return false; // uint128_t can never be equal to a negative integer
            }
        }
    }

    // Less than operator
    bool operator<(const uint128_t& other) const {
        if (high < other.high) return true;
        if (high > other.high) return false;
        return low < other.low;
    }

    // Less than operator for integral types
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator<(T other) const {
        if constexpr (std::is_unsigned<T>::value) {
            // Unsigned integral type
            if (high != 0) return false;
            return low < static_cast<uint64_t>(other);
        } else {
            // Signed integral type
            if (other >= 0) {
                // Positive value
                if (high != 0) return false;
                return low < static_cast<uint64_t>(other);
            } else {
                // Negative value
                return false; // uint128_t can never be less than a negative integer
            }
        }
    }

    // Additional comparison operators
    bool operator!=(const uint128_t& other) const {
        return !((*this) == other);
    }

    bool operator>(const uint128_t& other) const {
        return other < (*this);
    }

    bool operator<=(const uint128_t& other) const {
        return !((*this) > other);
    }

    bool operator>=(const uint128_t& other) const {
        return !((*this) < other);
    }

    // Additional comparison operators from integral types
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator!=(T other) const {
        return !((*this) == other);
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator>(T other) const {
        return other < (*this);
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator<=(T other) const {
        return !((*this) > other);
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator>=(T other) const {
        return !((*this) < other);
    }
    
    // BITWISE OPERATORS WITH ASSIGNMENT

    // Bitwise AND with assignment
    uint128_t& operator&=(const uint128_t& other) noexcept {
        high &= other.high;
        low &= other.low;
        return (*this);
    }

    // Bitwise OR with assignment
    uint128_t& operator|=(const uint128_t& other) noexcept {
        high |= other.high;
        low |= other.low;
        return (*this);
    }

    // Bitwise XOR with assignment
    uint128_t& operator^=(const uint128_t& other) noexcept {
        high ^= other.high;
        low ^= other.low;
        return (*this);
    }

    // Left shift with assignment
    uint128_t& operator<<=(unsigned int shift) noexcept {
        if (shift == 0) return *this;
        if (shift >= 128) {
            high = low = 0;
            return (*this);
        }
        if (shift >= 64) {
            high = low << (shift - 64);
            low = 0;
        } else {
            high = (high << shift) | (low >> (64 - shift));
            low <<= shift;
        }
        return (*this);
    }

    // Right shift with assignment
    uint128_t& operator>>=(unsigned int shift) noexcept {
        if (shift == 0) { 
            return (*this); 
        }
        if (shift >= 128) {
            high = low = 0;
            return (*this);
        }
        if (shift >= 64) {
            low = high >> (shift - 64);
            high = 0;
        } else {
            low = (low >> shift) | (high << (64 - shift));
            high >>= shift;
        }
        return (*this);
    }

    // BINARY BITWISE OPERATORS (using assignment operators)

    // Bitwise AND
    uint128_t operator&(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    // Bitwise OR
    uint128_t operator|(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    // Bitwise XOR
    uint128_t operator^(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    // Bitwise NOT
    uint128_t operator~() const noexcept {
        return uint128_t(~high, ~low);
    }

    // Left shift
    uint128_t operator<<(unsigned int shift) const noexcept {
        uint128_t result(*this);
        result <<= shift;
        return result;
    }

    // Right shift
    uint128_t operator>>(unsigned int shift) const noexcept {
        uint128_t result(*this);
        result >>= shift;
        return result;
    }

    // Unary minus (negation)
    uint128_t operator-() const noexcept {
        uint128_t result = ~(*this);
        ++result;
        return result;
    }
};
} // local close of namespace append
#endif // UINT128_T_HPP