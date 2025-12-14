#include "include/uint128_t.hpp"
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== Debug del Test 4 ===" << std::endl;

    // Test 4: División con divisor de 128 bits
    uint128_t big_dividend(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    uint128_t big_divisor(0x8000000000000000ULL, 0x0ULL);

    std::cout << "Dividendo: " << std::hex << big_dividend.high() << " " << big_dividend.low()
              << std::dec << std::endl;
    std::cout << "Divisor:   " << std::hex << big_divisor.high() << " " << big_divisor.low()
              << std::dec << std::endl;

    // Calcular manualmente lo que debería ser
    // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF / 80000000000000000000000000000000
    // = 1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (aproximadamente 2)

    auto big_result = big_dividend.knuth_D_divrem(big_divisor);
    if (big_result.has_value()) {
        auto [q, r] = big_result.value();
        std::cout << "Cociente calculado: " << q << " (hex: " << std::hex << q.high() << " "
                  << q.low() << ")" << std::dec << std::endl;
        std::cout << "Residuo calculado:  " << r << " (hex: " << std::hex << r.high() << " "
                  << r.low() << ")" << std::dec << std::endl;

        // Verificación manual
        uint128_t verification = q * big_divisor + r;
        std::cout << "Verificación: " << verification << " (hex: " << std::hex
                  << verification.high() << " " << verification.low() << ")" << std::dec
                  << std::endl;
        std::cout << "Original:     " << big_dividend << " (hex: " << std::hex
                  << big_dividend.high() << " " << big_dividend.low() << ")" << std::dec
                  << std::endl;

        std::cout << "Match: " << (verification == big_dividend ? "CORRECTO" : "ERROR")
                  << std::endl;

        // Cálculo esperado manual:
        // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF = 2 * 8000000000000000 0000000000000000 - 1
        uint128_t expected_quotient(0, 1); // Debería ser 1
        uint128_t expected_remainder = big_dividend - (expected_quotient * big_divisor);
        std::cout << std::endl << "Cálculo manual esperado:" << std::endl;
        std::cout << "Cociente esperado: " << expected_quotient << std::endl;
        std::cout << "Residuo esperado:  " << expected_remainder << std::endl;
    }

    // También probemos con el algoritmo divrem regular
    std::cout << std::endl << "=== Comparación con divrem regular ===" << std::endl;
    auto regular_result = big_dividend.divrem(big_divisor);
    if (regular_result.has_value()) {
        auto [q, r] = regular_result.value();
        std::cout << "divrem() Cociente: " << q << std::endl;
        std::cout << "divrem() Residuo:  " << r << std::endl;

        uint128_t verification = q * big_divisor + r;
        std::cout << "divrem() Verificación: "
                  << (verification == big_dividend ? "CORRECTO" : "ERROR") << std::endl;
    }

    return 0;
}