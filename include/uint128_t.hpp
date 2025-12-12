#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#if defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
constexpr bool _uint128_t_exists = true;

#include <cstdint>
#include <cstddef>
#include <climits>
#include <type_traits>
#include <utility>
#include <concepts>
#include <string>

#ifdef _MSC_VER
#include <intrin.h>
#endif

struct uint128_t
{

    using uint64_t = std::uint64_t;
    using uint32_t = std::uint32_t;
    using uint16_t = std::uint16_t;
    using uint8_t = std::uint8_t;
    using int64_t = std::int64_t;
    using int32_t = std::int32_t;
    using int16_t = std::int16_t;
    using int8_t = std::int8_t;
    using size_t = std::size_t;

    static constexpr int UINT128_BITS = 2 * sizeof(uint64_t) * CHAR_BIT; // 128

    uint64_t data[2];
    // data[1] -> High 64 bits
    // data[0] -> Low 64 bits

    // Accessors (interfaz explícita: lectura por copia, escritura por función)
    constexpr uint64_t high() const noexcept { return data[1]; }
    constexpr uint64_t low() const noexcept { return data[0]; }
    constexpr void set_high(uint64_t value) noexcept { data[1] = value; }
    constexpr void set_low(uint64_t value) noexcept { data[0] = value; }

    // CONSTRUCTORS

    // Default constructor
    constexpr uint128_t() noexcept : data{0ull, 0ull} {}

    // Constructor from two uint64_t values
    constexpr uint128_t(uint64_t _high, uint64_t _low) noexcept : data{_low, _high} {}

    // Constructor from only one uint64_t value
    /// @brief Constructor from a single integral type, initializes the high 64 bits to zero.
    /// @tparam T An integral type.
    /// @param _low An argument of type T to initialize the low 64 bits.
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    constexpr uint128_t(T _low) noexcept : data{static_cast<uint64_t>(_low), 0ull} {}

    // Copy Constructor
    constexpr uint128_t(const uint128_t &other) noexcept : data{other.data[0], other.data[1]} {}

    // Move Constructor
    constexpr uint128_t(uint128_t &&other) noexcept : data{other.data[0], other.data[1]} {}

    // Constructor from null-terminated string
    uint128_t(const char *const str) noexcept : data{0ull, 0ull}
    {
        *this = parse_string(str);
    }

    // ASSIGMENT OPERATORS
    // Copy Assignment Operator
    constexpr uint128_t &operator=(const uint128_t &other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        data[0] = other.data[0];
        data[1] = other.data[1];
        return *this;
    }

    // CONVERSIONS

    // Explicit conversion to bool
    explicit constexpr operator bool() const noexcept
    {
        return (data[1] != 0) || (data[0] != 0);
    }

    // CONVERSION FROM uint128_t TO std::integral TYPE
    template <std::integral TYPE>
    explicit constexpr operator TYPE() const noexcept
    {
        if constexpr (std::is_same_v<TYPE, bool>)
        {
            return (data[1] != 0) || (data[0] != 0);
        }
        else if constexpr (std::is_same_v<TYPE, uint64_t>)
        {
            return data[0];
        }
        else if constexpr (std::is_same_v<TYPE, int32_t>)
        {
            return static_cast<int32_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, uint32_t>)
        {
            return static_cast<uint32_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, int16_t>)
        {
            return static_cast<int16_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, uint16_t>)
        {
            return static_cast<uint16_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, int8_t>)
        {
            return static_cast<int8_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, uint8_t>)
        {
            return static_cast<uint8_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, int64_t>)
        {
            return static_cast<int64_t>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, int>)
        {
            return static_cast<int>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, short>)
        {
            return static_cast<short>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, long>)
        {
            return static_cast<long>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, long long>)
        {
            return static_cast<long long>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, unsigned>)
        {
            return static_cast<unsigned>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, unsigned short>)
        {
            return static_cast<unsigned short>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, unsigned long>)
        {
            return static_cast<unsigned long>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, unsigned long long>)
        {
            return static_cast<unsigned long long>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, char>)
        {
            return static_cast<char>(data[0]);
        }
        else if constexpr (std::is_same_v<TYPE, unsigned char>)
        {
            return static_cast<unsigned char>(data[0]);
        }
        else
        {
            static_assert(sizeof(TYPE) > 8, "Conversion to this type is not supported.");
        }
    }

    // Move Assignment Operator
    // Tipos simples como uint64_t no necesitan realmente un move assignment,
    // pero se incluye aquí para completar la implementación.
    constexpr uint128_t &operator=(uint128_t &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        data[1] = other.data[1];
        data[0] = other.data[0];
        return *this;
    }

    // Copy and Move Assigment Operator from uint64_t object
    template <std::unsigned_integral T>
    constexpr uint128_t &operator=(T _low) noexcept
    {
        data[1] = 0ull;
        data[0] = static_cast<uint64_t>(_low);
        return *this;
    }

