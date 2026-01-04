#include <int128_base.hpp>
#include <iostream>

using namespace nstd;

int main()
{
    uint128_t a(100);
    uint128_t b(200);

    std::cout << "a.low() = " << a.low() << ", a.high() = " << a.high() << std::endl;
    std::cout << "b.low() = " << b.low() << ", b.high() = " << b.high() << std::endl;

    uint128_t c = a + b;

    std::cout << "c.low() = " << c.low() << ", c.high() = " << c.high() << std::endl;
    std::cout << "Expected: c.low() = 300, c.high() = 0" << std::endl;

    return 0;
}
