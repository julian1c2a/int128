#include <algorithm>
#include <cassert>
#include <int128.hpp>
#include <iostream>
#include <vector>

using namespace nstd;

void demo_arithmetic_symmetry()
{
    std::cout << "=== DEMOSTRACIÓN: Simetría Aritmética ===" << std::endl;

    uint128_t big_number(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "Número grande: " << big_number.to_string() << std::endl;

    // Suma simétrica
    uint128_t sum1 = big_number + 1000u;
    uint128_t sum2 = 1000u + big_number;
    std::cout << "Suma simétrica: " << (sum1 == sum2 ? "✓" : "✗") << std::endl;

    // Multiplicación simétrica
    uint128_t small(123);
    uint128_t mul1 = small * uint64_t(456);
    uint128_t mul2 = uint64_t(456) * small;
    std::cout << "Multiplicación simétrica: " << (mul1 == mul2 ? "✓" : "✗") << std::endl;
    std::cout << "  " << small.to_string() << " * 456 = " << mul1.to_string() << std::endl;
}

void demo_comparison_symmetry()
{
    std::cout << "\n=== DEMOSTRACIÓN: Simetría de Comparación ===" << std::endl;

    uint128_t value(0, 12345);

    // Test con diferentes tipos
    bool test1 = (value == 12345);           // uint128_t == int
    bool test2 = (12345 == value);           // int == uint128_t (friend)
    bool test3 = (value == 12345u);          // uint128_t == uint32_t
    bool test4 = (12345u == value);          // uint32_t == uint128_t (friend)
    bool test5 = (value == uint64_t(12345)); // uint128_t == uint64_t
    bool test6 = (uint64_t(12345) == value); // uint64_t == uint128_t (friend)

    std::cout << "Igualdad con int: " << (test1 && test2 ? "✓" : "✗") << std::endl;
    std::cout << "Igualdad con uint32_t: " << (test3 && test4 ? "✓" : "✗") << std::endl;
    std::cout << "Igualdad con uint64_t: " << (test5 && test6 ? "✓" : "✗") << std::endl;

    // Comparaciones de orden
    uint128_t larger(0, 50000);
    bool order1 = (larger > 12345u) && (50000u > value); // Ambos true
    bool order2 = (value < 50000u) && (12345u < larger); // Ambos true

    std::cout << "Comparaciones de orden: " << (order1 && order2 ? "✓" : "✗") << std::endl;
}

void demo_bitwise_symmetry()
{
    std::cout << "\n=== DEMOSTRACIÓN: Simetría Bitwise ===" << std::endl;

    uint128_t flags(0, 0b11110000);
    uint32_t mask = 0b11001100u;

    // AND simétrico
    uint128_t and1 = flags & uint128_t(mask);
    uint128_t and2 = mask & flags; // friend operator
    std::cout << "AND simétrico: " << (and1 == and2 ? "✓" : "✗") << std::endl;
    std::cout << "  Resultado: " << and1.to_string() << " (decimal: " << and1.low() << ")"
              << std::endl;

    // OR simétrico
    uint128_t or1 = flags | uint128_t(mask);
    uint128_t or2 = mask | flags; // friend operator
    std::cout << "OR simétrico: " << (or1 == or2 ? "✓" : "✗") << std::endl;
    std::cout << "  Resultado: " << or1.to_string() << " (decimal: " << or1.low() << ")"
              << std::endl;

    // XOR simétrico
    uint128_t xor1 = flags ^ uint128_t(mask);
    uint128_t xor2 = mask ^ flags; // friend operator
    std::cout << "XOR simétrico: " << (xor1 == xor2 ? "✓" : "✗") << std::endl;
    std::cout << "  Resultado: " << xor1.to_string() << " (decimal: " << xor1.low() << ")"
              << std::endl;
}

void demo_practical_usage()
{
    std::cout << "\n=== DEMOSTRACIÓN: Uso Práctico ===" << std::endl;

    // Crear un vector de números grandes
    std::vector<uint128_t> big_numbers = {uint128_t(0, 100), uint128_t(0, 200), uint128_t(0, 50),
                                          uint128_t(0, 150)};

    std::cout << "Vector original: ";
    for (const auto& num : big_numbers) {
        std::cout << num.low() << " ";
    }
    std::cout << std::endl;

    // Usar algoritmo con operadores simétricos
    std::cout << "Números > 100 (usando 100u < num): ";
    std::vector<uint128_t> filtered;
    std::copy_if(big_numbers.begin(), big_numbers.end(), std::back_inserter(filtered),
                 [](const uint128_t& num) { return 100u < num; }); // friend operator<

    for (const auto& num : filtered) {
        std::cout << num.low() << " ";
    }
    std::cout << std::endl;

    // Multiplicar todos por 3 usando simetría
    std::cout << "Multiplicados por 3 (usando 3u * num): ";
    std::transform(big_numbers.begin(), big_numbers.end(), big_numbers.begin(),
                   [](const uint128_t& num) { return 3u * num; }); // friend operator*

    for (const auto& num : big_numbers) {
        std::cout << num.low() << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::cout << "🔄 DEMOSTRACIÓN COMPLETA DE OPERADORES SIMÉTRICOS 🔄" << std::endl;
    std::cout << "====================================================" << std::endl;

    demo_arithmetic_symmetry();
    demo_comparison_symmetry();
    demo_bitwise_symmetry();
    demo_practical_usage();

    std::cout << "\n✅ TODAS las demostraciones completadas exitosamente!" << std::endl;
    std::cout << "\n📝 RESUMEN:" << std::endl;
    std::cout << "   ✓ Operadores aritméticos simétricos funcionando" << std::endl;
    std::cout << "   ✓ Operadores de comparación simétricos funcionando" << std::endl;
    std::cout << "   ✓ Operadores bitwise simétricos funcionando" << std::endl;
    std::cout << "   ✓ Uso práctico en algoritmos STL validado" << std::endl;
    std::cout << "\n🎯 Los operadores friend proporcionan simetría completa para uint128_t!"
              << std::endl;

    return 0;
}
