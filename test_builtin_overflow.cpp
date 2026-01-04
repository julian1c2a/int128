#include <iostream>

int main()
{
    unsigned long long result = 0;
    unsigned long long carry_in = 0;
    unsigned long long carry_out;

    // Test __builtin_uaddll_overflow (GCC moderna)
    std::cout << "Test __builtin_uaddll_overflow(100, 200, &result)" << std::endl;
    carry_out = __builtin_uaddll_overflow(100, 200, &result);
    std::cout << "result = " << result << ", carry_out = " << carry_out << std::endl;
    std::cout << "Expected: result = 300, carry_out = 0" << std::endl;

    // Test con overflow
    result = 0;
    std::cout << "\nTest __builtin_uaddll_overflow(0xFFFFFFFFFFFFFFFF, 1, &result)" << std::endl;
    carry_out = __builtin_uaddll_overflow(0xFFFFFFFFFFFFFFFFULL, 1, &result);
    std::cout << "result = " << result << ", carry_out = " << carry_out << std::endl;
    std::cout << "Expected: result = 0, carry_out = 1" << std::endl;

    return 0;
}
