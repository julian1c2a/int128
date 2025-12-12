#include "../include/uint128_t.hpp"
#include <iostream>

// Struct sin referencias para comparar
struct uint128_simple
{
    uint64_t data[2];
};

int main()
{
    std::cout << "Tamaños en bytes:\n";
    std::cout << "uint64_t[2]:     " << sizeof(uint64_t[2]) << "\n";
    std::cout << "uint128_simple:  " << sizeof(uint128_simple) << "\n";
    std::cout << "uint128_t:       " << sizeof(uint128_t) << "\n";
    std::cout << "uint64_t&:       " << sizeof(uint64_t *) << " (las referencias ocupan como punteros)\n";

    std::cout << "\nEsperamos:\n";
    std::cout << "- uint64_t[2] = 16 bytes (8 + 8)\n";
    std::cout << "- uint128_simple = 16 bytes\n";
    std::cout << "- uint128_t = 32 bytes (16 + 8 + 8 para las dos referencias)\n";

    return 0;
}
