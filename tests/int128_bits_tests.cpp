/**
 * @file int128_bits_tests.cpp
 * @brief Tests completos para funciones de manipulacion de bits en int128_bits.hpp
 */

#include "int128/int128_bits.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace int128_bits;

// Colores ASCII para output
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

// Macro para tests
#define ASSERT_TEST(condition, message)                                                            \
    do {                                                                                           \
        if (!(condition)) {                                                                        \
            std::cerr << COLOR_RED << "[FAIL] " << message << COLOR_RESET << "\n";                 \
            std::cerr << "  Line: " << __LINE__ << "\n";                                           \
            return false;                                                                          \
        }                                                                                          \
    } while (0)

#define TEST_PASS(name) std::cout << COLOR_GREEN << "[PASS] " << name << COLOR_RESET << "\n"

// ===============================================================================
// TESTS STD NAMESPACE
// ===============================================================================

bool test_popcount()
{
    // Test con 0
    ASSERT_TEST(std::popcount(int128_t(0)) == 0, "popcount de 0 debe ser 0");

    // Test con valores positivos pequenos
    ASSERT_TEST(std::popcount(int128_t(1)) == 1, "popcount de 1 debe ser 1");
    ASSERT_TEST(std::popcount(int128_t(3)) == 2, "popcount de 3 (0b11) debe ser 2");
    ASSERT_TEST(std::popcount(int128_t(7)) == 3, "popcount de 7 (0b111) debe ser 3");

    // Test con -1 (todos los bits en 1)
    ASSERT_TEST(std::popcount(int128_t(-1)) == 128, "popcount de -1 debe ser 128");

    // Test con valores negativos
    int128_t neg_value = int128_t(-2); // Complemento a 2 de 2
    int pop = std::popcount(neg_value);
    ASSERT_TEST(pop == 127, "popcount de -2 debe ser 127");

    TEST_PASS("test_popcount");
    return true;
}

bool test_countl_zero()
{
    // Test con 0
    ASSERT_TEST(std::countl_zero(int128_t(0)) == 128, "countl_zero de 0 debe ser 128");

    // Test con valores positivos pequenos
    ASSERT_TEST(std::countl_zero(int128_t(1)) == 127, "countl_zero de 1 debe ser 127");
    ASSERT_TEST(std::countl_zero(int128_t(2)) == 126, "countl_zero de 2 debe ser 126");

    // Test con valores negativos (bit de signo = 1)
    ASSERT_TEST(std::countl_zero(int128_t(-1)) == 0, "countl_zero de -1 debe ser 0");
    ASSERT_TEST(std::countl_zero(int128_t(-100)) == 0, "countl_zero de -100 debe ser 0");

    // Test con valor positivo grande
    int128_t large_positive = int128_t(1) << 126;
    ASSERT_TEST(std::countl_zero(large_positive) == 1, "countl_zero de 2^126 debe ser 1");

    TEST_PASS("test_countl_zero");
    return true;
}

bool test_countr_zero()
{
    // Test con 0
    ASSERT_TEST(std::countr_zero(int128_t(0)) == 128, "countr_zero de 0 debe ser 128");

    // Test con valores positivos
    ASSERT_TEST(std::countr_zero(int128_t(1)) == 0, "countr_zero de 1 debe ser 0");
    ASSERT_TEST(std::countr_zero(int128_t(2)) == 1, "countr_zero de 2 debe ser 1");
    ASSERT_TEST(std::countr_zero(int128_t(4)) == 2, "countr_zero de 4 debe ser 2");
    ASSERT_TEST(std::countr_zero(int128_t(8)) == 3, "countr_zero de 8 debe ser 3");

    // Test con valores negativos pares
    ASSERT_TEST(std::countr_zero(int128_t(-2)) == 1, "countr_zero de -2 debe ser 1");
    ASSERT_TEST(std::countr_zero(int128_t(-4)) == 2, "countr_zero de -4 debe ser 2");

    TEST_PASS("test_countr_zero");
    return true;
}

bool test_countl_one()
{
    // Test con 0
    ASSERT_TEST(std::countl_one(int128_t(0)) == 0, "countl_one de 0 debe ser 0");

    // Test con valores positivos
    ASSERT_TEST(std::countl_one(int128_t(1)) == 0, "countl_one de 1 debe ser 0");

    // Test con -1 (todos unos)
    ASSERT_TEST(std::countl_one(int128_t(-1)) == 128, "countl_one de -1 debe ser 128");

    // Test con valores negativos pequenos
    // -2 en complemento a 2: 1111...1110
    ASSERT_TEST(std::countl_one(int128_t(-2)) == 127, "countl_one de -2 debe ser 127");

    TEST_PASS("test_countl_one");
    return true;
}

