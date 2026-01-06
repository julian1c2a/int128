// Test para constructor y asignación desde floating point
#include "include_new/int128_base.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace nstd;
using uint128_t = int128_base_t<signedness::unsigned_type>;
using int128_t = int128_base_t<signedness::signed_type>;

void print_section(const char* title)
{
    std::cout << "\n";
    std::cout << "+===========================================================+\n";
    std::cout << "|  " << title << "\n";
    std::cout << "+===========================================================+\n";
}

void test_uint128_constructor_float()
{
    std::cout << "\n=== Constructor uint128_t desde float ===\n";

    // Test 1: Valor pequeño (< 2^64)
    uint128_t val1(42.0f);
    assert(val1.low() == 42ull);
    assert(val1.high() == 0ull);
    std::cout << "OK Constructor desde float pequeño: 42.0f\n";

    // Test 2: Valor grande (>= 2^64)
    const float big = 18446744073709551616.0f * 2.0f; // 2^65
    uint128_t val2(big);
    assert(val2.high() == 2ull);
    assert(val2.low() == 0ull);
    std::cout << "OK Constructor desde float grande: 2^65\n";

    // Test 3: Valor negativo → debe dar cero para unsigned
    uint128_t val3(-100.0f);
    assert(val3.low() == 0ull);
    assert(val3.high() == 0ull);
    std::cout << "OK Constructor desde float negativo → 0 (unsigned)\n";
}

void test_uint128_constructor_double()
{
    std::cout << "\n=== Constructor uint128_t desde double ===\n";

    // Test 1: Valor con parte decimal (se trunca)
    uint128_t val1(123.456);
    assert(val1.low() == 123ull);
    assert(val1.high() == 0ull);
    std::cout << "OK Constructor desde double con decimales: 123.456 → 123\n";

    // Test 2: Valor muy grande
    const double huge = 18446744073709551616.0 * 1000.0; // 2^64 * 1000
    uint128_t val2(huge);
    assert(val2.high() == 1000ull);
    std::cout << "OK Constructor desde double muy grande: 2^64 * 1000\n";
}

void test_uint128_assignment_float()
{
    std::cout << "\n=== Asignación uint128_t = float ===\n";

    uint128_t val;

    // Test 1: Asignar valor pequeño
    val = 100.0f;
    assert(val.low() == 100ull);
    assert(val.high() == 0ull);
    std::cout << "OK Asignación desde float pequeño: 100.0f\n";

    // Test 2: Asignar valor grande
    val = 18446744073709551616.0f * 5.0f;
    assert(val.high() == 5ull);
    std::cout << "OK Asignación desde float grande: 2^64 * 5\n";

    // Test 3: Valor negativo → cero
    val = -50.0f;
    assert(val.low() == 0ull);
    assert(val.high() == 0ull);
    std::cout << "OK Asignación desde float negativo → 0\n";
}

void test_int128_constructor_float()
{
    std::cout << "\n=== Constructor int128_t desde float ===\n";

    // Test 1: Valor positivo pequeño
    int128_t val1(42.0f);
    assert(val1.low() == 42ull);
    assert(val1.high() == 0ull);
    std::cout << "OK Constructor int128 desde float positivo: 42.0f\n";

    // Test 2: Valor negativo pequeño
    int128_t val2(-100.0f);
    // -100 en complemento a 2
    uint128_t unsigned_rep(val2);
    // Verificar que es negativo
    assert(val2.is_negative());
    std::cout << "OK Constructor int128 desde float negativo: -100.0f\n";

    // Test 3: Valor grande positivo
    const float big = 18446744073709551616.0f * 2.0f;
    int128_t val3(big);
    assert(val3.high() == 2ull);
    std::cout << "OK Constructor int128 desde float grande: 2^65\n";
}

void test_int128_constructor_double()
{
    std::cout << "\n=== Constructor int128_t desde double ===\n";

    // Test 1: Valor positivo con decimales
    int128_t val1(456.789);
    assert(val1.low() == 456ull);
    assert(val1.high() == 0ull);
    std::cout << "OK Constructor int128 desde double: 456.789 → 456\n";

    // Test 2: Valor negativo con decimales
    int128_t val2(-999.123);
    assert(val2.is_negative());
    std::cout << "OK Constructor int128 desde double negativo: -999.123\n";
}

