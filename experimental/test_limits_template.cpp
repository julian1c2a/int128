/**
 * @file test_limits_template.cpp
 * @brief Test de numeric_limits para int128_base_t<S> unificado
 * @date 2026-01-07
 */

#include "../include_new/int128_base_limits.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test numeric_limits<int128_base_t<S>> ===\n\n";

    int passed = 0;
    int failed = 0;

    // Test 1: uint128_t is_signed
    {
        bool result = (numeric_limits<uint128_t>::is_signed == false);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] uint128_t::is_signed == false\n";
        result ? ++passed : ++failed;
    }

    // Test 2: int128_t is_signed
    {
        bool result = (numeric_limits<int128_t>::is_signed == true);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] int128_t::is_signed == true\n";
        result ? ++passed : ++failed;
    }

    // Test 3: uint128_t digits
    {
        bool result = (numeric_limits<uint128_t>::digits == 128);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] uint128_t::digits == 128\n";
        result ? ++passed : ++failed;
    }

    // Test 4: int128_t digits
    {
        bool result = (numeric_limits<int128_t>::digits == 127);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] int128_t::digits == 127\n";
        result ? ++passed : ++failed;
    }

    // Test 5: uint128_t min
    {
        auto min_val = numeric_limits<uint128_t>::min();
        bool result = (min_val.high() == 0 && min_val.low() == 0);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] uint128_t::min() == 0\n";
        result ? ++passed : ++failed;
    }

    // Test 6: uint128_t max
    {
        auto max_val = numeric_limits<uint128_t>::max();
        bool result = (max_val.high() == UINT64_MAX && max_val.low() == UINT64_MAX);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] uint128_t::max() == 2^128-1\n";
        result ? ++passed : ++failed;
    }

    // Test 7: int128_t min (should be -2^127)
    {
        auto min_val = numeric_limits<int128_t>::min();
        // -2^127 tiene high = 0x8000000000000000, low = 0
        bool result = (min_val.high() == 0x8000000000000000ULL && min_val.low() == 0);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] int128_t::min() == -2^127\n";
        result ? ++passed : ++failed;
    }

    // Test 8: int128_t max (should be 2^127-1)
    {
        auto max_val = numeric_limits<int128_t>::max();
        // 2^127-1 tiene high = 0x7FFFFFFFFFFFFFFF, low = 0xFFFFFFFFFFFFFFFF
        bool result = (max_val.high() == 0x7FFFFFFFFFFFFFFFULL && max_val.low() == UINT64_MAX);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] int128_t::max() == 2^127-1\n";
        result ? ++passed : ++failed;
    }

    // Test 9: is_modulo
    {
        bool result = (numeric_limits<uint128_t>::is_modulo == true &&
                       numeric_limits<int128_t>::is_modulo == false);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] is_modulo correcto\n";
        result ? ++passed : ++failed;
    }

    // Test 10: is_integer
    {
        bool result = (numeric_limits<uint128_t>::is_integer == true &&
                       numeric_limits<int128_t>::is_integer == true);
        std::cout << "[" << (result ? "OK" : "FAIL") << "] is_integer == true\n";
        result ? ++passed : ++failed;
    }

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
