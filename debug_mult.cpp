#include "include/uint128_t.hpp"
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== Debug detallado de multiplicación ===" << std::endl;

    uint128_t q(0, 1); // quotient = 1
    uint128_t divisor(0x8000000000000000ULL, 0x0ULL);
    uint128_t r(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // remainder

    std::cout << "q = " << q << " (hex: " << std::hex << q.high() << " " << q.low() << ")"
              << std::dec << std::endl;
    std::cout << "divisor = " << divisor << " (hex: " << std::hex << divisor.high() << " "
              << divisor.low() << ")" << std::dec << std::endl;
    std::cout << "r = " << r << " (hex: " << std::hex << r.high() << " " << r.low() << ")"
              << std::dec << std::endl;

    uint128_t product = q * divisor;
    std::cout << "q * divisor = " << product << " (hex: " << std::hex << product.high() << " "
              << product.low() << ")" << std::dec << std::endl;

    uint128_t sum = product + r;
    std::cout << "q * divisor + r = " << sum << " (hex: " << std::hex << sum.high() << " "
              << sum.low() << ")" << std::dec << std::endl;

    uint128_t expected(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "expected = " << expected << " (hex: " << std::hex << expected.high() << " "
              << expected.low() << ")" << std::dec << std::endl;

    std::cout << "¿Son iguales? " << (sum == expected ? "SÍ" : "NO") << std::endl;

    // Vamos a calcular manualmente
    // q=1, divisor = 0x8000000000000000 0000000000000000
    // 1 * 0x8000000000000000 0000000000000000 = 0x8000000000000000 0000000000000000
    // + r = 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF
    // = 0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF ✓

    std::cout << std::endl << "=== Cálculo manual ===" << std::endl;
    std::cout << "1 * 0x8000000000000000 0000000000000000 = 0x8000000000000000 0000000000000000"
              << std::endl;
    std::cout << "+ 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF" << std::endl;
    std::cout << "= 0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF" << std::endl;

    // ¡Debería ser correcto! Veamos qué está pasando con nuestra suma...
    uint128_t manual_product(0x8000000000000000ULL, 0x0ULL);
    uint128_t manual_sum = manual_product + r;
    std::cout << "Manual: " << manual_sum << " (hex: " << std::hex << manual_sum.high() << " "
              << manual_sum.low() << ")" << std::dec << std::endl;

    return 0;
}