#include "include/uint128_t.hpp"
#include <cstdint>
#include <iostream>

using namespace uint128_literals;

int main()
{
    std::cout << "Investigating uint128_t maximum value:" << std::endl;

    uint128_t test = ~uint128_t(0);
    std::cout << "~uint128_t(0) = " << test << std::endl;
    std::cout << "High: 0x" << std::hex << test.high() << std::endl;
    std::cout << "Low:  0x" << std::hex << test.low() << std::endl;

    // Check what high() and low() return for different values
    uint128_t explicit_max = uint128_t(~uint64_t(0), ~uint64_t(0));
    std::cout << "\nExplicit construction uint128_t(~uint64_t(0), ~uint64_t(0)):" << std::endl;
    std::cout << "Value: " << explicit_max << std::endl;
    std::cout << "High: 0x" << std::hex << explicit_max.high() << std::endl;
    std::cout << "Low:  0x" << std::hex << explicit_max.low() << std::endl;

    std::cout << "\nAre they equal? " << (test == explicit_max ? "Yes" : "No") << std::endl;

    // Check individual uint64_t max
    uint64_t u64_max = ~uint64_t(0);
    std::cout << "\n~uint64_t(0) = 0x" << std::hex << u64_max << std::dec;
    std::cout << " (decimal: " << u64_max << ")" << std::endl;

    // Count bits in uint64_t max
    int bits_in_u64 = __builtin_popcountll(u64_max);
    std::cout << "Bits in uint64_t max: " << bits_in_u64 << std::endl;

    return 0;
}