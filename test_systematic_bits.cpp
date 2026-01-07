// test_systematic_bits.cpp
// Tests sistematicos de funciones de bits para int128_base_t
// Estructura reutilizable para benchmarks
// Sigue PROMPT.md - Output ASCII only

#include "include_new/int128_base_tt.hpp"
#include <bit>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace nstd;

// Generador de numeros aleatorios
static std::mt19937_64 rng(42);

// ============================================================================
// GENERADORES DE VALORES DE TEST
// ============================================================================

inline uint128_t random_uint128()
{
    return uint128_t(rng(), rng());
}

inline uint128_t random_uint128_nonzero()
{
    uint128_t r;
    do {
        r = uint128_t(rng(), rng());
    } while (r == uint128_t(0));
    return r;
}

inline uint128_t power_of_2(int exp)
{
    if (exp < 64)
        return uint128_t(0, 1ULL << exp);
    else
        return uint128_t(1ULL << (exp - 64), 0);
}

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
// Funciones de referencia (calculo manual)
// ============================================================================

// Count leading zeros (reference)
int reference_clz(uint128_t v)
{
    if (v == uint128_t(0))
        return 128;
    if (v.high() != 0) {
        return std::countl_zero(v.high());
    } else {
        return 64 + std::countl_zero(v.low());
    }
}

// Count trailing zeros (reference)
int reference_ctz(uint128_t v)
{
    if (v == uint128_t(0))
        return 128;
    if (v.low() != 0) {
        return std::countr_zero(v.low());
    } else {
        return 64 + std::countr_zero(v.high());
    }
}

// Popcount (reference)
int reference_popcount(uint128_t v)
{
    return std::popcount(v.high()) + std::popcount(v.low());
}

// is_power_of_2 (reference)
bool reference_is_power_of_2(uint128_t v)
{
    return v != uint128_t(0) && (v & (v - uint128_t(1))) == uint128_t(0);
}

