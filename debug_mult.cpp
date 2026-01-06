#include "include_new/int128_base_tt.hpp"
#include <iostream>
using namespace nstd;
int main() {
    int128_t a(1000000ll);
    int64_t b = -5ll;
    int128_t result = a * b;
    std::cout << "a = " << a.to_string() << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "result = " << result.to_string() << "\n";
    std::cout << "expected = " << int128_t(-5000000ll).to_string() << "\n";
    std::cout << "result == expected: " << (result == int128_t(-5000000ll) ? "yes" : "no") << "\n";
    
    // Veamos los bits
    std::cout << "\nresult.high() = " << std::hex << result.high() << "\n";
    std::cout << "result.low() = " << std::hex << result.low() << "\n";
    
    int128_t expected(-5000000ll);
    std::cout << "expected.high() = " << std::hex << expected.high() << "\n";
    std::cout << "expected.low() = " << std::hex << expected.low() << "\n";
    return 0;
}
