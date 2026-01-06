#include "int128_base.hpp"
#include <iomanip>
#include <iostream>

using uint128_t = nstd::int128_base_t<nstd::signedness::unsigned_type>;
using int128_t = nstd::int128_base_t<nstd::signedness::signed_type>;

void print_result(const char* expr, const auto& result, const auto& expected)
{
    std::cout << expr << " = " << result.low() << " (expected: " << expected << ") ";
    if (result.low() == expected && result.high() == 0) {
        std::cout << "\u2713 PASS\n";
    } else {
        std::cout << "\u2717 FAIL\n";
    }
}

int main()
{
    std::cout << "=== TEST: Operadores Bitwise con integral_builtin ===\n\n";

    // Test 1: AND con uint32_t
    std::cout << "Test 1: Bitwise AND\n";
    uint128_t a(0xFF);
    auto result1 = a & 0x0Fu;
    print_result("0xFF & 0x0F", result1, 0x0F);

    uint128_t b(0xAAAA);
    b &= 0x5555u;
    print_result("0xAAAA &= 0x5555", b, 0);

    // Test 2: OR con uint32_t
    std::cout << "\nTest 2: Bitwise OR\n";
    uint128_t c(0xF0);
    auto result2 = c | 0x0Fu;
    print_result("0xF0 | 0x0F", result2, 0xFF);

    uint128_t d(0x1000);
    d |= 0x0020u;
    print_result("0x1000 |= 0x0020", d, 0x1020);

    // Test 3: XOR con uint32_t
    std::cout << "\nTest 3: Bitwise XOR\n";
    uint128_t e(0xFFFF);
    auto result3 = e ^ 0xFFFFu;
    print_result("0xFFFF ^ 0xFFFF", result3, 0);

    uint128_t f(0xAAAA);
    f ^= 0xFFFFu;
    print_result("0xAAAA ^= 0xFFFF", f, 0x5555);

    // Test 4: Operaciones con int (signed)
    std::cout << "\nTest 4: Bitwise con signed types\n";
    uint128_t g(0xFF);
    auto result4 = g & -1; // -1 es 0xFFFFFFFFFFFFFFFF en complemento a 2
    print_result("0xFF & (-1)", result4, 0xFF);

    // Test 5: Operaciones con diferentes tamanos
    std::cout << "\nTest 5: Bitwise con diferentes tamanos\n";
    uint128_t h(0x123456789ABCDEFULL);
    auto result5 = h & 0xFFULL;
    print_result("0x123456789ABCDEF & 0xFF", result5, 0xEF);

    uint128_t i(0x1000000000000000ULL);
    i |= 0x1ULL;
    std::cout << "0x1000000000000000 |= 0x1 = " << std::hex << i.low()
              << " (expected: 0x1000000000000001) ";
    if (i.low() == 0x1000000000000001ULL) {
        std::cout << "\u2713 PASS\n";
    } else {
        std::cout << "\u2717 FAIL\n";
    }

    // Test 6: int128_t con signed values
    std::cout << std::dec << "\nTest 6: int128_t con valores signed\n";
    int128_t j(-100);
    j &= 0xFFu; // Mascara solo los ultimos 8 bits
    std::cout << "(-100) &= 0xFF = " << static_cast<int64_t>(j.low()) << " (expected: 156) ";
    if (j.low() == 156) {
        std::cout << "\u2713 PASS\n";
    } else {
        std::cout << "\u2717 FAIL\n";
    }

    std::cout << "\n=== TESTS COMPLETADOS ===\n";
    return 0;
}
