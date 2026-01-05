// Test sin usar to_string()
#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test int128_t parsing sin to_string()" << std::endl;

    // Test parse
    auto [ierr, ival] = int128_t::parse("42");
    std::cout << "int128_t::parse(\"42\"): error=" << static_cast<int>(ierr) << std::endl;

    // Verificar si el valor es 42 (comparación directa)
    int128_t expected(0ull, 42ull); // high=0, low=42
    std::cout << "¿Es ival == 42? " << (ival == expected ? "SI" : "NO") << std::endl;

    // Test parse negativo
    auto [nerr, nval] = int128_t::parse("-42");
    std::cout << "\nint128_t::parse(\"-42\"): error=" << static_cast<int>(nerr) << std::endl;

    // Verificar si es negativo
    int128_t zero(0ull, 0ull);
    std::cout << "¿Es nval < 0? " << (nval < zero ? "SI" : "NO") << std::endl;

    // Test min/max sin to_string
    auto min_val = int128_t::min();
    auto max_val = int128_t::max();
    std::cout << "\n¿min < 0? " << (min_val < zero ? "SI" : "NO") << std::endl;
    std::cout << "¿max > 0? " << (max_val > zero ? "SI" : "NO") << std::endl;

    return 0;
}
