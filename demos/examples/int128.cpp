#include <cctype>
#include <cstddef>
#include <cstdint>
#include <int128.hpp>
#include <iomanip>
#include <iostream>

int main()
{
    // Prueba: Crear un entero de 128 bits
    // High: 0x1111222233334444
    // Low:  0x5555666677778888
    uint128_t num(0x1111222233334444, 0x5555666677778888);

    std::cout << "Valor 128-bit (Hex): 0x";
    std::cout << std::hex << std::uppercase << std::setfill('0');
    std::cout << std::setw(16) << num.high() << std::setw(16) << num.low() << "\n";

    return 0;
}
