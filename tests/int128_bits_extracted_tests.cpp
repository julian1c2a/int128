/**
 * @file int128_bits_extracted_tests.cpp
 * @brief Tests completos de int128_base_bits.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_bits.hpp tests (40+ tests)
 * - test_bits_template.cpp (77 tests)
 *
 * Total: 77+ tests cubriendo:
 * - popcount, countl_zero, countr_zero, countl_one, countr_one
 * - bit_width, has_single_bit
 * - bit_floor, bit_ceil
 * - rotl, rotr
 * - reverse_bits, byteswap
 * - test_bit, set_bit, clear_bit, flip_bit
 * - extract_bits, insert_bits
 * - find_first_set, find_last_set
 * - to_bitset, from_bitset
 * - parallel_deposit, parallel_extract
 */

#include "int128_base_bits.hpp"
#include <bitset>
#include <cassert>
#include <iostream>

using namespace nstd;

static int tests_passed = 0;
static int tests_failed = 0;

void check(bool condition, const char *test_name)
{
    if (condition)
    {
        std::cout << "[OK]   " << test_name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "[FAIL] " << test_name << std::endl;
        tests_failed++;
    }
}

// =============================================================================
// SECTION: popcount
// =============================================================================

void test_popcount()
{
    // uint128_t
    check(popcount(uint128_t(0)) == 0, "popcount(uint128(0)) == 0");
    check(popcount(uint128_t(1)) == 1, "popcount(uint128(1)) == 1");

    uint128_t all_low(0, ~uint64_t(0));
    check(popcount(all_low) == 64, "popcount(all_low) == 64");

    uint128_t all_high(~uint64_t(0), 0);
    check(popcount(all_high) == 64, "popcount(all_high) == 64");

    uint128_t all_ones(~uint64_t(0), ~uint64_t(0));
    check(popcount(all_ones) == 128, "popcount(all_ones) == 128");

    // int128_t
    check(popcount(int128_t(0)) == 0, "popcount(int128(0)) == 0");
    check(popcount(int128_t(1)) == 1, "popcount(int128(1)) == 1");
    check(popcount(int128_t(-1)) == 128, "popcount(int128(-1)) == 128");
    check(popcount(int128_t(-2)) == 127, "popcount(int128(-2)) == 127");
}

// =============================================================================
// SECTION: countl_zero / countr_zero
// =============================================================================

void test_count_zero()
{
    // countl_zero uint128_t
    check(countl_zero(uint128_t(0)) == 128, "countl_zero(uint128(0)) == 128");
    check(countl_zero(uint128_t(1)) == 127, "countl_zero(uint128(1)) == 127");
    check(countr_zero(uint128_t(0)) == 128, "countr_zero(uint128(0)) == 128");
    check(countr_zero(uint128_t(1)) == 0, "countr_zero(uint128(1)) == 0");

    uint128_t high_bit(uint64_t(1) << 63, 0);
    check(countl_zero(high_bit) == 0, "countl_zero(MSB) == 0");
    check(countr_zero(high_bit) == 127, "countr_zero(MSB) == 127");

    uint128_t low64_msb(0, uint64_t(1) << 63);
    check(countl_zero(low64_msb) == 64, "countl_zero(bit63) == 64");
    check(countr_zero(low64_msb) == 63, "countr_zero(bit63) == 63");

    // int128_t
    check(countl_zero(int128_t(0)) == 128, "countl_zero(int128(0)) == 128");
    check(countl_zero(int128_t(1)) == 127, "countl_zero(int128(1)) == 127");
    check(countl_zero(int128_t(-1)) == 0, "countl_zero(int128(-1)) == 0");
    check(countl_zero(int128_t(-100)) == 0, "countl_zero(int128(-100)) == 0");

    // countr_zero con negativos
    check(countr_zero(int128_t(-2)) == 1, "countr_zero(int128(-2)) == 1");
    check(countr_zero(int128_t(-4)) == 2, "countr_zero(int128(-4)) == 2");
}

// =============================================================================
// SECTION: countl_one / countr_one
// =============================================================================

void test_count_one()
{
    // uint128_t
    check(countl_one(uint128_t(0)) == 0, "countl_one(uint128(0)) == 0");
    check(countr_one(uint128_t(0)) == 0, "countr_one(uint128(0)) == 0");

    uint128_t all_ones(~uint64_t(0), ~uint64_t(0));
    check(countl_one(all_ones) == 128, "countl_one(all_ones) == 128");
    check(countr_one(all_ones) == 128, "countr_one(all_ones) == 128");

    // int128_t
    check(countl_one(int128_t(-1)) == 128, "countl_one(int128(-1)) == 128");
    check(countr_one(int128_t(-1)) == 128, "countr_one(int128(-1)) == 128");
    check(countl_one(int128_t(-2)) == 127, "countl_one(int128(-2)) == 127");
    check(countr_one(int128_t(-2)) == 0, "countr_one(int128(-2)) == 0");
}

