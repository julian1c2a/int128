#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t maxval = uint128_t::max();

    std::cout << "max().low() = " << maxval.low() << "\n";
    std::cout << "max().high() = " << maxval.high() << "\n";

    uint128_t base_val{10ull, 0ull};
    uint128_t max_div_base = maxval / base_val;

    std::cout << "max_div_base.low() = " << max_div_base.low() << "\n";
    std::cout << "max_div_base.high() = " << max_div_base.high() << "\n";

    uint128_t zero{0ull, 0ull};
    std::cout << "zero > max_div_base = " << (zero > max_div_base ? "TRUE [BUG!]" : "false")
              << "\n";

    return 0;
}
