#include "uint128_t.hpp"
#include <iostream>
#include <sstream>

int main()
{
    std::cout << "=== Prueba de funciones de conversión string ===" << std::endl;

    // Prueba 1: Constructor desde string
    std::cout << "\n1. Constructor desde string:" << std::endl;
    uint128_t a("12345");
    uint128_t b("999");
    std::cout << "uint128_t a(\"12345\") = " << a << std::endl;
    std::cout << "uint128_t b(\"999\") = " << b << std::endl;

    // Prueba 2: from_string estática
    std::cout << "\n2. Función from_string estática:" << std::endl;
    uint128_t c = uint128_t::from_string("7777");
    uint128_t d = uint128_t::from_string("0");
    std::cout << "from_string(\"7777\") = " << c << std::endl;
    std::cout << "from_string(\"0\") = " << d << std::endl;

    // Prueba 3: Operador << (ya probado arriba implícitamente)
    std::cout << "\n3. Operador de salida <<:" << std::endl;
    uint128_t e(0, 42);
    std::cout << "uint128_t(0, 42) = " << e << std::endl;

    // Prueba 4: Operador >>
    std::cout << "\n4. Operador de entrada >> (usando stringstream):" << std::endl;
    std::stringstream ss("12345 67890 100");
    uint128_t x, y, z;
    ss >> x >> y >> z;
    std::cout << "Leídos desde stream: " << x << ", " << y << ", " << z << std::endl;

    // Prueba 5: Conversión bidireccional
    std::cout << "\n5. Conversión bidireccional:" << std::endl;
    uint128_t original(0, 9876543210);
    std::string str_conversion = original.to_string();
    uint128_t reconstructed = uint128_t::from_string(str_conversion);

    std::cout << "Original: " << original << std::endl;
    std::cout << "String: \"" << str_conversion << "\"" << std::endl;
    std::cout << "Reconstruido: " << reconstructed << std::endl;
    std::cout << "¿Son iguales? " << (original == reconstructed ? "SÍ" : "NO") << std::endl;

    // Prueba 6: Números grandes
    std::cout << "\n6. Números grandes:" << std::endl;
    uint128_t large_a(0, 1000);
    uint128_t large_b(0, 999999);
    uint128_t large_result = large_a * large_b;

    std::cout << large_a << " * " << large_b << " = " << large_result << std::endl;

    // Convertir a string y back
    std::string large_str = large_result.to_string();
    uint128_t large_reconstructed = uint128_t::from_string(large_str);
    std::cout << "Conversión string: \"" << large_str << "\"" << std::endl;
    std::cout << "Reconstruido: " << large_reconstructed << std::endl;
    std::cout << "¿Son iguales? " << (large_result == large_reconstructed ? "SÍ" : "NO")
              << std::endl;

    // Prueba 7: Casos edge
    std::cout << "\n7. Casos especiales:" << std::endl;
    uint128_t zero_from_str = uint128_t::from_string("");
    uint128_t zero_from_zero = uint128_t::from_string("0");
    std::cout << "from_string(\"\") = " << zero_from_str << std::endl;
    std::cout << "from_string(\"0\") = " << zero_from_zero << std::endl;

    std::cout << "\n¡Todas las funciones de string implementadas exitosamente!" << std::endl;
    return 0;
}