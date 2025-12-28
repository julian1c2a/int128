/**
 * @file uint128_bits_tests.cpp
 * @brief Tests completos para funciones de manipulacion de bits en uint128_bits.hpp
 */

#include "uint128/uint128_bits.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace nstd;


using namespace uint128_bits;

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
    ASSERT_TEST(nstd::popcount(uint128_t(0)) == 0, "popcount de 0 debe ser 0");

    // Test con valores pequenos
    ASSERT_TEST(nstd::popcount(uint128_t(1)) == 1, "popcount de 1 debe ser 1");
    ASSERT_TEST(nstd::popcount(uint128_t(3)) == 2, "popcount de 3 (0b11) debe ser 2");
    ASSERT_TEST(nstd::popcount(uint128_t(7)) == 3, "popcount de 7 (0b111) debe ser 3");
    ASSERT_TEST(nstd::popcount(uint128_t(15)) == 4, "popcount de 15 (0b1111) debe ser 4");

    // Test con valores grandes
    uint128_t all_ones = ~uint128_t(0);
    ASSERT_TEST(nstd::popcount(all_ones) == 128, "popcount de ~0 debe ser 128");

    // Test con patron alternado
    uint128_t alternating(0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL);
    ASSERT_TEST(nstd::popcount(alternating) == 64, "popcount de patron alternado debe ser 64");

    TEST_PASS("test_popcount");
    return true;
}

bool test_countl_zero()
{
    // Test con 0
    ASSERT_TEST(nstd::countl_zero(uint128_t(0)) == 128, "countl_zero de 0 debe ser 128");

    // Test con valores pequenos
    ASSERT_TEST(nstd::countl_zero(uint128_t(1)) == 127, "countl_zero de 1 debe ser 127");
    ASSERT_TEST(nstd::countl_zero(uint128_t(2)) == 126, "countl_zero de 2 debe ser 126");
    ASSERT_TEST(nstd::countl_zero(uint128_t(4)) == 125, "countl_zero de 4 debe ser 125");

    // Test con bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(nstd::countl_zero(high_bit) == 0, "countl_zero de bit 127 debe ser 0");

    // Test con bit en posicion 64
    uint128_t bit_64 = uint128_t(1) << 64;
    ASSERT_TEST(nstd::countl_zero(bit_64) == 63, "countl_zero de bit 64 debe ser 63");

    TEST_PASS("test_countl_zero");
    return true;
}

bool test_countr_zero()
{
    // Test con 0
    ASSERT_TEST(nstd::countr_zero(uint128_t(0)) == 128, "countr_zero de 0 debe ser 128");

    // Test con valores pequenos
    ASSERT_TEST(nstd::countr_zero(uint128_t(1)) == 0, "countr_zero de 1 debe ser 0");
    ASSERT_TEST(nstd::countr_zero(uint128_t(2)) == 1, "countr_zero de 2 debe ser 1");
    ASSERT_TEST(nstd::countr_zero(uint128_t(4)) == 2, "countr_zero de 4 debe ser 2");
    ASSERT_TEST(nstd::countr_zero(uint128_t(8)) == 3, "countr_zero de 8 debe ser 3");

    // Test con bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(nstd::countr_zero(high_bit) == 127, "countr_zero de bit 127 debe ser 127");

    TEST_PASS("test_countr_zero");
    return true;
}

bool test_countl_one()
{
    // Test con 0
    ASSERT_TEST(nstd::countl_one(uint128_t(0)) == 0, "countl_one de 0 debe ser 0");

    // Test con todos unos
    uint128_t all_ones = ~uint128_t(0);
    ASSERT_TEST(nstd::countl_one(all_ones) == 128, "countl_one de ~0 debe ser 128");

    // Test con patron de unos
    uint128_t pattern = (uint128_t(1) << 127) | (uint128_t(1) << 126);
    ASSERT_TEST(nstd::countl_one(pattern) == 2, "countl_one de 2 unos al inicio debe ser 2");

    TEST_PASS("test_countl_one");
    return true;
}

