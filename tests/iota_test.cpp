#include "../include/uint128_t.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <numeric> // para std::iota

int main()
{
    std::cout << "=== Test de std::iota con uint128_t ===\n\n";

    // Test 1: iota con vector
    std::cout << "1. Test con std::vector:\n";
    std::vector<uint128_t> vec(10);
    std::iota(vec.begin(), vec.end(), uint128_t(100));

    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << "vec[" << i << "] = " << vec[i] << "\n";
    }

    // Test 2: iota con array
    std::cout << "\n2. Test con std::array:\n";
    std::array<uint128_t, 5> arr;
    std::iota(arr.begin(), arr.end(), uint128_t(1000));

    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";
    }

    // Test 3: iota con números grandes
    std::cout << "\n3. Test con números grandes:\n";
    std::vector<uint128_t> big_vec(5);
    uint128_t start_value("18446744073709551615"); // cerca del máximo uint64_t
    std::iota(big_vec.begin(), big_vec.end(), start_value);

    for (size_t i = 0; i < big_vec.size(); ++i)
    {
        std::cout << "big_vec[" << i << "] = " << big_vec[i] << "\n";
    }

    // Test 4: verificación de incremento correcto
    std::cout << "\n4. Verificación de incremento:\n";
    std::vector<uint128_t> test_vec(3);
    uint128_t base = 42;
    std::iota(test_vec.begin(), test_vec.end(), base);

    bool correct = true;
    for (size_t i = 0; i < test_vec.size(); ++i)
    {
        uint128_t expected = base + i;
        if (test_vec[i] != expected)
        {
            std::cout << "ERROR: test_vec[" << i << "] = " << test_vec[i]
                      << ", esperado = " << expected << "\n";
            correct = false;
        }
    }

    if (correct)
    {
        std::cout << "[OK] Todos los valores son correctos\n";
    }

    std::cout << "\n=== Test completado ===\n";

    return 0;
}
