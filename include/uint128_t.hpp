#ifndef UINT128_T_HPP
#define UINT128_T_HPP

#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#ifdef _MSC_VER
// Includes para intrínsecos de MSVC
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
// Includes para intrínsecos de GCC/Clang
#include <immintrin.h>
#if defined(__x86_64__) || defined(_M_X64)
#include <bmi2intrin.h>
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

    // CONSTRUCTORS - Constructor por defecto + constructor desde integrales básicos
    constexpr uint128_t() noexcept : data{0ull, 0ull} {}

    // Constructor básico para conversiones desde tipos integrales (mantiene trivialidad)
    template <typename T> constexpr uint128_t(T value) noexcept : data{0ull, 0ull}
    {
        static_assert(std::is_integral_v<T> && sizeof(T) <= 8,
                      "T must be an integral type <= 8 bytes");
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                // Para negativos en tipo unsigned, mantenemos solo la parte baja
                data[0] = static_cast<uint64_t>(value);
            } else {
                data[0] = static_cast<uint64_t>(value);
            }
        } else {
            data[0] = static_cast<uint64_t>(value);
        }
    }

    // Constructor desde dos uint64_t (necesario para operaciones internas)
    constexpr uint128_t(uint64_t high, uint64_t low) noexcept : data{low, high} {}

    // NOTA: Para construcciones más complejas, usar las funciones de fábrica en int128_factory.hpp:
    // - make_uint128(high, low) para construcción desde dos uint64_t
    // - make_uint128(str) para construcción desde string

    // ASSIGNMENT OPERATORS - Eliminados para trivialidad
    // El compilador genera versiones triviales automáticamente

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

#if defined(__SIZEOF_INT128__)
    // Conversión directa a __uint128_t para intrínsecos optimizados
    constexpr operator __uint128_t() const noexcept
    {
        return (static_cast<__uint128_t>(data[1]) << 64) | data[0];
    }
#endif

    // ARITHMETIC OPERATORS
    constexpr uint128_t& operator++() noexcept
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

    constexpr uint128_t operator++(int) noexcept
    {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    constexpr uint128_t& operator--() noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        const unsigned char borrow = _subborrow_u64(0, data[0], 1ull, &new_low);
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

    constexpr uint128_t operator--(int) noexcept
    {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    constexpr uint128_t& operator+=(const uint128_t& other) noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        const unsigned char carry = _addcarry_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _addcarry_u64(carry, data[1], other.data[1], &data[1]);
#elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang: usar builtin cuando sea posible
#if defined(__has_builtin)
#if __has_builtin(__builtin_addcll) && defined(__x86_64__)
        unsigned long long carry = 0;
        data[0] = __builtin_addcll(data[0], other.data[0], 0, &carry);
        data[1] = __builtin_addcll(data[1], other.data[1], carry, &carry);
#else
        // Fallback: método tradicional confiable
        const uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
#else
        // Fallback: método tradicional confiable
        const uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
#else
        // Fallback genérico
        const uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low) {
            ++data[1];
        }
#endif
        return *this;
    }

    constexpr uint128_t& operator-=(const uint128_t& other) noexcept
    {
#ifdef _MSC_VER
        uint64_t new_low;
        const unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _subborrow_u64(borrow, data[1], other.data[1], &data[1]);
#elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang: usar builtin cuando sea posible
#if defined(__has_builtin)
#if __has_builtin(__builtin_subcll) && defined(__x86_64__)
        unsigned long long borrow = 0;
        data[0] = __builtin_subcll(data[0], other.data[0], 0, &borrow);
        data[1] = __builtin_subcll(data[1], other.data[1], borrow, &borrow);
#else
        // Fallback: método tradicional confiable
        const uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low) {
            --data[1];
        }
#endif
#else
        // Fallback: método tradicional confiable
        const uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low) {
            --data[1];
        }
