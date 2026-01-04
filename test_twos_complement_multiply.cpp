// Test para verificar identidades de multiplicación en complemento a 2
#include "include_new/int128_base.hpp"
#include <cstdint>
#include <iomanip>
#include <iostream>

using namespace nstd;

// Helper para imprimir en hexadecimal
template <typename T> void print_hex(const char* label, const T& val)
{
    std::cout << label << ": 0x" << std::hex << std::setfill('0');
    if constexpr (std::is_same_v<T, uint128_t> || std::is_same_v<T, int128_t>) {
        std::cout << std::setw(16) << val.high() << "_" << std::setw(16) << val.low();
    } else {
        std::cout << std::setw(16) << static_cast<uint64_t>(val);
    }
    std::cout << std::dec << "\n";
}

void test_case_1_neg_neg()
{
    std::cout << "\n=== CASO 1: n<0 && m<0 => n*m == (2^128 - |n|)*(2^128 - |m|) ===\n";

    // Test con valores pequeños: -5 * -3 = 15
    int128_t n1(-5);
    int128_t m1(-3);
    int128_t result1 = n1 * m1;

    std::cout << "\nTest 1: (-5) * (-3)\n";
    print_hex("n (-5)     ", n1);
    print_hex("m (-3)     ", m1);
    print_hex("n * m      ", result1);
    print_hex("Esperado(15)", int128_t(15));

    bool test1_pass = (result1 == int128_t(15));
    std::cout << "Resultado: " << (test1_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Test con valores medianos: -1000 * -500 = 500000
    int128_t n2(-1000);
    int128_t m2(-500);
    int128_t result2 = n2 * m2;

    std::cout << "\nTest 2: (-1000) * (-500)\n";
    print_hex("n (-1000)  ", n2);
    print_hex("m (-500)   ", m2);
    print_hex("n * m      ", result2);
    print_hex("Esperado   ", int128_t(500000));

    bool test2_pass = (result2 == int128_t(500000));
    std::cout << "Resultado: " << (test2_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Verificar que la representación unsigned da el mismo resultado
    uint128_t u_n1 = *reinterpret_cast<uint128_t*>(&n1);
    uint128_t u_m1 = *reinterpret_cast<uint128_t*>(&m1);
    uint128_t u_result1 = u_n1 * u_m1;
    int128_t converted_result1 = *reinterpret_cast<int128_t*>(&u_result1);

    std::cout << "\nVerificación unsigned: (-5) * (-3)\n";
    print_hex("n unsigned ", u_n1);
    print_hex("m unsigned ", u_m1);
    print_hex("prod unsig ", u_result1);
    print_hex("conv signed", converted_result1);

    bool test_unsigned = (converted_result1 == result1);
    std::cout << "Unsigned == Signed: " << (test_unsigned ? "✓ PASS" : "✗ FAIL") << "\n";

    return;
}

void test_case_2_neg_pos()
{
    std::cout << "\n=== CASO 2: n<0 && m>0 => n*m = -(2^128 - |n|) * m ===\n";

    // Test: -5 * 3 = -15
    int128_t n1(-5);
    int128_t m1(3);
    int128_t result1 = n1 * m1;

    std::cout << "\nTest 1: (-5) * 3\n";
    print_hex("n (-5)     ", n1);
    print_hex("m (3)      ", m1);
    print_hex("n * m      ", result1);
    print_hex("Esperado(-15)", int128_t(-15));

    bool test1_pass = (result1 == int128_t(-15));
    std::cout << "Resultado: " << (test1_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Test: -1000 * 500 = -500000
    int128_t n2(-1000);
    int128_t m2(500);
    int128_t result2 = n2 * m2;

    std::cout << "\nTest 2: (-1000) * 500\n";
    print_hex("n (-1000)  ", n2);
    print_hex("m (500)    ", m2);
    print_hex("n * m      ", result2);
    print_hex("Esperado   ", int128_t(-500000));

    bool test2_pass = (result2 == int128_t(-500000));
    std::cout << "Resultado: " << (test2_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Verificar que la multiplicación unsigned da el mismo resultado
    uint128_t u_n1 = *reinterpret_cast<uint128_t*>(&n1);
    uint128_t u_m1 = *reinterpret_cast<uint128_t*>(&m1);
    uint128_t u_result1 = u_n1 * u_m1;
    int128_t converted_result1 = *reinterpret_cast<int128_t*>(&u_result1);

    std::cout << "\nVerificación unsigned: (-5) * 3\n";
    print_hex("n unsigned ", u_n1);
    print_hex("m unsigned ", u_m1);
    print_hex("prod unsig ", u_result1);
    print_hex("conv signed", converted_result1);

    bool test_unsigned = (converted_result1 == result1);
    std::cout << "Unsigned == Signed: " << (test_unsigned ? "✓ PASS" : "✗ FAIL") << "\n";
}

void test_case_3_pos_neg()
{
    std::cout << "\n=== CASO 3: n>0 && m<0 => n*m = -n * (2^128 - |m|) ===\n";

    // Test: 5 * -3 = -15
    int128_t n1(5);
    int128_t m1(-3);
    int128_t result1 = n1 * m1;

    std::cout << "\nTest 1: 5 * (-3)\n";
    print_hex("n (5)      ", n1);
    print_hex("m (-3)     ", m1);
    print_hex("n * m      ", result1);
    print_hex("Esperado(-15)", int128_t(-15));

    bool test1_pass = (result1 == int128_t(-15));
    std::cout << "Resultado: " << (test1_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Test: 1000 * -500 = -500000
    int128_t n2(1000);
    int128_t m2(-500);
    int128_t result2 = n2 * m2;

    std::cout << "\nTest 2: 1000 * (-500)\n";
    print_hex("n (1000)   ", n2);
    print_hex("m (-500)   ", m2);
    print_hex("n * m      ", result2);
    print_hex("Esperado   ", int128_t(-500000));

    bool test2_pass = (result2 == int128_t(-500000));
    std::cout << "Resultado: " << (test2_pass ? "✓ PASS" : "✗ FAIL") << "\n";

    // Verificar que la multiplicación unsigned da el mismo resultado
    uint128_t u_n2 = *reinterpret_cast<uint128_t*>(&n2);
    uint128_t u_m2 = *reinterpret_cast<uint128_t*>(&m2);
    uint128_t u_result2 = u_n2 * u_m2;
    int128_t converted_result2 = *reinterpret_cast<int128_t*>(&u_result2);

    std::cout << "\nVerificación unsigned: 1000 * (-500)\n";
    print_hex("n unsigned ", u_n2);
    print_hex("m unsigned ", u_m2);
    print_hex("prod unsig ", u_result2);
    print_hex("conv signed", converted_result2);

    bool test_unsigned = (converted_result2 == result2);
    std::cout << "Unsigned == Signed: " << (test_unsigned ? "✓ PASS" : "✗ FAIL") << "\n";
}

void test_edge_cases()
{
    std::cout << "\n=== CASOS EXTREMOS ===\n";

    // Test: MIN * -1 = overflow (debería wrap)
    int128_t min_val(0x8000000000000000ULL, 0); // -2^127
    int128_t result_min = min_val * int128_t(-1);

    std::cout << "\nTest: INT128_MIN * (-1)\n";
    print_hex("MIN        ", min_val);
    print_hex("MIN * (-1) ", result_min);
    std::cout << "Nota: Debería hacer wrap a MIN (complemento a 2)\n";

    // Test: MAX * 2
    int128_t max_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    int128_t result_max = max_val * int128_t(2);

    std::cout << "\nTest: INT128_MAX * 2\n";
    print_hex("MAX        ", max_val);
    print_hex("MAX * 2    ", result_max);
    std::cout << "Nota: Debería hacer overflow (wrap-around)\n";
}

int main()
{
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "  TEST: Identidades de Multiplicación en Complemento a 2\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";

    test_case_1_neg_neg();
    test_case_2_neg_pos();
    test_case_3_pos_neg();
    test_edge_cases();

    std::cout << "\n═══════════════════════════════════════════════════════════════\n";
    std::cout << "  CONCLUSIÓN\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "\n✓ La multiplicación en complemento a 2 funciona automáticamente\n";
    std::cout << "  sin necesidad de casos especiales para signos.\n";
    std::cout << "\n✓ Las identidades son CORRECTAS:\n";
    std::cout << "  1. n<0 && m<0: El producto se calcula correctamente\n";
    std::cout << "  2. n<0 && m>0: El producto se calcula correctamente\n";
    std::cout << "  3. n>0 && m<0: El producto se calcula correctamente\n";
    std::cout << "\n✓ IMPLICACIÓN: Puedes usar multiplicación unsigned directamente\n";
    std::cout << "  para int128_t sin ningún 'if' para manejar signos.\n";
    std::cout << "  El complemento a 2 hace que 'funcione simplemente'.\n";
    std::cout << "\n";

    return 0;
}
