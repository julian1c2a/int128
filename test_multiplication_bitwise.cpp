/**
 * @file test_multiplication_bitwise.cpp
 * @brief Verifica que la multiplicación es idéntica a nivel de bits para todos los tipos
 *
 * Según el teorema de complemento a 2:
 *   uint128_t * uint128_t, uint128_t * int128_t,
 *   int128_t * uint128_t, int128_t * int128_t
 * producen el MISMO resultado a nivel de bits (los 128 bits inferiores).
 */

#include "include_new/int128_base_tt.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>

using namespace nstd;

// Contadores globales
int tests_passed = 0;
int tests_failed = 0;

void check(bool condition, const char* test_name)
{
    if (condition) {
        std::cout << "[OK] " << test_name << "\n";
        ++tests_passed;
    } else {
        std::cout << "[FAIL] " << test_name << " FAILED\n";
        ++tests_failed;
    }
}

// Comparar dos valores a nivel de bits (ignorando tipo)
template <typename T1, typename T2> bool bits_equal(const T1& a, const T2& b)
{
    static_assert(sizeof(T1) == sizeof(T2), "Sizes must match");
    return std::memcmp(&a, &b, sizeof(T1)) == 0;
}

// ============================================================================
// TESTS DE MULTIPLICACIÓN - VERIFICAR IDENTIDAD DE BITS
// ============================================================================

void test_multiplication_types()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicación idéntica a nivel de bits            |\n";
    std::cout << "+============================================================+\n\n";

    // Caso 1: Valores pequeños positivos
    {
        uint128_t ua(12ull), ub(34ull);
        int128_t sa(12ll), sb(34ll);

        uint128_t r_uu = ua * ub;
        uint128_t r_us = ua * static_cast<uint128_t>(sb);
        int128_t r_ss = sa * sb;
        int128_t r_su = sa * static_cast<int128_t>(ub);

        // Todos deberían ser 408
        check(r_uu == uint128_t(408ull), "12*34 = 408 (u*u)");
        check(r_ss == int128_t(408ll), "12*34 = 408 (s*s)");
        check(bits_equal(r_uu, r_ss), "u*u == s*s a nivel de bits");
    }

    // Caso 2: Valores que causan overflow en 64 bits
    {
        uint128_t ua(0xFFFFFFFFFFFFFFFFull); // UINT64_MAX
        uint128_t ub(2ull);
        int128_t sa(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFull)); // -1 en signed
        int128_t sb(2ll);

        uint128_t r_uu = ua * ub; // Should be 2*UINT64_MAX = 0x1_FFFFFFFFFFFFFFFE
        int128_t r_ss = sa * sb;  // -1 * 2 = -2

        // El resultado unsigned es 2*UINT64_MAX
        check(r_uu == uint128_t(1ull, 0xFFFFFFFFFFFFFFFEull), "UINT64_MAX * 2 (unsigned)");

        // El resultado signed es -2 (complemento a 2)
        check(r_ss == int128_t(-2ll), "-1 * 2 = -2 (signed)");

        // A nivel de bits, -2 en complemento a 2 es diferente a 2*UINT64_MAX
        // Esto es ESPERADO porque los valores de entrada son diferentes
    }

    // Caso 3: Verificar que el producto de los MISMOS bits da el MISMO resultado
    {
        // Crear dos números con los mismos bits
        uint128_t ua(0x123456789ABCDEFull, 0xFEDCBA9876543210ull);
        uint128_t ub(0xAAAABBBBCCCCDDDDull);

        // Reinterpretar como signed (sin cambiar los bits)
        int128_t sa;
        std::memcpy(&sa, &ua, sizeof(ua));
        int128_t sb;
        std::memcpy(&sb, &ub, sizeof(ub));

        uint128_t r_uu = ua * ub;
        int128_t r_ss = sa * sb;

        // Los 128 bits inferiores DEBEN ser idénticos
        check(bits_equal(r_uu, r_ss), "Mismos bits de entrada → mismos bits de salida");
    }
}

void test_multiplication_random()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicación aleatoria - verificar bits          |\n";
    std::cout << "+============================================================+\n\n";

    std::mt19937_64 rng(42); // Semilla fija para reproducibilidad

    int random_passed = 0;
    const int NUM_TESTS = 100;

    for (int i = 0; i < NUM_TESTS; ++i) {
        // Generar valores aleatorios
        uint64_t a_hi = rng();
        uint64_t a_lo = rng();
        uint64_t b_hi = rng();
        uint64_t b_lo = rng();

        // Crear versiones unsigned y signed con los mismos bits
        uint128_t ua(a_hi, a_lo);
        uint128_t ub(b_hi, b_lo);

        int128_t sa;
        std::memcpy(&sa, &ua, sizeof(ua));
        int128_t sb;
        std::memcpy(&sb, &ub, sizeof(ub));

        // Multiplicar
        uint128_t r_uu = ua * ub;
        int128_t r_ss = sa * sb;

        // Verificar que los bits son idénticos
        if (bits_equal(r_uu, r_ss)) {
            ++random_passed;
        } else {
            std::cout << "[FAIL] Test " << i << " falló\n";
            std::cout << "    a = " << ua.to_string() << "\n";
            std::cout << "    b = " << ub.to_string() << "\n";
            std::cout << "    u*u = " << r_uu.to_string() << "\n";
            std::cout << "    s*s = " << r_ss.to_string() << "\n";
        }
    }

    check(random_passed == NUM_TESTS,
          ("Random tests: " + std::to_string(random_passed) + "/" + std::to_string(NUM_TESTS))
              .c_str());
}

