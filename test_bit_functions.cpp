/**
 * @file test_bit_functions.cpp
 * @brief Tests para funciones de manipulacion de bits de int128_base_t
 *
 * Tests para:
 * - trailing_zeros(): Contar ceros desde el LSB
 * - leading_zeros(): Contar ceros desde el MSB
 * - bit_width(): Numero de bits significativos
 * - is_power_of_2(): Verificar si es potencia de 2
 */

#include "include_new/int128_base_tt.hpp"
#include <iomanip>
#include <iostream>

using namespace nstd;

// Contadores globales
int tests_passed = 0;
int tests_failed = 0;

void check(bool condition, const char* test_name)
{
    if (condition) {
        std::cout << "[OK] " << test_name << "\n";
        ++tests_passed;
    } else {
        std::cout << "[FAIL] " << test_name << " FAILED\n";
        ++tests_failed;
    }
}

// ============================================================================
// TESTS PARA trailing_zeros()
// ============================================================================

void test_trailing_zeros()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: trailing_zeros()                                    |\n";
    std::cout << "+============================================================+\n\n";

    // Casos basicos
    check(uint128_t(0ull).trailing_zeros() == 128, "tz(0) == 128");
    check(uint128_t(1ull).trailing_zeros() == 0, "tz(1) == 0");
    check(uint128_t(2ull).trailing_zeros() == 1, "tz(2) == 1");
    check(uint128_t(4ull).trailing_zeros() == 2, "tz(4) == 2");
    check(uint128_t(8ull).trailing_zeros() == 3, "tz(8) == 3");
    check(uint128_t(16ull).trailing_zeros() == 4, "tz(16) == 4");

    // Potencias de 2 mas grandes
    check(uint128_t(1ull << 10).trailing_zeros() == 10, "tz(2^10) == 10");
    check(uint128_t(1ull << 32).trailing_zeros() == 32, "tz(2^32) == 32");
    check(uint128_t(1ull << 63).trailing_zeros() == 63, "tz(2^63) == 63");

    // Valores en parte alta (> 64 bits)
    uint128_t high_bit(1ull, 0ull); // 2^64
    check(high_bit.trailing_zeros() == 64, "tz(2^64) == 64");

    uint128_t high_bit_70(1ull << 6, 0ull); // 2^70
    check(high_bit_70.trailing_zeros() == 70, "tz(2^70) == 70");

    // Numeros impares (trailing zeros = 0)
    check(uint128_t(3ull).trailing_zeros() == 0, "tz(3) == 0");
    check(uint128_t(15ull).trailing_zeros() == 0, "tz(15) == 0");
    check(uint128_t(999ull).trailing_zeros() == 0, "tz(999) == 0");

    // Numeros con trailing zeros especificos
    check(uint128_t(12ull).trailing_zeros() == 2, "tz(12) == 2 (12=0b1100)");
    check(uint128_t(24ull).trailing_zeros() == 3, "tz(24) == 3 (24=0b11000)");
    check(uint128_t(80ull).trailing_zeros() == 4, "tz(80) == 4 (80=0b1010000)");

    // UINT128_MAX
    check(uint128_t::max().trailing_zeros() == 0, "tz(UINT128_MAX) == 0");

    // int128_t (signed)
    check(int128_t(8ll).trailing_zeros() == 3, "tz(int128_t(8)) == 3");
    check(int128_t(1ll).trailing_zeros() == 0, "tz(int128_t(1)) == 0");
}

// ============================================================================
// TESTS PARA leading_zeros()
// ============================================================================

void test_leading_zeros()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: leading_zeros()                                     |\n";
    std::cout << "+============================================================+\n\n";

    // Casos basicos
    check(uint128_t(0ull).leading_zeros() == 128, "lz(0) == 128");
    check(uint128_t(1ull).leading_zeros() == 127, "lz(1) == 127");
    check(uint128_t(2ull).leading_zeros() == 126, "lz(2) == 126");
    check(uint128_t(4ull).leading_zeros() == 125, "lz(4) == 125");
    check(uint128_t(255ull).leading_zeros() == 120, "lz(255) == 120 (8 bits)");

    // Potencias de 2
    check(uint128_t(1ull << 10).leading_zeros() == 117, "lz(2^10) == 117");
    check(uint128_t(1ull << 32).leading_zeros() == 95, "lz(2^32) == 95");
    check(uint128_t(1ull << 63).leading_zeros() == 64, "lz(2^63) == 64");

    // Valores en parte alta
    uint128_t high_bit(1ull, 0ull); // 2^64
    check(high_bit.leading_zeros() == 63, "lz(2^64) == 63");

    uint128_t high_bit_127(1ull << 63, 0ull); // 2^127
    check(high_bit_127.leading_zeros() == 0, "lz(2^127) == 0");

    // UINT128_MAX
    check(uint128_t::max().leading_zeros() == 0, "lz(UINT128_MAX) == 0");

    // Valores especificos
    check(uint128_t(0x1000000000000000ull).leading_zeros() == 64 + 3, "lz(0x1000...) == 67");
}

// ============================================================================
// TESTS PARA bit_width()
// ============================================================================

