#include "../include/uint128_t.hpp"
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

TEST_CASE("Verificación exhaustiva de type traits para uint128_t", "[traits][verification]")
{
    SECTION("Numeric limits")
    {
        REQUIRE(std::numeric_limits<uint128_t>::is_specialized);
        REQUIRE(std::numeric_limits<uint128_t>::is_signed == false);
        REQUIRE(std::numeric_limits<uint128_t>::is_integer);
        REQUIRE(std::numeric_limits<uint128_t>::digits == 128);
        REQUIRE(std::numeric_limits<uint128_t>::max() != uint128_t(0));
    }

    SECTION("Type traits básicos")
    {
        REQUIRE(std::is_integral_v<uint128_t>);
        REQUIRE(std::is_unsigned_v<uint128_t>);
        REQUIRE_FALSE(std::is_signed_v<uint128_t>);
        REQUIRE(std::is_arithmetic_v<uint128_t>);
        REQUIRE(std::is_scalar_v<uint128_t>);
    }

    SECTION("Make traits")
    {
        REQUIRE(std::is_same_v<std::make_unsigned_t<uint128_t>, uint128_t>);
        // make_signed debe fallar con mensaje claro
        // REQUIRE(std::is_same_v<std::make_signed_t<uint128_t>, int128_t>);
        // Esto no debería compilar por el static_assert
    }

    SECTION("Common type")
    {
        REQUIRE(std::is_same_v<std::common_type_t<uint128_t, uint64_t>, uint128_t>);
        REQUIRE(std::is_same_v<std::common_type_t<uint32_t, uint128_t>, uint128_t>);
        REQUIRE(std::is_same_v<std::common_type_t<uint128_t, uint128_t>, uint128_t>);
        REQUIRE(std::is_same_v<std::common_type_t<uint8_t, uint128_t>, uint128_t>);
    }

    SECTION("Conceptos C++20")
    {
        REQUIRE(std::integral<uint128_t>);
        REQUIRE(std::unsigned_integral<uint128_t>);
        REQUIRE_FALSE(std::signed_integral<uint128_t>);
    }

    SECTION("Compatibility con algoritmos STL")
    {
        uint128_t valores[] = {1, 2, 3, 4, 5};
        auto min_val = *std::min_element(valores, valores + 5);
        auto max_val = *std::max_element(valores, valores + 5);
        REQUIRE(min_val == uint128_t(1));
        REQUIRE(max_val == uint128_t(5));

        // Test que std::sort funciona
        uint128_t test_vals[] = {5, 1, 3, 2, 4};
        std::sort(test_vals, test_vals + 5);
        REQUIRE(test_vals[0] == uint128_t(1));
        REQUIRE(test_vals[4] == uint128_t(5));
    }
}

// Test de compilación - este test no debe compilar
/*
TEST_CASE("make_signed debe fallar", "[traits][negative]") {
    using signed_type = std::make_signed_t<uint128_t>; // Esto debe dar error
}
*/

TEST_CASE("Integración con templates genéricos", "[traits][templates]")
{
    SECTION("Template que requiere integral")
    {
        auto test_template = []<std::integral T>(T value) { return value + T(1); };

        uint128_t val(100);
        auto result = test_template(val);
        REQUIRE(result == uint128_t(101));
    }

    SECTION("Template que requiere unsigned")
    {
        auto test_template = []<std::unsigned_integral T>(T value) { return value * T(2); };

        uint128_t val(50);
        auto result = test_template(val);
        REQUIRE(result == uint128_t(100));
    }

    SECTION("Common type en templates")
    {
        auto add_mixed = []<typename T, typename U>(T a, U b) {
            using common_t = std::common_type_t<T, U>;
            return static_cast<common_t>(a) + static_cast<common_t>(b);
        };

        auto result = add_mixed(uint128_t(100), uint64_t(50));
        REQUIRE(std::is_same_v<decltype(result), uint128_t>);
        REQUIRE(result == uint128_t(150));
    }
}