// Test para verificar is_constant_evaluated en Intel ICX

#include <cstdint>
#include <iostream>
#include <type_traits>

#if defined(_MSC_VER) || (defined(__INTEL_LLVM_COMPILER) && defined(_WIN32))
#include <intrin.h>
#endif

inline unsigned char my_addcarry(uint64_t a, uint64_t b, uint64_t* result)
{
    if (std::is_constant_evaluated()) {
        // Version constexpr portable
        std::cout << "  -> Usando version CONSTEXPR (portable)" << std::endl;
        uint64_t sum = a + b;
        *result = sum;
        return (sum < a) ? 1 : 0;
    } else {
        // Version runtime con intrinseco
        std::cout << "  -> Usando version RUNTIME (intrinseco)" << std::endl;
        return _addcarry_u64(0, a, b, reinterpret_cast<unsigned long long*>(result));
    }
}

int main()
{
    std::cout << "=== Test is_constant_evaluated ===" << std::endl;

    std::cout << "\nLlamando a my_addcarry(100, 42):" << std::endl;
    uint64_t result = 0;
    unsigned char carry = my_addcarry(100, 42, &result);
    std::cout << "result = " << result << std::endl;
    std::cout << "carry = " << (int)carry << std::endl;
    std::cout << "Test: " << (result == 142 ? "PASS" : "FAIL") << std::endl;

    return 0;
}
