#include <algorithm>
#include <cassert>
#include <int128.hpp>
#include <iostream>
#include <vector>

using namespace nstd;

void demo_arithmetic_symmetry()
{
    std::cout << "=== DEMOSTRACIÓN: Simetría Aritmética ===" << std::endl;

    uint128_t big_number(1, 0);
    std::cout << "Número grande: " << big_number.to_string() << std::endl;

    // Suma simétrica - usando conversión explícita para evitar ambigüedad
    uint128_t sum1 = big_number + uint128_t(1000);
    uint128_t sum2 = 1000u + big_number; // friend operator con uint32_t
    std::cout << "Suma: " << big_number.to_string() << " + 1000 = " << sum1.to_string()
              << std::endl;
    std::cout << "Suma simétrica con uint32_t: 1000u + big_number = " << sum2.to_string()
              << std::endl;

    // Multiplicación simétrica
    uint128_t small(123);
    uint128_t mul1 = small * uint128_t(456);
    uint128_t mul2 = 456ul * small; // friend operator con uint64_t
    std::cout << "Multiplicación: " << small.to_string() << " * 456 = " << mul1.to_string()
              << std::endl;
    std::cout << "Multiplicación simétrica: 456ul * small = " << mul2.to_string() << std::endl;
}

void demo_comparison_symmetry()
{
    std::cout << "\n=== DEMOSTRACIÓN: Simetría de Comparación ===" << std::endl;

    uint128_t value(0, 12345);
    std::cout << "Valor de prueba: " << value.to_string() << std::endl;

    // Test con diferentes tipos - usando sufijos para evitar ambigüedad
    bool test1 = (value == uint128_t(12345)); // uint128_t == uint128_t
    bool test2 = (12345 == value);            // int == uint128_t (friend)
    std::cout << "Igualdad con int: value == uint128_t(12345): " << std::boolalpha << test1
              << std::endl;
    std::cout << "Simetría con int: 12345 == value: " << std::boolalpha << test2 << std::endl;
    std::cout << "Son iguales: " << (test1 && test2 ? "✓" : "✗") << std::endl;

    // Comparaciones específicas por tipo
    bool test3 = (12345u == value);  // uint32_t == uint128_t (friend)
    bool test4 = (12345ul == value); // uint64_t == uint128_t (friend)
    std::cout << "uint32_t == uint128_t: " << std::boolalpha << test3 << std::endl;
    std::cout << "uint64_t == uint128_t: " << std::boolalpha << test4 << std::endl;

    // Comparaciones de orden
    uint128_t larger(0, 50000);
    bool order1 = (50000u > value);  // friend operator> con uint32_t
    bool order2 = (12345u < larger); // friend operator< con uint32_t

    std::cout << "50000u > value: " << std::boolalpha << order1 << std::endl;
    std::cout << "12345u < larger: " << std::boolalpha << order2 << std::endl;
    std::cout << "Comparaciones de orden: " << (order1 && order2 ? "✓" : "✗") << std::endl;
}

void demo_bitwise_symmetry()
{
    std::cout << "\n=== DEMOSTRACIÓN: Simetría Bitwise ===" << std::endl;

    uint128_t flags(0, 0b11110000); // 240 en decimal
    uint32_t mask = 0b11001100u;    // 204 en decimal

    std::cout << "Flags: " << flags.to_string() << std::endl;
    std::cout << "Mask: " << mask << std::endl;

    // AND simétrico
    uint128_t and1 = flags & uint128_t(mask);
    uint128_t and2 = mask & flags; // friend operator
    std::cout << "AND normal: " << and1.to_string() << std::endl;
    std::cout << "AND simétrico: " << and2.to_string() << std::endl;
    std::cout << "AND simétrico: " << (and1 == and2 ? "✓" : "✗") << std::endl;

    // OR simétrico
    uint128_t or1 = flags | uint128_t(mask);
    uint128_t or2 = mask | flags; // friend operator
    std::cout << "OR normal: " << or1.to_string() << std::endl;
    std::cout << "OR simétrico: " << or2.to_string() << std::endl;
    std::cout << "OR simétrico: " << (or1 == or2 ? "✓" : "✗") << std::endl;

    // XOR simétrico
    uint128_t xor1 = flags ^ uint128_t(mask);
    uint128_t xor2 = mask ^ flags; // friend operator
    std::cout << "XOR normal: " << xor1.to_string() << std::endl;
    std::cout << "XOR simétrico: " << xor2.to_string() << std::endl;
    std::cout << "XOR simétrico: " << (xor1 == xor2 ? "✓" : "✗") << std::endl;
}

void demo_practical_usage()
{
    std::cout << "\n=== DEMOSTRACIÓN: Uso Práctico ===" << std::endl;

    // Crear un vector de números grandes
    std::vector<uint128_t> big_numbers = {uint128_t(0, 100), uint128_t(0, 200), uint128_t(0, 50),
                                          uint128_t(0, 150)};

    std::cout << "Vector original: ";
    for (const auto& num : big_numbers) {
        std::cout << num.to_string() << " ";
    }
    std::cout << std::endl;

    // Usar algoritmo con operadores simétricos
    std::cout << "Números > 100 (usando 100u < num): ";
    std::vector<uint128_t> filtered;
    std::copy_if(big_numbers.begin(), big_numbers.end(), std::back_inserter(filtered),
                 [](const uint128_t& num) { return 100u < num; }); // friend operator<

    for (const auto& num : filtered) {
        std::cout << num.to_string() << " ";
    }
    std::cout << std::endl;

    // Multiplicar todos por 3 usando simetría
    std::cout << "Multiplicados por 3 (usando 3u * num): ";
    std::transform(big_numbers.begin(), big_numbers.end(), big_numbers.begin(),
                   [](const uint128_t& num) { return 3u * num; }); // friend operator*

    for (const auto& num : big_numbers) {
        std::cout << num.to_string() << " ";
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
    std::cout << "   ✓ Operadores aritméticos simétricos funcionando con sufijos específicos"
              << std::endl;
    std::cout << "   ✓ Operadores de comparación simétricos funcionando" << std::endl;
    std::cout << "   ✓ Operadores bitwise simétricos funcionando" << std::endl;
    std::cout << "   ✓ Uso práctico en algoritmos STL validado" << std::endl;
    std::cout << "\n🎯 Los operadores friend proporcionan simetría para casos específicos!"
              << std::endl;
    std::cout << "\n📋 NOTA TÉCNICA:" << std::endl;
    std::cout << "   • Para evitar ambigüedad con built-ins, usar sufijos: 123u, 456ul"
              << std::endl;
    std::cout << "   • O usar conversión explícita: uint128_t(123)" << std::endl;
    std::cout << "   • Los operadores friend resuelven específicamente int, uint32_t, uint64_t"
              << std::endl;

    return 0;
}
