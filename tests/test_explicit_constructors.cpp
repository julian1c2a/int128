/*
 * Test simple para verificar que los constructores explícitos funcionan
 */

#include "../include/int128_t.hpp"
#include <iostream>

int main()
{
    std::cout << "=== Testing EXPLICIT CONSTRUCTORS ===" << std::endl;

    // Estos deben compilar (conversiones explícitas)
    int128_t a{42};                                      // Inicialización directa
    int128_t b = int128_t(42);                           // Conversión explícita
    int128_t c = int128_t(-42);                          // Conversión explícita
    int128_t d = int128_t(0x12345678ULL, 0x9ABCDEF0ULL); // Constructor de dos uint64_t

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;

    std::cout << "[OK] Explicit constructors working correctly" << std::endl;

    // Estos NO deberían compilar si descomentas (conversiones implícitas)
    // int128_t e = 42;           // [FAIL] Conversión implícita
    // int128_t f = -42;          // [FAIL] Conversión implícita
    // int128_t g;                // [FAIL] Constructor por defecto implícito

    return 0;
}