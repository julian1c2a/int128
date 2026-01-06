// Test rapido de divrem optimizado para tipos integrales builtin
#include "int128.hpp"
#include <cassert>
#include <chrono>
#include <iostream>

using namespace nstd;

void test_basic_divrem()
{
    std::cout << "=== Test Basico divrem(integral_builtin) ===\n";

    // Caso 1: Division por uint64_t
    uint128_t dividend(0x1234567890ABCDEF, 0xFEDCBA0987654321);
    uint64_t divisor = 0x123456;

    auto result = dividend.divrem(divisor);
    assert(result.has_value());

    auto [quotient, remainder] = result.value();

    // Verificar: quotient * divisor + remainder == dividend
    uint128_t verification = quotient * uint128_t(divisor) + remainder;
    assert(verification == dividend);

    std::cout << "OK Division por uint64_t: PASS\n";
    std::cout << "  Dividendo: 0x" << dividend.to_string_hex() << "\n";
    std::cout << "  Divisor:   0x" << std::hex << divisor << "\n";
    std::cout << "  Cociente:  0x" << quotient.to_string_hex() << "\n";
    std::cout << "  Resto:     0x" << remainder.to_string_hex() << "\n";

    // Caso 2: Division por uint32_t
    uint32_t divisor2 = 1000000;
    auto result2 = dividend.divrem(divisor2);
    assert(result2.has_value());

    auto [q2, r2] = result2.value();
    uint128_t verification2 = q2 * uint128_t(divisor2) + r2;
    assert(verification2 == dividend);

    std::cout << "OK Division por uint32_t: PASS\n";

    // Caso 3: Division por signed int64_t
    int64_t divisor3 = -12345;
    auto result3 = uint128_t(100000000000ULL).divrem(divisor3);
    assert(result3.has_value());

    std::cout << "OK Division por int64_t negativo: PASS\n";

    // Caso 4: Division por potencia de 2
    uint64_t divisor4 = 1024; // 2^10
    auto result4 = dividend.divrem(divisor4);
    assert(result4.has_value());

    auto [q4, r4] = result4.value();
    uint128_t verification4 = q4 * uint128_t(divisor4) + r4;
    assert(verification4 == dividend);

    std::cout << "OK Division por potencia de 2: PASS\n";

    // Caso 5: Division por 1
    auto result5 = dividend.divrem(1ULL);
    assert(result5.has_value());
    assert(result5.value().first == dividend);
    assert(result5.value().second == uint128_t(0));

    std::cout << "OK Division por 1: PASS\n";

    // Caso 6: Division por 0 (debe retornar nullopt)
    auto result6 = dividend.divrem(0ULL);
    assert(!result6.has_value());

    std::cout << "OK Division por 0 retorna nullopt: PASS\n";
}

void test_operators()
{
    std::cout << "\n=== Test Operadores /= y %= ===\n";

    uint128_t a(0x1000000000000000ULL, 0x5000000000000000ULL);
    uint64_t divisor = 123456789;

    uint128_t original_a = a;
    uint128_t q = a / divisor;
    uint128_t r = a % divisor;

    // Verificar: q * divisor + r == a
    uint128_t verification = q * uint128_t(divisor) + r;
    assert(verification == original_a);

    std::cout << "OK Operadores / y % coherentes: PASS\n";

    // Test con operadores compuestos
    uint128_t b = original_a;
    b /= divisor;
    assert(b == q);

    uint128_t c = original_a;
    c %= divisor;
    assert(c == r);

    std::cout << "OK Operadores /= y %= funcionan correctamente: PASS\n";
}

void benchmark_divrem()
{
    std::cout << "\n=== Benchmark divrem (integral vs uint128_t) ===\n";

    const int iterations = 1000000;
    uint128_t dividend(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);

    // Benchmark con uint64_t (optimizado)
    uint64_t divisor_u64 = 0x123456789ULL;
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        auto result = dividend.divrem(divisor_u64);
        (void)result; // Evitar optimizacion del compilador
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();

    // Benchmark con uint128_t (algoritmo general)
    uint128_t divisor_u128(0, divisor_u64);
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        auto result = dividend.divrem(divisor_u128);
        (void)result;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

    std::cout << "divrem(uint64_t):   " << duration1 << " μs (" << iterations << " iteraciones)\n";
    std::cout << "divrem(uint128_t):  " << duration2 << " μs (" << iterations << " iteraciones)\n";
    std::cout << "Speedup:            " << (double)duration2 / duration1 << "x\n";
}

int main()
{
    std::cout << "Test de divrem optimizado para tipos integrales builtin\n";
    std::cout << "========================================================\n\n";

    try {
        test_basic_divrem();
        test_operators();
        benchmark_divrem();

        std::cout << "\n========================================================\n";
        std::cout << "OK TODOS LOS TESTS PASARON\n";
        std::cout << "========================================================\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nFAIL ERROR: " << e.what() << "\n";
        return 1;
    }
}
