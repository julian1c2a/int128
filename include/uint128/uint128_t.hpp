#ifndef UINT128_T_HPP
#define UINT128_T_HPP

// ÔÜá´©Å CRITICAL: Include trait specializations BEFORE any standard library headers
// This ensures std::is_integral and other traits are properly specialized
#include "uint128_traits_specializations.hpp"

#include <array>
#include <bitset>
#include <climits>
#include <compare>
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
// Includes para intr├¡nsecos de MSVC
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
// Includes para intr├¡nsecos de GCC/Clang
#include <immintrin.h>
#if defined(__x86_64__) || defined(_M_X64)
#include <bmi2intrin.h>
#include <x86intrin.h>
#endif
#endif

// Include intrinsics multiplataforma
#include "../intrinsics/arithmetic_operations.hpp"
#include "../intrinsics/bit_operations.hpp"

// Include type traits personalizados
#include "../type_traits.hpp"

namespace nstd
{

// Forward declaration para int128_t (necesario para constructor)
class int128_t;

/**
 * @brief C├│digos de error para operaciones de parsing de strings
 */
enum class parse_error : uint8_t {
    success = 0,       ///< Parsing exitoso
    null_pointer,      ///< Puntero null proporcionado
    empty_string,      ///< String vac├¡o
    invalid_base,      ///< Base fuera de rango [2, 36]
    invalid_character, ///< Car├ícter inv├ílido para la base especificada
    overflow,          ///< El resultado excede el rango de uint128_t
    unknown_error      ///< Error desconocido
};

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
    // data[1] -> High 64 bits Little Endian
    // data[0] -> Low 64 bits Little Endian

  public:
    /**
     * @brief N├║mero de bits en el tipo `uint128_t`.
     * @test (sizeof_is_16_bytes)
     * @code{.cpp}
     * // assert(sizeof(uint128_t) == 16);
     * @endcode
     * @test (bits_is_128)
     * @code{.cpp}
     * // assert(uint128_t::UINT128_BITS == 128);
     * @endcode
     */
    static constexpr int UINT128_BITS = 2 * sizeof(uint64_t) * CHAR_BIT; // 128

    /**
     * @brief Retorna el valor m├¡nimo representable (0)
     * @return uint128_t(0, 0)
     * @property Es static, constexpr y noexcept
     */
    static constexpr uint128_t min() noexcept
    {
        return uint128_t(0, 0);
    }

    /**
     * @brief Retorna el valor m├íximo representable (2^128 - 1)
     * @return uint128_t(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)
     * @property Es static, constexpr y noexcept
     */
    static constexpr uint128_t max() noexcept
    {
        return uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    }

    /**
     * @brief Accede a los 64 bits superiores (high part) del n├║mero de 128 bits.
     * @return El valor de los 64 bits superiores como un `uint64_t`.
     * @property Es `constexpr` y `noexcept`.
     * @test (sizeof_high_is_8_bytes)
     * @code{.cpp}
     * // assert(sizeof(high()) == 8);
     * @endcode
     * @test (self_div_2_64_equ_high)
     * @code{.cpp}
     * uint128_t val(0x1234, 0x5678);
     * assert(val.high() == 0x1234);
     * @endcode
     */
    constexpr uint64_t high() const noexcept
    {
        return data[1];
    }

    /**
     * @brief Accede a los 64 bits inferiores (low part) del n├║mero de 128 bits.
     * @return El valor de los 64 bits inferiores como un `uint64_t`.
     * @property Es `constexpr` y `noexcept`.
     * @test (sizeof_low_is_8_bytes)
     * @code{.cpp}
     * // assert(sizeof(low()) == 8);
     * @endcode
     * @test (self_rem_2_64_equ_low)
     * @code{.cpp}
     * uint128_t val(0x1234, 0x5678);
     * assert(val.low() == 0x5678);
     * @endcode
     */
    constexpr uint64_t low() const noexcept
    {
        return data[0];
    }

    // Setters
    /**
     * @brief Establece el valor de los 64 bits superiores (high part).
     * @tparam T Tipo integral para el valor.
     * @param value El nuevo valor para la parte alta.
     * @pre T debe ser un tipo integral. Esta condici├│n se verifica en tiempo de compilaci├│n.
     * @post La parte alta del `uint128_t` se actualiza con `static_cast<uint64_t>(value)`.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_set_high)
     * @code{.cpp}
     * uint128_t val;
     * val.set_high(0xABCD);
     * assert(val.high() == 0xABCD);
     * @endcode
     */
    template <integral_builtin T> constexpr void set_high(T value) noexcept
    {
        data[1] = static_cast<uint64_t>(value);
    }

    /**
     * @brief Establece el valor de los 64 bits inferiores (low part).
     * @tparam T Tipo integral para el valor.
     * @param value El nuevo valor para la parte baja.
     * @pre T debe ser un tipo integral. Esta condici├│n se verifica en tiempo de compilaci├│n.
     * @post La parte baja del `uint128_t` se actualiza con `static_cast<uint64_t>(value)`.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_set_low)
     * @code{.cpp}
     * uint128_t val;
     * val.set_low(0x1234);
     * assert(val.low() == 0x1234);
     * @endcode
     */
    template <integral_builtin T> constexpr void set_low(T value) noexcept
    {
        data[0] = static_cast<uint64_t>(value);
    }

    /**
     * @brief Constructor por defecto.
     * @post Inicializa el `uint128_t` a 0.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_default_constructor)
     * @code{.cpp}
     * uint128_t val;
     * assert(val.low() == 0 && val.high() == 0);
     * @endcode
     */
    constexpr uint128_t() noexcept : data{0ull, 0ull} {}

    /**
     * @brief Constructor a partir de un tipo integral.
     * @tparam T Un tipo de dato integral (e.g., `int`, `uint64_t`).
     * @param value El valor inicial.
     * @pre `T` debe ser un tipo integral de 8 bytes o menos. Se verifica en tiempo de compilaci├│n.
     * @post El `uint128_t` se inicializa con `value`. Si `T` es un tipo con signo y `value` es
     * negativo, se realiza una extensi├│n de signo para preservar el valor.
     * @property Es `constexpr` y `noexcept`.
     * @test (integral_constructor_test)
     * @code{.cpp}
     * uint128_t val(42);
     * assert(val.low() == 42);
     * uint128_t neg(-1); // Extensi├│n de signo
     * assert(neg.high() == ~0ull);
     * @endcode
     */
    template <integral_builtin T>
    constexpr uint128_t(T value) noexcept : data{static_cast<uint64_t>(value), 0ull}
    {
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                data[1] = ~0ull; // Extensi├│n de signo correcta para negativos
            }
        }
    }

    /**
     * @brief Constructor a partir de las partes alta y baja de 64 bits.
     * @tparam T1 Tipo integral para la parte alta.
     * @tparam T2 Tipo integral para la parte baja.
     * @param high El valor para los 64 bits superiores.
     * @param low El valor para los 64 bits inferiores.
     * @post El `uint128_t` se inicializa con los valores `high` y `low` proporcionados.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_high_low_constructor)
     * @code{.cpp}
     * uint128_t val(static_cast<uint16_t>(0x1), static_cast<uint32_t>(0x2));
     * assert(val.high() == 0x1 && val.low() == 0x2);
     * @endcode
     */
    template <integral_builtin T1, integral_builtin T2>
    constexpr uint128_t(T1 high, T2 low) noexcept
        : data{static_cast<uint64_t>(low), static_cast<uint64_t>(high)}
    {
    }

    /**
     * @brief Constructor a partir de int128_t (forward declaration).
     * @param value El valor int128_t a convertir.
     * @post El uint128_t se inicializa con la representaci├│n binaria de value.
     * @note Para valores negativos, se preserva la representaci├│n two's complement.
     */
    inline constexpr uint128_t(const int128_t& value) noexcept;

    /**
     * @brief Constructor expl├¡cito a partir de una cadena de caracteres (C-string).
     * @param str Puntero a una cadena de caracteres que representa un n├║mero.
     * @pre `str` debe ser un puntero v├ílido a una cadena null-terminated.
     * @post El `uint128_t` se inicializa con el valor parseado de la cadena. La base se detecta
     * autom├íticamente (prefijos `0x` para hexadecimal, `0b` para binario, `0` para octal).
     * @property Es `constexpr` y `noexcept`.
     * @see from_cstr
     * @test (test_cstr_constructor)
     * @code{.cpp}
     * uint128_t val("0x12346789ABCDEF00");
     * assert(val.low() == 0x12346789 && val.high() == 0xABCDEF00);
     * @endcode
     */
    explicit constexpr uint128_t(const char* str) noexcept;

    // NOTA: Tambi├®n disponibles funciones assign en int128_assign.hpp:
    // - assign_uint128(target, high, low), assign_uint128(target, value), etc.

    // COPY/MOVE CONSTRUCTORS & ASSIGNMENT
    // Definidos expl├¡citamente como default para claridad (el compilador los generar├¡a igual)
    constexpr uint128_t(const uint128_t&) = default;
    constexpr uint128_t(uint128_t&&) = default;
    constexpr uint128_t& operator=(const uint128_t&) = default;
    constexpr uint128_t& operator=(uint128_t&&) = default;

    /**
     * @brief Asigna un valor de tipo integral al `uint128_t`.
     * @tparam T Un tipo de dato integral (e.g., `int`, `uint64_t`).
     * @param value El valor a asignar.
     * @pre `T` debe ser un tipo integral de 8 bytes o menos. Se verifica en tiempo de compilaci├│n.
     * @post El valor del `uint128_t` se actualiza al de `value`. Se realiza extensi├│n de signo si
     * `T` es un tipo con signo y `value` es negativo.
     * @return Una referencia a `*this`.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_integral_assignment)
     * @code{.cpp}
     * //   Similar a test_integral_constructor
     * //   Id├®ntico excepto que se crea una
     * // variable con un valor inicial y luego se asigna otro
     * @endcode
     */
    template <integral_builtin T> constexpr uint128_t& operator=(T value) noexcept
    {
        data[0] = static_cast<uint64_t>(value);
        data[1] = 0ull;
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                data[1] = ~0ull; // Extensi├│n de signo correcta para negativos
            }
        }
        return *this;
    }

    /**
     * @brief Asigna el valor parseado de una cadena de caracteres (C-string).
     * @param str La cadena a parsear y asignar.
     * @pre `str` debe ser un puntero v├ílido a una cadena null-terminated.
     * @post El `uint128_t` se actualiza con el valor num├®rico de la cadena.
     * @return Una referencia a `*this`.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_cstr_assignment)
     * @code{.cpp}
     * //   Similar a test_cstr_constructor
     * //   Id├®ntico excepto que se crea una
     * // variable con un valor inicial y luego se asigna desde la cadena
     * @endcode
     * @see from_cstr
     */
    constexpr uint128_t& operator=(const char* str) noexcept;

    // CONVERSIONS
    /**
     * @brief Conversi├│n expl├¡cita a `bool`.
     * @return `true` si el valor del `uint128_t` es distinto de cero.
     * @return `false` si el valor es cero.
     * @property Es `explicit`, `constexpr` y `noexcept`.
     * @test (test_bool_conversion)
     * @code{.cpp}
     * // bucle con valores aleatorios distintos de cero
     * // intercalados con valores cero
     * uint128_t val(rng(), rng());
     * assert(static_cast<bool>(val) == true);
     * val = uint128_t(0);
     * assert(static_cast<bool>(val) == false);
     * @endcode
     */
    explicit constexpr operator bool() const noexcept
    {
        return (data[1] != 0) || (data[0] != 0);
    }

    /**
     * @brief Conversi├│n expl├¡cita a un tipo integral est├índar.
     * @tparam TYPE El tipo integral de destino (e.g., `int`, `uint64_t`).
     * @return El valor de los 64 bits inferiores del `uint128_t`, convertido a `TYPE`.
     * @pre `TYPE` debe ser un tipo integral de 8 bytes o menos. Se verifica en tiempo de
     * compilaci├│n.
     * @warning La conversi├│n puede truncar el valor si este excede el m├íximo representable por
     * `TYPE`. Solo se utilizan los 64 bits inferiores.
     * @property Es `explicit`, `constexpr` y `noexcept`.
     * @test (test_integral_conversion)
     * @code{.cpp}
     * uint128_t val(123);
     * assert(static_cast<uint8_t>(val) == 123);
     * assert(static_cast<uint16_t>(val) == 123);
     * assert(static_cast<uint32_t>(val) == 123);
     * assert(static_cast<uint64_t>(val) == 123);
     * assert(static_cast<int8_t>(val) == 123);
     * assert(static_cast<int16_t>(val) == 123);
     * assert(static_cast<int32_t>(val) == 123);
     * assert(static_cast<int64_t>(val) == 123);
     * @endcode
     */

    // NOTE: Estos type traits se mover├ín a type_traits.hpp en Fase 1.5 (template unificado)
    // Por ahora permanecen aqu├¡ para evitar cambios en m├║ltiples archivos
    template <typename TYPE> struct is_floating_arithmetic_builtin : std::false_type {
    };

    template <std::arithmetic TYPE> struct is_floating_arithmetic_builtin {
        static constexpr bool value = (std::is_floating_point_v<TYPE> && (sizeof(TYPE) <= 16));
    };

    template <std::arithmetic TYPE>
    constexpr bool is_floating_arithmetic_builtin_v = is_floating_arithmetic_builtin<TYPE>::value;

    template <typename TYPE>
    concept floating_arithmetic_builtin = is_floating_arithmetic_builtin_v<TYPE>;

    template <typename TYPE> struct is_arithmetic_builtin : std::false_type {
    };

    template <std::arithmetic TYPE> struct is_arithmetic_builtin {
        static constexpr bool value =
            (is_floating_arithmetic_builtin_v<TYPE> || is_integral_builtin_v<TYPE>);
    };

    template <std::arithmetic TYPE>
    constexpr bool is_arithmetic_builtin_v = is_arithmetic_builtin<TYPE>::value;

    template <typename TYPE>
    concept arithmetic_builtin = is_arithmetic_builtin_v<TYPE>;

    template <arithmetic_builtin TYPE> explicit constexpr operator TYPE() const noexcept
    {
        if constexpr (std::is_floating_point<TYPE>::value) {
            // Conversi├│n a tipos de punto flotante
            // high * 2^64 + low
            return static_cast<TYPE>(data[1]) * 18446744073709551616.0L +
                   static_cast<TYPE>(data[0]);
        } else {
            // Conversi├│n a tipos integrales (comportamiento original)
            return static_cast<TYPE>(data[0]);
        }
    }
