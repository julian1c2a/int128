/*
 * Tests extraídos para uint128_traits.hpp
 * Pruebas individuales por función/especialización
 */

// [WARN] CRITICAL: Include traits specializations BEFORE any other header
// that might include <type_traits>
#include "../include/uint128/uint128_traits_specializations.hpp"

#include "../include/uint128/uint128_limits.hpp"
#include "../include/uint128/uint128_t.hpp"
#include "../include/uint128/uint128_traits.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <vector>

using namespace nstd;


// Los traits se definen en uint128_traits.hpp
// La macro UINT128_USING_LIBCPP indica si las especializaciones están disponibles
// (0 = disponibles en GCC/MSVC con libstdc++/MS STL, 1 = NO disponibles en libc++)

// =============================================================================
// TEST: is_integral
// =============================================================================
bool test_is_integral()
{
    std::cout << "\n=== TEST: is_integral ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_integral_v<uint128_t>;
    std::cout << "  std::is_integral_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser integral");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_arithmetic
// =============================================================================
bool test_is_arithmetic()
{
    std::cout << "\n=== TEST: is_arithmetic ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_arithmetic_v<uint128_t>;
    std::cout << "  std::is_arithmetic_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser arithmetic");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_unsigned
// =============================================================================
bool test_is_unsigned()
{
    std::cout << "\n=== TEST: is_unsigned ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_unsigned_v<uint128_t>;
    std::cout << "  std::is_unsigned_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser unsigned");

    bool not_signed = !std::is_signed_v<uint128_t>;
    std::cout << "  !std::is_signed_v<uint128_t> = " << not_signed << "\n";
    assert(not_signed && "uint128_t no debe ser signed");

    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_signed
