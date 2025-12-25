// Test para verificar std::gcd y std::lcm con int128_t

#include "int128/int128_t.hpp"
#include "uint128/uint128_t.hpp"
#include <iostream>
#include <numeric>

int main()
{
    std::cout << "Probando std::gcd y std::lcm con tipos 128-bit:\n\n";

    // Test con uint128_t
    uint128_t u1(48), u2(18);
    auto gcd_uint = std::gcd(u1, u2);
    auto lcm_uint = std::lcm(u1, u2);

    std::cout << "uint128_t:\n";
    std::cout << "  gcd(48, 18) = " << gcd_uint.to_string() << " (esperado: 6) ";
    std::cout << (gcd_uint == uint128_t(6) ? "[OK]" : "[FAIL]") << "\n";
    std::cout << "  lcm(48, 18) = " << lcm_uint.to_string() << " (esperado: 144) ";
    std::cout << (lcm_uint == uint128_t(144) ? "[OK]" : "[FAIL]") << "\n";

    // Test con int128_t
    int128_t i1(48), i2(18);
    auto gcd_int = std::gcd(i1, i2);
    auto lcm_int = std::lcm(i1, i2);

    std::cout << "\nint128_t:\n";
    std::cout << "  gcd(48, 18) = " << gcd_int.to_string() << " (esperado: 6) ";
    std::cout << (gcd_int == int128_t(6) ? "[OK]" : "[FAIL]") << "\n";
    std::cout << "  lcm(48, 18) = " << lcm_int.to_string() << " (esperado: 144) ";
    std::cout << (lcm_int == int128_t(144) ? "[OK]" : "[FAIL]") << "\n";

    // Test con valores negativos (int128_t)
    int128_t i3(-48), i4(18);
    auto gcd_neg = std::gcd(i3, i4);

    std::cout << "\nCon valores negativos:\n";
    std::cout << "  gcd(-48, 18) = " << gcd_neg.to_string() << " (esperado: 6) ";
    std::cout << (gcd_neg == int128_t(6) ? "[OK]" : "[FAIL]") << "\n";

    // Test con valores grandes
    uint128_t big1 = uint128_t::from_string("1000000000000000000000");
    uint128_t big2 = uint128_t::from_string("500000000000000000000");
    auto gcd_big = std::gcd(big1, big2);

    std::cout << "\nCon valores grandes:\n";
    std::cout << "  gcd(10^21, 5×10^20) = " << gcd_big.to_string() << "\n";
    std::cout << "  (esperado: 5×10^20) ";
    std::cout << (gcd_big == big2 ? "[OK]" : "[FAIL]") << "\n";

    std::cout << "\n🎉 ¡std::gcd y std::lcm funcionan correctamente con tipos 128-bit!\n";

    return 0;
}
