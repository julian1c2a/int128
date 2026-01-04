#include <int128_base.hpp>
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t a(100);

    std::cout << "Constructor uint128_t(100):" << std::endl;
    std::cout << "a.data[0] (low) = " << a.low() << std::endl;
    std::cout << "a.data[1] (high) = " << a.high() << std::endl;

    // Probar directamente el acceso a data
    std::cout << "\nDirect data access:" << std::endl;
    std::cout << "a.data[0] should be 100" << std::endl;

    return 0;
}
