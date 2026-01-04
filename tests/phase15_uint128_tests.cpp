// ==============================================================================
// phase15_uint128_tests.cpp - Tests para validar template unificado
// ==============================================================================

#include "int128_base.hpp"
#include <cassert>
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
    std::cout << "║   FASE 1.5 - TESTS uint128_t (Template Unificado)         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    // Constructores
    TEST("Constructor default")
    uint128_t a;
    assert(a.low() == 0 && a.high() == 0);
    END_TEST

    TEST("Constructor desde uint64_t")
    uint128_t b(12345ULL);
    assert(b.low() == 12345ULL && b.high() == 0);
    END_TEST

    TEST("Constructor (high, low)")
    uint128_t c(100, 200);
    assert(c.high() == 100 && c.low() == 200);
    END_TEST

    // Aritmética
    TEST("Suma básica")
    uint128_t a(0, 100), b(0, 200);
    uint128_t c = a + b;
    assert(c.low() == 300 && c.high() == 0);
    END_TEST

    TEST("Suma con carry")
    uint128_t a(0, 0xFFFFFFFFFFFFFFFFULL), b(0, 1);
    uint128_t c = a + b;
    assert(c.low() == 0 && c.high() == 1);
    END_TEST

    TEST("Resta básica")
    uint128_t a(0, 300), b(0, 100);
    uint128_t c = a - b;
    assert(c.low() == 200 && c.high() == 0);
    END_TEST

    TEST("Multiplicación")
    uint128_t a(0, 1000), b(0, 2);
    uint128_t c = a * b;
    assert(c.low() == 2000);
    END_TEST

    // Bitwise
    TEST("AND bitwise")
    uint128_t a(0xFFFFFFFF, 0xFFFFFFFFFFFFFFFFULL);
    uint128_t b(0x12345678, 0x9ABCDEF012345678ULL);
    uint128_t c = a & b;
    assert(c.high() == 0x12345678);
    END_TEST

    TEST("OR bitwise")
    uint128_t a(0, 0xFF00FF00FF00FF00ULL);
    uint128_t b(0, 0x00FF00FF00FF00FFULL);
    uint128_t c = a | b;
    assert(c.low() == 0xFFFFFFFFFFFFFFFFULL);
    END_TEST

    TEST("XOR bitwise")
    uint128_t a(0, 0xFF00FF00FF00FF00ULL);
    uint128_t b(0, 0xFF00FF00FF00FF00ULL);
    uint128_t c = a ^ b;
    assert(c.low() == 0 && c.high() == 0);
    END_TEST

    TEST("NOT bitwise")
    uint128_t a(0, 0);
    uint128_t b = ~a;
    assert(b.low() == 0xFFFFFFFFFFFFFFFFULL && b.high() == 0xFFFFFFFFFFFFFFFFULL);
    END_TEST

    // Shifts
    TEST("Left shift pequeño")
    uint128_t a(0, 1);
    uint128_t b = a << 8;
    assert(b.low() == 256 && b.high() == 0);
    END_TEST

    TEST("Left shift a high")
    uint128_t a(0, 1);
    uint128_t b = a << 64;
    assert(b.low() == 0 && b.high() == 1);
    END_TEST

    TEST("Right shift lógico")
    uint128_t a(1, 0);
    uint128_t b = a >> 64;
    assert(b.low() == 1 && b.high() == 0);
    END_TEST

    // Comparaciones
    TEST("Igualdad")
    uint128_t a(100, 200), b(100, 200);
    assert(a == b);
    END_TEST

    TEST("Desigualdad")
    uint128_t a(100, 200), b(100, 300);
    assert(a != b);
    END_TEST

    TEST("Menor que")
    uint128_t a(0, 100), b(0, 200);
    assert(a < b);
    END_TEST

    TEST("Mayor que")
    uint128_t a(0, 200), b(0, 100);
    assert(a > b);
    END_TEST

    // Incremento/Decremento
    TEST("Pre-incremento")
    uint128_t a(0, 99);
    ++a;
    assert(a.low() == 100);
    END_TEST

    TEST("Post-incremento")
    uint128_t a(0, 99);
    uint128_t b = a++;
    assert(b.low() == 99 && a.low() == 100);
    END_TEST

    TEST("Pre-decremento")
    uint128_t a(0, 101);
    --a;
    assert(a.low() == 100);
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
