// Test simple para debugging divrem
#include "int128.hpp"
#include <iomanip>
#include <iostream>

using namespace nstd;

int main()
{
    // Caso simple: 1000 / 3 = 333 resto 1
    uint128_t dividend(0, 1000);
    uint64_t divisor = 3;

    std::cout << "Test simple: 1000 / 3\n";
    std::cout << "Dividendo: " << dividend.to_string() << "\n";
    std::cout << "Divisor: " << divisor << "\n";

    auto result = dividend.divrem(divisor);
    if (!result.has_value()) {
        std::cout << "ERROR: divrem retornó nullopt\n";
        return 1;
    }

    auto [quotient, remainder] = result.value();

    std::cout << "Cociente: " << quotient.to_string() << "\n";
    std::cout << "Resto: " << remainder.to_string() << "\n";

    // Verificar
    uint128_t verification = quotient * uint128_t(divisor) + remainder;
    std::cout << "Verificación: " << verification.to_string() << "\n";

    if (verification == dividend) {
        std::cout << "✓ PASS\n";
        return 0;
    } else {
        std::cout << "✗ FAIL: " << verification.to_string() << " != " << dividend.to_string()
                  << "\n";
        return 1;
    }
}
