/*
 * Test para int128_traits.hpp e int128_limits.hpp
 */

#include "../include/int128_limits.hpp"
#include "../include/int128_t.hpp"
#include "../include/int128_traits.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_set>

void test_traits()
{
    std::cout << "=== Testing INT128_T TRAITS ===" << std::endl;

    // Type traits fundamentales
    std::cout << "\n[INFO] Type traits fundamentales:" << std::endl;
    std::cout << "is_integral<int128_t>: " << std::is_integral_v<int128_t> << std::endl;
    std::cout << "is_arithmetic<int128_t>: " << std::is_arithmetic_v<int128_t> << std::endl;
    std::cout << "is_signed<int128_t>: " << std::is_signed_v<int128_t> << std::endl;
    std::cout << "is_unsigned<int128_t>: " << std::is_unsigned_v<int128_t> << std::endl;

    static_assert(std::is_integral_v<int128_t>);
    static_assert(std::is_arithmetic_v<int128_t>);
    static_assert(std::is_signed_v<int128_t>);
    static_assert(!std::is_unsigned_v<int128_t>);

    // Transformación de tipos
    std::cout << "\n[INFO] Type transformations:" << std::endl;
    std::cout << "make_signed<int128_t> == int128_t: "
              << std::is_same_v<std::make_signed_t<int128_t>, int128_t> << std::endl;
    std::cout << "make_unsigned<int128_t> == uint128_t: "
              << std::is_same_v<std::make_unsigned_t<int128_t>, uint128_t> << std::endl;

    static_assert(std::is_same_v<std::make_signed_t<int128_t>, int128_t>);
    static_assert(std::is_same_v<std::make_unsigned_t<int128_t>, uint128_t>);

    // Common types
    std::cout << "\n[INFO] Common types:" << std::endl;
    std::cout << "common_type<int128_t, int64_t> == int128_t: "
              << std::is_same_v<std::common_type_t<int128_t, int64_t>, int128_t> << std::endl;
    std::cout << "common_type<int128_t, uint64_t> == int128_t: "
              << std::is_same_v<std::common_type_t<int128_t, uint64_t>, int128_t> << std::endl;
    std::cout << "common_type<int128_t, uint128_t> == uint128_t: "
              << std::is_same_v<std::common_type_t<int128_t, uint128_t>, uint128_t> << std::endl;

    static_assert(std::is_same_v<std::common_type_t<int128_t, int64_t>, int128_t>);
    static_assert(std::is_same_v<std::common_type_t<int128_t, uint64_t>, int128_t>);
    static_assert(std::is_same_v<std::common_type_t<int128_t, uint128_t>, uint128_t>);

    std::cout << "[OK] Traits OK" << std::endl;
}

void test_limits()
{
    std::cout << "\n=== Testing INT128_T LIMITS ===" << std::endl;

    using limits = std::numeric_limits<int128_t>;

    // Características básicas
    std::cout << "\n[INFO] Características básicas:" << std::endl;
    std::cout << "is_specialized: " << limits::is_specialized << std::endl;
    std::cout << "is_signed: " << limits::is_signed << std::endl;
    std::cout << "is_integer: " << limits::is_integer << std::endl;
    std::cout << "is_exact: " << limits::is_exact << std::endl;
    std::cout << "is_modulo: " << limits::is_modulo << std::endl;
    std::cout << "digits: " << limits::digits << std::endl;
    std::cout << "digits10: " << limits::digits10 << std::endl;

    static_assert(limits::is_specialized);
    static_assert(limits::is_signed);
    static_assert(limits::is_integer);
    static_assert(limits::is_exact);
    static_assert(!limits::is_modulo);    // Los tipos con signo no son modulo
    static_assert(limits::digits == 127); // 128 - 1 bit de signo

    // Valores especiales
    std::cout << "\n[INFO] Valores límite:" << std::endl;
    auto min_val = limits::min();
    auto max_val = limits::max();
    auto lowest_val = limits::lowest();

    std::cout << "min(): " << min_val << std::endl;
    std::cout << "max(): " << max_val << std::endl;
    std::cout << "lowest(): " << lowest_val << std::endl;

    // Verificar que los valores son correctos
    assert(min_val == int128_t_MIN);
    assert(max_val == int128_t_MAX);
    assert(lowest_val == min_val);

    // Verificar que min es realmente el más pequeño y max el más grande
    assert(min_val < max_val);
    assert(min_val.is_negative());
    assert(max_val.is_positive());

    std::cout << "[OK] Limits OK" << std::endl;
}

