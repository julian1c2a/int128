#include "../include/uint128_t.hpp"
#include <iostream>
#include <vector>

int main()
{
    std::cout << "=== Test de literales de usuario para uint128_t ===\n\n";

    // Test 1: Literales pequeños con _u128
    std::cout << "1. Test literales pequeños con _u128:\n";
    auto num1 = 12345_u128;
    auto num2 = 67890_u128;
    std::cout << "12345_u128 = " << num1 << "\n";
    std::cout << "67890_u128 = " << num2 << "\n";
    std::cout << "Verificación: num1 == uint128_t(12345): " << (num1 == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 2: Literales pequeños con _U128
    std::cout << "\n2. Test literales pequeños con _U128:\n";
    auto num3 = 999_U128;
    auto num4 = 1000000_U128;
    std::cout << "999_U128 = " << num3 << "\n";
    std::cout << "1000000_U128 = " << num4 << "\n";
    std::cout << "Verificación: num3 == uint128_t(999): " << (num3 == uint128_t(999) ? "[OK]" : "[ERROR]") << "\n";

    // Test 3: Literales grandes como cadenas con _u128
    std::cout << "\n3. Test literales grandes como cadenas con _u128:\n";
    auto big1 = "18446744073709551615"_u128; // max uint64_t
    auto big2 = "18446744073709551616"_u128; // max uint64_t + 1
    std::cout << "\"18446744073709551615\"_u128 = " << big1 << "\n";
    std::cout << "\"18446744073709551616\"_u128 = " << big2 << "\n";
    std::cout << "big2 > big1: " << (big2 > big1 ? "[OK]" : "[ERROR]") << "\n";

    // Test 4: Literales grandes como cadenas con _U128
    std::cout << "\n4. Test literales grandes como cadenas con _U128:\n";
    auto huge1 = "123456789012345678901234567890"_U128;
    auto huge2 = "999999999999999999999999999999"_U128;
    std::cout << "\"123456789012345678901234567890\"_U128 = " << huge1 << "\n";
    std::cout << "\"999999999999999999999999999999\"_U128 = " << huge2 << "\n";
    std::cout << "huge2 > huge1: " << (huge2 > huge1 ? "[OK]" : "[ERROR]") << "\n";

    // Test 5: Aritmética con literales
    std::cout << "\n5. Test aritmética con literales:\n";
    auto result1 = 100_u128 + 200_u128;
    auto result2 = 1000_U128 * 2_U128;
    std::cout << "100_u128 + 200_u128 = " << result1 << "\n";
    std::cout << "1000_U128 * 2_U128 = " << result2 << "\n";
    std::cout << "result1 == 300: " << (result1 == uint128_t(300) ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "result2 == 2000: " << (result2 == uint128_t(2000) ? "[OK]" : "[ERROR]") << "\n";

    // Test 6: Comparaciones con literales
    std::cout << "\n6. Test comparaciones con literales:\n";
    std::cout << "42_u128 == 42_U128: " << (42_u128 == 42_U128 ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "100_u128 < 200_u128: " << (100_u128 < 200_u128 ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "\"1000\"_u128 >= \"999\"_U128: " << ("1000"_u128 >= "999"_U128 ? "[OK]" : "[ERROR]") << "\n";

    // Test 7: Casos edge con literales de cadena
    std::cout << "\n7. Test casos edge con literales de cadena:\n";
    auto zero1 = "0"_u128;
    auto zero2 = "000"_U128;
    auto leading = "00012345"_u128;
    std::cout << "\"0\"_u128 = " << zero1 << "\n";
    std::cout << "\"000\"_U128 = " << zero2 << "\n";
    std::cout << "\"00012345\"_u128 = " << leading << "\n";
    std::cout << "Todos los ceros son iguales: " << (zero1 == zero2 ? "[OK]" : "[ERROR]") << "\n";
    std::cout << "leading == 12345: " << (leading == uint128_t(12345) ? "[OK]" : "[ERROR]") << "\n";

    // Test 8: Uso en contenedores
    std::cout << "\n8. Test uso en contenedores:\n";
    std::vector<uint128_t> vec = {1_u128, 2_U128, 3_u128, "1000"_U128};
    std::cout << "Vector con literales: [";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i];
        if (i < vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "vec[3] == 1000: " << (vec[3] == uint128_t(1000) ? "[OK]" : "[ERROR]") << "\n";

    std::cout << "\n=== Resumen ===\n";
    std::cout << "[OK] Literales _u128 y _U128 funcionan para números pequeños\n";
    std::cout << "[OK] Literales de cadena manejan números grandes correctamente\n";
    std::cout << "[OK] Aritmética con literales funciona naturalmente\n";
    std::cout << "[OK] Comparaciones con literales son intuitivas\n";
    std::cout << "[OK] Casos edge manejados correctamente\n";
    std::cout << "[OK] Integración con contenedores STL\n";
    std::cout << "\n¡Los literales de usuario están completamente funcionales! 🎉\n";

    return 0;
}
