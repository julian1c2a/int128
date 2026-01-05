#include "int128_base.hpp"
#include <cassert>
#include <iostream>
#include <string>

using namespace nstd;

#define TEST(name, expr, expected)                                                                 \
    do {                                                                                           \
        std::cout << "Test " << name << ": ";                                                      \
        auto result = (expr);                                                                      \
        if (result == (expected)) {                                                                \
            std::cout << "\u2713 PASS (\"" << result << "\")\n";                                   \
        } else {                                                                                   \
            std::cout << "\u2717 FAIL\n";                                                          \
            std::cout << "  Expected: \"" << (expected) << "\"\n";                                 \
            std::cout << "  Got:      \"" << result << "\"\n";                                     \
        }                                                                                          \
    } while (0)

int main()
{
    std::cout << "=== Edge Case Tests for to_string() ===\n\n";

    // ============================================================================
    // Test 1: Extremos de int128_t
    // ============================================================================
    std::cout << "--- Test 1: int128_t Extremos ---\n";

    // INT128_MIN = -2^127
    int128_t int_min = INT128_MIN;
    TEST("INT128_MIN", int_min.to_string(), "-170141183460469231731687303715884105728");

    // INT128_MAX = 2^127 - 1
    int128_t int_max = INT128_MAX;
    TEST("INT128_MAX", int_max.to_string(), "170141183460469231731687303715884105727");

    // Cerca de INT128_MIN
    int128_t int_min_plus1 = INT128_MIN + int128_t(1);
    TEST("INT128_MIN + 1", int_min_plus1.to_string(), "-170141183460469231731687303715884105727");

    // Cerca de INT128_MAX
    int128_t int_max_minus1 = INT128_MAX - int128_t(1);
    TEST("INT128_MAX - 1", int_max_minus1.to_string(), "170141183460469231731687303715884105726");

    std::cout << "\n";

    // ============================================================================
    // Test 2: Extremos de uint128_t
    // ============================================================================
    std::cout << "--- Test 2: uint128_t Extremos ---\n";

    // UINT128_MAX = 2^128 - 1
    uint128_t uint_max = UINT128_MAX;
    TEST("UINT128_MAX", uint_max.to_string(), "340282366920938463463374607431768211455");

    // Cerca de UINT128_MAX
    uint128_t uint_max_minus1 = UINT128_MAX - uint128_t(1);
    TEST("UINT128_MAX - 1", uint_max_minus1.to_string(), "340282366920938463463374607431768211454");

    // Segundo valor más grande
    uint128_t uint_max_minus2 = UINT128_MAX - uint128_t(2);
    TEST("UINT128_MAX - 2", uint_max_minus2.to_string(), "340282366920938463463374607431768211453");

    std::cout << "\n";

    // ============================================================================
    // Test 3: Valores cerca de cero (signed)
    // ============================================================================
    std::cout << "--- Test 3: Valores cerca de cero (signed) ---\n";

    TEST("int128_t(0)", int128_t(0).to_string(), "0");
    TEST("int128_t(1)", int128_t(1).to_string(), "1");
    TEST("int128_t(-1)", int128_t(-1).to_string(), "-1");
    TEST("int128_t(2)", int128_t(2).to_string(), "2");
    TEST("int128_t(-2)", int128_t(-2).to_string(), "-2");
    TEST("int128_t(10)", int128_t(10).to_string(), "10");
    TEST("int128_t(-10)", int128_t(-10).to_string(), "-10");

    std::cout << "\n";

    // ============================================================================
    // Test 4: Potencias de 2
    // ============================================================================
    std::cout << "--- Test 4: Potencias de 2 ---\n";

    // 2^64
    uint128_t pow2_64(1, 0); // (high=1, low=0) = 2^64
    TEST("2^64", pow2_64.to_string(), "18446744073709551616");

    // 2^96 = 2^32 * 2^64
    uint128_t pow2_96(4294967296ULL, 0); // (high=2^32, low=0)
    TEST("2^96", pow2_96.to_string(), "79228162514264337593543950336");

    // 2^120 = 2^56 * 2^64
    uint128_t pow2_120(72057594037927936ULL, 0); // (high=2^56, low=0)
    TEST("2^120", pow2_120.to_string(), "1329227995784915872903807060280344576");

    // 2^127 (INT128_MAX + 1 como unsigned)
    uint128_t pow2_127 = uint128_t(INT128_MAX) + uint128_t(1);
    TEST("2^127", pow2_127.to_string(), "170141183460469231731687303715884105728");

    std::cout << "\n";

    // ============================================================================
    // Test 5: to_string() con múltiples bases
    // ============================================================================
    std::cout << "--- Test 5: Conversión a diferentes bases ---\n";

    uint128_t val(255); // 0xFF
    TEST("255 base 2", val.to_string(2), "11111111");
    TEST("255 base 8", val.to_string(8), "377");
    TEST("255 base 10", val.to_string(10), "255");
    TEST("255 base 16", val.to_string(16), "ff");
    TEST("255 base 36", val.to_string(36), "73");

    std::cout << "\n";

    // ============================================================================
    // Test 6: Valores grandes con diferentes bases
    // ============================================================================
    std::cout << "--- Test 6: Valores grandes en diferentes bases ---\n";

    uint128_t big(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // UINT128_MAX

    // Base 2 (129 dígitos: 128 unos)
    auto bin_result = big.to_string(2);
    std::cout << "Test UINT128_MAX base 2: ";
    if (bin_result.length() == 128 && bin_result.find('0') == std::string::npos) {
        std::cout << "\u2713 PASS (128 unos)\n";
    } else {
        std::cout << "\u2717 FAIL (expected 128 '1' characters)\n";
    }

    // Base 16 (32 dígitos 'f')
    auto hex_result = big.to_string(16);
    TEST("UINT128_MAX base 16", hex_result, "ffffffffffffffffffffffffffffffff");

    std::cout << "\n";

    // ============================================================================
    // Test 7: Negación y to_string() encadenados
    // ============================================================================
    std::cout << "--- Test 7: Operaciones negación + to_string() ---\n";

    int128_t pos(42);
    int128_t neg = -pos;
    int128_t double_neg = -neg;

    TEST("pos(42)", pos.to_string(), "42");
    TEST("-pos", neg.to_string(), "-42");
    TEST("-(-pos)", double_neg.to_string(), "42");

    // Triple negación
    int128_t triple_neg = -double_neg;
    TEST("-(-(-pos))", triple_neg.to_string(), "-42");

    std::cout << "\n";

    // ============================================================================
    // Test 8: abs() + to_string()
    // ============================================================================
    std::cout << "--- Test 8: abs() + to_string() ---\n";

    int128_t neg_big(-12345678901234567890);
    int128_t abs_big = neg_big.abs();

    TEST("neg_big", neg_big.to_string(), "-12345678901234567890");
    TEST("abs(neg_big)", abs_big.to_string(), "12345678901234567890");
    TEST("abs(INT128_MIN)", INT128_MIN.abs().to_string(),
         "-170141183460469231731687303715884105728"); // abs(INT128_MIN) overflow!

    std::cout << "\n";

    // ============================================================================
    // Test 9: Parsing + to_string() (round-trip)
    // ============================================================================
    std::cout << "--- Test 9: Parse + to_string() (round-trip) ---\n";

    auto test_roundtrip = [](const char* str_in) {
        auto [val, err] = uint128_t::parse(str_in);
        std::string str_out = val.to_string();
        std::cout << "Roundtrip \"" << str_in << "\": ";
        if (err == 0 && str_out == str_in) {
            std::cout << "\u2713 PASS\n";
        } else {
            std::cout << "\u2717 FAIL (got \"" << str_out << "\", error=" << err << ")\n";
        }
    };

    test_roundtrip("0");
    test_roundtrip("1");
    test_roundtrip("12345");
    test_roundtrip("18446744073709551616");                    // 2^64
    test_roundtrip("340282366920938463463374607431768211455"); // UINT128_MAX

    std::cout << "\n";

    // ============================================================================
    // Test 10: Signed parsing + to_string() (round-trip)
    // ============================================================================
    std::cout << "--- Test 10: Signed parse + to_string() (round-trip) ---\n";

    auto test_signed_roundtrip = [](const char* str_in) {
        auto [val, err] = int128_t::parse(str_in);
        std::string str_out = val.to_string();
        std::cout << "Roundtrip \"" << str_in << "\": ";
        if (err == 0 && str_out == str_in) {
            std::cout << "\u2713 PASS\n";
        } else {
            std::cout << "\u2717 FAIL (got \"" << str_out << "\", error=" << err << ")\n";
        }
    };

    test_signed_roundtrip("0");
    test_signed_roundtrip("1");
    test_signed_roundtrip("-1");
    test_signed_roundtrip("42");
    test_signed_roundtrip("-42");
    test_signed_roundtrip("170141183460469231731687303715884105727");  // INT128_MAX
    test_signed_roundtrip("-170141183460469231731687303715884105728"); // INT128_MIN

    std::cout << "\n";
    std::cout << "=== Todos los tests de edge cases completados ===\n";

    return 0;
}
