#include <iostream>

int main() {
#if (defined(__GNUC__) && __GNUC__ >= 5) || (defined(__clang__) && __clang_major__ >= 3)
    unsigned long long a = 100, b = 30, r = 999;
    unsigned char borrow = __builtin_subcll(a, b, 0, &r);
    std::cout << "Usando __builtin_subcll" << std::endl;
    std::cout << "result = " << r << ", borrow = " << (int)borrow << std::endl;
#else
    std::cout << "NO using builtin" << std::endl;
#endif
    return 0;
}
