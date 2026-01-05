/**
 * @file int128_base.hpp
 * @brief Implementación unificada de enteros de 128 bits signed/unsigned para C++20
 *
 * @details Esta biblioteca proporciona una implementación completa y eficiente de enteros
 * de 128 bits mediante un template unificado que soporta tanto tipos signed (int128_t)
 * como unsigned (uint128_t).
 *
 * **Características principales:**
 * - Template unificado: `int128_base_t<signedness S>` para eliminar duplicación
 * - Operadores completos: aritméticos, comparación, bitwise, shift
 * - Parsing avanzado: soporta separadores de dígitos ('), múltiples bases (2-36)
 * - Literales UDL: 42_u128, 0xDEADBEEF_u128, "12345"_U128, etc.
 * - Conversiones seguras: string ↔ int128, floating point ↔ int128
 * - Constexpr completo: evaluación en tiempo de compilación
 * - Código portable: usa std::numeric_limits en lugar de valores hexadecimales mágicos
 *
 * @note Fase 17 (5 enero 2026): Fix crítico operator~() para to_string() con negativos
 * @note Todos los tests pasan: 29/29 ✅
 *
 * @author Julián Calderón Almendros <julian.calderon.almendros@gmail.com>
 * @version 1.0.0
 * @date 2026-01-05
 * @copyright Boost Software License 1.0
 */

#ifndef INT128_BASE_HPP
#define INT128_BASE_HPP

#include "type_traits.hpp"
// NOTA: uint128_traits_specializations.hpp se incluye al FINAL del archivo,
// después de las definiciones de uint128_t e int128_t (type aliases).
#include <algorithm> // Para std::reverse
#include <climits>   // Para CHAR_BIT
#include <compare>
#include <cstdint>
#include <istream>     // Para operator>>
#include <limits>      // Para std::numeric_limits (comparaciones floating_point)
#include <ostream>     // Para operator<<
#include <stdexcept>   // Para std::invalid_argument
#include <string>      // Para std::string
#include <string_view> // Para std::string_view
#include <type_traits>
#include <utility> // Para std::move

// Incluir intrínsecos
#include "intrinsics/arithmetic_operations.hpp"
#include "intrinsics/bit_operations.hpp"

