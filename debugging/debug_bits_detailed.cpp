#include "include/uint128_bits.hpp"
#include "include/uint128_t.hpp"
#include <bitset>
#include <iostream>

using namespace uint128_literals;

int main()
{
    uint128_t max_val = ~uint128_t(0);
    auto bitset_max = std::to_bitset(max_val);

    std::cout << "Bit-by-bit analysis:" << std::endl;

    // Check low 64 bits
    int low_count = 0;
    for (int i = 0; i < 64; i++) {
        if (bitset_max[i])
            low_count++;
    }

    // Check high 64 bits
    int high_count = 0;
    for (int i = 64; i < 128; i++) {
        if (bitset_max[i])
            high_count++;
    }

    std::cout << "Low 64 bits set: " << low_count << std::endl;
    std::cout << "High 64 bits set: " << high_count << std::endl;
    std::cout << "Total: " << (low_count + high_count) << std::endl;

    // Show first few bits that are NOT set
    std::cout << "\nFirst 20 bits:" << std::endl;
    for (int i = 0; i < 20; i++) {
        std::cout << "Bit " << i << ": " << (bitset_max[i] ? "1" : "0") << std::endl;
    }

    // Show last few bits
    std::cout << "\nLast 20 bits:" << std::endl;
    for (int i = 108; i < 128; i++) {
        std::cout << "Bit " << i << ": " << (bitset_max[i] ? "1" : "0") << std::endl;
    }

    // Debug the actual values being checked
    uint64_t low_part = max_val.low();
    uint64_t high_part = max_val.high();

    std::cout << "\nDirect bit checking:" << std::endl;
    std::cout << "Low part has " << __builtin_popcountll(low_part) << " bits set" << std::endl;
    std::cout << "High part has " << __builtin_popcountll(high_part) << " bits set" << std::endl;

    return 0;
}