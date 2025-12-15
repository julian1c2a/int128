#include "include/uint128_t.hpp"
#include <cassert>
#include <iostream>

// Función para imprimir información del compilador
void print_compiler_info()
{
    std::cout << "=== Cross-Compiler Compatibility Test ===" << std::endl;

#ifdef _MSC_VER
    std::cout << "Compilador: Microsoft Visual C++ " << _MSC_VER << std::endl;
    std::cout << "Intrínsecos MSVC: Activados" << std::endl;
#elif defined(__GNUC__)
    std::cout << "Compilador: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;

#ifdef __has_builtin
#if __has_builtin(__builtin_addcll)
    std::cout << "Intrínsecos GCC addcll: Disponibles" << std::endl;
#else
    std::cout << "Intrínsecos GCC addcll: No disponibles (usando fallback)" << std::endl;
#endif
#if __has_builtin(__builtin_subcll)
    std::cout << "Intrínsecos GCC subcll: Disponibles" << std::endl;
#else
    std::cout << "Intrínsecos GCC subcll: No disponibles (usando fallback)" << std::endl;
#endif
#else
    std::cout << "Intrínsecos GCC: No disponibles (usando fallback)" << std::endl;
#endif

#elif defined(__clang__)
    std::cout << "Compilador: Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;

#ifdef __has_builtin
#if __has_builtin(__builtin_addcll)
    std::cout << "Intrínsecos Clang addcll: Disponibles" << std::endl;
#else
    std::cout << "Intrínsecos Clang addcll: No disponibles (usando fallback)" << std::endl;
#endif
#if __has_builtin(__builtin_subcll)
    std::cout << "Intrínsecos Clang subcll: Disponibles" << std::endl;
#else
    std::cout << "Intrínsecos Clang subcll: No disponibles (usando fallback)" << std::endl;
#endif
#else
    std::cout << "Intrínsecos Clang: No disponibles (usando fallback)" << std::endl;
#endif
#else
    std::cout << "Compilador: Desconocido" << std::endl;
#endif
    std::cout << std::endl;
}

// Tests básicos de aritmética
void test_arithmetic()
{
    std::cout << "=== Test de Aritmética ===" << std::endl;

    // Test suma
    uint128_t a(0, 1); // 1
    uint128_t b(0, 2); // 2
    uint128_t result = a + b;
    assert(result == uint128_t(0, 3));
    std::cout << "✓ Suma: 1 + 2 = " << result.to_string() << std::endl;

    // Test suma con overflow de bajo orden
    uint128_t max_low(0, UINT64_MAX);
    uint128_t one(0, 1);
    uint128_t overflow_result = max_low + one;
    assert(overflow_result == uint128_t(1, 0));
    std::cout << "✓ Suma con overflow: " << max_low.to_string()
              << " + 1 = " << overflow_result.to_string() << std::endl;

    // Test resta
    uint128_t c(0, 5);
    uint128_t d(0, 3);
    uint128_t sub_result = c - d;
    assert(sub_result == uint128_t(0, 2));
    std::cout << "✓ Resta: 5 - 3 = " << sub_result.to_string() << std::endl;

    // Test resta con borrow
    uint128_t high_one(1, 0); // 2^64
    uint128_t borrow_result = high_one - one;
    uint64_t expected_low = UINT64_MAX;
    assert(borrow_result == uint128_t(0, expected_low));
    std::cout << "✓ Resta con borrow: 2^64 - 1 = " << borrow_result.to_string() << std::endl;

    std::cout << std::endl;
}

