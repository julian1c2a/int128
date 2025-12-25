#include "include/uint128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>

// Definir manualmente para forzar inline assembly
#ifdef __SIZEOF_INT128__
#undef __SIZEOF_INT128__
#endif

int main()
{
    std::cout << "=== TEST INLINE ASSEMBLY (sin __uint128_t ni BMI2) ===" << std::endl;

    // Test básico: caso conocido
    std::cout << "\n--- PRUEBA CASO CRÍTICO ---" << std::endl;
    uint128_t max_value(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    uint64_t result_critical = max_value.fullmult_times_uint64(2);
    std::cout << "Caso crítico (2^128-1)*2 = " << result_critical
              << (result_critical == 1 ? " [OK] CORRECTO" : " [ERROR] ERROR") << std::endl;

    // Test de rendimiento
    std::cout << "\n--- BENCHMARK INLINE ASSEMBLY ---" << std::endl;

    const int iterations = 1000000;
    uint128_t test_value(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t multiplier = 0x555555555555555ULL;

    auto start = std::chrono::high_resolution_clock::now();

    volatile uint64_t dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += test_value.fullmult_times_uint64(multiplier + (i & 0xFF));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    double ns_per_op = static_cast<double>(elapsed.count()) / iterations;

    std::cout << "Inline Assembly: " << std::fixed << std::setprecision(2) << ns_per_op << " ns/op"
              << std::endl;

    std::cout << "MSVC __umulh:    ~0.46 ns/op (referencia)" << std::endl;
    std::cout << "Factor:          " << std::fixed << std::setprecision(1) << (ns_per_op / 0.46)
              << "x" << std::endl;

    return 0;
}