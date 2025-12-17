/*
 * Test básico para int128_t
 */

#include "../include/int128/int128_t.hpp"
#include <cassert>
#include <iostream>

using namespace int128_literals;

void test_constructors()
{
    std::cout << "Testing constructors..." << std::endl;

    // Constructor por defecto
    int128_t a;
    assert(a.is_zero());
    assert(!a.is_negative());
    assert(!a.is_positive());

    // Constructor desde int64_t positivo
    int128_t b(42);
    assert(b.is_positive());
    assert(!b.is_negative());
    assert(!b.is_zero());

    // Constructor desde int64_t negativo
    int128_t c(-42);
    assert(c.is_negative());
    assert(!c.is_positive());
    assert(!c.is_zero());

    // Constructor desde uint64_t
    int128_t d(UINT64_MAX);
    assert(d.is_positive());
    assert(d.high() == 0);
    assert(d.low() == UINT64_MAX);

    std::cout << "✅ Constructors OK" << std::endl;
}

void test_arithmetic()
{
    std::cout << "Testing arithmetic..." << std::endl;

    int128_t a(10);
    int128_t b(5);
    int128_t c(-3);

    // Suma
    assert(a + b == int128_t(15));
    assert(a + c == int128_t(7));
    assert(c + c == int128_t(-6));

    // Resta
    assert(a - b == int128_t(5));
    assert(a - c == int128_t(13));
    assert(c - a == int128_t(-13));

    // Multiplicación
    assert(a * b == int128_t(50));
    assert(a * c == int128_t(-30));
    assert(c * c == int128_t(9));

    // División
    assert(a / b == int128_t(2));
    assert(a / c == int128_t(-3));
    assert(c / int128_t(-1) == int128_t(3));

    // Módulo
    assert(a % b == int128_t(0));
    assert(int128_t(17) % int128_t(5) == int128_t(2));
    assert(int128_t(-17) % int128_t(5) == int128_t(-2)); // Signo del dividendo

    std::cout << "✅ Arithmetic OK" << std::endl;
}

void test_unary()
{
    std::cout << "Testing unary operations..." << std::endl;

    int128_t a(42);
    int128_t b(-42);

    // Negación
    assert(-a == b);
    assert(-b == a);
    assert(-int128_t(0) == int128_t(0));

    // Operador +
    assert(+a == a);
    assert(+b == b);

    // Incremento/decremento
    int128_t c(10);
    assert(++c == int128_t(11));
    assert(c++ == int128_t(11));
    assert(c == int128_t(12));

    assert(--c == int128_t(11));
    assert(c-- == int128_t(11));
    assert(c == int128_t(10));

    std::cout << "✅ Unary operations OK" << std::endl;
}

void test_comparisons()
{
    std::cout << "Testing comparisons..." << std::endl;

    int128_t pos(42);
    int128_t neg(-42);
    int128_t zero(0);
    int128_t pos2(100);

    // Igualdad
    assert(pos == pos);
    assert(neg == neg);
    assert(zero == zero);
    assert(!(pos == neg));

    // Orden con signos diferentes
    assert(neg < zero);
    assert(neg < pos);
    assert(zero < pos);
    assert(!(pos < neg));

    // Orden con mismo signo
    assert(pos < pos2);
    assert(!(pos2 < pos));
    assert(int128_t(-100) < neg);
    assert(!(neg < int128_t(-100)));

    // Otros operadores
    assert(neg <= zero);
    assert(zero <= pos);
    assert(pos <= pos);
    assert(pos2 > pos);
    assert(pos >= zero);

    std::cout << "✅ Comparisons OK" << std::endl;
}

void test_bitwise()
{
    std::cout << "Testing bitwise operations..." << std::endl;

    int128_t a(0b1100); // 12
    int128_t b(0b1010); // 10

    // AND
    assert((a & b) == int128_t(0b1000)); // 8

    // OR
    assert((a | b) == int128_t(0b1110)); // 14

    // XOR
    assert((a ^ b) == int128_t(0b0110)); // 6

    // NOT
    int128_t c(0);
    int128_t not_c = ~c;
    assert(not_c == int128_t(-1));

    std::cout << "✅ Bitwise operations OK" << std::endl;
}

