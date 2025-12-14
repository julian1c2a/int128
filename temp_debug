/*
 * Test rápido para depurar to_string_base
 */

#include "../include/int128_t.hpp"
#include <iostream>

int main()
{
    int128_t val(255);
    std::cout << "int128_t(255).to_string_base(16) = '" << val.to_string_base(16) << "'"
              << std::endl;
    std::cout << "Expected: 'ff'" << std::endl;

    // Probar otros valores
    std::cout << "int128_t(15).to_string_base(16) = '" << int128_t(15).to_string_base(16) << "'"
              << std::endl;
    std::cout << "int128_t(16).to_string_base(16) = '" << int128_t(16).to_string_base(16) << "'"
              << std::endl;

    return 0;
}