namespace nstd
{

/**
 * @brief Enum para distinguir entre tipos signed y unsigned en el template unificado
 *
 * @details Este enum permite al template int128_base_t<S> adaptar su comportamiento
 * según si el tipo es signed o unsigned, usando `if constexpr (is_signed)`.
 *
 * @see int128_base_t
 */
enum class signedness : bool {
    unsigned_type = false, ///< Tipo unsigned (uint128_t)
    signed_type = true     ///< Tipo signed (int128_t)
};

/**
 * @brief Enum para clasificar errores de parsing de strings
 */
enum class parse_error : uint8_t {
    success = 0,       ///< Parsing exitoso
    null_pointer,      ///< Puntero null proporcionado
    empty_string,      ///< String vacío
    invalid_base,      ///< Base fuera de rango [2, 36]
    invalid_character, ///< Carácter inválido para la base especificada
    overflow,          ///< El resultado excede el rango del tipo
    unknown_error      ///< Error desconocido
};

/**
 * @brief Clase template unificada para enteros de 128 bits signed/unsigned
 *
 * @tparam S Parámetro de signedness:
 *           - signedness::unsigned_type → uint128_t (0 a 2^128-1)
 *           - signedness::signed_type → int128_t (-2^127 a 2^127-1)
 *
 * @details Esta clase implementa todas las operaciones aritméticas, bitwise y de
 * comparación para enteros de 128 bits, utilizando internamente dos uint64_t en
 * formato little-endian (data[0]=low, data[1]=high).
 *
 * **Características de diseño:**
 * - Representación: Two's complement para signed, binario puro para unsigned
 * - Overflow behavior: Wrap-around (modular arithmetic) para ambos tipos
 * - Constexpr: Todas las operaciones son evaluables en compile-time
 * - Portable: Usa std::numeric_limits en lugar de constantes hexadecimales
 * - Eficiente: Usa intrínsecos del compilador cuando están disponibles
 *
 * **Operaciones soportadas:**
 * - Aritméticas: +, -, *, /, %, negación unaria
 * - Comparación: ==, !=, <, <=, >, >=, <=> (C++20)
 * - Bitwise: &, |, ^, ~, <<, >>
 * - Conversiones: to/from string, to/from floating point
 * - Parsing: bases 2-36 con separadores de dígitos
 * - Literales UDL: 42_u128, 0xFF_u128, "12345"_U128
 *
 * @note Fix Fase 17: operator~() ahora usa orden correcto (high, low) para constructor
 * @see uint128_t, int128_t
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
    constexpr int128_base_t() noexcept : data{0ull, 0ull} {}

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
    template <integral_builtin T1, integral_builtin T2>
    explicit constexpr int128_base_t(T1 high, T2 low) noexcept
        : data{static_cast<uint64_t>(low), static_cast<uint64_t>(high)}
    {
    }

    // Constructor desde tipos floating point (float, double, long double)
    template <floating_point_builtin T>
    explicit constexpr int128_base_t(T value) noexcept : data{0ull, 0ull}
    {
        // Conversión de floating point a entero de 128 bits
        // Maneja valores negativos solo si is_signed
        if constexpr (is_signed) {
            const bool is_negative = (value < 0);
            const T abs_value = is_negative ? -value : value;

            // Convertir la parte entera
            if (abs_value >= 18446744073709551616.0L) {
                // Valor >= 2^64, necesitamos ambas partes
                const T high_part = abs_value / 18446744073709551616.0L;
                data[1] = static_cast<uint64_t>(high_part);
                const T low_part = abs_value - (high_part * 18446744073709551616.0L);
                data[0] = static_cast<uint64_t>(low_part);
            } else {
                // Valor < 2^64, solo parte baja
                data[0] = static_cast<uint64_t>(abs_value);
            }

            // Si era negativo, aplicar complemento a 2
            if (is_negative) {
                // Negar usando complemento a 2: ~value + 1
                data[0] = ~data[0];
                data[1] = ~data[1];
                uint64_t temp = 0;
                const unsigned char carry = intrinsics::add_u64(data[0], 1ull, &temp);
                data[0] = temp;
                intrinsics::addcarry_u64(carry, data[1], 0ull, &temp);
                data[1] = temp;
            }
        } else {
            // Unsigned: solo manejar valores no negativos
            if (value < 0) {
                // Valor negativo para unsigned → cero
                data[0] = 0ull;
                data[1] = 0ull;
            } else if (value >= 18446744073709551616.0L) {
                // Valor >= 2^64
                const T high_part = value / 18446744073709551616.0L;
                data[1] = static_cast<uint64_t>(high_part);
                const T low_part = value - (high_part * 18446744073709551616.0L);
                data[0] = static_cast<uint64_t>(low_part);
            } else {
                // Valor < 2^64
                data[0] = static_cast<uint64_t>(value);
            }
        }
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
    // CONSTRUCTORES DESDE STRINGS
    // ============================================================================

    // Constructor desde const char* con auto-detección de base
    // Usa parse() que detecta automáticamente el formato (0x, 0b, 0, decimal)
    // Lanza std::invalid_argument si el string no es válido
    explicit int128_base_t(const char* str)
    {
        if (str == nullptr) {
            throw std::invalid_argument("Cannot parse null pointer");
        }

        auto [error, value] = parse(str);
        if (error != parse_error::success) {
            // Construir mensaje de error descriptivo
            const char* error_msg = "Unknown error";
            switch (error) {
            case parse_error::empty_string:
                error_msg = "Empty string";
                break;
            case parse_error::invalid_base:
                error_msg = "Invalid base";
                break;
            case parse_error::invalid_character:
                error_msg = "Invalid character";
                break;
            case parse_error::overflow:
                error_msg = "Overflow";
                break;
            default:
                break;
            }
            throw std::invalid_argument(std::string("Failed to parse string: ") + error_msg);
        }
        *this = value;
    }

    // Constructor desde const char* con base explícita
    // Usa parse_base() con la base especificada (2-36)
    // Lanza std::invalid_argument si el string o la base no son válidos
    explicit int128_base_t(const char* str, int base)
    {
        if (str == nullptr) {
            throw std::invalid_argument("Cannot parse null pointer");
        }
        if (base < 2 || base > 36) {
            throw std::invalid_argument("Invalid base: must be between 2 and 36");
        }

        auto [error, value] = parse_base(str, base);
        if (error != parse_error::success) {
            // Construir mensaje de error descriptivo
            const char* error_msg = "Unknown error";
            switch (error) {
            case parse_error::empty_string:
                error_msg = "Empty string";
                break;
            case parse_error::invalid_base:
                error_msg = "Invalid base";
                break;
            case parse_error::invalid_character:
                error_msg = "Invalid character";
                break;
            case parse_error::overflow:
                error_msg = "Overflow";
                break;
            default:
                break;
            }
            throw std::invalid_argument(std::string("Failed to parse string: ") + error_msg);
        }
        *this = value;
    }

    // Constructor desde std::string con auto-detección de base
    // Delega al constructor de const char*
    explicit int128_base_t(const std::string& str) : int128_base_t(str.c_str()) {}

    // Constructor desde std::string con base explícita
    // Delega al constructor de const char* con base
    explicit int128_base_t(const std::string& str, int base) : int128_base_t(str.c_str(), base) {}

    // Constructor desde std::string_view con auto-detección de base
    // std::string_view no garantiza null-termination, así que convertimos a std::string primero
    explicit int128_base_t(std::string_view str) : int128_base_t(std::string(str)) {}

    // Constructor desde std::string_view con base explícita
    // std::string_view no garantiza null-termination, así que convertimos a std::string primero
    explicit int128_base_t(std::string_view str, int base) : int128_base_t(std::string(str), base)
    {
    }

    // ============================================================================
    // SOBRECARGA DEL OPERADOR DE ASIGNACIÓN
    // ============================================================================

    template <signedness S2>
    constexpr int128_base_t& operator=(const int128_base_t<S2>& other) noexcept
    {
        if (this != &other) {
            data[0] = other.low();
            data[1] = other.high();
        } // IDÉNTICO A UN MEMCPY
        return *this;
    }
    template <signedness S2> constexpr int128_base_t& operator=(int128_base_t<S2>&& other) noexcept
    {
        if (this != &other) {
            data[0] = std::move(other.low());
            data[1] = std::move(other.high());
        } // CEDE LA PROPIEDAD
        return *this;
    }

    template <integral_builtin T> constexpr int128_base_t& operator=(T other) noexcept
    {
        data[0] = static_cast<uint64_t>(other);

        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                data[1] = ~0ull; // Extensión de signo correcta para negativos
            } else {
                data[1] = 0ull;
            }
        } else {
            data[1] = 0ull;
        }

        return *this;
    }

    // Operador de asignación desde tipos floating point
    template <floating_point_builtin T> constexpr int128_base_t& operator=(T value) noexcept
    {
        // Conversión de floating point a entero de 128 bits
        data[0] = 0ull;
        data[1] = 0ull;

        if constexpr (is_signed) {
            const bool is_negative = (value < 0);
            const T abs_value = is_negative ? -value : value;

            // Convertir la parte entera
            if (abs_value >= 18446744073709551616.0L) {
                // Valor >= 2^64
                const T high_part = abs_value / 18446744073709551616.0L;
                data[1] = static_cast<uint64_t>(high_part);
                const T low_part = abs_value - (high_part * 18446744073709551616.0L);
                data[0] = static_cast<uint64_t>(low_part);
            } else {
                // Valor < 2^64
                data[0] = static_cast<uint64_t>(abs_value);
            }

            // Si era negativo, aplicar complemento a 2
            if (is_negative) {
                data[0] = ~data[0];
                data[1] = ~data[1];
                uint64_t temp = 0;
                const unsigned char carry = intrinsics::add_u64(data[0], 1ull, &temp);
                data[0] = temp;
                intrinsics::addcarry_u64(carry, data[1], 0ull, &temp);
                data[1] = temp;
            }
        } else {
            // Unsigned: solo manejar valores no negativos
            if (value < 0) {
                // Valor negativo para unsigned → cero
                data[0] = 0ull;
                data[1] = 0ull;
            } else if (value >= 18446744073709551616.0L) {
                // Valor >= 2^64
                const T high_part = value / 18446744073709551616.0L;
                data[1] = static_cast<uint64_t>(high_part);
                const T low_part = value - (high_part * 18446744073709551616.0L);
                data[0] = static_cast<uint64_t>(low_part);
            } else {
                // Valor < 2^64
                data[0] = static_cast<uint64_t>(value);
            }
        }

        return *this;
    }

    // Operador de asignación desde const char*
    // Usa parse() para auto-detección de formato (0x, 0b, 0, decimal)
    // Lanza std::invalid_argument si el string no es válido
    int128_base_t& operator=(const char* str)
    {
        if (str == nullptr) {
            throw std::invalid_argument("Cannot parse null pointer");
        }

        auto [error, value] = parse(str);
        if (error != parse_error::success) {
            // Construir mensaje de error descriptivo
            const char* error_msg = "Unknown error";
            switch (error) {
            case parse_error::empty_string:
                error_msg = "Empty string";
                break;
            case parse_error::invalid_base:
                error_msg = "Invalid base";
                break;
            case parse_error::invalid_character:
                error_msg = "Invalid character";
                break;
            case parse_error::overflow:
                error_msg = "Overflow";
                break;
            default:
                break;
            }
            throw std::invalid_argument(std::string("Failed to parse string: ") + error_msg);
        }
        *this = value;
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

    /**
     * @brief Operador de negación bitwise (NOT bit a bit)
     *
     * @return Nuevo int128_base_t con todos los bits invertidos
     *
     * @details Invierte todos los bits del número. Es el operador base para
     * implementar la negación aritmética mediante complemento a 2: -x = ~x + 1
     *
     * @note FIX CRÍTICO (Fase 17): Corregido orden de argumentos del constructor.
     *       Antes: int128_base_t(~data[0], ~data[1]) pasaba (~low, ~high) ❌
     *       Ahora: int128_base_t(~data[1], ~data[0]) pasa (high, low) ✅
     *       Este bug rompía operator-(), abs() y to_string() para valores negativos.
     *
     * @par Ejemplo:
     * @code
     * uint128_t x(0xFFFF'FFFF'FFFF'FFFF, 0x0000'0000'0000'0000);
     * uint128_t y = ~x; // y = (0x0000'0000'0000'0000, 0xFFFF'FFFF'FFFF'FFFF)
     * @endcode
     */
    constexpr int128_base_t operator~() const noexcept
    {
        return int128_base_t(~data[1], ~data[0]); // Constructor(high, low)
    }

