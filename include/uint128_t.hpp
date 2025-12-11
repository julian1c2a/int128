#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#ifdef _MSC_VER
#define _UINT128_T_

#include <cstdint>
#include <climits>
#include <type_traits>
#include <utility>
#include <concepts>

#include <intrin.h>
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
    static constexpr int UINT128_BITS = sizeof(uint128_t) * CHAR_BIT; // 128

    uint64_t data[2];
    uint64_t& low{data[0]};
    uint64_t& high{data[1]};

    // CONSTRUCTORS

    // Default constructor
    constexpr uint128_t() noexcept : high(0ull), low(0ull) {}

    // Constructor from two uint64_t values
    constexpr uint128_t(uint64_t _high, uint64_t _low) noexcept : high(_high), low(_low) {}

    // Constructor from only one uint64_t value
    template<std::unsigned_integral T>
    constexpr uint128_t(T _low) noexcept : high(0ull), low(static_cast<uint64_t>(_low)) {}

    // Copy Constructor
    constexpr uint128_t(const uint128_t& other) noexcept : high(other.high), low(other.low) {}

    // Move Constructor
    constexpr uint128_t(uint128_t&& other) noexcept : uint128_t(other.high, other.low) {}

    // ASSIGMENT OPERATORS
    // Copy Assignment Operator
    constexpr uint128_t& operator=(const uint128_t& other) noexcept {
        if (this == &other) {
            return *this;
        }
        high = other.high;
        low = other.low;
        return *this;
    }

    // CONVERSIONS
    
    // Explicit conversion to bool
    explicit constexpr operator bool() const noexcept {
        return (high != 0) || (low != 0);
    }

    // CONVERSION FROM uint128_t TO std::integral TYPE
    template<std::integral TYPE>
    explicit constexpr operator TYPE() const noexcept {
        if constexpr (std::is_same_v<TYPE, bool>) {
            return (high != 0) || (low != 0);
        } else if constexpr (std::is_same_v<TYPE, uint64_t>) {
            return low;
        } else if constexpr (std::is_same_v<TYPE, int32_t>) {
            return static_cast<int32_t>(low);
        } else if constexpr (std::is_same_v<TYPE, uint32_t>) {
            return static_cast<uint32_t>(low);
        } else if constexpr (std::is_same_v<TYPE, int16_t>) {
            return static_cast<int16_t>(low);
        } else if constexpr (std::is_same_v<TYPE, uint16_t>) {
            return static_cast<uint16_t>(low);
        } else if constexpr (std::is_same_v<TYPE, int8_t>) {
            return static_cast<int8_t>(low);
        } else if constexpr (std::is_same_v<TYPE, uint8_t>) {
            return static_cast<uint8_t>(low);
        } else if constexpr (std::is_same_v<TYPE, int64_t>) {
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
            return *this;
        } 
        high = other.high;
        low = other.low;
        return *this;
    }

    // Copy and Move Assigment Operator from uint64_t object
    template<std::unsigned_integral T>
    constexpr uint128_t& operator=(T _low) noexcept {
        high = 0ull;
        low = static_cast<uint64_t>(_low);
        return *this;
    }

    // PRE-INCREMENT OPERATOR
    uint128_t& operator++() noexcept {
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, low, 1ull, &new_low);
        low = new_low;
        if (carry) {
            high += 1ull;
        }
        return *this;
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
        return *this;
    }

    // POST-DECREMENT OPERATOR
    uint128_t operator--(int) noexcept {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    // Addition with assigment operator
    uint128_t& operator+=(const uint128_t& other) noexcept {
        uint64_t new_low, new_high;
        unsigned char carry = _addcarry_u64(0, low, other.low, &new_low);
        _addcarry_u64(carry, high, other.high, &new_high);
        high = new_high;
        low = new_low;
        return *this;
    }
    // Addition with assigment operator for integral types
    template<std::integral T>
    uint128_t& operator+=(T other) noexcept {
        if constexpr (std::is_unsigned_v<T>) {
            // Unsigned integral type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t new_low, new_high;
            unsigned char carry = _addcarry_u64(0, low, other_u64, &new_low);
            _addcarry_u64(carry, high, 0, &new_high);
            high = new_high;
            low = new_low;
            return *this;
        } else {
            // Signed integral type
            if (other >= 0) {
                // Positive value: normal addition
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t new_low, new_high;
                unsigned char carry = _addcarry_u64(0, low, other_u64, &new_low);
                _addcarry_u64(carry, high, 0, &new_high);
                high = new_high;
                low = new_low;
                return *this;
            } else {
                // Negative value: subtraction of absolute value
                uint64_t abs_other = static_cast<uint64_t>(-other);
                uint64_t new_low, new_high;
                unsigned char borrow = _subborrow_u64(0, low, abs_other, &new_low);
                _subborrow_u64(borrow, high, 0, &new_high);
                high = new_high;
                low = new_low;
                return *this;
            }
        }
    }
    
    // Subtraction with assigment operator
    uint128_t& operator-=(const uint128_t& other) noexcept {
        uint64_t new_low, new_high;
        unsigned char borrow = _subborrow_u64(0, low, other.low, &new_low);
        _subborrow_u64(borrow, high, other.high, &new_high);
        high = new_high;
        low = new_low;
        return *this;
    }
    
    // Subtraction with assigment operator for integral types
    template<std::integral T>
    uint128_t& operator-=(T other) noexcept {
        if constexpr (std::is_unsigned_v<T>) {
            // Unsigned integral type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t new_low, new_high;
            unsigned char borrow = _subborrow_u64(0, low, other_u64, &new_low);
            _subborrow_u64(borrow, high, 0, &new_high);
            high = new_high;
            low = new_low;
            return *this;
        } else {
            // Signed integral type
            if (other >= 0) {
                // Positive value: normal subtraction
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t new_low, new_high;
                unsigned char borrow =
                    _subborrow_u64(0, low, other_u64, &new_low);
                _subborrow_u64(borrow, high, 0, &new_high);
                high = new_high;
                low = new_low;
                return *this;
            } else {
                // Negative value: addition of absolute value

                uint64_t abs_other = static_cast<uint64_t>(-other);
                uint64_t new_low, new_high;
                unsigned char carry = _addcarry_u64(0, low, abs_other, &new_low);
                _addcarry_u64(carry, high, 0, &new_high);
                high = new_high;
                low = new_low;
                return *this;
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
    template<std::integral T>
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
    template<std::integral T>
    uint128_t operator-(T other) const noexcept {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Multiplication with assignment operator
    uint128_t& operator*=(const uint128_t& other) noexcept {
        uint64_t result_high, result_low;
        // low * other.low -> resultado completo de 128 bits
        result_low = _umul128(low, other.low, &result_high);
        // Agregar las multiplicaciones cruzadas al high
        result_high += high * other.low + low * other.high;
        high = result_high;
        low = result_low;
        return *this;
    }

    // Multiplication with assignment operator from an integral type
    template<std::integral T>
    uint128_t& operator*=(T other) noexcept {
        if constexpr (std::is_unsigned_v<T>) {
            // Unsigned Integral Type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t result_high;
            uint64_t result_low = _umul128(low, other_u64, &result_high);
            result_high += high * other_u64;
            high = result_high;
            low = result_low;
            return *this;
        } else {
            // Signed Integral Type
            if (other >= 0) {
                // Positive value
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t result_high;
                uint64_t result_low = _umul128(low, other_u64, &result_high);
                result_high += high * other_u64;
                high = result_high;
                low = result_low;
                return *this;
            } else {
                // Negative value
                // Multiplying by a negative number results in zero for uint128_t
                high = 0ull;
                low = 0ull;
                return *this;
            }
        }
    }

    // Multiplication operator (uses operator*=)
    uint128_t operator*(const uint128_t& other) const noexcept {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // Multiplication operator (uses operator*=)
    template<std::integral T>
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
    template<std::integral T>
    bool operator==(T other) const {
        if constexpr (std::is_unsigned_v<T>) {
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
    template<std::integral T>
    bool operator<(T other) const {
        if constexpr (std::is_unsigned_v<T>) {
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
        return !(*this == other);
    }

    bool operator>(const uint128_t& other) const {
        return other < *this;
    }

    bool operator<=(const uint128_t& other) const {
        return !(*this > other);
    }

    bool operator>=(const uint128_t& other) const {
        return !(*this < other);
    }

    // Additional comparison operators from integral types
    template<std::integral T>
    bool operator!=(T other) const {
        return !(*this == other);
    }

    template<std::integral T>
    bool operator>(T other) const {
        return other < *this;
    }

    template<std::integral T>
    bool operator<=(T other) const {
        return !(*this > other);
    }

    template<std::integral T>
    bool operator>=(T other) const {
        return !(*this < other);
    }
    
    // BITWISE OPERATORS WITH ASSIGNMENT

    // Bitwise AND with assignment
    uint128_t& operator&=(const uint128_t& other) noexcept {
        high &= other.high;
        low &= other.low;
        return *this;
    }

    // Bitwise OR with assignment
    uint128_t& operator|=(const uint128_t& other) noexcept {
        high |= other.high;
        low |= other.low;
        return *this;
    }

    // Bitwise XOR with assignment
    uint128_t& operator^=(const uint128_t& other) noexcept {
        high ^= other.high;
        low ^= other.low;
        return *this;
    }

    // Left shift with assignment
    uint128_t& operator<<=(unsigned int shift) noexcept {
        if (shift == 0) return *this;
        if (shift >= 128) {
            high = low = 0;
            return *this;
        }
        if (shift >= 64) {
            high = low << (shift - 64);
            low = 0;
        } else {
            high = (high << shift) | (low >> (64 - shift));
            low <<= shift;
        }
        return *this;
    }

    // Right shift with assignment
    uint128_t& operator>>=(unsigned int shift) noexcept {
        if (shift == 0) return *this;
        if (shift >= 128) {
            high = low = 0;
            return *this;
        }
        if (shift >= 64) {
            low = high >> (shift - 64);
            high = 0;
        } else {
            low = (low >> shift) | (high << (64 - shift));
            high >>= shift;
        }
        return *this;
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
#endif // _MSC_VER
#endif // UINT128_T_HPP