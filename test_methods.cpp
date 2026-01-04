#include <iostream>

int main() {
    unsigned long long a = 100, b = 30;
    
    // Probar diferentes intrínsecos
    
    // 1. __builtin_sub_overflow
    unsigned long long r1;
    bool overflow1 = __builtin_sub_overflow(a, b, &r1);
    std::cout << "__builtin_sub_overflow(100, 30): result = " << r1 << ", overflow = " << overflow1 << std::endl;
    
    // 2. __builtin_subcll
    unsigned long long r2 = 999;
    unsigned char borrow = __builtin_subcll(a, b, 0, &r2);
    std::cout << "__builtin_subcll(100, 30, 0): result = " << r2 << ", borrow = " << (int)borrow << std::endl;
    
    return 0;
}