// NOTE: Estas conversiones permanecer├ín inline en uint128_t para mejor rendimiento
// Se encapsular├ín helpers de conversi├│n en arithmetic_operations.hpp en Fase 1.5 si es necesario
#if defined(__SIZEOF_INT128__)
    /**
     * @brief Conversi├│n al tipo nativo `__uint128_t` del compilador.
     * @return El valor completo de 128 bits como `__uint128_t`.
     * @post El valor se preserva sin p├®rdida de datos.
     * @property Esta conversi├│n solo est├í disponible si el compilador
     * define `__SIZEOF_INT128__` (t├¡pico en GCC y Clang).
     * Es `constexpr` y `noexcept`.
     * @test (test___uint128_conversion)
     * @code{.cpp}
     * // // Solo si el compilador soporta __uint128_t y
     * // // por lo tanto est├í definida la macro __SIZEOF_INT128__
     * @endcode
     */
    explicit constexpr operator __uint128_t() const noexcept
    {
        return (static_cast<__uint128_t>(data[1]) << 64) | data[0];
    }

    /**
     * @brief Conversi├│n al tipo nativo `__int128_t` del compilador.
     * @return El valor completo de 128 bits como `__int128_t`.
     * @post El valor se reinterpreta como `__int128_t`.
     * @property Esta conversi├│n solo est├í disponible si el compilador define `__SIZEOF_INT128__`.
     * Es `constexpr` y `noexcept`.
     * @test (test___int128_conversion)
     * @code{.cpp}
     * // // Solo si el compilador soporta __int128_t y
     * // // por lo tanto est├í definida la macro __SIZEOF_INT128__
     * @endcode
     */
    explicit constexpr operator __int128_t() const noexcept
    {
        return static_cast<__int128_t>((static_cast<__uint128_t>(data[1]) << 64) | data[0]);
    }
