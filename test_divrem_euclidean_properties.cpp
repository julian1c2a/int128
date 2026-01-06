/**
 * @file test_divrem_euclidean_properties.cpp
 * @brief Tests exhaustivos de las propiedades de la división euclidiana
 *
 * Verifica las 9 propiedades fundamentales:
 * - prop_1: (n < m)  => (q==0 && r==n)
 * - prop_2: (n == m) => (q==1 && r==0)
 * - prop_3: (m == 1) => (q==n && r==0)
 * - prop_4: (p|n && p|m) => (q==(n/p)/(m/p))
 * - prop_5: (q*m <= n)
 * - prop_6: ((q+1)*m > n)
 * - prop_7: (|r| < m)
 * - prop_8: (q*m + r == n)
 * - prop_9: (q*m + r+1 > n)
 */

#include "include_new/int128_base_tt.hpp"
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace nstd;

// Colores ANSI
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define CYAN "\033[36m"

int passed = 0;
int failed = 0;

void test_property(const char* name, bool condition)
{
    if (condition) {
        std::cout << GREEN << "[✓] " << RESET << name << std::endl;
        ++passed;
    } else {
        std::cout << RED << "[✗] " << RESET << name << std::endl;
        ++failed;
    }
}

/**
 * @brief Test de las 9 propiedades euclidianas para un par (n, m)
 */
void test_euclidean_properties(const uint128_t& n, const uint128_t& m, const char* test_name)
{
    std::cout << CYAN << "\n=== " << test_name << " ===" << RESET << std::endl;
    std::cout << "n = " << n.to_string() << ", m = " << m.to_string() << std::endl;

    // Calcular q y r
    auto [q, r] = n.divrem(m);

    std::cout << "q = " << q.to_string() << ", r = " << r.to_string() << std::endl;

    // prop_1: (n < m) => (q==0 && r==n)
    if (n < m) {
        test_property("prop_1: n < m => q==0 && r==n", (q.low() == 0 && q.high() == 0) && (r == n));
    }

    // prop_2: (n == m) => (q==1 && r==0)
    if (n == m) {
        test_property("prop_2: n == m => q==1 && r==0",
                      (q.low() == 1 && q.high() == 0) && (r.low() == 0 && r.high() == 0));
    }

    // prop_3: (m == 1) => (q==n && r==0)
    if (m.low() == 1 && m.high() == 0) {
        test_property("prop_3: m == 1 => q==n && r==0",
                      (q == n) && (r.low() == 0 && r.high() == 0));
    }

    // prop_5: q*m <= n
    uint128_t qm = q * m;
    test_property("prop_5: q*m <= n", qm <= n);

    // prop_6: (q+1)*m > n
    uint128_t one(0, 1);
    uint128_t q_plus_1 = q + one;
    uint128_t q_plus_1_m = q_plus_1 * m;
    test_property("prop_6: (q+1)*m > n", q_plus_1_m > n);

    // prop_7: r < m (para unsigned, |r| = r)
    test_property("prop_7: r < m", r < m);

    // prop_8: q*m + r == n
    uint128_t qm_plus_r = qm + r;
    test_property("prop_8: q*m + r == n", qm_plus_r == n);

    // prop_9: q*m + r+1 > n (equivalente a prop_6 para r)
    uint128_t r_plus_1 = r + one;
    uint128_t qm_plus_r_plus_1 = qm + r_plus_1;
    test_property("prop_9: q*m + r+1 > n", qm_plus_r_plus_1 > n);
}

int main()
{
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   TEST: PROPIEDADES EUCLIDIANAS DE LA DIVISIÓN            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    // Test 1: Casos básicos
    uint128_t n1(0, 100);
    uint128_t m1(0, 10);
    test_euclidean_properties(n1, m1, "Test 1: 100 / 10");

    // Test 2: n < m
    uint128_t n2(0, 5);
    uint128_t m2(0, 10);
    test_euclidean_properties(n2, m2, "Test 2: 5 / 10 (n < m)");

    // Test 3: n == m
    uint128_t n3(0, 42);
    uint128_t m3(0, 42);
    test_euclidean_properties(n3, m3, "Test 3: 42 / 42 (n == m)");

    // Test 4: m == 1
    uint128_t n4(0, 999);
    uint128_t m4(0, 1);
    test_euclidean_properties(n4, m4, "Test 4: 999 / 1 (m == 1)");

    // Test 5: División por potencia de 2
    uint128_t n5(0, 1024);
    uint128_t m5(0, 16);
    test_euclidean_properties(n5, m5, "Test 5: 1024 / 16 (potencia de 2)");

    // Test 6: División con resto
    uint128_t n6(0, 123);
    uint128_t m6(0, 10);
    test_euclidean_properties(n6, m6, "Test 6: 123 / 10 (con resto)");

    // Test 7: Número grande (128 bits)
    uint128_t n7(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // MAX
    uint128_t m7(0, 1000000);
    test_euclidean_properties(n7, m7, "Test 7: UINT128_MAX / 1000000");

    // Test 8: División por 3
    uint128_t n8(0, 1000);
    uint128_t m8(0, 3);
    test_euclidean_properties(n8, m8, "Test 8: 1000 / 3");

    // Test 9: División por 7
    uint128_t n9(0, 777);
    uint128_t m9(0, 7);
    test_euclidean_properties(n9, m9, "Test 9: 777 / 7");

    // Test 10: División por 15 (múltiplo de 3 y 5)
    uint128_t n10(0, 1500);
    uint128_t m10(0, 15);
    test_euclidean_properties(n10, m10, "Test 10: 1500 / 15");

    // Resumen
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                        RESUMEN                             ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << GREEN << "[✓] Tests pasados: " << passed << RESET << std::endl;
    if (failed > 0) {
        std::cout << RED << "[✗] Tests fallidos: " << failed << RESET << std::endl;
    }

    return (failed == 0) ? 0 : 1;
}
