#include "include_new/int128_base_tt.hpp"
#include <cstdint>
#include <iostream>
using namespace nstd;

int main()
{
    // Test 1: int128_t positivo * int64_t negativo
    std::cout << "=== Test 1: 1000000 * (-5) ===" << std::endl;
    int128_t a(1000000ll);
    int64_t b = -5ll;

    std::cout << "a.high = 0x" << std::hex << a.high() << std::endl;
    std::cout << "a.low = 0x" << a.low() << std::dec << " = " << a.low() << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "static_cast<uint64_t>(b) = 0x" << std::hex << static_cast<uint64_t>(b) << std::dec
              << std::endl;

    int128_t result = a * b;

    std::cout << "result.high = 0x" << std::hex << result.high() << std::endl;
    std::cout << "result.low = 0x" << result.low() << std::dec << std::endl;
    std::cout << "result = " << result.to_string() << std::endl;

    // Verificar contra resultado esperado
    int128_t expected(-5000000ll);
    std::cout << "\nexpected.high = 0x" << std::hex << expected.high() << std::endl;
    std::cout << "expected.low = 0x" << expected.low() << std::dec << std::endl;
    std::cout << "expected = " << expected.to_string() << std::endl;

    std::cout << "\nresult == expected? " << (result == expected ? "YES" : "NO") << std::endl;

    // Test 2: Verificar paso a paso
    std::cout << "\n=== Calculo manual ===" << std::endl;
    // 1000000 * (-5) = -5000000
    // En complemento a 2: -5000000 = 0xFFFFFFFFFFFFFFFF'FFFFFFFFFB3B4C00
    // Donde -5000000 en hex es: 0xFFB3B4C0 (32 bits) -> extendido a 64 bits con signo
    int64_t manual = 1000000ll * (-5ll);
    std::cout << "1000000 * (-5) usando int64_t = " << manual << std::endl;
    std::cout << "En hex: 0x" << std::hex << static_cast<uint64_t>(manual) << std::dec << std::endl;

    return (result == expected) ? 0 : 1;
}
