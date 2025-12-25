#include "../include/uint128_t.hpp"
#include <iostream>
#include <cassert>

int main()
{
    std::cout << "Probando simetría de operadores...\n";

    uint128_t a(10, 20);
    int b = 5;
    uint64_t c = 100;

    // Test simetría aritmética
    {
        auto result1 = a + b; // uint128_t + int
        auto result2 = b + a; // int + uint128_t (usando friend)

        // Deberían ser iguales
        assert(result1.high() == result2.high());
        assert(result1.low() == result2.low());
        std::cout << "[OK] Simetría en suma (+)\n";
    }

    // Test simetría en comparación
    {
        uint128_t x(0, 50);
        int y = 60;

        bool comp1 = x < y; // uint128_t < int
        bool comp2 = y > x; // int > uint128_t (usando friend)

        assert(comp1 == comp2); // Ambas deberían ser true
        std::cout << "[OK] Simetría en comparación (</>)\n";
    }

    // Test simetría en operadores bitwise
    {
        uint128_t x(0, 0xFF);
        int mask = 0x0F;

        auto result1 = x & mask; // uint128_t & int
        auto result2 = mask & x; // int & uint128_t (usando friend)

        assert(result1.high() == result2.high());
        assert(result1.low() == result2.low());
        std::cout << "[OK] Simetría en AND bitwise (&)\n";
    }

    // Test con diferentes tipos integrales
    {
        uint128_t x(0, 10);
        uint64_t large = 1000000000ULL;

        auto result1 = x * large; // uint128_t * uint64_t
        auto result2 = large * x; // uint64_t * uint128_t (friend)

        assert(result1.high() == result2.high());
        assert(result1.low() == result2.low());
        std::cout << "[OK] Simetría con uint64_t (*)\n";
    }

    std::cout << "\n¡Todas las pruebas de simetría pasaron! 🎉\n";
    return 0;
}
