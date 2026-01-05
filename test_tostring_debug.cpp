#include "int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test to_string() para int128_t ===\n\n";

    // Test 1: Valores positivos
    std::cout << "Test 1: Valores positivos\n";
    int128_t pos42(0ull, 42ull);
    std::cout << "  int128_t(0, 42).to_string() = \"" << pos42.to_string() << "\"\n";
    std::cout << "  ¿Es \"42\"? " << (pos42.to_string() == "42" ? "SI" : "NO") << "\n\n";

    // Test 2: Valores negativos simples
    std::cout << "Test 2: Valores negativos simples\n";
    int128_t neg42 = -pos42;
    std::cout << "  -pos42 (negación de 42)\n";
    std::cout << "  low() = " << neg42.low() << "\n";
    std::cout << "  high() = " << neg42.high() << "\n";
    std::cout << "  is_negative() = " << (neg42.is_negative() ? "SI" : "NO") << "\n";
    std::cout << "  to_string() = \"" << neg42.to_string() << "\"\n";
    std::cout << "  ¿Es \"-42\"? " << (neg42.to_string() == "-42" ? "SI" : "NO") << "\n\n";

    // Test 3: Valor parseado negativo
    std::cout << "Test 3: Valor parseado de \"-42\"\n";
    auto [err, parsed_neg] = int128_t::parse("-42");
    std::cout << "  parse error = " << static_cast<int>(err) << "\n";
    std::cout << "  low() = " << parsed_neg.low() << "\n";
    std::cout << "  high() = " << parsed_neg.high() << "\n";
    std::cout << "  is_negative() = " << (parsed_neg.is_negative() ? "SI" : "NO") << "\n";
    std::cout << "  to_string() = \"" << parsed_neg.to_string() << "\"\n";
    std::cout << "  ¿Es \"-42\"? " << (parsed_neg.to_string() == "-42" ? "SI" : "NO") << "\n\n";

    // Test 4: Operador unario negación
    std::cout << "Test 4: Probar abs() explícitamente\n";
    std::cout << "  neg42.abs().low() = " << neg42.abs().low() << "\n";
    std::cout << "  neg42.abs().high() = " << neg42.abs().high() << "\n";
    std::cout << "  neg42.abs().to_string() = \"" << neg42.abs().to_string() << "\"\n\n";

    // Test 5: Probar operator-() directamente
    std::cout << "Test 5: Probar operator-() directamente\n";
    int128_t negated = -neg42;
    std::cout << "  -neg42 (doble negación, debería dar 42)\n";
    std::cout << "  low() = " << negated.low() << "\n";
    std::cout << "  high() = " << negated.high() << "\n";
}
