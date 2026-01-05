#include "int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test parse con traza detallada\n\n";

    // 1. Test parse_base directamente con valor simple
    std::cout << "1. int128_t::parse_base(\"42\", 10):\n";
    auto [err1, val1] = int128_t::parse_base("42", 10);
    std::cout << "   error=" << static_cast<int>(err1) << "\n";
    std::cout << "   val.high()=" << val1.high() << ", val.low()=" << val1.low() << "\n";

    // 2. Verificar max_val en parse_base
    std::cout << "\n2. Verificar UINT128_MAX usado en parse_base:\n";
    uint128_t uint_max = uint128_t::max();
    std::cout << "   uint128_t::max().high()=" << uint_max.high() << "\n";
    std::cout << "   uint128_t::max().low()=" << uint_max.low() << "\n";

    // 3. Test con valor que debería ser válido para int128
    std::cout << "\n3. int128_t::parse(\"42\"):\n";
    auto [err3, val3] = int128_t::parse("42");
    std::cout << "   error=" << static_cast<int>(err3) << "\n";
    std::cout << "   val.high()=" << val3.high() << ", val.low()=" << val3.low() << "\n";

    // 4. Verificar int128_t::max
    std::cout << "\n4. int128_t::max():\n";
    int128_t imax = int128_t::max();
    std::cout << "   high()=" << imax.high() << ", low()=" << imax.low() << "\n";
    std::cout << "   ¿Es positivo? " << (imax > int128_t(0ull, 0ull) ? "SI" : "NO") << "\n";

    return 0;
}
