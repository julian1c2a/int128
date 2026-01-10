// =============================================================================
// Demo: Practical UDL (User-Defined Literals) Usage
// Part of int128 Library - https://github.com/[repo]
// License: BSL-1.0
// =============================================================================
//
// Demonstrates practical use of uint128_t/int128_t with the available
// literal operators and formatting methods from the library API.
//
// Available literals: _u128, _U128, _i128, _I128
// Available methods: to_string(), to_string(base)
// =============================================================================

#include <int128_simple.hpp>
#include <int128_base_format.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace nstd;
using namespace nstd::int128_literals;
using namespace nstd::int128_format;

void demo_crypto_like_operations()
{
    std::cout << "=== DEMO: Crypto-like Operations ===" << std::endl;

    // Simulating keys and hash values using UDL literals
    // Note: _u128 supports 0x prefix for hex values
    const uint128_t private_key{0xDEADBEEFCAFEBABEull, 0x123456789ABCDEFull};
    const uint128_t public_key{0xFEDCBA9876543210ull, 0xABCDEF0123456789ull};
    const auto message_hash = 0x1A2B3C4D5E6F7890_u128;

    std::cout << "Private key: " << hex(private_key) << std::endl;
    std::cout << "Public key:  " << hex(public_key) << std::endl;
    std::cout << "Message hash: " << hex(message_hash) << std::endl;

    // XOR operation for simple "encryption"
    const auto encrypted = message_hash ^ private_key;
    std::cout << "Encrypted message: " << hex(encrypted) << std::endl;

    // Decryption
    const auto decrypted = encrypted ^ private_key;
    std::cout << "Decrypted message: " << hex(decrypted) << std::endl;
    std::cout << "Decryption correct: " << (decrypted == message_hash ? "[OK]" : "[FAIL]") << std::endl;
}

void demo_memory_addressing()
{
    std::cout << "\n=== DEMO: Memory Addressing ===" << std::endl;

    // Simulating 128-bit memory addresses
    const auto base_addr = 0x7FFF800000000000_u128;
    const auto page_size = 4096_u128;

    std::cout << "Base address: " << hex(base_addr) << std::endl;
    std::cout << "Page size: " << page_size.to_string() << " bytes" << std::endl;

    // Calculate page addresses
    std::vector<uint128_t> page_addresses{};
    for (int i = 0; i < 5; ++i)
    {
        const auto page_addr = base_addr + (page_size * uint128_t{static_cast<uint64_t>(i)});
        page_addresses.push_back(page_addr);
        std::cout << "Page " << i << ": " << hex(page_addr) << std::endl;
    }

    // Check alignment
    for (size_t i = 0; i < page_addresses.size(); ++i)
    {
        const auto aligned = (page_addresses[i] % page_size) == uint128_t{0};
        std::cout << "Page " << i << " aligned: " << (aligned ? "[OK]" : "[FAIL]") << std::endl;
    }
}

void demo_bit_manipulation()
{
    std::cout << "\n=== DEMO: Bit Manipulation ===" << std::endl;

    // Create bitmasks using hex literals
    const auto bit_mask = 0xF0F0F0F0_u128;
    const auto data = 0x12345678_u128;

    std::cout << "Original data: " << hex(data) << std::endl;
    std::cout << "Bit mask:      " << hex(bit_mask) << std::endl;
    std::cout << "Mask binary:   " << bin(bit_mask) << std::endl;

    // Apply mask
    const auto masked_data = data & bit_mask;
    std::cout << "Masked data: " << hex(masked_data) << std::endl;

    // Invert specific bits
    const auto inverted = data ^ bit_mask;
    std::cout << "Inverted bits: " << hex(inverted) << std::endl;

    // Set specific bits
    const auto set_bits = data | bit_mask;
    std::cout << "Set bits: " << hex(set_bits) << std::endl;
}

