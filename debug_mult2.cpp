#include "include_new/int128_base_tt.hpp"
#include <iostream>
using namespace nstd;
int main() {
    int128_t a(1000000ll);
    int64_t b = -5ll;
    int128_t result = a * b;
    std::cout << "a = " << a.to_string() << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "result = " << result.to_string() << std::endl;
    std::cout << "expected = -5000000" << std::endl;
    std::cout << "high = 0x" << std::hex << result.high() << std::endl;
    std::cout << "low = 0x" << std::hex << result.low() << std::endl;
    return 0;
}
