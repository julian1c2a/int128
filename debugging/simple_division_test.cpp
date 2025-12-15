#include "include/uint128_t.hpp"
#include <iostream>

int main()
{
    try {
        uint128_t a(100);
        uint128_t b(7);

        auto result = a.divrem(b);
        if (result.has_value()) {
            auto quotient = result.value().first;
            auto remainder = result.value().second;

            std::cout << "100 / 7 = " << static_cast<uint64_t>(quotient) << " rest "
                      << static_cast<uint64_t>(remainder) << std::endl;
        } else {
            std::cout << "Error en división" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}