// Test de leading zeros count
void test_leading_zeros()
{
    std::cout << "=== Test de Leading Zeros ===" << std::endl;

    uint128_t zero(0, 0);
    assert(zero.leading_zeros() == 128);
    std::cout << "✓ Leading zeros de 0: " << zero.leading_zeros() << std::endl;

    uint128_t one(0, 1);
    assert(one.leading_zeros() == 127);
    std::cout << "✓ Leading zeros de 1: " << one.leading_zeros() << std::endl;

    uint128_t high_bit(0x8000000000000000ull, 0);
    assert(high_bit.leading_zeros() == 0);
    std::cout << "✓ Leading zeros con MSB = 1: " << high_bit.leading_zeros() << std::endl;

    uint128_t mid_low(0, 0x1000000000000000ull); // bit 60 activado
    assert(mid_low.leading_zeros() == 67);       // 127 - 60
    std::cout << "✓ Leading zeros de número con bit 60: " << mid_low.leading_zeros() << std::endl;

    std::cout << std::endl;
}

// Test de incremento/decremento
void test_increment_decrement()
{
    std::cout << "=== Test de Incremento/Decremento ===" << std::endl;

    uint128_t val(0, 5);

    // Test pre-incremento
    ++val;
    assert(val == uint128_t(0, 6));
    std::cout << "✓ Pre-incremento: " << val.to_string() << std::endl;

    // Test post-incremento
    uint128_t old_val = val++;
    assert(old_val == uint128_t(0, 6));
    assert(val == uint128_t(0, 7));
    std::cout << "✓ Post-incremento: " << val.to_string() << std::endl;

    // Test decremento
    --val;
    assert(val == uint128_t(0, 6));
    std::cout << "✓ Pre-decremento: " << val.to_string() << std::endl;

    // Test incremento con overflow
    uint128_t overflow_test(0, UINT64_MAX);
    ++overflow_test;
    assert(overflow_test == uint128_t(1, 0));
    std::cout << "✓ Incremento con overflow: " << overflow_test.to_string() << std::endl;

    // Test decremento con borrow
    uint128_t borrow_test(1, 0);
    --borrow_test;
    assert(borrow_test == uint128_t(0, UINT64_MAX));
    std::cout << "✓ Decremento con borrow: " << borrow_test.to_string() << std::endl;

    std::cout << std::endl;
}

// Test de división
void test_division()
{
    std::cout << "=== Test de División ===" << std::endl;

    uint128_t dividend(0, 100);
    uint128_t divisor(0, 7);

    auto divrem_result = dividend.divrem(divisor);
    assert(divrem_result.has_value());

    uint128_t quotient = divrem_result.value().first;
    uint128_t remainder = divrem_result.value().second;

    assert(quotient == uint128_t(0, 14));
    assert(remainder == uint128_t(0, 2));

    std::cout << "✓ División: 100 ÷ 7 = " << quotient.to_string() << " resto "
              << remainder.to_string() << std::endl;

    // Verificar que quotient * divisor + remainder = dividend
    uint128_t verification = quotient * divisor + remainder;
    assert(verification == dividend);
    std::cout << "✓ Verificación: 14 × 7 + 2 = " << verification.to_string() << std::endl;

    std::cout << std::endl;
}

// Test de conversión a string
void test_string_conversion()
{
    std::cout << "=== Test de Conversión a String ===" << std::endl;

    uint128_t test1(0, 12345);
    std::string str1 = test1.to_string();
    assert(str1 == "12345");
    std::cout << "✓ to_string(12345): " << str1 << std::endl;

    uint128_t test2 = uint128_t::from_string("67890");
    assert(test2 == uint128_t(0, 67890));
    std::cout << "✓ from_string(\"67890\"): " << test2.to_string() << std::endl;

    // Test bidireccional
    uint128_t original(0, 9876543210ull);
    std::string str_rep = original.to_string();
    uint128_t converted = uint128_t::from_string(str_rep);
    assert(converted == original);
    std::cout << "✓ Conversión bidireccional: " << str_rep << std::endl;

    std::cout << std::endl;
}

int main()
{
    try {
        print_compiler_info();
        test_arithmetic();
        test_leading_zeros();
        test_increment_decrement();
        test_division();
        test_string_conversion();

        std::cout << "🎉 ¡Todos los tests pasaron! El código es compatible multiplataforma."
                  << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Error desconocido" << std::endl;
        return 1;
    }
}