#include "include/uint128_t.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

// Función de referencia usando aritmética de 64 bits manual para verificar
uint64_t reference_fullmult_times_uint64(uint64_t high, uint64_t low, uint64_t multiplier)
{
    // Implementación de referencia usando descomposición manual
    const uint64_t mask32 = 0xFFFFFFFFULL;

    // Descomponer en partes de 32 bits
    uint64_t a0 = low & mask32;
    uint64_t a1 = low >> 32;
    uint64_t a2 = high & mask32;
    uint64_t a3 = high >> 32;

    uint64_t b0 = multiplier & mask32;
    uint64_t b1 = multiplier >> 32;

    // Calcular todos los productos parciales
    uint64_t p00 = a0 * b0;
    uint64_t p01 = a0 * b1;
    uint64_t p10 = a1 * b0;
    uint64_t p11 = a1 * b1;
    uint64_t p20 = a2 * b0;
    uint64_t p21 = a2 * b1;
    uint64_t p30 = a3 * b0;
    uint64_t p31 = a3 * b1;

    // Construir el resultado de 192 bits en partes de 64 bits
    uint64_t low64 = p00;

    uint64_t mid64 = (p00 >> 32) + (p01 & mask32) + (p10 & mask32);
    mid64 += (low64 >> 32); // Cualquier carry de low64

    uint64_t high64 = p11 + (p01 >> 32) + (p10 >> 32) + p20;
    high64 += (mid64 >> 32);

    uint64_t highest64 = p21 + p30 + (p20 >> 32);
    highest64 += (high64 >> 32);

    uint64_t overflow = p31 + (p21 >> 32) + (p30 >> 32);
    overflow += (highest64 >> 32);

    return overflow;
}

void test_basic_cases()
{
    std::cout << "🧪 Test casos básicos..." << std::endl;

    // Caso 1: Multiplicar por 0
    uint128_t zero(0, 0);
    assert(zero.fullmult_times_uint64(0xFFFFFFFFFFFFFFFFULL) == 0);
    assert(zero.fullmult_times_uint64(1) == 0);
    std::cout << "✓ Multiplicación por 0" << std::endl;

    // Caso 2: Multiplicar 0 por algo
    uint128_t max_val(UINT64_MAX, UINT64_MAX);
    assert(max_val.fullmult_times_uint64(0) == 0);
    std::cout << "✓ Multiplicar 0 por número" << std::endl;

    // Caso 3: Multiplicar por 1 (no debería haber overflow para números < 2^128)
    uint128_t test1(0x8000000000000000ULL, 0); // 2^127
    assert(test1.fullmult_times_uint64(1) == 0);

    uint128_t test2(UINT64_MAX, 0);
    assert(test2.fullmult_times_uint64(1) == 0);
    std::cout << "✓ Multiplicación por 1" << std::endl;

    // Caso 4: Casos que deberían generar overflow
    uint128_t large(UINT64_MAX, UINT64_MAX); // 2^128 - 1
    uint64_t result = large.fullmult_times_uint64(2);
    assert(result == 1); // (2^128 - 1) * 2 = 2^129 - 2, overflow = 1
    std::cout << "✓ Overflow con máximo valor" << std::endl;
}

