#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test simple de parsing\n";

    // Test directo de parse()
    auto [error1, value1] = uint128_t::parse("12345");
    std::cout << "parse(\"12345\"): error=" << static_cast<int>(error1)
              << ", value=" << value1.low() << "\n";

    // Test directo de parse_base()
    auto [error2, value2] = uint128_t::parse_base("12345", 10);
    std::cout << "parse_base(\"12345\", 10): error=" << static_cast<int>(error2)
              << ", value=" << value2.low() << "\n";

    // Test constructor
    try {
        uint128_t u("12345");
        std::cout << "Constructor exitoso: " << u.low() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Constructor fallo: " << e.what() << "\n";
    }

    return 0;
}
