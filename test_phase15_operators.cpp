#include "include_new/int128_base.hpp"
#include <cassert>
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Testing Phase 1.5 Operators (after __builtin_uaddll_overflow fix) ==="
              << std::endl;

    // Test 1: Simple addition
    {
        uint128_t a(100);
        uint128_t b(200);
        uint128_t c = a + b;

        std::cout << "\nTest 1: 100 + 200" << std::endl;
        std::cout << "  Result: " << c.low() << std::endl;
        assert(c.low() == 300 && "100 + 200 should equal 300");
        assert(c.high() == 0 && "High part should be 0");
        std::cout << "  PASSED OK" << std::endl;
    }

    // Test 2: Addition with carry
    {
        uint128_t a(0, 0xFFFFFFFFFFFFFFFFULL); // high=0, low=max
        uint128_t b(0, 1);                     // high=0, low=1
        uint128_t c = a + b;

        std::cout << "\nTest 2: 0xFFFFFFFFFFFFFFFF + 1 (carry to high)" << std::endl;
        std::cout << "  Result: low=" << c.low() << ", high=" << c.high() << std::endl;
        assert(c.low() == 0 && "Low part should overflow to 0");
        assert(c.high() == 1 && "High part should receive carry");
        std::cout << "  PASSED OK" << std::endl;
    }

    // Test 3: Compound assignment
    {
        uint128_t a(100);
        a += uint128_t(200);

        std::cout << "\nTest 3: a=100; a += 200" << std::endl;
        std::cout << "  Result: " << a.low() << std::endl;
        assert(a.low() == 300 && "100 += 200 should equal 300");
        std::cout << "  PASSED OK" << std::endl;
    }

    // Test 4: Multiple operations
    {
        uint128_t a(10);
        uint128_t b(20);
        uint128_t c(30);
        uint128_t result = a + b + c;

        std::cout << "\nTest 4: 10 + 20 + 30" << std::endl;
        std::cout << "  Result: " << result.low() << std::endl;
        assert(result.low() == 60 && "10 + 20 + 30 should equal 60");
        std::cout << "  PASSED OK" << std::endl;
    }

    // Test 5: int128_t (signed) addition
    {
        int128_t a(100);
        int128_t b(200);
        int128_t c = a + b;

        std::cout << "\nTest 5: int128_t: 100 + 200" << std::endl;
        std::cout << "  Result: " << c.low() << std::endl;
        assert(c.low() == 300 && "int128_t: 100 + 200 should equal 300");
        std::cout << "  PASSED OK" << std::endl;
    }

    std::cout << "\n=== All Phase 1.5 Operator Tests PASSED OK ===" << std::endl;
    return 0;
}