#endif
#else
        // Fallback genérico
        const uint64_t old_low = data[0];
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
    constexpr int leading_zeros() const noexcept
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
    constexpr int effective_length() const noexcept
    {
        return 128 - leading_zeros();
    }

    // Desplaza a la izquierda
    constexpr uint128_t shift_left(int positions) const noexcept
    {
        if (positions <= 0)
            return *this;
        if (positions >= 128)
            return uint128_t(0, 0);

        if (positions >= 64) {
            return uint128_t(data[0] << (positions - 64), 0);
        } else {
            const uint64_t new_high = (data[1] << positions) | (data[0] >> (64 - positions));
            const uint64_t new_low = data[0] << positions;
            return uint128_t(new_high, new_low);
        }
    }

    // Desplaza a la derecha
    constexpr uint128_t shift_right(int positions) const noexcept
    {
        if (positions <= 0)
            return *this;
        if (positions >= 128)
            return uint128_t(0, 0);

        if (positions >= 64) {
            return uint128_t(0, data[1] >> (positions - 64));
        } else {
            const uint64_t new_low = (data[0] >> positions) | (data[1] << (64 - positions));
            const uint64_t new_high = data[1] >> positions;
            return uint128_t(new_high, new_low);
        }
    }

    // COMPARISON OPERATORS
    constexpr bool operator==(const uint128_t& other) const noexcept
    {
        return (data[0] == other.data[0]) && (data[1] == other.data[1]);
    }

    constexpr bool operator!=(const uint128_t& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr bool operator<(const uint128_t& other) const noexcept
    {
        if (data[1] != other.data[1]) {
            return data[1] < other.data[1];
        }
        return data[0] < other.data[0];
    }

    constexpr bool operator<=(const uint128_t& other) const noexcept
    {
        return (*this < other) || (*this == other);
    }

    constexpr bool operator>(const uint128_t& other) const noexcept
    {
        return !(*this <= other);
    }

    constexpr bool operator>=(const uint128_t& other) const noexcept
    {
        return !(*this < other);
    }

  private:
    // Normalizar divisor para tener la misma longitud efectiva que el dividendo
    constexpr std::pair<uint128_t, int> normalize_divisor(const uint128_t& dividendo) const noexcept
    {
        const int dividend_length = dividendo.effective_length();
        const int divisor_length = this->effective_length();
        const int shift_amount = dividend_length - divisor_length;

        if (shift_amount <= 0) {
            return std::make_pair(*this, 0);
        }

        return std::make_pair(this->shift_left(shift_amount), shift_amount);
    }

  public:
    // OPERADORES DE DIVISIÓN

    // División con resto - función base para /= y %=
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
    divrem(const uint128_t& divisor) const noexcept
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
        const auto normalized_pair = divisor.normalize_divisor(*this);
        uint128_t normalized_divisor = normalized_pair.first;
        const int shift_amount = normalized_pair.second;

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
    constexpr uint128_t& operator/=(const uint128_t& other) noexcept
    {
        const auto result = divrem(other);
        if (result.has_value()) {
            *this = result.value().first; // quotient
        } else {
            *this = uint128_t(0, 0); // División por cero -> 0
        }
        return *this;
    }

    // División con assignment para tipos integrales
    template <typename T> constexpr uint128_t& operator/=(T other) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return *this /= uint128_t(other);
    }

    // Módulo con assignment
    constexpr uint128_t& operator%=(const uint128_t& other) noexcept
    {
        const auto result = divrem(other);
        if (result.has_value()) {
            *this = result.value().second; // remainder
        } else {
            *this = uint128_t(0, 0); // División por cero -> 0
        }
        return *this;
    }

    // Módulo con assignment para tipos integrales
    template <typename T> constexpr uint128_t& operator%=(T other) noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return *this %= uint128_t(other);
    }

    // División operator (uses operator/=)
    constexpr uint128_t operator/(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result /= other;
        return result;
    }

    // División operator para tipos integrales (uses operator/=)
    template <typename T> constexpr uint128_t operator/(T other) const noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        uint128_t result(*this);
        result /= other;
        return result;
    }

    // Módulo operator (uses operator%=)
    constexpr uint128_t operator%(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result %= other;
        return result;
    }

    // Módulo operator para tipos integrales (uses operator%=)
    template <typename T> constexpr uint128_t operator%(T other) const noexcept
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        uint128_t result(*this);
        result %= other;
        return result;
    }

    // OTHER ARITHMETIC OPERATORS
    constexpr uint128_t operator+(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result += other;
        return result;
    }

    constexpr uint128_t operator-(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    // Multiplicación básica (para verificación)
    constexpr uint128_t& operator*=(const uint128_t& other) noexcept
    {
        // Guardamos los valores originales antes de modificar
        const uint64_t orig_low = data[0];
        const uint64_t orig_high = data[1];

        // Implementación básica usando descomposición
        const uint64_t a_low = orig_low & 0xFFFFFFFF;
        const uint64_t a_high = orig_low >> 32;
        const uint64_t b_low = other.data[0] & 0xFFFFFFFF;
        const uint64_t b_high = other.data[0] >> 32;

        const uint64_t result0 = a_low * b_low;
        uint64_t result1 = a_high * b_low + a_low * b_high;
        uint64_t result2 = a_high * b_high;

        result1 += result0 >> 32;
        result2 += result1 >> 32;

        data[0] = (result1 << 32) | (result0 & 0xFFFFFFFF);
        data[1] = result2 + (orig_high * other.data[0]) + (orig_low * other.data[1]);

        return *this;
    }

    constexpr uint128_t operator*(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    // BITWISE OPERATORS
    constexpr uint128_t& operator<<=(int shift) noexcept
    {
        *this = shift_left(shift);
        return *this;
    }

    constexpr uint128_t& operator>>=(int shift) noexcept
    {
        *this = shift_right(shift);
        return *this;
    }

    constexpr uint128_t operator<<(int shift) const noexcept
    {
        return shift_left(shift);
    }

    constexpr uint128_t operator>>(int shift) const noexcept
    {
        return shift_right(shift);
    }

    constexpr uint128_t& operator&=(const uint128_t& other) noexcept
    {
        data[0] &= other.data[0];
        data[1] &= other.data[1];
        return *this;
    }

    constexpr uint128_t& operator|=(const uint128_t& other) noexcept
    {
        data[0] |= other.data[0];
        data[1] |= other.data[1];
        return *this;
    }

    constexpr uint128_t& operator^=(const uint128_t& other) noexcept
    {
        data[0] ^= other.data[0];
        data[1] ^= other.data[1];
        return *this;
    }

    constexpr uint128_t operator&(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    constexpr uint128_t operator|(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    constexpr uint128_t operator^(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    constexpr uint128_t operator~() const noexcept
    {
        return uint128_t(~data[1], ~data[0]);
    }

    /// Multiplicación completa: uint128_t * uint64_t -> parte alta (bits 128-191)
    /// Esta función calcula los bits superiores del resultado de multiplicar
    /// un número de 128 bits por uno de 64 bits, útil para el algoritmo de Knuth

  private:
    // Emulación optimizada de __umulh para GCC/Clang
    static inline uint64_t umulh_emulation(uint64_t a, uint64_t b) noexcept
    {
#if defined(__x86_64__) && defined(__BMI2__)
        // BMI2: usar _mulx_u64 (equivalente directo a __umulh)
        unsigned long long high_part;
        (void)_mulx_u64(a, b, &high_part);
        return high_part;

#elif defined(__SIZEOF_INT128__)
        // __uint128_t: más portable y generalmente muy eficiente
        const __uint128_t result = static_cast<__uint128_t>(a) * b;
        return static_cast<uint64_t>(result >> 64);

#elif defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))
        // Inline assembly: mapeo directo a mulq (exactamente como __umulh)
        uint64_t high_part, low_part;
        __asm__("mulq %3" : "=a"(low_part), "=d"(high_part) : "a"(a), "r"(b) : "cc");
        return high_part;

#else
        // Fallback: implementación manual optimizada
        const uint32_t a_lo = static_cast<uint32_t>(a);
        const uint32_t a_hi = static_cast<uint32_t>(a >> 32);
        const uint32_t b_lo = static_cast<uint32_t>(b);
        const uint32_t b_hi = static_cast<uint32_t>(b >> 32);

        const uint64_t p0 = static_cast<uint64_t>(a_lo) * b_lo;
        const uint64_t p1 = static_cast<uint64_t>(a_lo) * b_hi;
        const uint64_t p2 = static_cast<uint64_t>(a_hi) * b_lo;
        const uint64_t p3 = static_cast<uint64_t>(a_hi) * b_hi;

        const uint64_t middle = p1 + (p0 >> 32) + static_cast<uint32_t>(p2);

        return p3 + (middle >> 32) + (p2 >> 32);
#endif
    }

    // === FUNCIONES AUXILIARES PARA OPTIMIZACIONES ===

    // Detectar si un número es potencia de 2
    static constexpr bool is_power_of_2(const uint128_t& n) noexcept
    {
        return (n != uint128_t(0, 0)) && ((n & (n - uint128_t(0, 1))) == uint128_t(0, 0));
    }

    // Contar ceros al final (trailing zeros) para calcular la potencia de 2
    constexpr int count_trailing_zeros() const noexcept
    {
        if (data[0] != 0) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward64(&index, data[0]);
            return static_cast<int>(index);
#else
            return __builtin_ctzll(data[0]);
#endif
        } else if (data[1] != 0) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward64(&index, data[1]);
            return 64 + static_cast<int>(index);
#else
            return 64 + __builtin_ctzll(data[1]);
#endif
        } else {
            return 128; // Número es cero
        }
    }

    static constexpr int count_trailing_zeros(const uint128_t& n) noexcept
    {
        return n.count_trailing_zeros();
    }

    // Verificar si el número cabe efectivamente en 64 bits
    static constexpr bool fits_in_64_bits(const uint128_t& n) noexcept
    {
        return n.data[1] == 0;
    }

    // Extraer el valor de 64 bits cuando se sabe que cabe
    static constexpr uint64_t effective_to_64_bits(const uint128_t& n) noexcept
    {
        return n.data[0];
    }

    // División rápida por potencias de 10 (10^n = 2^n * 5^n)
    std::optional<std::pair<uint128_t, uint128_t>>
    try_divide_by_power_of_10(const uint128_t& divisor) const noexcept
    {
        // Detectar si es potencia de 10: 1, 10, 100, 1000, etc.
        if (divisor == uint128_t(0, 1))
            return std::make_pair(*this, uint128_t(0, 0));
        if (divisor == uint128_t(0, 10))
            return divide_by_10();
        if (divisor == uint128_t(0, 100))
            return divide_by_100();
        if (divisor == uint128_t(0, 1000))
            return divide_by_1000();

        // Otras potencias de 10 pueden implementarse según necesidad
        return std::nullopt;
    }

    // División rápida por potencias de 3
    std::optional<std::pair<uint128_t, uint128_t>>
    try_divide_by_power_of_3(const uint128_t& divisor) const noexcept
    {
        if (divisor == uint128_t(0, 3))
            return divide_by_3();
        if (divisor == uint128_t(0, 9))
            return divide_by_9();
        if (divisor == uint128_t(0, 27))
            return divide_by_27();

        return std::nullopt;
    }

    // División rápida por potencias de 5
    std::optional<std::pair<uint128_t, uint128_t>>
    try_divide_by_power_of_5(const uint128_t& divisor) const noexcept
    {
        if (divisor == uint128_t(0, 5))
            return divide_by_5();
        if (divisor == uint128_t(0, 25))
            return divide_by_25();
        if (divisor == uint128_t(0, 125))
            return divide_by_125();

        return std::nullopt;
    }

    // Implementaciones específicas de división rápida
    std::pair<uint128_t, uint128_t> divide_by_10() const noexcept
    {
        // Implementación correcta: dividir por 10 directamente por ahora
        auto result = divrem(uint128_t(0, 10));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    std::pair<uint128_t, uint128_t> divide_by_100() const noexcept
    {
        // Implementación correcta: dividir por 100 directamente por ahora
        auto result = divrem(uint128_t(0, 100));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    std::pair<uint128_t, uint128_t> divide_by_1000() const noexcept
    {
        // Implementación correcta: dividir por 1000 directamente por ahora
        auto result = divrem(uint128_t(0, 1000));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    std::pair<uint128_t, uint128_t> divide_by_3() const noexcept
    {
        // Usar multiplicación por inverso: 1/3 ≈ 0x55555555555555555555555555555556
        // Esta es una aproximación, necesitaría implementación más precisa
        auto result = divrem(uint128_t(0, 3));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    std::pair<uint128_t, uint128_t> divide_by_9() const noexcept
    {
        auto [q3, r3] = divide_by_3();
        auto [final_q, temp_r] = q3.divide_by_3();
        const uint128_t final_r = temp_r * uint128_t(0, 3) + r3;
        return std::make_pair(final_q, final_r);
    }

    std::pair<uint128_t, uint128_t> divide_by_27() const noexcept
    {
        auto [q9, r9] = divide_by_9();
        auto [final_q, temp_r] = q9.divide_by_3();
        const uint128_t final_r = temp_r * uint128_t(0, 9) + r9;
        return std::make_pair(final_q, final_r);
    }

    std::pair<uint128_t, uint128_t> divide_by_5() const noexcept
    {
        // Usar multiplicación por inverso: 1/5 ≈ 0x33333333333333333333333333333334
        // Implementación simplificada por ahora
        auto result = divrem(uint128_t(0, 5));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    std::pair<uint128_t, uint128_t> divide_by_25() const noexcept
    {
        auto [q5, r5] = divide_by_5();
        auto [final_q, temp_r] = q5.divide_by_5();
        const uint128_t final_r = temp_r * uint128_t(0, 5) + r5;
        return std::make_pair(final_q, final_r);
    }

    std::pair<uint128_t, uint128_t> divide_by_125() const noexcept
    {
        auto [q25, r25] = divide_by_25();
        auto [final_q, temp_r] = q25.divide_by_5();
        const uint128_t final_r = temp_r * uint128_t(0, 25) + r25;
        return std::make_pair(final_q, final_r);
    }

  public:
    uint64_t fullmult_times_uint64(uint64_t multiplier) const noexcept
    {
#ifdef _MSC_VER
        // MSVC: usar __umulh nativo (el más rápido)
        const uint64_t high_high = __umulh(data[1], multiplier);
        const uint64_t low_high = __umulh(data[0], multiplier);
        const uint64_t mid_low = data[1] * multiplier;

        // Sumar la parte media con carry
        const uint64_t sum = low_high + mid_low;
        const uint64_t carry = (sum < low_high) ? 1 : 0;

        return high_high + carry;

#else
        // GCC/Clang: usar nuestra emulación optimizada de __umulh
        const uint64_t high_high = umulh_emulation(data[1], multiplier);
        const uint64_t low_high = umulh_emulation(data[0], multiplier);
        const uint64_t mid_low = data[1] * multiplier;

        // Sumar la parte media con carry (idéntico a MSVC)
        const uint64_t sum = low_high + mid_low;
        const uint64_t carry = (sum < low_high) ? 1 : 0;

        return high_high + carry;
#endif
    }

    /// División avanzada usando Algoritmo D de Knuth con optimizaciones
    /// Implementa división precisa para 128-bit / 128-bit con múltiples rutas optimizadas
    /// @param v_in Divisor (uint128_t)
    /// @return std::optional<std::pair<quotient, remainder>> o nullopt si división por cero
    std::optional<std::pair<uint128_t, uint128_t>>
    knuth_D_divrem(const uint128_t& v_in) const noexcept
    {
        // 0. Casos triviales
        if (v_in == uint128_t(0, 0))
            return std::nullopt; // División por cero
        if (*this < v_in) {
            return std::make_pair(uint128_t(0, 0), *this);
        }

        // === OPTIMIZACIONES RÁPIDAS ===

        // 1. OPTIMIZACIÓN: División por potencias de 2 (usar shift)
        if (is_power_of_2(v_in)) {
            const int shift_amount = count_trailing_zeros(v_in);
            const uint128_t quotient = this->shift_right(shift_amount);
            const uint128_t mask = v_in - uint128_t(0, 1); // 2^n - 1
            const uint128_t remainder = *this & mask;
            return std::make_pair(quotient, remainder);
        }

        // 2. OPTIMIZACIÓN: División por potencias de 10 (descomponer en 2^n * 5^n)
        auto power_of_10_result = try_divide_by_power_of_10(v_in);
        if (power_of_10_result.has_value()) {
            return power_of_10_result;
        }

        // 3. OPTIMIZACIÓN: División por potencias de 3
        auto power_of_3_result = try_divide_by_power_of_3(v_in);
        if (power_of_3_result.has_value()) {
            return power_of_3_result;
        }

        // 4. OPTIMIZACIÓN: División por potencias de 5
        auto power_of_5_result = try_divide_by_power_of_5(v_in);
        if (power_of_5_result.has_value()) {
            return power_of_5_result;
        }

        // 5. OPTIMIZACIÓN: Ambos números caben efectivamente en 64 bits
        if (fits_in_64_bits(*this) && fits_in_64_bits(v_in)) {
            const uint64_t dividend_64 = effective_to_64_bits(*this);
            const uint64_t divisor_64 = effective_to_64_bits(v_in);
            const uint64_t q_64 = dividend_64 / divisor_64;
            const uint64_t r_64 = dividend_64 % divisor_64;
            return std::make_pair(uint128_t(0, q_64), uint128_t(0, r_64));
        }

        // === RUTAS ESTÁNDAR ===

        // Si el divisor cabe en 64 bits (data[1] == 0), usamos una ruta rápida.
        if (v_in.data[1] == 0) {
#if defined(__SIZEOF_INT128__)
            const __uint128_t dividend = (static_cast<__uint128_t>(data[1]) << 64) | data[0];
            const uint64_t divisor = v_in.data[0];
            const __uint128_t q = dividend / divisor;
            const __uint128_t r = dividend % divisor;
            const uint128_t quotient(static_cast<uint64_t>(q >> 64), static_cast<uint64_t>(q));
            const uint128_t remainder(0, static_cast<uint64_t>(r));
            return std::make_pair(quotient, remainder);
#else
            // Fallback para compiladores sin __uint128_t
            return divrem(v_in);
#endif
        }

        // --- ALGORITMO D DE KNUTH (Para divisor > 64 bits) ---

        // D1. Normalización
        // Desplazamos u y v para que el MSB de v sea 1.
        const int s = v_in.leading_zeros() > 0 ? v_in.leading_zeros() - 64 : 0;
        const uint128_t v = v_in.shift_left(s);
        const uint128_t u_shifted = this->shift_left(s);

        // Capturamos el dígito extra de u que se salió por la izquierda al hacer shift.
        uint64_t u_extension = 0;
        if (s > 0) {
            u_extension = data[1] >> (64 - s);
        }

        // D3. Calcular q_hat (Estimación del cociente)
        // Dividimos los dos dígitos más significativos del dividendo
#if defined(__SIZEOF_INT128__)
        const __uint128_t numerator =
            (static_cast<__uint128_t>(u_extension) << 64) | u_shifted.data[1];
        const uint64_t divisor_high = v.data[1];

        // División nativa de 128/64 para estimar
        __uint128_t q_hat_wide = numerator / divisor_high;
        __uint128_t r_hat_wide = numerator % divisor_high;

        uint64_t q_hat = static_cast<uint64_t>(q_hat_wide);

        // Ajuste de q_hat: El bucle interno de Knuth para refinar la estimación
        while (true) {
            if (q_hat_wide >= 0xFFFFFFFFFFFFFFFFULL) {
                q_hat--;
                r_hat_wide += divisor_high;
                if (r_hat_wide > 0xFFFFFFFFFFFFFFFFULL)
                    continue;
            }

            const __uint128_t left = static_cast<__uint128_t>(q_hat) * v.data[0];
            const __uint128_t right = (r_hat_wide << 64) | u_shifted.data[0];

            if (left > right) {
                q_hat--;
                r_hat_wide += divisor_high;
                if (r_hat_wide > 0xFFFFFFFFFFFFFFFFULL)
                    break;
            } else {
                break;
            }
            q_hat_wide = q_hat;
        }
#else
        // Fallback sin __uint128_t - usar algoritmo básico
        return divrem(v_in);
#endif

        // D4. Multiplicar y Restar: u -= q_hat * v
        const uint128_t p_low = v * uint128_t(0, q_hat);
        const uint64_t p_ext = v.fullmult_times_uint64(q_hat);

        // Guardamos estado para detectar "borrow" (underflow) en la resta de 128 bits
        const uint128_t u_shifted_old = u_shifted;
        uint128_t u_result = u_shifted - p_low;

        // Detectamos si la resta pidió prestado
        const uint64_t borrow = (u_result > u_shifted_old) ? 1 : 0;

        // Realizamos la resta final en la parte más significativa
        const int64_t final_balance =
            static_cast<int64_t>(u_extension) - static_cast<int64_t>(p_ext + borrow);

        // D5. Test de Resto (Corrección de signo)
        // Si el balance es negativo, q_hat era 1 unidad muy grande.
        if (final_balance < 0) {
            // D6. Add Back
            q_hat--;
            u_result += v; // Devolvemos una vez el divisor al resto
        }

        const uint128_t quotient(0, q_hat);

        // D8. Desnormalizar Resto
        const uint128_t remainder = u_result.shift_right(s);

        return std::make_pair(quotient, remainder);
    }

    // STRING CONVERSION (basic implementation)
    // Conversión a string decimal (por defecto)
    std::string to_string() const
    {
        return to_string_base(10);
    }

    // Conversión a string con base específica
    std::string to_string_base(int base) const
    {
        if (base < 2 || base > 36) {
            throw std::invalid_argument("Base debe estar entre 2 y 36");
        }

        if (*this == uint128_t(0, 0)) {
            return "0";
        }

        std::string result;
        uint128_t temp = *this;
        const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        while (temp != uint128_t(0, 0)) {
            const auto divmod = temp.divrem(uint128_t(0, static_cast<uint64_t>(base)));
            if (divmod.has_value()) {
                const uint64_t remainder = static_cast<uint64_t>(divmod.value().second);
                result = digits[remainder] + result;
                temp = divmod.value().first;
            } else {
                break;
            }
        }

        return result.empty() ? "0" : result;
    }

    // Conversiones específicas con prefijos
    std::string to_string_hex(bool with_prefix = false) const
    {
        std::string result = to_string_base(16);
        return with_prefix ? "0x" + result : result;
    }

    std::string to_string_bin(bool with_prefix = false) const
    {
        std::string result = to_string_base(2);
        return with_prefix ? "0b" + result : result;
    }

    std::string to_string_oct(bool with_prefix = false) const
    {
        std::string result = to_string_base(8);
        return with_prefix ? "0" + result : result;
    }

    // Función constexpr para convertir C-string a uint128_t
    static constexpr uint128_t from_cstr(const char* str)
    {
        if (!str || !*str) {
            return uint128_t(0, 0);
        }

        // Detectar base y posición inicial
        int base = 10;
        const char* start = str;

        if (str[0] == '0' && str[1]) {
            if (str[1] == 'x' || str[1] == 'X') {
                base = 16;
                start = str + 2;
            } else if (str[1] == 'b' || str[1] == 'B') {
                base = 2;
                start = str + 2;
            } else {
                // Verificar si es octal válido
                bool is_octal = true;
                for (const char* p = str + 1; *p && is_octal; ++p) {
                    if (*p < '0' || *p > '7') {
                        is_octal = false;
                    }
                }
                if (is_octal) {
                    base = 8;
                    start = str + 1;
                }
            }
        }

        return from_cstr_base(start, base);
    }

    // Función constexpr para convertir desde C-string con base específica
    static constexpr uint128_t from_cstr_base(const char* str, int base)
    {
        if (!str || !*str || base < 2 || base > 36) {
            return uint128_t(0, 0);
        }

        uint128_t result(0, 0);
        const uint128_t base_val(0, static_cast<uint64_t>(base));

        for (const char* p = str; *p; ++p) {
            int digit_value = -1;

            if (*p >= '0' && *p <= '9') {
                digit_value = *p - '0';
            } else if (*p >= 'A' && *p <= 'Z') {
                digit_value = *p - 'A' + 10;
            } else if (*p >= 'a' && *p <= 'z') {
                digit_value = *p - 'a' + 10;
            }

            if (digit_value == -1 || digit_value >= base) {
                // Caracteres inválidos terminan el parsing
                break;
            }

            result = result * base_val + uint128_t(0, static_cast<uint64_t>(digit_value));
        }

        return result;
    }

    // Función para convertir a C-string (buffer estático con rotación)
    const char* to_cstr() const
    {
        return to_cstr_base(10);
    }

    // Función para convertir a C-string con base específica
    const char* to_cstr_base(int base) const
    {
        // Buffer estático rotativo para permitir múltiples llamadas
        static thread_local char buffers[4][64]; // 4 buffers de 64 chars cada uno
        static thread_local int current_buffer = 0;

        current_buffer = (current_buffer + 1) % 4;
        char* buffer = buffers[current_buffer];

        if (base < 2 || base > 36) {
            buffer[0] = '0';
            buffer[1] = '\0';
            return buffer;
        }

        if (*this == uint128_t(0, 0)) {
            buffer[0] = '0';
            buffer[1] = '\0';
            return buffer;
        }

        const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char temp_buffer[64];
        int pos = 63;
        temp_buffer[63] = '\0';

        uint128_t temp = *this;
        while (temp != uint128_t(0, 0)) {
            const auto divmod = temp.divrem(uint128_t(0, static_cast<uint64_t>(base)));
            if (divmod.has_value()) {
                const uint64_t remainder = static_cast<uint64_t>(divmod.value().second);
                temp_buffer[--pos] = digits[remainder];
                temp = divmod.value().first;
            } else {
                break;
            }
        }

        // Copiar resultado al buffer rotativo
        const char* result_start = &temp_buffer[pos];
        int len = 0;
        while (result_start[len] != '\0') {
            buffer[len] = result_start[len];
            ++len;
        }
        buffer[len] = '\0';

        return buffer;
    }

    // Funciones C-string específicas por formato
    const char* to_cstr_hex() const
    {
        return to_cstr_base(16);
    }
    const char* to_cstr_bin() const
    {
        return to_cstr_base(2);
    }
    const char* to_cstr_oct() const
    {
        return to_cstr_base(8);
    }

    // Función estática para convertir string a uint128_t
    // Función que detecta automáticamente la base del string
    static uint128_t from_string(const std::string& str)
    {
        if (str.empty()) {
            return uint128_t(0, 0);
        }

        std::string cleaned = str;
        int base = 10;
        size_t start_pos = 0;

        // Detectar prefijo y base
        if (cleaned.size() > 2) {
            if (cleaned.substr(0, 2) == "0x" || cleaned.substr(0, 2) == "0X") {
                base = 16;
                start_pos = 2;
            } else if (cleaned.substr(0, 2) == "0b" || cleaned.substr(0, 2) == "0B") {
                base = 2;
                start_pos = 2;
            }
        }
        if (cleaned.size() > 1 && cleaned[0] == '0' && base == 10) {
            // Detectar octal (empieza con 0 pero no es 0x o 0b)
            bool is_octal = true;
            for (size_t i = 1; i < cleaned.size(); ++i) {
                if (cleaned[i] < '0' || cleaned[i] > '7') {
                    is_octal = false;
                    break;
                }
            }
            if (is_octal && cleaned.size() > 1) {
                base = 8;
                start_pos = 1;
            }
        }

        return from_string_base(cleaned.substr(start_pos), base);
    }

    // Función para convertir desde string con base específica
    static uint128_t from_string_base(const std::string& str, int base)
    {
        if (str.empty() || base < 2 || base > 36) {
            return uint128_t(0, 0);
        }

        uint128_t result(0, 0);
        const uint128_t base_val(0, static_cast<uint64_t>(base));

        for (char c : str) {
            int digit_value = -1;

            if (c >= '0' && c <= '9') {
                digit_value = c - '0';
            } else if (c >= 'A' && c <= 'Z') {
                digit_value = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'z') {
                digit_value = c - 'a' + 10;
            }

            if (digit_value == -1 || digit_value >= base) {
                // Caracteres inválidos se ignoran
                continue;
            }

            result *= base_val;
            result += uint128_t(0, static_cast<uint64_t>(digit_value));
        }

        return result;
    }
};

// ========================= OPERADORES FRIEND FUERA DE LA CLASE =========================
// Para evitar ambigüedad con operadores existentes, se definen fuera de la clase

// Operadores aritméticos simétricos para int
inline constexpr uint128_t operator+(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) + rhs;
}
inline constexpr uint128_t operator-(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) - rhs;
}
inline constexpr uint128_t operator*(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) * rhs;
}
inline constexpr uint128_t operator/(int lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) / rhs;
}
inline constexpr uint128_t operator%(int lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) % rhs;
}

// Operadores aritméticos simétricos para uint32_t
inline constexpr uint128_t operator+(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) + rhs;
}
inline constexpr uint128_t operator-(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) - rhs;
}
inline constexpr uint128_t operator*(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) * rhs;
}
inline constexpr uint128_t operator/(std::uint32_t lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) / rhs;
}
inline constexpr uint128_t operator%(std::uint32_t lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) % rhs;
}

