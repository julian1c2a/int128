#include <iostream>

int main()
{
    unsigned long long result = 0;
    unsigned char carry_out;

    // Test directo de __builtin_addcll
    std::cout << "Test __builtin_addcll(100, 200, 0, &result)" << std::endl;
    carry_out = __builtin_addcll(100, 200, 0, &result);
    std::cout << "result = " << result << ", carry_out = " << (int)carry_out << std::endl;
    std::cout << "Expected: result = 300, carry_out = 0" << std::endl;

    // Test con carry
    result = 0;
    std::cout << "\nTest __builtin_addcll(0xFFFFFFFFFFFFFFFF, 1, 0, &result)" << std::endl;
    carry_out = __builtin_addcll(0xFFFFFFFFFFFFFFFFULL, 1, 0, &result);
    std::cout << "result = " << result << ", carry_out = " << (int)carry_out << std::endl;
    std::cout << "Expected: result = 0, carry_out = 1" << std::endl;

    return 0;
}
