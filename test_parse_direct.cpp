// Test mínimo para verificar parse() directamente sin constructores
#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test 1: Parse directo uint128_t" << std::endl;
    auto [err1, val1] = uint128_t::parse("12345");
    std::cout << "parse(\"12345\"): error=" << static_cast<int>(err1)
              << ", value=" << val1.to_string() << std::endl;

    std::cout << "\nTest 2: Parse con separadores" << std::endl;
    auto [err2, val2] = uint128_t::parse("1'234'567");
    std::cout << "parse(\"1'234'567\"): error=" << static_cast<int>(err2)
              << ", value=" << val2.to_string() << std::endl;

    std::cout << "\nTest 3: Parse hexadecimal" << std::endl;
    auto [err3, val3] = uint128_t::parse("0xFF'AA'BB'CC");
    std::cout << "parse(\"0xFF'AA'BB'CC\"): error=" << static_cast<int>(err3)
              << ", value (dec)=" << val3.to_string() << ", value (hex)=" << val3.to_string(16)
              << std::endl;

    std::cout << "\nTest 4: Parse int128_t positivo" << std::endl;
    auto [err4, val4] = int128_t::parse("42");
    std::cout << "parse(\"42\"): error=" << static_cast<int>(err4) << ", value=" << val4.to_string()
              << std::endl;

    std::cout << "\nTest 5: Parse int128_t negativo" << std::endl;
    auto [err5, val5] = int128_t::parse("-42");
    std::cout << "parse(\"-42\"): error=" << static_cast<int>(err5)
              << ", value=" << val5.to_string() << std::endl;

    std::cout << "\nTest 6: Parse inválido" << std::endl;
    auto [err6, val6] = uint128_t::parse("invalid");
    std::cout << "parse(\"invalid\"): error=" << static_cast<int>(err6)
              << ", value=" << val6.to_string() << std::endl;

    std::cout << "\n✓ Tests completados sin usar constructores" << std::endl;
    return 0;
}
