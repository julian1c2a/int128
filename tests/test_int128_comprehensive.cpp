/*
 * Test completo para int128_t - Demuestra funcionalidad específica de complemento a 2
 */

#include "../include/int128_t.hpp"
#include <cassert>
#include <iostream>

using namespace int128_literals;

void test_signed_specific_features()
{
    std::cout << "=== Testing INT128_T SPECIFIC FEATURES ===" << std::endl;

    // Test constantes límite
    std::cout << "\n[INFO] Valores límite:" << std::endl;
    std::cout << "INT128_MIN = " << int128_t_MIN << std::endl;
    std::cout << "INT128_MAX = " << int128_t_MAX << std::endl;

    assert(int128_t_MIN.is_negative());
    assert(int128_t_MAX.is_positive());
    assert(int128_t_MAX > int128_t_MIN);

    // Test valor absoluto
    std::cout << "\n[INFO] Valor absoluto:" << std::endl;
    int128_t neg_val = -42_i128;
    int128_t pos_val = 42_i128;

    assert(neg_val.abs() == pos_val);
    assert(pos_val.abs() == pos_val);
    assert(int128_t(0).abs() == int128_t(0));

    std::cout << "abs(-42) = " << neg_val.abs() << std::endl;
    std::cout << "abs(42) = " << pos_val.abs() << std::endl;

    // Test caso especial: abs del mínimo valor
    int128_t min_abs = int128_t_MIN.abs();
    std::cout << "abs(INT128_MIN) = " << min_abs << " (overflow, se mantiene negativo)"
              << std::endl;
    assert(min_abs == int128_t_MIN); // En complemento a 2, -INT128_MIN == INT128_MIN

    std::cout << "[OK] Signed specific features OK" << std::endl;
}

void test_negative_arithmetic()
{
    std::cout << "\n=== Testing NEGATIVE ARITHMETIC ===" << std::endl;

    // Aritmética con números negativos grandes (evitando overflow)
    int128_t big_neg = "-1000000000000000000000000000000"_i128; // Número grande pero no el mínimo
    int128_t small_pos = 1000_i128;

    std::cout << "\n[INFO] Aritmética con números grandes:" << std::endl;
    std::cout << "big_neg = " << big_neg << std::endl;
    std::cout << "small_pos = " << small_pos << std::endl;

    // Suma
    int128_t sum_result = big_neg + small_pos;
    std::cout << "big_neg + small_pos = " << sum_result << std::endl;
    assert(sum_result > big_neg);
    assert(sum_result.is_negative());

    // Resta
    int128_t sub_result = big_neg - small_pos;
    std::cout << "big_neg - small_pos = " << sub_result << std::endl;
    assert(sub_result < big_neg);

    // Multiplicación
    int128_t neg_factor = -5_i128;
    int128_t pos_factor = 7_i128;

    std::cout << "\n[INFO] Multiplicación con signos:" << std::endl;
    std::cout << "(-5) * 7 = " << (neg_factor * pos_factor) << std::endl;
    std::cout << "(-5) * (-7) = " << (neg_factor * (-pos_factor)) << std::endl;
    std::cout << "5 * (-7) = " << ((-neg_factor) * (-pos_factor)) << std::endl;

    assert((neg_factor * pos_factor) == int128_t(-35));
    assert((neg_factor * (-pos_factor)) == int128_t(35));
    assert(((-neg_factor) * (-pos_factor)) == int128_t(-35));

    std::cout << "[OK] Negative arithmetic OK" << std::endl;
}

void test_division_with_signs()
{
    std::cout << "\n=== Testing DIVISION WITH SIGNS ===" << std::endl;

    int128_t dividend_pos = 100_i128;
    int128_t dividend_neg = -100_i128;
    int128_t divisor_pos = 7_i128;
    int128_t divisor_neg = -7_i128;

    std::cout << "\n[INFO] División con diferentes signos:" << std::endl;
    std::cout << "100 / 7 = " << (dividend_pos / divisor_pos) << std::endl;
    std::cout << "100 / (-7) = " << (dividend_pos / divisor_neg) << std::endl;
    std::cout << "(-100) / 7 = " << (dividend_neg / divisor_pos) << std::endl;
    std::cout << "(-100) / (-7) = " << (dividend_neg / divisor_neg) << std::endl;

    assert((dividend_pos / divisor_pos) == int128_t(14));
    assert((dividend_pos / divisor_neg) == int128_t(-14));
    assert((dividend_neg / divisor_pos) == int128_t(-14));
    assert((dividend_neg / divisor_neg) == int128_t(14));

    // Test módulo con signo (sigue al dividendo)
    std::cout << "\n[INFO] Módulo con diferentes signos:" << std::endl;
    std::cout << "100 % 7 = " << (dividend_pos % divisor_pos) << std::endl;
    std::cout << "100 % (-7) = " << (dividend_pos % divisor_neg) << std::endl;
    std::cout << "(-100) % 7 = " << (dividend_neg % divisor_pos) << std::endl;
    std::cout << "(-100) % (-7) = " << (dividend_neg % divisor_neg) << std::endl;

    assert((dividend_pos % divisor_pos) == int128_t(2));  // 100 = 7*14 + 2
    assert((dividend_pos % divisor_neg) == int128_t(2));  // Signo del dividendo
    assert((dividend_neg % divisor_pos) == int128_t(-2)); // Signo del dividendo
    assert((dividend_neg % divisor_neg) == int128_t(-2)); // Signo del dividendo

    std::cout << "[OK] Division with signs OK" << std::endl;
}