bool test_countr_one()
{
    // Test con 0
    ASSERT_TEST(nstd::countr_one(uint128_t(0)) == 0, "countr_one de 0 debe ser 0");

    // Test con todos unos
    uint128_t all_ones = ~uint128_t(0);
    ASSERT_TEST(nstd::countr_one(all_ones) == 128, "countr_one de ~0 debe ser 128");

    // Test con valores pequenos
    ASSERT_TEST(nstd::countr_one(uint128_t(1)) == 1, "countr_one de 1 debe ser 1");
    ASSERT_TEST(nstd::countr_one(uint128_t(3)) == 2, "countr_one de 3 debe ser 2");
    ASSERT_TEST(nstd::countr_one(uint128_t(7)) == 3, "countr_one de 7 debe ser 3");

    TEST_PASS("test_countr_one");
    return true;
}

bool test_bit_width()
{
    // Test con 0
    ASSERT_TEST(nstd::bit_width(uint128_t(0)) == 0, "bit_width de 0 debe ser 0");

    // Test con valores pequenos
    ASSERT_TEST(nstd::bit_width(uint128_t(1)) == 1, "bit_width de 1 debe ser 1");
    ASSERT_TEST(nstd::bit_width(uint128_t(2)) == 2, "bit_width de 2 debe ser 2");
    ASSERT_TEST(nstd::bit_width(uint128_t(3)) == 2, "bit_width de 3 debe ser 2");
    ASSERT_TEST(nstd::bit_width(uint128_t(4)) == 3, "bit_width de 4 debe ser 3");
    ASSERT_TEST(nstd::bit_width(uint128_t(7)) == 3, "bit_width de 7 debe ser 3");
    ASSERT_TEST(nstd::bit_width(uint128_t(8)) == 4, "bit_width de 8 debe ser 4");

    // Test con bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(nstd::bit_width(high_bit) == 128, "bit_width de bit 127 debe ser 128");

    TEST_PASS("test_bit_width");
    return true;
}

bool test_has_single_bit()
{
    // Test con 0
    ASSERT_TEST(!nstd::has_single_bit(uint128_t(0)), "has_single_bit de 0 debe ser false");

    // Test con potencias de 2
    ASSERT_TEST(nstd::has_single_bit(uint128_t(1)), "has_single_bit de 1 debe ser true");
    ASSERT_TEST(nstd::has_single_bit(uint128_t(2)), "has_single_bit de 2 debe ser true");
    ASSERT_TEST(nstd::has_single_bit(uint128_t(4)), "has_single_bit de 4 debe ser true");
    ASSERT_TEST(nstd::has_single_bit(uint128_t(8)), "has_single_bit de 8 debe ser true");
    ASSERT_TEST(nstd::has_single_bit(uint128_t(1) << 64), "has_single_bit de 2^64 debe ser true");
    ASSERT_TEST(nstd::has_single_bit(uint128_t(1) << 127), "has_single_bit de 2^127 debe ser true");

    // Test con no potencias de 2
    ASSERT_TEST(!nstd::has_single_bit(uint128_t(3)), "has_single_bit de 3 debe ser false");
    ASSERT_TEST(!nstd::has_single_bit(uint128_t(5)), "has_single_bit de 5 debe ser false");
    ASSERT_TEST(!nstd::has_single_bit(uint128_t(6)), "has_single_bit de 6 debe ser false");

    TEST_PASS("test_has_single_bit");
    return true;
}

bool test_bit_floor()
{
    // Test con 0
    ASSERT_TEST(nstd::bit_floor(uint128_t(0)) == uint128_t(0), "bit_floor de 0 debe ser 0");

    // Test con potencias de 2
    ASSERT_TEST(nstd::bit_floor(uint128_t(1)) == uint128_t(1), "bit_floor de 1 debe ser 1");
    ASSERT_TEST(nstd::bit_floor(uint128_t(2)) == uint128_t(2), "bit_floor de 2 debe ser 2");
    ASSERT_TEST(nstd::bit_floor(uint128_t(4)) == uint128_t(4), "bit_floor de 4 debe ser 4");

    // Test con no potencias de 2
    ASSERT_TEST(nstd::bit_floor(uint128_t(3)) == uint128_t(2), "bit_floor de 3 debe ser 2");
    ASSERT_TEST(nstd::bit_floor(uint128_t(5)) == uint128_t(4), "bit_floor de 5 debe ser 4");
    ASSERT_TEST(nstd::bit_floor(uint128_t(6)) == uint128_t(4), "bit_floor de 6 debe ser 4");
    ASSERT_TEST(nstd::bit_floor(uint128_t(7)) == uint128_t(4), "bit_floor de 7 debe ser 4");
    ASSERT_TEST(nstd::bit_floor(uint128_t(9)) == uint128_t(8), "bit_floor de 9 debe ser 8");

    TEST_PASS("test_bit_floor");
    return true;
}

