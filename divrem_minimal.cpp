#include <cstdint>
#include <iostream>
#include <optional>
#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#endif

struct uint128_simple {
    uint64_t data[2];

    uint128_simple(uint64_t high = 0, uint64_t low = 0)
    {
        data[0] = low;
        data[1] = high;
    }

    bool is_zero() const
    {
        return data[0] == 0 && data[1] == 0;
    }

    void print() const
    {
        std::cout << "High: " << data[1] << ", Low: " << data[0] << std::endl;
    }

    // Paso 1: Funciones de conteo de bits
    int leading_zeros() const
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

    int effective_length() const
    {
        return 128 - leading_zeros();
    }

    // Paso 2: Operadores de comparación
    bool operator==(const uint128_simple& other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1];
    }

    bool operator<(const uint128_simple& other) const
    {
        if (data[1] < other.data[1])
            return true;
        if (data[1] > other.data[1])
            return false;
        return data[0] < other.data[0];
    }

    bool operator>=(const uint128_simple& other) const
    {
        return !(*this < other);
    }

    // Paso 3: Desplazamientos
    uint128_simple shift_left(int positions) const
    {
        if (positions == 0)
            return *this;
        if (positions >= 128)
            return uint128_simple(0, 0);

        if (positions >= 64) {
            return uint128_simple(data[0] << (positions - 64), 0);
        } else {
            uint64_t new_high = (data[1] << positions) | (data[0] >> (64 - positions));
            uint64_t new_low = data[0] << positions;
            return uint128_simple(new_high, new_low);
        }
    }

    uint128_simple shift_right(int positions) const
    {
        if (positions == 0)
            return *this;
        if (positions >= 128)
            return uint128_simple(0, 0);

        if (positions >= 64) {
            return uint128_simple(0, data[1] >> (positions - 64));
        } else {
            uint64_t new_low = (data[0] >> positions) | (data[1] << (64 - positions));
            uint64_t new_high = data[1] >> positions;
            return uint128_simple(new_high, new_low);
        }
    }

    // Paso 4: Operador de sustracción (asume que this >= other)
    uint128_simple operator-(const uint128_simple& other) const
    {
        uint64_t new_low, new_high;

#ifdef _MSC_VER
        unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        _subborrow_u64(borrow, data[1], other.data[1], &new_high);
#else
        // Fallback manual para otros compiladores
        if (data[0] >= other.data[0]) {
            new_low = data[0] - other.data[0];
            new_high = data[1] - other.data[1];
        } else {
            new_low = data[0] - other.data[0];      // Underflow intencional
            new_high = data[1] - other.data[1] - 1; // Borrow
        }
#endif
        return uint128_simple(new_high, new_low);
    }

    // Paso 5: Normalización
    std::pair<uint128_simple, int> normalize_divisor(const uint128_simple& dividendo) const
    {
        int dividend_length = dividendo.effective_length();
        int divisor_length = this->effective_length();
        int shift_amount = dividend_length - divisor_length;

        if (shift_amount <= 0) {
            return std::make_pair(*this, 0);
        }

        return std::make_pair(this->shift_left(shift_amount), shift_amount);
    }

    // Paso 6: Función divrem principal
    std::optional<std::pair<uint128_simple, uint128_simple>>
    divrem(const uint128_simple& divisor) const
    {
        const uint128_simple& dividendo = *this;

        // Detectar división por cero
        if (divisor.is_zero()) {
            return std::nullopt;
        }

        uint128_simple quotient(0, 0);
        uint128_simple remainder(0, 0);

        // Casos especiales
        if (dividendo < divisor) {
            remainder = dividendo;
            return std::make_pair(quotient, remainder);
        } else if (dividendo == divisor) {
            quotient = uint128_simple(0, 1);
            return std::make_pair(quotient, remainder);
        } else if (divisor == uint128_simple(0, 1)) {
            quotient = dividendo;
            return std::make_pair(quotient, remainder);
        }

        // CASO GENERAL: Algoritmo de división larga binaria
        auto normalized_pair = divisor.normalize_divisor(dividendo);
        uint128_simple normalized_divisor = normalized_pair.first;
        int shift_amount = normalized_pair.second;

        remainder = dividendo;
        quotient = uint128_simple(0, 0);

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
};

int main()
{
    std::cout << "=== Prueba del algoritmo divrem completo ===" << std::endl;

    uint128_simple a(0, 100);  // Dividendo: 100
    uint128_simple b(0, 7);    // Divisor: 7
    uint128_simple zero(0, 0); // Divisor cero

    std::cout << "Dividendo: ";
    a.print();
    std::cout << "Divisor: ";
    b.print();

    // Prueba división normal: 100 / 7 = 14 resto 2
    auto result = a.divrem(b);
    if (result.has_value()) {
        auto quotient = result.value().first;
        auto remainder = result.value().second;

        std::cout << "\nResultado de 100 / 7:" << std::endl;
        std::cout << "Cociente: ";
        quotient.print();
        std::cout << "Resto: ";
        remainder.print();

        // Verificación: 7 * 14 + 2 = 100
        std::cout << "Verificación: 7 * " << quotient.data[0] << " + " << remainder.data[0] << " = "
                  << (7 * quotient.data[0] + remainder.data[0]) << std::endl;
    } else {
        std::cout << "Error inesperado en división" << std::endl;
    }

    // Prueba división por cero
    std::cout << "\n--- Prueba división por cero ---" << std::endl;
    auto result_zero = a.divrem(zero);
    if (result_zero.has_value()) {
        std::cout << "ERROR: División por cero no detectada correctamente" << std::endl;
    } else {
        std::cout << "División por cero detectada correctamente" << std::endl;
    }

    return 0;
}