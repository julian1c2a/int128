/**
 * @file test_divrem_compile.cpp
 * @brief Test basico de compilacion para divrem en int128_base_tt.hpp
 */

#include "include_new/int128_base_tt.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test de Compilacion: divrem ===" << std::endl;

    // Test basico de division
    uint128_t a(0, 100);
    uint128_t b(0, 10);
    auto [q, r] = a.divrem(b);

    std::cout << "100 / 10 = " << q.to_string() << " remainder " << r.to_string() << std::endl;

    // Test de division por potencia de 2
    uint128_t c(0, 1024);
    uint128_t d(0, 16); // 16 = 2^4
    auto [q2, r2] = c.divrem(d);

    std::cout << "1024 / 16 = " << q2.to_string() << " remainder " << r2.to_string() << std::endl;

    std::cout << "\nOK Compilacion exitosa" << std::endl;
    return 0;
}
