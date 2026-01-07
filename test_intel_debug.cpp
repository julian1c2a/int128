// Test minimo para diagnosticar problema Intel icx
#include "include_new/int128_base_tt.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t n(0, 100);
    uint128_t m(0, 10);

    std::cout << "n = " << n.to_string() << std::endl;
    std::cout << "m = " << m.to_string() << std::endl;

    auto [q, r] = n.divrem(m);

    std::cout << "q = " << q.to_string() << std::endl;
    std::cout << "r = " << r.to_string() << std::endl;

    // Verificacion manual
    uint128_t qm = q * m;
    std::cout << "q * m = " << qm.to_string() << std::endl;
    std::cout << "qm.high = " << qm.high() << ", qm.low = " << qm.low() << std::endl;

    // Test suma simple
    std::cout << "\n--- TEST SUMA SIMPLE ---" << std::endl;
    uint128_t a(0, 100);
    uint128_t b(0, 0);
    uint128_t c = a + b;
    std::cout << "100 + 0 = " << c.to_string() << std::endl;

    uint128_t d(0, 50);
    uint128_t e(0, 25);
    uint128_t f = d + e;
    std::cout << "50 + 25 = " << f.to_string() << std::endl;

    // Test qm + r
    std::cout << "\n--- TEST QM + R ---" << std::endl;
    uint128_t qm_plus_r = qm + r;
    std::cout << "qm + r = " << qm_plus_r.to_string() << std::endl;
    std::cout << "qm_plus_r.high = " << qm_plus_r.high() << ", qm_plus_r.low = " << qm_plus_r.low()
              << std::endl;

    bool prop8 = (qm_plus_r == n);
    std::cout << "q*m + r == n? " << (prop8 ? "SI" : "NO") << std::endl;

    return prop8 ? 0 : 1;
}