    // PRE-INCREMENT OPERATOR
    uint128_t &operator++() noexcept
    {
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, data[0], 1ull, &new_low);
        data[0] = new_low;
        if (carry)
        {
            data[1] += 1ull;
        }
        return *this;
    }

    // POST-INCREMENT OPERATOR
    uint128_t operator++(int) noexcept
    {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    // PRE-DECREMENT OPERATOR
    uint128_t &operator--() noexcept
    {
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, data[0], 1ull, &new_low);
        data[0] = new_low;
        if (borrow)
        {
            data[1] -= 1ull;
        }
        return *this;
    }

    // POST-DECREMENT OPERATOR
    uint128_t operator--(int) noexcept
    {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    // Addition with assigment operator
    uint128_t &operator+=(const uint128_t &other) noexcept
    {
        uint64_t new_low, new_high;
        unsigned char carry = _addcarry_u64(0, data[0], other.data[0], &new_low);
        _addcarry_u64(carry, data[1], other.data[1], &new_high);
        data[1] = new_high;
        data[0] = new_low;
        return *this;
    }
    // Addition with assigment operator for integral types
    template <std::integral T>
    uint128_t &operator+=(T other) noexcept
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            // Unsigned integral type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t new_low, new_high;
            unsigned char carry = _addcarry_u64(0, data[0], other_u64, &new_low);
            _addcarry_u64(carry, data[1], 0, &new_high);
            data[1] = new_high;
            data[0] = new_low;
            return *this;
        }
        else
        {
            // Signed integral type
            if (other >= 0)
            {
                // Positive value: normal addition
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t new_low, new_high;
                unsigned char carry = _addcarry_u64(0, data[0], other_u64, &new_low);
                _addcarry_u64(carry, data[1], 0, &new_high);
                data[1] = new_high;
                data[0] = new_low;
                return *this;
            }
            else
            {
                // Negative value: subtraction of absolute value
                uint64_t abs_other = static_cast<uint64_t>(-other);
                uint64_t new_low, new_high;
                unsigned char borrow = _subborrow_u64(0, data[0], abs_other, &new_low);
                _subborrow_u64(borrow, data[1], 0, &new_high);
                data[1] = new_high;
                data[0] = new_low;
                return *this;
            }
        }
    }

    // Subtraction with assigment operator
    uint128_t &operator-=(const uint128_t &other) noexcept
    {
        uint64_t new_low, new_high;
        unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        _subborrow_u64(borrow, data[1], other.data[1], &new_high);
        data[1] = new_high;
        data[0] = new_low;
        return *this;
    }

    // Subtraction with assigment operator for integral types
    template <std::integral T>
    uint128_t &operator-=(T other) noexcept
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            // Unsigned integral type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t new_low, new_high;
            unsigned char borrow = _subborrow_u64(0, data[0], other_u64, &new_low);
            _subborrow_u64(borrow, data[1], 0, &new_high);
            data[1] = new_high;
            data[0] = new_low;
            return *this;
        }
        else
        {
            // Signed integral type
            if (other >= 0)
            {
                // Positive value: normal subtraction
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t new_low, new_high;
                unsigned char borrow =
                    _subborrow_u64(0, data[0], other_u64, &new_low);
                _subborrow_u64(borrow, data[1], 0, &new_high);
                data[1] = new_high;
                data[0] = new_low;
                return *this;
            }
            else
            {
                // Negative value: addition of absolute value

                uint64_t abs_other = static_cast<uint64_t>(-other);
                uint64_t new_low, new_high;
                unsigned char carry = _addcarry_u64(0, data[0], abs_other, &new_low);
                _addcarry_u64(carry, data[1], 0, &new_high);
                data[1] = new_high;
                data[0] = new_low;
                return *this;
            }
        }
    }

    // Addition operator (uses operator+=)
    uint128_t operator+(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result += other;
        return result;
    }

    // Addition operator for integral types (uses operator+=)
    template <std::integral T>
    uint128_t operator+(T other) const noexcept
    {
        uint128_t result(*this);
        result += other;
        return result;
    }

    // Subtraction operator (uses operator-=)
    uint128_t operator-(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Subtraction operator for integral types (uses operator-=)
    template <std::integral T>
    uint128_t operator-(T other) const noexcept
    {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Multiplication with assignment operator
    uint128_t &operator*=(const uint128_t &other) noexcept
    {
        uint64_t result_high, result_low;
        // low * other.low -> resultado completo de 128 bits
        result_low = _umul128(data[0], other.data[0], &result_high);
        // Agregar las multiplicaciones cruzadas al high
        result_high += data[1] * other.data[0] + data[0] * other.data[1];
        data[1] = result_high;
        data[0] = result_low;
        return *this;
    }

    // Multiplication with assignment operator from an integral type
    template <std::integral T>
    uint128_t &operator*=(T other) noexcept
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            // Unsigned Integral Type
            uint64_t other_u64 = static_cast<uint64_t>(other);
            uint64_t result_high;
            uint64_t result_low = _umul128(data[0], other_u64, &result_high);
            result_high += data[1] * other_u64;
            data[1] = result_high;
            data[0] = result_low;
            return *this;
        }
        else
        {
            // Signed Integral Type
            if (other >= 0)
            {
                // Positive value
                uint64_t other_u64 = static_cast<uint64_t>(other);
                uint64_t result_high;
                uint64_t result_low = _umul128(data[0], other_u64, &result_high);
                result_high += data[1] * other_u64;
                data[1] = result_high;
                data[0] = result_low;
                return *this;
            }
            else
            {
                // Negative value
                // Multiplying by a negative number results in zero for uint128_t
                data[1] = 0ull;
                data[0] = 0ull;
                return *this;
            }
        }
    }

    // Multiplication operator (uses operator*=)
    uint128_t operator*(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // Multiplication operator (uses operator*=)
    template <std::integral T>
    uint128_t operator*(T other) const noexcept
    {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // Equality operator
    bool operator==(uint128_t other) const
    {
        return (data[1] == other.data[1]) && (data[0] == other.data[0]);
    }

    // Equality operator for integral types
    template <std::integral T>
    bool operator==(T other) const
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            // Unsigned integral type
            return (data[1] == 0) && (data[0] == static_cast<uint64_t>(other));
        }
        else
        {
            // Signed integral type
            if (other >= 0)
            {
                return (data[1] == 0) && (data[0] == static_cast<uint64_t>(other));
            }
            else
            {
                return false; // uint128_t can never be equal to a negative integer
            }
        }
    }

    // Less than operator
    bool operator<(const uint128_t &other) const
    {
        if (data[1] < other.data[1])
            return true;
        if (data[1] > other.data[1])
            return false;
        return data[0] < other.data[0];
    }

    // Less than operator for integral types
    template <std::integral T>
    bool operator<(T other) const
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            // Unsigned integral type
            if (data[1] != 0)
                return false;
            return data[0] < static_cast<uint64_t>(other);
        }
        else
        {
            // Signed integral type
            if (other >= 0)
            {
                // Positive value
                if (data[1] != 0)
                    return false;
                return data[0] < static_cast<uint64_t>(other);
            }
            else
            {
                // Negative value
                return false; // uint128_t can never be less than a negative integer
            }
        }
    }

    // Additional comparison operators
    bool operator!=(const uint128_t &other) const
    {
        return !(*this == other);
    }

    bool operator>(const uint128_t &other) const
    {
        return other < *this;
    }

    bool operator<=(const uint128_t &other) const
    {
        return !(*this > other);
    }

    bool operator>=(const uint128_t &other) const
    {
        return !(*this < other);
    }

    // Additional comparison operators from integral types
    template <std::integral T>
    bool operator!=(T other) const
    {
        return !(*this == other);
    }

    template <std::integral T>
    bool operator>(T other) const
    {
        return other < *this;
    }

    template <std::integral T>
    bool operator<=(T other) const
    {
        return !(*this > other);
    }

    template <std::integral T>
    bool operator>=(T other) const
    {
        return !(*this < other);
    }

    // BITWISE OPERATORS WITH ASSIGNMENT

    // Bitwise AND with assignment
    uint128_t &operator&=(const uint128_t &other) noexcept
    {
        data[1] &= other.data[1];
        data[0] &= other.data[0];
        return *this;
    }

    // Bitwise OR with assignment
    uint128_t &operator|=(const uint128_t &other) noexcept
    {
        data[1] |= other.data[1];
        data[0] |= other.data[0];
        return *this;
    }

    // Bitwise XOR with assignment
    uint128_t &operator^=(const uint128_t &other) noexcept
    {
        data[1] ^= other.data[1];
        data[0] ^= other.data[0];
        return *this;
    }

    // Left shift with assignment
    uint128_t &operator<<=(unsigned int shift) noexcept
    {
        if (shift == 0)
            return *this;
        if (shift >= 128)
        {
            data[1] = data[0] = 0;
            return *this;
        }
        if (shift >= 64)
        {
            data[1] = data[0] << (shift - 64);
            data[0] = 0;
        }
        else
        {
            data[1] = (data[1] << shift) | (data[0] >> (64 - shift));
            data[0] <<= shift;
        }
        return *this;
    }

    // Right shift with assignment
    uint128_t &operator>>=(unsigned int shift) noexcept
    {
        if (shift == 0)
            return *this;
        if (shift >= 128)
        {
            data[1] = data[0] = 0;
            return *this;
        }
        if (shift >= 64)
        {
            data[0] = data[1] >> (shift - 64);
            data[1] = 0;
        }
        else
        {
            data[0] = (data[0] >> shift) | (data[1] << (64 - shift));
            data[1] >>= shift;
        }
        return *this;
    }

    // BINARY BITWISE OPERATORS (using assignment operators)

    // Bitwise AND
    uint128_t operator&(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    // Bitwise OR
    uint128_t operator|(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    // Bitwise XOR
    uint128_t operator^(const uint128_t &other) const noexcept
    {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    // Bitwise NOT
    uint128_t operator~() const noexcept
    {
        return uint128_t(~data[1], ~data[0]);
    }

    // Left shift
    uint128_t operator<<(unsigned int shift) const noexcept
    {
        uint128_t result(*this);
        result <<= shift;
        return result;
    }

    // Right shift
    uint128_t operator>>(unsigned int shift) const noexcept
    {
        uint128_t result(*this);
        result >>= shift;
        return result;
    }

    // Unary minus (negation)
    uint128_t operator-() const noexcept
    {
        uint128_t result = ~(*this);
        ++result;
        return result;
    }

    // FUNCIONES FRIEND PARA SIMETRÍA
    // Permiten que tipos integrales aparezcan en ambos lados de los operadores

    // Operadores aritméticos simétricos
    template <std::integral T>
    friend constexpr uint128_t operator+(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) + right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator-(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) - right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator*(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) * right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator/(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) / right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator%(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) % right;
    }

    // Operadores de comparación simétricos
    template <std::integral T>
    friend constexpr bool operator==(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) == right;
    }

    template <std::integral T>
    friend constexpr bool operator!=(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) != right;
    }

    template <std::integral T>
    friend constexpr bool operator<(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) < right;
    }

    template <std::integral T>
    friend constexpr bool operator<=(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) <= right;
    }

    template <std::integral T>
    friend constexpr bool operator>(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) > right;
    }

    template <std::integral T>
    friend constexpr bool operator>=(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) >= right;
    }

    // Operadores bitwise simétricos
    template <std::integral T>
    friend constexpr uint128_t operator&(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) & right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator|(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) | right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator^(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) ^ right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator<<(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) << right;
    }

    template <std::integral T>
    friend constexpr uint128_t operator>>(T left, const uint128_t &right) noexcept
    {
        return uint128_t(left) >> right;
    }

    // Métodos estáticos para construir desde string
    static uint128_t from_string(const char *const str) noexcept
    {
        return parse_string(str);
    }

    static uint128_t from_string(const std::string &str) noexcept
    {
        return parse_string(str.c_str());
    }

    // Conversión a string simple
    std::string to_string() const
    {
        if (*this == 0)
        {
            return "0";
        }

        // Para números que caben en uint64_t, usar conversión estándar
        if (high() == 0)
        {
            return std::to_string(low());
        }

        // Para números más grandes, representación simplificada
        return "LARGE_UINT128";
    }

