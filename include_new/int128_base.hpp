#ifndef INT128_BASE_HPP
#define INT128_BASE_HPP

#include "type_traits.hpp"
// NOTA: uint128_traits_specializations.hpp se incluye al FINAL del archivo,
// después de las definiciones de uint128_t e int128_t (type aliases).
#include <climits> // Para CHAR_BIT
#include <compare>
#include <cstdint>
#include <type_traits>
#include <utility> // Para std::move

// Incluir intrínsecos
#include "intrinsics/arithmetic_operations.hpp"
#include "intrinsics/bit_operations.hpp"

namespace nstd
{

/**
 * @brief Enum para distinguir entre signed y unsigned
 */
enum class signedness : bool { unsigned_type = false, signed_type = true };

/**
 * @brief Clase template unificada para enteros de 128 bits
 * @tparam S signedness::unsigned_type para uint128_t, signedness::signed_type para int128_t
 */
template <signedness S> class int128_base_t
{
    // ============================================================================
    // CONSTANTES Y TIPOS
    // ============================================================================

    static constexpr bool is_signed = (S == signedness::signed_type);
    static constexpr int BITS = 128;
    static constexpr int BYTES = BITS / CHAR_BIT;

    uint64_t data[2]; // data[0] = low, data[1] = high (little-endian)

  public:
    // ============================================================================
    // CONSTRUCTORES
    // ============================================================================

    // Constructor por defecto
    consteval int128_base_t() noexcept : data{0ull, 0ull} {}

    // Constructor desde tipos integrales builtin
    template <integral_builtin T>
    explicit constexpr int128_base_t(T value) noexcept : data{static_cast<uint64_t>(value), 0ull}
    {
        if constexpr (is_signed && std::is_signed_v<T>) {
            // Sign extension para signed types
            if (value < 0) {
                data[1] = ~0ull;
            }
        }
    }

    // Constructor desde high y low
    template <typename T1, typename T2>
    explicit constexpr int128_base_t(T1 high, T2 low) noexcept
        requires(integral_builtin<T1> && integral_builtin<T2>)
        : data{static_cast<uint64_t>(low), static_cast<uint64_t>(high)}
    {
    }

    // Copy/move constructors (trivial)
    constexpr int128_base_t(const int128_base_t&) = default;
    constexpr int128_base_t(int128_base_t&&) = default;
    constexpr int128_base_t& operator=(const int128_base_t&) = default;
    constexpr int128_base_t& operator=(int128_base_t&&) = default;

    // Constructor de conversión desde signedness diferente
    template <signedness S2>
    explicit constexpr int128_base_t(const int128_base_t<S2>& other) noexcept
        : data{other.low(), other.high()}
    {
        // Conversión directa de bits, sin sign extension
        // uint128_t(int128_t(-1)) = 0xFFFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF (interpreta como unsigned)
        // int128_t(uint128_t(MAX)) = -1 (interpreta como signed)
    }
    template <signedness S2>
    explicit constexpr int128_base_t(int128_base_t<S2>&& other) noexcept
        : data{std::move(other.low()), std::move(other.high())}
    {
        // Movimiento directo de bits, sin sign extension
    }

    // ============================================================================
    //SOBRECARGA DEL OPERADOR DE ASIGNACIÓN
    // ============================================================================ 

    template<signedness S2>
    constexpr int128_base_t& operator=(const int128_base_t<S2>& other) noexcept
    {
        if (this != &other) {
            data[0] = other.low();
            data[1] = other.high();
        } // IDÉNTICO A UN MEMCPY
        return *this;
    }
    template<signedness S2>
    constexpr int128_base_t& operator=(int128_base_t<S2>&& other) noexcept
    {
        if (this != &other) {
            data[0] = std::move(other.low());
            data[1] = std::move(other.high());
        } // CEDE LA PROPIEDAD
        return *this;
    }
    
    template<integral_builtin T>
    constexpr int128_base_t& operator=(T other) noexcept
    {
        data[0] = static_cast<uint64_t>(other);
        data[1] = 0ull;
        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                data[1] = ~0ull; // Extensión de signo correcta para negativos
            }
        }
        return *this;
    }

    // ============================================================================
    // ACCESSORS
    // ============================================================================

    constexpr uint64_t high() const noexcept
    {
        return data[1];
    }
    constexpr uint64_t low() const noexcept
    {
        return data[0];
    }

    template <typename T> constexpr void set_high(T value) noexcept
    {
        data[1] = static_cast<uint64_t>(value);
    }

    template <typename T> constexpr void set_low(T value) noexcept
    {
        data[0] = static_cast<uint64_t>(value);
    }

    // ============================================================================
    // OPERADORES ARITMÉTICOS
    // ============================================================================

    // Operador negación bit a bit
    constexpr int128_base_t operator~() const noexcept
    {
        return int128_base_t(~data[0], ~data[1]);
    }

    // Operador negación (según el comportamiento estándar
    // es idéntico para signed y unsigned, comportamiento modular)
    constexpr int128_base_t operator-() const noexcept
    {
        return ~(*this) + int128_base_t(1);
    }

    // Operador suma (idéntico para signed/unsigned)
    constexpr int128_base_t& operator+=(const int128_base_t& other) noexcept
    {
        uint64_t temp_low = 0;
        const unsigned char carry = intrinsics::add_u64(data[0], other.data[0], &temp_low);
        data[0] = temp_low;
        uint64_t temp_high = 0;
        intrinsics::addcarry_u64(carry, data[1], other.data[1], &temp_high);
        data[1] = temp_high;
        return *this;
    }

    constexpr int128_base_t operator+(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result += other;
        return result;
    }

    // Operador resta (idéntico para signed/unsigned)
    constexpr int128_base_t& operator-=(const int128_base_t& other) noexcept
    {
        uint64_t temp_low = 0;
        const unsigned char borrow = intrinsics::sub_u64(data[0], other.data[0], &temp_low);
        data[0] = temp_low;
        uint64_t temp_high = 0;
        intrinsics::subborrow_u64(borrow, data[1], other.data[1], &temp_high);
        data[1] = temp_high;
        return *this;
    }

    constexpr int128_base_t operator-(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result -= other;
        return result;
    }

    // Sobrecargas para tipos builtin
    template <integral_builtin T> constexpr int128_base_t& operator+=(T other) noexcept
    {
        return *this += int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator+(T other) const noexcept
    {
        int128_base_t result(*this);
        result += other;
        return result;
    }

    template <integral_builtin T> constexpr int128_base_t& operator-=(T other) noexcept
    {
        return *this -= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator-(T other) const noexcept
    {
        int128_base_t result(*this);
        result -= other;
        return result;
    }

    // Sobrecargas para signedness diferente
    template <signedness S2>
    constexpr int128_base_t& operator+=(const int128_base_t<S2>& other) noexcept
    {
        uint64_t temp_low = 0;
        const unsigned char carry = intrinsics::add_u64(data[0], other.low(), &temp_low);
        data[0] = temp_low;
        uint64_t temp_high = 0;
        intrinsics::addcarry_u64(carry, data[1], other.high(), &temp_high);
        data[1] = temp_high;
        return *this;
    }

    template <signedness S2>
    constexpr int128_base_t operator+(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result += other;
        return result;
    }

    template <signedness S2>
    constexpr int128_base_t& operator-=(const int128_base_t<S2>& other) noexcept
    {
        uint64_t temp_low = 0;
        const unsigned char borrow = intrinsics::sub_u64(data[0], other.low(), &temp_low);
        data[0] = temp_low;
        uint64_t temp_high = 0;
        intrinsics::subborrow_u64(borrow, data[1], other.high(), &temp_high);
        data[1] = temp_high;
        return *this;
    }

    template <signedness S2>
    constexpr int128_base_t operator-(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result -= other;
        return result;
    }

    // Multiplicación entre dos int128_base_t (mismo signedness)
    constexpr int128_base_t& operator*=(const int128_base_t& other) noexcept
    {
        // Usamos el algoritmo estándar de multiplicación 128x128:
        // (a1*2^64 + a0) * (b1*2^64 + b0) = a0*b0 + (a0*b1 + a1*b0)*2^64 + a1*b1*2^128
        // Solo necesitamos los 128 bits bajos del resultado, entonces a1*b1*2^128 se descarta.

        const uint64_t a0 = data[0];
        const uint64_t a1 = data[1];
        const uint64_t b0 = other.data[0];
        const uint64_t b1 = other.data[1];

        // a0 * b0 (128 bits)
        uint64_t high_part;
        const uint64_t low_part = intrinsics::umul128(a0, b0, &high_part);

        // a0 * b1 (solo necesitamos parte baja, va sumada a high_part)
        const uint64_t cross1 = a0 * b1;

        // a1 * b0 (solo necesitamos parte baja, va sumada a high_part)
        const uint64_t cross2 = a1 * b0;

        data[0] = low_part;
        data[1] = high_part + cross1 + cross2;
        return *this;
    }

    constexpr int128_base_t operator*(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result *= other;
        return result;
    }

    // Operador multiplicación (FASE 0.5 optimizado)
    template <integral_builtin T> constexpr int128_base_t& operator*=(T other) noexcept
    {
        const uint64_t b = static_cast<uint64_t>(other);

        // Fast path: valor cabe en 64 bits
        if (data[1] == 0) {
            const uint64_t low_part = intrinsics::umul128(data[0], b, &data[1]);
            data[0] = low_part;

            if constexpr (is_signed && std::is_signed_v<T>) {
                if (other < 0) {
                    data[1] -= data[0];
                }
            }
            return *this;
        }

        // General case: multiplicación 128×64
        uint64_t high_part;
        const uint64_t low_part = intrinsics::umul128(data[0], b, &high_part);
        const uint64_t cross_product = data[1] * b;

        if constexpr (is_signed && std::is_signed_v<T>) {
            if (other < 0) {
                high_part -= data[0];
            }
        }

        data[0] = low_part;
        data[1] = high_part + cross_product;
        return *this;
    }

    template <integral_builtin T> constexpr int128_base_t operator*(T other) const noexcept
    {
        int128_base_t result(*this);
        result *= other;
        return result;
    }

    // Sobrecarga para signedness diferente
    template <signedness S2>
    constexpr int128_base_t& operator*=(const int128_base_t<S2>& other) noexcept
    {
        // Convertimos a nuestro tipo y usamos el operador existente
        return *this *= int128_base_t(other);
    }

    template <signedness S2>
    constexpr int128_base_t operator*(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result *= other;
        return result;
    }

    // ============================================================================
    // OPERADORES BITWISE
    // ============================================================================

    constexpr int128_base_t& operator&=(const int128_base_t& other) noexcept
    {
        data[0] &= other.data[0];
        data[1] &= other.data[1];
        return *this;
    }

    constexpr int128_base_t operator&(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result &= other;
        return result;
    }

    constexpr int128_base_t& operator|=(const int128_base_t& other) noexcept
    {
        data[0] |= other.data[0];
        data[1] |= other.data[1];
        return *this;
    }

    constexpr int128_base_t operator|(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result |= other;
        return result;
    }

    constexpr int128_base_t& operator^=(const int128_base_t& other) noexcept
    {
        data[0] ^= other.data[0];
        data[1] ^= other.data[1];
        return *this;
    }

    constexpr int128_base_t operator^(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result ^= other;
        return result;
    }

    // Left shift (idéntico para signed/unsigned)
    constexpr int128_base_t& operator<<=(int shift) noexcept
    {
        if (shift <= 0) {
            return *this;
        }
        if (shift >= 128) {
            data[0] = 0;
            data[1] = 0;
            return *this;
        }

        if (shift >= 64) {
            const uint64_t new_high = data[0] << (shift - 64);
            data[0] = 0;
            data[1] = new_high;
        } else {
            const uint64_t new_high = (data[1] << shift) | (data[0] >> (64 - shift));
            const uint64_t new_low = data[0] << shift;
            data[0] = new_low;
            data[1] = new_high;
        }

        return *this;
    }

    constexpr int128_base_t operator<<(int shift) const noexcept
    {
        int128_base_t result(*this);
        result <<= shift;
        return result;
    }

    // Right shift: arithmetic (signed) vs logical (unsigned)
    constexpr int128_base_t& operator>>=(int shift) noexcept
    {
        if (shift <= 0) {
            return *this;
        }
        if (shift >= 128) {
            if constexpr (is_signed) {
                // Arithmetic shift: propagar signo
                const bool is_negative = (static_cast<int64_t>(data[1]) < 0);
                data[0] = is_negative ? ~0ull : 0ull;
                data[1] = is_negative ? ~0ull : 0ull;
            } else {
                // Logical shift: rellenar con ceros
                data[0] = 0;
                data[1] = 0;
            }
            return *this;
        }

        if (shift >= 64) {
            if constexpr (is_signed) {
                // Arithmetic shift
                const uint64_t new_low =
                    static_cast<uint64_t>(static_cast<int64_t>(data[1]) >> (shift - 64));
                const uint64_t new_high =
                    static_cast<uint64_t>(static_cast<int64_t>(data[1]) >> 63);
                data[0] = new_low;
                data[1] = new_high;
            } else {
                // Logical shift
                const uint64_t new_low = data[1] >> (shift - 64);
                data[0] = new_low;
                data[1] = 0;
            }
        } else {
            const uint64_t new_low = (data[0] >> shift) | (data[1] << (64 - shift));
            if constexpr (is_signed) {
                const uint64_t new_high =
                    static_cast<uint64_t>(static_cast<int64_t>(data[1]) >> shift);
                data[0] = new_low;
                data[1] = new_high;
            } else {
                const uint64_t new_high = data[1] >> shift;
                data[0] = new_low;
                data[1] = new_high;
            }
        }

        return *this;
    }

    constexpr int128_base_t operator>>(int shift) const noexcept
    {
        int128_base_t result(*this);
        result >>= shift;
        return result;
    }

    // ============================================================================
    // COMPARACIONES (idénticas para signed/unsigned entre mismo tipo)
    // ============================================================================

    constexpr bool operator==(const int128_base_t& other) const noexcept
    {
        return (data[0] == other.data[0]) && (data[1] == other.data[1]);
    }

    constexpr bool operator!=(const int128_base_t& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr bool operator<(const int128_base_t& other) const noexcept
    {
        if constexpr (is_signed) {
            // Comparación signed
            const int64_t this_high = static_cast<int64_t>(data[1]);
            const int64_t other_high = static_cast<int64_t>(other.data[1]);
            if (this_high != other_high) {
                return this_high < other_high;
            }
            return data[0] < other.data[0];
        } else {
            // Comparación unsigned
            if (data[1] != other.data[1]) {
                return data[1] < other.data[1];
            }
            return data[0] < other.data[0];
        }
    }

    constexpr std::strong_ordering operator<=>(const int128_base_t& other) const noexcept
    {
        if (*this < other)
            return std::strong_ordering::less;
        if (*this == other)
            return std::strong_ordering::equal;
        return std::strong_ordering::greater;
    }

    // ============================================================================
    // INCREMENT/DECREMENT
    // ============================================================================

    // Pre-incremento
    constexpr int128_base_t& operator++() noexcept
    {
        *this += int128_base_t(1);
        return *this;
    }

    // Post-incremento
    constexpr int128_base_t operator++(int) noexcept
    {
        int128_base_t temp(*this);
        ++(*this);
        return temp;
    }

    // Pre-decremento
    constexpr int128_base_t& operator--() noexcept
    {
        *this -= int128_base_t(1);
        return *this;
    }

    // Post-decremento
    constexpr int128_base_t operator--(int) noexcept
    {
        int128_base_t temp(*this);
        --(*this);
        return temp;
    }

    // ============================================================================
    // CONVERSIONES
    // ============================================================================

    explicit constexpr operator bool() const noexcept
    {
        return (data[0] != 0) || (data[1] != 0);
    }

    template <arithmetic_builtin T> explicit constexpr operator T() const noexcept
    {
        if constexpr (floating_point_builtin<T>) {
            return static_cast<T>(data[1]) * 18446744073709551616.0L + static_cast<T>(data[0]);
        } else {
            return static_cast<T>(data[0]);
        }
    }

    // ============================================================================
    // HELPERS
    // ============================================================================

    constexpr int leading_zeros() const noexcept
    {
        if (data[1] != 0) {
            return intrinsics::clz64(data[1]);
        } else if (data[0] != 0) {
            return 64 + intrinsics::clz64(data[0]);
        } else {
            return 128;
        }
    }

    constexpr bool is_negative() const noexcept
        requires(is_signed)
    {
        return static_cast<int64_t>(data[1]) < 0;
    }

    // Valor absoluto (solo para signed)
    constexpr int128_base_t abs() const noexcept
    {
        if constexpr (!is_signed) {
            return *this;
        } else {
            return is_negative() ? -(*this) : *this;
        }
    }

    // ============================================================================
    // CONSTANTES ESTÁTICAS
    // ============================================================================

    static constexpr int128_base_t min() noexcept
    {
        if constexpr (is_signed) {
            // INT128_MIN = -2^127 = 0x8000000000000000'0000000000000000
            return int128_base_t(0x8000000000000000ull, 0ull);
        } else {
            // UINT128_MIN = 0
            return int128_base_t(0ull, 0ull);
        }
    }

    static constexpr int128_base_t max() noexcept
    {
        if constexpr (is_signed) {
            // INT128_MAX = 2^127-1 = 0x7FFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF
            return int128_base_t(0x7FFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
        } else {
            // UINT128_MAX = 2^128-1 = 0xFFFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF
            return int128_base_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
        }
    }
};

// ============================================================================
// TYPE ALIASES (reemplazan las clases originales)
// ============================================================================

using uint128_t = int128_base_t<signedness::unsigned_type>;
using int128_t = int128_base_t<signedness::signed_type>;

} // namespace nstd

// ============================================================================
// TYPE TRAITS SPECIALIZATIONS
// ============================================================================
// NOTA: Este archivo debe incluirse DESPUÉS de las definiciones de los type aliases
// para que std::type_traits vea correctamente uint128_t e int128_t.
#include "uint128_traits_specializations.hpp"

#endif // INT128_BASE_HPP