#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
// Includes para intrínsecos de GCC/Clang
#include <immintrin.h>
#if defined(__x86_64__) || defined(_M_X64)
#include <x86intrin.h>
#endif
#endif

class uint128_t
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

    uint64_t data[2];
    // data[1] -> High 64 bits
    // data[0] -> Low 64 bits

  public:
    static constexpr int UINT128_BITS = 2 * sizeof(uint64_t) * CHAR_BIT; // 128

    /// Accessors
    constexpr uint64_t high() const noexcept
    {
        return data[1];
    }
    constexpr uint64_t low() const noexcept
    {
        return data[0];
    }

    // Setters
    template <typename T> constexpr void set_high(T value) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        data[1] = static_cast<uint64_t>(value);
    }

    template <typename T> constexpr void set_low(T value) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        data[0] = static_cast<uint64_t>(value);
    }

    // CONSTRUCTORS
    constexpr uint128_t() noexcept : data{0ull, 0ull} {}

    template <typename T1, typename T2>
    constexpr uint128_t(T1 _high, T2 _low) noexcept
        : data{static_cast<uint64_t>(_low), static_cast<uint64_t>(_high)}
    {
        static_assert(std::is_integral<T1>::value && std::is_integral<T2>::value,
                      "T1 and T2 must be integral types");
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    constexpr uint128_t(T _low) noexcept : data{static_cast<uint64_t>(_low), 0ull}
    {
    }

    constexpr uint128_t(const uint128_t& other) noexcept : data{other.data[0], other.data[1]} {}
    constexpr uint128_t(uint128_t&& other) noexcept : data{other.data[0], other.data[1]} {}

    // Constructor desde string (definido después de la clase)
    explicit uint128_t(const std::string& str);

    // ASSIGNMENT OPERATORS
    constexpr uint128_t& operator=(const uint128_t& other) noexcept
    {
        if (this != &other) {
            data[0] = other.data[0];
            data[1] = other.data[1];
        }
        return *this;
    }

    constexpr uint128_t& operator=(uint128_t&& other) noexcept
    {
        if (this != &other) {
            data[1] = other.data[1];
            data[0] = other.data[0];
        }
        return *this;
    }

    template <typename T> constexpr uint128_t& operator=(T _low) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        data[1] = 0ull;
        data[0] = static_cast<uint64_t>(_low);
        return *this;
    }

    // CONVERSIONS
    explicit constexpr operator bool() const noexcept
    {
        return (data[1] != 0) || (data[0] != 0);
    }

    template <typename TYPE> explicit constexpr operator TYPE() const noexcept
    {
        static_assert(std::is_integral<TYPE>::value && (sizeof(TYPE) <= 8),
                      "TYPE must be an integral type of 8 bytes or less");
        return static_cast<TYPE>(data[0]);
    }

    // ARITHMETIC OPERATORS
    uint128_t& operator++() noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, data[0], 1ull, &new_low);
        data[0] = new_low;
        if (carry) {
            data[1] += 1ull;
        }
#else
        // GCC/Clang y otros: usar el método confiable sin intrínsecos
        if (++data[0] == 0) {
            ++data[1];
        }
#endif
        return *this;
    }

    uint128_t operator++(int) noexcept
    {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    uint128_t& operator--() noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, data[0], 1ull, &new_low);
        data[0] = new_low;
        if (borrow) {
            data[1] -= 1ull;
        }
#else
        // GCC/Clang y otros: usar el método confiable sin intrínsecos
        if (data[0]-- == 0) {
            --data[1];
        }
#endif
        return *this;
    }

    uint128_t operator--(int) noexcept
    {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    uint128_t& operator+=(const uint128_t& other) noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _addcarry_u64(carry, data[1], other.data[1], &data[1]);
#elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang: usar builtin cuando sea posible
#if defined(__has_builtin)
#if __has_builtin(__builtin_addcll) && defined(__x86_64__)
        unsigned long long carry;
        data[0] = __builtin_addcll(data[0], other.data[0], 0, &carry);
        data[1] = __builtin_addcll(data[1], other.data[1], carry, &carry);
#else
        // Fallback: método tradicional confiable
        uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
#else
        // Fallback: método tradicional confiable
        uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
#else
        // Fallback genérico
        uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
        return *this;
    }

    uint128_t& operator-=(const uint128_t& other) noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _subborrow_u64(borrow, data[1], other.data[1], &data[1]);
#elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang: usar builtin cuando sea posible
#if defined(__has_builtin)
#if __has_builtin(__builtin_subcll) && defined(__x86_64__)
        unsigned long long borrow;
        data[0] = __builtin_subcll(data[0], other.data[0], 0, &borrow);
        data[1] = __builtin_subcll(data[1], other.data[1], borrow, &borrow);
#else
        // Fallback: método tradicional confiable
        uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low) {
            --data[1];
        }
#endif
#else
        // Fallback: método tradicional confiable
        uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low) {
            --data[1];
        }
#endif
#else
        // Fallback genérico
        uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low) {
            --data[1];
        }
