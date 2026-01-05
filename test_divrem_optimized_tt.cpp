/**
 * @file test_divrem_optimized_tt.cpp
 * @brief Tests de división optimizada para int128_base_tt.hpp
 *
 * Verifica las optimizaciones implementadas:
 * [1] Potencias de 2, 3, 5 y divisores hasta 15
 * [3] Ambos valores en 64 bits
 * [2] Divisor de 64 bits con dividendo de 128 bits
 * [0] Caso general 128/128
 */

#include "include_new/int128_base_tt.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

// Helper para imprimir resultados
template <signedness S>
void print_divrem(const int128_base_t<S>& dividend, const int128_base_t<S>& divisor)
{
    auto [q, r] = dividend.divrem(divisor);
    std::cout << "  " << dividend.to_string() << " / " << divisor.to_string() << " = "
              << q.to_string() << " remainder " << r.to_string() << "\n";

    // Verificar: dividend == q * divisor + r
    auto check = q * divisor + r;
    assert(check == dividend && "divrem verification failed!");
}

int main()
{
    std::cout << "=== Tests de División Optimizada int128_base_tt.hpp ===\n\n";

    // Usar uint128_t como alias del template
    using uint128_t = int128_base_t<signedness::unsigned_type>;

    // [1] Potencias de 2 (shift optimizado)
    std::cout << "[1] Potencias de 2 (shift):\n";
    uint128_t big1(0x123456789ABCDEFULL, 0xFEDCBA9876543210ULL);
    print_divrem(big1, uint128_t(0, 2));    // /2
    print_divrem(big1, uint128_t(0, 4));    // /4
    print_divrem(big1, uint128_t(0, 8));    // /8
    print_divrem(big1, uint128_t(0, 16));   // /16
    print_divrem(big1, uint128_t(0, 256));  // /256
    print_divrem(big1, uint128_t(0, 1024)); // /1024

    // [1] Divisores específicos (3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15)
    std::cout << "\n[1] Divisores específicos hasta 15:\n";
    uint128_t medium(0, 1000000);
    print_divrem(medium, uint128_t(0, 3));
    print_divrem(medium, uint128_t(0, 5));
    print_divrem(medium, uint128_t(0, 6));
    print_divrem(medium, uint128_t(0, 7));
    print_divrem(medium, uint128_t(0, 9));
    print_divrem(medium, uint128_t(0, 10));
    print_divrem(medium, uint128_t(0, 11));
    print_divrem(medium, uint128_t(0, 12));
    print_divrem(medium, uint128_t(0, 13));
    print_divrem(medium, uint128_t(0, 14));
    print_divrem(medium, uint128_t(0, 15));

    // [3] Ambos valores en 64 bits (división nativa CPU)
    std::cout << "\n[3] Ambos valores en 64 bits:\n";
    uint128_t small_dividend(0, 12345678);
    uint128_t small_divisor(0, 123);
    print_divrem(small_dividend, small_divisor);

    // [2] Divisor de 64 bits, dividendo de 128 bits
    std::cout << "\n[2] Divisor 64 bits, dividendo 128 bits:\n";
    uint128_t big_dividend(0x100, 0); // 2^64 * 256
    uint128_t divisor_64(0, 17);
    print_divrem(big_dividend, divisor_64);

    // [0] Caso general: división 128/128
    std::cout << "\n[0] Caso general 128/128 bits:\n";
    uint128_t very_big1(0x8000000000000000ULL, 0);
    uint128_t very_big2(0x4000000000000000ULL, 0);
    print_divrem(very_big1, very_big2);

    // Test de potencias de 10
    std::cout << "\n[Bonus] Potencias de 10:\n";
    uint128_t val(0, 123456789012345ULL);
    print_divrem(val, uint128_t(0, 10));
    print_divrem(val, uint128_t(0, 100));
    print_divrem(val, uint128_t(0, 1000));
    print_divrem(val, uint128_t(0, 10000));

    std::cout << "\n✅ Todos los tests de división optimizada pasaron!\n";
    return 0;
}