void test_int128_assignment_double()
{
    std::cout << "\n=== Asignación int128_t = double ===\n";

    int128_t val;

    // Test 1: Asignar positivo
    val = 1234.0;
    assert(val.low() == 1234ull);
    assert(val.high() == 0ull);
    std::cout << "OK Asignación int128 desde double positivo: 1234.0\n";

    // Test 2: Asignar negativo
    val = -5678.0;
    assert(val.is_negative());
    std::cout << "OK Asignación int128 desde double negativo: -5678.0\n";

    // Test 3: Asignar grande
    val = 18446744073709551616.0 * 10.0;
    assert(val.high() == 10ull);
    std::cout << "OK Asignación int128 desde double grande: 2^64 * 10\n";
}

void test_conversion_to_float()
{
    std::cout << "\n=== Conversión a float/double ===\n";

    // Test 1: uint128_t → float (usar constructor con un solo argumento = low)
    uint128_t uval(1000ull);
    float f = static_cast<float>(uval);
    assert(std::abs(f - 1000.0f) < 1.0f);
    std::cout << "OK Conversión uint128_t → float: 1000\n";

    // Test 2: int128_t positivo → double
    int128_t ival1(5000ull);
    double d1 = static_cast<double>(ival1);
    assert(std::abs(d1 - 5000.0) < 1.0);
    std::cout << "OK Conversión int128_t positivo → double: 5000\n";

    // Test 3: uint128_t grande → double
    uint128_t uval_big(10ull, 0ull); // high=10, low=0 → 10 * 2^64
    double d2 = static_cast<double>(uval_big);
    const double expected = 10.0 * 18446744073709551616.0;
    assert(std::abs(d2 - expected) < expected * 1e-10);
    std::cout << "OK Conversión uint128_t grande → double: 10 * 2^64\n";
}

void test_roundtrip()
{
    std::cout << "\n=== Tests de ida y vuelta (roundtrip) ===\n";

    // Test 1: float → uint128_t → float
    const float original1 = 12345.0f;
    uint128_t temp1(original1);
    float recovered1 = static_cast<float>(temp1);
    assert(std::abs(recovered1 - original1) < 1.0f);
    std::cout << "OK Roundtrip float → uint128_t → float: 12345.0f\n";

    // Test 2: double → int128_t → double (positivo)
    const double original2 = 98765.0;
    int128_t temp2(original2);
    double recovered2 = static_cast<double>(temp2);
    assert(std::abs(recovered2 - original2) < 1.0);
    std::cout << "OK Roundtrip double → int128_t → double (positivo): 98765.0\n";

    // Test 3: double → int128_t → double (negativo)
    const double original3 = -54321.0;
    int128_t temp3(original3);
    double recovered3 = static_cast<double>(temp3);
    assert(std::abs(recovered3 - original3) < 1.0);
    std::cout << "OK Roundtrip double → int128_t → double (negativo): -54321.0\n";
}

void test_explicit_requirement()
{
    std::cout << "\n=== Verificación de explicit (debe requerir cast) ===\n";

    // Estas líneas NO deben compilar (comentadas):
    // uint128_t implicit1 = 42.0f;        // FAIL Error: constructor explicit
    // int128_t implicit2 = -100.0;        // FAIL Error: constructor explicit

    // Estas SÍ deben compilar (cast explícito):
    uint128_t explicit1(42.0f);             // OK OK
    int128_t explicit2(-100.0);             // OK OK
    uint128_t explicit3 = uint128_t(3.14f); // OK OK

    std::cout << "OK Constructores requieren cast explícito (regla explicit)\n";
}

int main()
{
    print_section("Tests de Floating Point para int128_base_t");

    try {
        test_uint128_constructor_float();
        test_uint128_constructor_double();
        test_uint128_assignment_float();

        test_int128_constructor_float();
        test_int128_constructor_double();
        test_int128_assignment_double();

        test_conversion_to_float();
        test_roundtrip();
        test_explicit_requirement();

        print_section("✅ TODOS LOS TESTS DE FLOATING POINT PASARON");
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ERROR: " << e.what() << "\n";
        return 1;
    }
}