#endif
        return *this;
    }

    // FUNCIONES AUXILIARES PARA DIVISIÓN

    // Cuenta cuántos ceros hay al inicio (MSB) del número
    int leading_zeros() const noexcept
    {
        if (data[1] != 0) {
#ifdef _MSC_VER
            return static_cast<int>(__lzcnt64(data[1]));
#else
            return __builtin_clzll(data[1]);
#endif
        } else if (data[0] != 0) {
#ifdef _MSC_VER
            return 64 + static_cast<int>(__lzcnt64(data[0]));
#else
            return 64 + __builtin_clzll(data[0]);
#endif
        } else {
            return 128;
        }
    }

    // Calcula la longitud efectiva (bits significativos)
    int effective_length() const noexcept
    {
        return 128 - leading_zeros();
    }

    // Desplaza a la izquierda
    uint128_t shift_left(int positions) const noexcept
    {
        if (positions <= 0)
            return *this;
        if (positions >= 128)
            return uint128_t(0, 0);

        if (positions >= 64) {
            return uint128_t(data[0] << (positions - 64), 0);
        } else {
            uint64_t new_high = (data[1] << positions) | (data[0] >> (64 - positions));
            uint64_t new_low = data[0] << positions;
            return uint128_t(new_high, new_low);
        }
    }

    // Desplaza a la derecha
    uint128_t shift_right(int positions) const noexcept
    {
        if (positions <= 0)
            return *this;
        if (positions >= 128)
            return uint128_t(0, 0);

        if (positions >= 64) {
            return uint128_t(0, data[1] >> (positions - 64));
        } else {
            uint64_t new_low = (data[0] >> positions) | (data[1] << (64 - positions));
            uint64_t new_high = data[1] >> positions;
            return uint128_t(new_high, new_low);
        }
    }

    // COMPARISON OPERATORS
    bool operator==(const uint128_t& other) const noexcept
    {
        return (data[0] == other.data[0]) && (data[1] == other.data[1]);
    }

    bool operator!=(const uint128_t& other) const noexcept
    {
        return !(*this == other);
    }

    bool operator<(const uint128_t& other) const noexcept
    {
        if (data[1] != other.data[1]) {
            return data[1] < other.data[1];
        }
        return data[0] < other.data[0];
    }

    bool operator<=(const uint128_t& other) const noexcept
    {
        return (*this < other) || (*this == other);
    }

    bool operator>(const uint128_t& other) const noexcept
    {
        return !(*this <= other);
    }

    bool operator>=(const uint128_t& other) const noexcept
    {
        return !(*this < other);
    }

  private:
    // Normalizar divisor para tener la misma longitud efectiva que el dividendo
    std::pair<uint128_t, int> normalize_divisor(const uint128_t& dividendo) const noexcept
    {
        int dividend_length = dividendo.effective_length();
        int divisor_length = this->effective_length();
        int shift_amount = dividend_length - divisor_length;

        if (shift_amount <= 0) {
            return std::make_pair(*this, 0);
        }

        return std::make_pair(this->shift_left(shift_amount), shift_amount);
    }

  public:
    // OPERADORES DE DIVISIÓN

    // División con resto - función base para /= y %=
    std::optional<std::pair<uint128_t, uint128_t>> divrem(const uint128_t& divisor) const noexcept
    {
        // Detectar división por cero
        if (divisor == uint128_t(0, 0)) {
            return std::nullopt;
        }

        uint128_t quotient(0, 0);
        uint128_t remainder(0, 0);

        // Casos especiales
        if (*this < divisor) {
            remainder = *this;
            return std::make_pair(quotient, remainder);
        } else if (*this == divisor) {
            quotient = uint128_t(0, 1);
            return std::make_pair(quotient, remainder);
        } else if (divisor == uint128_t(0, 1)) {
            quotient = *this;
            return std::make_pair(quotient, remainder);
        }

        // CASO GENERAL: Algoritmo de división larga binaria
        auto normalized_pair = divisor.normalize_divisor(*this);
        uint128_t normalized_divisor = normalized_pair.first;
        int shift_amount = normalized_pair.second;

        remainder = *this;
        quotient = uint128_t(0, 0);

        // Bucle principal de división binaria
        for (int i = shift_amount; i >= 0; i--) {
            quotient = quotient.shift_left(1);

            if (remainder >= normalized_divisor) {
                remainder = remainder - normalized_divisor;
                quotient.data[0] |= 1; // Establecer bit menos significativo
            }

            normalized_divisor = normalized_divisor.shift_right(1);
        }

        return std::make_pair(quotient, remainder);
    }

    // División con assignment
    uint128_t& operator/=(const uint128_t& other) noexcept
    {
        auto result = divrem(other);
        if (result.has_value()) {
            *this = result.value().first; // quotient
        } else {
            *this = uint128_t(0, 0); // División por cero -> 0
        }
        return *this;
    }

    // División con assignment para tipos integrales
    template <typename T> uint128_t& operator/=(T other) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return *this /= uint128_t(other);
    }

    // Módulo con assignment
    uint128_t& operator%=(const uint128_t& other) noexcept
    {
        auto result = divrem(other);
        if (result.has_value()) {
            *this = result.value().second; // remainder
        } else {
            *this = uint128_t(0, 0); // División por cero -> 0
        }
        return *this;
    }

    // Módulo con assignment para tipos integrales
    template <typename T> uint128_t& operator%=(T other) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return *this %= uint128_t(other);
    }

    // División operator (uses operator/=)
    uint128_t operator/(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result /= other;
        return result;
    }

    // División operator para tipos integrales (uses operator/=)
    template <typename T> uint128_t operator/(T other) const noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        uint128_t result(*this);
        result /= other;
        return result;
    }

    // Módulo operator (uses operator%=)
    uint128_t operator%(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result %= other;
        return result;
    }

    // Módulo operator para tipos integrales (uses operator%=)
    template <typename T> uint128_t operator%(T other) const noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        uint128_t result(*this);
        result %= other;
        return result;
    }

    // OTHER ARITHMETIC OPERATORS
    uint128_t operator+(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result += other;
        return result;
    }

    uint128_t operator-(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Multiplicación básica (para verificación)
    uint128_t& operator*=(const uint128_t& other) noexcept
    {
        // Implementación básica usando descomposición
        uint64_t a_low = data[0] & 0xFFFFFFFF;
        uint64_t a_high = data[0] >> 32;
        uint64_t b_low = other.data[0] & 0xFFFFFFFF;
        uint64_t b_high = other.data[0] >> 32;

        uint64_t result0 = a_low * b_low;
        uint64_t result1 = a_high * b_low + a_low * b_high;
        uint64_t result2 = a_high * b_high;

        result1 += result0 >> 32;
        result2 += result1 >> 32;

        data[0] = (result1 << 32) | (result0 & 0xFFFFFFFF);
        data[1] = result2 + (data[1] * other.data[0]) + (data[0] * other.data[1]);

        return *this;
    }

    uint128_t operator*(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // BITWISE OPERATORS
    uint128_t& operator<<=(int shift) noexcept
    {
        *this = shift_left(shift);
        return *this;
    }

    uint128_t& operator>>=(int shift) noexcept
    {
        *this = shift_right(shift);
        return *this;
    }

    uint128_t operator<<(int shift) const noexcept
    {
        return shift_left(shift);
    }

    uint128_t operator>>(int shift) const noexcept
    {
        return shift_right(shift);
    }

    uint128_t& operator&=(const uint128_t& other) noexcept
    {
        data[0] &= other.data[0];
        data[1] &= other.data[1];
        return *this;
    }

    uint128_t& operator|=(const uint128_t& other) noexcept
    {
        data[0] |= other.data[0];
        data[1] |= other.data[1];
        return *this;
    }

    uint128_t& operator^=(const uint128_t& other) noexcept
    {
        data[0] ^= other.data[0];
        data[1] ^= other.data[1];
        return *this;
    }

    uint128_t operator&(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    uint128_t operator|(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    uint128_t operator^(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    uint128_t operator~() const noexcept
    {
        return uint128_t(~data[1], ~data[0]);
    }

    // STRING CONVERSION (basic implementation)
    std::string to_string() const
    {
        if (*this == uint128_t(0, 0)) {
            return "0";
        }

        std::string result;
        uint128_t temp = *this;

        while (temp != uint128_t(0, 0)) {
            auto divmod = temp.divrem(uint128_t(0, 10));
            if (divmod.has_value()) {
                char digit = '0' + static_cast<char>(static_cast<uint64_t>(divmod.value().second));
                result = digit + result;
                temp = divmod.value().first;
            } else {
                break;
            }
        }

        return result.empty() ? "0" : result;
    }

    // Función estática para convertir string a uint128_t
    static uint128_t from_string(const std::string& str)
    {
        if (str.empty()) {
            return uint128_t(0, 0);
        }

        uint128_t result(0, 0);
        uint128_t base(0, 10);

        for (char c : str) {
            if (c < '0' || c > '9') {
                // Caracteres inválidos se ignoran (podrías lanzar excepción si prefieres)
                continue;
            }

            // result = result * 10 + (c - '0')
            result *= base;
            result += uint128_t(0, static_cast<uint64_t>(c - '0'));
        }

        return result;
    }
};

// Constructor desde string (definido después de from_string)
inline uint128_t::uint128_t(const std::string& str) : data{0ull, 0ull}
{
    *this = from_string(str);
}

// Operadores de flujo definidos fuera de la clase
inline std::ostream& operator<<(std::ostream& os, const uint128_t& value)
{
    return os << value.to_string();
}

inline std::istream& operator>>(std::istream& is, uint128_t& value)
{
    std::string str;
    is >> str;
    if (is.good() || is.eof()) {
        value = uint128_t::from_string(str);
    }
    return is;
}

// Constante MAX definida después de la clase
constexpr uint128_t uint128_t_MAX = uint128_t(UINT64_MAX, UINT64_MAX);

#endif // UINT128_T_HPP