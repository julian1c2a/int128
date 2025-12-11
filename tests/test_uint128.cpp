#include "../include/uint128_t.hpp"

#include <catch2/catch_test_macros.hpp>

// Helper for creating uint128_t from hex strings for easier testing
uint128_t from_hex(const std::string& hex) {
    if (hex.size() > 32) throw std::invalid_argument("Hex string too long for uint128_t");
    uint128_t result = 0;
    std::string full_hex = std::string(32 > hex.size() ? 32 - hex.size() : 0, '0') + hex;

    std::string high_hex = full_hex.substr(0, 16);
    std::string low_hex = full_hex.substr(16, 16);
    
    uint64_t high = std::stoull(high_hex, nullptr, 16);
    uint64_t low = std::stoull(low_hex, nullptr, 16);

    return uint128_t(high, low);
}


TEST_CASE("Constructors and Assignment", "[uint128]") {
    SECTION("Default constructor") {
        uint128_t a;
        REQUIRE(a.high == 0);
        REQUIRE(a.low == 0);
    }

    SECTION("Value constructor") {
        uint128_t a(123);
        REQUIRE(a.high == 0);
        REQUIRE(a.low == 123);
    }

    SECTION("High/Low constructor") {
        uint128_t a(0x1234, 0x5678);
        REQUIRE(a.high == 0x1234);
        REQUIRE(a.low == 0x5678);
    }

    SECTION("Copy constructor and assignment") {
        uint128_t a(1, 2);
        uint128_t b = a;
        REQUIRE(b.high == 1);
        REQUIRE(b.low == 2);

        uint128_t c;
        c = a;
        REQUIRE(c.high == 1);
        REQUIRE(c.low == 2);
    }
}

TEST_CASE("Comparison Operators", "[uint128]") {
    uint128_t a(1, 10);
    uint128_t b(1, 10);
    uint128_t c(1, 11);
    uint128_t d(2, 5);

    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(a != d);
    
    REQUIRE(a < c);
    REQUIRE(a < d);
    REQUIRE(c < d);

    REQUIRE(c > a);
    REQUIRE(d > a);
    REQUIRE(d > c);

    REQUIRE(a <= b);
    REQUIRE(a <= c);
    REQUIRE(c >= a);
    REQUIRE(b >= a);
}

#ifdef _MSC_VER // These tests rely on MSVC-specific intrinsics
TEST_CASE("MSVC Arithmetic Operators", "[uint128][msvc]") {
    
    SECTION("Addition") {
        uint128_t a(0, 1);
        uint128_t b(0, 2);
        REQUIRE((a + b) == uint128_t(0, 3));

        uint128_t max_low(0, 0xFFFFFFFFFFFFFFFF);
        uint128_t c = max_low + uint128_t(0, 1);
        REQUIRE(c.high == 1);
        REQUIRE(c.low == 0);
    }

    SECTION("Subtraction") {
        uint128_t a(0, 3);
        uint128_t b(0, 2);
        REQUIRE((a - b) == uint128_t(0, 1));

        uint128_t c(1, 0);
        uint128_t d = c - uint128_t(0, 1);
        REQUIRE(d.high == 0);
        REQUIRE(d.low == 0xFFFFFFFFFFFFFFFF);
    }

    SECTION("Multiplication") {
        uint128_t a(0, 2);
        uint128_t b(0, 3);
        REQUIRE((a * b) == uint128_t(0, 6));

        uint128_t c(0, 0xFFFFFFFFFFFFFFFF); // max 64-bit
        uint128_t d(0, 2);
        uint128_t res = c * d;
        REQUIRE(res.high == 1);
        REQUIRE(res.low == 0xFFFFFFFFFFFFFFFE);
    }
    
    SECTION("Division and Modulo by uint64_t") {
        uint128_t a(1, 0); // a = 2^64
        uint64_t b = 2;
        
        uint128_t res_div = a / b;
        REQUIRE(res_div.high == 0);
        REQUIRE(res_div.low == 0x8000000000000000);

        uint128_t c(0, 7);
        uint64_t d = 3;
        REQUIRE((c / d) == uint128_t(0, 2));
        REQUIRE((c % d) == uint128_t(0, 1));
        
        uint128_t e = from_hex("123456789ABCDEF0123456789ABCDEF0");
        uint64_t f = 0xFEDCBA9876543210;
        
        REQUIRE((e / f) == uint128_t(0, 0x125B63A27D3768D4));
        REQUIRE((e % f) == uint128_t(0, 0x8B342B24A8765430));
    }
}
#endif

TEST_CASE("Bitwise Operators", "[uint128]") {
    uint128_t a(0x0F0F, 0xAAAA);
    uint128_t b(0xF0F0, 0x5555);

    SECTION("AND") {
        uint128_t res = a & b;
        REQUIRE(res.high == 0x0000);
        REQUIRE(res.low == 0x0000);
    }

    SECTION("OR") {
        uint128_t res = a | b;
        REQUIRE(res.high == 0xFFFF);
        REQUIRE(res.low == 0xFFFF);
    }

    SECTION("XOR") {
        uint128_t res = a ^ b;
        REQUIRE(res.high == 0xFFFF);
        REQUIRE(res.low == 0xFFFF);
    }

    SECTION("NOT") {
        uint128_t res = ~a;
        REQUIRE(res.high == 0xFFFFFFFFFFFFf0f0);
        REQUIRE(res.low == 0xFFFFFFFFFFFF5555);
    }
}

TEST_CASE("Shift Operators", "[uint128]") {
    SECTION("Left shift") {
        uint128_t a(1, 0);
        uint128_t b = a << 1;
        REQUIRE(b.high == 2);
        REQUIRE(b.low == 0);

        uint128_t c(0, 0x8000000000000000);
        c <<= 1;
        REQUIRE(c.high == 1);
        REQUIRE(c.low == 0);
    }

    SECTION("Right shift") {
        uint128_t a(1, 0);
        uint128_t b = a >> 1;
        REQUIRE(b.high == 0);
        REQUIRE(b.low == 0x8000000000000000);

        uint128_t c(1, 0);
        c >>= 64;
        REQUIRE(c.high == 0);
        REQUIRE(c.low == 1);
    }
}