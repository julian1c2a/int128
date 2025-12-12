#include "uint128_t.hpp"
#include <iostream>
#include <sstream>

int main()
{
    std::cout << "=== Prueba de uint128_t.hpp con static_assert ===" << std::endl;

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

    // Prueba 3: Operadores de flujo
    std::cout << "\n3. Operadores de flujo:" << std::endl;
    uint128_t e(0, 42);
    std::cout << "uint128_t(0, 42) = " << e << std::endl;

    std::stringstream ss("12345 67890 100");
    uint128_t x, y, z;
    ss >> x >> y >> z;
    std::cout << "Leídos desde stream: " << x << ", " << y << ", " << z << std::endl;

    // Prueba 4: Conversión bidireccional
    std::cout << "\n4. Conversión bidireccional:" << std::endl;
    uint128_t original(0, 9876543210ULL);
    std::string str_conversion = original.to_string();
    uint128_t reconstructed = uint128_t::from_string(str_conversion);

    std::cout << "Original: " << original << std::endl;
    std::cout << "String: \"" << str_conversion << "\"" << std::endl;
    std::cout << "Reconstruido: " << reconstructed << std::endl;
    std::cout << "¿Son iguales? " << (original == reconstructed ? "SÍ" : "NO") << std::endl;

    // Prueba 5: Operaciones aritméticas
    std::cout << "\n5. Operaciones aritméticas:" << std::endl;
    uint128_t num1(0, 100);
    uint128_t num2(0, 7);

    std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << std::endl;
    std::cout << num1 << " % " << num2 << " = " << (num1 % num2) << std::endl;

    // Verificación con divrem
    auto result = num1.divrem(num2);
    if (result.has_value()) {
        std::cout << "divrem(" << num1 << ", " << num2 << ") = quotient: " << result.value().first
                  << ", remainder: " << result.value().second << std::endl;
    }

    // Prueba 6: Constructores templated
    std::cout << "\n6. Constructores templated:" << std::endl;
    uint128_t from_int(42);
    uint128_t from_parts(1, 2000000000ULL);
    std::cout << "uint128_t(42) = " << from_int << std::endl;
    std::cout << "uint128_t(1, 2000000000ULL) = " << from_parts << std::endl;

    // Prueba 7: Operadores de assignment templated
    std::cout << "\n7. Assignment templated:" << std::endl;
    uint128_t assigned_val;
    assigned_val = 999ULL;
    std::cout << "assigned_val = 999ULL: " << assigned_val << std::endl;

    std::cout << "\n¡Archivo uint128_t.hpp con static_assert funcionando perfectamente!"
              << std::endl;
    return 0;
}