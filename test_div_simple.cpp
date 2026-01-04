#include "int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test simple de division\n";

    uint128_t a(100);
    uint128_t b(10);

    std::cout << "a.low() = " << a.low() << ", a.high() = " << a.high() << "\n";
    std::cout << "b.low() = " << b.low() << ", b.high() = " << b.high() << "\n";

    auto [q, r] = a.divrem(b);

    std::cout << "quotient.low() = " << q.low() << ", quotient.high() = " << q.high() << "\n";
    std::cout << "remainder.low() = " << r.low() << ", remainder.high() = " << r.high() << "\n";

    auto result = a / b;
    std::cout << "a / b = " << result.low() << ", " << result.high() << "\n";

    return 0;
}