bool test_countr_one()
{
    // Test con 0
    ASSERT_TEST(std::countr_one(int128_t(0)) == 0, "countr_one de 0 debe ser 0");

    // Test con valores positivos
    ASSERT_TEST(std::countr_one(int128_t(1)) == 1, "countr_one de 1 debe ser 1");
    ASSERT_TEST(std::countr_one(int128_t(3)) == 2, "countr_one de 3 debe ser 2");
    ASSERT_TEST(std::countr_one(int128_t(7)) == 3, "countr_one de 7 debe ser 3");

    // Test con -1 (todos unos)
    ASSERT_TEST(std::countr_one(int128_t(-1)) == 128, "countr_one de -1 debe ser 128");

    TEST_PASS("test_countr_one");
    return true;
}

bool test_bit_width()
{
    // Test con 0
    ASSERT_TEST(std::bit_width(int128_t(0)) == 0, "bit_width de 0 debe ser 0");

    // Test con valores positivos
    ASSERT_TEST(std::bit_width(int128_t(1)) == 1, "bit_width de 1 debe ser 1");
    ASSERT_TEST(std::bit_width(int128_t(2)) == 2, "bit_width de 2 debe ser 2");
    ASSERT_TEST(std::bit_width(int128_t(7)) == 3, "bit_width de 7 debe ser 3");
    ASSERT_TEST(std::bit_width(int128_t(8)) == 4, "bit_width de 8 debe ser 4");

    // Test con valores negativos (usan todos los bits)
    ASSERT_TEST(std::bit_width(int128_t(-1)) == 128, "bit_width de -1 debe ser 128");
    ASSERT_TEST(std::bit_width(int128_t(-100)) == 128, "bit_width de -100 debe ser 128");

    TEST_PASS("test_bit_width");
    return true;
}

bool test_has_single_bit()
{
    // Test con 0
    ASSERT_TEST(!std::has_single_bit(int128_t(0)), "has_single_bit de 0 debe ser false");

    // Test con potencias de 2 positivas
    ASSERT_TEST(std::has_single_bit(int128_t(1)), "has_single_bit de 1 debe ser true");
    ASSERT_TEST(std::has_single_bit(int128_t(2)), "has_single_bit de 2 debe ser true");
    ASSERT_TEST(std::has_single_bit(int128_t(4)), "has_single_bit de 4 debe ser true");
    ASSERT_TEST(std::has_single_bit(int128_t(8)), "has_single_bit de 8 debe ser true");

    // Test con no potencias de 2
    ASSERT_TEST(!std::has_single_bit(int128_t(3)), "has_single_bit de 3 debe ser false");
    ASSERT_TEST(!std::has_single_bit(int128_t(5)), "has_single_bit de 5 debe ser false");

    // Test con valores negativos (no son potencias de 2)
    ASSERT_TEST(!std::has_single_bit(int128_t(-1)), "has_single_bit de -1 debe ser false");
    ASSERT_TEST(!std::has_single_bit(int128_t(-2)), "has_single_bit de -2 debe ser false");

    TEST_PASS("test_has_single_bit");
    return true;
}

bool test_bit_floor()
{
    // Test con 0
    ASSERT_TEST(std::bit_floor(int128_t(0)) == int128_t(0), "bit_floor de 0 debe ser 0");

    // Test con potencias de 2
    ASSERT_TEST(std::bit_floor(int128_t(1)) == int128_t(1), "bit_floor de 1 debe ser 1");
    ASSERT_TEST(std::bit_floor(int128_t(2)) == int128_t(2), "bit_floor de 2 debe ser 2");
    ASSERT_TEST(std::bit_floor(int128_t(4)) == int128_t(4), "bit_floor de 4 debe ser 4");

    // Test con no potencias de 2
    ASSERT_TEST(std::bit_floor(int128_t(3)) == int128_t(2), "bit_floor de 3 debe ser 2");
    ASSERT_TEST(std::bit_floor(int128_t(5)) == int128_t(4), "bit_floor de 5 debe ser 4");
    ASSERT_TEST(std::bit_floor(int128_t(7)) == int128_t(4), "bit_floor de 7 debe ser 4");

    // Test con valores negativos (no definido)
    ASSERT_TEST(std::bit_floor(int128_t(-1)) == int128_t(0), "bit_floor de -1 debe ser 0");
    ASSERT_TEST(std::bit_floor(int128_t(-100)) == int128_t(0), "bit_floor de -100 debe ser 0");

    TEST_PASS("test_bit_floor");
    return true;
}

