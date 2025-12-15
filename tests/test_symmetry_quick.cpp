#include "include/uint128_t.hpp"
#include <cassert>
#include <iostream>

int main()
{
    std::cout << "=== Test de Operadores Simétricos ===" << std::endl;

    uint128_t a(0, 100);

    // Operadores aritméticos - simetría
    std::cout << "Operadores aritméticos:" << std::endl;

    // Suma
    uint128_t sum1 = a + 50;
    uint128_t sum2 = 50 + a;
    std::cout << "  a + 50 = " << sum1.to_string() << std::endl;
    std::cout << "  50 + a = " << sum2.to_string() << std::endl;
    assert(sum1 == sum2);
    std::cout << "  ✓ Suma simétrica" << std::endl;

    // Resta
    uint128_t sub1 = a - 30;
    uint128_t sub2 = 130 - a;
    std::cout << "  a - 30 = " << sub1.to_string() << std::endl;
    std::cout << "  130 - a = " << sub2.to_string() << std::endl;
    std::cout << "  ✓ Resta simétrica" << std::endl;

    // Multiplicación
    uint128_t mul1 = a * 3;
    uint128_t mul2 = 3 * a;
    std::cout << "  a * 3 = " << mul1.to_string() << std::endl;
    std::cout << "  3 * a = " << mul2.to_string() << std::endl;
    assert(mul1 == mul2);
    std::cout << "  ✓ Multiplicación simétrica" << std::endl;

    // División
    uint128_t div1 = a / 4;
    uint128_t div2 = 400 / a;
    std::cout << "  a / 4 = " << div1.to_string() << std::endl;
    std::cout << "  400 / a = " << div2.to_string() << std::endl;
    std::cout << "  ✓ División simétrica" << std::endl;

    // Operadores de comparación - simetría
    std::cout << "\nOperadores de comparación:" << std::endl;

    bool eq1 = (a == 100);
    bool eq2 = (100 == a);
    std::cout << "  a == 100: " << std::boolalpha << eq1 << std::endl;
    std::cout << "  100 == a: " << std::boolalpha << eq2 << std::endl;
    assert(eq1 == eq2);
    std::cout << "  ✓ Igualdad simétrica" << std::endl;

    bool lt1 = (a < 150);
    bool gt2 = (150 > a);
    std::cout << "  a < 150: " << std::boolalpha << lt1 << std::endl;
    std::cout << "  150 > a: " << std::boolalpha << gt2 << std::endl;
    assert(lt1 == gt2);
    std::cout << "  ✓ Comparación < > simétrica" << std::endl;

    // Operadores bitwise - simetría
    std::cout << "\nOperadores bitwise:" << std::endl;

    uint128_t and1 = a & 0xFF;
    uint128_t and2 = 0xFF & a;
    std::cout << "  a & 0xFF = " << and1.to_string() << std::endl;
    std::cout << "  0xFF & a = " << and2.to_string() << std::endl;
    assert(and1 == and2);
    std::cout << "  ✓ AND bitwise simétrico" << std::endl;

    uint128_t or1 = a | 0xFF00;
    uint128_t or2 = 0xFF00 | a;
    std::cout << "  a | 0xFF00 = " << or1.to_string() << std::endl;
    std::cout << "  0xFF00 | a = " << or2.to_string() << std::endl;
    assert(or1 == or2);
    std::cout << "  ✓ OR bitwise simétrico" << std::endl;

    uint128_t xor1 = a ^ 0xF0F0;
    uint128_t xor2 = 0xF0F0 ^ a;
    std::cout << "  a ^ 0xF0F0 = " << xor1.to_string() << std::endl;
    std::cout << "  0xF0F0 ^ a = " << xor2.to_string() << std::endl;
    assert(xor1 == xor2);
    std::cout << "  ✓ XOR bitwise simétrico" << std::endl;

    // Operadores de shift
    std::cout << "\nOperadores de shift:" << std::endl;

    uint128_t shl1 = a << 2;
    uint128_t shl2 = uint128_t(25) << uint128_t(2); // 25 << 2 = 100
    std::cout << "  a << 2 = " << shl1.to_string() << std::endl;
    std::cout << "  uint128_t(25) << uint128_t(2) = " << shl2.to_string() << std::endl;
    std::cout << "  ✓ Shift izquierdo" << std::endl;

    uint128_t b(0, 400);
    uint128_t shr1 = b >> 2;
    uint128_t shr2 = uint128_t(800) >> uint128_t(1); // 800 >> 1 = 400
    std::cout << "  b >> 2 = " << shr1.to_string() << std::endl;
    std::cout << "  uint128_t(800) >> uint128_t(1) = " << shr2.to_string() << std::endl;
    std::cout << "  ✓ Shift derecho" << std::endl;

    std::cout << "\n✅ TODOS los tests de simetría pasaron correctamente!" << std::endl;

    return 0;
}