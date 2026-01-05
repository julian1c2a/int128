// Test para debug detallado del parsing
#include "int128_base.hpp"
#include <iostream>

using uint128_t = nstd::int128_base_t<nstd::signedness::unsigned_type>;

void test_parse_detailed()
{
    std::cout << "=== Test detallado de parse_base ===" << std::endl;

    // Test simple: "10"
    auto [err1, val1] = uint128_t::parse_base("10", 10);
    std::cout << "parse_base(\"10\", 10): error=" << static_cast<int>(err1)
              << ", value=" << val1.to_string() << std::endl;

    // Test simple: "1"
    auto [err2, val2] = uint128_t::parse_base("1", 10);
    std::cout << "parse_base(\"1\", 10): error=" << static_cast<int>(err2)
              << ", value=" << val2.to_string() << std::endl;

    // Test simple: "0"
    auto [err3, val3] = uint128_t::parse_base("0", 10);
    std::cout << "parse_base(\"0\", 10): error=" << static_cast<int>(err3)
              << ", value=" << val3.to_string() << std::endl;

    // Test problema: "12345"
    auto [err4, val4] = uint128_t::parse_base("12345", 10);
    std::cout << "parse_base(\"12345\", 10): error=" << static_cast<int>(err4)
              << ", value=" << val4.to_string() << std::endl;
}

int main()
{
    test_parse_detailed();
    return 0;
}
