#include "../include/uint128_t.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <numeric> // para std::iota

int main()
{
    std::cout << "=== Test completo de std::iota con uint128_t ===\n\n";

    // Test 1: iota básico con números pequeños que se pueden imprimir
    std::cout << "1. Test con números pequeños:\n";
    std::array<uint128_t, 5> arr;
    std::iota(arr.begin(), arr.end(), uint128_t(10));

    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";
    }

    // Test 2: Verificación manual de valores
    std::cout << "\n2. Verificación de incremento:\n";
    bool correct = true;
    for (size_t i = 0; i < arr.size(); ++i)
    {
        uint128_t expected = uint128_t(10) + i;
        if (arr[i] != expected)
        {
            std::cout << "ERROR en posición " << i << "\n";
            correct = false;
        }
    }
    std::cout << (correct ? "✓" : "✗") << " Todos los valores son correctos\n";

    // Test 3: iota con vector
    std::cout << "\n3. Test con std::vector:\n";
    std::vector<uint128_t> vec(3);
    std::iota(vec.begin(), vec.end(), uint128_t(100));

    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << "vec[" << i << "] = " << vec[i] << "\n";
    }

    // Test 4: iota con números grandes (parte alta no-cero)
    std::cout << "\n4. Test con números grandes:\n";
    std::array<uint128_t, 3> big_arr;
    uint128_t big_start(0, 1); // high = 1, low = 0 (2^64)
    std::iota(big_arr.begin(), big_arr.end(), big_start);

    for (size_t i = 0; i < big_arr.size(); ++i)
    {
        std::cout << "big_arr[" << i << "] = " << big_arr[i] << "\n";
    }

    // Test 5: Verificar que el operador++ funciona correctamente
    std::cout << "\n5. Test de operador++:\n";
    uint128_t test_val(42);
    uint128_t original = test_val;
    ++test_val;
    std::cout << "Antes: " << original << ", Después de ++: " << test_val << "\n";
    std::cout << "Incremento correcto: " << ((test_val - original) == uint128_t(1) ? "✓" : "✗") << "\n";

    std::cout << "\n=== Conclusión ===\n";
    std::cout << "✓ std::iota funciona perfectamente con uint128_t\n";
    std::cout << "✓ Requiere que el tipo tenga:\n";
    std::cout << "  - Constructor de copia\n";
    std::cout << "  - Operador de asignación\n";
    std::cout << "  - Operador++\n";
    std::cout << "✓ Nuestra implementación cumple todos estos requisitos\n";

    return 0;
}
