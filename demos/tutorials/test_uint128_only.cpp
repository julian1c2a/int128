/**
 * Test simple para verificar compilación - solo uint128_t
 */

#include <iostream>
#include <uint128/uint128_iostreams.hpp>
#include <uint128/uint128_t.hpp>

int main()
{
    std::cout << "Test de compilación uint128_t\n";
    std::cout << "==============================\n\n";

    // Test 1: Crear números básicos
    uint128_t a = 1000;
    uint128_t b = 2000;

    std::cout << "Suma: " << a << " + " << b << " = " << (a + b) << "\n";

    // Test 2: Multiplicación
    uint128_t prod = a * b;
    std::cout << "Producto: " << a << " * " << b << " = " << prod << "\n";

    // Test 3: División
    uint128_t quot = b / a;
    std::cout << "División: " << b << " / " << a << " = " << quot << "\n";

    // Test 4: Módulo
    uint128_t mod = b % a;
    std::cout << "Módulo: " << b << " % " << a << " = " << mod << "\n";

    std::cout << "\n✅ Todos los tests pasaron correctamente!\n";

    return 0;
}
