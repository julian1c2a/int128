// Test de literales de usuario y separadores de dígitos
// Demuestra todas las formas de crear int128_t/uint128_t desde strings

#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;
using namespace nstd::int128_literals;

int main()
{
    std::cout << "=== Test 1: Literales básicos (unsigned) ===" << std::endl;
    auto u1 = 12345_u128;
    std::cout << "12345_u128 = " << u1.to_string() << std::endl;

    auto u2 = 0xDEADBEEF_u128;
    std::cout << "0xDEADBEEF_u128 = " << u2.to_string() << " (hex)" << std::endl;

    std::cout << "\n=== Test 2: Separadores de dígitos ===" << std::endl;
    auto u3 = 1'234'567_u128;
    std::cout << "1'234'567_u128 = " << u3.to_string() << std::endl;

    auto u4 = 0xFF'AA'BB'CC_u128;
    std::cout << "0xFF'AA'BB'CC_u128 = " << u4.to_string() << " (decimal)" << std::endl;
    std::cout << "  En hex: 0x" << u4.to_string(16) << std::endl;

    std::cout << "\n=== Test 3: Literales firmados (NO usan signo explícito) ===" << std::endl;
    std::cout << "NOTA: -42_i128 se parsea como -(42_i128), el literal NO recibe el signo"
              << std::endl;
    auto i1 = 42_i128; // Parse "42" como signed
    std::cout << "42_i128 = " << i1.to_string() << std::endl;

    // Para negativos, usar factory functions con string explícita:
    auto i2 = make_int128_signed("-42");
    std::cout << "make_int128_signed(\"-42\") = " << i2.to_string() << std::endl;

    auto i3 = make_int128_signed("-1'234'567");
    std::cout << "make_int128_signed(\"-1'234'567\") = " << i3.to_string() << std::endl;

    std::cout << "\n=== Test 4: Factory functions ===" << std::endl;
    auto f1 = make_uint128("999'999'999");
    std::cout << "make_uint128(\"999'999'999\") = " << f1.to_string() << std::endl;

    auto f2 = make_int128_signed("-888'888'888");
    std::cout << "make_int128_signed(\"-888'888'888\") = " << f2.to_string() << std::endl;

    std::cout << "\n=== Test 5: Verificación de errores ===" << std::endl;
    auto [err1, val1] = make_int128_checked<signedness::unsigned_type>("123'456");
    std::cout << "make_int128_checked(\"123'456\") -> error=" << static_cast<int>(err1)
              << ", value=" << val1.to_string() << std::endl;

    auto [err2, val2] = make_int128_checked<signedness::unsigned_type>("invalid");
    std::cout << "make_int128_checked(\"invalid\") -> error=" << static_cast<int>(err2)
              << ", value=" << val2.to_string() << std::endl;

    std::cout << "\n=== Test 6: Combinación con operadores ===" << std::endl;
    auto sum = 1'000_u128 + 234'567_u128;
    std::cout << "1'000_u128 + 234'567_u128 = " << sum.to_string() << std::endl;

    auto prod = 999_i128 * make_int128_signed("1000");
    std::cout << "999_i128 * make_int128_signed(\"1000\") = " << prod.to_string() << std::endl;

    std::cout << "\n=== Test 7: Constexpr en compile-time ===" << std::endl;
    constexpr auto c1 = 12345_u128;
    constexpr auto c2 = make_uint128("999");
    std::cout << "constexpr 12345_u128 = " << c1.to_string() << std::endl;
    std::cout << "constexpr make_uint128(\"999\") = " << c2.to_string() << std::endl;

    std::cout << "\n✓ Todos los tests completados exitosamente" << std::endl;
    return 0;
}
