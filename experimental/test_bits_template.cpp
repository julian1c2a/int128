/**
 * @file test_bits_template.cpp
 * @brief Test de operaciones de bits para int128_base_t<S>
 * @date 2026-01-07
 */

#include "../include_new/int128_base_bits.hpp"
#include <bitset>
#include <iostream>

using namespace nstd;

int passed = 0;
int failed = 0;

void test(bool condition, const char* name)
{
    std::cout << "[" << (condition ? "OK" : "FAIL") << "] " << name << "\n";
    condition ? ++passed : ++failed;
}

int main()
{
    std::cout << "=== Test bits para int128_base_t<S> ===\n\n";

    // ==========================================================================
    // POPCOUNT
    // ==========================================================================
    std::cout << "--- popcount ---\n";
    {
        uint128_t zero(0);
        test(popcount(zero) == 0, "popcount(0) == 0");

        uint128_t one(1);
        test(popcount(one) == 1, "popcount(1) == 1");

        uint128_t all_low(0, ~uint64_t(0));
        test(popcount(all_low) == 64, "popcount(all_low) == 64");

        uint128_t all_high(~uint64_t(0), 0);
        test(popcount(all_high) == 64, "popcount(all_high) == 64");

        uint128_t all_ones(~uint64_t(0), ~uint64_t(0));
        test(popcount(all_ones) == 128, "popcount(all_ones) == 128");

        int128_t neg_one(-1);
        test(popcount(neg_one) == 128, "popcount(int128_t(-1)) == 128");
    }

    // ==========================================================================
    // COUNTL_ZERO / COUNTR_ZERO
    // ==========================================================================
    std::cout << "\n--- countl_zero / countr_zero ---\n";
    {
        uint128_t zero(0);
        test(countl_zero(zero) == 128, "countl_zero(0) == 128");
        test(countr_zero(zero) == 128, "countr_zero(0) == 128");

        uint128_t one(1);
        test(countl_zero(one) == 127, "countl_zero(1) == 127");
        test(countr_zero(one) == 0, "countr_zero(1) == 0");

        uint128_t high_bit(uint64_t(1) << 63, 0);
        test(countl_zero(high_bit) == 0, "countl_zero(MSB) == 0");
        test(countr_zero(high_bit) == 127, "countr_zero(MSB) == 127");

        uint128_t low64_msb(0, uint64_t(1) << 63);
        test(countl_zero(low64_msb) == 64, "countl_zero(bit63) == 64");
        test(countr_zero(low64_msb) == 63, "countr_zero(bit63) == 63");

        int128_t neg(-1);
        test(countl_zero(neg) == 0, "countl_zero(int128_t(-1)) == 0 (signed)");
    }

    // ==========================================================================
    // COUNTL_ONE / COUNTR_ONE
    // ==========================================================================
    std::cout << "\n--- countl_one / countr_one ---\n";
    {
        uint128_t zero(0);
        test(countl_one(zero) == 0, "countl_one(0) == 0");
        test(countr_one(zero) == 0, "countr_one(0) == 0");

        uint128_t all_ones(~uint64_t(0), ~uint64_t(0));
        test(countl_one(all_ones) == 128, "countl_one(all_ones) == 128");
        test(countr_one(all_ones) == 128, "countr_one(all_ones) == 128");

        int128_t neg(-1);
        test(countl_one(neg) == 128, "countl_one(int128_t(-1)) == 128");
        test(countr_one(neg) == 128, "countr_one(int128_t(-1)) == 128");

        int128_t neg_two(-2); // ...1110
        test(countr_one(neg_two) == 0, "countr_one(int128_t(-2)) == 0");
    }

    // ==========================================================================
    // BIT_WIDTH
    // ==========================================================================
    std::cout << "\n--- bit_width ---\n";
    {
        uint128_t zero(0);
        test(bit_width(zero) == 0, "bit_width(0) == 0");

        uint128_t one(1);
        test(bit_width(one) == 1, "bit_width(1) == 1");

        uint128_t fifteen(15);
        test(bit_width(fifteen) == 4, "bit_width(15) == 4");

        uint128_t sixteen(16);
        test(bit_width(sixteen) == 5, "bit_width(16) == 5");

        int128_t neg(-1);
        test(bit_width(neg) == 128, "bit_width(int128_t(-1)) == 128 (signed)");
    }

    // ==========================================================================
    // HAS_SINGLE_BIT
    // ==========================================================================
    std::cout << "\n--- has_single_bit ---\n";
    {
        uint128_t zero(0);
        test(!has_single_bit(zero), "!has_single_bit(0)");

        uint128_t one(1);
        test(has_single_bit(one), "has_single_bit(1)");

        uint128_t two(2);
        test(has_single_bit(two), "has_single_bit(2)");

        uint128_t three(3);
        test(!has_single_bit(three), "!has_single_bit(3)");

        uint128_t high_bit(uint64_t(1) << 63, 0);
        test(has_single_bit(high_bit), "has_single_bit(2^127)");

        int128_t neg(-1);
        test(!has_single_bit(neg), "!has_single_bit(int128_t(-1))");

        int128_t neg_pot2(-16);
        test(!has_single_bit(neg_pot2), "!has_single_bit(int128_t(-16)) (negativo)");
    }

    // ==========================================================================
    // BIT_FLOOR / BIT_CEIL
    // ==========================================================================
    std::cout << "\n--- bit_floor / bit_ceil ---\n";
    {
        uint128_t zero(0);
        test(bit_floor(zero) == uint128_t(0), "bit_floor(0) == 0");
        test(bit_ceil(zero) == uint128_t(1), "bit_ceil(0) == 1");

        uint128_t one(1);
        test(bit_floor(one) == uint128_t(1), "bit_floor(1) == 1");
        test(bit_ceil(one) == uint128_t(1), "bit_ceil(1) == 1");

        uint128_t five(5);
        test(bit_floor(five) == uint128_t(4), "bit_floor(5) == 4");
        test(bit_ceil(five) == uint128_t(8), "bit_ceil(5) == 8");

        uint128_t eight(8);
        test(bit_floor(eight) == uint128_t(8), "bit_floor(8) == 8");
        test(bit_ceil(eight) == uint128_t(8), "bit_ceil(8) == 8");
    }

    // ==========================================================================
    // ROTL / ROTR
    // ==========================================================================
    std::cout << "\n--- rotl / rotr ---\n";
    {
        uint128_t val(1);
        test(rotl(val, 1) == uint128_t(2), "rotl(1, 1) == 2");
        test(rotl(val, 64) == uint128_t(1, 0), "rotl(1, 64) == 2^64");
        test(rotr(rotl(val, 7), 7) == val, "rotr(rotl(x, n), n) == x");

        uint128_t high(uint64_t(1) << 63, 0);
        test(rotl(high, 1) == uint128_t(1), "rotl(2^127, 1) == 1 (wrap)");
    }

    // ==========================================================================
    // REVERSE_BITS / BYTESWAP
    // ==========================================================================
    std::cout << "\n--- reverse_bits / byteswap ---\n";
    {
        uint128_t val(1);
        auto rev = reverse_bits(val);
        test(test_bit(rev, 127), "reverse_bits(1) sets bit 127");
        test(!test_bit(rev, 0), "reverse_bits(1) clears bit 0");

        uint128_t rev_rev = reverse_bits(rev);
        test(rev_rev == val, "reverse_bits(reverse_bits(x)) == x");

        uint128_t bs_val(0x0102030405060708ULL, 0x090A0B0C0D0E0F10ULL);
        auto swapped = byteswap(bs_val);
        auto swapped_again = byteswap(swapped);
        test(swapped_again == bs_val, "byteswap(byteswap(x)) == x");
    }

    // ==========================================================================
    // TEST_BIT / SET_BIT / CLEAR_BIT / FLIP_BIT
    // ==========================================================================
    std::cout << "\n--- test/set/clear/flip_bit ---\n";
    {
        uint128_t zero(0);
        test(!test_bit(zero, 0), "!test_bit(0, 0)");
        test(!test_bit(zero, 64), "!test_bit(0, 64)");
        test(!test_bit(zero, 127), "!test_bit(0, 127)");

        uint128_t one(1);
        test(test_bit(one, 0), "test_bit(1, 0)");
        test(!test_bit(one, 1), "!test_bit(1, 1)");

        auto set_result = set_bit(zero, 5);
        test(test_bit(set_result, 5), "set_bit(0, 5) sets bit 5");
        test(set_result == uint128_t(32), "set_bit(0, 5) == 32");

        auto clear_result = clear_bit(one, 0);
        test(clear_result == uint128_t(0), "clear_bit(1, 0) == 0");

        auto flip_result = flip_bit(zero, 3);
        test(flip_result == uint128_t(8), "flip_bit(0, 3) == 8");
        auto flip_back = flip_bit(flip_result, 3);
        test(flip_back == zero, "flip_bit(flip_bit(x, n), n) == x");

        // Bit alto (>= 64)
        auto high_set = set_bit(zero, 100);
        test(test_bit(high_set, 100), "set_bit(0, 100) sets bit 100");
    }

    // ==========================================================================
    // EXTRACT_BITS / INSERT_BITS
    // ==========================================================================
    std::cout << "\n--- extract_bits / insert_bits ---\n";
    {
        uint128_t val(0xFF00FF00ULL);
        auto extracted = extract_bits(val, 8, 8);
        test(extracted == uint128_t(0xFF), "extract_bits(0xFF00FF00, 8, 8) == 0xFF");

        uint128_t zero(0);
        auto inserted = insert_bits(zero, uint128_t(0xAB), 16, 8);
        test(inserted == uint128_t(0xAB0000ULL), "insert_bits(0, 0xAB, 16, 8)");

        // Round trip
        uint128_t orig(0x123456789ABCDEF0ULL);
        auto ex = extract_bits(orig, 0, 64);
        test(ex == orig, "extract_bits(x, 0, 64) preserves value");
    }

    // ==========================================================================
    // FIND_FIRST_SET / FIND_LAST_SET
    // ==========================================================================
    std::cout << "\n--- find_first_set / find_last_set ---\n";
    {
        uint128_t zero(0);
        test(find_first_set(zero) == -1, "find_first_set(0) == -1");
        test(find_last_set(zero) == -1, "find_last_set(0) == -1");

        uint128_t one(1);
        test(find_first_set(one) == 0, "find_first_set(1) == 0");
        test(find_last_set(one) == 0, "find_last_set(1) == 0");

        uint128_t val(0b10100); // bits 2 y 4
        test(find_first_set(val) == 2, "find_first_set(0b10100) == 2");
        test(find_last_set(val) == 4, "find_last_set(0b10100) == 4");

        int128_t neg(-1);
        test(find_first_set(neg) == 0, "find_first_set(int128(-1)) == 0");
        test(find_last_set(neg) == 127, "find_last_set(int128(-1)) == 127");
    }

    // ==========================================================================
    // TO_BITSET / FROM_BITSET
    // ==========================================================================
    std::cout << "\n--- to_bitset / from_bitset ---\n";
    {
        uint128_t val(0x12345678, 0x9ABCDEF0);
        auto bs = to_bitset(val);
        auto back = from_bitset<signedness::unsigned_type>(bs);
        test(back == val, "from_bitset(to_bitset(x)) == x");

        // Verificar algunos bits específicos
        test(bs[4] == true, "bitset[4] correctamente establecido");

        int128_t neg(-42);
        auto bs_neg = to_bitset(neg);
        auto back_neg = from_bitset<signedness::signed_type>(bs_neg);
        test(back_neg == neg, "from_bitset(to_bitset(neg)) == neg");
    }

    // ==========================================================================
    // PARALLEL_DEPOSIT / PARALLEL_EXTRACT
    // ==========================================================================
    std::cout << "\n--- parallel_deposit / parallel_extract ---\n";
    {
        uint128_t val(0b1111);      // 4 bits
        uint128_t mask(0b10101010); // posiciones 1,3,5,7
        auto deposited = parallel_deposit(val, mask);
        // bits 0,1,2,3 de val → posiciones 1,3,5,7
        test(deposited == uint128_t(0b10101010), "parallel_deposit simple");

        uint128_t src(0b10101010);
        auto extracted = parallel_extract(src, mask);
        test(extracted == uint128_t(0b1111), "parallel_extract simple");
    }

    // ==========================================================================
    // RESULTADO FINAL
    // ==========================================================================
    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
