// ==============================================================================
// phase15_int128_tests.cpp - Tests para validar int128_t con template unificado
// ==============================================================================

#include <cassert>
#include <int128_base.hpp>
#include <iostream>

using namespace nstd;

int test_count = 0;
int passed_count = 0;

#define TEST(name)                                                                                 \
    std::cout << "Test " << ++test_count << ": " << name << "... ";                                \
    try {

#define END_TEST                                                                                   \
    std::cout << "✅ PASS\n";                                                                      \
    ++passed_count;                                                                                \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        std::cout << "❌ FAIL\n";                                                                  \
    }

int main()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║   FASE 1.5 - TESTS int128_t (Template Unificado)          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    // Constructores con sign extension
    TEST("Constructor default")
    int128_t a;
    assert(a.low() == 0 && a.high() == 0);
    END_TEST

    TEST("Constructor desde int64_t positivo")
    int128_t b(12345LL);
    assert(b.low() == 12345ULL && b.high() == 0);
    END_TEST

    TEST("Constructor desde int64_t negativo (sign extension)")
    int128_t c(-1LL);
    assert(c.low() == 0xFFFFFFFFFFFFFFFFULL && c.high() == 0xFFFFFFFFFFFFFFFFULL);
    END_TEST

    TEST("Constructor (high, low)")
    int128_t d(100, 200);
    assert(d.high() == 100 && d.low() == 200);
    END_TEST

    // Aritmética signed
    TEST("Suma con positivos")
    int128_t a(0, 100), b(0, 200);
    int128_t c = a + b;
    assert(c.low() == 300 && c.high() == 0);
    END_TEST

    TEST("Suma: positivo + negativo")
    int128_t a(0, 100);
    int128_t b(-50LL);
    int128_t c = a + b;
    assert(c.low() == 50);
    END_TEST

    TEST("Resta básica")
    int128_t a(0, 300), b(0, 100);
    int128_t c = a - b;
    assert(c.low() == 200);
    END_TEST

    TEST("Multiplicación con negativos")
    int128_t a(-10LL), b(5LL);
    int128_t c = a * b;
    // -10 * 5 = -50 (complemento a 2)
    assert(c.is_negative());
    END_TEST

    // Negación (solo signed)
    TEST("Negación de positivo")
    int128_t a(0, 100);
    int128_t b = -a;
    assert(b.is_negative());
    END_TEST

    TEST("Negación de negativo")
    int128_t a(-100LL);
    int128_t b = -a;
    assert(!b.is_negative());
    END_TEST

    TEST("Negación doble")
    int128_t a(0, 42);
    int128_t b = -(-a);
    assert(b.low() == 42 && b.high() == 0);
    END_TEST

    // Bitwise (mismo que unsigned, pero con signo)
    TEST("AND bitwise")
    int128_t a(0xFFFFFFFF, 0xFFFFFFFFFFFFFFFFULL);
    int128_t b(0x12345678, 0x9ABCDEF012345678ULL);
    int128_t c = a & b;
    assert(c.high() == 0x12345678);
    END_TEST

    TEST("OR bitwise")
    int128_t a(0, 0xFF00FF00FF00FF00ULL);
    int128_t b(0, 0x00FF00FF00FF00FFULL);
    int128_t c = a | b;
    assert(c.low() == 0xFFFFFFFFFFFFFFFFULL);
    END_TEST

    // Arithmetic right shift (con sign extension)
    TEST("Right shift aritmético (positivo)")
    int128_t a(1, 0);
    int128_t b = a >> 64;
    assert(b.low() == 1 && b.high() == 0);
    END_TEST

    TEST("Right shift aritmético (negativo)")
    int128_t a(-1LL);
    int128_t b = a >> 10;
    // Debe preservar signo (rellenar con 1s)
    assert(b.is_negative());
    END_TEST

    // Comparaciones con signo
    TEST("Negativo < Positivo")
    int128_t a(-100LL), b(100LL);
    assert(a < b);
    END_TEST

    TEST("Comparación entre negativos")
    int128_t a(-200LL), b(-100LL);
    assert(a < b);
    END_TEST

    TEST("Igualdad con negativos")
    int128_t a(-42LL), b(-42LL);
    assert(a == b);
    END_TEST

    // Detección de signo
    TEST("is_negative() con positivo")
    int128_t a(0, 100);
    assert(!a.is_negative());
    END_TEST

    TEST("is_negative() con negativo")
    int128_t a(-1LL);
    assert(a.is_negative());
    END_TEST

    TEST("is_negative() con cero")
    int128_t a(0, 0);
    assert(!a.is_negative());
    END_TEST

    // Resumen
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "RESUMEN: " << passed_count << "/" << test_count << " tests pasados\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

    if (passed_count == test_count) {
        std::cout << "✅ Todos los tests pasaron\n\n";
        return 0;
    } else {
        std::cout << "❌ " << (test_count - passed_count) << " tests fallaron\n\n";
        return 1;
    }
}
