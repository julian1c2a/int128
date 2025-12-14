#include "include/uint128_bits.hpp"
#include "include/uint128_t.hpp"
#include <bitset>
#include <iostream>

using namespace uint128_literals;

int main()
{
    std::cout << "🔍 DEBUGGING BITSET CONVERSION\n" << std::endl;

    uint128_t max_val = ~uint128_t(0);
    std::cout << "Max value: " << std::hex << max_val << std::endl;
    std::cout << "High part: 0x" << std::hex << max_val.high() << std::endl;
    std::cout << "Low part:  0x" << std::hex << max_val.low() << std::endl;

    auto bitset_max = std::to_bitset(max_val);
    std::cout << "Bitset count: " << bitset_max.count() << std::endl;
    std::cout << "Bitset all(): " << bitset_max.all() << std::endl;

    // Check specific bits in the high range
    std::cout << "High bits:" << std::endl;
    for (int i = 120; i < 128; i++) {
        std::cout << "Bit " << i << ": " << (bitset_max[i] ? "1" : "0") << std::endl;
    }

    // Manually check what ~uint128_t(0) gives us
    uint128_t zero = uint128_t(0);
    uint128_t manual_max = ~zero;
    std::cout << "\nManual max creation:" << std::endl;
    std::cout << "Zero: " << std::hex << zero << std::endl;
    std::cout << "~Zero: " << std::hex << manual_max << std::endl;
    std::cout << "High part: 0x" << std::hex << manual_max.high() << std::endl;
    std::cout << "Low part:  0x" << std::hex << manual_max.low() << std::endl;

    return 0;
}