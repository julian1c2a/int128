#include "uint128_t_fixed.hpp"
#include <iostream>

int main()
{
    std::cout << "=== Prueba de uint128_t con divrem integrado ===" << std::endl;

    // Crear números de prueba
    uint128_t a(0, 100); // 100
    uint128_t b(0, 7);   // 7

    std::cout << "Dividendo: " << a.to_string() << ", Divisor: " << b.to_string() << std::endl;

    // Probar divrem directamente
    auto result = a.divrem(b);
    if (result.has_value()) {
        std::cout << "divrem(" << a.to_string() << ", " << b.to_string() << ") = ";
        std::cout << "quotient: " << result.value().first.to_string()
                  << ", remainder: " << result.value().second.to_string() << std::endl;
    }

    // Probar operadores de división
    uint128_t quotient = a / b;
    uint128_t remainder = a % b;

    std::cout << a.to_string() << " / " << b.to_string() << " = " << quotient.to_string()
              << std::endl;
    std::cout << a.to_string() << " % " << b.to_string() << " = " << remainder.to_string()
              << std::endl;

    // Verificar que quotient * divisor + remainder == dividendo
    uint128_t verification = quotient * b + remainder;
    std::cout << "Verificación: " << quotient.to_string() << " * " << b.to_string() << " + "
              << remainder.to_string() << " = " << verification.to_string() << std::endl;

    if (verification == a) {
        std::cout << "[OK] Verificación exitosa" << std::endl;
    } else {
        std::cout << "[ERROR] Verificación falló" << std::endl;
    }

    // Probar caso más grande
    uint128_t large(0, 1000);
    uint128_t divisor(0, 13);

    std::cout << "\nPrueba con números más grandes:" << std::endl;
    std::cout << large.to_string() << " / " << divisor.to_string() << " = "
              << (large / divisor).to_string() << " resto " << (large % divisor).to_string()
              << std::endl;

    // Verificación
    uint128_t large_verification = (large / divisor) * divisor + (large % divisor);
    std::cout << "Verificación: " << (large / divisor).to_string() << " * " << divisor.to_string()
              << " + " << (large % divisor).to_string() << " = " << large_verification.to_string()
              << std::endl;

    if (large_verification == large) {
        std::cout << "[OK] Verificación exitosa para números grandes" << std::endl;
    } else {
        std::cout << "[ERROR] Verificación falló para números grandes" << std::endl;
    }

    std::cout << "\n¡Algoritmo divrem integrado exitosamente!" << std::endl;

    return 0;
}