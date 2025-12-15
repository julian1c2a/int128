#include "include/uint128_t.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

// Función de referencia CORREGIDA usando la lógica matemáticamente correcta
uint64_t correct_reference_fullmult_times_uint64(uint64_t high, uint64_t low, uint64_t multiplier)
{
    // Función para multiplicar 64x64 -> 128 bits
    auto mul64x64 = [](uint64_t a, uint64_t b) -> std::pair<uint64_t, uint64_t> {
        uint64_t a_lo = a & 0xFFFFFFFFULL;
        uint64_t a_hi = a >> 32;
        uint64_t b_lo = b & 0xFFFFFFFFULL;
        uint64_t b_hi = b >> 32;

        uint64_t p0 = a_lo * b_lo;
        uint64_t p1 = a_lo * b_hi;
        uint64_t p2 = a_hi * b_lo;
        uint64_t p3 = a_hi * b_hi;

        uint64_t carry = (p0 >> 32) + (p1 & 0xFFFFFFFFULL) + (p2 & 0xFFFFFFFFULL);
        uint64_t result_low = p0 + ((p1 + p2) << 32);
        uint64_t result_high = p3 + (p1 >> 32) + (p2 >> 32) + (carry >> 32);

        return {result_low, result_high};
    };

    // low * multiplier (bits 0-127 del resultado)
    auto low_mult = mul64x64(low, multiplier);

    // high * multiplier (bits 64-191, desplazado 64 hacia la izquierda)
    auto high_mult = mul64x64(high, multiplier);

    // Los bits 128-191 vienen de:
    // 1. La parte alta de high * multiplier (bits 64-127 de high_mult van a 128-191)
    // 2. Cualquier carry de la suma en bits 64-127

    // Bits 64-127 del resultado = low_mult.second + high_mult.first
    uint64_t sum_bits_64_127 = low_mult.second + high_mult.first;
    uint64_t carry_to_128 = (sum_bits_64_127 < low_mult.second) ? 1 : 0;

    // Bits 128-191 del resultado = high_mult.second + carry_to_128
    return high_mult.second + carry_to_128;
}

void test_basic_cases()
{
    std::cout << "🧪 Test casos básicos..." << std::endl;

    // Test 1: Multiplicación por 0
    uint128_t zero(0, 0);
    uint64_t result = zero.fullmult_times_uint64(12345);
    assert(result == 0);
    std::cout << "✓ Multiplicación por 0" << std::endl;

    // Test 2: Multiplicar 0 por número
    uint128_t number(123, 456);
    result = number.fullmult_times_uint64(0);
    assert(result == 0);
    std::cout << "✓ Multiplicar 0 por número" << std::endl;

    // Test 3: Multiplicación por 1
    result = number.fullmult_times_uint64(1);
    assert(result == 0);
    std::cout << "✓ Multiplicación por 1" << std::endl;

    // Test 4: Overflow con máximo valor - CORREGIDO
    uint128_t large(UINT64_MAX, UINT64_MAX);
    result = large.fullmult_times_uint64(2);
    assert(result == 1); // (2^128 - 1) * 2 = 2^129 - 2 -> overflow = 1
    std::cout << "✓ Overflow con máximo valor" << std::endl;
}

void test_specific_values()
{
    std::cout << "🎯 Test valores específicos..." << std::endl;

    // Test 1: Caso específico
    uint128_t test1(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t mult1 = 0x1000000000000000ULL; // 2^60

    uint64_t result1 = test1.fullmult_times_uint64(mult1);
    uint64_t expected1 = correct_reference_fullmult_times_uint64(0x123456789ABCDEF0ULL,
                                                                 0xFEDCBA0987654321ULL, mult1);

    std::cout << "Test 1: 0x" << std::hex << 0x123456789ABCDEF0ULL << std::hex
              << 0xFEDCBA0987654321ULL << " * 0x" << mult1 << std::endl;
    std::cout << "             Resultado: 0x" << std::hex << std::setfill('0') << std::setw(16)
              << result1 << " | Esperado: 0x" << std::hex << std::setfill('0') << std::setw(16)
              << expected1 << std::endl;

    assert(result1 == expected1);

    // Test 2: Otro caso específico
    uint128_t test2(0x0000000000000001ULL, 0x0000000000000000ULL);
    uint64_t mult2 = UINT64_MAX;

    uint64_t result2 = test2.fullmult_times_uint64(mult2);
    uint64_t expected2 = correct_reference_fullmult_times_uint64(0x0000000000000001ULL,
                                                                 0x0000000000000000ULL, mult2);

    assert(result2 == expected2);

    std::cout << "✓ Todos los casos específicos pasaron" << std::endl;
}

void test_random_values()
{
    std::cout << "🎲 Test valores aleatorios..." << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    int passed = 0;
    int total = 1000;

    for (int i = 0; i < total; ++i) {
        uint64_t high = dis(gen);
        uint64_t low = dis(gen);
        uint64_t mult = dis(gen);

        if (mult == 0)
            mult = 1; // Evitar multiplicación por 0 para casos más interesantes

        uint128_t test_num(high, low);
        uint64_t result = test_num.fullmult_times_uint64(mult);
        uint64_t expected = correct_reference_fullmult_times_uint64(high, low, mult);

        if (result == expected) {
            passed++;
        } else {
            std::cout << "❌ Falló: high=0x" << std::hex << high << " low=0x" << low << " mult=0x"
                      << mult << " got 0x" << result << " expected 0x" << expected << std::endl;
            // Mostrar solo los primeros errores
            if ((total - passed) > 10)
                break;
        }
    }

    std::cout << "✓ Pasaron " << passed << "/" << total << " tests aleatorios" << std::endl;
    assert(passed == total);
}

void test_performance()
{
    std::cout << "⚡ Test de rendimiento..." << std::endl;

    uint128_t test_num(0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL);
    uint64_t mult = 0x1000000000000000ULL;

    const int iterations = 1000000;
    volatile uint64_t result; // volatile para evitar optimización

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        result = test_num.fullmult_times_uint64(mult);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    double ns_per_operation = static_cast<double>(duration.count()) / iterations;

    std::cout << "✓ Rendimiento: " << std::fixed << std::setprecision(2) << ns_per_operation
              << " ns por operación" << std::endl;
    std::cout << "  Resultado final: 0x" << std::hex << result << std::endl;
}

int main()
{
    std::cout << "🚀 TEST FULLMULT_TIMES_UINT64 CORREGIDO" << std::endl;
    std::cout << "==========================================" << std::endl;

    test_basic_cases();
    test_specific_values();
    test_random_values();
    test_performance();

    std::cout << "\n🎉 ¡Todos los tests pasaron!" << std::endl;
    return 0;
}