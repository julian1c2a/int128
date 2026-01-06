// Test con debug traces
#include "int128.hpp"
#include <iostream>

using namespace nstd;

// Wrapper con tracing
std::optional<std::pair<uint128_t, uint128_t>> test_divrem(const uint128_t& dividend,
                                                           uint64_t divisor)
{
    std::cout << "=== Entrando a divrem ===\n";
    std::cout << "dividend.data[1] = " << dividend.high() << "\n";
    std::cout << "dividend.data[0] = " << dividend.low() << "\n";
    std::cout << "divisor_u64 = " << divisor << "\n";

    auto result = dividend.divrem(divisor);

    if (result.has_value()) {
        std::cout << "quotient.data[1] = " << result.value().first.high() << "\n";
        std::cout << "quotient.data[0] = " << result.value().first.low() << "\n";
        std::cout << "remainder.data[1] = " << result.value().second.high() << "\n";
        std::cout << "remainder.data[0] = " << result.value().second.low() << "\n";
    } else {
        std::cout << "Result: nullopt\n";
    }

    return result;
}

int main()
{
    uint128_t dividend(0, 1000);
    uint64_t divisor = 3;

    std::cout << "Test: 1000 / 3\n\n";

    auto result = test_divrem(dividend, divisor);

    if (!result.has_value()) {
        std::cout << "\nERROR: divrem retorno nullopt\n";
        return 1;
    }

    auto [quotient, remainder] = result.value();

    std::cout << "\nResultado:\n";
    std::cout << "Cociente = " << quotient.to_string() << " (esperado: 333)\n";
    std::cout << "Resto = " << remainder.to_string() << " (esperado: 1)\n";

    // Verificar
    uint128_t verification = quotient * uint128_t(divisor) + remainder;
    std::cout << "Verificacion: " << verification.to_string() << "\n";

    if (verification == dividend && quotient.low() == 333 && remainder.low() == 1) {
        std::cout << "OK PASS\n";
        return 0;
    } else {
        std::cout << "FAIL FAIL\n";
        return 1;
    }
}
