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

    uint128_simple operator-(const uint128_simple& other) const
    {
        uint64_t new_low, new_high;

#ifdef _MSC_VER
        unsigned char borrow = _subborrow_u64(0, data[0], other.data[0], &new_low);
        _subborrow_u64(borrow, data[1], other.data[1], &new_high);
#else
        if (data[0] >= other.data[0]) {
            new_low = data[0] - other.data[0];
            new_high = data[1] - other.data[1];
        } else {
            new_low = data[0] - other.data[0];
            new_high = data[1] - other.data[1] - 1;
        }
#endif
        return uint128_simple(new_high, new_low);
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
        if (divisor.is_zero()) {
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
};

int main()
{
    std::cout << "=== Prueba de integración de divrem ===" << std::endl;

    uint128_simple a(0, 100);
    uint128_simple b(0, 7);

    std::cout << "Dividendo: 100, Divisor: 7" << std::endl;

    auto result = a.divrem(b);
    if (result.has_value()) {
        auto quotient = result.value().first;
        auto remainder = result.value().second;

        std::cout << "Resultado: " << quotient.data[0] << " resto " << remainder.data[0]
                  << std::endl;

        // Pruebas adicionales
        uint128_simple x(0, 1000);
        uint128_simple y(0, 13);
        auto result2 = x.divrem(y);
        if (result2.has_value()) {
            auto q = result2.value().first;
            auto r = result2.value().second;
            std::cout << "1000 / 13 = " << q.data[0] << " resto " << r.data[0] << std::endl;
            std::cout << "Verificación: 13 * " << q.data[0] << " + " << r.data[0] << " = "
                      << (13 * q.data[0] + r.data[0]) << std::endl;
        }

    } else {
        std::cout << "Error en división" << std::endl;
    }

    std::cout << "\n¡Algoritmo divrem listo para integrar!" << std::endl;
    return 0;
}