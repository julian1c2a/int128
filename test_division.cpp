#include "int128_base.hpp"
#include <iomanip>
#include <iostream>

using namespace nstd;

void print_result(const char* expr, const auto& result, const auto& expected)
{
    std::cout << expr << " = " << result.low() << " (expected: " << expected << ") ";
    if (result.low() == expected) {
        std::cout << "\u2713 PASS\n";
    } else {
        std::cout << "\u2717 FAIL\n";
    }
}

int main()
{
    std::cout << "=== TEST: Operadores de Division y Modulo ===\n\n";

    // Test 1: Division basica
    std::cout << "Test 1: Division basica\n";
    uint128_t a(100);
    uint128_t b(10);
    auto result1 = a / b;
    print_result("100 / 10", result1, 10);

    // Test 2: Division con resto
    uint128_t c(123);
    uint128_t d(10);
    auto result2 = c / d;
    auto result3 = c % d;
    print_result("123 / 10", result2, 12);
    print_result("123 % 10", result3, 3);

    // Test 3: Division con integral_builtin
    std::cout << "\nTest 2: Division con integral_builtin\n";
    uint128_t e(1000);
    auto result4 = e / 25u;
    auto result5 = e % 25u;
    print_result("1000 / 25", result4, 40);
    print_result("1000 % 25", result5, 0);

    // Test 4: Division grande
    std::cout << "\nTest 3: Division de numeros grandes\n";
    uint128_t big(0xFFFFFFFFFFFFFFFF, 0); // 2^64 - 1
    auto result6 = big / 1000000u;
    std::cout << "(2^64-1) / 1000000 = " << result6.low() << " ";
    if (result6.low() == 18446744073709) {
        std::cout << "\u2713 PASS\n";
    } else {
        std::cout << "\u2717 FAIL (expected: 18446744073709)\n";
    }

    // Test 5: Division por 1
    std::cout << "\nTest 4: Division por 1\n";
    uint128_t f(12345);
    auto result7 = f / 1u;
    print_result("12345 / 1", result7, 12345);

    // Test 6: Division por si mismo
    std::cout << "\nTest 5: Division por si mismo\n";
    uint128_t g(9999);
    auto result8 = g / g;
    print_result("9999 / 9999", result8, 1);

    // Test 7: Modulo por potencia de 2
    std::cout << "\nTest 6: Modulo por potencia de 2\n";
    uint128_t h(12345);
    auto result9 = h % 256u;
    print_result("12345 % 256", result9, 12345 % 256);

    // Test 8: Division con signedness diferente
    std::cout << "\nTest 7: Division con signedness diferente\n";
    uint128_t ui(1000);
    int128_t si(1000);
    auto result10 = ui / uint128_t(10); // uint / uint
    auto result11 = si / int128_t(10);  // int / int
    std::cout << "uint128(1000) / uint128(10) = " << result10.low() << " ";
    std::cout << (result10.low() == 100 ? "\u2713 PASS\n" : "\u2717 FAIL\n");
    std::cout << "int128(1000) / int128(10) = " << result11.low() << " ";
    std::cout << (result11.low() == 100 ? "\u2713 PASS\n" : "\u2717 FAIL\n");

    std::cout << "\n=== TESTS COMPLETADOS ===\n";
    return 0;
}