void test_shifts()
{
    std::cout << "Testing shift operations..." << std::endl;

    // Shift izquierdo
    int128_t a(1);
    assert((a << 1) == int128_t(2));
    assert((a << 63) == int128_t(0x8000000000000000ULL));
    assert((a << 64) == int128_t(1ULL, 0));

    // Shift derecho aritmético (positivo)
    int128_t b(16);
    assert((b >> 1) == int128_t(8));
    assert((b >> 2) == int128_t(4));
    assert((b >> 4) == int128_t(1));
    assert((b >> 5) == int128_t(0));

    // Shift derecho aritmético (negativo) - extensión de signo
    int128_t c(-16);
    assert((c >> 1) == int128_t(-8));
    assert((c >> 2) == int128_t(-4));
    assert((c >> 4) == int128_t(-1));
    assert((c >> 100) == int128_t(-1)); // Extensión completa

    std::cout << "✅ Shift operations OK" << std::endl;
}

void test_string_conversion()
{
    std::cout << "Testing string conversions..." << std::endl;

    // Conversión a string
    assert(int128_t(0).to_string() == "0");
    assert(int128_t(42).to_string() == "42");
    assert(int128_t(-42).to_string() == "-42");
    assert(int128_t(123456789).to_string() == "123456789");
    assert(int128_t(-123456789).to_string() == "-123456789");

    // Parsing desde string
    assert(int128_t::from_string("0") == int128_t(0));
    assert(int128_t::from_string("42") == int128_t(42));
    assert(int128_t::from_string("-42") == int128_t(-42));
    assert(int128_t::from_string("+42") == int128_t(42));
    assert(int128_t::from_string("123456789") == int128_t(123456789));

    // Base 16
    assert(int128_t(255).to_string_base(16) == "FF");
    assert(int128_t(-255).to_string_base(16) == "-FF");

    std::cout << "✅ String conversions OK" << std::endl;
}

void test_literals()
{
    std::cout << "Testing literals..." << std::endl;

    auto a = 42_i128;
    assert(a == int128_t(42));

    auto b = "123456789012345678901234567890"_i128;
    assert(b.to_string() == "123456789012345678901234567890");

    auto c = "-987654321098765432109876543210"_i128;
    assert(c.to_string() == "-987654321098765432109876543210");
    assert(c.is_negative());

    std::cout << "✅ Literals OK" << std::endl;
}

void test_edge_cases()
{
    std::cout << "Testing edge cases..." << std::endl;

    // Valores límite
    int128_t max_val = int128_t_MAX;
    int128_t min_val = int128_t_MIN;

    assert(max_val.is_positive());
    assert(min_val.is_negative());
    assert(max_val > min_val);

    // Overflow en suma (comportamiento definido en complemento a 2)
    int128_t almost_max = max_val - int128_t(1);
    int128_t overflow_result = almost_max + int128_t(2);
    assert(overflow_result.is_negative()); // Wrap around

    // Negación del valor mínimo (caso especial)
    int128_t neg_min = -min_val;
    assert(neg_min == min_val); // -INT128_MIN == INT128_MIN en complemento a 2

    std::cout << "✅ Edge cases OK" << std::endl;
}

void test_iostream()
{
    std::cout << "Testing iostream operations..." << std::endl;

    // Output
    std::cout << "Positive: " << int128_t(42) << std::endl;
    std::cout << "Negative: " << int128_t(-42) << std::endl;
    std::cout << "Zero: " << int128_t(0) << std::endl;
    std::cout << "Large positive: " << "123456789012345678901234567890"_i128 << std::endl;
    std::cout << "Large negative: " << "-987654321098765432109876543210"_i128 << std::endl;

    std::cout << "✅ Iostream OK" << std::endl;
}

int main()
{
    std::cout << "=== Testing int128_t ===" << std::endl;

    try {
        test_constructors();
        test_arithmetic();
        test_unary();
        test_comparisons();
        test_bitwise();
        test_shifts();
        test_string_conversion();
        test_literals();
        test_edge_cases();
        test_iostream();

        std::cout << "\n🎉 ¡Todos los tests pasaron! int128_t funciona correctamente." << std::endl;
        std::cout << "\n📊 Datos internos:" << std::endl;
        std::cout << "• Rango: -2^127 a 2^127-1" << std::endl;
        std::cout << "• Representación: Complemento a 2" << std::endl;
        std::cout << "• MSB de data[1] es el bit de signo" << std::endl;
        std::cout << "• Misma estructura interna que uint128_t" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}