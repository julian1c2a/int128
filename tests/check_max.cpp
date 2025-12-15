#include "include/uint128_t.hpp"
#include <iostream>
#include <limits>

using namespace uint128_literals;

int main()
{
    auto max_from_limits = std::numeric_limits<uint128_t>::max();
    std::cout << "max from numeric_limits = " << max_from_limits << std::endl;

    // Crear el valor máximo manualmente
    uint128_t max_manual = ~uint128_t(0); // Todos los bits en 1
    std::cout << "max manual = " << max_manual << std::endl;

    // Verificar que son diferentes
    std::cout << "Are they equal? " << (max_from_limits == max_manual) << std::endl;

    return 0;
}