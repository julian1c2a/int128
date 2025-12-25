// Test con implementación temporal inline
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>

class uint128_t_simple
{
    uint64_t data[2]; // [0] = low, [1] = high
  public:
    uint128_t_simple(uint64_t h, uint64_t l) : data{l, h} {}

    uint64_t high() const
    {
        return data[1];
    }
    uint64_t low() const
    {
        return data[0];
    }

    uint64_t fullmult_times_uint64(uint64_t multiplier) const noexcept
    {
        // Función para multiplicar 64x64 -> 128 bits
        auto mul64x64 = [](uint64_t a, uint64_t b) -> std::pair<uint64_t, uint64_t> {
            uint64_t a_lo = a & 0xFFFFFFFFULL;
            uint64_t a_hi = a >> 32;
            uint64_t b_lo = b & 0xFFFFFFFFULL;
            uint64_t b_hi = b >> 32;

            uint64_t p0 = a_lo * b_lo;
            uint64_t p1 = a_lo * b_hi;
            uint64_t p2 = a_hi * b_lo;
            uint64_t p3 = a_hi * b_hi;

            uint64_t carry = (p0 >> 32) + (p1 & 0xFFFFFFFFULL) + (p2 & 0xFFFFFFFFULL);
            uint64_t result_low = p0 + ((p1 + p2) << 32);
            uint64_t result_high = p3 + (p1 >> 32) + (p2 >> 32) + (carry >> 32);

            return {result_low, result_high};
        };

        // data[0] * multiplier (bits 0-127 del resultado)
        auto low_mult = mul64x64(data[0], multiplier);

        // data[1] * multiplier (bits 64-191, desplazado 64 hacia la izquierda)
        auto high_mult = mul64x64(data[1], multiplier);

        // Bits 64-127 del resultado = low_mult.second + high_mult.first
        uint64_t sum_bits_64_127 = low_mult.second + high_mult.first;
        uint64_t carry_to_128 = (sum_bits_64_127 < low_mult.second) ? 1 : 0;

        // Bits 128-191 del resultado = high_mult.second + carry_to_128
        return high_mult.second + carry_to_128;
    }
};

int main()
{
    std::cout << "🔍 TEST IMPLEMENTACIÓN SIMPLE TEMPORAL" << std::endl;

    // Test caso básico que falla: (2^128 - 1) * 2 = 1 overflow
    uint128_t_simple large(UINT64_MAX, UINT64_MAX);
    uint64_t result = large.fullmult_times_uint64(2);

    std::cout << "\n=== RESULTADO ===" << std::endl;
    std::cout << "Número: 0x" << std::hex << UINT64_MAX << UINT64_MAX << std::endl;
    std::cout << "Multiplicador: 2" << std::endl;
    std::cout << "Resultado: 0x" << std::hex << result << " = " << std::dec << result << std::endl;
    std::cout << "Esperado: 1" << std::endl;
    std::cout << "¿Correcto?: " << (result == 1 ? "SÍ [OK]" : "NO [ERROR]") << std::endl;

    // Test caso específico que funcionaba antes
    uint128_t_simple test1(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t mult1 = 0x1000000000000000ULL;
    uint64_t result1 = test1.fullmult_times_uint64(mult1);

    std::cout << "\n=== CASO ESPECÍFICO ===" << std::endl;
    std::cout << "Resultado: 0x" << std::hex << std::setfill('0') << std::setw(16) << result1
              << std::endl;

    return 0;
}