// =============================================================================
bool test_is_signed()
{
    std::cout << "\n=== TEST: is_signed ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_signed_v<uint128_t>;
    std::cout << "  std::is_signed_v<uint128_t> = " << result << "\n";
    assert(!result && "uint128_t NO debe ser signed");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_trivially_copyable
// =============================================================================
bool test_is_trivially_copyable()
{
    std::cout << "\n=== TEST: is_trivially_copyable ===\n";
    bool result = std::is_trivially_copyable_v<uint128_t>;
    std::cout << "  std::is_trivially_copyable_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente copiable");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: is_trivially_copy_constructible
// =============================================================================
bool test_is_trivially_copy_constructible()
{
    std::cout << "\n=== TEST: is_trivially_copy_constructible ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_trivially_copy_constructible_v<uint128_t>;
    std::cout << "  std::is_trivially_copy_constructible_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente copy constructible");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_trivially_move_constructible
// =============================================================================
bool test_is_trivially_move_constructible()
{
    std::cout << "\n=== TEST: is_trivially_move_constructible ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_trivially_move_constructible_v<uint128_t>;
    std::cout << "  std::is_trivially_move_constructible_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente move constructible");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_trivially_copy_assignable
// =============================================================================
bool test_is_trivially_copy_assignable()
{
    std::cout << "\n=== TEST: is_trivially_copy_assignable ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_trivially_copy_assignable_v<uint128_t>;
    std::cout << "  std::is_trivially_copy_assignable_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente copy assignable");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_trivially_move_assignable
// =============================================================================
bool test_is_trivially_move_assignable()
{
    std::cout << "\n=== TEST: is_trivially_move_assignable ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_trivially_move_assignable_v<uint128_t>;
    std::cout << "  std::is_trivially_move_assignable_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente move assignable");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_trivially_destructible
// =============================================================================
bool test_is_trivially_destructible()
{
    std::cout << "\n=== TEST: is_trivially_destructible ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_trivially_destructible_v<uint128_t>;
    std::cout << "  std::is_trivially_destructible_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe ser trivialmente destructible");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: is_standard_layout
// =============================================================================
bool test_is_standard_layout()
{
    std::cout << "\n=== TEST: is_standard_layout ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_standard_layout_v<uint128_t>;
    std::cout << "  std::is_standard_layout_v<uint128_t> = " << result << "\n";
    assert(result && "uint128_t debe tener standard layout");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: make_unsigned
// =============================================================================
bool test_make_unsigned()
{
    std::cout << "\n=== TEST: make_unsigned ===\n";
#if !UINT128_USING_LIBCPP
    bool result = std::is_same_v<std::make_unsigned_t<uint128_t>, uint128_t>;
    std::cout << "  std::make_unsigned_t<uint128_t> == uint128_t: " << result << "\n";
    assert(result && "make_unsigned<uint128_t> debe devolver uint128_t");
    std::cout << "  [OK] PASS\n";
#else
    std::cout << "  [SKIP] No soportado en esta configuración STL\n";
#endif
    return true;
}

// =============================================================================
// TEST: common_type with uint64_t
// =============================================================================
bool test_common_type_uint64()
{
    std::cout << "\n=== TEST: common_type<uint128_t, uint64_t> ===\n";
    bool result1 = std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t>;
    bool result2 = std::is_same_v<std::common_type_t<uint64_t, uint128_t>, uint128_t>;

    std::cout << "  common_type<uint128_t, uint64_t> = uint128_t: " << result1 << "\n";
    std::cout << "  common_type<uint64_t, uint128_t> = uint128_t: " << result2 << "\n";

    assert(result1 && "common_type<uint128_t, uint64_t> debe ser uint128_t");
    assert(result2 && "common_type<uint64_t, uint128_t> debe ser uint128_t");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: common_type with uint32_t
// =============================================================================
bool test_common_type_uint32()
{
    std::cout << "\n=== TEST: common_type<uint128_t, uint32_t> ===\n";
    bool result1 = std::is_same_v<std::common_type_t<uint128_t, uint32_t>, uint128_t>;
    bool result2 = std::is_same_v<std::common_type_t<uint32_t, uint128_t>, uint128_t>;

    std::cout << "  common_type<uint128_t, uint32_t> = uint128_t: " << result1 << "\n";
    std::cout << "  common_type<uint32_t, uint128_t> = uint128_t: " << result2 << "\n";

    assert(result1 && "common_type<uint128_t, uint32_t> debe ser uint128_t");
    assert(result2 && "common_type<uint32_t, uint128_t> debe ser uint128_t");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: common_type with int
// =============================================================================
bool test_common_type_int()
{
    std::cout << "\n=== TEST: common_type<uint128_t, int> ===\n";
    bool result1 = std::is_same_v<std::common_type_t<uint128_t, int>, uint128_t>;
    bool result2 = std::is_same_v<std::common_type_t<int, uint128_t>, uint128_t>;

    std::cout << "  common_type<uint128_t, int> = uint128_t: " << result1 << "\n";
    std::cout << "  common_type<int, uint128_t> = uint128_t: " << result2 << "\n";

    assert(result1 && "common_type<uint128_t, int> debe ser uint128_t");
    assert(result2 && "common_type<int, uint128_t> debe ser uint128_t");
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: std::hash
// =============================================================================
bool test_hash()
{
    std::cout << "\n=== TEST: std::hash<uint128_t> ===\n";

    std::hash<uint128_t> hasher;

    // Probar valores diferentes
    uint128_t val1(0, 42);
    uint128_t val2(0, 43);
    uint128_t val3(1, 42);
    uint128_t val4(0, 42); // Igual a val1

    size_t hash1 = hasher(val1);
    size_t hash2 = hasher(val2);
    size_t hash3 = hasher(val3);
    size_t hash4 = hasher(val4);

    std::cout << "  hash(0, 42) = " << hash1 << "\n";
    std::cout << "  hash(0, 43) = " << hash2 << "\n";
    std::cout << "  hash(1, 42) = " << hash3 << "\n";
    std::cout << "  hash(0, 42) [dup] = " << hash4 << "\n";

    assert(hash1 == hash4 && "Valores iguales deben tener el mismo hash");
    assert(hash1 != hash2 && "Valores diferentes deben tener hashes diferentes");
    assert(hash1 != hash3 && "Valores diferentes deben tener hashes diferentes");

    // Probar en un unordered_map
    std::unordered_map<uint128_t, int> map;
    map[val1] = 100;
    map[val2] = 200;
    map[val3] = 300;

    assert(map[val1] == 100);
    assert(map[val4] == 100); // val4 == val1
    assert(map[val2] == 200);
    assert(map[val3] == 300);

    std::cout << "  Unordered map con uint128_t funciona correctamente\n";
    std::cout << "  [OK] PASS\n";
    return true;
}

// =============================================================================
// TEST: numeric_limits
// =============================================================================
bool test_numeric_limits()
{
    std::cout << "\n=== TEST: std::numeric_limits<uint128_t> ===\n";

    using limits = std::numeric_limits<uint128_t>;

    std::cout << "  is_specialized = " << limits::is_specialized << "\n";
    std::cout << "  is_signed = " << limits::is_signed << "\n";
    std::cout << "  is_integer = " << limits::is_integer << "\n";
    std::cout << "  digits = " << limits::digits << "\n";
    std::cout << "  digits10 = " << limits::digits10 << "\n";

    if (!limits::is_specialized) {
        std::cout << "  [SKIP] numeric_limits no está especializado en esta configuración STL\n";
        return true;
    }

    assert(!limits::is_signed && "uint128_t no debe ser signed");
    assert(limits::is_integer && "uint128_t debe ser integer");
    assert(limits::digits == 128 && "uint128_t debe tener 128 bits");

    uint128_t min_val = limits::min();
    uint128_t max_val = limits::max();

    assert(min_val == uint128_t(0) && "min() debe ser 0");
    assert(max_val != uint128_t(0) && "max() debe ser != 0");

    std::cout << "  min() = 0\n";
    std::cout << "  max() != 0\n";
    std::cout << "  [OK] PASS\n";

    return true;
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    std::cout << "====================================================\n";
    std::cout << "  uint128_traits.hpp - Tests Extraídos Individuales\n";
    std::cout << "====================================================\n";

    int passed = 0;
    int total = 0;

#define RUN_TEST(func)                                                                             \
    total++;                                                                                       \
    try {                                                                                          \
        if (func())                                                                                \
            passed++;                                                                              \
    } catch (const std::exception& e) {                                                            \
        std::cout << "  [FAIL] EXCEPTION: " << e.what() << "\n";                                       \
    }

    // Tests de type traits fundamentales
    RUN_TEST(test_is_integral);
    RUN_TEST(test_is_arithmetic);
    RUN_TEST(test_is_unsigned);
    RUN_TEST(test_is_signed);

    // Tests de trivialidad
    RUN_TEST(test_is_trivially_copyable);
    RUN_TEST(test_is_trivially_copy_constructible);
    RUN_TEST(test_is_trivially_move_constructible);
    RUN_TEST(test_is_trivially_copy_assignable);
    RUN_TEST(test_is_trivially_move_assignable);
    RUN_TEST(test_is_trivially_destructible);
    RUN_TEST(test_is_standard_layout);

    // Tests de transformación de tipos
    RUN_TEST(test_make_unsigned);

    // Tests de common_type
    RUN_TEST(test_common_type_uint64);
    RUN_TEST(test_common_type_uint32);
    RUN_TEST(test_common_type_int);

    // Tests de hash
    RUN_TEST(test_hash);

    // Tests de numeric_limits
    RUN_TEST(test_numeric_limits);

    std::cout << "\n====================================================\n";
    std::cout << "  RESUMEN: " << passed << "/" << total << " tests pasados\n";
    std::cout << "====================================================\n";

    return (passed == total) ? 0 : 1;
}
