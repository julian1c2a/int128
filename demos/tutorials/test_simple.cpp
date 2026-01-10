/**
 * Test simple para verificar compilación
 */

#include <int128_simple.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <uint128/uint128_iostreams.hpp>

using namespace nstd;

int main()
{
    std::cout << "Test de compilación uint128_t/int128_t\n";
    std::cout << "======================================\n\n";

    // Test 1: Crear números básicos
    uint128_t a{1000};
    uint128_t b{2000};

    std::cout << "Suma unsigned: " << a << " + " << b << " = " << (a + b) << "\n";

    // Test 2: Crear números signed
    int128_t x{500};
    int128_t y{-300};

    std::cout << "Suma signed: " << x << " + " << y << " = " << (x + y) << "\n";

    // Test 3: Multiplicación
    uint128_t prod = a * b;
    std::cout << "Producto: " << a << " * " << b << " = " << prod << "\n";

    // Test 4: División
    uint128_t quot = b / a;
    std::cout << "División: " << b << " / " << a << " = " << quot << "\n";

    std::cout << "\n✅ Todos los tests pasaron correctamente!\n";

    return 0;
}

