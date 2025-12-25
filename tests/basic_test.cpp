#include "../include/uint128_t.hpp"
#include <iostream>
#include <cassert>

int main()
{
    std::cout << "Iniciando tests de uint128_t...\n";

    // Test constructor por defecto
    {
        uint128_t a;
        assert(a.high() == 0);
        assert(a.low() == 0);
        std::cout << "[OK] Constructor por defecto\n";
    }

    // Test constructor de un valor
    {
        uint128_t a(123);
        assert(a.high() == 0);
        assert(a.low() == 123);
        std::cout << "[OK] Constructor de un valor\n";
    }

    // Test constructor de dos valores
    {
        uint128_t a(0x1234, 0x5678);
        assert(a.high() == 0x1234);
        assert(a.low() == 0x5678);
        std::cout << "[OK] Constructor de dos valores\n";
    }

    // Test copy constructor
    {
        uint128_t a(1, 2);
        uint128_t b = a;
        assert(b.high() == 1);
        assert(b.low() == 2);
        std::cout << "[OK] Copy constructor\n";
    }

    // Test assignment
    {
        uint128_t a(1, 2);
        uint128_t c;
        c = a;
        assert(c.high() == 1);
        assert(c.low() == 2);
        std::cout << "[OK] Assignment operator\n";
    }

    // Test escritura explícita
    {
        uint128_t a;
        a.set_high(0xABCD);
        a.set_low(0xEF12);
        assert(a.high() == 0xABCD);
        assert(a.low() == 0xEF12);
        std::cout << "[OK] Escritura explícita (set_high/set_low)\n";
    }

    // Test modificación
    {
        uint128_t a(0x1111, 0x2222);
        a.set_high(0x9999);
        assert(a.high() == 0x9999);
        assert(a.low() == 0x2222); // low no debe cambiar
        std::cout << "[OK] Modificación de parte alta\n";
    }

    std::cout << "\n¡Todos los tests pasaron! 🎉\n";
    return 0;
}