void test_hash()
{
    std::cout << "\n=== Testing HASH FUNCTIONALITY ===" << std::endl;

    std::hash<int128_t> hasher;

    // Test básico de hash
    int128_t val1(42);
    int128_t val2(-42);
    int128_t val3(42); // Igual a val1

    auto hash1 = hasher(val1);
    auto hash2 = hasher(val2);
    auto hash3 = hasher(val3);

    std::cout << "\n[INFO] Hash values:" << std::endl;
    std::cout << "hash(42): " << hash1 << std::endl;
    std::cout << "hash(-42): " << hash2 << std::endl;
    std::cout << "hash(42) again: " << hash3 << std::endl;

    // Valores iguales deben tener el mismo hash
    assert(hash1 == hash3);

    // Valores diferentes probablemente tienen hash diferentes (no garantizado)
    std::cout << "Different values have different hashes: " << (hash1 != hash2) << std::endl;

    // Test con unordered_set
    std::cout << "\n[INFO] Testing with unordered_set:" << std::endl;
    std::unordered_set<int128_t> set;

    set.insert(int128_t(1));
    set.insert(int128_t(-1));
    set.insert(int128_t(1000));
    set.insert(int128_t(-1000));
    set.insert(int128_t(1)); // Duplicado

    std::cout << "Set size: " << set.size() << " (should be 4)" << std::endl;
    assert(set.size() == 4);

    // Verificar que contiene los elementos esperados
    assert(set.count(int128_t(1)) == 1);
    assert(set.count(int128_t(-1)) == 1);
    assert(set.count(int128_t(1000)) == 1);
    assert(set.count(int128_t(-1000)) == 1);
    assert(set.count(int128_t(999)) == 0);

    std::cout << "[OK] Hash OK" << std::endl;
}

void test_constexpr_compatibility()
{
    std::cout << "\n=== Testing CONSTEXPR COMPATIBILITY ===" << std::endl;

    // Verificar que los limits son constexpr
    constexpr auto min_val = std::numeric_limits<int128_t>::min();
    constexpr auto max_val = std::numeric_limits<int128_t>::max();
    constexpr auto digits = std::numeric_limits<int128_t>::digits;

    std::cout << "\n[INFO] Constexpr values:" << std::endl;
    std::cout << "constexpr min: " << min_val << std::endl;
    std::cout << "constexpr max: " << max_val << std::endl;
    std::cout << "constexpr digits: " << digits << std::endl;

    // Compilar estas líneas significa que son verdaderamente constexpr
    static_assert(digits == 127);
    static_assert(min_val.is_negative());
    static_assert(max_val.is_positive());

    std::cout << "[OK] Constexpr compatibility OK" << std::endl;
}

int main()
{
    std::cout << "🧮 COMPREHENSIVE INT128_T TRAITS & LIMITS TESTING" << std::endl;
    std::cout << "==================================================" << std::endl;

    try {
        test_traits();
        test_limits();
        test_hash();
        test_constexpr_compatibility();

        std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!" << std::endl;
        std::cout << "\n🔧 CARACTERÍSTICAS CONFIRMADAS:" << std::endl;
        std::cout << "[OK] std::is_integral<int128_t> == true" << std::endl;
        std::cout << "[OK] std::is_signed<int128_t> == true" << std::endl;
        std::cout << "[OK] std::is_arithmetic<int128_t> == true" << std::endl;
        std::cout << "[OK] std::numeric_limits<int128_t> especializado" << std::endl;
        std::cout << "[OK] std::hash<int128_t> funcional" << std::endl;
        std::cout << "[OK] std::common_type compatibilidad completa" << std::endl;
        std::cout << "[OK] make_signed/make_unsigned correcto" << std::endl;
        std::cout << "[OK] Compatibilidad constexpr total" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}