void test_specific_values()
{
    std::cout << "\n🎯 Test valores específicos..." << std::endl;

    // Test 1: Valor conocido
    uint128_t test1(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t mult1 = 0x1000000000000000ULL; // 2^60

    uint64_t result1 = test1.fullmult_times_uint64(mult1);
    uint64_t expected1 =
        reference_fullmult_times_uint64(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL, mult1);

    std::cout << "Test 1: 0x" << std::hex << std::setfill('0') << std::setw(16)
              << 0x123456789ABCDEF0ULL << std::setw(16) << 0xFEDCBA0987654321ULL << " * 0x"
              << std::setw(16) << mult1 << std::endl;
    std::cout << "Resultado: 0x" << std::setw(16) << result1 << " | Esperado: 0x" << std::setw(16)
              << expected1 << std::endl;

    assert(result1 == expected1);
    std::cout << "✓ Test valor específico 1" << std::endl;

    // Test 2: Otro valor
    uint128_t test2(0x8000000000000000ULL, 0x8000000000000000ULL);
    uint64_t mult2 = 0x8000000000000000ULL;

    uint64_t result2 = test2.fullmult_times_uint64(mult2);
    uint64_t expected2 =
        reference_fullmult_times_uint64(0x8000000000000000ULL, 0x8000000000000000ULL, mult2);

    assert(result2 == expected2);
    std::cout << "✓ Test valor específico 2" << std::endl;
}

void test_random_values()
{
    std::cout << "\n🎲 Test valores aleatorios..." << std::endl;

    std::mt19937_64 rng(12345); // Seed fijo para reproducibilidad

    const int num_tests = 1000;
    int passed = 0;

    for (int i = 0; i < num_tests; ++i) {
        uint64_t high = rng();
        uint64_t low = rng();
        uint64_t multiplier = rng();

        uint128_t test_val(high, low);
        uint64_t result = test_val.fullmult_times_uint64(multiplier);
        uint64_t expected = reference_fullmult_times_uint64(high, low, multiplier);

        if (result == expected) {
            passed++;
        } else {
            std::cout << "❌ Fallo en test " << i << ":" << std::endl;
            std::cout << "  Input: 0x" << std::hex << high << low << " * 0x" << multiplier
                      << std::endl;
            std::cout << "  Got: 0x" << result << " | Expected: 0x" << expected << std::endl;
            assert(false);
        }

        if (i > 0 && i % 100 == 0) {
            std::cout << "  Progreso: " << std::dec << i << "/" << num_tests << " tests"
                      << std::endl;
        }
    }

    std::cout << "✓ " << passed << "/" << num_tests << " tests aleatorios pasados" << std::endl;
}

void test_edge_cases()
{
    std::cout << "\n🔍 Test casos límite..." << std::endl;

    // Caso límite 1: Solo parte alta
    uint128_t high_only(0x123456789ABCDEF0ULL, 0);
    uint64_t mult = 0xFEDCBA0987654321ULL;

    uint64_t result = high_only.fullmult_times_uint64(mult);
    uint64_t expected = reference_fullmult_times_uint64(0x123456789ABCDEF0ULL, 0, mult);
    assert(result == expected);
    std::cout << "✓ Solo parte alta" << std::endl;

    // Caso límite 2: Solo parte baja
    uint128_t low_only(0, 0x123456789ABCDEF0ULL);
    result = low_only.fullmult_times_uint64(mult);
    expected = reference_fullmult_times_uint64(0, 0x123456789ABCDEF0ULL, mult);
    assert(result == expected);
    std::cout << "✓ Solo parte baja" << std::endl;

    // Caso límite 3: Potencias de 2
    uint128_t pow2_test(0x4000000000000000ULL, 0); // 2^126
    result = pow2_test.fullmult_times_uint64(4);   // * 2^2 = 2^128 = overflow de 1
    assert(result == 1);
    std::cout << "✓ Potencias de 2" << std::endl;
}

void benchmark_performance()
{
    std::cout << "\n⚡ Benchmark de rendimiento..." << std::endl;

    std::mt19937_64 rng(54321);
    const int iterations = 1000000;

    uint128_t test_vals[100];
    uint64_t multipliers[100];

    // Preparar datos
    for (int i = 0; i < 100; ++i) {
        test_vals[i] = uint128_t(rng(), rng());
        multipliers[i] = rng();
    }

    auto start = std::chrono::high_resolution_clock::now();

    volatile uint64_t dummy_result = 0;
    for (int i = 0; i < iterations; ++i) {
        int idx = i % 100;
        dummy_result += test_vals[idx].fullmult_times_uint64(multipliers[idx]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    double avg_time = static_cast<double>(duration.count()) / iterations;
    std::cout << "✓ Tiempo promedio: " << std::fixed << std::setprecision(2) << avg_time
              << " ns por operación" << std::endl;
    std::cout << "✓ Dummy result (para evitar optimización): " << std::hex << dummy_result
              << std::endl;
}

int main()
{
    std::cout << "🚀 TEST FULLMULT_TIMES_UINT64" << std::endl;
    std::cout << "==============================" << std::endl;

    try {
        test_basic_cases();
        test_specific_values();
        test_random_values();
        test_edge_cases();
        benchmark_performance();

        std::cout << "\n🎉 ¡Todos los tests pasaron!" << std::endl;
        std::cout << "✅ La función fullmult_times_uint64 es correcta y eficiente" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Error desconocido" << std::endl;
        return 1;
    }
}