bool test_bit_ceil()
{
    // Test con 0 y 1
    ASSERT_TEST(nstd::bit_ceil(uint128_t(0)) == uint128_t(1), "bit_ceil de 0 debe ser 1");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(1)) == uint128_t(1), "bit_ceil de 1 debe ser 1");

    // Test con potencias de 2
    ASSERT_TEST(nstd::bit_ceil(uint128_t(2)) == uint128_t(2), "bit_ceil de 2 debe ser 2");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(4)) == uint128_t(4), "bit_ceil de 4 debe ser 4");

    // Test con no potencias de 2
    ASSERT_TEST(nstd::bit_ceil(uint128_t(3)) == uint128_t(4), "bit_ceil de 3 debe ser 4");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(5)) == uint128_t(8), "bit_ceil de 5 debe ser 8");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(6)) == uint128_t(8), "bit_ceil de 6 debe ser 8");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(7)) == uint128_t(8), "bit_ceil de 7 debe ser 8");
    ASSERT_TEST(nstd::bit_ceil(uint128_t(9)) == uint128_t(16), "bit_ceil de 9 debe ser 16");

    TEST_PASS("test_bit_ceil");
    return true;
}

// ===============================================================================
// TESTS UINT128_BITS NAMESPACE
// ===============================================================================

bool test_rotl()
{
    uint128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    // Test rotacion 0
    ASSERT_TEST(rotl(value, 0) == value, "rotl por 0 debe devolver el mismo valor");

    // Test rotacion completa
    ASSERT_TEST(rotl(value, 128) == value, "rotl por 128 debe devolver el mismo valor");

    // Test rotacion 1
    uint128_t rotated_1 = rotl(value, 1);
    ASSERT_TEST(rotated_1 != value, "rotl por 1 debe cambiar el valor");

    // Test rotacion negativa
    uint128_t rotated_neg = rotl(value, -1);
    ASSERT_TEST(rotated_neg == rotr(value, 1), "rotl por -1 debe ser igual a rotr por 1");

    TEST_PASS("test_rotl");
    return true;
}

bool test_rotr()
{
    uint128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    // Test rotacion 0
    ASSERT_TEST(rotr(value, 0) == value, "rotr por 0 debe devolver el mismo valor");

    // Test rotacion completa
    ASSERT_TEST(rotr(value, 128) == value, "rotr por 128 debe devolver el mismo valor");

    // Test rotacion 1
    uint128_t rotated_1 = rotr(value, 1);
    ASSERT_TEST(rotated_1 != value, "rotr por 1 debe cambiar el valor");

    // Test simetria: rotl(rotr(x, n), n) == x
    ASSERT_TEST(rotl(rotr(value, 5), 5) == value, "rotl debe deshacer rotr");

    TEST_PASS("test_rotr");
    return true;
}

bool test_reverse_bits()
{
    // Test con 0
    ASSERT_TEST(reverse_bits(uint128_t(0)) == uint128_t(0), "reverse_bits de 0 debe ser 0");

    // Test con 1
    uint128_t one(1);
    uint128_t reversed_one = reverse_bits(one);
    ASSERT_TEST(reversed_one == (uint128_t(1) << 127), "reverse_bits de 1 debe ser 2^127");

    // Test doble reversa
    uint128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    ASSERT_TEST(reverse_bits(reverse_bits(value)) == value,
                "doble reverse_bits debe devolver el original");

    TEST_PASS("test_reverse_bits");
    return true;
}

bool test_byteswap()
{
    // Test con 0
    ASSERT_TEST(byteswap(uint128_t(0)) == uint128_t(0), "byteswap de 0 debe ser 0");

    // Test doble swap
    uint128_t value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);
    ASSERT_TEST(byteswap(byteswap(value)) == value, "doble byteswap debe devolver el original");

    // Test con valor conocido
    uint128_t test_val(0x0102030405060708ULL, 0x090A0B0C0D0E0F10ULL);
    uint128_t swapped = byteswap(test_val);
    ASSERT_TEST(swapped != test_val, "byteswap debe cambiar el valor");

    TEST_PASS("test_byteswap");
    return true;
}

