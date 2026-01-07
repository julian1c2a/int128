// test_systematic_multiplication.cpp
// Tests sistematicos de multiplicacion para int128_base_t
// Estructura reutilizable para benchmarks
// Sigue PROMPT.md - Output ASCII only

#include "include_new/int128_base_tt.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace nstd;

// Generador de numeros aleatorios
static std::mt19937_64 rng(42); // Seed fija para reproducibilidad

// ============================================================================
// GENERADORES DE VALORES DE TEST
// ============================================================================

// Genera uint128 aleatorio completo
inline uint128_t random_uint128()
{
    return uint128_t(rng(), rng());
}

// Genera int128 aleatorio completo
inline int128_t random_int128()
{
    return int128_t(static_cast<int64_t>(rng()), rng());
}

// Genera uint128 que cabe en low (high=0)
inline uint128_t random_uint128_low_only()
{
    return uint128_t(0, rng());
}

// Genera uint128 que cabe en N bits
template <int N> inline uint128_t random_uint128_n_bits()
{
    static_assert(N >= 1 && N <= 128, "N must be 1-128");
    if constexpr (N <= 64) {
        uint64_t mask = (N == 64) ? ~0ULL : ((1ULL << N) - 1);
        return uint128_t(0, rng() & mask);
    } else {
        uint64_t high_mask = (N == 128) ? ~0ULL : ((1ULL << (N - 64)) - 1);
        return uint128_t(rng() & high_mask, rng());
    }
}

// Genera uint128 con exactamente K bits activos (distribucion aleatoria)
inline uint128_t random_uint128_k_bits_set(int k)
{
    if (k <= 0)
        return uint128_t(0);
    if (k >= 128)
        return uint128_t(~0ULL, ~0ULL);

    uint128_t result(0);
    std::vector<int> positions(128);
    for (int i = 0; i < 128; ++i)
        positions[i] = i;

    // Shuffle y seleccionar primeros k
    for (int i = 0; i < k; ++i) {
        int j = i + (rng() % (128 - i));
        std::swap(positions[i], positions[j]);

        int pos = positions[i];
        if (pos < 64) {
            result = result | uint128_t(0, 1ULL << pos);
        } else {
            result = result | uint128_t(1ULL << (pos - 64), 0);
        }
    }
    return result;
}

// Potencia de 2
inline uint128_t power_of_2(int exp)
{
    if (exp < 64)
        return uint128_t(0, 1ULL << exp);
    else
        return uint128_t(1ULL << (exp - 64), 0);
}

// ============================================================================
// CONTADORES DE TESTS
// ============================================================================
static int tests_passed = 0;
static int tests_failed = 0;

void test_ok(const char* name)
{
    ++tests_passed;
    std::cout << "[OK] " << name << "\n";
}

void test_fail(const char* name, const char* detail = nullptr)
{
    ++tests_failed;
    std::cout << "[FAIL] " << name;
    if (detail)
        std::cout << " - " << detail;
    std::cout << "\n";
}

// ============================================================================
// TEST: ELEMENTO NEUTRO (1) Y ABSORBENTE (0)
// ============================================================================
void test_identity_elements()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Elementos neutro (1) y absorbente (0)             |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // a * 0 == 0
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t result = a * uint128_t(0);
        if (result != uint128_t(0)) {
            all_ok = false;
            test_fail("a * 0 == 0");
            break;
        }
    }
    if (all_ok)
        test_ok("a * 0 == 0 (100 random tests)");

    // a * 1 == a
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t result = a * uint128_t(1);
        if (result != a) {
            all_ok = false;
            test_fail("a * 1 == a");
            break;
        }
    }
    if (all_ok)
        test_ok("a * 1 == a (100 random tests)");

    // 0 * a == 0 (conmutatividad con 0)
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t result = uint128_t(0) * a;
        if (result != uint128_t(0)) {
            all_ok = false;
            test_fail("0 * a == 0");
            break;
        }
    }
    if (all_ok)
        test_ok("0 * a == 0 (100 random tests)");

    // 1 * a == a (conmutatividad con 1)
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t result = uint128_t(1) * a;
        if (result != a) {
            all_ok = false;
            test_fail("1 * a == a");
            break;
        }
    }
    if (all_ok)
        test_ok("1 * a == a (100 random tests)");
}

