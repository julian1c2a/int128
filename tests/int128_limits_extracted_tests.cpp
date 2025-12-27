/*
 * Tests extraídos para int128_limits.hpp
 * Pruebas de std::numeric_limits<int128_t>
 */

#include "../include/int128/int128_limits.hpp"
#include "../include/int128/int128_t.hpp"
#include "test_iostream_helpers.hpp"
#include <cassert>
#include <iostream>
#include <limits>

using namespace nstd;


// =============================================================================
// TEST: is_specialized
// =============================================================================
bool test_is_specialized()
{
    std::cout << "\n=== TEST: is_specialized ===\n";
    bool result = std::numeric_limits<int128_t>::is_specialized;
    std::cout << "  is_specialized = " << result << "\n";
    assert(result && "numeric_limits debe estar especializado");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_signed
// =============================================================================
bool test_is_signed()
{
    std::cout << "\n=== TEST: is_signed ===\n";
    bool result = std::numeric_limits<int128_t>::is_signed;
    std::cout << "  is_signed = " << result << "\n";
    assert(result && "int128_t debe ser signed");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_integer
// =============================================================================
bool test_is_integer()
{
    std::cout << "\n=== TEST: is_integer ===\n";
    bool result = std::numeric_limits<int128_t>::is_integer;
    std::cout << "  is_integer = " << result << "\n";
    assert(result && "int128_t debe ser integer");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_exact
// =============================================================================
bool test_is_exact()
{
    std::cout << "\n=== TEST: is_exact ===\n";
    bool result = std::numeric_limits<int128_t>::is_exact;
    std::cout << "  is_exact = " << result << "\n";
    assert(result && "int128_t debe ser exacto");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: has_infinity
// =============================================================================
bool test_has_infinity()
{
    std::cout << "\n=== TEST: has_infinity ===\n";
    bool result = std::numeric_limits<int128_t>::has_infinity;
    std::cout << "  has_infinity = " << result << "\n";
    assert(!result && "int128_t no debe tener infinity");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: has_quiet_NaN
// =============================================================================
bool test_has_quiet_NaN()
{
    std::cout << "\n=== TEST: has_quiet_NaN ===\n";
    bool result = std::numeric_limits<int128_t>::has_quiet_NaN;
    std::cout << "  has_quiet_NaN = " << result << "\n";
    assert(!result && "int128_t no debe tener quiet_NaN");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_bounded
// =============================================================================
bool test_is_bounded()
{
    std::cout << "\n=== TEST: is_bounded ===\n";
    bool result = std::numeric_limits<int128_t>::is_bounded;
    std::cout << "  is_bounded = " << result << "\n";
    assert(result && "int128_t debe ser bounded");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_modulo
// =============================================================================
bool test_is_modulo()
{
    std::cout << "\n=== TEST: is_modulo ===\n";
    bool result = std::numeric_limits<int128_t>::is_modulo;
    std::cout << "  is_modulo = " << result << "\n";
    assert(!result && "int128_t signed no debe ser modulo");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: digits
// =============================================================================
bool test_digits()
{
    std::cout << "\n=== TEST: digits ===\n";
    int result = std::numeric_limits<int128_t>::digits;
    std::cout << "  digits = " << result << "\n";
    assert(result == 127 && "int128_t debe tener 127 bits (sin signo)");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: digits10
// =============================================================================
bool test_digits10()
{
    std::cout << "\n=== TEST: digits10 ===\n";
    int result = std::numeric_limits<int128_t>::digits10;
    std::cout << "  digits10 = " << result << "\n";
    assert(result == 38 && "int128_t debe tener 38 dígitos decimales");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: radix
// =============================================================================
bool test_radix()
{
    std::cout << "\n=== TEST: radix ===\n";
    int result = std::numeric_limits<int128_t>::radix;
    std::cout << "  radix = " << result << "\n";
    assert(result == 2 && "int128_t debe ser binario (radix 2)");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: min()
// =============================================================================
bool test_min()
{
    std::cout << "\n=== TEST: min() ===\n";
    int128_t result = std::numeric_limits<int128_t>::min();
    std::cout << "  min() = " << result << "\n";
    assert(result < int128_t(0) && "min() debe ser negativo para signed");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: lowest()
// =============================================================================
bool test_lowest()
{
    std::cout << "\n=== TEST: lowest() ===\n";
    int128_t result = std::numeric_limits<int128_t>::lowest();
    int128_t min_val = std::numeric_limits<int128_t>::min();
    std::cout << "  lowest() = " << result << "\n";
    assert(result == min_val && "lowest() debe ser igual a min() para enteros signed");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: max()
// =============================================================================
bool test_max()
{
    std::cout << "\n=== TEST: max() ===\n";
    int128_t result = std::numeric_limits<int128_t>::max();
    std::cout << "  max() = " << result << "\n";
    assert(result > int128_t(0) && "max() debe ser positivo");
    // Verificar que max() + 1 hace overflow a negativo
    int128_t max_plus_one = result + int128_t(1);
    assert(max_plus_one < int128_t(0) && "max() + 1 debe hacer overflow a negativo");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: epsilon()
// =============================================================================
bool test_epsilon()
{
    std::cout << "\n=== TEST: epsilon() ===\n";
    int128_t result = std::numeric_limits<int128_t>::epsilon();
    std::cout << "  epsilon() = " << result << "\n";
    assert(result == int128_t(0) && "epsilon() debe ser 0 para enteros");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: round_error()
// =============================================================================
bool test_round_error()
{
    std::cout << "\n=== TEST: round_error() ===\n";
    int128_t result = std::numeric_limits<int128_t>::round_error();
    std::cout << "  round_error() = " << result << "\n";
    assert(result == int128_t(0) && "round_error() debe ser 0 para enteros");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: infinity()
// =============================================================================
bool test_infinity()
{
    std::cout << "\n=== TEST: infinity() ===\n";
    int128_t result = std::numeric_limits<int128_t>::infinity();
    std::cout << "  infinity() = " << result << "\n";
    assert(result == int128_t(0) && "infinity() debe ser 0 para enteros");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: quiet_NaN()
// =============================================================================
bool test_quiet_NaN()
{
    std::cout << "\n=== TEST: quiet_NaN() ===\n";
    int128_t result = std::numeric_limits<int128_t>::quiet_NaN();
    std::cout << "  quiet_NaN() = " << result << "\n";
    assert(result == int128_t(0) && "quiet_NaN() debe ser 0 para enteros");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: signaling_NaN()
// =============================================================================
bool test_signaling_NaN()
{
    std::cout << "\n=== TEST: signaling_NaN() ===\n";
    int128_t result = std::numeric_limits<int128_t>::signaling_NaN();
    std::cout << "  signaling_NaN() = " << result << "\n";
    assert(result == int128_t(0) && "signaling_NaN() debe ser 0 para enteros");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: denorm_min()
// =============================================================================
bool test_denorm_min()
{
    std::cout << "\n=== TEST: denorm_min() ===\n";
    int128_t result = std::numeric_limits<int128_t>::denorm_min();
    int128_t min_val = std::numeric_limits<int128_t>::min();
    std::cout << "  denorm_min() = " << result << "\n";
    assert(result == min_val && "denorm_min() debe ser igual a min()");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "  TESTS: int128_limits.hpp\n";
    std::cout << "========================================\n";

    int passed = 0;
    int total = 0;

#define RUN_TEST(func)                                                                             \
    total++;                                                                                       \
    try {                                                                                          \
        if (func())                                                                                \
            passed++;                                                                              \
    } catch (const std::exception& e) {                                                            \
        std::cout << "  [FAIL] EXCEPTION: " << e.what() << "\n";                                   \
    }

    // Tests de propiedades booleanas
    RUN_TEST(test_is_specialized);
    RUN_TEST(test_is_signed);
    RUN_TEST(test_is_integer);
    RUN_TEST(test_is_exact);
    RUN_TEST(test_has_infinity);
    RUN_TEST(test_has_quiet_NaN);
    RUN_TEST(test_is_bounded);
    RUN_TEST(test_is_modulo);

    // Tests de constantes numéricas
    RUN_TEST(test_digits);
    RUN_TEST(test_digits10);
    RUN_TEST(test_radix);

    // Tests de métodos
    RUN_TEST(test_min);
    RUN_TEST(test_lowest);
    RUN_TEST(test_max);
    RUN_TEST(test_epsilon);
    RUN_TEST(test_round_error);
    RUN_TEST(test_infinity);
    RUN_TEST(test_quiet_NaN);
    RUN_TEST(test_signaling_NaN);
    RUN_TEST(test_denorm_min);

    std::cout << "\n========================================\n";
    std::cout << "  RESUMEN: " << passed << "/" << total << " tests pasados\n";
    std::cout << "========================================\n";

    return (passed == total) ? 0 : 1;
}
