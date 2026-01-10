/**
 * @file int128_traits_extracted_tests.cpp
 * @brief Tests completos de int128_base_traits.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_traits.hpp tests (18 tests)
 * - test_traits_template.cpp (15 tests)
 *
 * Total: 30+ tests cubriendo:
 * - Type traits fundamentales (is_integral, is_arithmetic, is_signed, is_unsigned)
 * - Traits de trivialidad (copyable, constructible, destructible)
 * - Transformaciones de tipos (make_signed, make_unsigned)
 * - common_type con varios tipos
 * - hash para uint128_t e int128_t
 * - unordered containers
 */

#include "int128_base_traits.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace nstd;

static int tests_passed = 0;
static int tests_failed = 0;

void check(bool condition, const char *test_name)
{
      if (condition)
      {
            std::cout << "[OK]   " << test_name << std::endl;
            tests_passed++;
      }
      else
      {
            std::cout << "[FAIL] " << test_name << std::endl;
            tests_failed++;
      }
}

// =============================================================================
// SECTION: Type traits fundamentales usando namespace nstd
// =============================================================================

void test_nstd_is_integral()
{
      check(nstd::is_integral_v<uint128_t>, "nstd::is_integral_v<uint128_t>");
      check(nstd::is_integral_v<int128_t>, "nstd::is_integral_v<int128_t>");
      check(!nstd::is_integral_v<float>, "!nstd::is_integral_v<float>");
}

void test_nstd_is_signed()
{
      check(nstd::is_signed_v<int128_t>, "nstd::is_signed_v<int128_t>");
      check(!nstd::is_signed_v<uint128_t>, "!nstd::is_signed_v<uint128_t>");
}

void test_nstd_is_unsigned()
{
      check(nstd::is_unsigned_v<uint128_t>, "nstd::is_unsigned_v<uint128_t>");
      check(!nstd::is_unsigned_v<int128_t>, "!nstd::is_unsigned_v<int128_t>");
}

void test_nstd_is_arithmetic()
{
      check(nstd::is_arithmetic_v<uint128_t>, "nstd::is_arithmetic_v<uint128_t>");
      check(nstd::is_arithmetic_v<int128_t>, "nstd::is_arithmetic_v<int128_t>");
}

// =============================================================================
// SECTION: Transformaciones de tipos
// =============================================================================

void test_make_signed()
{
      using signed_from_unsigned = nstd::make_signed_t<uint128_t>;
      using signed_from_signed = nstd::make_signed_t<int128_t>;

      check(std::is_same_v<signed_from_unsigned, int128_t>,
            "make_signed_t<uint128_t> = int128_t");
      check(std::is_same_v<signed_from_signed, int128_t>,
            "make_signed_t<int128_t> = int128_t");
}

void test_make_unsigned()
{
      using unsigned_from_unsigned = nstd::make_unsigned_t<uint128_t>;
      using unsigned_from_signed = nstd::make_unsigned_t<int128_t>;

      check(std::is_same_v<unsigned_from_unsigned, uint128_t>,
            "make_unsigned_t<uint128_t> = uint128_t");
      check(std::is_same_v<unsigned_from_signed, uint128_t>,
            "make_unsigned_t<int128_t> = uint128_t");
}

// =============================================================================
// SECTION: common_type tests
// =============================================================================

void test_common_type_same()
{
      check(std::is_same_v<nstd::common_type_t<uint128_t, uint128_t>, uint128_t>,
            "common_type<uint128_t, uint128_t> = uint128_t");
      check(std::is_same_v<nstd::common_type_t<int128_t, int128_t>, int128_t>,
            "common_type<int128_t, int128_t> = int128_t");
}

void test_common_type_cross()
{
      // signed tiene prioridad
      check(std::is_same_v<nstd::common_type_t<uint128_t, int128_t>, int128_t>,
            "common_type<uint128_t, int128_t> = int128_t");
      check(std::is_same_v<nstd::common_type_t<int128_t, uint128_t>, int128_t>,
            "common_type<int128_t, uint128_t> = int128_t");
}

void test_common_type_uint64()
{
      check(std::is_same_v<nstd::common_type_t<uint128_t, uint64_t>, uint128_t>,
            "common_type<uint128_t, uint64_t> = uint128_t");
      check(std::is_same_v<nstd::common_type_t<uint64_t, uint128_t>, uint128_t>,
            "common_type<uint64_t, uint128_t> = uint128_t");
}

void test_common_type_int64()
{
      check(std::is_same_v<nstd::common_type_t<int128_t, int64_t>, int128_t>,
            "common_type<int128_t, int64_t> = int128_t");
      check(std::is_same_v<nstd::common_type_t<int64_t, int128_t>, int128_t>,
            "common_type<int64_t, int128_t> = int128_t");
}

void test_common_type_int()
{
      check(std::is_same_v<nstd::common_type_t<int128_t, int>, int128_t>,
            "common_type<int128_t, int> = int128_t");
      check(std::is_same_v<nstd::common_type_t<int, int128_t>, int128_t>,
            "common_type<int, int128_t> = int128_t");
}

// =============================================================================
// SECTION: Trivialidad (usando std:: directamente)
// =============================================================================

void test_trivially_copyable()
{
      check(std::is_trivially_copyable_v<uint128_t>,
            "std::is_trivially_copyable<uint128_t>");
      check(std::is_trivially_copyable_v<int128_t>,
            "std::is_trivially_copyable<int128_t>");
}

void test_trivially_constructible()
{
      check(std::is_trivially_copy_constructible_v<uint128_t>,
            "std::is_trivially_copy_constructible<uint128_t>");
      check(std::is_trivially_copy_constructible_v<int128_t>,
            "std::is_trivially_copy_constructible<int128_t>");

      check(std::is_trivially_move_constructible_v<uint128_t>,
            "std::is_trivially_move_constructible<uint128_t>");
      check(std::is_trivially_move_constructible_v<int128_t>,
            "std::is_trivially_move_constructible<int128_t>");
}