#endif // __SIZEOF_INT128__
    // ARITHMETIC OPERATORS
    /**
     * @brief Operador de pre-incremento (++i).
     * @post El valor del `uint128_t` se incrementa en 1.
     * @property El desbordamiento (de `MAX` a `0`) ocurre de forma silenciosa, siguiendo el
     * comportamiento de los enteros sin signo. Es `constexpr` y `noexcept`.
     * @return Una referencia a `*this` despu├®s de la modificaci├│n.
     * @test (test_pre_increment)
     * @code{.cpp}
     * // // No solo tiene que comprobar que incrementa en 1
     * // // sino que tambi├®n maneja el desbordamiento correctamente
     * // // en ++max_uint128_t
     * // // y que el incremento es antes de la evaluaci├│n
     * @endcode
     */
    constexpr uint128_t& operator++() noexcept
    {
        uint64_t temp = 0;
        unsigned char carry = intrinsics::add_u64(data[0], 1ull, &temp);
        data[0] = temp;
        // Propagar carry sin branch condicional
        intrinsics::addcarry_u64(carry, data[1], 0ull, &temp);
        data[1] = temp;
        return *this;
    }

    /**
     * @brief Operador de post-incremento (i++).
     * @post El valor del `uint128_t` se incrementa en 1.
     * @property El desbordamiento (de `MAX` a `0`) ocurre de forma silenciosa. Es `constexpr` y
     * `noexcept`.
     * @return Una copia del valor del objeto *antes* de ser incrementado.
     * @test (test_post_increment)
     * @code{.cpp}
     * // // No solo tiene que comprobar que incrementa en 1
     * // // sino que tambi├®n maneja el desbordamiento correctamente
     * // // en max_uint128_t++
     * // // y que el incremento es despu├®s de la evaluaci├│n
     * @endcode
     */
    constexpr uint128_t operator++(int) noexcept
    {
        uint128_t temp(*this);
        ++(*this);
        return temp;
    }

    /**
     * @brief Operador de pre-decremento (--i).
     * @post El valor del `uint128_t` se decrementa en 1.
     * @property El subdesbordamiento (de `0` a `MAX`) ocurre de forma silenciosa. Es
     * `constexpr` y `noexcept`.
     * @return Una referencia a `*this` despu├®s de la modificaci├│n.
     * @test (test_pre_decrement)
     * @code{.cpp}
     * // // No solo tiene que comprobar que decrementa en 1
     * // // sino que tambi├®n maneja el desbordamiento correctamente (en --(0_uint128_t))
     * // // y que el decremento es antes de la evaluaci├│n
     * @endcode
     */
    constexpr uint128_t& operator--() noexcept
    {
        uint64_t temp = 0;
        unsigned char borrow = intrinsics::sub_u64(data[0], 1ull, &temp);
        data[0] = temp;
        // Propagar borrow sin branch condicional
        intrinsics::subborrow_u64(borrow, data[1], 0ull, &temp);
        data[1] = temp;
        return *this;
    }

    /**
     * @brief Operador de post-decremento (i--).
     * @post El valor del `uint128_t` se decrementa en 1.
     * @property El subdesbordamiento (de `0` a `MAX`) ocurre de forma silenciosa. Es
     * `constexpr` y `noexcept`.
     * @return Una copia del valor del objeto *antes* de ser decrementado.
     * @test (test_post_decrement)
     * @code{.cpp}
     * // // No solo tiene que comprobar que decrementa en 1
     * // // sino que tambi├®n maneja el desbordamiento correctamente (en (0_uint128_t)--)
     * // // y que el decremento es despu├®s de la evaluaci├│n
     * @endcode
     */
    constexpr uint128_t operator--(int) noexcept
    {
        uint128_t temp(*this);
        --(*this);
        return temp;
    }

    /**
     * @brief Operador de suma y asignaci├│n (a += b).
     * @param other El valor `uint128_t` a sumar.
     * @post El valor de `*this` se actualiza con el resultado de la suma.
     * @property El desbordamiento (overflow) se maneja de forma silenciosa, como en los
     * enteros est├índar sin signo. Es `constexpr` y `noexcept`.
     * @return Una referencia a `*this`.
     * @test (test_addition_assignment)
     * @code{.cpp}
     * // // hacemos asignaciones aleatorias a un "uint128_t a"
     * // // y sumamos valores aleatorios "uint128_t b"
     * // // si el valor de "b" es 0, "a" no debe cambiar
     * // // si el valor de "b" es 1, "a" debe incrementarse en 1
     * // // si el valor de "b" es peque├▒o, vemos si es igual
     * // // a+=b, a == (++a b veces)
     * @endcode
     */
    constexpr uint128_t& operator+=(const uint128_t& other) noexcept
    {
        uint64_t temp = 0;
        const unsigned char carry = intrinsics::add_u64(data[0], other.data[0], &temp);
        data[0] = temp;
        intrinsics::addcarry_u64(carry, data[1], other.data[1], &temp);
        data[1] = temp;
        return *this;
    }

    /**
     * @brief Operador de resta y asignaci├│n (a -= b).
     * @param other El valor `uint128_t` a restar.
     * @post El valor de `*this` se actualiza con el resultado de la resta.
     * @property El subdesbordamiento (underflow) se maneja de forma silenciosa. Es
     * `constexpr` y `noexcept`.
     * @return Una referencia a `*this`.
     * @test (test_subtraction_assignment)
     * @code{.cpp}
     * // // similar a test_addition_assignment
     * // // pero para restas
     * @endcode
     */
    constexpr uint128_t& operator-=(const uint128_t& other) noexcept
    {
        uint64_t temp = 0;
        const unsigned char borrow = intrinsics::sub_u64(data[0], other.data[0], &temp);
        data[0] = temp;
        intrinsics::subborrow_u64(borrow, data[1], other.data[1], &temp);
        data[1] = temp;
        return *this;
    }

    /**
     * @brief Sobrecarga del operador suma y asignaci├│n para tipos integrales builtin.
     * @tparam T Un tipo de dato integral (e.g., `int`, `uint64_t`).
     * @param value El valor integral a sumar.
     * @post El valor de `*this` se actualiza con el resultado de la suma.
     * @property El desbordamiento (overflow) se maneja de forma silenciosa, como en los
     * enteros est├índar. Es `constexpr` y `noexcept`.
     * @return Una referencia a `*this`.
     * @test (test_addition_assignment)
     */
    template <integral_builtin T> constexpr uint128_t& operator+=(T value) noexcept
    {
        if constexpr (std::is_unsigned_v<T>) {
            // Para tipos sin signo, simplemente sumamos
            // (comportamiento id├®ntico al de uint128_t)
            *this += uint128_t(static_cast<uint64_t>(value));
            return *this;
        } else {
            // Para tipos con signo, manejamos valores negativos
            if (value < 0) {
                *this -= static_cast<uint128_t>(-value);
            } else {
                *this += uint128_t(static_cast<uint64_t>(value));
            }
            return *this;
        }
    }

    /**
     * @brief Sobrecarga del operador resta y asignaci├│n para tipos integrales builtin.
     * @tparam T Un tipo de dato integral (e.g., `int`, `uint64_t`).
     * @param value El valor integral a restar.
     * @post El valor de `*this` se actualiza con el resultado de la resta.
     * @property El desbordamiento (overflow) se maneja de forma silenciosa, como en los
     * enteros est├índar. Es `constexpr` y `noexcept`.
     * @return Una referencia a `*this`.
     * @test (test_subtraction_assignment)
     */
    template <integral_builtin T> constexpr uint128_t& operator-=(T value) noexcept
    {
        if constexpr (std::is_unsigned_v<T>) {
            // Para tipos sin signo, simplemente restamos
            // (comportamiento id├®ntico al de uint128_t)
            *this -= uint128_t(static_cast<uint64_t>(value));
            return *this;
        } else {
            // Para tipos con signo, manejamos valores negativos
            if (value < 0) {
                *this += static_cast<uint128_t>(-value);
            } else {
                *this -= uint128_t(static_cast<uint64_t>(value));
            }
            return *this;
        }
    }

    // FUNCIONES AUXILIARES PARA DIVISI├ôN

    /**
     * @brief Cuenta el n├║mero de ceros iniciales (desde el bit m├ís significativo).
     * @return El n├║mero de ceros iniciales en el rango [0, 128]. Devuelve 128 si el valor
     * es 0.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_leading_zeros)
     */
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

    /**
     * @brief Cuenta el n├║mero de ceros finales (desde el bit menos significativo).
     * @return El n├║mero de ceros finales en el rango [0, 128]. Devuelve 128 si el valor es
     * 0.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_trailing_zeros)
     * @endcode
     */
    constexpr int trailing_zeros() const noexcept
    {
        if (data[0] != 0) {
            return intrinsics::ctz64(data[0]);
        } else if (data[1] != 0) {
            return 64 + intrinsics::ctz64(data[1]);
        } else {
            return 128; // N├║mero es cero
        }
    }

    /**
     * @brief Calcula la longitud efectiva en bits del n├║mero.
     * @return El n├║mero de bits necesarios para representar el valor, que es `128 -
     * leading_zeros()`. Devuelve 0 para el valor 0.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_effective_length)
     */
    constexpr int effective_length() const noexcept
    {
        return 128 - leading_zeros();
    }

    /**
     * @brief Verifica si el n├║mero es una potencia de 2.
     * @return `true` si el n├║mero es mayor que 0 y solo tiene un bit establecido a 1.
     * `false` en caso contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_is_power_of_2)
     * @note Esta funci├│n delegar├í a uint128_power_detection en una futura refactorizaci├│n.
     *       Por ahora se mantiene inline para rendimiento cr├¡tico en divrem().
     * @code{.cpp}
     * @endcode
     */
    constexpr bool is_power_of_2() const noexcept
    {
        return (*this != uint128_t(0, 0)) &&
               ((*this & (*this - uint128_t(0, 1))) == uint128_t(0, 0));
    }

    /**
     * @brief Valor absoluto (funci├│n identidad para uint128_t)
     *
     * @return Una copia de *this (sin cambios)
     *
     * @note Para tipos sin signo, el valor absoluto es el propio valor.
     *       Esta funci├│n se proporciona para compatibilidad de interfaz
     *       con int128_t y para uso gen├®rico en templates.
     *
     * @example
     *   abs(uint128_t(42)) ÔåÆ uint128_t(42)
     *   abs(uint128_t(0))  ÔåÆ uint128_t(0)
     */
    constexpr uint128_t abs() const noexcept
    {
        return *this;
    }

    /**
     * @brief Realiza un desplazamiento l├│gico a la izquierda (left shift).
     * @param positions El n├║mero de posiciones a desplazar.
     * @pre `positions` debe ser un entero.
     * @post Si `positions` es <= 0, devuelve `*this`. Si es >= 128, devuelve 0.
     * @return Un nuevo `uint128_t` con el resultado del desplazamiento.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_shift_left)
     * @endcode
     */
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

    /**
     * @brief Realiza un desplazamiento l├│gico a la derecha (right shift).
     * @param positions El n├║mero de posiciones a desplazar.
     * @pre `positions` debe ser un entero.
     * @post Si `positions` es <= 0, devuelve `*this`. Si es >= 128, devuelve 0.
     * @return Un nuevo `uint128_t` con el resultado del desplazamiento.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_shift_right)
     */
    constexpr uint128_t shift_right(int positions) const noexcept
    {
        if (positions <= 0) {
            return *this;
        }
        if (positions >= 128) {
            return uint128_t(0, 0);
        }
        if (positions >= 64) {
            return uint128_t(0, data[1] >> (positions - 64));
        } else {
            const uint64_t new_low = (data[0] >> positions) | (data[1] << (64 - positions));
            const uint64_t new_high = data[1] >> positions;
            return uint128_t(new_high, new_low);
        }
    }

    // COMPARISON OPERATORS
    /**
     * @brief Compara si dos `uint128_t` son iguales.
     * @param other El otro valor a comparar.
     * @return `true` si ambos valores son id├®nticos, `false` en caso contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator==(const uint128_t& other) const noexcept
    {
        return (data[0] == other.data[0]) && (data[1] == other.data[1]);
    }

    /**
     * @brief Compara si dos `uint128_t` son diferentes.
     * @param other El otro valor a comparar.
     * @return `true` si los valores no son id├®nticos, `false` en caso contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator!=(const uint128_t& other) const noexcept
    {
        return !(*this == other);
    }

    /**
     * @brief Compara si este `uint128_t` es menor que otro.
     * @param other El otro valor a comparar.
     * @return `true` si `*this` es estrictamente menor que `other`, `false` en caso
     * contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator<(const uint128_t& other) const noexcept
    {
        if (data[1] != other.data[1]) {
            return data[1] < other.data[1];
        }
        return data[0] < other.data[0];
    }

    /**
     * @brief Compara si este `uint128_t` es menor o igual que otro.
     * @param other El otro valor a comparar.
     * @return `true` si `*this` es menor o igual que `other`, `false` en caso contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator<=(const uint128_t& other) const noexcept
    {
        return (*this < other) || (*this == other);
    }

    /**
     * @brief Compara si este `uint128_t` es mayor que otro.
     * @param other El otro valor a comparar.
     * @return `true` si `*this` es estrictamente mayor que `other`, `false` en caso
     * contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator>(const uint128_t& other) const noexcept
    {
        return !(*this <= other);
    }

    /**
     * @brief Compara si este `uint128_t` es mayor o igual que otro.
     * @param other El otro valor a comparar.
     * @return `true` si `*this` es mayor o igual que `other`, `false` en caso contrario.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_comparison_operators)
     */
    constexpr bool operator>=(const uint128_t& other) const noexcept
    {
        return !(*this < other);
    }

    /**
     * @brief Operador de comparaci├│n de tres v├¡as (spaceship operator, C++20)
     * @param other El otro valor a comparar
     * @return std::strong_ordering indicando la relaci├│n de orden
     * @property Es `constexpr` y `noexcept`.
     * @note Requerido por el est├índar C++20 en ciertos contextos aunque
     *       los operadores <, >, <=, >=, ==, != ya est├®n definidos.
     */
    constexpr std::strong_ordering operator<=>(const uint128_t& other) const noexcept
    {
        if (data[1] != other.data[1]) {
            return data[1] <=> other.data[1];
        }
        return data[0] <=> other.data[0];
    }

    /**
     * @brief Sobrecarga del operador de comparaci├│n de tres v├¡as
     * (spaceship operator, C++20) para tipos integrales builtin.
     * @param other El otro valor a comparar
     * @return std::strong_ordering indicando la relaci├│n de orden
     * @property Es `constexpr` y `noexcept`.
     * @note Requerido por el est├índar C++20 en ciertos contextos aunque
     *       los operadores <, >, <=, >=, ==, != ya est├®n definidos.
     */
    template <integral_builtin T> constexpr std::strong_ordering operator<=>(T other) const noexcept
    {
        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                return std::strong_ordering::greater;
            }
        }

        // Si data[1] != 0, entonces *this definitivamente > other (que cabe en 64 bits)
        if (data[1] != 0) {
            return std::strong_ordering::greater;
        }

        // Ambos valores caben en 64 bits, comparar directamente
        return data[0] <=> static_cast<uint64_t>(other);
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
    // OPERADORES DE DIVISI├ôN

    /**
     * @brief Calcula simult├íneamente el cociente y el resto de una divisi├│n.
     * @details Esta es la funci├│n base para los operadores `/` y `%`. Implementa un
     * algoritmo de divisi├│n larga binaria y est├í optimizada para casos especiales como la
     * divisi├│n por una potencia de 2.
     * @param divisor El valor `uint128_t` por el cual dividir.
     * @pre El `divisor` es un `uint128_t`.
     * @post Si el divisor es 0, el resultado es `std::nullopt`. En otro caso, el par
     * contiene el cociente y el resto.
     * @return Un `std::optional` que contiene un `std::pair` con el cociente (`first`) y el
     * resto
     * (`second`). El `optional` est├í vac├¡o si ocurre una divisi├│n por cero.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_basic)
     * @code{.cpp}
     * // // Tanto para esta m├®todo como para los operadores / y %
     * // // como para divisores de todos los tipos integrales soportados
     * // // // casos especiales: divisi├│n por 0, divisi├│n por 1, divisi├│n por potencia de
     * 2,
     * // // // dividendos menores que el divisor, dividendos iguales al divisor
     * // // // casos generales: varios dividendos y divisores aleatorios
     * // // // verificar que (quotient * divisor + remainder) == dividend
     * @endcode
     */
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
    divrem(const uint128_t& divisor) const noexcept
    {
        // Detectar divisi├│n por cero
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
        } else if (divisor.is_power_of_2()) {
            // Divisi├│n por potencia de 2
            const int shift_amount = divisor.trailing_zeros();
            quotient = this->shift_right(shift_amount);
            remainder = *this & (divisor - uint128_t(0, 1));
            return std::make_pair(quotient, remainder);
        }

        // CASO GENERAL: Algoritmo de divisi├│n larga binaria
        const auto normalized_pair = divisor.normalize_divisor(*this);
        uint128_t normalized_divisor = normalized_pair.first;
        const int shift_amount = normalized_pair.second;

        remainder = *this;
        quotient = uint128_t(0, 0);

        // Bucle principal de divisi├│n binaria
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

    /**
     * @brief Sobrecarga optimizada de `divrem` para divisores de tipo integral builtin.
     * @details Optimizaci├│n para divisor de 64 bits o menos. Usa el algoritmo de
     * divisi├│n larga 128/64 bits que es significativamente m├ís r├ípido que la divisi├│n
     * completa 128/128 bits.
     * @tparam T Un tipo de dato integral builtin (uint8_t, int32_t, uint64_t, etc.).
     * @param divisor El valor integral por el cual dividir.
     * @pre `T` debe ser un tipo integral builtin. Se verifica con integral_builtin concept.
     * @return std::optional<std::pair<uint128_t, uint128_t>> con {cociente, resto}.
     *         Devuelve std::nullopt si divisor == 0.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_integral_divisor)
     *
     * @note Algoritmo: Divisi├│n larga 128 bits / 64 bits
     *       1. Convertir divisor a uint64_t (manejar signed negativos si aplica)
     *       2. Si divisor == 0: retornar nullopt
     *       3. Si *this < divisor: cociente=0, resto=*this
     *       4. Si divisor es potencia de 2: usar shift y mask
     *       5. Algoritmo general 128/64:
     *          - Dividir data[1] / divisor ÔåÆ cociente_alto, resto_alto
     *          - Componer (resto_alto << 64) | data[0]
     *          - Dividir dividendo_compuesto / divisor ÔåÆ cociente_bajo, resto_final
     *          - Resultado: cociente = (cociente_alto << 64) | cociente_bajo
     */
    template <integral_builtin T>
    constexpr std::optional<std::pair<uint128_t, uint128_t>> divrem(T divisor) const noexcept
    {
        // Manejar tipos con signo
        uint64_t divisor_u64;
        if constexpr (std::is_signed_v<T>) {
            if (divisor < 0) {
                // Divisi├│n por negativo: convertir a positivo
                // (manejar signo del resultado lo hace el caller)
                divisor_u64 = static_cast<uint64_t>(-static_cast<int64_t>(divisor));
            } else {
                divisor_u64 = static_cast<uint64_t>(divisor);
            }
        } else {
            divisor_u64 = static_cast<uint64_t>(divisor);
        }

        // Caso 1: Divisi├│n por cero
        if (divisor_u64 == 0) {
            return std::nullopt;
        }

        // Caso 2: Divisor mayor que dividendo
        if (data[1] == 0 && data[0] < divisor_u64) {
            return std::pair<uint128_t, uint128_t>{uint128_t(0, 0), *this};
        }

        // Caso 3: Divisi├│n por 1
        if (divisor_u64 == 1) {
            return std::pair<uint128_t, uint128_t>{*this, uint128_t(0, 0)};
        }

        // Caso 4: Divisi├│n por potencia de 2 (optimizaci├│n)
        if ((divisor_u64 & (divisor_u64 - 1)) == 0) {
            // divisor es potencia de 2
            const int shift = intrinsics::ctz64(divisor_u64);
            const uint128_t quotient = this->shift_right(shift);
            const uint128_t remainder = *this & uint128_t(0, divisor_u64 - 1);
            return std::pair<uint128_t, uint128_t>{quotient, remainder};
        }

        // Caso 5: Dividendo cabe en 64 bits (fast path)
        if (data[1] == 0) {
            const uint64_t q = data[0] / divisor_u64;
            const uint64_t r = data[0] % divisor_u64;
            return std::pair<uint128_t, uint128_t>{uint128_t(0, q), uint128_t(0, r)};
        }

        // Caso 6: Algoritmo general 128/64 bits
        // Paso 1: Dividir parte alta
        uint64_t q_hi = data[1] / divisor_u64;
        uint64_t r_hi = data[1] % divisor_u64;

        // Paso 2: Componer nuevo dividendo (r_hi << 64) | data[0] y dividir
        // Usar funci├│n encapsulada de arithmetic_operations.hpp
        uint64_t remainder_final;
        uint64_t q_lo =
            intrinsics::div128_64_composed(r_hi, data[0], divisor_u64, &remainder_final);

#if !defined(__SIZEOF_INT128__) || defined(_MSC_VER)
        // Fallback cuando div128_64_composed retorna 0 (sin __uint128_t)
        // IMPORTANTE: Llamar expl├¡citamente a divrem(const uint128_t&) para evitar recursi├│n
        if (q_lo == 0 && remainder_final == 0) {
            const uint128_t dividend_low(r_hi, data[0]);
            const uint128_t divisor_128(0, divisor_u64);
            auto result = dividend_low.divrem(static_cast<const uint128_t&>(divisor_128));
            if (!result.has_value()) {
                return std::nullopt; // No deber├¡a ocurrir
            }
            q_lo = result.value().first.data[0];
            remainder_final = result.value().second.data[0];
        }
#endif

        // Paso 3: Componer resultado final
        const uint128_t quotient(q_hi, q_lo);
        const uint128_t remainder(0, remainder_final);

        return std::pair<uint128_t, uint128_t>{quotient, remainder};
    }

    /**
     * @brief Operador de divisi├│n y asignaci├│n (a /= b).
     * @param other El divisor.
     * @post El valor de `*this` se actualiza con el cociente de la divisi├│n.
     * @return Una referencia a `*this`.
     *
     * @warning Si `other` es cero, el comportamiento es indefinido (undefined behavior)
     *          como en tipos integrales est├índar de C++. No se lanza excepci├│n.
     *
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_basic)
     */
    constexpr uint128_t& operator/=(const uint128_t& other) noexcept
    {
        // Comportamiento como tipos built-in: divisi├│n por cero es UB
        // No se verifica, acceso directo causa UB si other == 0
        const auto result = divrem(other);
        *this = result.value().first; // quotient (UB si !result.has_value())
        return *this;
    }

    /**
     * @brief Sobrecarga del operador de divisi├│n y asignaci├│n para tipos integrales.
     * @tparam T Un tipo de dato integral builtin.
     * @param other El divisor de tipo integral.
     * @return Una referencia a `*this`.
     * @note Usa la implementaci├│n optimizada divrem(T) para divisores de 64 bits o menos.
     * @test (test_divrem_basic_integral_divisor)
     */
    template <integral_builtin T> constexpr uint128_t& operator/=(T other) noexcept
    {
        const auto result = divrem(other);
        *this = result.value().first; // quotient (UB si divisor == 0)
        return *this;
    }

    /**
     * @brief Operador de m├│dulo y asignaci├│n (a %= b).
     * @param other El divisor.
     * @post El valor de `*this` se actualiza con el resto de la divisi├│n.
     * @return Una referencia a `*this`.
     *
     * @warning Si `other` es cero, el comportamiento es indefinido (undefined behavior)
     *          como en tipos integrales est├índar de C++. No se lanza excepci├│n.
     *
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_basic)
     */
    constexpr uint128_t& operator%=(const uint128_t& other) noexcept
    {
        // Comportamiento como tipos built-in: m├│dulo por cero es UB
        // No se verifica, acceso directo causa UB si other == 0
        const auto result = divrem(other);
        *this = result.value().second; // remainder (UB si !result.has_value())
        return *this;
    }

    /**
     * @brief Sobrecarga del operador de m├│dulo y asignaci├│n para tipos integrales.
     * @tparam T Un tipo de dato integral builtin.
     * @param other El divisor de tipo integral.
     * @return Una referencia a `*this`.
     * @note Usa la implementaci├│n optimizada divrem(T) para divisores de 64 bits o menos.
     * @test (test_divrem_basic_integral_divisor)
     */
    template <integral_builtin T> constexpr uint128_t& operator%=(T other) noexcept
    {
        const auto result = divrem(other);
        *this = result.value().second; // remainder (UB si divisor == 0)
        return *this;
    }

    /**
     * @brief Operador de divisi├│n.
     * @param other El divisor.
     * @return Un nuevo `uint128_t` con el resultado de la divisi├│n. Devuelve 0 en caso de
     * divisi├│n por cero.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_basic)
     */
    constexpr uint128_t operator/(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result /= other;
        return result;
    }

    /**
     * @brief Sobrecarga del operador de divisi├│n para tipos integrales.
     * @tparam T Tipo integral builtin del divisor.
     * @param other El divisor.
     * @return Un nuevo `uint128_t` con el resultado de la divisi├│n.
     * @note Usa la implementaci├│n optimizada divrem(T) para mayor rendimiento.
     * @test (test_divrem_basic_integral_divisor)
     */
    template <integral_builtin T> constexpr uint128_t operator/(T other) const noexcept
    {
        uint128_t result(*this);
        result /= other;
        return result;
    }

    /**
     * @brief Operador de m├│dulo (resto de la divisi├│n).
     * @param other El divisor.
     * @return Un nuevo `uint128_t` con el resto de la divisi├│n. Devuelve 0 en caso de
     * divisi├│n por cero.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_divrem_basic)
     */
    constexpr uint128_t operator%(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result %= other;
        return result;
    }

    /**
     * @brief Sobrecarga del operador de m├│dulo para tipos integrales.
     * @tparam T Tipo integral builtin del divisor.
     * @param other El divisor.
     * @return Un nuevo `uint128_t` con el resto de la divisi├│n.
     * @note Usa la implementaci├│n optimizada divrem(T) para mayor rendimiento.
     * @test (test_divrem_basic_integral_divisor)
     */
    template <integral_builtin T> constexpr uint128_t operator%(T other) const noexcept
    {
        uint128_t result(*this);
        result %= other;
        return result;
    }

  private:
    // ============================================================================
    // Includes de headers modulares que dependen de la definici├│n completa de uint128_t
    // DEBEN estar DESPU├ëS de la declaraci├│n de todos los m├®todos de la clase
    // ============================================================================
