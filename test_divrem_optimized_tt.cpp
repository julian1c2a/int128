/**
 * @file test_divrem_optimized_tt.cpp
 * @brief Tests de división optimizada para int128_base_tt.hpp
 *
 * Verifica las optimizaciones implementadas:
 * [1] Potencias de 2, 3, 5 y divisores hasta 15
 * [3] Factorización de potencias de 2 comunes
 * [2] Divisor de 64 bits con dividendo de 128 bits
 * [0] Caso general 128/128
 *
 * **TESTS DE PROPIEDADES EUCLIDIANAS:**
 * Para todo n (dividendo) y m (divisor != 0):
 *   q = n / m (cociente)
 *   r = n % m (resto)
 *
 * prop_1: (n < m)  => (q==0 && r==n)
 * prop_2: (n == m) => (q==1 && r==0)
 * prop_3: (m == 1) => (q==n && r==0)
 * prop_4: (p|n && p|m) => (q==(n/p)/(m/p))      [factorización común]
 * prop_5: (q*m <= n)                             [cociente no excede]
 * prop_6: ((q+1)*m > n)                          [cociente es máximo]
 * prop_7: (r < m)                                [resto menor que divisor]
 * prop_8: (q*m + r == n)                         [división euclidiana]
 * prop_9: (q*m + r+1 > n)                        [resto es mínimo]
 */

#include "include_new/int128_base_tt.hpp"
#include <cassert>
#include <iomanip>
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

// Test de propiedades euclidianas
template <signedness S>
void test_euclidean_properties(const int128_base_t<S>& n, const int128_base_t<S>& m,
                               const char* test_name)
{
    using int128 = int128_base_t<S>;

    std::cout << "\n--- Euclidean Properties Test: " << test_name << " ---\n";
    std::cout << "n (dividend) = " << n.to_string() << "\n";
    std::cout << "m (divisor)  = " << m.to_string() << "\n";

    auto [q, r] = n.divrem(m);
    std::cout << "q (quotient) = " << q.to_string() << "\n";
    std::cout << "r (remainder)= " << r.to_string() << "\n";

    int128 zero(0ull, 0ull);
    int128 one(0ull, 1ull);

    // prop_1: (n < m) => (q==0 && r==n)
    if (n < m) {
        assert(q == zero && "prop_1 failed: q should be 0");
        assert(r == n && "prop_1 failed: r should be n");
        std::cout << "OK prop_1: (n < m) => (q==0 && r==n)\n";
    }

    // prop_2: (n == m) => (q==1 && r==0)
    if (n == m) {
        assert(q == one && "prop_2 failed: q should be 1");
        assert(r == zero && "prop_2 failed: r should be 0");
        std::cout << "OK prop_2: (n == m) => (q==1 && r==0)\n";
    }

    // prop_3: (m == 1) => (q==n && r==0)
    if (m == one) {
        assert(q == n && "prop_3 failed: q should be n");
        assert(r == zero && "prop_3 failed: r should be 0");
        std::cout << "OK prop_3: (m == 1) => (q==n && r==0)\n";
    }

    // prop_4: (p|n && p|m) => (q==(n/p)/(m/p)) - testeado separadamente con GCD

    // prop_5: (q*m <= n)
    int128 qm = q * m;
    assert(qm <= n && "prop_5 failed: q*m should be <= n");
    std::cout << "OK prop_5: (q*m <= n)\n";

    // prop_6: ((q+1)*m > n)
    int128 q_plus_1 = q + one;
    int128 q_plus_1_m = q_plus_1 * m;
    assert(q_plus_1_m > n && "prop_6 failed: (q+1)*m should be > n");
    std::cout << "OK prop_6: ((q+1)*m > n)\n";

    // prop_7: (r < m)
    assert(r < m && "prop_7 failed: r should be < m");
    std::cout << "OK prop_7: (r < m)\n";

    // prop_8: (q*m + r == n) - División euclidiana fundamental
    int128 reconstructed = q * m + r;
    assert(reconstructed == n && "prop_8 failed: q*m + r should equal n");
    std::cout << "OK prop_8: (q*m + r == n)\n";

    // prop_9: (q*m + r+1 > n) - Resto es mínimo
    int128 r_plus_1 = r + one;
    int128 qm_plus_r_plus_1 = q * m + r_plus_1;
    assert(qm_plus_r_plus_1 > n && "prop_9 failed: q*m + r+1 should be > n");
    std::cout << "OK prop_9: (q*m + r+1 > n)\n";

    std::cout << "✅ All euclidean properties verified for " << test_name << "\n";
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

    // [3] Factorización de potencias de 2 comunes
    std::cout << "\n[3] Factorización de potencias de 2 comunes:\n";
    uint128_t n1(0, 1024 * 7); // 7168 = 2^10 * 7
    uint128_t m1(0, 64 * 3);   // 192 = 2^6 * 3
    print_divrem(n1, m1);      // Debería simplificar a (7*16)/3 = 37 con resto 1*64 = 64

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

    // ========================================================================
    // [0] TESTS DE PROPIEDADES EUCLIDIANAS
    // ========================================================================

    std::cout << "\n\n=== Tests de Propiedades Euclidianas ===\n";

    // Test 1: n < m (prop_1)
    test_euclidean_properties(uint128_t(0, 42), uint128_t(0, 100), "n < m");

    // Test 2: n == m (prop_2)
    test_euclidean_properties(uint128_t(0, 777), uint128_t(0, 777), "n == m");

    // Test 3: m == 1 (prop_3)
    test_euclidean_properties(uint128_t(0, 12345), uint128_t(0, 1), "m == 1");

    // Test 4: División normal con resto
    test_euclidean_properties(uint128_t(0, 100), uint128_t(0, 7), "100/7");

    // Test 5: Número grande / número pequeño
    test_euclidean_properties(uint128_t(0x1000, 0), uint128_t(0, 256), "big/small");

    // Test 6: Potencias de 2
    test_euclidean_properties(uint128_t(0, 1024), uint128_t(0, 16), "power of 2");

    // Test 7: Con factores comunes de 2 (prop_4 indirectamente)
    test_euclidean_properties(uint128_t(0, 1024 * 7), uint128_t(0, 64 * 3), "common factor 2^6");

    std::cout << "\n✅ Todos los tests de división optimizada pasaron!\n";
    return 0;
}