void test_trivially_assignable()
{
      check(std::is_trivially_copy_assignable_v<uint128_t>,
            "std::is_trivially_copy_assignable<uint128_t>");
      check(std::is_trivially_copy_assignable_v<int128_t>,
            "std::is_trivially_copy_assignable<int128_t>");

      check(std::is_trivially_move_assignable_v<uint128_t>,
            "std::is_trivially_move_assignable<uint128_t>");
      check(std::is_trivially_move_assignable_v<int128_t>,
            "std::is_trivially_move_assignable<int128_t>");
}

void test_trivially_destructible()
{
      check(std::is_trivially_destructible_v<uint128_t>,
            "std::is_trivially_destructible<uint128_t>");
      check(std::is_trivially_destructible_v<int128_t>,
            "std::is_trivially_destructible<int128_t>");
}

void test_standard_layout()
{
      check(std::is_standard_layout_v<uint128_t>,
            "std::is_standard_layout<uint128_t>");
      check(std::is_standard_layout_v<int128_t>,
            "std::is_standard_layout<int128_t>");
}

// =============================================================================
// SECTION: Hash
// =============================================================================

void test_hash_uint128()
{
      nstd::hash<uint128_t> hasher;

      uint128_t val1(123);
      uint128_t val2(123);
      uint128_t val3(456);

      size_t hash1 = hasher(val1);
      size_t hash2 = hasher(val2);
      size_t hash3 = hasher(val3);

      check(hash1 == hash2, "hash<uint128_t>: iguales tienen mismo hash");
      check(hash1 != hash3, "hash<uint128_t>: diferentes tienen distinto hash");
}

void test_hash_int128()
{
      nstd::hash<int128_t> hasher;

      int128_t val1(-42);
      int128_t val2(-42);
      int128_t val3(42);

      size_t hash1 = hasher(val1);
      size_t hash2 = hasher(val2);
      size_t hash3 = hasher(val3);

      check(hash1 == hash2, "hash<int128_t>: iguales tienen mismo hash");
      check(hash1 != hash3, "hash<int128_t>: diferentes tienen distinto hash");
}

void test_hash_with_high_bits()
{
      nstd::hash<uint128_t> hasher;

      // Valores que difieren solo en high bits
      uint128_t val1(uint64_t(0), uint64_t(1));
      uint128_t val2(uint64_t(1), uint64_t(1));

      check(hasher(val1) != hasher(val2), "hash distingue high bits diferentes");
}

// =============================================================================
// SECTION: Unordered containers
// =============================================================================

void test_unordered_set_uint128()
{
      std::unordered_set<uint128_t, nstd::hash<uint128_t>> set;
      set.insert(uint128_t(100));
      set.insert(uint128_t(200));
      set.insert(uint128_t(100)); // duplicado

      check(set.size() == 2, "unordered_set<uint128_t> tamaño correcto");
      check(set.count(uint128_t(100)) == 1, "unordered_set<uint128_t> count(100)");
      check(set.count(uint128_t(200)) == 1, "unordered_set<uint128_t> count(200)");
      check(set.count(uint128_t(300)) == 0, "unordered_set<uint128_t> count(300)=0");
}

void test_unordered_set_int128()
{
      std::unordered_set<int128_t, nstd::hash<int128_t>> set;
      set.insert(int128_t(-100));
      set.insert(int128_t(100));
      set.insert(int128_t(-100)); // duplicado

      check(set.size() == 2, "unordered_set<int128_t> tamaño correcto");
      check(set.count(int128_t(-100)) == 1, "unordered_set<int128_t> count(-100)");
      check(set.count(int128_t(100)) == 1, "unordered_set<int128_t> count(100)");
}

void test_unordered_map()
{
      std::unordered_map<uint128_t, int, nstd::hash<uint128_t>> map;

      uint128_t key1(42);
      uint128_t key2(123);

      map[key1] = 100;
      map[key2] = 200;

      check(map[key1] == 100, "unordered_map[key1]=100");
      check(map[key2] == 200, "unordered_map[key2]=200");
      check(map.size() == 2, "unordered_map size=2");
}

// NOTE: numeric_limits tests are in int128_limits_extracted_tests.cpp
// The "limits" feature tests numeric_limits exhaustively (44 tests)

// =============================================================================
// MAIN
// =============================================================================

int main()
{
      std::cout << "=== int128_base_traits.hpp tests ===\n\n";

      std::cout << "--- Type traits fundamentales (nstd) ---\n";
      test_nstd_is_integral();
      test_nstd_is_signed();
      test_nstd_is_unsigned();
      test_nstd_is_arithmetic();

      std::cout << "\n--- Transformaciones de tipos ---\n";
      test_make_signed();
      test_make_unsigned();

      std::cout << "\n--- common_type ---\n";
      test_common_type_same();
      test_common_type_cross();
      test_common_type_uint64();
      test_common_type_int64();
      test_common_type_int();

      std::cout << "\n--- Trivialidad (std) ---\n";
      test_trivially_copyable();
      test_trivially_constructible();
      test_trivially_assignable();
      test_trivially_destructible();
      test_standard_layout();

      std::cout << "\n--- Hash ---\n";
      test_hash_uint128();
      test_hash_int128();
      test_hash_with_high_bits();

      std::cout << "\n--- Unordered containers ---\n";
      test_unordered_set_uint128();
      test_unordered_set_int128();
      test_unordered_map();

      std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
                << " tests pasaron ===\n";

      return (tests_failed == 0) ? 0 : 1;
}
