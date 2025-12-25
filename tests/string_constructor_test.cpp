#include "../include/uint128_t.hpp"
#include <iostream>
#include <string>

int main()
{
    std::cout << "=== Test de constructores desde string y from_string ===\n\n";

    // Test 1: Constructor desde const char*
    std::cout << "1. Test constructor desde const char*:\n";
    uint128_t num1("12345");
    std::cout << "num1 = uint128_t(\"12345\") = " << num1 << "\n";
    std::cout << "Verificación: num1 == 12345: " << (num1 == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 2: Método from_string con const char*
    std::cout << "\n2. Test from_string con const char*:\n";
    uint128_t num2 = uint128_t::from_string("67890");
    std::cout << "num2 = from_string(\"67890\") = " << num2 << "\n";
    std::cout << "Verificación: num2 == 67890: " << (num2 == uint128_t(67890) ? "[OK]" : "[ERROR]") << "\n";

    // Test 3: Método from_string con std::string
    std::cout << "\n3. Test from_string con std::string:\n";
    std::string str = "999999999";
    uint128_t num3 = uint128_t::from_string(str);
    std::cout << "num3 = from_string(std::string(\"999999999\")) = " << num3 << "\n";
    std::cout << "Verificación: num3 == 999999999: " << (num3 == uint128_t(999999999) ? "[OK]" : "[ERROR]") << "\n";

    // Test 4: Números más grandes que uint64_t
    std::cout << "\n4. Test con números grandes:\n";
    uint128_t num4("18446744073709551616"); // 2^64
    std::cout << "num4 = uint128_t(\"18446744073709551616\") = " << num4 << "\n";

    // Verificamos que es mayor que el máximo uint64_t
    uint128_t max_uint64(0, UINT64_MAX); // high=0, low=max
    std::cout << "num4 > max_uint64: " << (num4 > max_uint64 ? "[OK]" : "[ERROR]") << "\n";

    // Test 5: Cadenas con espacios al principio
    std::cout << "\n5. Test con espacios al principio:\n";
    uint128_t num5("  12345  ");
    std::cout << "num5 = uint128_t(\"  12345  \") = " << num5 << "\n";
    std::cout << "Verificación: num5 == 12345: " << (num5 == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 6: Cadena vacía y casos especiales
    std::cout << "\n6. Test casos especiales:\n";
    uint128_t num6("");
    uint128_t num7("0");
    std::cout << "num6 = uint128_t(\"\") = " << num6 << "\n";
    std::cout << "num7 = uint128_t(\"0\") = " << num7 << "\n";
    std::cout << "Ambos deben ser 0: " << (num6 == uint128_t(0) && num7 == uint128_t(0) ? "[OK]" : "[ERROR]") << "\n";

    // Test 7: Números de diferentes tamaños
    std::cout << "\n7. Test con diferentes tamaños:\n";
    uint128_t small("1");
    uint128_t medium("1234567890");
    uint128_t large("12345678901234567890");

    std::cout << "small = " << small << "\n";
    std::cout << "medium = " << medium << "\n";
    std::cout << "large = " << large << "\n";

    // Verificaciones básicas
    std::cout << "small == 1: " << (small == uint128_t(1) ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "medium == 1234567890: " << (medium == uint128_t(1234567890) ? "[OK]" : "[ERROR]") << "\n";

    std::cout << "\n=== Tests completados ===\n";

    return 0;
}