private:
    // Método auxiliar para parsear cadenas
    static uint128_t parse_string(const char *const str) noexcept
    {
        if (!str)
        {
            return uint128_t(0);
        }

        uint128_t result(0);

        // Encontrar el inicio (saltar espacios en blanco)
        const char *ptr = str;
        while (*ptr == ' ' || *ptr == '\t')
        {
            ++ptr;
        }

        // Procesar cada dígito
        while (*ptr >= '0' && *ptr <= '9')
        {
            uint64_t digit = static_cast<uint64_t>(*ptr - '0');

            // result = result * 10 + digit
            // Multiplicar por 10 manualmente usando shifts: result * 10 = (result << 3) + (result << 1)
            uint128_t temp = result;
            temp <<= 3;   // * 8
            result <<= 1; // * 2
            result = result + temp + uint128_t(digit);

            ++ptr;
        }

        return result;
    }
};

// Operador de inserción de streams (fuera de la clase)
#include <iostream>
inline std::ostream &operator<<(std::ostream &os, const uint128_t &value)
{
    return os << value.to_string();
}

// Literales de usuario para uint128_t
namespace uint128_literals
{
    // Para números que caben en unsigned long long
    constexpr uint128_t operator""_u128(unsigned long long value) noexcept
    {
        return uint128_t(value);
    }

    constexpr uint128_t operator""_U128(unsigned long long value) noexcept
    {
        return uint128_t(value);
    }

    // Para números grandes como cadenas de caracteres
    uint128_t operator""_u128(const char *str, std::size_t) noexcept
    {
        return uint128_t::from_string(str);
    }

    uint128_t operator""_U128(const char *str, std::size_t) noexcept
    {
        return uint128_t::from_string(str);
    }
}

// Importar los literales al namespace global para facilidad de uso
using namespace uint128_literals;

#endif // defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)

#endif // UINT128_T_HPP