// Test para verificar std::is_integral<int128_t>

#include "int128/int128_t.hpp"
#include "uint128/uint128_t.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "Verificando std::is_integral para tipos 128-bit:\n\n";

    // uint128_t
    std::cout << "uint128_t:\n";
    std::cout << "  std::is_integral_v<uint128_t> = "
              << (std::is_integral_v<uint128_t> ? "true [OK]" : "false [FAIL]") << "\n";
    std::cout << "  std::is_arithmetic_v<uint128_t> = "
              << (std::is_arithmetic_v<uint128_t> ? "true [OK]" : "false [FAIL]") << "\n";
    std::cout << "  std::is_unsigned_v<uint128_t> = "
              << (std::is_unsigned_v<uint128_t> ? "true [OK]" : "false [FAIL]") << "\n";

    std::cout << "\nint128_t:\n";
    std::cout << "  std::is_integral_v<int128_t> = "
              << (std::is_integral_v<int128_t> ? "true [OK]" : "false [FAIL]") << "\n";
    std::cout << "  std::is_arithmetic_v<int128_t> = "
              << (std::is_arithmetic_v<int128_t> ? "true [OK]" : "false [FAIL]") << "\n";
    std::cout << "  std::is_signed_v<int128_t> = "
              << (std::is_signed_v<int128_t> ? "true [OK]" : "false [FAIL]") << "\n";

    std::cout << "\nMacros de compilador:\n";
#if defined(_LIBCPP_VERSION)
    std::cout << "  _LIBCPP_VERSION definido (libc++)\n";
#endif
#if defined(_MSC_VER)
    std::cout << "  _MSC_VER definido (MSVC)\n";
#endif
#if defined(__GNUC__)
    std::cout << "  __GNUC__ = " << __GNUC__ << " (GCC)\n";
#endif
#if defined(UINT128_USING_LIBCPP)
    std::cout << "  UINT128_USING_LIBCPP = " << UINT128_USING_LIBCPP << "\n";
#else
    std::cout << "  UINT128_USING_LIBCPP no definido\n";
#endif

    // Test con gcd si es_integral
    if (std::is_integral_v<int128_t>) {
        std::cout << "\n🎉 ¡int128_t es integral! std::gcd debería funcionar.\n";
    } else {
        std::cout << "\n[WARN]  int128_t NO es integral. std::gcd no funcionará.\n";
    }

    return 0;
}
