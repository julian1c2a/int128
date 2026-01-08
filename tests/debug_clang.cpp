#include <iostream>
#include "../include/int128_base_tt.hpp"
using namespace nstd;
int main() {
    std::cout << "Starting..." << std::endl;
    uint128_t u(42);
    std::cout << "Created uint128_t: " << u.low() << std::endl;
    int128_t i(-42);
    std::cout << "Created int128_t: " << i.low() << std::endl;
    std::cout << "Division test: " << (int128_t(-100) / int128_t(10)) << std::endl;
    return 0;
}
