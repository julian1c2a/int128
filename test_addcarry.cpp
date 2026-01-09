// Test especifico para diagnosticar _addcarry_u64 en Intel ICX

#include <cstdint>
#include <iostream>

#if defined(_MSC_VER) || (defined(__INTEL_LLVM_COMPILER) && defined(_WIN32))
#include <intrin.h>
#endif

int main()
{
    std::cout << "=== Test _addcarry_u64 ===" << std::endl;

    std::cout << "\nMacros de compilador:" << std::endl;
#ifdef _MSC_VER
    std::cout << "_MSC_VER = " << _MSC_VER << std::endl;
#else
    std::cout << "_MSC_VER no definido" << std::endl;
#endif

#ifdef __INTEL_LLVM_COMPILER
    std::cout << "__INTEL_LLVM_COMPILER = " << __INTEL_LLVM_COMPILER << std::endl;
#else
    std::cout << "__INTEL_LLVM_COMPILER no definido" << std::endl;
#endif

#ifdef __cpp_lib_is_constant_evaluated
    std::cout << "__cpp_lib_is_constant_evaluated = " << __cpp_lib_is_constant_evaluated
              << std::endl;
#else
    std::cout << "__cpp_lib_is_constant_evaluated no definido" << std::endl;
#endif

    // Test 1: _addcarry_u64 directamente
    std::cout << "\nTest _addcarry_u64 directo:" << std::endl;
    {
        uint64_t a = 100;
        uint64_t b = 42;
        unsigned long long result = 0xDEADBEEF;
        unsigned char carry = _addcarry_u64(0, a, b, &result);
        std::cout << "a=" << a << ", b=" << b << std::endl;
        std::cout << "result=" << result << ", carry=" << (int)carry << std::endl;
        std::cout << "Esperado: result=142, carry=0" << std::endl;
        std::cout << "Test: " << (result == 142 ? "PASS" : "FAIL") << std::endl;
    }

    // Test 2: Con reinterpret_cast
    std::cout << "\nTest _addcarry_u64 con reinterpret_cast:" << std::endl;
    {
        uint64_t a = 100;
        uint64_t b = 42;
        uint64_t result = 0xDEADBEEF;
        unsigned char carry =
            _addcarry_u64(0, a, b, reinterpret_cast<unsigned long long*>(&result));
        std::cout << "a=" << a << ", b=" << b << std::endl;
        std::cout << "result=" << result << ", carry=" << (int)carry << std::endl;
        std::cout << "Esperado: result=142, carry=0" << std::endl;
        std::cout << "Test: " << (result == 142 ? "PASS" : "FAIL") << std::endl;
    }

    // Test 3: Implementacion portable
    std::cout << "\nTest implementacion portable:" << std::endl;
    {
        uint64_t a = 100;
        uint64_t b = 42;
        uint64_t sum = a + b;
        uint64_t sum_with_carry = sum + 0;
        uint64_t result = sum_with_carry;
        unsigned char carry = (sum < a) || (sum_with_carry < sum) ? 1 : 0;
        std::cout << "a=" << a << ", b=" << b << std::endl;
        std::cout << "result=" << result << ", carry=" << (int)carry << std::endl;
        std::cout << "Esperado: result=142, carry=0" << std::endl;
        std::cout << "Test: " << (result == 142 ? "PASS" : "FAIL") << std::endl;
    }

    return 0;
}