bool test_bit_ceil()
{
    // Test con 0 y 1
    ASSERT_TEST(std::bit_ceil(int128_t(0)) == int128_t(1), "bit_ceil de 0 debe ser 1");
    ASSERT_TEST(std::bit_ceil(int128_t(1)) == int128_t(1), "bit_ceil de 1 debe ser 1");

    // Test con potencias de 2
    ASSERT_TEST(std::bit_ceil(int128_t(2)) == int128_t(2), "bit_ceil de 2 debe ser 2");
    ASSERT_TEST(std::bit_ceil(int128_t(4)) == int128_t(4), "bit_ceil de 4 debe ser 4");

    // Test con no potencias de 2
    ASSERT_TEST(std::bit_ceil(int128_t(3)) == int128_t(4), "bit_ceil de 3 debe ser 4");
    ASSERT_TEST(std::bit_ceil(int128_t(5)) == int128_t(8), "bit_ceil de 5 debe ser 8");
    ASSERT_TEST(std::bit_ceil(int128_t(9)) == int128_t(16), "bit_ceil de 9 debe ser 16");

    // Test con valores negativos (no definido)
    ASSERT_TEST(std::bit_ceil(int128_t(-1)) == int128_t(0), "bit_ceil de -1 debe ser 0");

    TEST_PASS("test_bit_ceil");
    return true;
}

// ===============================================================================
// TESTS INT128_BITS NAMESPACE
// ===============================================================================

bool test_rotl()
{
    int128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    // Test rotacion 0
    ASSERT_TEST(rotl(value, 0) == value, "rotl por 0 debe devolver el mismo valor");

    // Test rotacion completa
    ASSERT_TEST(rotl(value, 128) == value, "rotl por 128 debe devolver el mismo valor");

    // Test rotacion 1
    int128_t rotated_1 = rotl(value, 1);
    ASSERT_TEST(rotated_1 != value, "rotl por 1 debe cambiar el valor");

    // Test rotacion negativa
    int128_t rotated_neg = rotl(value, -1);
    ASSERT_TEST(rotated_neg == rotr(value, 1), "rotl por -1 debe ser igual a rotr por 1");

    // Test con valores negativos
    int128_t neg_value = int128_t(-100);
    int128_t rotated_neg_val = rotl(neg_value, 5);
    ASSERT_TEST(rotated_neg_val != neg_value, "rotl de valor negativo debe funcionar");

    TEST_PASS("test_rotl");
    return true;
}

bool test_rotr()
{
    int128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    // Test rotacion 0
    ASSERT_TEST(rotr(value, 0) == value, "rotr por 0 debe devolver el mismo valor");

    // Test rotacion completa
    ASSERT_TEST(rotr(value, 128) == value, "rotr por 128 debe devolver el mismo valor");

    // Test simetria
    ASSERT_TEST(rotl(rotr(value, 7), 7) == value, "rotl debe deshacer rotr");

    // Test con valores negativos
    int128_t neg_value = int128_t(-42);
    int128_t rotated = rotr(neg_value, 3);
    ASSERT_TEST(rotated != neg_value, "rotr de valor negativo debe funcionar");

    TEST_PASS("test_rotr");
    return true;
}

bool test_reverse_bits()
{
    // Test con 0
    ASSERT_TEST(reverse_bits(int128_t(0)) == int128_t(0), "reverse_bits de 0 debe ser 0");

    // Test doble reversa
    int128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    ASSERT_TEST(reverse_bits(reverse_bits(value)) == value,
                "doble reverse_bits debe devolver el original");

    // Test con valores negativos
    int128_t neg_value = int128_t(-1);
    ASSERT_TEST(reverse_bits(neg_value) == neg_value, "reverse_bits de -1 debe ser -1");

    int128_t neg_value2 = int128_t(-100);
    int128_t reversed = reverse_bits(neg_value2);
    ASSERT_TEST(reverse_bits(reversed) == neg_value2, "doble reverse de negativo debe funcionar");

    TEST_PASS("test_reverse_bits");
    return true;
}

