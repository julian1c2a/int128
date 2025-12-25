#include "../include/uint128_t.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <numeric> // para std::iota

int main()
{
    std::cout << "=== Test simple de std::iota con uint128_t ===\n\n";

    // Test básico con números pequeños
    std::cout << "1. Test básico:\n";
    std::array<uint128_t, 3> arr;
    std::iota(arr.begin(), arr.end(), uint128_t(5));

    // Verificar que los valores son correctos sin imprimir directamente
    bool correct = true;
    for (size_t i = 0; i < arr.size(); ++i)
    {
        uint128_t expected = 5 + i;
        if (arr[i] != expected)
        {
            std::cout << "ERROR en posicion " << i << "\n";
            correct = false;
        }
    }

    if (correct)
    {
        std::cout << "[OK] std::iota funciona correctamente\n";
        std::cout << "arr[0] == 5: " << (arr[0] == uint128_t(5) ? "[OK]" : "[ERROR]") << "\n";
        std::cout << "arr[1] == 6: " << (arr[1] == uint128_t(6) ? "[OK]" : "[ERROR]") << "\n";
        std::cout << "arr[2] == 7: " << (arr[2] == uint128_t(7) ? "[OK]" : "[ERROR]") << "\n";
    }
    else
    {
        std::cout << "[ERROR] Error en std::iota\n";
    }

    // Test con números un poco más grandes
    std::cout << "\n2. Test con números más grandes:\n";
    std::vector<uint128_t> vec(5);
    std::iota(vec.begin(), vec.end(), uint128_t(100));

    bool vec_correct = true;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        uint128_t expected = 100 + i;
        if (vec[i] != expected)
        {
            vec_correct = false;
        }
    }

    std::cout << (vec_correct ? "[OK]" : "[ERROR]") << " Iota con números más grandes\n";

    std::cout << "\n=== Test completado ===\n";

    return 0;
}
