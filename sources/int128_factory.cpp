#include "include/int128_factory.hpp"
#include <sstream>
#include <stdexcept>

// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE FÁBRICA DESDE STRING
// =============================================================================

uint128_t make_uint128(const std::string& str, int base)
{
    if (str.empty()) {
        throw std::invalid_argument("Empty string cannot be converted to uint128_t");
    }

    uint128_t result{};
    size_t pos = 0;

    // Manejo de prefijos
    if (base == 16 && str.size() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        pos = 2;
    } else if (base == 8 && str.size() > 1 && str[0] == '0') {
        pos = 1;
    } else if (base == 2 && str.size() > 2 && str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        pos = 2;
    }

    uint128_t base_val = make_uint128(static_cast<uint64_t>(base));

    for (size_t i = pos; i < str.size(); ++i) {
        char c = str[i];
        int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else {
            throw std::invalid_argument("Invalid character in number string");
        }

        if (digit >= base) {
            throw std::invalid_argument("Digit exceeds base");
        }

        result = result * base_val + make_uint128(static_cast<uint64_t>(digit));
    }

    return result;
}

int128_t make_int128(const std::string& str, int base)
{
    if (str.empty()) {
        throw std::invalid_argument("Empty string cannot be converted to int128_t");
    }

    bool negative = false;
    size_t start_pos = 0;

    // Manejo del signo
    if (str[0] == '-') {
        negative = true;
        start_pos = 1;
    } else if (str[0] == '+') {
        start_pos = 1;
    }

    if (start_pos >= str.size()) {
        throw std::invalid_argument("Invalid number string");
    }

    // Usar make_uint128 para parsear la parte numérica
    std::string abs_str = str.substr(start_pos);
    uint128_t abs_value = make_uint128(abs_str, base);

    // Verificar overflow para valores positivos
    uint128_t max_positive = make_uint128(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    if (!negative && abs_value > max_positive) {
        throw std::overflow_error("Value too large for int128_t");
    }

    // Verificar overflow para valores negativos (-2^127)
    uint128_t max_negative = make_uint128(0x8000000000000000ULL, 0x0000000000000000ULL);
    if (negative && abs_value > max_negative) {
        throw std::overflow_error("Value too negative for int128_t");
    }

    int128_t result = make_int128(abs_value);
    if (negative) {
        result = -result;
    }

    return result;
}

// =============================================================================
// FUNCIONES DE CONVERSIÓN SEGURA
// =============================================================================

int128_t safe_uint128_to_int128(const uint128_t& value)
{
    // Verificar que el bit de signo no esté puesto
    if (value.high() & 0x8000000000000000ULL) {
        throw std::overflow_error("uint128_t value too large for int128_t");
    }

    return make_int128(value.high(), value.low());
}

uint128_t safe_int128_to_uint128(const int128_t& value)
{
    // Verificar que el valor no sea negativo
    if (value.is_negative()) {
        throw std::underflow_error("Cannot convert negative int128_t to uint128_t");
    }

    return make_uint128(value.high(), value.low());
}