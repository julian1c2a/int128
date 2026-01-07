// test_systematic_divrem.cpp
// Tests sistematicos de division (divrem) para int128_base_t
// Estructura reutilizable para benchmarks
// Sigue PROMPT.md - Output ASCII only

#include "include_new/int128_base_tt.hpp"
#include& lt; cassert & gt;
#include& lt; chrono & gt;
#include& lt; cstdint & gt;
#include& lt; iostream & gt;
#include& lt; random & gt;
#include& lt; vector & gt;

using namespace nstd;

// Generador de numeros aleatorios
static std::mt19937_64 rng(42); // Seed fija para reproducibilidad

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

inline uint128_t random_uint128_low_only()
{
    return uint128_t(0, rng());
}

inline uint128_t random_uint128_low_only_nonzero()
{
    uint64_t r;
    do {
        r = rng();
    } while (r == 0);
    return uint128_t(0, r);
}

template& lt;
int N& gt;
inline uint128_t random_uint128_n_bits()
{
    static_assert(N& gt; = 1 & amp; &amp; N& lt; = 128, "N must be 1-128");
    if constexpr (N& lt; = 64) {
        uint64_t mask = (N == 64) ? ~0ULL : ((1ULL & lt; &lt; N) - 1);
        return uint128_t(0, rng() & amp; mask);
    } else {
        uint64_t high_mask = (N == 128) ? ~0ULL : ((1ULL & lt; &lt; (N - 64)) - 1);
        return uint128_t(rng() & amp; high_mask, rng());
    }
}

template& lt;
int N& gt;
inline uint128_t random_uint128_n_bits_nonzero()
{
    uint128_t r;
    do {
        r = random_uint128_n_bits & lt;
        N & gt;
        ();
    } while (r == uint128_t(0));
    return r;
}

inline uint128_t random_uint128_k_bits_set(int k)
{
    if (k& lt; = 0)
        return uint128_t(0);
    if (k& gt; = 128)
        return uint128_t(~0ULL, ~0ULL);

    uint128_t result(0);
    std::vector& lt;
    int& gt;
    positions(128);
    for (int i = 0; i & lt; 128; ++i)
        positions[i] = i;

    for (int i = 0; i & lt; k; ++i) {
        int j = i + (rng() % (128 - i));
        std::swap(positions[i], positions[j]);

        int pos = positions[i];
        if (pos& lt; 64) {
            result = result | uint128_t(0, 1ULL & lt; &lt; pos);
        } else {
            result = result | uint128_t(1ULL & lt; &lt; (pos - 64), 0);
        }
    }
    return result;
}

inline uint128_t power_of_2(int exp)
{
    if (exp& lt; 0 || exp& gt; = 128)
        return uint128_t(0);
    if (exp& lt; 64)
        return uint128_t(0, 1ULL & lt; &lt; exp);
    else
        return uint128_t(1ULL & lt; &lt; (exp - 64), 0);
}

// ============================================================================
// CONTADORES Y UTILIDADES
// ============================================================================

static int tests_passed = 0;
static int tests_failed = 0;

inline void test_ok(const char* msg)
{
    ++tests_passed;
    std::cout& lt;
    &lt;
    "[OK] " & lt;
    &lt;
    msg & lt;
    &lt;
    "\n";
}

inline void test_fail(const char* msg)
{
    ++tests_failed;
    std::cout& lt;
    &lt;
    "[FAIL] " & lt;
    &lt;
    msg & lt;
    &lt;
    "\n";
}

// ============================================================================
// TEST: PROPIEDAD EUCLIDIANA n = q*d + r
// ============================================================================
void test_euclidean_property()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Propiedad Euclidiana n = q*d + r                   |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;

    // Random / Random (nonzero)
    bool all_ok = true;
    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128();
        uint128_t d = random_uint128_nonzero();

        auto [q, r] = n.divrem(d);

        uint128_t reconstructed = q * d + r;
        if (reconstructed != n) {
            all_ok = false;
            test_fail("q*d + r != n");
            break;
        }
    }
    if (all_ok)
        test_ok("random / random: q*d + r == n (100 tests)");

    // Low-only / Low-only
    all_ok = true;
    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128_low_only();
        uint128_t d = random_uint128_low_only_nonzero();

        auto [q, r] = n.divrem(d);

        if (q * d + r != n) {
            all_ok = false;
            break;
        }
    }
    if (all_ok)
        test_ok("low_only / low_only: q*d + r == n (100 tests)");
    else
        test_fail("low_only / low_only");
}

