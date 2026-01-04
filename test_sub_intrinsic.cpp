#include "include_new/intrinsics/arithmetic_operations.hpp"
#include <cstdint>
#include <iostream>

int main()
{
    uint64_t a = 100;
    uint64_t b = 30;
    uint64_t result = 0;

    std::cout << "a = " << a << ", b = " << b << std::endl;

    unsigned char borrow = intrinsics::sub_u64(a, b, &result);

    std::cout << "result = " << result << std::endl;
    std::cout << "borrow = " << (int)borrow << std::endl;
    std::cout << "Esperado: result = 70, borrow = 0" << std::endl;

    return 0;
}