// Operadores aritméticos simétricos para uint64_t
inline constexpr uint128_t operator+(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) + rhs;
}
inline constexpr uint128_t operator-(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) - rhs;
}
inline constexpr uint128_t operator*(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) * rhs;
}
inline constexpr uint128_t operator/(std::uint64_t lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) / rhs;
}
inline constexpr uint128_t operator%(std::uint64_t lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) % rhs;
}

// Operadores de comparación simétricos para int
inline constexpr bool operator==(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) == rhs;
}
inline constexpr bool operator!=(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) != rhs;
}
inline constexpr bool operator<(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) < rhs;
}
inline constexpr bool operator<=(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) <= rhs;
}
inline constexpr bool operator>(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) > rhs;
}
inline constexpr bool operator>=(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) >= rhs;
}

// Operadores de comparación simétricos para uint32_t
inline constexpr bool operator==(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) == rhs;
}
inline constexpr bool operator!=(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) != rhs;
}
inline constexpr bool operator<(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) < rhs;
}
inline constexpr bool operator<=(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) <= rhs;
}
inline constexpr bool operator>(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) > rhs;
}
inline constexpr bool operator>=(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) >= rhs;
}

// Operadores de comparación simétricos para uint64_t
inline constexpr bool operator==(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) == rhs;
}
inline constexpr bool operator!=(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) != rhs;
}
inline constexpr bool operator<(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) < rhs;
}
inline constexpr bool operator<=(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) <= rhs;
}
inline constexpr bool operator>(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) > rhs;
}
inline constexpr bool operator>=(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) >= rhs;
}

