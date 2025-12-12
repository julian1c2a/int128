#include "../include/uint128_t.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
    std::cout << "=== Casos de uso prácticos con literales uint128_t ===\n\n";

    // Test 1: Constantes naturales
    std::cout << "1. Definición de constantes:\n";
    const auto KILOBYTE = 1024_u128;
    const auto MEGABYTE = 1024_u128 * KILOBYTE;
    const auto GIGABYTE = 1024_u128 * MEGABYTE;
    const auto TERABYTE = 1024_u128 * GIGABYTE;

    std::cout << "KILOBYTE = " << KILOBYTE << "\n";
    std::cout << "MEGABYTE = " << MEGABYTE << "\n";
    std::cout << "GIGABYTE = " << GIGABYTE << "\n";
    std::cout << "TERABYTE = " << TERABYTE << "\n";

    // Test 2: Números muy grandes de forma legible
    std::cout << "\n2. Números grandes con literales de cadena:\n";
    auto world_population = "8000000000"_u128;            // ~8 billones
    auto atoms_in_gram = "602214076000000000000000"_U128; // Número de Avogadro
    auto seconds_in_universe = "436117076900000000"_U128; // aprox

    std::cout << "Población mundial aprox: " << world_population << "\n";
    std::cout << "Átomos en un gramo (Avogadro): " << atoms_in_gram << "\n";
    std::cout << "Segundos en edad del universo: " << seconds_in_universe << "\n";

    // Test 3: Cálculos financieros (crypto, wei, etc)
    std::cout << "\n3. Cálculos con precisión alta:\n";
    const auto WEI_PER_ETHER = "1000000000000000000"_u128; // 10^18
    auto transaction_wei = 50000_u128 * WEI_PER_ETHER;     // 50,000 ETH

    std::cout << "WEI_PER_ETHER = " << WEI_PER_ETHER << "\n";
    std::cout << "50,000 ETH en wei = " << transaction_wei << "\n";

    // Test 4: Arrays con literales
    std::cout << "\n4. Arrays inicializados con literales:\n";
    uint128_t fibonacci[] = {
        0_u128, 1_u128, 1_u128, 2_u128, 3_u128, 5_u128, 8_u128, 13_u128};

    std::cout << "Fibonacci: ";
    for (size_t i = 0; i < sizeof(fibonacci) / sizeof(fibonacci[0]); ++i)
    {
        std::cout << fibonacci[i];
        if (i < sizeof(fibonacci) / sizeof(fibonacci[0]) - 1)
            std::cout << ", ";
    }
    std::cout << "\n";

    // Test 5: Operaciones con iota usando literales
    std::cout << "\n5. std::iota con literales:\n";
    std::vector<uint128_t> range(5);
    std::iota(range.begin(), range.end(), 1000000_u128);

    std::cout << "Range desde 1,000,000: ";
    for (size_t i = 0; i < range.size(); ++i)
    {
        std::cout << range[i];
        if (i < range.size() - 1)
            std::cout << ", ";
    }
    std::cout << "\n";

    // Test 6: Comparaciones directas
    std::cout << "\n6. Comparaciones naturales:\n";
    std::cout << "¿1_u128 < 2_U128? " << (1_u128 < 2_U128 ? "Sí" : "No") << "\n";
    std::cout << "¿\"999999\"_u128 == \"999999\"_U128? " << ("999999"_u128 == "999999"_U128 ? "Sí" : "No") << "\n";
    std::cout << "¿42_u128 != 43_U128? " << (42_u128 != 43_U128 ? "Sí" : "No") << "\n";

    // Test 7: Operaciones bit a bit con literales
    std::cout << "\n7. Operaciones bit a bit:\n";
    auto mask = 0xFF_u128;
    auto data = 0x12345678_u128;
    auto result = data & mask;

    std::cout << "data = 0x12345678 = " << data << "\n";
    std::cout << "mask = 0xFF = " << mask << "\n";
    std::cout << "data & mask = " << result << "\n";
    std::cout << "result == 0x78: " << (result == uint128_t(0x78) ? "✓" : "✗") << "\n";

    // Test 8: Expresiones complejas
    std::cout << "\n8. Expresiones matemáticas complejas:\n";
    auto complex_calc = (100_u128 * 200_u128) + (50_u128 << 2) - 42_U128;
    auto expected = 100 * 200 + 50 * 4 - 42; // = 20158

    std::cout << "(100 * 200) + (50 << 2) - 42 = " << complex_calc << "\n";
    std::cout << "Verificación: " << (complex_calc == uint128_t(expected) ? "✓" : "✗") << "\n";

    std::cout << "\n=== Beneficios de los literales uint128_t ===\n";
    std::cout << "✓ Sintaxis natural: 42_u128 vs uint128_t(42)\n";
    std::cout << "✓ Números grandes legibles: \"123456789\"_u128\n";
    std::cout << "✓ Funciona con auto: auto x = 42_u128\n";
    std::cout << "✓ Integración perfecta con operadores\n";
    std::cout << "✓ Constantes de tiempo de compilación\n";
    std::cout << "✓ Código más limpio y expresivo\n";

    return 0;
}