void demo_number_systems()
{
    std::cout << "\n=== DEMO: Number Systems ===" << std::endl;

    // Same number in different formats
    const auto decimal_val = 1234567890_u128;
    const auto hex_val = 0x499602D2_u128; // Same value in hex

    std::cout << "Number: 1234567890 in different bases:" << std::endl;
    std::cout << "Decimal: " << decimal_val.to_string() << std::endl;
    std::cout << "Hexadecimal: " << hex(hex_val) << std::endl;
    std::cout << "Octal: " << oct(decimal_val) << std::endl;
    std::cout << "Binary: " << bin(decimal_val) << std::endl;

    // Verify they represent the same value
    std::cout << "\nVerifications:" << std::endl;
    std::cout << "decimal == hex: " << (decimal_val == hex_val ? "[OK]" : "[FAIL]") << std::endl;
}

void demo_large_numbers()
{
    std::cout << "\n=== DEMO: Extremely Large Numbers ===" << std::endl;

    // Using constructor for max values
    const uint128_t huge_number{0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull};
    const uint128_t almost_max{0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull};

    std::cout << "Huge number: " << huge_number.to_string() << std::endl;
    std::cout << "Almost max:  " << almost_max.to_string() << std::endl;

    // Operations with large numbers
    const auto difference = huge_number - almost_max;
    std::cout << "Difference: " << difference.to_string() << std::endl;

    // Sum that would overflow native types
    const uint128_t big1{0xFFFFFFFFFFFFFFFFull, 0x0000000000000000ull};
    const uint128_t big2{0x0000000000000000ull, 0xFFFFFFFFFFFFFFFFull};
    const auto big_sum = big1 + big2;

    std::cout << "Large sum:" << std::endl;
    std::cout << "  " << hex(big1) << std::endl;
    std::cout << "+ " << hex(big2) << std::endl;
    std::cout << "= " << hex(big_sum) << std::endl;
    std::cout << "Decimal: " << big_sum.to_string() << std::endl;
}

void demo_signed_literals()
{
    std::cout << "\n=== DEMO: Signed Literals ===" << std::endl;

    // Using signed literals
    const auto positive = 42_i128;
    const auto negative = -42_i128;
    const auto large_positive = 170141183460469231731687303715884105727_i128; // INT128_MAX

    std::cout << "Positive: " << positive.to_string() << std::endl;
    std::cout << "Negative: " << negative.to_string() << std::endl;
    std::cout << "Large positive: " << large_positive.to_string() << std::endl;

    // Arithmetic with signed values
    const auto sum = positive + negative;
    std::cout << "42 + (-42) = " << sum.to_string() << std::endl;

    const auto product = positive * negative;
    std::cout << "42 * (-42) = " << product.to_string() << std::endl;
}

int main()
{
    std::cout << "+======================================================+" << std::endl;
    std::cout << "|  PRACTICAL DEMO: UDL LITERALS AND FORMATS            |" << std::endl;
    std::cout << "+======================================================+" << std::endl;

    demo_crypto_like_operations();
    demo_memory_addressing();
    demo_bit_manipulation();
    demo_number_systems();
    demo_large_numbers();
    demo_signed_literals();

    std::cout << "\n+======================================================+" << std::endl;
    std::cout << "|  DEMO COMPLETED SUCCESSFULLY                         |" << std::endl;
    std::cout << "+======================================================+" << std::endl;

    std::cout << "\nBENEFITS DEMONSTRATED:" << std::endl;
    std::cout << "   [OK] UDL literals make code more readable and expressive" << std::endl;
    std::cout << "   [OK] Format functions simplify output formatting" << std::endl;
    std::cout << "   [OK] Multi-base support facilitates different systems" << std::endl;
    std::cout << "   [OK] uint128_t integrates naturally in modern C++ code" << std::endl;
    std::cout << "\nuint128_t is ready for real 128-bit applications!" << std::endl;

    return 0;
}