bool test_extract_bits()
{
    uint128_t value(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);

    // Test extraccion de 8 bits en posicion 0
    uint128_t extracted = extract_bits(value, 0, 8);
    ASSERT_TEST(extracted == uint128_t(0xFF), "extract_bits de 8 bits debe ser 0xFF");

    // Test extraccion de 16 bits en posicion 0
    extracted = extract_bits(value, 0, 16);
    ASSERT_TEST(extracted == uint128_t(0xFFFF), "extract_bits de 16 bits debe ser 0xFFFF");

    // Test extraccion con offset
    // Valor: 0x00FF00FF00FF00FF
    // bits 0-7: 0xFF, bits 8-15: 0x00, bits 16-23: 0xFF, bits 24-31: 0x00, ...
    uint128_t value2(0x0, 0x00FF00FF00FF00FFULL);
    extracted = extract_bits(value2, 8, 8);
    ASSERT_TEST(extracted == uint128_t(0x00), "extract_bits con offset debe funcionar");

    // Test extraccion de bits 0-7 (debe ser 0xFF)
    extracted = extract_bits(value2, 0, 8);
    ASSERT_TEST(extracted == uint128_t(0xFF), "extract_bits bits 0-7 debe ser 0xFF");

    // Test limites invalidos
    ASSERT_TEST(extract_bits(value, -1, 8) == uint128_t(0),
                "extract_bits con offset negativo debe ser 0");
    ASSERT_TEST(extract_bits(value, 0, 0) == uint128_t(0), "extract_bits con width 0 debe ser 0");

    TEST_PASS("test_extract_bits");
    return true;
}

bool test_insert_bits()
{
    uint128_t dest(0);
    uint128_t src(0xFF);

    // Test insercion en posicion 0
    uint128_t result = insert_bits(dest, src, 0, 8);
    ASSERT_TEST(result == uint128_t(0xFF), "insert_bits en pos 0 debe funcionar");

    // Test insercion en posicion 8
    result = insert_bits(dest, src, 8, 8);
    ASSERT_TEST(result == uint128_t(0xFF00), "insert_bits en pos 8 debe funcionar");

    // Test insercion sobre valor existente
    dest = uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    src = uint128_t(0);
    result = insert_bits(dest, src, 8, 8);
    ASSERT_TEST((result & uint128_t(0xFF00)) == uint128_t(0),
                "insert_bits debe limpiar bits existentes");

    TEST_PASS("test_insert_bits");
    return true;
}

bool test_parallel_deposit()
{
    uint128_t value(0xF);
    uint128_t mask(0xFF);

    uint128_t result = parallel_deposit(value, mask);
    ASSERT_TEST(result == uint128_t(0xF), "parallel_deposit basico debe funcionar");

    // Test con mascara alternada
    mask = uint128_t(0xAA); // 10101010
    value = uint128_t(0xF); // 1111
    result = parallel_deposit(value, mask);
    ASSERT_TEST(result == uint128_t(0xAA), "parallel_deposit con mascara alternada");

    TEST_PASS("test_parallel_deposit");
    return true;
}

bool test_parallel_extract()
{
    uint128_t value(0xFF);
    uint128_t mask(0xFF);

    uint128_t result = parallel_extract(value, mask);
    ASSERT_TEST(result == uint128_t(0xFF), "parallel_extract basico debe funcionar");

    // Test con mascara alternada
    value = uint128_t(0xAA); // 10101010
    mask = uint128_t(0xFF);
    result = parallel_extract(value, mask);
    ASSERT_TEST(result == uint128_t(0xAA), "parallel_extract con mascara completa");

    TEST_PASS("test_parallel_extract");
    return true;
}

bool test_get_bit()
{
    uint128_t value(0xFF);

    // Test bits dentro del rango
    ASSERT_TEST(get_bit(value, 0) == true, "get_bit pos 0 debe ser true");
    ASSERT_TEST(get_bit(value, 7) == true, "get_bit pos 7 debe ser true");
    ASSERT_TEST(get_bit(value, 8) == false, "get_bit pos 8 debe ser false");

    // Test bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(get_bit(high_bit, 127) == true, "get_bit pos 127 debe ser true");

    // Test limites invalidos
    ASSERT_TEST(get_bit(value, -1) == false, "get_bit pos negativa debe ser false");
    ASSERT_TEST(get_bit(value, 128) == false, "get_bit pos >= 128 debe ser false");

    TEST_PASS("test_get_bit");
    return true;
}