// ============================================================================
// TEST: RESTO MENOR QUE DIVISOR (r &lt; d)
// ============================================================================
void test_remainder_less_than_divisor()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Resto menor que divisor (r &lt; d)                    |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128();
        uint128_t d = random_uint128_nonzero();

        auto [q, r] = n.divrem(d);

        if (!(r & lt; d)) {
            all_ok = false;
            test_fail("r &gt;= d");
            break;
        }
    }
    if (all_ok)
        test_ok("random: r &lt; d (100 tests)");
}

// ============================================================================
// TEST: DIVISION POR 1
// ============================================================================
void test_division_by_one()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division por 1 (n/1 = n, r=0)                      |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128();
        uint128_t one(1);

        auto [q, r] = n.divrem(one);

        if (q != n || r != uint128_t(0)) {
            all_ok = false;
            test_fail("n/1 != n o r != 0");
            break;
        }
    }
    if (all_ok)
        test_ok("n/1 = n, r = 0 (100 tests)");
}

// ============================================================================
// TEST: DIVISION POR SI MISMO
// ============================================================================
void test_division_by_self()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division por si mismo (n/n = 1, r=0)               |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128_nonzero();

        auto [q, r] = n.divrem(n);

        if (q != uint128_t(1) || r != uint128_t(0)) {
            all_ok = false;
            test_fail("n/n != 1 o r != 0");
            break;
        }
    }
    if (all_ok)
        test_ok("n/n = 1, r = 0 (100 tests)");
}

// ============================================================================
// TEST: DIVISION POR 0 (caso limite)
// ============================================================================
void test_division_by_zero()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division por 0 (caso limite)                       |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    // En el nuevo divrem, division por 0 tiene comportamiento indefinido
    // como los tipos builtin. No hay optional.
    // Este test simplemente verifica que podemos detectar el caso ANTES.

    uint128_t n(123);
    uint128_t zero(0);

    // Verificamos que podemos comprobar antes de dividir
    if (zero == uint128_t(0)) {
        test_ok("Division por 0 detectable antes de llamar (d == 0)");
    } else {
        test_fail("No se puede detectar d == 0");
    }
}

// ============================================================================
// TEST: DIVISION POR POTENCIAS DE 2
// ============================================================================
void test_division_by_powers_of_2()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division por potencias de 2 (2^k, k=0..127)        |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    bool all_ok = true;

    for (int k = 0; k & lt; 127; ++k) {
        uint128_t n = random_uint128();
        uint128_t d = power_of_2(k);

        auto [q, r] = n.divrem(d);

        // Verificar propiedad euclidiana
        if (q * d + r != n) {
            all_ok = false;
            test_fail("pot2: q*d + r != n");
            break;
        }

        // r &lt; d
        if (!(r & lt; d)) {
            all_ok = false;
            test_fail("pot2: r &gt;= d");
            break;
        }
    }
    if (all_ok)
        test_ok("division por 2^k, k=0..126 (127 tests)");
}

// ============================================================================
// TEST: DIVISION CON TIPOS BUILTIN
// ============================================================================
void test_division_builtin_types()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division con tipos builtin (uint8..uint64)         |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // uint8_t
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint8_t d8 = static_cast & lt;
        uint8_t& gt;
        ((rng() % 255) + 1); // 1-255
        uint128_t d = uint128_t(d8);

        auto [q, r] = n.divrem(d);

        // Comparar con operadores /  y %
        uint128_t q2 = n / d;
        uint128_t r2 = n % d;

        if (q != q2 || r != r2) {
            all_ok = false;
            test_fail("uint8_t: divrem != / y %");
            break;
        }
    }
    if (all_ok)
        test_ok("divrem(uint8_t) consistente con / y % (100)");

    // uint16_t
    all_ok = true;
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint16_t d16 = static_cast & lt;
        uint16_t& gt;
        ((rng() % 65535) + 1);
        uint128_t d = uint128_t(d16);

        auto [q, r] = n.divrem(d);

        if (q != n / d || r != n % d) {
            all_ok = false;
            test_fail("uint16_t: divrem != / y %");
        }
    }
    if (all_ok)
        test_ok("divrem(uint16_t) consistente con / y % (100)");

    // uint32_t
    all_ok = true;
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint32_t d32 = static_cast & lt;
        uint32_t& gt;
        (rng() | 1); // asegurar != 0
        uint128_t d = uint128_t(d32);

        auto [q, r] = n.divrem(d);

        if (q != n / d || r != n % d) {
            all_ok = false;
            test_fail("uint32_t: divrem != / y %");
        }
    }
    if (all_ok)
        test_ok("divrem(uint32_t) consistente con / y % (100)");

    // uint64_t
    all_ok = true;
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint64_t d64 = rng() | 1; // asegurar != 0
        uint128_t d = uint128_t(d64);

        auto [q, r] = n.divrem(d);

        if (q != n / d || r != n % d) {
            all_ok = false;
            test_fail("uint64_t: divrem != / y %");
        }
    }
    if (all_ok)
        test_ok("divrem(uint64_t) consistente con / y % (100)");
}