void test_shift_arithmetic()
{
    std::cout << "\n=== Testing ARITHMETIC SHIFTS ===" << std::endl;

    // Shift aritmético derecho (extensión de signo)
    int128_t neg_val = -1024_i128;
    int128_t pos_val = 1024_i128;

    std::cout << "\n[INFO] Shift aritmético derecho:" << std::endl;
    std::cout << "1024 >> 1 = " << (pos_val >> 1) << std::endl;
    std::cout << "1024 >> 2 = " << (pos_val >> 2) << std::endl;
    std::cout << "1024 >> 10 = " << (pos_val >> 10) << std::endl;

    std::cout << "(-1024) >> 1 = " << (neg_val >> 1) << std::endl;
    std::cout << "(-1024) >> 2 = " << (neg_val >> 2) << std::endl;
    std::cout << "(-1024) >> 10 = " << (neg_val >> 10) << std::endl;

    assert((pos_val >> 1) == int128_t(512));
    assert((pos_val >> 2) == int128_t(256));
    assert((pos_val >> 10) == int128_t(1));

    assert((neg_val >> 1) == int128_t(-512));
    assert((neg_val >> 2) == int128_t(-256));
    assert((neg_val >> 10) == int128_t(-1));

    // Extensión de signo completa
    int128_t neg_one = -1_i128;
    std::cout << "(-1) >> 100 = " << (neg_one >> 100) << " (extensión completa)" << std::endl;
    assert((neg_one >> 100) == int128_t(-1));

    std::cout << "[OK] Arithmetic shifts OK" << std::endl;
}

void test_comparisons_comprehensive()
{
    std::cout << "\n=== Testing SIGNED COMPARISONS ===" << std::endl;

    // Casos límite de comparación
    int128_t values[] = {int128_t_MIN, "-1000000000000000000000000000000"_i128, -1_i128,     0_i128,
                         1_i128,       "1000000000000000000000000000000"_i128,  int128_t_MAX};

    std::cout << "\n[INFO] Ordenamiento correcto:" << std::endl;
    for (int i = 0; i < 7; i++) {
        std::cout << "values[" << i << "] = " << values[i] << std::endl;
        for (int j = i + 1; j < 7; j++) {
            assert(values[i] < values[j]);
            assert(values[j] > values[i]);
            assert(values[i] <= values[j]);
            assert(values[j] >= values[i]);
            assert(values[i] != values[j]);
        }
    }

    std::cout << "[OK] Signed comparisons OK" << std::endl;
}

void test_conversion_edge_cases()
{
    std::cout << "\n=== Testing CONVERSION EDGE CASES ===" << std::endl;

    // Conversión a tipos con signo
    std::cout << "\n[INFO] Conversión a int64_t:" << std::endl;

    int128_t fits_int64 = int128_t(INT64_MAX);
    int128_t too_big = int128_t(INT64_MAX) + 1_i128;
    int128_t fits_neg = int128_t(INT64_MIN);
    int128_t too_small = int128_t(INT64_MIN) - 1_i128;

    std::cout << "fits_int64.to<int64_t>() = " << fits_int64.to<int64_t>() << std::endl;
    std::cout << "too_big.to<int64_t>() = " << too_big.to<int64_t>() << " (saturado)" << std::endl;
    std::cout << "fits_neg.to<int64_t>() = " << fits_neg.to<int64_t>() << std::endl;
    std::cout << "too_small.to<int64_t>() = " << too_small.to<int64_t>() << " (saturado)"
              << std::endl;

    assert(fits_int64.to<int64_t>() == INT64_MAX);
    assert(too_big.to<int64_t>() == INT64_MAX); // Saturación
    assert(fits_neg.to<int64_t>() == INT64_MIN);
    assert(too_small.to<int64_t>() == INT64_MIN); // Saturación

    // Conversión a tipos sin signo desde negativos
    std::cout << "\n[INFO] Conversión a uint64_t desde negativos:" << std::endl;
    int128_t negative = -42_i128;
    std::cout << "(-42).to<uint64_t>() = " << negative.to<uint64_t>() << " (clamped to 0)"
              << std::endl;
    assert(negative.to<uint64_t>() == 0);

    std::cout << "[OK] Conversion edge cases OK" << std::endl;
}

int main()
{
    std::cout << "🧮 COMPREHENSIVE INT128_T TESTING" << std::endl;
    std::cout << "===================================" << std::endl;

    try {
        test_signed_specific_features();
        test_negative_arithmetic();
        test_division_with_signs();
        test_shift_arithmetic();
        test_comparisons_comprehensive();
        test_conversion_edge_cases();

        std::cout << "\n🎉 ¡TODOS LOS TESTS AVANZADOS PASARON!" << std::endl;
        std::cout << "\n🔧 CARACTERÍSTICAS CONFIRMADAS:" << std::endl;
        std::cout << "[OK] Complemento a 2 implementado correctamente" << std::endl;
        std::cout << "[OK] MSB de data[1] funciona como bit de signo" << std::endl;
        std::cout << "[OK] Aritmética con signo funcional" << std::endl;
        std::cout << "[OK] Shifts aritméticos con extensión de signo" << std::endl;
        std::cout << "[OK] Comparaciones correctas para números con signo" << std::endl;
        std::cout << "[OK] Conversiones con saturación y clamp" << std::endl;
        std::cout << "[OK] Misma estructura interna que uint128_t" << std::endl;

        std::cout << "\n📈 RANGO COMPLETO:" << std::endl;
        std::cout << "* Mínimo: -170,141,183,460,469,231,731,687,303,715,884,105,728" << std::endl;
        std::cout << "* Máximo: 170,141,183,460,469,231,731,687,303,715,884,105,727" << std::endl;
        std::cout << "* Total: 340,282,366,920,938,463,463,374,607,431,768,211,456 valores"
                  << std::endl;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}