bool test_byteswap()
{
    // Test con 0
    ASSERT_TEST(byteswap(int128_t(0)) == int128_t(0), "byteswap de 0 debe ser 0");

    // Test doble swap
    int128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    ASSERT_TEST(byteswap(byteswap(value)) == value, "doble byteswap debe devolver el original");

    // Test con valores negativos
    int128_t neg_value = int128_t(-42);
    int128_t swapped = byteswap(neg_value);
    ASSERT_TEST(byteswap(swapped) == neg_value, "doble byteswap de negativo debe funcionar");

    TEST_PASS("test_byteswap");
    return true;
}

bool test_extract_bits()
{
    int128_t value(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);

    // Test extraccion basica
    int128_t extracted = extract_bits(value, 0, 8);
    ASSERT_TEST(extracted == int128_t(0xFF), "extract_bits de 8 bits debe ser 0xFF");

    // Test con offset
    int128_t value2(0x0, 0x00FF00FF00FF00FFULL);
    extracted = extract_bits(value2, 8, 8);
    ASSERT_TEST(extracted == int128_t(0xFF), "extract_bits con offset debe funcionar");

    // Test limites invalidos
    ASSERT_TEST(extract_bits(value, -1, 8) == int128_t(0),
                "extract_bits con offset negativo debe ser 0");

    // Test con valores negativos
    int128_t neg_value = int128_t(-1);
    extracted = extract_bits(neg_value, 0, 8);
    ASSERT_TEST(extracted == int128_t(0xFF), "extract_bits de -1 debe extraer correctamente");

    TEST_PASS("test_extract_bits");
    return true;
}

bool test_insert_bits()
{
    int128_t dest(0);
    int128_t src(0xFF);

    // Test insercion en posicion 0
    int128_t result = insert_bits(dest, src, 0, 8);
    ASSERT_TEST(result == int128_t(0xFF), "insert_bits en pos 0 debe funcionar");

    // Test insercion en posicion 8
    result = insert_bits(dest, src, 8, 8);
    ASSERT_TEST(result == int128_t(0xFF00), "insert_bits en pos 8 debe funcionar");

    // Test con valores negativos
    dest = int128_t(-1);
    src = int128_t(0);
    result = insert_bits(dest, src, 8, 8);
    ASSERT_TEST((result & int128_t(0xFF00)) == int128_t(0),
                "insert_bits debe limpiar bits en valor negativo");

    TEST_PASS("test_insert_bits");
    return true;
}

bool test_test_bit()
{
    int128_t value(0xFF);

    // Test bits dentro del rango
    ASSERT_TEST(test_bit(value, 0) == true, "test_bit pos 0 debe ser true");
    ASSERT_TEST(test_bit(value, 7) == true, "test_bit pos 7 debe ser true");
    ASSERT_TEST(test_bit(value, 8) == false, "test_bit pos 8 debe ser false");

    // Test bit de signo en negativo
    int128_t neg_value = int128_t(-1);
    ASSERT_TEST(test_bit(neg_value, 127) == true, "test_bit pos 127 de -1 debe ser true");

    // Test limites invalidos
    ASSERT_TEST(test_bit(value, -1) == false, "test_bit pos negativa debe ser false");
    ASSERT_TEST(test_bit(value, 128) == false, "test_bit pos >= 128 debe ser false");

    TEST_PASS("test_test_bit");
    return true;
}

bool test_set_bit()
{
    int128_t value(0);

    // Test set bit 0
    int128_t result = set_bit(value, 0);
    ASSERT_TEST(result == int128_t(1), "set_bit pos 0 debe ser 1");

    // Test set bit 7
    result = set_bit(value, 7);
    ASSERT_TEST(result == int128_t(128), "set_bit pos 7 debe ser 128");

    // Test set bit en valor negativo
    value = int128_t(-2);
    result = set_bit(value, 0);
    ASSERT_TEST(result == int128_t(-1), "set_bit pos 0 en -2 debe ser -1");

    // Test limites invalidos
    result = set_bit(value, -1);
    ASSERT_TEST(result == value, "set_bit pos negativa debe dejar valor sin cambios");

    TEST_PASS("test_set_bit");
    return true;
}

bool test_clear_bit()
{
    int128_t value(0xFF);

    // Test clear bit 0
    int128_t result = clear_bit(value, 0);
    ASSERT_TEST(result == int128_t(0xFE), "clear_bit pos 0 debe limpiar bit");

    // Test clear en -1
    value = int128_t(-1);
    result = clear_bit(value, 0);
    ASSERT_TEST(result == int128_t(-2), "clear_bit pos 0 en -1 debe ser -2");

    // Test limites invalidos
    result = clear_bit(value, -1);
    ASSERT_TEST(result == value, "clear_bit pos negativa debe dejar valor sin cambios");

    TEST_PASS("test_clear_bit");
    return true;
}

