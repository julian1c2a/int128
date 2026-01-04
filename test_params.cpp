#include <iostream>

int main() {
    unsigned long long a = 100, b = 30, r = 999;
    unsigned char borrow1 = __builtin_subcll(a, b, 0, &r);
    std::cout << "subcll(100, 30, 0, &r): result = " << r << ", borrow = " << (int)borrow1 << std::endl;
    
    r = 999;
    unsigned char borrow2 = __builtin_subcll(b, a, 0, &r);
    std::cout << "subcll(30, 100, 0, &r): result = " << r << ", borrow = " << (int)borrow2 << std::endl;
    
    return 0;
}