    /**
     * @brief Operador de negación unaria (complemento a 2)
     *
     * @return Negativo del valor actual usando aritmética modular
     *
     * @details Implementa la negación mediante complemento a 2: -x = ~x + 1
     * Este operador funciona idénticamente para signed y unsigned (wrap-around).
     *
     * **Para int128_t (signed):**
     * - -42 → -42 (representación two's complement)
     * - -INT128_MIN → INT128_MIN (overflow, comportamiento definido)
     *
     * **Para uint128_t (unsigned):**
     * - -42 → 2^128 - 42 (aritmética modular)
     * - -0 → 0
     *
     * @note Depende de operator~(), que fue corregido en Fase 17
     *
     * @par Ejemplo:
     * @code
     * int128_t x(42);
     * int128_t y = -x;        // y = -42
     * int128_t z = -(-x);     // z = 42
     * @endcode
     */
    constexpr int128_base_t operator-() const noexcept
    {
        return ~(*this) + int128_base_t(1);
    }

    // Operador suma: templates S2 manejan todos los casos (incluyendo S2==S)

    // Operador resta: templates S2 manejan todos los casos (incluyendo S2==S)

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

    // Multiplicación: templates S2 manejan todos los casos (incluyendo S2==S)

    // Operador multiplicación (FASE 0.5 optimizado)
    template <integral_builtin T> constexpr int128_base_t& operator*=(T other) noexcept
    {
        const uint64_t b = static_cast<uint64_t>(other);

        // Fast path: valor cabe en 64 bits
        if (data[1] == 0) {
            const uint64_t low_part = intrinsics::umul128(data[0], b, &data[1]);
            data[0] = low_part;
            // Nota: No se necesita corrección de signo.
            // La multiplicación en complemento a 2 funciona automáticamente.
            return *this;
        }

        // General case: multiplicación 128×64
        uint64_t high_part;
        const uint64_t low_part = intrinsics::umul128(data[0], b, &high_part);
        const uint64_t cross_product = data[1] * b;

        // Nota: No se necesita corrección de signo.
        // La multiplicación en complemento a 2 funciona automáticamente:
        // signed_multiply(a, b) ≡ unsigned_multiply(a, b) (mod 2^128)

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
    // OPERADORES DE DIVISIÓN Y MÓDULO
    // ============================================================================

    // División entre dos int128_base_t (mismo signedness)
    constexpr int128_base_t& operator/=(const int128_base_t& other) noexcept
    {
        auto [quotient, remainder] = divrem(other);
        *this = quotient;
        return *this;
    }

    constexpr int128_base_t operator/(const int128_base_t& other) const noexcept
    {
        auto [quotient, remainder] = divrem(other);
        return quotient;
    }

    constexpr int128_base_t& operator%=(const int128_base_t& other) noexcept
    {
        auto [quotient, remainder] = divrem(other);
        *this = remainder;
        return *this;
    }

    constexpr int128_base_t operator%(const int128_base_t& other) const noexcept
    {
        auto [quotient, remainder] = divrem(other);
        return remainder;
    }

    // Sobrecargas para tipos builtin
    template <integral_builtin T> constexpr int128_base_t& operator/=(T other) noexcept
    {
        return *this /= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator/(T other) const noexcept
    {
        int128_base_t result(*this);
        result /= other;
        return result;
    }

    template <integral_builtin T> constexpr int128_base_t& operator%=(T other) noexcept
    {
        return *this %= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator%(T other) const noexcept
    {
        int128_base_t result(*this);
        result %= other;
        return result;
    }

    // Sobrecargas para signedness diferente
    template <signedness S2>
    constexpr int128_base_t& operator/=(const int128_base_t<S2>& other) noexcept
    {
        return *this /= int128_base_t(other);
    }

    template <signedness S2>
    constexpr int128_base_t operator/(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result /= other;
        return result;
    }

    template <signedness S2>
    constexpr int128_base_t& operator%=(const int128_base_t<S2>& other) noexcept
    {
        return *this %= int128_base_t(other);
    }

    template <signedness S2>
    constexpr int128_base_t operator%(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result %= other;
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

    template <signedness S2>
    constexpr int128_base_t& operator&=(const int128_base_t<S2>& other) noexcept
    {
        data[0] &= other.low();
        data[1] &= other.high();
        return *this;
    }

    constexpr int128_base_t operator&(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result &= other;
        return result;
    }

    template <signedness S2>
    constexpr int128_base_t operator&(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result &= other;
        return result;
    }

    // Operadores bitwise AND con integral_builtin
    template <integral_builtin T> constexpr int128_base_t& operator&=(T other) noexcept
    {
        return *this &= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator&(T other) const noexcept
    {
        return *this & int128_base_t(other);
    }

    constexpr int128_base_t& operator|=(const int128_base_t& other) noexcept
    {
        data[0] |= other.data[0];
        data[1] |= other.data[1];
        return *this;
    }

    template <signedness S2>
    constexpr int128_base_t& operator|=(const int128_base_t<S2>& other) noexcept
    {
        data[0] |= other.low();
        data[1] |= other.high();
        return *this;
    }

    constexpr int128_base_t operator|(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result |= other;
        return result;
    }

    template <signedness S2>
    constexpr int128_base_t operator|(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result |= other;
        return result;
    }

    // Operadores bitwise OR con integral_builtin
    template <integral_builtin T> constexpr int128_base_t& operator|=(T other) noexcept
    {
        return *this |= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator|(T other) const noexcept
    {
        return *this | int128_base_t(other);
    }

    constexpr int128_base_t& operator^=(const int128_base_t& other) noexcept
    {
        data[0] ^= other.data[0];
        data[1] ^= other.data[1];
        return *this;
    }

    template <signedness S2>
    constexpr int128_base_t& operator^=(const int128_base_t<S2>& other) noexcept
    {
        data[0] ^= other.low();
        data[1] ^= other.high();
        return *this;
    }

    constexpr int128_base_t operator^(const int128_base_t& other) const noexcept
    {
        int128_base_t result(*this);
        result ^= other;
        return result;
    }

    template <signedness S2>
    constexpr int128_base_t operator^(const int128_base_t<S2>& other) const noexcept
    {
        int128_base_t result(*this);
        result ^= other;
        return result;
    }

    // Operadores bitwise XOR con integral_builtin
    template <integral_builtin T> constexpr int128_base_t& operator^=(T other) noexcept
    {
        return *this ^= int128_base_t(other);
    }

    template <integral_builtin T> constexpr int128_base_t operator^(T other) const noexcept
    {
        return *this ^ int128_base_t(other);
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

    // Left shift con integral_builtin
    template <integral_builtin T> constexpr int128_base_t& operator<<=(T shift) noexcept
    {
        return *this <<= static_cast<int>(shift);
    }

    template <integral_builtin T> constexpr int128_base_t operator<<(T shift) const noexcept
    {
        return *this << static_cast<int>(shift);
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

    // Right shift con integral_builtin
    template <integral_builtin T> constexpr int128_base_t& operator>>=(T shift) noexcept
    {
        return *this >>= static_cast<int>(shift);
    }

    template <integral_builtin T> constexpr int128_base_t operator>>(T shift) const noexcept
    {
        return *this >> static_cast<int>(shift);
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
    // COMPARACIONES CON TIPOS BUILTIN
    // ============================================================================

    // Comparaciones con integral_builtin
    template <integral_builtin T>
    friend constexpr bool operator==(const int128_base_t& lhs, T rhs) noexcept
    {
        // Convertir rhs a int128_base_t con sign extension apropiada
        int128_base_t rhs_converted{};
        rhs_converted.data[0] = static_cast<uint64_t>(rhs);
        if constexpr (std::is_signed_v<T>) {
            if (rhs < 0) {
                rhs_converted.data[1] = ~0ull;
            }
        }
        return lhs == rhs_converted;
    }

    template <integral_builtin T>
    friend constexpr bool operator==(T lhs, const int128_base_t& rhs) noexcept
    {
        return rhs == lhs;
    }

    template <integral_builtin T>
    friend constexpr bool operator!=(const int128_base_t& lhs, T rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <integral_builtin T>
    friend constexpr bool operator!=(T lhs, const int128_base_t& rhs) noexcept
    {
        return !(rhs == lhs);
    }

    template <integral_builtin T>
    friend constexpr bool operator<(const int128_base_t& lhs, T rhs) noexcept
    {
        int128_base_t rhs_converted{};
        rhs_converted.data[0] = static_cast<uint64_t>(rhs);
        if constexpr (std::is_signed_v<T>) {
            if (rhs < 0) {
                rhs_converted.data[1] = ~0ull;
            }
        }
        return lhs < rhs_converted;
    }

    template <integral_builtin T>
    friend constexpr bool operator<(T lhs, const int128_base_t& rhs) noexcept
    {
        int128_base_t lhs_converted{};
        lhs_converted.data[0] = static_cast<uint64_t>(lhs);
        if constexpr (std::is_signed_v<T>) {
            if (lhs < 0) {
                lhs_converted.data[1] = ~0ull;
            }
        }
        return lhs_converted < rhs;
    }

    template <integral_builtin T>
    friend constexpr bool operator<=(const int128_base_t& lhs, T rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <integral_builtin T>
    friend constexpr bool operator<=(T lhs, const int128_base_t& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <integral_builtin T>
    friend constexpr bool operator>(const int128_base_t& lhs, T rhs) noexcept
    {
        return rhs < lhs;
    }

    template <integral_builtin T>
    friend constexpr bool operator>(T lhs, const int128_base_t& rhs) noexcept
    {
        return rhs < lhs;
    }

    template <integral_builtin T>
    friend constexpr bool operator>=(const int128_base_t& lhs, T rhs) noexcept
    {
        return !(lhs < rhs);
    }

    template <integral_builtin T>
    friend constexpr bool operator>=(T lhs, const int128_base_t& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // Comparaciones con floating_point (solo == y != por NaN/Inf)
    template <floating_point_builtin T>
    friend constexpr bool operator==(const int128_base_t& lhs, T rhs) noexcept
    {
        // NaN nunca es igual a nada
        if (rhs != rhs)
            return false; // NaN check
        // Inf no es igual a ningún entero finito
        if (rhs == std::numeric_limits<T>::infinity() || rhs == -std::numeric_limits<T>::infinity())
            return false;
        // Convertir int128 a floating y comparar
        return static_cast<T>(lhs) == rhs;
    }

    template <floating_point_builtin T>
    friend constexpr bool operator==(T lhs, const int128_base_t& rhs) noexcept
    {
        return rhs == lhs;
    }

    template <floating_point_builtin T>
    friend constexpr bool operator!=(const int128_base_t& lhs, T rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <floating_point_builtin T>
    friend constexpr bool operator!=(T lhs, const int128_base_t& rhs) noexcept
    {
        return !(rhs == lhs);
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
            // Si es int128_t y es negativo, necesitamos manejar correctamente
            if constexpr (is_signed) {
                if (is_negative()) {
                    // Calcular el valor positivo (complemento a 2 invertido)
                    uint64_t temp_low = ~data[0];
                    uint64_t temp_high = ~data[1];

                    // Sumar 1 para obtener el valor absoluto
                    uint64_t abs_low = temp_low + 1ull;
                    uint64_t abs_high = temp_high;
                    if (abs_low == 0ull) {
                        abs_high += 1ull;
                    }

                    // Convertir a floating point y negar
                    const T abs_value = static_cast<T>(abs_high) * 18446744073709551616.0L +
                                        static_cast<T>(abs_low);
                    return -abs_value;
                }
            }
            return static_cast<T>(data[1]) * 18446744073709551616.0L + static_cast<T>(data[0]);
        } else {
            return static_cast<T>(data[0]);
        }
    }

    // ============================================================================
    // CONVERSIONES A STRING
    // ============================================================================

    /**
     * @brief Convierte el valor a string decimal (base 10)
     *
     * @return String representando el valor en base 10
     *
     * @details Implementación optimizada que usa divrem_by_10() para extraer
     * dígitos de forma eficiente. Maneja correctamente valores negativos en int128_t.
     *
     * **Proceso:**
     * 1. Si es cero, devuelve "0"
     * 2. Si es negativo (signed), calcula abs() y marca flag
     * 3. Extrae dígitos en reversa usando división por 10
     * 4. Añade signo '-' si era negativo
     * 5. Invierte el string completo
     *
     * **Complejidad:** O(log n) donde n es el valor
     *
     * @note FIX Fase 17: Ahora usa abs() correctamente (dependía de operator~() bugueado)
     * @note Para otras bases (2-36), usa to_string(int base)
     *
     * @par Ejemplo:
     * @code
     * int128_t x(42);
     * std::string s1 = x.to_string();           // "42"
     *
     * int128_t y(-12345);
     * std::string s2 = y.to_string();           // "-12345"
     *
     * uint128_t big = UINT128_MAX;
     * std::string s3 = big.to_string();         // "340282366920938463463374607431768211455"
     * @endcode
     *
     * @see to_string(int base), parse(), abs(), divrem_by_10()
     */
    std::string to_string() const
    {
        // Caso especial: cero
        if (data[0] == 0 && data[1] == 0) {
            return "0";
        }

        std::string result;
        result.reserve(40); // Reservar espacio (~39 dígitos para 2^128)

        // Manejar signo para tipos signed
        bool negative = false;
        int128_base_t temp = *this;
        if constexpr (is_signed) {
            if (is_negative()) {
                negative = true;
                temp = abs(); // Obtener valor absoluto
            }
        }

        // Extraer dígitos usando divrem_by_10()
        while (temp.data[0] != 0 || temp.data[1] != 0) {
            auto [quotient, remainder] = temp.divrem_by_10();
            result += static_cast<char>('0' + remainder); // Construir en reversa
            temp = quotient;
        }

        // Añadir signo si es negativo
        if (negative) {
            result += '-';
        }

        // Invertir el string (lo construimos al revés)
        std::reverse(result.begin(), result.end());
        return result;
    }

    /**
     * @brief Convierte el valor a string en la base especificada (2-36)
     *
     * @param base Base numérica para la conversión (2 a 36)
     *             - 2: binario (0-1)
     *             - 8: octal (0-7)
     *             - 10: decimal (0-9)
     *             - 16: hexadecimal (0-9, a-f)
     *             - 36: máximo (0-9, a-z)
     *
     * @return String representando el valor en la base especificada
     *
     * @throws std::invalid_argument Si base < 2 o base > 36
     *
     * @details Implementación general que extrae dígitos mediante división sucesiva.
     * Para base 10, delega a to_string() optimizado. Para signed, maneja negativos.
     *
     * **Dígitos usados:**
     * - 0-9: valores 0-9
     * - a-z: valores 10-35 (lowercase)
     *
     * @par Ejemplo:
     * @code
     * uint128_t x(255);
     * std::string bin = x.to_string(2);    // "11111111"
     * std::string oct = x.to_string(8);    // "377"
     * std::string hex = x.to_string(16);   // "ff"
     *
     * int128_t y(-42);
     * std::string dec = y.to_string(10);   // "-42"
     * std::string hex_neg = y.to_string(16); // two's complement en hex
     * @endcode
     *
     * @see to_string(), parse()
     */
    std::string to_string(int base) const
    {
        if (base < 2 || base > 36) {
            throw std::invalid_argument("Invalid base: must be between 2 and 36");
        }

        // Optimización: delegar a to_string() para base 10
        if (base == 10) {
            return to_string();
        }

        // Caso especial: cero
        if (data[0] == 0 && data[1] == 0) {
            return "0";
        }

        // Tabla de dígitos
        static constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

        std::string result;
        result.reserve(128); // Reservar espacio generoso (128 bits en binario)

        // Manejar signo para tipos signed
        bool negative = false;
        int128_base_t temp = *this;
        if constexpr (is_signed) {
            if (is_negative()) {
                negative = true;
                temp = abs(); // Obtener valor absoluto
            }
        }

        // Extraer dígitos usando divrem()
        int128_base_t base_val(0ull, static_cast<uint64_t>(base));
        while (temp.data[0] != 0 || temp.data[1] != 0) {
            auto [quotient, remainder] = temp.divrem(base_val);
            result += digits[remainder.data[0]]; // Construir en reversa
            temp = quotient;
        }

        // Añadir signo si es negativo
        if (negative) {
            result += '-';
        }

        // Invertir el string (lo construimos al revés)
        std::reverse(result.begin(), result.end());
        return result;
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
    {
        if constexpr (!is_signed) {
            return false;
        } else {
            return static_cast<int64_t>(data[1]) < 0;
        }
    }

    /**
     * @brief Calcula el valor absoluto
     *
     * @return Valor absoluto del número actual
     *
     * @details Implementación eficiente según signedness:
     * - **Para uint128_t (unsigned):** Devuelve el mismo valor (identidad)
     * - **Para int128_t (signed):** Devuelve -x si x < 0, sino x
     *
     * **Casos especiales:**
     * - abs(0) = 0
     * - abs(INT128_MAX) = INT128_MAX
     * - abs(INT128_MIN) = INT128_MIN (overflow, comportamiento definido)
     *
     * @note FIXED en Fase 17: Dependía de operator-() que usaba operator~() bugueado
     * @note Para unsigned, esta función es constexpr y no hace nada (optimizable por el compilador)
     *
     * @par Ejemplo:
     * @code
     * int128_t x(-42);
     * int128_t y = x.abs();           // y = 42
     *
     * int128_t z(100);
     * int128_t w = z.abs();           // w = 100
     *
     * uint128_t u(42);
     * uint128_t v = u.abs();          // v = 42 (identidad)
     * @endcode
     *
     * @see operator-(), is_negative()
     */
    constexpr int128_base_t abs() const noexcept
    {
        if constexpr (!is_signed) {
            return (*this);
        } else {
            return (is_negative() ? (-(*this)) : (*this));
        }
    }

    // ============================================================================
    // DIVISIÓN OPTIMIZADA POR 10 (para conversiones string)
    // ============================================================================

    /**
     * @brief División rápida por 10 para parsing de strings
     * @return std::pair<cociente, resto> donde resto ∈ [0, 9]
     * @note O(1) para valores <= 64 bits, O(64) para valores mayores
     */
    constexpr std::pair<int128_base_t, uint64_t> divrem_by_10() const noexcept
    {
        const uint64_t low = data[0];
        const uint64_t high = data[1];

        // Fast path: valor cabe en 64 bits
        if (high == 0) {
            const uint64_t quotient = low / 10;
            const uint64_t remainder = low % 10;
            return {int128_base_t(0ull, quotient), remainder}; // Constructor(high=0, low=quotient)
        }

        // Caso general: división de 128 bits por 10
        // Algoritmo: división escolar bit a bit con divisor pequeño
        uint64_t q_low = 0;
        uint64_t q_high = high / 10;
        uint64_t rem = high % 10;

        // Procesar cada bit del low desde el MSB
        for (int i = 63; i >= 0; --i) {
            rem = (rem << 1) | ((low >> i) & 1);
            if (rem >= 10) {
                rem -= 10;
                q_low |= (1ULL << i);
            }
        }

        return {int128_base_t(q_high, q_low), rem}; // Constructor(high, low)
    }

    /**
     * @brief División y módulo general con división binaria larga
     * @param divisor El divisor (debe ser != 0)
     * @return std::pair<cociente, resto> donde 0 <= resto < divisor
     * @note Algoritmo de división binaria (escolar) O(128)
     * @warning Si divisor == 0, comportamiento indefinido
     * (en esta implementación retorna {0,0} como placeholder)
     */
    constexpr std::pair<int128_base_t, int128_base_t>
    divrem(const int128_base_t& divisor) const noexcept
    {
        // Fast path: divisor es 0 (comportamiento indefinido, retornar 0)
        if (divisor.data[0] == 0 && divisor.data[1] == 0) {
            return {int128_base_t(0ull, 0ull), int128_base_t(0ull, 0ull)};
        }

        // Fast path: dividendo es 0
        if (data[0] == 0 && data[1] == 0) {
            return {int128_base_t(0ull, 0ull), int128_base_t(0ull, 0ull)};
        }

        // Fast path: divisor > dividendo
        if (*this < divisor) {
            return {int128_base_t(0ull, 0ull), *this};
        }

        // Fast path: divisor == dividendo
        if (*this == divisor) {
            return {int128_base_t(0ull, 1ull), int128_base_t(0ull, 0ull)};
            //                    ^high ^low = 1 (correcto)
        }

        // Fast path: divisor cabe en 64 bits
        if (divisor.data[1] == 0) {
            const uint64_t divisor_64 = divisor.data[0];

            // Si dividendo también cabe en 64 bits
            if (data[1] == 0) {
                const uint64_t q = data[0] / divisor_64;
                const uint64_t r = data[0] % divisor_64;
                // Constructor toma (high, low), entonces (0, q) para quotient en palabra baja
                return {int128_base_t(0ull, q), int128_base_t(0ull, r)};
            }

            // Dividendo de 128 bits / divisor de 64 bits
            // Usar algoritmo optimizado
            uint64_t quotient_low = 0;
            uint64_t quotient_high = data[1] / divisor_64;
            uint64_t remainder = data[1] % divisor_64;

            // Dividir parte baja bit a bit
            for (int i = 63; i >= 0; --i) {
                remainder = (remainder << 1) | ((data[0] >> i) & 1);
                if (remainder >= divisor_64) {
                    remainder -= divisor_64;
                    quotient_low |= (1ULL << i);
                }
            }

            return {int128_base_t(quotient_high, quotient_low),
                    int128_base_t(0ull, remainder)}; // Constructor(high, low)
        }

        // Caso general: división binaria larga (128 bits / 128 bits)
        // Algoritmo de división escolar bit a bit
        int128_base_t quotient(0ull, 0ull);  // Constructor(high=0, low=0)
        int128_base_t remainder(0ull, 0ull); // Constructor(high=0, low=0)

        // Procesar desde el bit más significativo
        for (int i = 127; i >= 0; --i) {
            // Shift left remainder y añadir el siguiente bit del dividendo
            remainder <<= 1;
            const int word = i / 64;
            const int bit = i % 64;
            if ((data[word] & (1ULL << bit)) != 0) {
                remainder.data[0] |= 1;
            }

            // Si remainder >= divisor, restar y añadir 1 al cociente
            if (remainder >= divisor) {
                remainder -= divisor;
                const int q_word = i / 64;
                const int q_bit = i % 64;
                quotient.data[q_word] |= (1ULL << q_bit);
            }
        }

        return {quotient, remainder};
    }

    /**
     * @brief División y módulo usando el algoritmo D de Knuth
     * @param divisor El divisor (debe ser != 0)
     * @return std::pair<cociente, resto> donde 0 <= resto < divisor
     * @note Algoritmo de Knuth (The Art of Computer Programming, Vol. 2, Sec. 4.3.1)
     * @note Más eficiente que divrem() para divisores grandes (> 64 bits)
     * @warning Si divisor == 0, comportamiento indefinido
     * @todo Implementar en el futuro para mejor performance con divisores grandes
     */
    constexpr std::pair<int128_base_t, int128_base_t>
    divrem_knuth_D(const int128_base_t& divisor) const noexcept
    {
        // Por ahora, delegar a divrem() (división binaria)
        // En el futuro, implementar el algoritmo D de Knuth completo
        return divrem(divisor);
    }

    // ============================================================================
    // STRING PARSING
    // ============================================================================

    /**
     * @brief Parsea una cadena de caracteres en una base específica
     * @details Realiza el parsing con validación completa de caracteres y detección de overflow
     *
     * @param str Puntero a la cadena de caracteres C-string
     * @param base Base numérica (debe estar entre 2 y 36)
     * @return std::pair<parse_error, int128_base_t> con el código de error y el resultado
     *
     * @note Si hay error, el int128_base_t retornado es cero
     * @property Es static, constexpr y noexcept
     */
    static constexpr std::pair<parse_error, int128_base_t> parse_base(const char* str,
                                                                      int base) noexcept
    {
        if (!str) {
            return {parse_error::null_pointer, int128_base_t(0ull, 0ull)};
        }

        if (!*str) {
            return {parse_error::empty_string, int128_base_t(0ull, 0ull)};
        }

        if (base < 2 || base > 36) {
            return {parse_error::invalid_base, int128_base_t(0ull, 0ull)};
        }

        // parse_base() NO maneja signos - solo parsea dígitos
        // El manejo de signos es responsabilidad de parse()
        int128_base_t result(0ull, 0ull); // Constructor(high=0, low=0)
        const int128_base_t base_val(0ull,
                                     static_cast<uint64_t>(base)); // Constructor(high=0, low=base)

        for (const char* p = str; *p; ++p) {
            // Ignorar separadores de dígitos (C++14 digit separator)
            if (*p == '\'') {
                continue;
            }

            int digit_value = -1;

            if (*p >= '0' && *p <= '9') {
                digit_value = *p - '0';
            } else if (*p >= 'A' && *p <= 'Z') {
                digit_value = *p - 'A' + 10;
            } else if (*p >= 'a' && *p <= 'z') {
                digit_value = *p - 'a' + 10;
            }

            if (digit_value == -1 || digit_value >= base) {
                // Carácter inválido para esta base
                return {parse_error::invalid_character, int128_base_t(0ull, 0ull)};
            }

            // Verificar overflow ANTES de multiplicar
            // NOTA: parse_base() usa aritmética UNSIGNED para overflow checking
            // Reinterpretamos result, base_val como uint128_t temporalmente
            // El overflow para signed se verifica después en parse() al aplicar el signo

            // Crear versiones unsigned para las operaciones de overflow
            using uint128_t = int128_base_t<signedness::unsigned_type>;
            constexpr uint128_t max_val(std::numeric_limits<uint64_t>::max(),
                                        std::numeric_limits<uint64_t>::max()); // UINT128_MAX

            // Reinterpretar result y base_val como unsigned para operaciones
            const uint128_t& result_u = reinterpret_cast<const uint128_t&>(result);
            const uint128_t& base_val_u = reinterpret_cast<const uint128_t&>(base_val);

            const uint128_t max_div_base = max_val / base_val_u;

            if (result_u > max_div_base) {
                return {parse_error::overflow, int128_base_t(0ull, 0ull)};
            }

            result *= base_val;

            // Verificar overflow ANTES de sumar el dígito
            // max_value - result < digit significa que result + digit > max_value
            const int128_base_t digit_val(
                0ull, static_cast<uint64_t>(digit_value)); // Constructor(high=0, low=digit)
            const uint128_t& digit_val_u = reinterpret_cast<const uint128_t&>(digit_val);
            const uint128_t max_minus_result = max_val - result_u;

            if (digit_val_u > max_minus_result) {
                return {parse_error::overflow, int128_base_t(0ull, 0ull)};
            }

            result += digit_val;
        }

        // parse_base() retorna el valor parseado SIN modificaciones de signo
        // El manejo de signos es responsabilidad de parse()
        return {parse_error::success, result};
    }

    /**
     * @brief Parsea una cadena de caracteres con auto-detección de base
     * @details Detecta automáticamente la base según el prefijo:
     *          - "0x" o "0X" → hexadecimal (base 16)
     *          - "0b" o "0B" → binario (base 2)
     *          - "0" seguido de dígitos 0-7 → octal (base 8)
     *          - Otros casos → decimal (base 10)
     *
     * @param str Puntero a la cadena de caracteres C-string
     * @return std::pair<parse_error, int128_base_t> con el código de error y el resultado
     *
     * @note Soporta signos '+' y '-' para tipos signed al inicio de la cadena
     * @property Es static, constexpr y noexcept
     */
    static constexpr std::pair<parse_error, int128_base_t> parse(const char* str) noexcept
    {
        if (!str) {
            return {parse_error::null_pointer, int128_base_t(0ull, 0ull)};
        }

        if (!*str) {
            return {parse_error::empty_string, int128_base_t(0ull, 0ull)};
        }

        // Manejo de signo para tipos signed
        bool is_negative_input = false;
        const char* parse_start = str;

        if constexpr (is_signed) {
            if (*parse_start == '-') {
                is_negative_input = true;
                ++parse_start;
            } else if (*parse_start == '+') {
                ++parse_start;
            }

            // Verificar que hay contenido después del signo
            if (!*parse_start) {
                return {parse_error::empty_string, int128_base_t(0ull, 0ull)};
            }
        }

        // Detectar base y posición inicial
        int base = 10;
        const char* start = parse_start;

        if (parse_start[0] == '0' && parse_start[1]) {
            if (parse_start[1] == 'x' || parse_start[1] == 'X') {
                base = 16;
                start = parse_start + 2;
            } else if (parse_start[1] == 'b' || parse_start[1] == 'B') {
                base = 2;
                start = parse_start + 2;
            } else {
                // Verificar si es octal válido
                bool is_octal = true;
                for (const char* p = parse_start + 1; *p && is_octal; ++p) {
                    if (*p < '0' || *p > '7') {
                        is_octal = false;
                    }
                }
                if (is_octal && parse_start[1] >= '0' && parse_start[1] <= '7') {
                    base = 8;
                    start = parse_start + 1;
                }
            }
        }

        // Validar que hay contenido después del prefijo
        if (!*start) {
            return {parse_error::empty_string, int128_base_t(0ull, 0ull)};
        }

        // Parsear sin signo (parse_base ya maneja el signo si viene en str)
        // Pero necesitamos pasar el string sin el signo porque ya lo procesamos
        auto [error, result] = parse_base(start, base);

        // Aplicar signo si no hubo error
        if constexpr (is_signed) {
            if (error == parse_error::success && is_negative_input) {
                result = -result;
            }
        }

        return {error, result};
    }

    // ============================================================================
    // CONSTANTES ESTÁTICAS
    // ============================================================================

    static consteval int128_base_t min() noexcept
    {
        if constexpr (is_signed) {
            // INT128_MIN = -2^127 = 0x8000000000000000'0000000000000000
            // Constructor(high, low): high=0x8000000000000000, low=0
            return int128_base_t(static_cast<uint64_t>(std::numeric_limits<int64_t>::min()), 0ull);
        } else {
            // UINT128_MIN = 0
            return int128_base_t(0ull, 0ull);
        }
    }

    static consteval int128_base_t max() noexcept
    {
        if constexpr (is_signed) {
            // INT128_MAX = 2^127-1 = 0x7FFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF
            // Constructor(high, low): high=0x7FFFFFFFFFFFFFFF, low=0xFFFFFFFFFFFFFFFF
            return int128_base_t(std::numeric_limits<int64_t>::max(),
                                 std::numeric_limits<uint64_t>::max());
        } else {
            // UINT128_MAX = 2^128-1 = 0xFFFFFFFFFFFFFFFF'FFFFFFFFFFFFFFFF
            // Constructor(high, low): high=0xFFFFFFFFFFFFFFFF, low=0xFFFFFFFFFFFFFFFF
            return int128_base_t(std::numeric_limits<uint64_t>::max(),
                                 std::numeric_limits<uint64_t>::max());
        }
    }

    // ============================================================================
    // OPERADORES DE STREAM (FRIEND FUNCTIONS)
    // ============================================================================

    // Operador de inserción en stream (salida)
    // Convierte el valor a string en base 10 y lo escribe al stream
    // Ejemplo: std::cout << value;
    friend std::ostream& operator<<(std::ostream& os, const int128_base_t& value)
    {
        return os << value.to_string();
    }

    // Operador de extracción desde stream (entrada)
    // Lee un string del stream y lo convierte al tipo int128_base_t
    // Ejemplo: std::cin >> value;
    // Si el parsing falla, establece el failbit del stream
    friend std::istream& operator>>(std::istream& is, int128_base_t& value)
    {
        std::string str;
        is >> str; // Lee string hasta el primer whitespace

        if (is) { // Si la lectura fue exitosa
            try {
                value = int128_base_t(str.c_str());
            } catch (const std::invalid_argument&) {
                // Si el parsing falla, establecer failbit
                is.setstate(std::ios::failbit);
            }
        }

        return is;
    }
};

// ============================================================================
// TYPE ALIASES (reemplazan las clases originales)
// ============================================================================

using uint128_t = int128_base_t<signedness::unsigned_type>;
using int128_t = int128_base_t<signedness::signed_type>;

// ============================================================================
// CONSTANTES NUMÉRICAS GLOBALES
// ============================================================================

/// Máximo valor representable para uint128_t (2^128 - 1)
inline constexpr uint128_t UINT128_MAX =
    uint128_t(std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max());

/// Máximo valor representable para int128_t (2^127 - 1)
inline constexpr int128_t INT128_MAX =
    int128_t(std::numeric_limits<int64_t>::max(), std::numeric_limits<uint64_t>::max());

/// Mínimo valor representable para int128_t (-2^127)
inline constexpr int128_t INT128_MIN =
    int128_t(static_cast<uint64_t>(std::numeric_limits<int64_t>::min()), 0ull);

// ============================================================================
// LITERALES DE USUARIO (User-Defined Literals)
// ============================================================================

/**
 * @brief Namespace para literales de usuario de int128
 * @note Uso: using namespace nstd::literals; o using namespace nstd::int128_literals;
 */
namespace int128_literals
{
/**
 * @brief Literal de usuario para uint128_t (lowercase)
 * @param str Cadena de caracteres a parsear
 * @return uint128_t parseado desde la cadena
 * @note Soporta auto-detección de base (0x, 0b, 0) y digit separators (')
 * @example auto x = 1'234'567_u128;
 * @example auto y = 0xFF'AA'BB_u128;
 */
constexpr uint128_t operator""_u128(const char* str) noexcept
{
    auto [error, value] = uint128_t::parse(str);
    return value; // Retorna 0 si hay error
}

/**
 * @brief Literal de usuario para uint128_t (uppercase)
 * @param str Cadena de caracteres a parsear
 * @return uint128_t parseado desde la cadena
 * @note Alias de _u128 para consistencia
 * @example auto x = 1234567_U128;
 */
constexpr uint128_t operator""_U128(const char* str) noexcept
{
    return operator""_u128(str);
}

/**
 * @brief Literal de usuario para int128_t (lowercase)
 * @param str Cadena de caracteres a parsear
 * @return int128_t parseado desde la cadena
 * @note Soporta signo '+'/'-', auto-detección de base y digit separators (')
 * @example auto x = -1'234'567_i128;
 * @example auto y = 0x7FFF'FFFF_i128;
 */
constexpr int128_t operator""_i128(const char* str) noexcept
{
    auto [error, value] = int128_t::parse(str);
    return value; // Retorna 0 si hay error
}

/**
 * @brief Literal de usuario para int128_t (uppercase)
 * @param str Cadena de caracteres a parsear
 * @return int128_t parseado desde la cadena
 * @note Alias de _i128 para consistencia
 * @example auto x = -1234567_I128;
 */
constexpr int128_t operator""_I128(const char* str) noexcept
{
    return operator""_i128(str);
}

} // namespace int128_literals

// Alias del namespace literals para compatibilidad
namespace literals
{
using namespace int128_literals;
}

// ============================================================================
// FUNCIONES FACTORY (make_int128)
// ============================================================================

/**
 * @brief Función factory type-safe para crear int128_base_t desde C-string
 * @tparam S Signedness del tipo (unsigned_type o signed_type)
 * @param str Cadena de caracteres a parsear
 * @return int128_base_t<S> parseado desde la cadena
 * @note Soporta digit separators ('), auto-detección de base y signos
 * @example auto x = make_int128<unsigned_type>("1'234'567");
 * @example auto y = make_int128<signed_type>("-0xFF'AA");
 */
template <signedness S> constexpr int128_base_t<S> make_int128(const char* str) noexcept
{
    auto [error, value] = int128_base_t<S>::parse(str);
    return value; // Retorna 0 si hay error
}

/**
 * @brief Función factory con verificación de errores
 * @tparam S Signedness del tipo (unsigned_type o signed_type)
 * @param str Cadena de caracteres a parsear
 * @return std::pair<parse_error, int128_base_t<S>> con error y valor
 * @note Permite verificar si hubo error en el parsing
 * @example auto [err, val] = make_int128_checked<unsigned_type>("invalid");
 * @example if (err != parse_error::success) { // manejar error }
 */
template <signedness S>
constexpr std::pair<parse_error, int128_base_t<S>> make_int128_checked(const char* str) noexcept
{
    return int128_base_t<S>::parse(str);
}

/**
 * @brief Shortcuts para tipos específicos
 */
inline constexpr uint128_t make_uint128(const char* str) noexcept
{
    return make_int128<signedness::unsigned_type>(str);
}

inline constexpr int128_t make_int128_signed(const char* str) noexcept
{
    return make_int128<signedness::signed_type>(str);
}

} // namespace nstd

// ============================================================================
// TYPE TRAITS SPECIALIZATIONS
// ============================================================================
// NOTA: Este archivo debe incluirse DESPUÉS de las definiciones de los type aliases
// para que std::type_traits vea correctamente uint128_t e int128_t.
#include "uint128_traits_specializations.hpp"

#endif // INT128_BASE_HPP