bool test_flip_bit()
{
    int128_t value(0);

    // Test flip de bit 0
    int128_t result = flip_bit(value, 0);
    ASSERT_TEST(result == int128_t(1), "flip_bit de bit 0 debe ser 1");

    // Test flip de bit ya establecido
    value = int128_t(1);
    result = flip_bit(value, 0);
    ASSERT_TEST(result == int128_t(0), "flip_bit de bit establecido debe limpiarlo");

    // Test flip en valor negativo
    value = int128_t(-1);
    result = flip_bit(value, 0);
    ASSERT_TEST(result == int128_t(-2), "flip_bit pos 0 en -1 debe ser -2");

    // Test limites invalidos
    result = flip_bit(value, -1);
    ASSERT_TEST(result == value, "flip_bit pos negativa debe dejar valor sin cambios");

    TEST_PASS("test_flip_bit");
    return true;
}

bool test_find_first_set()
{
    // Test con 0
    ASSERT_TEST(find_first_set(int128_t(0)) == -1, "find_first_set de 0 debe ser -1");

    // Test con valores positivos
    ASSERT_TEST(find_first_set(int128_t(1)) == 0, "find_first_set de 1 debe ser 0");
    ASSERT_TEST(find_first_set(int128_t(2)) == 1, "find_first_set de 2 debe ser 1");
    ASSERT_TEST(find_first_set(int128_t(4)) == 2, "find_first_set de 4 debe ser 2");
    ASSERT_TEST(find_first_set(int128_t(8)) == 3, "find_first_set de 8 debe ser 3");

    // Test con valores negativos
    ASSERT_TEST(find_first_set(int128_t(-1)) == 0, "find_first_set de -1 debe ser 0");
    ASSERT_TEST(find_first_set(int128_t(-2)) == 1, "find_first_set de -2 debe ser 1");
    ASSERT_TEST(find_first_set(int128_t(-4)) == 2, "find_first_set de -4 debe ser 2");

    TEST_PASS("test_find_first_set");
    return true;
}

bool test_find_last_set()
{
    // Test con 0
    ASSERT_TEST(find_last_set(int128_t(0)) == -1, "find_last_set de 0 debe ser -1");

    // Test con valores positivos
    ASSERT_TEST(find_last_set(int128_t(1)) == 0, "find_last_set de 1 debe ser 0");
    ASSERT_TEST(find_last_set(int128_t(2)) == 1, "find_last_set de 2 debe ser 1");
    ASSERT_TEST(find_last_set(int128_t(4)) == 2, "find_last_set de 4 debe ser 2");

    // Test con valores negativos (bit mas alto siempre es 1)
    int fls_neg1 = find_last_set(int128_t(-1));
    ASSERT_TEST(fls_neg1 >= 0, "find_last_set de -1 debe encontrar bit alto");

    TEST_PASS("test_find_last_set");
    return true;
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  int128_bits.hpp Tests\n";
    std::cout << "========================================\n\n";

    std::cout << "--- Tests std namespace ---\n";
    bool all_passed = true;

    all_passed &= test_popcount();
    all_passed &= test_countl_zero();
    all_passed &= test_countr_zero();
    all_passed &= test_countl_one();
    all_passed &= test_countr_one();
    all_passed &= test_bit_width();
    all_passed &= test_has_single_bit();
    all_passed &= test_bit_floor();
    all_passed &= test_bit_ceil();

    std::cout << "\n--- Tests int128_bits namespace ---\n";

    all_passed &= test_rotl();
    all_passed &= test_rotr();
    all_passed &= test_reverse_bits();
    all_passed &= test_byteswap();
    all_passed &= test_extract_bits();
    all_passed &= test_insert_bits();
    all_passed &= test_test_bit();
    all_passed &= test_set_bit();
    all_passed &= test_clear_bit();
    all_passed &= test_flip_bit();
    all_passed &= test_find_first_set();
    all_passed &= test_find_last_set();

    std::cout << "\n========================================\n";
    if (all_passed) {
        std::cout << COLOR_GREEN << "[OK] Todos los tests pasaron" << COLOR_RESET << "\n";
        std::cout << "========================================\n";
        return 0;
    } else {
        std::cout << COLOR_RED << "[FAIL] Algunos tests fallaron" << COLOR_RESET << "\n";
        std::cout << "========================================\n";
        return 1;
    }
}