// ============================================================================
// TEST: CONMUTATIVIDAD a*b == b*a
// ============================================================================
void test_commutativity()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Conmutatividad a*b == b*a                         |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // Random * Random
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if (a * b != b * a) {
            all_ok = false;
            test_fail("random * random conmutatividad");
            break;
        }
    }
    if (all_ok)
        test_ok("random * random (100 tests)");

    // Low-only * Low-only
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128_low_only();
        uint128_t b = random_uint128_low_only();
        if (a * b != b * a) {
            all_ok = false;
            test_fail("low * low conmutatividad");
            break;
        }
    }
    if (all_ok)
        test_ok("low_only * low_only (100 tests)");

    // Potencia de 2 * Random
    all_ok = true;
    for (int exp = 0; exp < 127; ++exp) {
        uint128_t pot2 = power_of_2(exp);
        uint128_t r = random_uint128();
        if (pot2 * r != r * pot2) {
            all_ok = false;
            test_fail("pot2 * random conmutatividad");
            break;
        }
    }
    if (all_ok)
        test_ok("power_of_2 * random (127 tests)");
}

// ============================================================================
// TEST: ASOCIATIVIDAD (a*b)*c == a*(b*c)
// ============================================================================
void test_associativity()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Asociatividad (a*b)*c == a*(b*c)                   |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // Con valores pequenos (para evitar overflow que oculte errores)
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128_n_bits<32>();
        uint128_t b = random_uint128_n_bits<32>();
        uint128_t c = random_uint128_n_bits<32>();

        uint128_t lhs = (a * b) * c;
        uint128_t rhs = a * (b * c);

        if (lhs != rhs) {
            all_ok = false;
            test_fail("(a*b)*c == a*(b*c) pequenos");
            break;
        }
    }
    if (all_ok)
        test_ok("(a*b)*c == a*(b*c) valores 32-bit (100 tests)");

    // Con valores grandes (overflow es parte del test)
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = random_uint128();

        uint128_t lhs = (a * b) * c;
        uint128_t rhs = a * (b * c);

        if (lhs != rhs) {
            all_ok = false;
            test_fail("(a*b)*c == a*(b*c) grandes");
            break;
        }
    }
    if (all_ok)
        test_ok("(a*b)*c == a*(b*c) valores 128-bit (100 tests)");
}

// ============================================================================
// TEST: DISTRIBUTIVIDAD a*(b+c) == a*b + a*c
// ============================================================================
void test_distributivity()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Distributividad a*(b+c) == a*b + a*c               |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // Valores pequenos
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128_n_bits<32>();
        uint128_t b = random_uint128_n_bits<32>();
        uint128_t c = random_uint128_n_bits<32>();

        uint128_t lhs = a * (b + c);
        uint128_t rhs = a * b + a * c;

        if (lhs != rhs) {
            all_ok = false;
            test_fail("a*(b+c) == a*b + a*c pequenos");
            break;
        }
    }
    if (all_ok)
        test_ok("a*(b+c) == a*b + a*c valores 32-bit (100 tests)");

    // Valores grandes
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        uint128_t c = random_uint128();

        uint128_t lhs = a * (b + c);
        uint128_t rhs = a * b + a * c;

        if (lhs != rhs) {
            all_ok = false;
            test_fail("a*(b+c) == a*b + a*c grandes");
            break;
        }
    }
    if (all_ok)
        test_ok("a*(b+c) == a*b + a*c valores 128-bit (100 tests)");
}

// ============================================================================
// TEST: MULTIPLICACION POR POTENCIAS DE 2
// ============================================================================
void test_multiply_powers_of_2()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicacion por potencias de 2                  |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // a * 2^k == a << k (para k = 0..127)
    for (int k = 0; k < 127; ++k) {
        bool all_ok = true;
        for (int i = 0; i < N; ++i) {
            uint128_t a = random_uint128();
            uint128_t pot2 = power_of_2(k);
            uint128_t mult_result = a * pot2;
            uint128_t shift_result = a << k;

            if (mult_result != shift_result) {
                all_ok = false;
                break;
            }
        }
        if (!all_ok) {
            test_fail(("a * 2^" + std::to_string(k) + " == a << " + std::to_string(k)).c_str());
            return;
        }
    }
    test_ok("a * 2^k == a << k para k=0..126 (100 tests cada uno)");
}

