#ifndef UINT128_TRAITS_HPP
#define UINT128_TRAITS_HPP

#include "uint128_t.hpp"
#include <type_traits>
#include <limits>
#include <concepts>
#include <optional>
#include <algorithm>
#include <functional>

// ===============================================================================
// ESPECIALIZACIÓN DE std::numeric_limits PARA uint128_t
// ===============================================================================

namespace std
{

    template <>
    class numeric_limits<uint128_t>
    {
    public:
        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = false;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr bool has_infinity = false;
        static constexpr bool has_quiet_NaN = false;
        static constexpr bool has_signaling_NaN = false;
        static constexpr bool has_denorm_loss = false;
        static constexpr bool is_iec559 = false;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = true;
        static constexpr bool traps = false;
        static constexpr bool tinyness_before = false;

        static constexpr int digits = 128;
        static constexpr int digits10 = 38; // floor(128 * log10(2)) = 38
        static constexpr int max_digits10 = 0;
        static constexpr int radix = 2;

        static constexpr int min_exponent = 0;
        static constexpr int min_exponent10 = 0;
        static constexpr int max_exponent = 0;
        static constexpr int max_exponent10 = 0;

        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr float_round_style round_style = round_toward_zero;

        // Valores especiales
        static constexpr uint128_t min() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t lowest() noexcept
        {
            return min();
        }

        static constexpr uint128_t max() noexcept
        {
            return uint128_t(UINT64_MAX, UINT64_MAX);
        }

        static constexpr uint128_t epsilon() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t round_error() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t infinity() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t quiet_NaN() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t signaling_NaN() noexcept
        {
            return uint128_t(0, 0);
        }

        static constexpr uint128_t denorm_min() noexcept
        {
            return min();
        }
    };

} // namespace std

// ===============================================================================
// EXTENSIONES DE TYPE_TRAITS PARA uint128_t
// ===============================================================================

namespace std
{

    // Especialización para is_integral
    template <>
    struct is_integral<uint128_t> : std::true_type
    {
    };

    // Especialización para is_arithmetic
    template <>
    struct is_arithmetic<uint128_t> : std::true_type
    {
    };

    // Especialización para is_unsigned
    template <>
    struct is_unsigned<uint128_t> : std::true_type
    {
    };

    // Especialización para is_signed
    template <>
    struct is_signed<uint128_t> : std::false_type
    {
    };

    // Especialización para make_unsigned (es él mismo)
    template <>
    struct make_unsigned<uint128_t>
    {
        using type = uint128_t;
    };

} // namespace std

// ===============================================================================
// CONCEPTS PERSONALIZADOS PARA uint128_t
// ===============================================================================

namespace uint128_concepts
{

    // Concept para tipos que pueden convertirse a uint128_t
    template <typename T>
    concept uint128_convertible = std::integral<T> || std::same_as<T, uint128_t>;

    // Concept para tipos compatibles con operaciones aritméticas con uint128_t
    template <typename T>
    concept uint128_arithmetic = uint128_convertible<T> && requires(T a, uint128_t b) {
        { a + b } -> std::same_as<uint128_t>;
        { a - b } -> std::same_as<uint128_t>;
        { a * b } -> std::same_as<uint128_t>;
    };

    // Concept para tipos compatibles con comparaciones con uint128_t
    template <typename T>
    concept uint128_comparable = uint128_convertible<T> && requires(T a, uint128_t b) {
        { a == b } -> std::same_as<bool>;
        { a != b } -> std::same_as<bool>;
        { a < b } -> std::same_as<bool>;
        { a <= b } -> std::same_as<bool>;
        { a > b } -> std::same_as<bool>;
        { a >= b } -> std::same_as<bool>;
    };

    // Concept para tipos compatibles con operaciones bitwise con uint128_t
    template <typename T>
    concept uint128_bitwise = uint128_convertible<T> && requires(T a, uint128_t b) {
        { a & b } -> std::same_as<uint128_t>;
        { a | b } -> std::same_as<uint128_t>;
        { a ^ b } -> std::same_as<uint128_t>;
    };

} // namespace uint128_concepts

// ===============================================================================
// UTILIDADES ADICIONALES
// ===============================================================================

namespace uint128_utils
{

    // Helper para obtener el tipo más grande entre dos tipos
    template <typename T1, typename T2>
    using larger_type = std::conditional_t<(sizeof(T1) > sizeof(T2)), T1, T2>;

    // Helper para verificar si un tipo puede contener completamente a uint128_t
    template <typename T>
    constexpr bool can_hold_uint128_v = sizeof(T) >= sizeof(uint128_t) && std::is_unsigned_v<T>;

    // Helper para verificar overflow potencial
    template <typename T>
    constexpr bool may_overflow_from_uint128_v = sizeof(T) < sizeof(uint128_t);

    // Función para conversión segura con verificación de overflow
    template <std::integral T>
    constexpr std::optional<T> safe_cast(const uint128_t &value) noexcept
    {
        if constexpr (sizeof(T) >= sizeof(uint128_t))
        {
            // El tipo destino puede contener cualquier valor uint128_t
            return static_cast<T>(value);
        }
        else
        {
            // Verificar overflow
            if (value > std::numeric_limits<T>::max())
            {
                return std::nullopt;
            }
            return static_cast<T>(value);
        }
    }

} // namespace uint128_utils

// ===============================================================================
// FUNCIONES DE HASH PARA UNORDERED_MAP/SET
// ===============================================================================

namespace std
{

    template <>
    struct hash<uint128_t>
    {
        std::size_t operator()(const uint128_t &value) const noexcept
        {
            // Combinar hash de las dos partes usando XOR
            std::hash<uint64_t> hasher;
            return hasher(value.high()) ^ (hasher(value.low()) << 1);
        }
    };

} // namespace std

#endif // UINT128_TRAITS_HPP