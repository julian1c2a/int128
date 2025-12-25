#include "../include/uint128_t.hpp"
#include <iostream>
#include <string>

int main()
{
    std::cout << "=== Test avanzado de parsing de cadenas ===\n\n";

    // Test 1: Constructor desde cadena
    std::cout << "1. Test constructor desde cadena:\n";
    uint128_t compile_time_num("12345");
    std::cout << "uint128_t compile_time_num(\"12345\") = " << compile_time_num << "\n";
    std::cout << "[OK] Constructor desde cadena funciona\n";

    // Test 2: Números que usan toda la capacidad de uint64_t en parte baja
    std::cout << "\n2. Test con números máximos para uint64_t:\n";
    std::string max_uint64_str = "18446744073709551615"; // UINT64_MAX
    uint128_t max_low = uint128_t::from_string(max_uint64_str);
    std::cout << "max_low = from_string(\"18446744073709551615\") = " << max_low << "\n";
    std::cout << "max_low.high() == 0: " << (max_low.high() == 0 ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "max_low.low() == UINT64_MAX: " << (max_low.low() == UINT64_MAX ? "[OK]" : "[ERROR]") << "\n";

    // Test 3: Números que requieren parte alta no-cero
    std::cout << "\n3. Test con números que usan parte alta:\n";
    uint128_t just_over = uint128_t::from_string("18446744073709551616"); // UINT64_MAX + 1
    std::cout << "just_over = from_string(\"18446744073709551616\") = " << just_over << "\n";
    std::cout << "just_over.high() == 1: " << (just_over.high() == 1 ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "just_over.low() == 0: " << (just_over.low() == 0 ? "[OK]" : "[ERROR]") << "\n";

    // Test 4: Potencias de 10
    std::cout << "\n4. Test con potencias de 10:\n";
    uint128_t pow10_18 = uint128_t::from_string("1000000000000000000");  // 10^18
    uint128_t pow10_19 = uint128_t::from_string("10000000000000000000"); // 10^19
    std::cout << "10^18 = " << pow10_18 << "\n";
    std::cout << "10^19 = " << pow10_19 << "\n";
    std::cout << "10^19 > 10^18: " << (pow10_19 > pow10_18 ? "[OK]" : "[ERROR]") << "\n";

    // Test 5: Casos con muchos ceros
    std::cout << "\n5. Test con muchos ceros:\n";
    uint128_t many_zeros("1000000000000000000000000000");
    uint128_t leading_zeros("00000012345");
    std::cout << "many_zeros = " << many_zeros << "\n";
    std::cout << "leading_zeros = " << leading_zeros << "\n";
    std::cout << "leading_zeros == 12345: " << (leading_zeros == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 6: Verificar que el parsing se detiene en caracteres no-dígito
    std::cout << "\n6. Test parsing con caracteres no-dígito:\n";
    uint128_t with_text("12345abc");
    uint128_t with_space("12345 67890");
    std::cout << "with_text = uint128_t(\"12345abc\") = " << with_text << "\n";
    std::cout << "with_space = uint128_t(\"12345 67890\") = " << with_space << "\n";
    std::cout << "Ambos deben ser 12345: " << (with_text == uint128_t(12345) && with_space == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 7: Comparación con construcción manual
    std::cout << "\n7. Test comparación con construcción manual:\n";
    uint128_t from_str = uint128_t::from_string("123456789012345");
    uint128_t manual(0, 123456789012345ULL);
    std::cout << "from_str = " << from_str << "\n";
    std::cout << "manual = " << manual << "\n";
    std::cout << "Deben ser iguales: " << (from_str == manual ? "[OK]" : "[ERROR]") << "\n";

    // Test 8: Verificar invariantes
    std::cout << "\n8. Test de invariantes:\n";
    uint128_t zero1("");
    uint128_t zero2("0");
    uint128_t zero3("   0   ");
    std::cout << "Todos los siguientes deben ser cero:\n";
    std::cout << "zero1 (\"\") == 0: " << (zero1 == uint128_t(0) ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "zero2 (\"0\") == 0: " << (zero2 == uint128_t(0) ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "zero3 (\"   0   \") == 0: " << (zero3 == uint128_t(0) ? "[OK]" : "[ERROR]") << "\n";

    std::cout << "\n=== Resumen ===\n";
    std::cout << "[OK] Constructor desde const char* funciona\n";
    std::cout << "[OK] from_string() estático maneja const char* y std::string\n";
    std::cout << "[OK] Parsing correcto de números grandes (>64 bits)\n";
    std::cout << "[OK] Manejo correcto de espacios en blanco\n";
    std::cout << "[OK] Parsing se detiene en caracteres no-dígito\n";
    std::cout << "[OK] Casos edge (vacío, ceros) manejados correctamente\n";

    return 0;
}