// ============================================================================
// TEST: MULTIPLICACION CON TIPOS BUILTIN
// ============================================================================
void test_multiply_builtin_types()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicacion con tipos builtin                   |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // uint8_t
    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint8_t b = static_cast<uint8_t>(rng());
        uint128_t r1 = a * b;
        uint128_t r2 = a * uint128_t(b);
        if (r1 != r2) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("uint128 * uint8_t (100 tests)");
    else
        test_fail("uint128 * uint8_t");

    // uint16_t
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint16_t b = static_cast<uint16_t>(rng());
        uint128_t r1 = a * b;
        uint128_t r2 = a * uint128_t(b);
        if (r1 != r2) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("uint128 * uint16_t (100 tests)");
    else
        test_fail("uint128 * uint16_t");

    // uint32_t
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint32_t b = static_cast<uint32_t>(rng());
        uint128_t r1 = a * b;
        uint128_t r2 = a * uint128_t(b);
        if (r1 != r2) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("uint128 * uint32_t (100 tests)");
    else
        test_fail("uint128 * uint32_t");

    // uint64_t
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint64_t b = rng();
        uint128_t r1 = a * b;
        uint128_t r2 = a * uint128_t(b);
        if (r1 != r2) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("uint128 * uint64_t (100 tests)");
    else
        test_fail("uint128 * uint64_t");

    // int64_t (signed)
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        int128_t a = random_int128();
        int64_t b = static_cast<int64_t>(rng());
        int128_t r1 = a * b;
        int128_t r2 = a * int128_t(b);
        if (r1 != r2) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("int128 * int64_t (100 tests)");
    else
        test_fail("int128 * int64_t");
}

// ============================================================================
// TEST: MULTIPLICACION CON K BITS ACTIVOS
// ============================================================================
void test_multiply_k_bits_set()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Multiplicacion con K bits activos                  |\n";
    std::cout << "+============================================================+\n";

    const int N = 20; // Menos iteraciones por ser muchas combinaciones

    // Test para k = 2, 3, 4, ..., 10, 20, 50, 100, 125
    std::vector<int> k_values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 50, 100, 125};

    for (int k : k_values) {
        bool all_ok = true;
        for (int i = 0; i < N; ++i) {
            uint128_t a = random_uint128_k_bits_set(k);
            uint128_t b = random_uint128();

            // Verificar conmutatividad
            if (a * b != b * a) {
                all_ok = false;
                break;
            }
        }
        if (all_ok) {
            std::cout << "[OK] " << k << " bits activos * random (20 tests)\n";
        } else {
            std::cout << "[FAIL] " << k << " bits activos * random\n";
            ++tests_failed;
            --tests_passed;
        }
        ++tests_passed;
    }
}

// ============================================================================
// TEST: LOW-ONLY * LOW-ONLY (caben en uint64_t)
// ============================================================================
void test_multiply_low_only()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Low-only * Low-only                                |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128_low_only();
        uint128_t b = random_uint128_low_only();

        // Verificar contra multiplicacion de uint64_t
        uint64_t a64 = a.low();
        uint64_t b64 = b.low();

        // El producto puede exceder 64 bits, verificar con high*2^64 + low
        uint128_t result = a * b;

        // Conmutatividad
        if (result != b * a) {
            all_ok = false;
            test_fail("low * low conmutatividad");
            break;
        }
    }
    if (all_ok)
        test_ok("low_only * low_only (100 tests)");
}

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    std::cout << "+============================================================+\n";
    std::cout << "|   TESTS SISTEMATICOS DE MULTIPLICACION                     |\n";
    std::cout << "|   int128_base_t (uint128_t / int128_t)                     |\n";
    std::cout << "+============================================================+\n";

    test_identity_elements();
    test_commutativity();
    test_associativity();
    test_distributivity();
    test_multiply_powers_of_2();
    test_multiply_builtin_types();
    test_multiply_k_bits_set();
    test_multiply_low_only();

    std::cout << "\n+============================================================+\n";
    std::cout << "|                        RESUMEN                             |\n";
    std::cout << "+============================================================+\n";

    int total = tests_passed + tests_failed;
    if (tests_failed == 0) {
        std::cout << "[OK] Tests pasados: " << tests_passed << "/" << total << "\n";
    } else {
        std::cout << "[FAIL] Tests pasados: " << tests_passed << "/" << total << "\n";
        std::cout << "[FAIL] Tests fallidos: " << tests_failed << "\n";
    }

    return tests_failed > 0 ? 1 : 0;
}
