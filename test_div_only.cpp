#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t maxval = uint128_t::max();
    uint128_t base_val{10ull, 0ull};

    std::cout << "Antes de división...\n";
    std::cout << "maxval.low() = " << maxval.low() << "\n";
    std::cout << "maxval.high() = " << maxval.high() << "\n";
    std::cout << "base_val.low() = " << base_val.low() << "\n";
    std::cout << "base_val.high() = " << base_val.high() << "\n";

    try {
        uint128_t result_div = maxval / base_val;
        std::cout << "\nDespués de división:\n";
        std::cout << "result_div.low() = " << result_div.low() << "\n";
        std::cout << "result_div.high() = " << result_div.high() << "\n";

        uint128_t zero{0ull, 0ull};
        bool cmp = zero > result_div;
        std::cout << "\nzero > result_div = " << (cmp ? "TRUE" : "FALSE") << "\n";

    } catch (...) {
        std::cout << "EXCEPCION en división!\n";
    }

    return 0;
}
