#include "include_new/int128_base_tt.hpp"
#include <iostream>
using namespace nstd;
int main() {
    uint128_t n(0, 100);
    uint128_t m(0, 10);
    std::cout << "n = " << n.to_string() << std::endl;
    std::cout << "m = " << m.to_string() << std::endl;
    auto [q, r] = n.divrem(m);
    std::cout << "q = " << q.to_string() << std::endl;
    std::cout << "r = " << r.to_string() << std::endl;
    // Tests prop_5 y prop_8
    uint128_t qm = q * m;
    std::cout << "q*m = " << qm.to_string() << std::endl;
    uint128_t one(0, 1);
    uint128_t q_plus_1 = q + one;
    std::cout << "q+1 = " << q_plus_1.to_string() << std::endl;
    uint128_t qm_1 = q_plus_1 * m;
    std::cout << "(q+1)*m = " << qm_1.to_string() << std::endl;
    std::cout << "(q+1)*m > n? " << (qm_1 > n ? "yes" : "no") << std::endl;
    return 0;
}

