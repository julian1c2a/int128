/**
 * @file test_cmath_template.cpp
 * @brief Test de funciones matematicas para int128_base_t<S> unificado
 * @date 2026-01-07
 */

#include "../include_new/int128_base_cmath.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test cmath para int128_base_t<S> ===\n\n";

    int passed = 0;
    int failed = 0;

    // Test 1: gcd unsigned
    {
        auto result = gcd(uint128_t(48), uint128_t(18));
        bool ok = (result == uint128_t(6));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] gcd(48, 18) == 6\n";
        ok ? ++passed : ++failed;
    }

    // Test 2: gcd signed
    {
        auto result = gcd(int128_t(-48), int128_t(18));
        bool ok = (result == int128_t(6));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] gcd(-48, 18) == 6\n";
        ok ? ++passed : ++failed;
    }

    // Test 3: lcm unsigned
    {
        auto result = lcm(uint128_t(4), uint128_t(6));
        bool ok = (result == uint128_t(12));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] lcm(4, 6) == 12\n";
        ok ? ++passed : ++failed;
    }

    // Test 4: lcm signed
    {
        auto result = lcm(int128_t(-4), int128_t(6));
        bool ok = (result == int128_t(12));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] lcm(-4, 6) == 12\n";
        ok ? ++passed : ++failed;
    }

    // Test 5: pow unsigned
    {
        auto result = pow(uint128_t(2), uint128_t(10));
        bool ok = (result == uint128_t(1024));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] pow(2, 10) == 1024\n";
        ok ? ++passed : ++failed;
    }

    // Test 6: pow signed negativo
    {
        auto result = pow(int128_t(-2), int128_t(3));
        bool ok = (result == int128_t(-8));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] pow(-2, 3) == -8\n";
        ok ? ++passed : ++failed;
    }

    // Test 7: sqrt unsigned
    {
        auto result = sqrt(uint128_t(100));
        bool ok = (result == uint128_t(10));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] sqrt(100) == 10\n";
        ok ? ++passed : ++failed;
    }

    // Test 8: abs signed
    {
        auto result = abs(int128_t(-42));
        bool ok = (result == int128_t(42));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] abs(-42) == 42\n";
        ok ? ++passed : ++failed;
    }

    // Test 9: min/max
    {
        bool ok = (min(uint128_t(5), uint128_t(10)) == uint128_t(5) &&
                   max(uint128_t(5), uint128_t(10)) == uint128_t(10));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] min(5,10)==5, max(5,10)==10\n";
        ok ? ++passed : ++failed;
    }

    // Test 10: sign
    {
        bool ok = (sign(int128_t(-5)) == int128_t(-1) && sign(int128_t(0)) == int128_t(0) &&
                   sign(int128_t(5)) == int128_t(1));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] sign(-5)==-1, sign(0)==0, sign(5)==1\n";
        ok ? ++passed : ++failed;
    }

    // Test 11: midpoint
    {
        auto result = midpoint(uint128_t(10), uint128_t(20));
        bool ok = (result == uint128_t(15));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] midpoint(10, 20) == 15\n";
        ok ? ++passed : ++failed;
    }

    // Test 12: clamp
    {
        auto result = clamp(uint128_t(15), uint128_t(0), uint128_t(10));
        bool ok = (result == uint128_t(10));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] clamp(15, 0, 10) == 10\n";
        ok ? ++passed : ++failed;
    }

    // Test 13: divmod
    {
        auto [q, r] = divmod(uint128_t(17), uint128_t(5));
        bool ok = (q == uint128_t(3) && r == uint128_t(2));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] divmod(17, 5) == (3, 2)\n";
        ok ? ++passed : ++failed;
    }

    // Test 14: gcd con integral_builtin
    {
        auto result = gcd(uint128_t(48), 18);
        bool ok = (result == uint128_t(6));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] gcd(uint128_t(48), int 18) == 6\n";
        ok ? ++passed : ++failed;
    }

    // Test 15: lcm con integral_builtin
    {
        auto result = lcm(4, uint128_t(6));
        bool ok = (result == uint128_t(12));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] lcm(int 4, uint128_t(6)) == 12\n";
        ok ? ++passed : ++failed;
    }

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
