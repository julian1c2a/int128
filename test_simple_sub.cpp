#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t u1(100);
    std::cout << "u1.low() inicial: " << u1.low() << std::endl;

    // Probar operator-= con builtin
    u1 -= 30;

    std::cout << "u1.low() despues de -= 30: " << u1.low() << std::endl;
    std::cout << "Esperado: 70" << std::endl;

    if (u1.low() == 70) {
        std::cout << "OK CORRECTO" << std::endl;
    } else {
        std::cout << "FAIL ERROR: Resultado = " << u1.low() << std::endl;
    }

    return 0;
}
