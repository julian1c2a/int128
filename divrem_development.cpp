#include <cstdint>
#include <iostream>
#include <optional>
#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#endif

// Versión simplificada para desarrollar divrem
struct uint128_simple {
    uint64_t data[2]; // data[1]=high, data[0]=low

    uint128_simple(uint64_t high = 0, uint64_t low = 0) : data()
    {
        data[0] = low;
        data[1] = high;
    }

    // Métodos auxiliares para el desarrollo
    bool is_zero() const
    {
        return data[0] == 0 && data[1] == 0;
    }

    void print() const
    {
        std::cout << "High: " << data[1] << ", Low: " << data[0] << std::endl;
    }

    // Función que comprueba si self es potencia de dos (corregida para 128 bits)
    bool is_power_of_two() const
    {
        if (is_zero())
            return false;

        // Contar cuántas partes tienen bits activos
        int non_zero_parts = 0;
        if (data[0] != 0)
            non_zero_parts++;
        if (data[1] != 0)
            non_zero_parts++;

        if (non_zero_parts == 1) {
            // Solo una parte tiene bits, verificar si es potencia de 2
            uint64_t active_part = (data[1] != 0) ? data[1] : data[0];
            return (active_part & (active_part - 1)) == 0;
        }

        return false; // Más de una parte activa no puede ser potencia de 2
    }

    // Cuenta cuántos ceros hay al inicio (MSB) del número
    int leading_zeros() const
    {
        if (data[1] != 0) {
            // Hay bits en la parte alta, contar ceros en data[1]
#ifdef _MSC_VER
            return static_cast<int>(__lzcnt64(data[1]));
#else
            return __builtin_clzll(data[1]);
#endif
        } else if (data[0] != 0) {
            // Solo hay bits en la parte baja, contar ceros en data[0] + 64
#ifdef _MSC_VER
            return 64 + static_cast<int>(__lzcnt64(data[0]));
#else
            return 64 + __builtin_clzll(data[0]);
#endif
        } else {
            // El número es cero, todos los bits son ceros
            return 128;
        }
    }