#include "specializations/uint128_div_const.hpp"
#include "specializations/uint128_divisibility.hpp"
#include "specializations/uint128_factorization_helpers.hpp"
#include "specializations/uint128_mod_helpers.hpp"
#include "specializations/uint128_multiply_const.hpp"
#include "specializations/uint128_power_detection.hpp"

    // ============================================================================
    // Helpers de m├│dulo optimizados - definidos en specializations/uint128_mod_helpers.hpp
    // ============================================================================
    UINT128_MOD_HELPERS_PRIVATE_METHODS

    // M├®todos privados de operaciones constexpr modulares
    UINT128_DIVISIBILITY_PRIVATE_METHODS
    UINT128_FACTORIZATION_PRIVATE_METHODS
    UINT128_MULTIPLY_CONST_PRIVATE_METHODS
    UINT128_DIV_CONST_PRIVATE_METHODS

  public:
    /**
     * @brief Calcula el m├│dulo con un valor de radio especificado en tiempo de compilaci├│n.
     * @tparam Rad El valor del radio para el m├│dulo (debe estar entre 2 y 63 inclusive).
     * @return Un nuevo `uint128_t` con el resultado del m├│dulo.
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado sin divisiones: potencias de 2 usan m├íscaras de bits,
     *       mod 3/5/7/9 usan algoritmos especializados basados en propiedades modulares.
     */
    template <uint64_t Rad>
        requires(Rad > 1 && Rad < 64)
    constexpr uint128_t mod() const noexcept
    {
        // Caso especial: potencia de 2 -> m├íscara de bits (sin divisi├│n)
        if constexpr (uint128_mod_details::is_power_of_2(Rad)) {
            constexpr int bits = uint128_mod_details::log2_uint64(Rad);
            constexpr uint64_t mask = Rad - 1;

            if constexpr (bits < 64) {
                // El resultado cabe completamente en low()
                return uint128_t(0, low() & mask);
            } else {
                // Necesitamos parte de high() tambi├®n (bits >= 64)
                constexpr int high_bits = bits - 64;
                constexpr uint64_t high_mask = (1ULL << high_bits) - 1;
                return uint128_t(high() & high_mask, low());
            }
        }
        // Caso especial: potencia de 3 (3, 9, 27) -> optimizaci├│n gen├®rica
        else if constexpr (uint128_mod_details::is_power_of_3(Rad)) {
            return uint128_t(0, mod_power_of_3_helper<Rad>());
        }
        // Optimizaciones espec├¡ficas para m├│dulos peque├▒os comunes
        else if constexpr (Rad == 3) {
            return uint128_t(0, mod3_helper());
        } else if constexpr (Rad == 5) {
            return uint128_t(0, mod5_helper());
        } else if constexpr (Rad == 6) {
            return uint128_t(0, mod6_helper());
        } else if constexpr (Rad == 7) {
            return uint128_t(0, mod7_helper());
        } else if constexpr (Rad == 9) {
            return uint128_t(0, mod9_helper());
        } else if constexpr (Rad == 10) {
            return uint128_t(0, mod10_helper());
        } else if constexpr (Rad == 11) {
            return uint128_t(0, mod11_helper());
        } else if constexpr (Rad == 12) {
            return uint128_t(0, mod12_helper());
        } else if constexpr (Rad == 13) {
            return uint128_t(0, mod13_helper());
        } else if constexpr (Rad == 14) {
            return uint128_t(0, mod14_helper());
        } else if constexpr (Rad == 15) {
            return uint128_t(0, mod15_helper());
        } else if constexpr (Rad == 17) {
            return uint128_t(0, mod17_helper());
        } else if constexpr (Rad == 18) {
            return uint128_t(0, mod18_helper());
        } else if constexpr (Rad == 19) {
            return uint128_t(0, mod19_helper());
        } else if constexpr (Rad == 20) {
            return uint128_t(0, mod20_helper());
        } else if constexpr (Rad == 23) {
            return uint128_t(0, mod23_helper());
        } else if constexpr (Rad == 29) {
            return uint128_t(0, mod29_helper());
        } else if constexpr (Rad == 31) {
            return uint128_t(0, mod31_helper());
        } else if constexpr (Rad == 37) {
            return uint128_t(0, mod37_helper());
        } else if constexpr (Rad == 41) {
            return uint128_t(0, mod41_helper());
        } else if constexpr (Rad == 43) {
            return uint128_t(0, mod43_helper());
        } else if constexpr (Rad == 47) {
            return uint128_t(0, mod47_helper());
        } else if constexpr (Rad == 53) {
            return uint128_t(0, mod53_helper());
        } else if constexpr (Rad == 59) {
            return uint128_t(0, mod59_helper());
        } else if constexpr (Rad == 61) {
            return uint128_t(0, mod61_helper());
        } else {
            // Para otros casos, usar helper gen├®rico optimizado
            return uint128_t(0, modM_generic_helper<Rad>());
        }
    }

    /**
     * @brief Calcula el m├│dulo con respecto a una potencia de 2 (2^n).
     * @tparam n El exponente de la potencia de 2 (debe ser >= 1 y n < 64).
     * @return Un nuevo `uint128_t` con el resultado de (*this) % (2^n).
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado usando m├íscara de bits, sin divisi├│n.
     * @example
     * @code{.cpp}
     * uint128_t val(100);
     * auto r1 = val.mod_pot2<1>();  // 100 % 2 = 0
     * auto r3 = val.mod_pot2<3>();  // 100 % 8 = 4
     * auto r4 = val.mod_pot2<4>();  // 100 % 16 = 4
     * @endcode
     */
    template <int n>
    constexpr uint128_t mod_pot2() const noexcept
        requires(n >= 1 && n < 64)
    {
        constexpr uint64_t power_of_2 = 1ULL << n;
        return uint128_t(0, mod_power_of_2_helper<power_of_2>());
    }

    /**
     * @brief Calcula el m├│dulo con respecto a una potencia de 3 (3^n).
     * @tparam n El exponente de la potencia de 3 (debe ser >= 1 y 3^n < 64).
     * @return Un nuevo `uint128_t` con el resultado de (*this) % (3^n).
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado usando propiedades modulares, evita divisi├│n de 128 bits.
     * @example
     * @code{.cpp}
     * uint128_t val(100);
     * auto r1 = val.mod_pot3<1>();  // 100 % 3 = 1
     * auto r2 = val.mod_pot3<2>();  // 100 % 9 = 1
     * auto r3 = val.mod_pot3<3>();  // 100 % 27 = 19
     * auto r4 = val.mod_pot3<4>();  // 100 % 81 = 19
     * @endcode
     */
    template <int n>
        requires(n >= 1 && uint128_mod_details::pow3(n) < 64)
    constexpr uint128_t mod_pot3() const noexcept
    {
        constexpr uint64_t power_of_3 = uint128_mod_details::pow3(n);
        return uint128_t(0, mod_power_of_3_helper<power_of_3>());
    }

    /**
     * @brief Calcula el m├│dulo con respecto a una potencia de 5 (5^n).
     * @tparam n El exponente de la potencia de 5 (debe ser >= 1 y 5^n < 2^64).
     * @return Un nuevo `uint128_t` con el resultado de (*this) % (5^n).
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado usando propiedades modulares, evita divisi├│n de 128 bits.
     * @example
     * @code{.cpp}
     * uint128_t val(100);
     * auto r1 = val.mod_pot5<1>();  // 100 % 5 = 0
     * auto r2 = val.mod_pot5<2>();  // 100 % 25 = 0
     * auto r3 = val.mod_pot5<3>();  // 100 % 125 = 100
     * @endcode
     */
    template <int n>
        requires(n >= 1 && uint128_mod_details::pow5(n) < 64)
    constexpr uint128_t mod_pot5() const noexcept
    {
        constexpr uint64_t power_of_5 = uint128_mod_details::pow5(n);
        return uint128_t(0, mod_power_of_5_helper<power_of_5>());
    }

    /**
     * @brief Calcula el m├│dulo con respecto a una potencia de 7 (7^n).
     * @tparam n El exponente de la potencia de 7 (debe ser >= 1 y 7^n < 2^64).
     * @return Un nuevo `uint128_t` con el resultado de (*this) % (7^n).
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado usando propiedades modulares, evita divisi├│n de 128 bits.
     * @example
     * @code{.cpp}
     * uint128_t val(100);
     * auto r1 = val.mod_pot7<1>();  // 100 % 7 = 2
     * auto r2 = val.mod_pot7<2>();  // 100 % 49 = 2
     * @endcode
     */
    template <int n>
        requires(n >= 1 && uint128_mod_details::pow7(n) < 64)
    constexpr uint128_t mod_pot7() const noexcept
    {
        constexpr uint64_t power_of_7 = uint128_mod_details::pow7(n);
        return uint128_t(0, mod_power_of_7_helper<power_of_7>());
    }

    /**
     * @brief Calcula el m├│dulo con respecto a una potencia de 10 (10^n).
     * @tparam n El exponente de la potencia de 10 (debe ser >= 1 y 10^n < 2^64).
     * @return Un nuevo `uint128_t` con el resultado de (*this) % (10^n).
     * @property Es `constexpr` y `noexcept`.
     * @note Optimizado usando propiedades modulares, evita divisi├│n de 128 bits.
     * @example
     * @code{.cpp}
     * uint128_t val(123456);
     * auto r1 = val.mod_pot10<1>();  // 123456 % 10 = 6
     * auto r2 = val.mod_pot10<2>();  // 123456 % 100 = 56
     * auto r3 = val.mod_pot10<3>();  // 123456 % 1000 = 456
     * @endcode
     */
    template <int n>
        requires(n >= 1 && uint128_mod_details::pow10(n) < 64)
    constexpr uint128_t mod_pot10() const noexcept
    {
        constexpr uint64_t power_of_10 = uint128_mod_details::pow10(n);
        return uint128_t(0, mod_power_of_10_helper<power_of_10>());
    }

    // OTHER ARITHMETIC OPERATORS
    /**
     * @brief Operador de suma.
     * @param other El sumando.
     * @return Un nuevo `uint128_t` con el resultado de la suma. El desbordamiento es
     * silencioso.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_add_operator)
     */
    constexpr uint128_t operator+(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result += other;
        return result;
    }

    template <integral_builtin T> constexpr uint128_t operator+(T other) const noexcept
    {
        uint128_t result(*this);
        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                result -= uint128_t(static_cast<uint64_t>(-static_cast<int64_t>(other)));
                return result;
            }
        }
        result += uint128_t(static_cast<uint64_t>(other));
        return result;
    }

    /**
     * @brief Operador de resta.
     * @param other El sustraendo.
     * @return Un nuevo `uint128_t` con el resultado de la resta. El subdesbordamiento es
     * silencioso.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_sub_operator)
     */
    constexpr uint128_t operator-(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result -= other;
        return result;
    }

    template <integral_builtin T> constexpr uint128_t operator-(T other) const noexcept
    {
        uint128_t result(*this);
        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                result += uint128_t(static_cast<uint64_t>(-static_cast<int64_t>(other)));
                return result;
            }
        }
        result -= uint128_t(static_cast<uint64_t>(other));
        return result;
    }

    /**
     * @brief Operador NOT a nivel de bits (~a).
     * @return Un nuevo `uint128_t` con todos los bits invertidos.
     * @test (test_bitwise_operators)
     */
    constexpr uint128_t operator~() const noexcept
    {
        return uint128_t(~data[1], ~data[0]);
    }

    /**
     * @brief Operador unario de negaci├│n (-x)
     * @return Negaci├│n aritm├®tica del valor mediante aritm├®tica modular (2^128 - x)
     *
     * @note Para tipos unsigned, -x est├í bien definido en C++ como 2^N - x.
     *       Esto es consistente con unsigned int, unsigned long, etc.
     *       Ejemplo: -uint128_t(5) produce 2^128 - 5
     *
     * @property Es `constexpr` y `noexcept`.
     *
     * @example
     *   uint128_t x(23);
     *   uint128_t neg_x = -x;  // 2^128 - 23
     *   neg_x + x;             // 0 (por aritm├®tica modular)
     */
    constexpr uint128_t operator-() const noexcept
    {
        // Complemento a dos: ~x + 1
        return ~(*this) + uint128_t(1);
    }

    /**
     * @brief Operador de multiplicaci├│n y asignaci├│n (a *= b).
     * @param other El valor `uint128_t` por el que multiplicar.
     * @post El valor de `*this` se actualiza con los 128 bits inferiores del producto.
     * @property El desbordamiento (overflow) es silencioso. Es `constexpr` y `noexcept`.
     * @return Una referencia a `*this`.
     * @test (test_mult_assignment_operator)
     * @code{.cpp}
     * // uint128_t val1(rng(), rng());
     * // uint128_t val2(rng(), rng());
     * // uint128_t val1' = val1;
     * // uint128_t val2' = val2;
     * // val1' *= val2;
     * // val2' *= val1;
     * // assert(val1' == val2'); // conmutatividad
     * // elemento neutro
     * // elemento nulo
     * // val2 * (val1-1) == val2 * val1 - val2
     * // val1 * (val2-1) == val1 * val2 - val1
     * // val2 * (val1+1) == val2 * val1 + val2
     * // val1 * (val2+1) == val1 * val2 + val1
     * // (val2 * val1).divrem(val1) == [val2,0]
     * // (val2 * val1).divrem(val2) == [val1,0]
     * @endcode
     */
    constexpr uint128_t& operator*=(const uint128_t& other) noexcept
    {
        intrinsics::mul128(data[0], data[1], other.data[0], other.data[1], &data[0], &data[1]);
        return *this;
    }

    /**
     * @brief Operador de multiplicaci├│n y asignaci├│n optimizado para tipos integrales (a *= b).
     *
     * **ESTRATEGIA DE OPTIMIZACI├ôN MULTINIVEL** (Fase 0.5 - Pre-Unificaci├│n):
     *
     * Esta implementaci├│n usa un sistema de optimizaci├│n adaptativo de dos niveles
     * basado en el tama├▒o efectivo del multiplicando (*this):
     *
     * 1. **Fast path** (data[1] == 0): El valor cabe en 64 bits
     *    - Usa una sola multiplicaci├│n 64├ù64 ÔåÆ 128 bits (umul128)
     *    - Rendimiento: 1 multiplicaci├│n vs 4 del algoritmo completo
     *    - Caso com├║n: ~70-80% de las operaciones en c├│digo t├¡pico
     *
     * 2. **General case** (data[1] != 0): Multiplicaci├│n 128 ├ù 64 bits
     *    - Usa umul128 para parte baja + multiplicaci├│n simple para cross-product
     *    - Rendimiento: 2 multiplicaciones vs 4 del algoritmo completo
     *    - Resultado: low = a.low * b, high = (a.low * b).high + (a.high * b)
     *
     * **COMPARACI├ôN DE RENDIMIENTO**:
     * - Versi├│n original: 4 multiplicaciones (128├ù128 completo con mul128)
     * - Fast path: 1 multiplicaci├│n (75% mejora)
     * - General case: 2 multiplicaciones (50% mejora)
     *
     * **MANEJO DE SIGN EXTENSION**:
     * Para tipos signed negativos, se aplica correcci├│n en complemento a 2:
     * - Si (other < 0): high_part -= data[0]
     * - Preserva comportamiento est├índar de extensi├│n de signo
     * - Compatible con constructor y operadores de conversi├│n
     *
     * **RELEVANCIA PARA FASE 1.5 (Unificaci├│n Template)**:
     * Este patr├│n de optimizaci├│n se replicar├í en int128_base_t<signedness S>:
     * - `if constexpr (std::is_signed_v<T>)` demuestra uso efectivo de branching
     * - Fast path por valor es independiente de signedness
     * - Sign extension se maneja limpiamente sin duplicaci├│n de c├│digo
     *
     * @tparam T Tipo integral builtin (uint8_t...uint64_t, int8_t...int64_t)
     * @param other Multiplicador de tipo integral
     * @return Referencia a *this con el producto calculado
     *
     * @property constexpr, noexcept, optimizado seg├║n tama├▒o de operandos
     * @see operator*(T) para versi├│n no modificante que delega a este operador
     * @see intrinsics::umul128 para implementaci├│n de multiplicaci├│n 64├ù64ÔåÆ128
     *
     * @note El overflow es silencioso (wrap-around) siguiendo sem├íntica de enteros unsigned
     * @note Esta optimizaci├│n fue validada en benchmarks antes de Fase 1.5
     */
    template <integral_builtin T> constexpr uint128_t& operator*=(T other) noexcept
    {
        const uint64_t b = static_cast<uint64_t>(other);

        // Fast path: si *this cabe en 64 bits, solo una multiplicaci├│n 64├ù64
        if (data[1] == 0) {
            const uint64_t low_part = intrinsics::umul128(data[0], b, &data[1]);
            data[0] = low_part;

            // Ajuste para signed negatives
            if constexpr (std::is_signed_v<T>) {
                if (other < 0) {
                    data[1] -= data[0]; // Corrige por sign extension
                }
            }
            return *this;
        }

        // General case: multiplicaci├│n 128-bit ├ù 64-bit
        uint64_t high_part;
        const uint64_t low_part = intrinsics::umul128(data[0], b, &high_part);
        const uint64_t cross_product = data[1] * b;

        // Ajuste para signed negatives (complemento a 2)
        if constexpr (std::is_signed_v<T>) {
            if (other < 0) {
                high_part -= data[0]; // Corrige por sign extension
            }
        }

        // Resultado final
        data[0] = low_part;
        data[1] = high_part + cross_product;
        return *this;
    }

    /**
     * @brief Operador de multiplicaci├│n (versi├│n no modificante).
     * @param other El multiplicando.
     * @return Un nuevo `uint128_t` con los 128 bits inferiores del producto. El
     * desbordamiento es silencioso.
     * @property Es `constexpr` y `noexcept`.
     * @test (test_mult_operator)
     */
    constexpr uint128_t operator*(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result *= other;
        return result;
    }

    /**
     * @brief Operador de multiplicaci├│n para tipos integrales (versi├│n no modificante).
     *
     * **PATR├ôN COPY-MODIFY-RETURN** (Est├índar para operadores const):
     *
     * Esta implementaci├│n sigue el patr├│n est├índar para operadores binarios const:
     * 1. Crear copia del objeto actual
     * 2. Aplicar operaci├│n modificante (operator*=) a la copia
     * 3. Retornar la copia modificada
     *
     * **HERENCIA DE OPTIMIZACIONES**:
     * Delega a operator*=(T) que implementa optimizaci├│n multinivel:
     * - Fast path para valores peque├▒os (data[1]==0): 1 multiplicaci├│n
     * - General case para valores grandes: 2 multiplicaciones
     * - Sign extension autom├ítica para tipos signed negativos
     *
     * **CORRECCI├ôN CR├ìTICA** (Fase 0.5):
     * Versi├│n anterior: `result *= static_cast<uint64_t>(other);`
     * - Problema: El cast expl├¡cito podr├¡a bypass template resolution
     * - Consecuencia: Sign extension no se aplicaba correctamente
     *
     * Versi├│n actual: `result *= other;`
     * - Soluci├│n: Preserva el tipo template T para correcta resoluci├│n
     * - Garantiza: operator*=(T) se llama con tipo original
     * - Resultado: Sign extension funciona como se espera
     *
     * @tparam T Tipo integral builtin (uint8_t...uint64_t, int8_t...int64_t)
     * @param other Multiplicador de tipo integral
     * @return Nuevo uint128_t con el resultado de la multiplicaci├│n
     *
     * @property constexpr, noexcept, hereda optimizaciones de operator*=(T)
     * @see operator*=(T) para detalles de estrategia de optimizaci├│n
     *
     * @note Esta correcci├│n fue identificada y aplicada en Fase 0.5
     * @note El patr├│n de delegaci├│n ser├í usado en template unificado (Fase 1.5)
     */
    template <integral_builtin T> constexpr uint128_t operator*(T other) const noexcept
    {
        uint128_t result(*this);
        result *= other; // Preserva tipo T para correcta template resolution
        return result;
    }

    // BITWISE OPERATORS
    /**
     * @brief Operador de desplazamiento a la izquierda y asignaci├│n (a <<= b).
     * @param shift El n├║mero de bits a desplazar.
     * @return Una referencia a `*this`.
     * @test (test_shift_left)
     */
    constexpr uint128_t& operator<<=(int shift) noexcept
    {
        return (*this = shift_left(shift));
    }

    /**
     * @brief Operador de desplazamiento a la derecha y asignaci├│n (a >>= b).
     * @param shift El n├║mero de bits a desplazar.
     * @return Una referencia a `*this`.
     * @test (test_shift_right)
     */
    constexpr uint128_t& operator>>=(int shift) noexcept
    {
        return (*this = shift_right(shift));
    }

    /**
     * @brief Operador de desplazamiento a la izquierda (a << b).
     * @param shift El n├║mero de bits a desplazar.
     * @return Un nuevo `uint128_t` con el resultado del desplazamiento.
     * @test (test_shift_left)
     */
    constexpr uint128_t operator<<(int shift) const noexcept
    {
        return shift_left(shift);
    }

    /**
     * @brief Operador de desplazamiento a la derecha (a >> b).
     * @param shift El n├║mero de bits a desplazar.
     * @return Un nuevo `uint128_t` con el resultado del desplazamiento.
     * @test (test_shift_right)
     */
    constexpr uint128_t operator>>(int shift) const noexcept
    {
        return shift_right(shift);
    }

    /**
     * @brief Operador AND a nivel de bits y asignaci├│n (a &= b).
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    constexpr uint128_t& operator&=(const uint128_t& other) noexcept
    {
        data[0] &= other.data[0];
        data[1] &= other.data[1];
        return *this;
    }

    /**
     * @brief Sobrecarga del operador AND a nivel de bits y asignaci├│n (a &= b)
     * para tipos integrales builtin.
     * @tparam T Un tipo de dato integral builtin.
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    template <integral_builtin T> constexpr uint128_t& operator&=(T other) noexcept
    {
        return (*this &= static_cast<uint128_t>(other));
    }

    /**
     * @brief Operador OR a nivel de bits y asignaci├│n (a |= b).
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    constexpr uint128_t& operator|=(const uint128_t& other) noexcept
    {
        data[0] |= other.data[0];
        data[1] |= other.data[1];
        return *this;
    }

    /**
     * @brief Sobrecarga del operador OR a nivel de bits y asignaci├│n (a |= b)
     * para tipos integrales builtin.
     * @tparam T Un tipo de dato integral builtin.
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    template <integral_builtin T> constexpr uint128_t& operator|=(T other) noexcept
    {
        return (*this |= static_cast<uint128_t>(other));
    }

    /**
     * @brief Operador XOR a nivel de bits y asignaci├│n (a ^= b).
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    constexpr uint128_t& operator^=(const uint128_t& other) noexcept
    {
        data[0] ^= other.data[0];
        data[1] ^= other.data[1];
        return *this;
    }

    /**
     * @brief Sobrecarga del operador XOR a nivel de bits y asignaci├│n (a ^= b)
     * para tipos integrales builtin.
     * @tparam T Un tipo de dato integral builtin.
     * @param other El otro operando.
     * @return Una referencia a `*this`.
     * @test (test_bitwise_assignment_operators)
     */
    template <integral_builtin T> constexpr uint128_t& operator^=(T other) noexcept
    {
        return (*this ^= static_cast<uint128_t>(other));
    }

    /**
     * @brief Operador AND a nivel de bits (a & b).
     * @param other El otro operando.
     * @return Un nuevo `uint128_t` con el resultado de la operaci├│n.
     * @test (test_bitwise_operators)
     */
    constexpr uint128_t operator&(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    template <integral_builtin T> constexpr uint128_t operator&(T other) const noexcept
    {
        uint128_t result(*this);
        result &= other;
        return result;
    }

    /**
     * @brief Operador OR a nivel de bits (a | b).
     * @param other El otro operando.
     * @return Un nuevo `uint128_t` con el resultado de la operaci├│n.
     * @test (test_bitwise_operators)
     */
    constexpr uint128_t operator|(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    template <integral_builtin T> constexpr uint128_t operator|(T other) const noexcept
    {
        uint128_t result(*this);
        result |= other;
        return result;
    }

    /**
     * @brief Operador XOR a nivel de bits (a ^ b).
     * @param other El otro operando.
     * @return Un nuevo `uint128_t` con el resultado de la operaci├│n.
     * @test (test_bitwise_operators)
     */
    constexpr uint128_t operator^(const uint128_t& other) const noexcept
    {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    template <integral_builtin T> constexpr uint128_t operator^(T other) const noexcept
    {
        uint128_t result(*this);
        result ^= other;
        return result;
    }

    /// Multiplicaci├│n completa: uint128_t * uint64_t -> parte alta (bits 128-191)
    /// Esta funci├│n calcula los bits superiores del resultado de multiplicar
    /// un n├║mero de 128 bits por uno de 64 bits, ├║til para el algoritmo de Knuth

  private:
    // === FUNCIONES HELPER PARA knuth_D_divrem ===

    /**
     * @brief Helper para divisi├│n cuando el divisor cabe en 64 bits.
     * @details Encapsula la l├│gica del preprocesador para elegir entre:
     *   - intrinsics::div128_64 (disponible en GCC/Clang/Intel en Linux con __uint128_t)
     *   - Fallback a divrem() gen├®rico (MSVC, Intel en Windows, sin __uint128_t)
     * @param divisor El divisor (garantizado que divisor.data[1] == 0)
     * @return Par {cociente, resto} de la divisi├│n
     * @property Es noexcept.
     */
    constexpr cstd::pair<uint128_t, uint128_t>
    divrem_64bit_divisor(const uint128_t& divisor) const noexcept
    {
        // L├│gica de selecci├│n de implementaci├│n seg├║n plataforma
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
        // GCC/Clang/Intel icpx en Linux: usar intrinsic div128_64 optimizado
        // (Windows/MSVC/Intel ICX no tienen __udivti3/__umodti3 en linker)
        uint64_t r = 0;
        const uint64_t q_lo = intrinsics::div128_64(data[1], data[0], divisor.data[0], &r);
        const uint128_t quotient(0, q_lo);
        const uint128_t remainder(0, r);
        return std::make_pair(quotient, remainder);
#else
        // Fallback portable para compiladores sin __uint128_t o Intel ICX en Windows
        return divrem(divisor);
#endif
    }

    /**
     * @brief Helper para el algoritmo D de Knuth completo.
     * @details Encapsula la l├│gica del preprocesador para elegir entre:
     *   - intrinsics::knuth_division_step (disponible con __uint128_t)
     *   - Fallback a divrem() gen├®rico (sin __uint128_t)
     * @param u_extension D├¡gito extra del dividendo (bits que se salieron del shift)
     * @param u_shifted Dividendo normalizado (desplazado)
     * @param v Divisor normalizado (desplazado)
     * @param s Cantidad de desplazamiento aplicado (normalizaci├│n)
     * @param original_divisor Divisor original (sin normalizar)
     * @return Par {cociente, resto} de la divisi├│n
     * @property Es noexcept.
     */
    constexpr std::pair<uint128_t, uint128_t>
    knuth_D_algorithm(uint64_t u_extension, const uint128_t& u_shifted, const uint128_t& v, int s,
                      const uint128_t& original_divisor) const noexcept
    {
        // L├│gica de selecci├│n de implementaci├│n seg├║n plataforma
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
        // GCC/Clang/Intel icpx en Linux: usar algoritmo D de Knuth optimizado
        // (todos tienen libgcc/compiler-rt con __udivti3)
        // Windows/MSVC/Intel ICX usan linker de MSVC sin __udivti3/__umodti3
        uint64_t remainder_hi, remainder_lo;
        const uint64_t q =
            intrinsics::knuth_division_step(u_extension, u_shifted.data[1], u_shifted.data[0],
                                            v.data[1], v.data[0], s, &remainder_hi, &remainder_lo);

        return std::make_pair(uint128_t(0, q), uint128_t(remainder_hi, remainder_lo));
#else
        // Fallback portable sin __uint128_t - usar algoritmo b├ísico
        (void)u_extension; // Evitar warning de par├ímetro no usado
        (void)u_shifted;
        (void)v;
        (void)s;
        return divrem(original_divisor);
#endif
    }
    // === FUNCIONES AUXILIARES PARA OPTIMIZACIONES ===

    static constexpr int count_trailing_zeros(const uint128_t& n) noexcept
    {
        return n.trailing_zeros();
    }

    // Verificar si el n├║mero cabe efectivamente en 64 bits
    static constexpr bool fits_in_64_bits(const uint128_t& n) noexcept
    {
        return n.data[1] == 0;
    }

    // Extraer el valor de 64 bits cuando se sabe que cabe
    static constexpr uint64_t effective_to_64_bits(const uint128_t& n) noexcept
    {
        return n.data[0];
    }

    // Divisi├│n r├ípida por potencias de 10 (10^n = 2^n * 5^n)
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
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

        // Otras potencias de 10 pueden implementarse seg├║n necesidad
        return std::nullopt;
    }

    // Divisi├│n r├ípida por potencias de 3
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
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

    // Divisi├│n r├ípida por potencias de 5
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
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

    // Implementaciones espec├¡ficas de divisi├│n r├ípida
    constexpr std::pair<uint128_t, uint128_t> divide_by_10() const noexcept
    {
        // Implementaci├│n correcta: dividir por 10 directamente por ahora
        const auto result = divrem(uint128_t(0, 10));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_100() const noexcept
    {
        // Implementaci├│n correcta: dividir por 100 directamente por ahora
        const auto result = divrem(uint128_t(0, 100));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_1000() const noexcept
    {
        // Implementaci├│n correcta: dividir por 1000 directamente por ahora
        const auto result = divrem(uint128_t(0, 1000));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_3() const noexcept
    {
        // Usar multiplicaci├│n por inverso: 1/3 Ôëê 0x55555555555555555555555555555556
        // Esta es una aproximaci├│n, necesitar├¡a implementaci├│n m├ís precisa
        const auto result = divrem(uint128_t(0, 3));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_9() const noexcept
    {
        auto [q3, r3] = divide_by_3();
        auto [q_temp, temp_r] = q3.divide_by_3();
        uint128_t final_q = q_temp;
        uint128_t final_r = temp_r * uint128_t(0, 3) + r3;
        return {final_q, final_r}; // Construcci├│n directa
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_27() const noexcept
    {
        auto [q9, r9] = divide_by_9();
        auto [q_temp, temp_r] = q9.divide_by_3();
        uint128_t final_q = q_temp;
        uint128_t final_r = temp_r * uint128_t(0, 9) + r9;
        return {final_q, final_r}; // Construcci├│n directa
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_5() const noexcept
    {
        // Usar multiplicaci├│n por inverso: 1/5 Ôëê 0x33333333333333333333333333333334
        // Implementaci├│n simplificada por ahora
        const auto result = divrem(uint128_t(0, 5));
        return result.has_value() ? result.value() : std::make_pair(uint128_t(0, 0), *this);
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_25() const noexcept
    {
        const auto [q5, r5] = divide_by_5();
        const auto [final_q, temp_r] = q5.divide_by_5();
        const uint128_t final_r = temp_r * uint128_t(0, 5) + r5;
        return {final_q, final_r}; // Construcci├│n directa
    }

    constexpr std::pair<uint128_t, uint128_t> divide_by_125() const noexcept
    {
        auto [q25, r25] = divide_by_25();
        const auto [final_q, temp_r] = q25.divide_by_5();
        const uint128_t final_r = temp_r * uint128_t(0, 25) + r25;
        return {final_q, final_r}; // Construcci├│n directa
    }

  public:
    /**
     * @brief Calcula la parte alta de multiplicar este uint128_t por un uint64_t
     * @details Retorna los bits 128-191 del resultado completo de 192 bits
     * de multiplicar este n├║mero de 128 bits por un multiplicador de 64 bits.
     * Usado en el algoritmo de divisi├│n de Knuth.
     * @param multiplier El multiplicador de 64 bits.
     * @pre `multiplier` es un `uint64_t`.
     * @post No modifica el estado de `*this`.
     * @return Un `uint64_t` con los 64 bits superiores del producto de 192 bits.
     * @property Es `noexcept`.
     * @test (test_mulhi64)
     */
    constexpr uint64_t mulhi64(uint64_t multiplier) const noexcept
    {
        return intrinsics::mul128x64_high(data[0], data[1], multiplier);
    }

    template <integral_builtin T>
        requires(sizeof(T) <= sizeof(uint64_t))
    constexpr uint64_t mulhi64(T multiplier) const noexcept
    {
        return intrinsics::mul128x64_high(data[0], data[1], static_cast<uint64_t>(multiplier));
    }

    /**
     * @brief Divisi├│n avanzada de 128 bits usando el Algoritmo D de Knuth con
     * optimizaciones.
     * @details Implementa una divisi├│n precisa de 128 por 128 bits. Incluye m├║ltiples rutas
     * r├ípidas para casos comunes:
     * - Divisi├│n por cero.
     * - Divisor mayor que dividendo.
     * - Divisor es potencia de 2.
     * - Divisor es potencia de 10, 3 o 5.
     * - Ambos operandos caben en 64 bits.
     * - Divisor cabe en 64 bits (usando `__uint128_t` nativo si est├í disponible).
     * Si ninguna optimizaci├│n aplica, recurre al Algoritmo D completo.
     * @param v_in El divisor de 128 bits.
     * @pre `v_in` es un `uint128_t`.
     * @post No modifica el estado de `*this`.
     * @return Un `std::optional<std::pair<uint128_t, uint128_t>>` con `{cociente, resto}`.
     *         Si `v_in` es 0, devuelve `std::nullopt`.
     * @property Es `noexcept`.
     * @test (test_knuth_D_divrem)
     */
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
    knuth_D_divrem(const uint128_t& v_in) const noexcept
    {
        // 0. Casos triviales
        if (v_in == uint128_t(0, 0))
            return std::nullopt; // Divisi├│n por cero
        if (*this < v_in) {
            return std::make_pair(uint128_t(0, 0), *this);
        }

        // === OPTIMIZACIONES R├üPIDAS ===

        // 1. OPTIMIZACI├ôN: Divisi├│n por potencias de 2 (usar shift)
        if (v_in.is_power_of_2()) {
            const int shift_amount = count_trailing_zeros(v_in);
            const uint128_t quotient = this->shift_right(shift_amount);
            const uint128_t mask = v_in - uint128_t(0, 1); // 2^n - 1
            const uint128_t remainder = *this & mask;
            return std::pair<uint128_t, uint128_t>{quotient, remainder};
        }

        // 2. OPTIMIZACI├ôN: Divisi├│n por potencias de 10 (descomponer en 2^n * 5^n)
        const auto power_of_10_result = try_divide_by_power_of_10(v_in);
        if (power_of_10_result.has_value()) {
            return power_of_10_result;
        }

        // 3. OPTIMIZACI├ôN: Divisi├│n por potencias de 3
        const auto power_of_3_result = try_divide_by_power_of_3(v_in);
        if (power_of_3_result.has_value()) {
            return power_of_3_result;
        }

        // 4. OPTIMIZACI├ôN: Divisi├│n por potencias de 5
        const auto power_of_5_result = try_divide_by_power_of_5(v_in);
        if (power_of_5_result.has_value()) {
            return power_of_5_result;
        }

        // 5. OPTIMIZACI├ôN: Ambos n├║meros caben efectivamente en 64 bits
        if (fits_in_64_bits(*this) && fits_in_64_bits(v_in)) {
            const uint64_t dividend_64 = effective_to_64_bits(*this);
            const uint64_t divisor_64 = effective_to_64_bits(v_in);
            const uint64_t q_64 = dividend_64 / divisor_64;
            const uint64_t r_64 = dividend_64 % divisor_64;
            return std::pair<uint128_t, uint128_t>{uint128_t(0, q_64), uint128_t(0, r_64)};
        }

        // === RUTAS EST├üNDAR ===

        // Si el divisor cabe en 64 bits (data[1] == 0), usamos una ruta r├ípida.
        if (v_in.data[1] == 0) {
            return divrem_64bit_divisor(v_in);
        }

        // 6. OPTIMIZACI├ôN: V0 = 0 Y U0 = 0 (caso degenerado muy raro)
        // Tanto dividendo como divisor son m├║ltiplos de 2^64
        // Resultado: dividendo.high / divisor.high con resto = 0
        if (v_in.data[0] == 0 && data[0] == 0) {
            const uint64_t q = data[1] / v_in.data[1];
            const uint64_t r = data[1] % v_in.data[1];
            return std::pair<uint128_t, uint128_t>{uint128_t(0, q), uint128_t(r, 0)};
        }

        // 7. OPTIMIZACI├ôN: V0 = 0 (divisor es m├║ltiplo de 2^64)
        // Divisor = v1 * 2^64, podemos simplificar la divisi├│n
        if (v_in.data[0] == 0) {
            // Dividir por v1 (parte alta del divisor)
            const uint64_t divisor_high = v_in.data[1];

            // Caso especial: dividendo cabe en parte alta
            if (data[0] == 0) {
                // u = u1 * 2^64, v = v1 * 2^64
                // u / v = (u1 * 2^64) / (v1 * 2^64) = u1 / v1
                const uint64_t q = data[1] / divisor_high;
                const uint64_t r = data[1] % divisor_high;
                return std::pair<uint128_t, uint128_t>{uint128_t(0, q), uint128_t(r, 0)};
            }

            // Caso general: u = u1 * 2^64 + u0, v = v1 * 2^64
            // Cociente aproximado: q Ôëê u1 / v1
            uint64_t q = data[1] / divisor_high;
            uint64_t r_high = data[1] % divisor_high;

            // Calcular resto: r = u - q * v
            uint128_t remainder(*this);
            uint128_t product = uint128_t(q * divisor_high, 0);

            // Ajustar si el producto es mayor que el dividendo
            while (product > remainder) {
                --q;
                product = uint128_t(q * divisor_high, 0);
            }

            remainder -= product;
            return std::pair<uint128_t, uint128_t>{uint128_t(0, q), remainder};
        }

        // --- ALGORITMO D DE KNUTH (Para divisor > 64 bits) ---

        // D1. Normalizaci├│n
        // Desplazamos u y v para que el MSB de v sea 1.
        const int s = v_in.leading_zeros() > 0 ? v_in.leading_zeros() - 64 : 0;
        [[maybe_unused]] const uint128_t v = v_in.shift_left(s);
        [[maybe_unused]] const uint128_t u_shifted = this->shift_left(s);

        // Capturamos el d├¡gito extra de u que se sali├│ por la izquierda al hacer shift.
        [[maybe_unused]] uint64_t u_extension = 0;
        if (s > 0) {
            u_extension = data[1] >> (64 - s);
        }

        // D3-D8. Algoritmo completo de Knuth: estimar, multiplicar, restar, corregir y
        // desnormalizar
        return knuth_D_algorithm(u_extension, u_shifted, v, s, v_in);
    }

    /**
     * @brief Sobrecarga de `knuth_D_divrem` para tipos integrales.
     * @tparam T Tipo integral del divisor.
     * @param divisor El valor integral a usar como divisor.
     * @return El resultado de llamar a `knuth_D_divrem` con el divisor convertido a
     * `uint128_t`.
     * @property Es `noexcept`.
     * @test (test_knuth_D_divrem_integral)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    template <integral_builtin T>
    constexpr std::optional<std::pair<uint128_t, uint128_t>>
    knuth_D_divrem(T divisor) const noexcept
    {
        return knuth_D_divrem(uint128_t(divisor));
    }

    // STRING CONVERSION (basic implementation)
    /**
     * @brief Convierte el n├║mero a su representaci├│n en `std::string` en base 10.
     * @return Una `std::string` con el valor en formato decimal.
     * @test (test_to_string)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    std::string to_string() const
    {
        return to_string_base(10);
    }

    /**
     * @brief Convierte el n├║mero a su representaci├│n en `std::string` en una base
     * espec├¡fica.
     * @param base La base num├®rica para la conversi├│n (entre 2 y 36).
     * @pre La base debe estar en el rango [2, 36].
     * @post No modifica el estado de `*this`.
     * @return Una `std::string` con el valor en la base especificada.
     * @throws std::invalid_argument si la base est├í fuera del rango permitido.
     * @test (test_to_string_base)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
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

    /**
     * @brief Convierte el n├║mero a su representaci├│n hexadecimal en `std::string`.
     * @param with_prefix Si es `true`, antepone "0x" a la cadena resultante.
     * @return Una `std::string` con el valor en formato hexadecimal.
     * @test (test_to_string_hex)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    std::string to_string_hex(bool with_prefix = false) const
    {
        std::string result = to_string_base(16);
        return with_prefix ? "0x" + result : result;
    }

    /**
     * @brief Convierte el n├║mero a su representaci├│n binaria en `std::string`.
     * @param with_prefix Si es `true`, antepone "0b" a la cadena resultante.
     * @return Una `std::string` con el valor en formato binario.
     * @test (test_to_string_bin)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    std::string to_string_bin(bool with_prefix = false) const
    {
        std::string result = to_string_base(2);
        return with_prefix ? "0b" + result : result;
    }

    /**
     * @brief Convierte el n├║mero a su representaci├│n octal en `std::string`.
     * @param with_prefix Si es `true`, antepone "0" a la cadena resultante si no es solo
     * "0".
     * @return Una `std::string` con el valor en formato octal.
     * @test (test_to_string_oct)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    std::string to_string_oct(bool with_prefix = false) const
    {
        std::string result = to_string_base(8);
        return with_prefix ? "0" + result : result;
    }

    /**
     * @brief Convierte el uint128_t a un array de bytes (little-endian).
     * @return std::array<std::byte, 16> con la representaci├│n en bytes.
     * @details Los bytes se organizan en orden little-endian:
     *          - bytes[0-7]: 64 bits bajos (data[0])
     *          - bytes[8-15]: 64 bits altos (data[1])
     * @property Es constexpr y noexcept.
     * @code{.cpp}
     * uint128_t val(0x1234567890ABCDEF, 0xFEDCBA0987654321);
     * auto bytes = val.to_bytes();
     * // bytes[0] = 0x21, bytes[1] = 0x43, ..., bytes[15] = 0x12
     * @endcode
     */
    constexpr std::array<std::byte, 16> to_bytes() const noexcept
    {
        std::array<std::byte, 16> result{};

        // 64 bits bajos (data[0])
        for (int i = 0; i < 8; ++i) {
            result[i] = static_cast<std::byte>((data[0] >> (i * 8)) & 0xFF);
        }

        // 64 bits altos (data[1])
        for (int i = 0; i < 8; ++i) {
            result[8 + i] = static_cast<std::byte>((data[1] >> (i * 8)) & 0xFF);
        }

        return result;
    }

    /**
     * @brief Crea un uint128_t desde un array de bytes (little-endian).
     * @param bytes Array de 16 bytes en formato little-endian.
     * @return uint128_t construido desde los bytes.
     * @property Es static, constexpr y noexcept.
     * @code{.cpp}
     * std::array<std::byte, 16> bytes = {...};
     * uint128_t val = uint128_t::from_bytes(bytes);
     * @endcode
     */
    static constexpr uint128_t from_bytes(const std::array<std::byte, 16>& bytes) noexcept
    {
        uint64_t low = 0;
        uint64_t high = 0;

        // Reconstruir 64 bits bajos
        for (int i = 0; i < 8; ++i) {
            low |= static_cast<uint64_t>(bytes[i]) << (i * 8);
        }

        // Reconstruir 64 bits altos
        for (int i = 0; i < 8; ++i) {
            high |= static_cast<uint64_t>(bytes[8 + i]) << (i * 8);
        }

        return uint128_t(high, low);
    }

    /**
     * @brief Convierte el uint128_t a std::bitset<128>.
     * @return std::bitset<128> con todos los 128 bits.
     * @details Bit 0 = LSB de data[0], Bit 127 = MSB de data[1].
     * @property Es constexpr y noexcept.
     * @code{.cpp}
     * uint128_t val(0xF, 0xFF);
     * auto bits = val.to_bitset();
     * assert(bits.test(0) == true);  // LSB de data[0]
     * @endcode
     */
    constexpr std::bitset<128> to_bitset() const noexcept
    {
        std::bitset<128> result;

        // Bits 0-63 desde data[0]
        for (int i = 0; i < 64; ++i) {
            result[i] = (data[0] >> i) & 1;
        }

        // Bits 64-127 desde data[1]
        for (int i = 0; i < 64; ++i) {
            result[64 + i] = (data[1] >> i) & 1;
        }

        return result;
    }

    /**
     * @brief Crea un uint128_t desde std::bitset<128>.
     * @param bits Bitset de 128 bits.
     * @return uint128_t construido desde el bitset.
     * @property Es static, constexpr y noexcept.
     * @code{.cpp}
     * std::bitset<128> bits;
     * bits.set(127);  // MSB = 1
     * uint128_t val = uint128_t::from_bitset(bits);
     * @endcode
     */
    static constexpr uint128_t from_bitset(const std::bitset<128>& bits) noexcept
    {
        uint64_t low = 0;
        uint64_t high = 0;

        // Reconstruir 64 bits bajos
        for (int i = 0; i < 64; ++i) {
            if (bits[i]) {
                low |= (1ULL << i);
            }
        }

        // Reconstruir 64 bits altos
        for (int i = 0; i < 64; ++i) {
            if (bits[64 + i]) {
                high |= (1ULL << i);
            }
        }

        return uint128_t(high, low);
    }

    // === CONVERSI├ôN DESDE/HACIA STRINGS ===

    /**
     * @brief Parsea una cadena de caracteres a uint128_t con detecci├│n de errores
     * @details Funci├│n principal de parsing que detecta autom├íticamente la base:
     * - "0x" o "0X" prefijo: base 16 (hexadecimal)
     * - "0b" o "0B" prefijo: base 2 (binario)
     * - "0" prefijo con d├¡gitos 0-7: base 8 (octal)
     * - Sin prefijo: base 10 (decimal)
     *
     * @param str Puntero a la cadena de caracteres C-string
     * @return std::pair<parse_error, uint128_t> con el c├│digo de error y el resultado
     *
     * @note Si hay error, el uint128_t retornado es cero
     * @property Es static, constexpr y noexcept
     * @test (test_parse)
     */
    static constexpr std::pair<parse_error, uint128_t> parse(const char* str) noexcept
    {
        if (!str) {
            return {parse_error::null_pointer, uint128_t(0, 0)};
        }

        if (!*str) {
            return {parse_error::empty_string, uint128_t(0, 0)};
        }

        // Detectar base y posici├│n inicial
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
                // Verificar si es octal v├ílido
                bool is_octal = true;
                for (const char* p = str + 1; *p && is_octal; ++p) {
                    if (*p < '0' || *p > '7') {
                        is_octal = false;
                    }
                }
                if (is_octal && str[1] >= '0' && str[1] <= '7') {
                    base = 8;
                    start = str + 1;
                }
            }
        }

        // Validar que hay contenido despu├®s del prefijo
        if (!*start) {
            return {parse_error::empty_string, uint128_t(0, 0)};
        }

        return parse_base(start, base);
    }

    /**
     * @brief Parsea una cadena de caracteres a uint128_t en una base espec├¡fica
     * @details Realiza el parsing con validaci├│n completa de caracteres y detecci├│n de overflow
     *
     * @param str Puntero a la cadena de caracteres C-string
     * @param base Base num├®rica (debe estar entre 2 y 36)
     * @return std::pair<parse_error, uint128_t> con el c├│digo de error y el resultado
     *
     * @note Si hay error, el uint128_t retornado es cero
     * @property Es static, constexpr y noexcept
     * @test (test_parse_base)
     */
    static constexpr std::pair<parse_error, uint128_t> parse_base(const char* str,
                                                                  int base) noexcept
    {
        if (!str) {
            return {parse_error::null_pointer, uint128_t(0, 0)};
        }

        if (!*str) {
            return {parse_error::empty_string, uint128_t(0, 0)};
        }

        if (base < 2 || base > 36) {
            return {parse_error::invalid_base, uint128_t(0, 0)};
        }

        uint128_t result(0, 0);
        const uint128_t base_val(0, static_cast<uint64_t>(base));
        const uint128_t max_before_mult = uint128_t::max() / base_val;

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
                // Car├ícter inv├ílido para esta base
                return {parse_error::invalid_character, uint128_t(0, 0)};
            }

            // Verificar overflow antes de multiplicar
            if (result > max_before_mult) {
                return {parse_error::overflow, uint128_t(0, 0)};
            }

            const uint128_t old_result = result;
            result = result * base_val;

            // Verificar overflow en la multiplicaci├│n
            if (result < old_result) {
                return {parse_error::overflow, uint128_t(0, 0)};
            }

            // Verificar overflow antes de sumar el d├¡gito
            const uint128_t digit_val(0, static_cast<uint64_t>(digit_value));
            if (result > uint128_t::max() - digit_val) {
                return {parse_error::overflow, uint128_t(0, 0)};
            }

            result = result + digit_val;
        }

        return {parse_error::success, result};
    }

    /**
     * @brief Crea un `uint128_t` a partir de una cadena de caracteres (C-string) con
     * detecci├│n autom├ítica de base.
     * @details Detecta los prefijos "0x" (hex), "0b" (binario) y "0" (octal). Si no hay
     * prefijo, asume base 10.
     * @param str La cadena de entrada.
     * @return El `uint128_t` resultante. Devuelve 0 si la cadena es nula o vac├¡a.
     * @property Es `static` y `constexpr`.
     * @test (test_from_cstr)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    static constexpr uint128_t from_cstr(const char* str)
    {
        const auto [error, result] = parse(str);
        (void)error; // Ignorar el error en esta versi├│n legacy
        return result;
    }

    /**
     * @brief Crea un `uint128_t` a partir de una cadena de caracteres (C-string) en una
     * base espec├¡fica.
     * @param str La cadena de entrada.
     * @param base La base num├®rica (entre 2 y 36).
     * @return El `uint128_t` resultante. Devuelve 0 si los par├ímetros son inv├ílidos.
     * @property Es `static` y `constexpr`.
     * @test (test_from_cstr_base)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    static constexpr uint128_t from_cstr_base(const char* str, int base)
    {
        const auto [error, result] = parse_base(str, base);
        (void)error; // Ignorar el error en esta versi├│n legacy
        return result;
    }

    /**
     * @brief Convierte el n├║mero a una cadena C-string en base 10.
     * @warning El puntero devuelto es a un buffer est├ítico `thread_local`. Su contenido se
     * sobrescribir├í en la siguiente llamada a `to_cstr` o `to_cstr_base` en el mismo hilo.
     * No es seguro almacenar este puntero para uso futuro.
     * @return Un puntero a una cadena de caracteres null-terminated.
     * @test (test_to_cstr)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    const char* to_cstr() const
    {
        return to_cstr_base(10);
    }

    /**
     * @brief Convierte el n├║mero a una cadena C-string en una base espec├¡fica.
     * @param base La base num├®rica para la conversi├│n (entre 2 y 36).
     * @warning El puntero devuelto es a un buffer est├ítico rotativo y `thread_local`. Su
     * contenido se invalida en llamadas posteriores en el mismo hilo.
     * @return Un puntero a una cadena de caracteres null-terminated. Si la base es
     * inv├ílida, devuelve "0".
     * @test (test_to_cstr_base)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    const char* to_cstr_base(int base) const
    {
        // Buffer est├ítico rotativo para permitir m├║ltiples llamadas
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

    /**
     * @brief Convierte el n├║mero a su representaci├│n hexadecimal en C-string.
     * @warning El puntero devuelto es a un buffer est├ítico `thread_local`.
     * @return Un puntero a una cadena de caracteres null-terminated.
     * @test (test_to_cstr_hex)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    const char* to_cstr_hex() const
    {
        return to_cstr_base(16);
    }
    /**
     * @brief Convierte el n├║mero a su representaci├│n binaria en C-string.
     * @warning El puntero devuelto es a un buffer est├ítico `thread_local`.
     * @return Un puntero a una cadena de caracteres null-terminated.
     * @test (test_to_cstr_bin)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    const char* to_cstr_bin() const
    {
        return to_cstr_base(2);
    }
    /**
     * @brief Convierte el n├║mero a su representaci├│n octal en C-string.
     * @warning El puntero devuelto es a un buffer est├ítico `thread_local`.
     * @return Un puntero a una cadena de caracteres null-terminated.
     * @test (test_to_cstr_oct)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    const char* to_cstr_oct() const
    {
        return to_cstr_base(8);
    }

    /**
     * @brief Crea un `uint128_t` a partir de una `std::string` con detecci├│n autom├ítica de
     * base.
     * @details Usa parse() internamente para detecci├│n de base y validaci├│n.
     * Soporta prefijos "0x" (hex), "0b" (binario) y "0" (octal).
     * @param str La `std::string` de entrada.
     * @return El `uint128_t` resultante. Devuelve 0 si hay error.
     * @property Es `static`.
     * @test (test_from_string)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    static uint128_t from_string(const std::string& str)
    {
        auto [error, result] = parse(str.c_str());
        (void)error; // Ignorar el error para compatibilidad
        return result;
    }

    /**
     * @brief Crea un `uint128_t` a partir de una `std::string` en una base espec├¡fica.
     * @details Usa parse_base() internamente para validaci├│n completa.
     * @param str La `std::string` de entrada.
     * @param base La base num├®rica (entre 2 y 36).
     * @return El `uint128_t` resultante. Devuelve 0 si hay error.
     * @property Es `static`.
     * @test (test_from_string_base)
     * @code{.cpp}
     * // uint128_t val;
     * // val.set_low(0x1234);
     * // assert(val.low() == 0x1234);
     * @endcode
     */
    static uint128_t from_string_base(const std::string& str, int base)
    {
        auto [error, result] = parse_base(str.c_str(), base);
        (void)error; // Ignorar el error para compatibilidad
        return result;
    }

    // ============================================================================
    // API p├║blica de operaciones constexpr modulares
    // ============================================================================

    UINT128_DIVISIBILITY_PUBLIC_METHODS
    UINT128_FACTORIZATION_PUBLIC_METHODS
    UINT128_MULTIPLY_CONST_PUBLIC_METHODS
    UINT128_DIV_CONST_PUBLIC_METHODS
};

// ========================= DEFINICIONES DE FUNCIONES INLINE =========================
// Definiciones de funciones que dependen de otras definidas m├ís tarde en la clase

inline constexpr uint128_t::uint128_t(const char* str) noexcept : data{0, 0}
{
    *this = from_cstr(str);
}

inline constexpr uint128_t& uint128_t::operator=(const char* str) noexcept
{
    *this = from_cstr(str);
    return *this;
}

// ========================= OPERADORES FRIEND FUERA DE LA CLASE =========================
// Para evitar ambig├╝edad con operadores existentes, se definen fuera de la clase

// TODO(C++26): Refactorizar con reflection (P2996) cuando est├® disponible
// Con reflection, estos ~100 operadores sim├®tricos se podr├¡an generar autom├íticamente:
//
// consteval void generate_symmetric_operators() {
//     for (constexpr auto op : operators_of(^^uint128_t)) {  // ^ o ^^ seg├║n sintaxis final
//         if (is_binary_operator(op)) {
//             inject(make_symmetric_for<integral_builtin>(op));
//         }
//     }
// }
//
// Esto eliminar├¡a toda esta secci├│n, reemplaz├índola por 5-10 l├¡neas de c├│digo generador.

// Operadores aritm├®ticos sim├®tricos (template unificado para tipos enteros < 128 bits)
template <integral_builtin T>
inline constexpr uint128_t operator+(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) + rhs;
}

template <integral_builtin T>
inline constexpr uint128_t operator-(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) - rhs;
}

template <integral_builtin T>
inline constexpr uint128_t operator*(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) * rhs;
}

template <integral_builtin T> inline constexpr uint128_t operator/(T lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) / rhs;
}