void test_bit_width()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: bit_width()                                         |\n";
    std::cout << "+============================================================+\n\n";

    // Casos basicos
    check(uint128_t(0ull).bit_width() == 0, "bw(0) == 0");
    check(uint128_t(1ull).bit_width() == 1, "bw(1) == 1");
    check(uint128_t(2ull).bit_width() == 2, "bw(2) == 2");
    check(uint128_t(3ull).bit_width() == 2, "bw(3) == 2");
    check(uint128_t(4ull).bit_width() == 3, "bw(4) == 3");
    check(uint128_t(7ull).bit_width() == 3, "bw(7) == 3");
    check(uint128_t(8ull).bit_width() == 4, "bw(8) == 4");
    check(uint128_t(255ull).bit_width() == 8, "bw(255) == 8");
    check(uint128_t(256ull).bit_width() == 9, "bw(256) == 9");

    // Valores mas grandes
    check(uint128_t(1ull << 32).bit_width() == 33, "bw(2^32) == 33");
    check(uint128_t(1ull << 63).bit_width() == 64, "bw(2^63) == 64");

    // Valores en parte alta
    uint128_t high_bit(1ull, 0ull); // 2^64
    check(high_bit.bit_width() == 65, "bw(2^64) == 65");

    // UINT128_MAX
    check(uint128_t::max().bit_width() == 128, "bw(UINT128_MAX) == 128");
}

// ============================================================================
// TESTS PARA is_power_of_2()
// ============================================================================

void test_is_power_of_2()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: is_power_of_2()                                     |\n";
    std::cout << "+============================================================+\n\n";

    // Caso especial: 0 no es potencia de 2
    check(!uint128_t(0ull).is_power_of_2(), "0 NO es potencia de 2");

    // Potencias de 2
    check(uint128_t(1ull).is_power_of_2(), "1 = 2^0 es potencia de 2");
    check(uint128_t(2ull).is_power_of_2(), "2 = 2^1 es potencia de 2");
    check(uint128_t(4ull).is_power_of_2(), "4 = 2^2 es potencia de 2");
    check(uint128_t(8ull).is_power_of_2(), "8 = 2^3 es potencia de 2");
    check(uint128_t(16ull).is_power_of_2(), "16 = 2^4 es potencia de 2");
    check(uint128_t(1ull << 32).is_power_of_2(), "2^32 es potencia de 2");
    check(uint128_t(1ull << 63).is_power_of_2(), "2^63 es potencia de 2");

    // Potencias de 2 en parte alta
    uint128_t pow64(1ull, 0ull); // 2^64
    check(pow64.is_power_of_2(), "2^64 es potencia de 2");

    uint128_t pow100(1ull << 36, 0ull); // 2^100
    check(pow100.is_power_of_2(), "2^100 es potencia de 2");

    uint128_t pow127(1ull << 63, 0ull); // 2^127
    check(pow127.is_power_of_2(), "2^127 es potencia de 2");

    // No son potencias de 2
    check(!uint128_t(3ull).is_power_of_2(), "3 NO es potencia de 2");
    check(!uint128_t(5ull).is_power_of_2(), "5 NO es potencia de 2");
    check(!uint128_t(6ull).is_power_of_2(), "6 NO es potencia de 2");
    check(!uint128_t(7ull).is_power_of_2(), "7 NO es potencia de 2");
    check(!uint128_t(9ull).is_power_of_2(), "9 NO es potencia de 2");
    check(!uint128_t(10ull).is_power_of_2(), "10 NO es potencia de 2");
    check(!uint128_t(12ull).is_power_of_2(), "12 NO es potencia de 2");
    check(!uint128_t(15ull).is_power_of_2(), "15 NO es potencia de 2");
    check(!uint128_t(100ull).is_power_of_2(), "100 NO es potencia de 2");
    check(!uint128_t(1000ull).is_power_of_2(), "1000 NO es potencia de 2");

    // UINT128_MAX no es potencia de 2
    check(!uint128_t::max().is_power_of_2(), "UINT128_MAX NO es potencia de 2");

    // Numeros con multiples bits = no son potencia de 2
    uint128_t multi_bits(3ull, 0ull); // 2^64 + 2^65
    check(!multi_bits.is_power_of_2(), "3*2^64 NO es potencia de 2");
}

// ============================================================================
// TESTS DE COHERENCIA ENTRE FUNCIONES
// ============================================================================

void test_coherence()
{
    std::cout << "\n+============================================================+\n";
    std::cout << "|   TEST: Coherencia entre funciones                         |\n";
    std::cout << "+============================================================+\n\n";

    // Para potencias de 2: trailing_zeros() + leading_zeros() + 1 == 128
    for (int i = 0; i < 64; ++i) {
        uint128_t pow2(1ull << i);
        int tz = pow2.trailing_zeros();
        int lz = pow2.leading_zeros();
        int bw = pow2.bit_width();

        bool coherent = (tz == i) && (lz == 127 - i) && (bw == i + 1);
        check(coherent, ("Coherencia para 2^" + std::to_string(i)).c_str());
    }

    // Para potencias de 2 en parte alta
    for (int i = 0; i < 64; ++i) {
        uint128_t pow2(1ull << i, 0ull); // 2^(64+i)
        int tz = pow2.trailing_zeros();
        int lz = pow2.leading_zeros();
        int bw = pow2.bit_width();

        bool coherent = (tz == 64 + i) && (lz == 63 - i) && (bw == 65 + i);
        check(coherent, ("Coherencia para 2^" + std::to_string(64 + i)).c_str());
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "+============================================================+\n";
    std::cout << "|   TESTS DE FUNCIONES DE MANIPULACION DE BITS               |\n";
    std::cout << "+============================================================+\n";

    test_trailing_zeros();
    test_leading_zeros();
    test_bit_width();
    test_is_power_of_2();
    test_coherence();

    std::cout << "\n+============================================================+\n";
    std::cout << "|                        RESUMEN                             |\n";
    std::cout << "+============================================================+\n";

    std::cout << "[";
    if (tests_failed == 0) {
        std::cout << "OK";
    } else {
        std::cout << "FAIL";
    }
    std::cout << "] Tests pasados: " << tests_passed << "/" << (tests_passed + tests_failed)
              << "\n";

    return tests_failed > 0 ? 1 : 0;
}
