#include "include/uint128_t.hpp"
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== Test del Algoritmo D de Knuth ===" << std::endl;

    // Test 1: División básica
    uint128_t dividend(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    uint128_t divisor(0, 0x123456789ABCDEFULL);

    auto result = dividend.knuth_D_divrem(divisor);

    if (result.has_value()) {
        auto [quotient, remainder] = result.value();

        std::cout << "Test 1: División básica" << std::endl;
        std::cout << "Dividendo:  " << dividend << std::endl;
        std::cout << "Divisor:    " << divisor << std::endl;
        std::cout << "Cociente:   " << quotient << std::endl;
        std::cout << "Residuo:    " << remainder << std::endl;

        // Verificación: dividend == quotient * divisor + remainder
        uint128_t verification = quotient * divisor + remainder;
        std::cout << "Verificación: " << (verification == dividend ? "CORRECTO" : "ERROR")
                  << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "Error: División por cero detectada incorrectamente" << std::endl;
    }

    // Test 2: División por cero
    auto zero_result = dividend.knuth_D_divrem(uint128_t(0, 0));
    std::cout << "Test 2: División por cero" << std::endl;
    std::cout << "Resultado: "
              << (zero_result.has_value() ? "ERROR - no detectó división por cero"
                                          : "CORRECTO - detectó división por cero")
              << std::endl;
    std::cout << std::endl;

    // Test 3: Dividendo menor que divisor
    uint128_t small_dividend(0, 0x123ULL);
    uint128_t large_divisor(0, 0x456789ABULL);

    auto small_result = small_dividend.knuth_D_divrem(large_divisor);
    if (small_result.has_value()) {
        auto [q, r] = small_result.value();
        std::cout << "Test 3: Dividendo < Divisor" << std::endl;
        std::cout << "Cociente: " << q << " (debería ser 0)" << std::endl;
        std::cout << "Residuo:  " << r << " (debería ser igual al dividendo)" << std::endl;
        std::cout << "Verificación: "
                  << (q == uint128_t(0, 0) && r == small_dividend ? "CORRECTO" : "ERROR")
                  << std::endl;
        std::cout << std::endl;
    }

    // Test 4: División con divisor de 128 bits
    uint128_t big_dividend(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    uint128_t big_divisor(0x8000000000000000ULL, 0x0ULL);

    auto big_result = big_dividend.knuth_D_divrem(big_divisor);
    if (big_result.has_value()) {
        auto [q, r] = big_result.value();
        std::cout << "Test 4: División de 128 bits" << std::endl;
        std::cout << "Dividendo:  " << big_dividend << std::endl;
        std::cout << "Divisor:    " << big_divisor << std::endl;
        std::cout << "Cociente:   " << q << std::endl;
        std::cout << "Residuo:    " << r << std::endl;

        // Verificación
        uint128_t verification = q * big_divisor + r;
        std::cout << "Verificación: " << (verification == big_dividend ? "CORRECTO" : "ERROR")
                  << std::endl;
    }

    return 0;
}