    // Cuenta cuántos ceros hay al final (LSB) del número
    int trailing_zeros() const
    {
        if (data[0] != 0) {
            // Hay bits en la parte baja, contar ceros en data[0]
#ifdef _MSC_VER
            return static_cast<int>(_tzcnt_u64(data[0]));
#else
            return __builtin_ctzll(data[0]);
#endif
        } else if (data[1] != 0) {
            // Solo hay bits en la parte alta, contar ceros en data[1] + 64
#ifdef _MSC_VER
            return 64 + static_cast<int>(_tzcnt_u64(data[1]));
#else
            return 64 + __builtin_ctzll(data[1]);
#endif

            // Calcula la longitud efectiva (bits significativos)
            int effective_length() const
            {
                return 128 - leading_zeros();
            }

            // Desplaza a la izquierda (normalización)
            uint128_simple shift_left(int positions) const
            {
                if (positions == 0)
                    return *this;
                if (positions >= 128)
                    return uint128_simple(0, 0);

                if (positions >= 64) {
                    // Desplazamiento >= 64, mover todo a la parte alta
                    return uint128_simple(data[0] << (positions - 64), 0);
                } else {
                    // Desplazamiento < 64, dividir entre ambas partes
                    uint64_t new_high = (data[1] << positions) | (data[0] >> (64 - positions));
                    uint64_t new_low = data[0] << positions;
                    return uint128_simple(new_high, new_low);
                }
            }

            // Normalizar divisor para tener la misma longitud efectiva que el dividendo
            std::pair<uint128_simple, int> normalize_divisor(const uint128_simple& dividendo) const
            {
                int dividend_length = dividendo.effective_length();
                int divisor_length = this->effective_length();
                int shift_amount = dividend_length - divisor_length;

                if (shift_amount <= 0) {
                    return std::make_pair(*this, 0); // No necesita normalización
                }

                return std::make_pair(this->shift_left(shift_amount), shift_amount);
            }

            // Operadores de comparación necesarios para división
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

            // Operador de sustracción (asume que this >= other)
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

            // Desplazamiento a la derecha
            uint128_simple shift_right(int positions) const
            {
                if (positions == 0)
                    return *this;
                if (positions >= 128)
                    return uint128_simple(0, 0);

                if (positions >= 64) {
                    // Desplazamiento >= 64, mover todo a la parte baja
                    return uint128_simple(0, data[1] >> (positions - 64));
                } else {
                    // Desplazamiento < 64, dividir entre ambas partes
                    uint64_t new_low = (data[0] >> positions) | (data[1] << (64 - positions));
                    uint64_t new_high = data[1] >> positions;
                    return uint128_simple(new_high, new_low);
                }
            }

            // divrem retorna std::optional - std::nullopt si división por cero
            std::optional<std::pair<uint128_simple, uint128_simple>> divrem(
                const uint128_simple& divisor) const noexcept
            {
                const uint128_simple& dividendo = *this;
                // Detectar división por cero
                if (divisor.is_zero()) {
                    return std::nullopt; // No excepción, solo indica fallo
                }

                uint128_simple quotient(0, 0);
                uint128_simple remainder(0, 0);

                if (dividendo < divisor) {
                    // Si el dividendo es menor que el divisor, el cociente es 0 y el resto es el
                    // dividendo
                    remainder = dividendo;
                    return std::make_pair(quotient, remainder);
                } else if (dividendo == divisor) {
                    // Si el dividendo es igual al divisor, el cociente es 1 y el resto es 0
                    quotient = uint128_simple(0, 1);
                    return std::make_pair(quotient, remainder);
                } else if (divisor == uint128_simple(0, 1)) {
                    // División por 1
                    quotient = dividendo;
                    return std::make_pair(quotient, remainder);
                } else if (divisor.is_power_of_two()) {
                    // División por potencia de dos
                    int shift_amount = 0;
                    if (divisor.data[1] != 0) {
                        // Potencia de dos en la parte alta
                        uint64_t val = divisor.data[1];
                        while (val > 1) {
                            val >>= 1;
                            shift_amount++;
                        }
                        shift_amount += 64; // Ajuste por la parte alta
                    } else {
                        // Potencia de dos en la parte baja
                        uint64_t val = divisor.data[0];
                        while (val > 1) {
                            val >>= 1;
                            shift_amount++;
                        }
                    }

                    // Calcular cociente y resto usando shifts
                    if (shift_amount < 64) {
                        quotient.data[0] = dividendo.data[0] >> shift_amount;
                        quotient.data[1] = (dividendo.data[1] >> shift_amount) |
                                           (dividendo.data[0] << (64 - shift_amount));
                        remainder.data[0] = dividendo.data[0] & ((1ULL << shift_amount) - 1);
                        remainder.data[1] = 0;
                    } else {
                        quotient.data[0] = dividendo.data[1] >> (shift_amount - 64);
                        quotient.data[1] = 0;
                        remainder.data[0] = dividendo.data[0];
                        remainder.data[1] = dividendo.data[1] & ((1ULL << (shift_amount - 64)) - 1);
                    }

                    return std::make_pair(quotient, remainder);
                }

                // CASO GENERAL: Algoritmo de división larga binaria

                // Paso 1: Normalizar el divisor
                auto [normalized_divisor, shift_amount] = divisor.normalize_divisor(dividendo);

                // Paso 2: Inicializar remainder con el dividendo
                remainder = dividendo;
                quotient = uint128_simple(0, 0);

                // Paso 3: Bucle principal de división binaria
                for (int i = shift_amount; i >= 0; i--) {
                    // Desplazar quotient a la izquierda para hacer espacio al siguiente bit
                    quotient = quotient.shift_left(1);

                    // Comparar remainder con normalized_divisor
                    if (remainder >= normalized_divisor) {
                        // remainder >= divisor: restar y poner 1 en el bit menos significativo del
                        // quotient
                        remainder = remainder - normalized_divisor;
                        quotient.data[0] |= 1; // Establecer el bit menos significativo
                    }
                    // Si remainder < divisor, no hacemos nada (bit 0 implícito)

                    // Desplazar el divisor normalizado a la derecha para la siguiente iteración
                    normalized_divisor = normalized_divisor.shift_right(1);
                }

                // El resto ya está correcto (no necesita desnormalización)
                // porque trabajamos con el dividendo original

                return std::make_pair(quotient, remainder);
            }
        };

        int main()
        {
            std::cout << "=== Desarrollo del método divrem para uint128 ===" << std::endl;
            std::cout << "Paso 1: Estructura básica con std::optional" << std::endl;

            uint128_simple a(0, 100);  // Número 100
            uint128_simple b(0, 7);    // Divisor 7
            uint128_simple zero(0, 0); // Divisor cero para probar

            std::cout << "Dividendo: ";
            a.print();
            std::cout << "Divisor: ";
            b.print();

            // Resultado esperado: 100 / 7 = 14 resto 2
            std::cout << "Resultado esperado: cociente=14, resto=2" << std::endl;

            // Probar división normal
            auto result = a.divrem(b);
            if (result.has_value()) {
                std::cout << "División exitosa!" << std::endl;
                auto [quotient, remainder] = result.value();
                std::cout << "Cociente: ";
                quotient.print();
                std::cout << "Resto: ";
                remainder.print();
            } else {
                std::cout << "Error en división" << std::endl;
            }

            // Probar división por cero
            std::cout << "\nProbando división por cero:" << std::endl;
            auto result_zero = a.divrem(zero);
            if (result_zero.has_value()) {
                std::cout << "División exitosa (inesperado!)" << std::endl;
            } else {
                std::cout << "División por cero detectada correctamente" << std::endl;
            }

            return 0;
        }

        // Resultado esperado: 100 / 7 = 14 resto 2
        std::cout << "Resultado esperado: cociente=14, resto=2" << std::endl;

        return 0;
    }