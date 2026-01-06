// Test para verificar la encapsulacion de div128_64_composed
#include "int128.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test Encapsulation div128_64_composed ===\n\n";

    // Test 1: Caso simple (1000 / 3)
    uint128_t val1(0, 1000); // high=0, low=1000
    std::cout << "Test 1: 1000 / 3\n";
    std::cout << "  val1.high() = " << val1.high() << "\n";
    std::cout << "  val1.low() = " << val1.low() << "\n";

    auto res1 = val1.divrem(uint64_t(3));
    if (res1.has_value()) {
        auto [q, r] = res1.value();
        std::cout << "  Quotient: " << q.low() << "\n";
        std::cout << "  Remainder: " << r.low() << "\n";
        std::cout << "  Expected: 333 remainder 1\n";
        if (q.low() == 333 && r.low() == 1) {
            std::cout << "  [PASS]\n";
        } else {
            std::cout << "  [FAIL]\n";
        }
    } else {
        std::cout << "  [ERROR: nullopt]\n";
    }

    std::cout << "\n";

    // Test 2: Caso que activa general algorithm (high != 0)
    uint128_t val2(1, 0); // 2^64
    std::cout << "Test 2: 2^64 / 2\n";
    std::cout << "  val2.high() = " << val2.high() << "\n";
    std::cout << "  val2.low() = " << val2.low() << "\n";

    auto res2 = val2.divrem(uint64_t(2));
    if (res2.has_value()) {
        auto [q, r] = res2.value();
        std::cout << "  Quotient high: " << q.high() << "\n";
        std::cout << "  Quotient low: " << q.low() << "\n";
        std::cout << "  Remainder: " << r.low() << "\n";
        std::cout << "  Expected: 2^63 remainder 0\n";
        if (q.high() == 0 && q.low() == (1ULL << 63) && r.low() == 0) {
            std::cout << "  [PASS]\n";
        } else {
            std::cout << "  [FAIL]\n";
        }
    } else {
        std::cout << "  [ERROR: nullopt]\n";
    }

    return 0;
}
