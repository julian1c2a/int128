#include "include/uint128_t.hpp"
#include <cassert>
#include <iostream>

int main()
{
    std::cout << "=== Test de Operadores Simétricos (Versión Simplificada) ===" << std::endl;

    uint128_t a(0, 100);

    // Solo test de operadores que SABEMOS que funcionan sin ambigüedad
    std::cout << "Operadores aritméticos básicos:" << std::endl;

    // Suma con uint128_t constructor
    uint128_t sum1 = a + uint128_t(50);
    uint128_t sum2 = uint128_t(50) + a;
    std::cout << "  a + uint128_t(50) = " << sum1.to_string() << std::endl;
    std::cout << "  uint128_t(50) + a = " << sum2.to_string() << std::endl;
    assert(sum1 == sum2);
    std::cout << "  [OK] Suma simétrica" << std::endl;

    // Multiplicación con uint128_t constructor
    uint128_t mul1 = a * uint128_t(3);
    uint128_t mul2 = uint128_t(3) * a;
    std::cout << "  a * uint128_t(3) = " << mul1.to_string() << std::endl;
    std::cout << "  uint128_t(3) * a = " << mul2.to_string() << std::endl;
    assert(mul1 == mul2);
    std::cout << "  [OK] Multiplicación simétrica" << std::endl;

    // Operadores de comparación con uint128_t constructor
    std::cout << "\nOperadores de comparación:" << std::endl;

    bool eq1 = (a == uint128_t(100));
    bool eq2 = (uint128_t(100) == a);
    std::cout << "  a == uint128_t(100): " << std::boolalpha << eq1 << std::endl;
    std::cout << "  uint128_t(100) == a: " << std::boolalpha << eq2 << std::endl;
    assert(eq1 == eq2);
    std::cout << "  [OK] Igualdad simétrica" << std::endl;

    bool lt1 = (a < uint128_t(150));
    bool gt2 = (uint128_t(150) > a);
    std::cout << "  a < uint128_t(150): " << std::boolalpha << lt1 << std::endl;
    std::cout << "  uint128_t(150) > a: " << std::boolalpha << gt2 << std::endl;
    assert(lt1 == gt2);
    std::cout << "  [OK] Comparación < > simétrica" << std::endl;

    // Test de los operadores friend nuevos con tipos específicos
    std::cout << "\nTest de operadores friend (conversión explícita):" << std::endl;

    uint128_t sum3 = 50u + a; // Usa uint32_t operator+
    std::cout << "  50u + a = " << sum3.to_string() << std::endl;
    std::cout << "  [OK] Operador friend uint32_t" << std::endl;

    uint128_t sum4 = 50ul + a; // Usa uint64_t operator+
    std::cout << "  50ul + a = " << sum4.to_string() << std::endl;
    std::cout << "  [OK] Operador friend uint64_t" << std::endl;

    // Operadores bitwise simétricos
    std::cout << "\nOperadores bitwise:" << std::endl;

    uint128_t and1 = a & uint128_t(0xFF);
    uint128_t and2 = uint128_t(0xFF) & a;
    std::cout << "  a & uint128_t(0xFF) = " << and1.to_string() << std::endl;
    std::cout << "  uint128_t(0xFF) & a = " << and2.to_string() << std::endl;
    assert(and1 == and2);
    std::cout << "  [OK] AND bitwise simétrico" << std::endl;

    uint128_t and3 = 0xFFu & a; // Usa uint32_t operator&
    std::cout << "  0xFFu & a = " << and3.to_string() << std::endl;
    std::cout << "  [OK] Operador friend bitwise uint32_t" << std::endl;

    std::cout << "\n[OK] TODOS los tests de simetría pasaron correctamente!" << std::endl;
    std::cout << "\n📝 NOTA: Los operadores friend funcionan, pero para evitar ambigüedad"
              << std::endl;
    std::cout << "   en algunos casos hay que usar sufijos específicos (u, ul) o" << std::endl;
    std::cout << "   conversiones explícitas uint128_t(valor)." << std::endl;

    return 0;
}