bool test_set_bit()
{
    uint128_t value(0);

    // Test set a true
    uint128_t result = set_bit(value, 0, true);
    ASSERT_TEST(result == uint128_t(1), "set_bit pos 0 a true debe ser 1");

    result = set_bit(value, 7, true);
    ASSERT_TEST(result == uint128_t(128), "set_bit pos 7 a true debe ser 128");

    // Test set a false
    value = uint128_t(0xFF);
    result = set_bit(value, 0, false);
    ASSERT_TEST(result == uint128_t(0xFE), "set_bit pos 0 a false debe limpiar bit");

    // Test limites invalidos
    result = set_bit(value, -1, true);
    ASSERT_TEST(result == value, "set_bit pos negativa debe dejar valor sin cambios");

    TEST_PASS("test_set_bit");
    return true;
}

bool test_flip_bit()
{
    uint128_t value(0);

    // Test flip de bit 0
    uint128_t result = flip_bit(value, 0);
    ASSERT_TEST(result == uint128_t(1), "flip_bit de bit 0 debe ser 1");

    // Test flip de bit ya establecido
    value = uint128_t(1);
    result = flip_bit(value, 0);
    ASSERT_TEST(result == uint128_t(0), "flip_bit de bit establecido debe limpiarlo");

    // Test limites invalidos
    result = flip_bit(value, -1);
    ASSERT_TEST(result == value, "flip_bit pos negativa debe dejar valor sin cambios");

    TEST_PASS("test_flip_bit");
    return true;
}

bool test_find_first_set()
{
    // Test con 0
    ASSERT_TEST(find_first_set(uint128_t(0)) == 0, "find_first_set de 0 debe ser 0");

    // Test con valores pequenos
    ASSERT_TEST(find_first_set(uint128_t(1)) == 1, "find_first_set de 1 debe ser 1");
    ASSERT_TEST(find_first_set(uint128_t(2)) == 2, "find_first_set de 2 debe ser 2");
    ASSERT_TEST(find_first_set(uint128_t(4)) == 3, "find_first_set de 4 debe ser 3");
    ASSERT_TEST(find_first_set(uint128_t(8)) == 4, "find_first_set de 8 debe ser 4");

    // Test con bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(find_first_set(high_bit) == 128, "find_first_set de bit 127 debe ser 128");

    TEST_PASS("test_find_first_set");
    return true;
}

bool test_find_last_set()
{
    // Test con 0
    ASSERT_TEST(find_last_set(uint128_t(0)) == 0, "find_last_set de 0 debe ser 0");

    // Test con valores pequenos
    ASSERT_TEST(find_last_set(uint128_t(1)) == 1, "find_last_set de 1 debe ser 1");
    ASSERT_TEST(find_last_set(uint128_t(2)) == 2, "find_last_set de 2 debe ser 2");
    ASSERT_TEST(find_last_set(uint128_t(3)) == 2, "find_last_set de 3 debe ser 2");
    ASSERT_TEST(find_last_set(uint128_t(4)) == 3, "find_last_set de 4 debe ser 3");

    // Test con bit alto
    uint128_t high_bit = uint128_t(1) << 127;
    ASSERT_TEST(find_last_set(high_bit) == 128, "find_last_set de bit 127 debe ser 128");

    TEST_PASS("test_find_last_set");
    return true;
}

// ===============================================================================
// MAIN
// ===============================================================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "  uint128_bits.hpp Tests\n";
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

    std::cout << "\n--- Tests uint128_bits namespace ---\n";

    all_passed &= test_rotl();
    all_passed &= test_rotr();
    all_passed &= test_reverse_bits();
    all_passed &= test_byteswap();
    all_passed &= test_extract_bits();
    all_passed &= test_insert_bits();
    all_passed &= test_parallel_deposit();
    all_passed &= test_parallel_extract();
    all_passed &= test_get_bit();
    all_passed &= test_set_bit();
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
