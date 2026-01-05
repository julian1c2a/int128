// Test para debugging int128_t::parse()
#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Debug test int128_t::parse(\"42\")" << std::endl;

    // Verificar signedness
    std::cout << "is_signed for int128_t: " << int128_t::is_signed << std::endl;
    std::cout << "is_signed for uint128_t: " << uint128_t::is_signed << std::endl;

    // Test uint128_t
    auto [uerr, uval] = uint128_t::parse("42");
    std::cout << "\nuint128_t::parse(\"42\"): error=" << static_cast<int>(uerr)
              << ", value=" << uval.to_string() << std::endl;

    // Test int128_t
    auto [ierr, ival] = int128_t::parse("42");
    std::cout << "int128_t::parse(\"42\"): error=" << static_cast<int>(ierr)
              << ", value=" << ival.to_string() << std::endl;

    // Test parse_base directamente
    auto [berr, bval] = int128_t::parse_base("42", 10);
    std::cout << "int128_t::parse_base(\"42\", 10): error=" << static_cast<int>(berr)
              << ", value=" << bval.to_string() << std::endl;

    // Ver el overflow límite
    std::cout << "\nint128_t::max() = " << int128_t::max().to_string() << std::endl;
    std::cout << "uint128_t::max() = " << uint128_t::max().to_string() << std::endl;

    return 0;
}
