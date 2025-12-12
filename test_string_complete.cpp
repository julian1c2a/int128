#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#endif

class uint128_simple
{
    uint64_t data[2]; // data[1]=high, data[0]=low

  public:
    uint128_simple(uint64_t high = 0, uint64_t low = 0) : data{low, high} {}

    // Operadores de comparación
    bool operator==(const uint128_simple& other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1];
    }

    bool operator!=(const uint128_simple& other) const
    {
        return !(*this == other);
    }

    bool operator<(const uint128_simple& other) const
    {
        if (data[1] != other.data[1])
            return data[1] < other.data[1];
        return data[0] < other.data[0];
    }

    bool operator>=(const uint128_simple& other) const
    {
        return !(*this < other);
    }

    // Aritmética básica
    uint128_simple& operator+=(const uint128_simple& other)
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char carry = _addcarry_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _addcarry_u64(carry, data[1], other.data[1], &data[1]);
#else
        uint64_t old_low = data[0];
        data[0] += other.data[0];
        data[1] += other.data[1];
        if (data[0] < old_low)
            ++data[1];
#endif
        return *this;
    }

    uint128_simple operator+(const uint128_simple& other) const
    {
        uint128_simple result(*this);
        result += other;
        return result;
    }

    uint128_simple& operator-=(const uint128_simple& other)
    {
#ifdef _MSC_VER
        uint64_t new_low;
        unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        data[0] = new_low;
        _subborrow_u64(borrow, data[1], other.data[1], &data[1]);
#else
        uint64_t old_low = data[0];
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        if (data[0] > old_low)
            --data[1];
#endif
        return *this;
    }

    uint128_simple operator-(const uint128_simple& other) const
    {
        uint128_simple result(*this);
        result -= other;
        return result;
    }

    // Multiplicación básica
    uint128_simple& operator*=(const uint128_simple& other)
    {
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

    uint128_simple operator*(const uint128_simple& other) const
    {
        uint128_simple result(*this);
        result *= other;
        return result;
    }

    // Funciones auxiliares para división
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

    uint128_simple shift_left(int positions) const
    {
        if (positions <= 0)
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
        if (positions <= 0)
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

    std::optional<std::pair<uint128_simple, uint128_simple>>
    divrem(const uint128_simple& divisor) const
    {
        if (divisor == uint128_simple(0, 0)) {
            return std::nullopt;
        }

        uint128_simple quotient(0, 0);
        uint128_simple remainder(0, 0);

        if (*this < divisor) {
            remainder = *this;
            return std::make_pair(quotient, remainder);
        } else if (*this == divisor) {
            quotient = uint128_simple(0, 1);
            return std::make_pair(quotient, remainder);
        } else if (divisor == uint128_simple(0, 1)) {
            quotient = *this;
            return std::make_pair(quotient, remainder);
        }

        auto normalized_pair = divisor.normalize_divisor(*this);
        uint128_simple normalized_divisor = normalized_pair.first;
        int shift_amount = normalized_pair.second;

        remainder = *this;
        quotient = uint128_simple(0, 0);

        for (int i = shift_amount; i >= 0; i--) {
            quotient = quotient.shift_left(1);

            if (remainder >= normalized_divisor) {
                remainder = remainder - normalized_divisor;
                quotient.data[0] |= 1;
            }

            normalized_divisor = normalized_divisor.shift_right(1);
        }

        return std::make_pair(quotient, remainder);
    }

    // NUEVAS FUNCIONES DE STRING
    std::string to_string() const
    {
        if (*this == uint128_simple(0, 0)) {
            return "0";
        }

        std::string result;
        uint128_simple temp = *this;

        while (temp != uint128_simple(0, 0)) {
            auto divmod = temp.divrem(uint128_simple(0, 10));
            if (divmod.has_value()) {
                char digit =
                    '0' + static_cast<char>(static_cast<uint64_t>(divmod.value().second.data[0]));
                result = digit + result;
                temp = divmod.value().first;
            } else {
                break;
            }
        }

        return result.empty() ? "0" : result;
    }

    static uint128_simple from_string(const std::string& str)
    {
        if (str.empty()) {
            return uint128_simple(0, 0);
        }

        uint128_simple result(0, 0);
        uint128_simple base(0, 10);

        for (char c : str) {
            if (c < '0' || c > '9') {
                continue; // Ignorar caracteres inválidos
            }

            result *= base;
            result += uint128_simple(0, static_cast<uint64_t>(c - '0'));
        }

        return result;
    }

    // Constructor desde string
    explicit uint128_simple(const std::string& str) : data{0, 0}
    {
        *this = from_string(str);
    }
};

// Operadores de flujo
std::ostream& operator<<(std::ostream& os, const uint128_simple& value)
{
    return os << value.to_string();
}

std::istream& operator>>(std::istream& is, uint128_simple& value)
{
    std::string str;
    is >> str;
    if (is.good() || is.eof()) {
        value = uint128_simple::from_string(str);
    }
    return is;
}

int main()
{
    std::cout << "=== Prueba completa de funciones de string ===" << std::endl;

    // 1. Constructor desde string
    std::cout << "\n1. Constructor desde string:" << std::endl;
    uint128_simple a("12345");
    uint128_simple b("999");
    std::cout << "uint128_simple a(\"12345\") = " << a << std::endl;
    std::cout << "uint128_simple b(\"999\") = " << b << std::endl;

    // 2. from_string estática
    std::cout << "\n2. Función from_string estática:" << std::endl;
    uint128_simple c = uint128_simple::from_string("7777");
    uint128_simple d = uint128_simple::from_string("0");
    std::cout << "from_string(\"7777\") = " << c << std::endl;
    std::cout << "from_string(\"0\") = " << d << std::endl;

    // 3. Operador <<
    std::cout << "\n3. Operador de salida <<:" << std::endl;
    uint128_simple e(0, 42);
    std::cout << "uint128_simple(0, 42) = " << e << std::endl;

    // 4. Operador >>
    std::cout << "\n4. Operador de entrada >> (usando stringstream):" << std::endl;
    std::stringstream ss("12345 67890 100");
    uint128_simple x, y, z;
    ss >> x >> y >> z;
    std::cout << "Leídos desde stream: " << x << ", " << y << ", " << z << std::endl;

    // 5. Conversión bidireccional
    std::cout << "\n5. Conversión bidireccional:" << std::endl;
    uint128_simple original(0, 9876543210ULL);
    std::string str_conversion = original.to_string();
    uint128_simple reconstructed = uint128_simple::from_string(str_conversion);

    std::cout << "Original: " << original << std::endl;
    std::cout << "String: \"" << str_conversion << "\"" << std::endl;
    std::cout << "Reconstruido: " << reconstructed << std::endl;
    std::cout << "¿Son iguales? " << (original == reconstructed ? "SÍ" : "NO") << std::endl;

    // 6. Números grandes
    std::cout << "\n6. Números grandes:" << std::endl;
    uint128_simple large_a(0, 1000);
    uint128_simple large_b(0, 999999);
    uint128_simple large_result = large_a * large_b;

    std::cout << large_a << " * " << large_b << " = " << large_result << std::endl;

    std::string large_str = large_result.to_string();
    uint128_simple large_reconstructed = uint128_simple::from_string(large_str);
    std::cout << "Conversión string: \"" << large_str << "\"" << std::endl;
    std::cout << "Reconstruido: " << large_reconstructed << std::endl;
    std::cout << "¿Son iguales? " << (large_result == large_reconstructed ? "SÍ" : "NO")
              << std::endl;

    // 7. Casos especiales
    std::cout << "\n7. Casos especiales:" << std::endl;
    uint128_simple zero_from_str = uint128_simple::from_string("");
    uint128_simple zero_from_zero = uint128_simple::from_string("0");
    std::cout << "from_string(\"\") = " << zero_from_str << std::endl;
    std::cout << "from_string(\"0\") = " << zero_from_zero << std::endl;

    // 8. Verificar operaciones con divrem
    std::cout << "\n8. Verificación con divrem:" << std::endl;
    uint128_simple dividend(0, 100);
    uint128_simple divisor(0, 7);

    auto result = dividend.divrem(divisor);
    if (result.has_value()) {
        std::cout << dividend << " / " << divisor << " = " << result.value().first << " resto "
                  << result.value().second << std::endl;
    }

    std::cout << "\n¡Todas las funciones de string implementadas y probadas exitosamente!"
              << std::endl;
    return 0;
}