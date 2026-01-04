#include "int128_base.hpp"
#include <iomanip>
#include <iostream>

using uint128_t = nstd::int128_base_t<nstd::signedness::unsigned_type>;
using int128_t = nstd::int128_base_t<nstd::signedness::signed_type>;

void print_hex(const char* label, const auto& value)
{
    std::cout << label << ": 0x" << std::hex << value.high() << "_" << value.low() << std::dec
              << "\n";
}

int main()
{
    std::cout << "=== TEST: Operadores Shift con integral_builtin ===\n\n";

    // Test 1: Left shift con diferentes tipos
    std::cout << "Test 1: Left shift con diferentes tipos\n";
    uint128_t a(0, 0xFF);
    print_hex("Valor inicial", a);

    a <<= 8u; // uint32_t
    print_hex("Despues de <<= 8u", a);
    std::cout << "Expected: 0x0_ff00, " << (a.low() == 0xFF00 ? "PASS" : "FAIL") << "\n\n";

    uint128_t b(0, 0x1);
    auto c = b << 64ULL; // uint64_t
    print_hex("0x1 << 64ULL", c);
    std::cout << "Expected: 0x1_0, " << (c.high() == 1 && c.low() == 0 ? "PASS" : "FAIL") << "\n\n";

    // Test 2: Right shift unsigned con diferentes tipos
    std::cout << "Test 2: Right shift unsigned\n";
    uint128_t d(0x1000, 0);
    print_hex("Valor inicial", d);

    d >>= 4u; // uint32_t
    print_hex("Despues de >>= 4u", d);
    std::cout << "Expected: 0x100_0, " << (d.high() == 0x100 && d.low() == 0 ? "PASS" : "FAIL")
              << "\n\n";

    uint128_t e(0x1, 0);
    auto f = e >> static_cast<uint8_t>(1); // uint8_t
    print_hex("0x1_0 >> 1", f);
    std::cout << "Expected: 0x0_8000000000000000, "
              << (f.high() == 0 && f.low() == 0x8000000000000000ULL ? "PASS" : "FAIL") << "\n\n";

    // Test 3: Right shift signed (arithmetic)
    std::cout << "Test 3: Right shift signed (arithmetic)\n";
    int128_t g(0x8000000000000000ULL, 0); // Valor negativo
    print_hex("Valor inicial (negativo)", g);

    g >>= 4ULL; // uint64_t
    print_hex("Despues de >>= 4ULL (arithmetic)", g);
    std::cout << "Expected: 0xf800000000000000_0, "
              << (g.high() == 0xf800000000000000ULL ? "PASS" : "FAIL") << "\n";
    std::cout << "Sign bit propagated: "
              << (static_cast<int64_t>(g.high()) < 0 ? "YES (PASS)" : "NO (FAIL)") << "\n\n";

    // Test 4: Shift con size_t (común en loops)
    std::cout << "Test 4: Shift con size_t\n";
    uint128_t h(0, 1);
    for (size_t i = 0; i < 8; ++i) {
        h <<= i; // size_t
    }
    print_hex("Despues de shifts acumulativos", h);

    // Test 5: Shift grande (>= 64)
    std::cout << "\nTest 5: Shift >= 64 bits\n";
    uint128_t i(0, 0xABCDEF);
    auto j = i << 80ULL;
    print_hex("0xABCDEF << 80", j);
    std::cout << "Expected: 0xabcdef0000_0, "
              << (j.high() == 0xABCDEF0000ULL && j.low() == 0 ? "PASS" : "FAIL") << "\n\n";

    // Test 6: Shift con signed shift amount
    std::cout << "Test 6: Shift con signed types\n";
    uint128_t k(0, 0xFF);
    k <<= static_cast<int16_t>(4);
    print_hex("0xFF << 4", k);
    std::cout << "Expected: 0x0_ff0, " << (k.low() == 0xFF0 ? "PASS" : "FAIL") << "\n\n";

    std::cout << "=== TESTS COMPLETADOS ===\n";
    return 0;
}