// Operadores bitwise simétricos para int
inline constexpr uint128_t operator&(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) & rhs;
}
inline constexpr uint128_t operator|(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) | rhs;
}
inline constexpr uint128_t operator^(int lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) ^ rhs;
}

// Operadores bitwise simétricos para uint32_t
inline constexpr uint128_t operator&(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) & rhs;
}
inline constexpr uint128_t operator|(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) | rhs;
}
inline constexpr uint128_t operator^(std::uint32_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) ^ rhs;
}

// Operadores bitwise simétricos para uint64_t
inline constexpr uint128_t operator&(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) & rhs;
}
inline constexpr uint128_t operator|(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) | rhs;
}
inline constexpr uint128_t operator^(std::uint64_t lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) ^ rhs;
}

// Constructor desde string ELIMINADO para mantener trivialidad
// Usar make_uint128(str) en lugar de uint128_t(str)

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

// ========================= LITERALES DEFINIDOS POR EL USUARIO =========================
// Namespace para los literales UDL
namespace uint128_literals
{

// Literal para enteros pequeños (hasta uint64_t)
constexpr uint128_t operator""_u128(unsigned long long value) noexcept
{
    return uint128_t(0, value);
}

// Literal para strings (soporta múltiples formatos)
uint128_t operator""_u128(const char* str, std::size_t len)
{
    return uint128_t::from_string(std::string(str, len));
}

// Literales específicos por formato
uint128_t operator""_u128_hex(const char* str, std::size_t len)
{
    return uint128_t::from_string_base(std::string(str, len), 16);
}

uint128_t operator""_u128_bin(const char* str, std::size_t len)
{
    return uint128_t::from_string_base(std::string(str, len), 2);
}

uint128_t operator""_u128_oct(const char* str, std::size_t len)
{
    return uint128_t::from_string_base(std::string(str, len), 8);
}

} // namespace uint128_literals

// Conveniencia: importar literales al namespace global cuando se usa
// using namespace uint128_literals;

#endif // UINT128_T_HPP

//     return true;
// }