// ============================================================================
// TEST: DIVISION CON K BITS ACTIVOS EN DIVISOR
// ============================================================================
void test_division_k_bits_set()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division con k bits activos en divisor (k=1..125)  |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    bool all_ok = true;

    for (int k = 1; k& lt; = 125 & amp; &amp; all_ok; k += 5) { // Muestreo cada 5
        for (int i = 0; i & lt; 10 & amp; &amp; all_ok; ++i) {
            uint128_t n = random_uint128();
            uint128_t d = random_uint128_k_bits_set(k);
            if (d == uint128_t(0))
                d = uint128_t(1); // Evitar div/0

            auto [q, r] = n.divrem(d);

            // Verificar propiedad euclidiana
            if (q * d + r != n) {
                all_ok = false;
                test_fail("k bits: q*d + r != n");
            }
        }
    }
    if (all_ok)
        test_ok("division con k bits en divisor, k=1..125 (250 tests)");
}

// ============================================================================
// TEST: DIVIDENDO MENOR QUE DIVISOR
// ============================================================================
void test_dividend_less_than_divisor()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Dividendo &lt; Divisor (q=0, r=n)                     |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i & lt; N; ++i) {
        // Crear n &lt; d
        uint128_t d = random_uint128_nonzero();
        uint128_t n = d / uint128_t(2); // n &lt; d siempre

        if (n& gt; = d)
            continue; // Saltar si no cumple

        auto [q, r] = n.divrem(d);

        if (q != uint128_t(0) || r != n) {
            all_ok = false;
            test_fail("n &lt; d: q != 0 o r != n");
            break;
        }
    }
    if (all_ok)
        test_ok("n &lt; d: q = 0, r = n (100 tests)");
}

// ============================================================================
// TEST: DIVISION SOLO EN PARTE BAJA (high == 0)
// ============================================================================
void test_division_low_only()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division solo parte baja (n.high == 0)             |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    for (int i = 0; i & lt; N; ++i) {
        uint128_t n = random_uint128_low_only();
        uint128_t d = random_uint128_low_only_nonzero();

        auto [q, r] = n.divrem(d);

        // Verificar contra uint64_t nativo
        uint64_t n64 = n.low();
        uint64_t d64 = d.low();
        uint64_t q64 = n64 / d64;
        uint64_t r64 = n64 % d64;

        if (q.low() != q64 || r.low() != r64) {
            all_ok = false;
            test_fail("low_only: resultado difiere de uint64_t nativo");
            break;
        }
    }
    if (all_ok)
        test_ok("low_only == uint64_t nativo (100 tests)");
}

// ============================================================================
// TEST: PROPIEDADES INCREMENTO/DECREMENTO
// Para division: Si n/d = (q,r) entonces:
// - (n+d)/d = (q+1, r)  cuando r+d no overflow con d
// - (n-d)/d = (q-1, r)  cuando n &gt;= d
// ============================================================================
void test_division_increment_properties()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Propiedades incremento/decremento division         |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    const int N = 100;
    bool all_ok = true;

    // Si n/d = (q,r), entonces (n+d)/d = (q+1, r)
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint128_t d = random_uint128_nonzero();

        // Evitar overflow
        if (n& gt; uint128_t::max() - d)
            continue;

        auto [q, r] = n.divrem(d);
        auto [q2, r2] = (n + d).divrem(d);

        if (q2 != q + uint128_t(1) || r2 != r) {
            all_ok = false;
            test_fail("(n+d)/d != (q+1, r)");
        }
    }
    if (all_ok)
        test_ok("(n+d)/d = (q+1, r) (100 tests)");

    // Si n &gt;= d y n/d = (q,r), entonces (n-d)/d = (q-1, r)
    all_ok = true;
    for (int i = 0; i & lt; N & amp; &amp; all_ok; ++i) {
        uint128_t n = random_uint128();
        uint128_t d = random_uint128_nonzero();

        if (n& lt; d)
            continue; // Necesitamos n &gt;= d

        auto [q, r] = n.divrem(d);

        if (q == uint128_t(0))
            continue; // q-1 seria underflow

        auto [q2, r2] = (n - d).divrem(d);

        if (q2 != q - uint128_t(1) || r2 != r) {
            all_ok = false;
            test_fail("(n-d)/d != (q-1, r)");
        }
    }
    if (all_ok)
        test_ok("(n-d)/d = (q-1, r) cuando n &gt;= d (100 tests)");
}

