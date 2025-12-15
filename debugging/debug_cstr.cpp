#include "include/uint128_t.hpp"
#include <iostream>

int main()
{
    uint128_t test(0, 12345);

    std::cout << "Número original: " << test.to_string() << std::endl;
    std::cout << "to_cstr resultado: '" << test.to_cstr() << "'" << std::endl;
    std::cout << "to_cstr_base(10) resultado: '" << test.to_cstr_base(10) << "'" << std::endl;

    // Test con números pequeños
    uint128_t small(0, 5);
    std::cout << "Número 5: '" << small.to_cstr() << "'" << std::endl;

    uint128_t zero(0, 0);
    std::cout << "Número 0: '" << zero.to_cstr() << "'" << std::endl;

    return 0;
}