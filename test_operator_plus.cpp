// Test detallado para diagnosticar operator+= en Intel ICX

#include "include_new/int128_base_tt.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Debug operator+= ===" << std::endl;

    // Crear dos numeros
    uint128_t a(100);
    uint128_t b(42);

    std::cout << "\n--- ANTES DE SUMA ---" << std::endl;
    std::cout << "a = " << a.to_string() << " (low=" << a.low() << ", high=" << a.high() << ")"
              << std::endl;
    std::cout << "b = " << b.to_string() << " (low=" << b.low() << ", high=" << b.high() << ")"
              << std::endl;

    // Test de b.low()
    std::cout << "\n--- Test b.low() ---" << std::endl;
    uint64_t b_low = b.low();
    std::cout << "b.low() = " << b_low << std::endl;

    // Ahora la suma
    a += b;

    std::cout << "\n--- DESPUES DE SUMA ---" << std::endl;
    std::cout << "a = a + b = " << a.to_string() << " (low=" << a.low() << ", high=" << a.high()
              << ")" << std::endl;
    std::cout << "Esperado: 142 (low=142, high=0)" << std::endl;

    std::cout << "\nTest: " << (a.low() == 142 && a.high() == 0 ? "PASS" : "FAIL") << std::endl;

    // Test directo de operator+
    std::cout << "\n--- Test operator+ ---" << std::endl;
    uint128_t c(50);
    uint128_t d(25);
    uint128_t e = c + d;
    std::cout << "50 + 25 = " << e.to_string() << " (low=" << e.low() << ", high=" << e.high()
              << ")" << std::endl;
    std::cout << "Test: " << (e.low() == 75 && e.high() == 0 ? "PASS" : "FAIL") << std::endl;

    return 0;
}