// =============================================================================
// SECTION: bit_width
// =============================================================================

void test_bit_width()
{
    check(bit_width(uint128_t(0)) == 0, "bit_width(uint128(0)) == 0");
    check(bit_width(uint128_t(1)) == 1, "bit_width(uint128(1)) == 1");
    check(bit_width(uint128_t(15)) == 4, "bit_width(uint128(15)) == 4");
    check(bit_width(uint128_t(16)) == 5, "bit_width(uint128(16)) == 5");

    check(bit_width(int128_t(0)) == 0, "bit_width(int128(0)) == 0");
    check(bit_width(int128_t(1)) == 1, "bit_width(int128(1)) == 1");
    check(bit_width(int128_t(-1)) == 128, "bit_width(int128(-1)) == 128");
}

// =============================================================================
// SECTION: has_single_bit
// =============================================================================

void test_has_single_bit()
{
    check(!has_single_bit(uint128_t(0)), "!has_single_bit(uint128(0))");
    check(has_single_bit(uint128_t(1)), "has_single_bit(uint128(1))");
    check(has_single_bit(uint128_t(2)), "has_single_bit(uint128(2))");
    check(!has_single_bit(uint128_t(3)), "!has_single_bit(uint128(3))");

    uint128_t high_bit(uint64_t(1) << 63, 0);
    check(has_single_bit(high_bit), "has_single_bit(2^127)");

    check(!has_single_bit(int128_t(-1)), "!has_single_bit(int128(-1))");
    check(!has_single_bit(int128_t(-16)), "!has_single_bit(int128(-16))");
}

// =============================================================================
// SECTION: bit_floor / bit_ceil
// =============================================================================

void test_bit_floor_ceil()
{
    check(bit_floor(uint128_t(0)) == uint128_t(0), "bit_floor(0) == 0");
    check(bit_ceil(uint128_t(0)) == uint128_t(1), "bit_ceil(0) == 1");

    check(bit_floor(uint128_t(1)) == uint128_t(1), "bit_floor(1) == 1");
    check(bit_ceil(uint128_t(1)) == uint128_t(1), "bit_ceil(1) == 1");

    check(bit_floor(uint128_t(5)) == uint128_t(4), "bit_floor(5) == 4");
    check(bit_ceil(uint128_t(5)) == uint128_t(8), "bit_ceil(5) == 8");

    check(bit_floor(uint128_t(8)) == uint128_t(8), "bit_floor(8) == 8");
    check(bit_ceil(uint128_t(8)) == uint128_t(8), "bit_ceil(8) == 8");

    // Negativos
    check(bit_floor(int128_t(-1)) == int128_t(0), "bit_floor(int128(-1)) == 0");
    check(bit_floor(int128_t(-100)) == int128_t(0), "bit_floor(int128(-100)) == 0");
}

// =============================================================================
// SECTION: rotl / rotr
// =============================================================================

void test_rotations()
{
    uint128_t val(1);
    check(rotl(val, 1) == uint128_t(2), "rotl(1, 1) == 2");
    check(rotl(val, 64) == uint128_t(uint64_t(1), 0), "rotl(1, 64) == 2^64");
    check(rotr(rotl(val, 7), 7) == val, "rotr(rotl(x, n), n) == x");

    uint128_t high(uint64_t(1) << 63, 0);
    check(rotl(high, 1) == uint128_t(1), "rotl(2^127, 1) == 1 (wrap)");
}

// =============================================================================
// SECTION: reverse_bits / byteswap
// =============================================================================

void test_reverse_byteswap()
{
    uint128_t val(1);
    auto rev = reverse_bits(val);
    check(test_bit(rev, 127), "reverse_bits(1) sets bit 127");
    check(!test_bit(rev, 0), "reverse_bits(1) clears bit 0");
    check(reverse_bits(rev) == val, "reverse_bits(reverse_bits(x)) == x");

    uint128_t bs_val(0x0102030405060708ULL, 0x090A0B0C0D0E0F10ULL);
    auto swapped = byteswap(bs_val);
    check(byteswap(swapped) == bs_val, "byteswap(byteswap(x)) == x");
}

// =============================================================================
// SECTION: test_bit / set_bit / clear_bit / flip_bit
// =============================================================================