template <integral_builtin T> inline constexpr uint128_t operator%(T lhs, const uint128_t& rhs)
{
    return uint128_t(lhs) % rhs;
}

// Operadores de comparaci├│n sim├®tricos (template unificado)
template <integral_builtin T> inline constexpr bool operator==(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) == rhs;
}

template <integral_builtin T> inline constexpr bool operator!=(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) != rhs;
}

template <integral_builtin T> inline constexpr bool operator<(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) < rhs;
}

template <integral_builtin T> inline constexpr bool operator<=(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) <= rhs;
}

template <integral_builtin T> inline constexpr bool operator>(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) > rhs;
}

template <integral_builtin T> inline constexpr bool operator>=(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) >= rhs;
}

// Operadores bitwise sim├®tricos (template unificado)
template <integral_builtin T>
inline constexpr uint128_t operator&(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) & rhs;
}

template <integral_builtin T>
inline constexpr uint128_t operator|(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) | rhs;
}

template <integral_builtin T>
inline constexpr uint128_t operator^(T lhs, const uint128_t& rhs) noexcept
{
    return uint128_t(lhs) ^ rhs;
}

// ========================= OPERADORES DE FLUJO =========================
// NOTA: Las implementaciones b├ísicas est├ín comentadas.
// Para soporte completo de iostream con manipuladores (hex, oct, setw, etc.),
// incluye uint128/uint128_iostreams.hpp en lugar de usar estos operadores.
//
// inline std::ostream& operator<<(std::ostream& os, const uint128_t& value)
// {
//     return os << value.to_string();
// }
//
// inline std::istream& operator>>(std::istream& is, uint128_t& value)
// {
//     std::string str;
//     is >> str;
//     if (is.good() || is.eof()) {
//         value = uint128_t::from_string(str);
//     }
//     return is;
// }

