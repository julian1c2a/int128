#include "include_new/int128_base_tt.hpp"
#include <iostream>
using namespace nstd;
int main() {
    std::cout << "Starting..." << std::endl;
    uint128_t q(0, 10);
    uint128_t m(0, 10);
    std::cout << "Before multiplication" << std::endl;
    uint128_t qm = q * m;
    std::cout << "After multiplication" << std::endl;
    std::cout << "q*m = " << qm.to_string() << std::endl;
    return 0;
}