// ============================================================================
// TEST: LEADING ZEROS
// ============================================================================
void test_leading_zeros()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Leading Zeros (CLZ)                                |\n";
    std::cout << "+============================================================+\n";

    // Caso especial: 0
    if (uint128_t(0).leading_zeros() == 128) {
        test_ok("clz(0) == 128");
    } else {
        test_fail("clz(0) == 128");
    }

    // Potencias de 2
    bool all_ok = true;
    for (int i = 0; i < 128; ++i) {
        uint128_t v = power_of_2(i);
        int expected = 127 - i;
        int actual = v.leading_zeros();
        if (actual != expected) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("clz(2^k) == 127-k para k=0..127");
    else
        test_fail("clz(2^k)");

    // Random
    all_ok = true;
    for (int i = 0; i < 100; ++i) {
        uint128_t v = random_uint128_nonzero();
        int expected = reference_clz(v);
        int actual = v.leading_zeros();
        if (actual != expected) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("clz(random) (100 tests)");
    else
        test_fail("clz(random)");
}

// ============================================================================
// TEST: TRAILING ZEROS
// ============================================================================
void test_trailing_zeros()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Trailing Zeros (CTZ)                               |\n";
    std::cout << "+============================================================+\n";

    // Caso especial: 0
    if (uint128_t(0).trailing_zeros() == 128) {
        test_ok("ctz(0) == 128");
    } else {
        test_fail("ctz(0) == 128");
    }

    // Potencias de 2
    bool all_ok = true;
    for (int i = 0; i < 128; ++i) {
        uint128_t v = power_of_2(i);
        int expected = i;
        int actual = v.trailing_zeros();
        if (actual != expected) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("ctz(2^k) == k para k=0..127");
    else
        test_fail("ctz(2^k)");

    // Random
    all_ok = true;
    for (int i = 0; i < 100; ++i) {
        uint128_t v = random_uint128_nonzero();
        int expected = reference_ctz(v);
        int actual = v.trailing_zeros();
        if (actual != expected) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("ctz(random) (100 tests)");
    else
        test_fail("ctz(random)");
}

// ============================================================================
// TEST: IS POWER OF 2
// ============================================================================
void test_is_power_of_2()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Is Power of 2                                      |\n";
    std::cout << "+============================================================+\n";

    // 0 no es potencia de 2
    if (!uint128_t(0).is_power_of_2()) {
        test_ok("0 no es potencia de 2");
    } else {
        test_fail("0 no es potencia de 2");
    }

    // Potencias de 2 reales
    bool all_ok = true;
    for (int i = 0; i < 128; ++i) {
        uint128_t v = power_of_2(i);
        if (!v.is_power_of_2()) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("2^k es potencia de 2 para k=0..127");
    else
        test_fail("2^k es potencia de 2");

    // No potencias de 2
    all_ok = true;
    for (int i = 0; i < 127; ++i) {
        uint128_t v = power_of_2(i) + uint128_t(1);
        if (v.is_power_of_2() && v != uint128_t(2)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("2^k + 1 no es potencia de 2");
    else
        test_fail("2^k + 1 no es potencia de 2");

    // K bits set (no potencia si k > 1)
    all_ok = true;
    for (int k = 2; k <= 10; ++k) {
        for (int i = 0; i < 20; ++i) {
            uint128_t v = random_uint128_k_bits_set(k);
            if (v.is_power_of_2()) {
                all_ok = false;
                break;
            }
        }
        if (!all_ok)
            break;
    }
    if (all_ok)
        test_ok("k bits activos (k>1) no es potencia de 2");
    else
        test_fail("k bits activos");
}

// ============================================================================
// TEST: SHIFT LEFT
// ============================================================================
void test_shift_left()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Shift Left                                         |\n";
    std::cout << "+============================================================+\n";

    // 1 << k == 2^k
    bool all_ok = true;
    for (int k = 0; k < 128; ++k) {
        uint128_t result = uint128_t(1) << k;
        uint128_t expected = power_of_2(k);
        if (result != expected) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("1 << k == 2^k para k=0..127");
    else
        test_fail("1 << k == 2^k");

    // Shift >= 128 da 0
    all_ok = true;
    for (int k = 128; k < 140; ++k) {
        uint128_t v = random_uint128();
        if ((v << k) != uint128_t(0)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("x << k == 0 para k >= 128");
    else
        test_fail("x << k >= 128");

    // (a << k) >> k == a (para a sin bits altos que se pierdan)
    all_ok = true;
    for (int k = 1; k < 64; ++k) {
        for (int i = 0; i < 20; ++i) {
            uint128_t a = uint128_t(0, rng() >> k); // Solo bits bajos
            uint128_t shifted = (a << k) >> k;
            if (shifted != a) {
                all_ok = false;
                break;
            }
        }
        if (!all_ok)
            break;
    }
    if (all_ok)
        test_ok("(a << k) >> k == a (sin perdida)");
    else
        test_fail("(a << k) >> k");
}

// ============================================================================
// TEST: SHIFT RIGHT
// ============================================================================
void test_shift_right()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Shift Right                                        |\n";
    std::cout << "+============================================================+\n";

    // 2^k >> k == 1
    bool all_ok = true;
    for (int k = 0; k < 128; ++k) {
        uint128_t pot2 = power_of_2(k);
        uint128_t result = pot2 >> k;
        if (result != uint128_t(1)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("2^k >> k == 1 para k=0..127");
    else
        test_fail("2^k >> k == 1");

    // Shift >= 128 da 0
    all_ok = true;
    for (int k = 128; k < 140; ++k) {
        uint128_t v = random_uint128();
        if ((v >> k) != uint128_t(0)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("x >> k == 0 para k >= 128");
    else
        test_fail("x >> k >= 128");

    // a >> k == a / 2^k
    all_ok = true;
    for (int k = 1; k < 127; ++k) {
        for (int i = 0; i < 20; ++i) {
            uint128_t a = random_uint128();
            uint128_t shift_result = a >> k;
            uint128_t div_result = a / power_of_2(k);
            if (shift_result != div_result) {
                all_ok = false;
                break;
            }
        }
        if (!all_ok)
            break;
    }
    if (all_ok)
        test_ok("a >> k == a / 2^k");
    else
        test_fail("a >> k == a / 2^k");
}

// ============================================================================
// TEST: BITWISE AND
// ============================================================================
void test_bitwise_and()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Bitwise AND                                        |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // a & 0 == 0
    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a & uint128_t(0)) != uint128_t(0)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a & 0 == 0 (100 tests)");
    else
        test_fail("a & 0 == 0");

    // a & ~0 == a
    all_ok = true;
    uint128_t all_ones(~0ULL, ~0ULL);
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a & all_ones) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a & ~0 == a (100 tests)");
    else
        test_fail("a & ~0 == a");

    // a & a == a
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a & a) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a & a == a (100 tests)");
    else
        test_fail("a & a == a");

    // Conmutatividad: a & b == b & a
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if ((a & b) != (b & a)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a & b == b & a (100 tests)");
    else
        test_fail("a & b == b & a");
}

// ============================================================================
// TEST: BITWISE OR
// ============================================================================
void test_bitwise_or()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Bitwise OR                                         |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // a | 0 == a
    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a | uint128_t(0)) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a | 0 == a (100 tests)");
    else
        test_fail("a | 0 == a");

    // a | ~0 == ~0
    all_ok = true;
    uint128_t all_ones(~0ULL, ~0ULL);
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a | all_ones) != all_ones) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a | ~0 == ~0 (100 tests)");
    else
        test_fail("a | ~0 == ~0");

    // a | a == a
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a | a) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a | a == a (100 tests)");
    else
        test_fail("a | a == a");

    // Conmutatividad
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if ((a | b) != (b | a)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a | b == b | a (100 tests)");
    else
        test_fail("a | b == b | a");
}

// ============================================================================
// TEST: BITWISE XOR
// ============================================================================
void test_bitwise_xor()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Bitwise XOR                                        |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // a ^ 0 == a
    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a ^ uint128_t(0)) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a ^ 0 == a (100 tests)");
    else
        test_fail("a ^ 0 == a");

    // a ^ a == 0
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a ^ a) != uint128_t(0)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a ^ a == 0 (100 tests)");
    else
        test_fail("a ^ a == 0");

    // a ^ ~0 == ~a
    all_ok = true;
    uint128_t all_ones(~0ULL, ~0ULL);
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if ((a ^ all_ones) != ~a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a ^ ~0 == ~a (100 tests)");
    else
        test_fail("a ^ ~0 == ~a");

    // Conmutatividad
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if ((a ^ b) != (b ^ a)) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("a ^ b == b ^ a (100 tests)");
    else
        test_fail("a ^ b == b ^ a");

    // Inversa: (a ^ b) ^ b == a
    all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        uint128_t b = random_uint128();
        if (((a ^ b) ^ b) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("(a ^ b) ^ b == a (100 tests)");
    else
        test_fail("(a ^ b) ^ b == a");
}

// ============================================================================
// TEST: BITWISE NOT
// ============================================================================
void test_bitwise_not()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Bitwise NOT                                        |\n";
    std::cout << "+============================================================+\n";

    const int N = 100;

    // ~(~a) == a
    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        uint128_t a = random_uint128();
        if (~(~a) != a) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("~~a == a (100 tests)");
    else
        test_fail("~~a == a");

    // ~0 == all ones
    uint128_t all_ones(~0ULL, ~0ULL);
    if (~uint128_t(0) == all_ones) {
        test_ok("~0 == all ones");
    } else {
        test_fail("~0 == all ones");
    }

    // ~(all ones) == 0
    if (~all_ones == uint128_t(0)) {
        test_ok("~(all ones) == 0");
    } else {
        test_fail("~(all ones) == 0");
    }
}

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    std::cout << "+============================================================+\n";
    std::cout << "|   TESTS SISTEMATICOS DE FUNCIONES DE BITS                  |\n";
    std::cout << "|   int128_base_t (uint128_t / int128_t)                     |\n";
    std::cout << "+============================================================+\n";

    test_leading_zeros();
    test_trailing_zeros();
    test_is_power_of_2();
    test_shift_left();
    test_shift_right();
    test_bitwise_and();
    test_bitwise_or();
    test_bitwise_xor();
    test_bitwise_not();

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