void test_bit_manipulation()
{
    uint128_t zero(0);
    check(!test_bit(zero, 0), "!test_bit(0, 0)");
    check(!test_bit(zero, 64), "!test_bit(0, 64)");
    check(!test_bit(zero, 127), "!test_bit(0, 127)");

    uint128_t one(1);
    check(test_bit(one, 0), "test_bit(1, 0)");
    check(!test_bit(one, 1), "!test_bit(1, 1)");

    auto set_result = set_bit(zero, 5);
    check(test_bit(set_result, 5), "set_bit(0, 5) sets bit 5");
    check(set_result == uint128_t(32), "set_bit(0, 5) == 32");

    auto clear_result = clear_bit(one, 0);
    check(clear_result == uint128_t(0), "clear_bit(1, 0) == 0");

    auto flip_result = flip_bit(zero, 3);
    check(flip_result == uint128_t(8), "flip_bit(0, 3) == 8");
    check(flip_bit(flip_result, 3) == zero, "flip_bit(flip_bit(x, n), n) == x");

    // Bit alto (>= 64)
    auto high_set = set_bit(zero, 100);
    check(test_bit(high_set, 100), "set_bit(0, 100) sets bit 100");
}

// =============================================================================
// SECTION: extract_bits / insert_bits
// =============================================================================

void test_extract_insert_bits()
{
    uint128_t val(0xFF00FF00ULL);
    auto extracted = extract_bits(val, 8, 8);
    check(extracted == uint128_t(0xFF), "extract_bits(0xFF00FF00, 8, 8) == 0xFF");

    uint128_t zero(0);
    auto inserted = insert_bits(zero, uint128_t(0xAB), 16, 8);
    check(inserted == uint128_t(0xAB0000ULL), "insert_bits(0, 0xAB, 16, 8)");

    // Round trip
    uint128_t orig(0x123456789ABCDEF0ULL);
    auto ex = extract_bits(orig, 0, 64);
    check(ex == orig, "extract_bits(x, 0, 64) preserves value");
}

// =============================================================================
// SECTION: find_first_set / find_last_set
// =============================================================================

void test_find_set()
{
    check(find_first_set(uint128_t(0)) == -1, "find_first_set(0) == -1");
    check(find_last_set(uint128_t(0)) == -1, "find_last_set(0) == -1");

    check(find_first_set(uint128_t(1)) == 0, "find_first_set(1) == 0");
    check(find_last_set(uint128_t(1)) == 0, "find_last_set(1) == 0");

    uint128_t val(0b10100); // bits 2 y 4
    check(find_first_set(val) == 2, "find_first_set(0b10100) == 2");
    check(find_last_set(val) == 4, "find_last_set(0b10100) == 4");

    check(find_first_set(int128_t(-1)) == 0, "find_first_set(int128(-1)) == 0");
    check(find_last_set(int128_t(-1)) == 127, "find_last_set(int128(-1)) == 127");
}

// =============================================================================
// SECTION: to_bitset / from_bitset
// =============================================================================

void test_bitset_conversion()
{
    uint128_t val(0x12345678, 0x9ABCDEF0);
    auto bs = to_bitset(val);
    auto back = from_bitset<signedness::unsigned_type>(bs);
    check(back == val, "from_bitset(to_bitset(x)) == x");

    check(bs[4] == true, "bitset[4] correctamente establecido");

    int128_t neg(-42);
    auto bs_neg = to_bitset(neg);
    auto back_neg = from_bitset<signedness::signed_type>(bs_neg);
    check(back_neg == neg, "from_bitset(to_bitset(neg)) == neg");
}

// =============================================================================
// SECTION: parallel_deposit / parallel_extract
// =============================================================================

void test_parallel_ops()
{
    uint128_t val(0b1111);      // 4 bits
    uint128_t mask(0b10101010); // posiciones 1,3,5,7
    auto deposited = parallel_deposit(val, mask);
    check(deposited == uint128_t(0b10101010), "parallel_deposit simple");

    uint128_t src(0b10101010);
    auto extracted = parallel_extract(src, mask);
    check(extracted == uint128_t(0b1111), "parallel_extract simple");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_bits.hpp tests ===\n\n";

    std::cout << "--- popcount ---\n";
    test_popcount();

    std::cout << "\n--- countl_zero / countr_zero ---\n";
    test_count_zero();

    std::cout << "\n--- countl_one / countr_one ---\n";
    test_count_one();

    std::cout << "\n--- bit_width ---\n";
    test_bit_width();

    std::cout << "\n--- has_single_bit ---\n";
    test_has_single_bit();

    std::cout << "\n--- bit_floor / bit_ceil ---\n";
    test_bit_floor_ceil();

    std::cout << "\n--- rotl / rotr ---\n";
    test_rotations();

    std::cout << "\n--- reverse_bits / byteswap ---\n";
    test_reverse_byteswap();

    std::cout << "\n--- test/set/clear/flip_bit ---\n";
    test_bit_manipulation();

    std::cout << "\n--- extract_bits / insert_bits ---\n";
    test_extract_insert_bits();

    std::cout << "\n--- find_first_set / find_last_set ---\n";
    test_find_set();

    std::cout << "\n--- to_bitset / from_bitset ---\n";
    test_bitset_conversion();

    std::cout << "\n--- parallel_deposit / parallel_extract ---\n";
    test_parallel_ops();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