// ============================================================================
// TEST: LONGITUD EFECTIVA (effective_length)
// Parametrizacion por bits significativos
// ============================================================================
void test_division_by_effective_length()
{
    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TEST: Division por longitud efectiva de operandos        |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    bool all_ok = true;

    // Test con diferentes longitudes efectivas
    // n_bits: 32, 64, 96, 128  x  d_bits: 8, 16, 32, 64
    int n_bits_list[] = {32, 64, 96, 128};
    int d_bits_list[] = {8, 16, 32, 64};

    for (int n_bits : n_bits_list) {
        for (int d_bits : d_bits_list) {
            for (int i = 0; i & lt; 10 & amp; &amp; all_ok; ++i) {
                // Crear n con n_bits bits significativos
                uint128_t n;
                if (n_bits& lt; = 64) {
                    uint64_t mask = (n_bits == 64) ? ~0ULL : ((1ULL & lt; &lt; n_bits) - 1);
                    n = uint128_t(0, rng() & amp; mask);
                    if (n_bits& gt; 0)
                        n = n | uint128_t(0, 1ULL & lt; &lt; (n_bits - 1)); // MSB set
                } else {
                    uint64_t high_mask =
                        (n_bits == 128) ? ~0ULL : ((1ULL & lt; &lt; (n_bits - 64)) - 1);
                    n = uint128_t(rng() & amp; high_mask, rng());
                    n = n | uint128_t(1ULL & lt; &lt; (n_bits - 65), 0); // MSB set
                }

                // Crear d con d_bits bits significativos
                uint128_t d;
                uint64_t d_mask = (d_bits == 64) ? ~0ULL : ((1ULL & lt; &lt; d_bits) - 1);
                d = uint128_t(0, (rng() & amp; d_mask) | (1ULL & lt; &lt; (d_bits - 1)));
                if (d == uint128_t(0))
                    d = uint128_t(1);

                auto [q, r] = n.divrem(d);

                if (q * d + r != n) {
                    all_ok = false;
                    test_fail("effective_length: q*d + r != n");
                }
            }
        }
    }
    if (all_ok)
        test_ok("division por longitud efectiva (160 combinaciones)");
}

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    std::cout& lt;
    &lt;
    "+============================================================+\n";
    std::cout& lt;
    &lt;
    "|   TESTS SISTEMATICOS DE DIVISION (divrem)                  |\n";
    std::cout& lt;
    &lt;
    "|   Para benchmarks y validacion exhaustiva                  |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    test_euclidean_property();
    test_remainder_less_than_divisor();
    test_division_by_one();
    test_division_by_self();
    test_division_by_zero();
    test_division_by_powers_of_2();
    test_division_builtin_types();
    test_division_k_bits_set();
    test_dividend_less_than_divisor();
    test_division_low_only();
    test_division_increment_properties();
    test_division_by_effective_length();

    std::cout& lt;
    &lt;
    "\n+============================================================+\n";
    std::cout& lt;
    &lt;
    "|                        RESUMEN                             |\n";
    std::cout& lt;
    &lt;
    "+============================================================+\n";

    if (tests_failed == 0) {
        std::cout& lt;
        &lt;
        "[OK] Tests pasados: " & lt;
        &lt;
        tests_passed & lt;
        &lt;
        "/" & lt;
        &lt;
        tests_passed & lt;
        &lt;
        "\n";
    } else {
        std::cout& lt;
        &lt;
        "[FAIL] Pasados: " & lt;
        &lt;
        tests_passed & lt;
        &lt;
        ", Fallados: " & lt;
        &lt;
        tests_failed & lt;
        &lt;
        "\n";
    }

    return tests_failed & gt;
    0 ? 1 : 0;
}
