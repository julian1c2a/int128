#include <bitset>
#include <int128_simple.hpp>
#include <iostream>
using namespace nstd;

using namespace uint128_literals;

int main()
{
    uint128_t max_val = ~uint128_t(0);
    auto bitset_max = nstd::to_bitset(max_val);

    std::cout << "Detailed bit analysis:" << std::endl;
    std::cout << "High part: 0x" << std::hex << max_val.high() << std::endl;
    std::cout << "Low part:  0x" << std::hex << max_val.low() << std::endl;

    // Count bits manually
    int manual_count = 0;
    for (int i = 0; i < 128; i++) {
        if (bitset_max[i]) {
            manual_count++;
        }
    }

    std::cout << "\nBit analysis:" << std::endl;
    std::cout << "Manual count: " << manual_count << std::endl;
    std::cout << "Bitset count(): " << bitset_max.count() << std::endl;
    std::cout << "Bitset size(): " << bitset_max.size() << std::endl;
    std::cout << "All bits set: " << bitset_max.all() << std::endl;

    // Check if high part is actually all 1s
    uint64_t expected_high = ~uint64_t(0);
    uint64_t expected_low = ~uint64_t(0);

    std::cout << "\nExpected values:" << std::endl;
    std::cout << "Expected high: 0x" << std::hex << expected_high << std::endl;
    std::cout << "Expected low:  0x" << std::hex << expected_low << std::endl;
    std::cout << "Actual high:   0x" << std::hex << max_val.high() << std::endl;
    std::cout << "Actual low:    0x" << std::hex << max_val.low() << std::endl;

    // Verify the conversion back
    auto back_converted = nstd::from_bitset(bitset_max);
    std::cout << "\nRound trip test:" << std::endl;
    std::cout << "Original: " << max_val.to_string() << std::endl;
    std::cout << "Back:     " << back_converted.to_string() << std::endl;
    std::cout << "Match:    " << (max_val == back_converted ? "✓" : "✗") << std::endl;

    return 0;
}
