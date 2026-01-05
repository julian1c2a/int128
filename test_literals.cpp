#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;
using namespace nstd::literals; // Para usar los literales

int main()
{
    std::cout << "=== Test de Literales de Usuario y Digit Separators ===\n\n";

    // Test 1: Literales básicos sin separadores
    auto u1 = 12345_u128;
    auto i1 = -42_i128;
    std::cout << "Test 1 - Literales básicos:\n";
    std::cout << "  12345_u128 = " << u1.to_string() << "\n";
    std::cout << "  -42_i128 = " << i1.to_string() << "\n\n";

    // Test 2: Literales con digit separators
    auto u2 = 1'234'567_u128;
    auto i2 = -1'234'567_i128;
    std::cout << "Test 2 - Digit separators:\n";
    std::cout << "  1'234'567_u128 = " << u2.to_string() << "\n";
    std::cout << "  -1'234'567_i128 = " << i2.to_string() << "\n\n";

    // Test 3: Hexadecimal con separadores
    auto u3 = 0xFF'AA'BB'CC_u128;
    std::cout << "Test 3 - Hexadecimal con separadores:\n";
    std::cout << "  0xFF'AA'BB'CC_u128 = " << u3.to_string() << "\n\n";

    // Test 4: make_int128 functions
    auto u4 = make_uint128("999'999'999");
    auto i4 = make_int128_signed("-888'888'888");
    std::cout << "Test 4 - make_int128 functions:\n";
    std::cout << "  make_uint128(\"999'999'999\") = " << u4.to_string() << "\n";
    std::cout << "  make_int128_signed(\"-888'888'888\") = " << i4.to_string() << "\n\n";

    // Test 5: make_int128 template con verificación de errores
    auto [err1, val1] = make_int128_checked<signedness::unsigned_type>("123'456");
    auto [err2, val2] = make_int128_checked<signedness::unsigned_type>("invalid");
    std::cout << "Test 5 - make_int128_checked con errores:\n";
    std::cout << "  make_int128_checked(\"123'456\") -> error=" << static_cast<int>(err1)
              << ", value=" << val1.to_string() << "\n";
    std::cout << "  make_int128_checked(\"invalid\") -> error=" << static_cast<int>(err2)
              << ", value=" << val2.to_string() << "\n\n";

    // Test 6: Constructores con digit separators
    uint128_t u5("1'000'000'000");
    int128_t i5("-1'000'000'000");
    std::cout << "Test 6 - Constructores con digit separators:\n";
    std::cout << "  uint128_t(\"1'000'000'000\") = " << u5.to_string() << "\n";
    std::cout << "  int128_t(\"-1'000'000'000\") = " << i5.to_string() << "\n\n";

    std::cout << "=== Todos los tests completados ===\n";

    return 0;
}
