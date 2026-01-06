// Test de compilacion para int128_base.hpp con las nuevas caracteristicas
#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    // Test 1: divrem_by_10()
    uint128_t val1(0, 123);
    auto [q1, r1] = val1.divrem_by_10();
    std::cout << "Test divrem_by_10(): PASS\n";

    // Test 2: Comparaciones con integral_builtin
    uint128_t val2(0, 100);
    bool eq = (val2 == 100u);
    bool ne = (val2 != 50u);
    bool lt = (val2 < 200u);
    std::cout << "Test comparaciones integral: PASS\n";

    // Test 3: Comparaciones con floating_point
    double d = 3.14;
    bool feq = (val2 == d);
    bool fne = (val2 != d);
    std::cout << "Test comparaciones floating: PASS\n";

    // Test 4: Operadores bitwise con S2
    uint128_t u_val(0, 0xFF);
    int128_t s_val(0, 0x0F);

    auto result1 = u_val & s_val; // uint128 & int128
    auto result2 = u_val | s_val; // uint128 | int128
    auto result3 = u_val ^ s_val; // uint128 ^ int128
    std::cout << "Test operadores bitwise S2: PASS\n";

    // Test 5: Operadores aritmeticos con S2 (ya existian)
    auto result4 = u_val + s_val; // uint128 + int128
    auto result5 = u_val - s_val; // uint128 - int128
    std::cout << "Test operadores aritmeticos S2: PASS\n";

    std::cout << "\n=== TODOS LOS TESTS PASARON ===\n";
    return 0;
}
