#include "include_new/intrinsics/arithmetic_operations.hpp"
#include <iostream>

using namespace intrinsics;

int main()
{
    uint64_t result = 0;

    // Test directo de la función intrínseca
    std::cout << "Test intrinsics::add_u64(100, 200, &result)" << std::endl;
    unsigned char carry = add_u64(100, 200, &result);
    std::cout << "result = " << result << ", carry = " << (int)carry << std::endl;

    // Test con valores que causan carry
    result = 0;
    std::cout << "\nTest con carry: add_u64(0xFFFFFFFFFFFFFFFF, 1, &result)" << std::endl;
    carry = add_u64(0xFFFFFFFFFFFFFFFFULL, 1, &result);
    std::cout << "result = " << result << ", carry = " << (int)carry << std::endl;
    std::cout << "Expected: result = 0, carry = 1" << std::endl;

    return 0;
}