void test_multiplication_edge_cases()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicación - casos extremos                    |\n";
    std::cout << "+============================================================+\n\n";

    // Caso 1: Multiplicación por 0
    {
        uint128_t u(12345ull);
        int128_t s(12345ll);

        check((u * uint128_t(0ull)) == uint128_t(0ull), "u * 0 = 0");
        check((s * int128_t(0ll)) == int128_t(0ll), "s * 0 = 0");
    }

    // Caso 2: Multiplicación por 1
    {
        uint128_t u(0xABCDEF123456789Aull, 0xFEDCBA9876543210ull);
        int128_t s;
        std::memcpy(&s, &u, sizeof(u));

        check((u * uint128_t(1ull)) == u, "u * 1 = u");
        check((s * int128_t(1ll)) == s, "s * 1 = s");
    }

    // Caso 3: UINT128_MAX * 1
    {
        uint128_t max = uint128_t::max();
        check((max * uint128_t(1ull)) == max, "UINT128_MAX * 1 = UINT128_MAX");
    }

    // Caso 4: Cuadrado de potencia de 2
    {
        uint128_t pow2(1ull << 32);     // 2^32
        uint128_t result = pow2 * pow2; // 2^64
        check(result == uint128_t(1ull, 0ull), "(2^32)^2 = 2^64");
    }

    // Caso 5: Overflow que produce 0 (mod 2^128)
    {
        // 2^127 * 2 = 2^128 = 0 (mod 2^128)
        uint128_t pow127(1ull << 63, 0ull); // 2^127
        uint128_t result = pow127 * uint128_t(2ull);
        check(result == uint128_t(0ull), "2^127 * 2 = 0 (overflow)");
    }

    // Caso 6: INT128_MIN * 1 = INT128_MIN
    {
        int128_t min_val = int128_t::min();
        int128_t result = min_val * int128_t(1ll);
        check(result == min_val, "INT128_MIN * 1 = INT128_MIN");
    }

    // Caso 7: -1 * -1 = 1
    {
        int128_t neg1(-1ll);
        int128_t result = neg1 * neg1;
        check(result == int128_t(1ll), "(-1) * (-1) = 1");
    }

    // Caso 8: Verificar conmutatividad
    {
        uint128_t a(0x123456789ABCDEFull, 0xFEDCBA9876543210ull);
        uint128_t b(0xAAAABBBBCCCCDDDDull, 0x1111222233334444ull);

        check((a * b) == (b * a), "a * b == b * a (conmutatividad)");
    }
}

void test_multiplication_with_smaller_types()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicación con tipos más pequeños              |\n";
    std::cout << "+============================================================+\n\n";

    // uint128_t * uint64_t
    {
        uint128_t a(0x1000000000000000ull, 0x2000000000000000ull);
        uint64_t b = 3ull;
        uint128_t expected = a * uint128_t(b);
        uint128_t result = a * b;
        check(result == expected, "uint128_t * uint64_t");
    }

    // int128_t * int64_t
    {
        int128_t a(1000000ll);
        int64_t b = -5ll;
        int128_t result = a * b;
        check(result == int128_t(-5000000ll), "int128_t * int64_t negativo");
    }

    // uint128_t * int32_t (extensión de signo)
    {
        uint128_t a(100ull);
        int32_t b = -1;
        // -1 se extiende a int128_t y luego se convierte a uint128_t
        // El resultado depende de cómo se maneje la conversión
    }

    // Verificar que la multiplicación con uint64_t es igual a uint128_t
    {
        uint128_t a(0xABCDEF1234567890ull, 0x1234567890ABCDEFull);
        uint64_t b = 0xDEADBEEFCAFEBABEull;

        uint128_t r1 = a * b;            // usando operador con uint64_t
        uint128_t r2 = a * uint128_t(b); // usando operador con uint128_t

        check(r1 == r2, "a * uint64_t == a * uint128_t(uint64_t)");
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "+============================================================+\n";
    std::cout << "|   TESTS DE MULTIPLICACIÓN - IDENTIDAD DE BITS              |\n";
    std::cout << "|   Verifica que la multiplicación es idéntica para          |\n";
    std::cout << "|   uint128_t y int128_t cuando los bits son los mismos      |\n";
    std::cout << "+============================================================+\n";

    test_multiplication_types();
    test_multiplication_random();
    test_multiplication_edge_cases();
    test_multiplication_with_smaller_types();

    std::cout << "\n+============================================================+\n";
    std::cout << "|                        RESUMEN                             |\n";
    std::cout << "+============================================================+\n";

    std::cout << "[";
    if (tests_failed == 0) {
        std::cout << "OK";
    } else {
        std::cout << "FAIL";
    }
    std::cout << "] Tests pasados: " << tests_passed << "/" << (tests_passed + tests_failed)
              << "\n";

    return tests_failed > 0 ? 1 : 0;
}
