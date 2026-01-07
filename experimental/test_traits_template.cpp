/**
 * @file test_traits_template.cpp
 * @brief Test de type traits para int128_base_t<S> unificado
 * @date 2026-01-07
 */

#include "../include_new/int128_base_traits.hpp"
#include <iostream>
#include <unordered_set>

using namespace nstd;

int main()
{
    std::cout << "=== Test traits para int128_base_t<S> ===\n\n";

    int passed = 0;
    int failed = 0;

    // Test 1: is_integral
    {
        bool ok = is_integral_v<uint128_t> && is_integral_v<int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] is_integral<uint128_t/int128_t>\n";
        ok ? ++passed : ++failed;
    }

    // Test 2: is_unsigned
    {
        bool ok = is_unsigned_v<uint128_t> && !is_unsigned_v<int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] is_unsigned correcto\n";
        ok ? ++passed : ++failed;
    }

    // Test 3: is_signed
    {
        bool ok = !is_signed_v<uint128_t> && is_signed_v<int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] is_signed correcto\n";
        ok ? ++passed : ++failed;
    }

    // Test 4: is_arithmetic
    {
        bool ok = is_arithmetic_v<uint128_t> && is_arithmetic_v<int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] is_arithmetic correcto\n";
        ok ? ++passed : ++failed;
    }

    // Test 5: make_signed
    {
        bool ok = std::is_same_v<make_signed_t<uint128_t>, int128_t> &&
                  std::is_same_v<make_signed_t<int128_t>, int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] make_signed correcto\n";
        ok ? ++passed : ++failed;
    }

    // Test 6: make_unsigned
    {
        bool ok = std::is_same_v<make_unsigned_t<uint128_t>, uint128_t> &&
                  std::is_same_v<make_unsigned_t<int128_t>, uint128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] make_unsigned correcto\n";
        ok ? ++passed : ++failed;
    }

    // Test 7: common_type uint128_t consigo mismo
    {
        bool ok = std::is_same_v<common_type_t<uint128_t, uint128_t>, uint128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] common_type<uint128_t, uint128_t>\n";
        ok ? ++passed : ++failed;
    }

    // Test 8: common_type int128_t consigo mismo
    {
        bool ok = std::is_same_v<common_type_t<int128_t, int128_t>, int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] common_type<int128_t, int128_t>\n";
        ok ? ++passed : ++failed;
    }

    // Test 9: common_type cruzado (signed tiene prioridad)
    {
        bool ok = std::is_same_v<common_type_t<uint128_t, int128_t>, int128_t> &&
                  std::is_same_v<common_type_t<int128_t, uint128_t>, int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] common_type<uint128, int128> = int128\n";
        ok ? ++passed : ++failed;
    }

    // Test 10: common_type con uint64_t
    {
        bool ok = std::is_same_v<common_type_t<uint128_t, uint64_t>, uint128_t> &&
                  std::is_same_v<common_type_t<uint64_t, uint128_t>, uint128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] common_type<uint128, uint64>\n";
        ok ? ++passed : ++failed;
    }

    // Test 11: common_type con int64_t
    {
        bool ok = std::is_same_v<common_type_t<int128_t, int64_t>, int128_t> &&
                  std::is_same_v<common_type_t<int64_t, int128_t>, int128_t>;
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] common_type<int128, int64>\n";
        ok ? ++passed : ++failed;
    }

    // Test 12: hash uint128_t
    {
        hash<uint128_t> hasher;
        uint128_t val1(123);
        uint128_t val2(123);
        uint128_t val3(456);
        bool ok = (hasher(val1) == hasher(val2)) && (hasher(val1) != hasher(val3));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hash<uint128_t> funciona\n";
        ok ? ++passed : ++failed;
    }

    // Test 13: hash int128_t
    {
        hash<int128_t> hasher;
        int128_t val1(-42);
        int128_t val2(-42);
        int128_t val3(42);
        bool ok = (hasher(val1) == hasher(val2)) && (hasher(val1) != hasher(val3));
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] hash<int128_t> funciona\n";
        ok ? ++passed : ++failed;
    }

    // Test 14: unordered_set con uint128_t
    {
        std::unordered_set<uint128_t, hash<uint128_t>> set;
        set.insert(uint128_t(100));
        set.insert(uint128_t(200));
        set.insert(uint128_t(100)); // duplicado
        bool ok = (set.size() == 2) && (set.count(uint128_t(100)) == 1);
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] unordered_set<uint128_t>\n";
        ok ? ++passed : ++failed;
    }

    // Test 15: unordered_set con int128_t
    {
        std::unordered_set<int128_t, hash<int128_t>> set;
        set.insert(int128_t(-100));
        set.insert(int128_t(100));
        set.insert(int128_t(-100)); // duplicado
        bool ok = (set.size() == 2) && (set.count(int128_t(-100)) == 1);
        std::cout << "[" << (ok ? "OK" : "FAIL") << "] unordered_set<int128_t>\n";
        ok ? ++passed : ++failed;
    }

    std::cout << "\n=== RESULTADO: " << passed << "/" << (passed + failed)
              << " tests pasaron ===\n";

    return (failed == 0) ? 0 : 1;
}