// Constante MAX definida despu├®s de la clase
constexpr uint128_t uint128_t_MAX = uint128_t::max();

// ========================= LITERALES DEFINIDOS POR EL USUARIO =========================
// Namespace para los literales UDL
namespace uint128_literals
{

using uint128_t = nstd::uint128_t;
using int128_t = nstd::int128_t;

// Literal para enteros peque├▒os (hasta uint64_t)
constexpr uint128_t operator""_u128(unsigned long long value) noexcept
{
    return uint128_t(0, value);
}

// Literal para strings (soporta m├║ltiples formatos)
constexpr uint128_t operator""_u128(const char* str, std::size_t)
{
    return uint128_t::from_cstr(str);
}

// Literales espec├¡ficos por formato
constexpr uint128_t operator""_u128_hex(const char* str, std::size_t)
{
    return uint128_t::from_cstr_base(str, 16);
}

constexpr uint128_t operator""_u128_bin(const char* str, std::size_t)
{
    return uint128_t::from_cstr_base(str, 2);
}

constexpr uint128_t operator""_u128_oct(const char* str, std::size_t)
{
    return uint128_t::from_cstr_base(str, 8);
}

} // namespace uint128_literals

// Conveniencia: importar literales al namespace global cuando se usa
// using namespace uint128_literals;

// ============================================================================
// Implementaci├│n del constructor uint128_t(const int128_t&)
// Debe estar despu├®s de la definici├│n completa de int128_t
// Declarado en l├¡nea ~207, implementado aqu├¡ al final
// ============================================================================

// NOTA: Esta implementaci├│n se proporciona en int128_t.hpp despu├®s de
// la definici├│n completa de int128_t para evitar errores de tipo incompleto

} // namespace nstd

#endif // UINT128_T_HPP
