#include "include_new/int128_base.hpp"
#include <iostream>

using uint128_t = nstd::uint128_t;

int main()
{
    // Test copy constructor
    uint128_t a(100);
    std::cout << "Original a: low=" << a.low() << ", high=" << a.high() << std::endl;

    uint128_t b(a); // Copy constructor
    std::cout << "Copied b: low=" << b.low() << ", high=" << b.high() << std::endl;

    // Test operator+ step by step
    uint128_t c(200);
    std::cout << "c before: low=" << c.low() << ", high=" << c.high() << std::endl;

    // Create result manually to debug
    uint128_t result(a); // Copy a into result
    std::cout << "result after copy: low=" << result.low() << ", high=" << result.high()
              << std::endl;

    result += c; // Add c to result
    std::cout << "result after +=: low=" << result.low() << ", high=" << result.high() << std::endl;

    return 0;
}
