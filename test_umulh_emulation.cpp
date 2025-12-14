#include "include/uint128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

// Test específico para la función umulh_emulation
int main()
{
    std::cout << "=== ANÁLISIS DE EMULACIÓN __umulh ===" << std::endl;

    // Verificación de correctitud
    std::cout << "\n--- VERIFICACIÓN DE CORRECTITUD ---" << std::endl;

    struct TestCase {
        uint64_t a, b;
        const char* description;
    };

    TestCase test_cases[] = {
        {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "Máximo * Máximo"},
        {0x8000000000000000ULL, 2ULL, "2^63 * 2"},
        {0x1000000000000000ULL, 16ULL, "2^60 * 16"},
        {0x123456789ABCDEF0ULL, 0xFEDCBA0987654321ULL, "Valores aleatorios"},
        {1ULL, 0xFFFFFFFFFFFFFFFFULL, "1 * máximo"},
        {0xFFFFFFFFFFFFFFFFULL, 1ULL, "Máximo * 1"},
        {0ULL, 0xFFFFFFFFFFFFFFFFULL, "Cero * máximo"},
    };

    bool all_correct = true;

    for (const auto& tc : test_cases) {
        uint128_t value(0, tc.a); // Solo ponemos 'a' en la parte baja para test simple
        uint64_t result = value.fullmult_times_uint64(tc.b);

        // Calcular resultado esperado usando aritmética de alta precisión
        // Para uint128_t(0, a) * b, el resultado debería ser (a * b) >> 64
#if defined(__SIZEOF_INT128__)
        __uint128_t expected_full = static_cast<__uint128_t>(tc.a) * tc.b;
        uint64_t expected = static_cast<uint64_t>(expected_full >> 64);
#else
        // Fallback manual para verificación
        uint64_t a_lo = tc.a & 0xFFFFFFFFULL;
        uint64_t a_hi = tc.a >> 32;
        uint64_t b_lo = tc.b & 0xFFFFFFFFULL;
        uint64_t b_hi = tc.b >> 32;

        uint64_t p0 = a_lo * b_lo;
        uint64_t p1 = a_lo * b_hi;
        uint64_t p2 = a_hi * b_lo;
        uint64_t p3 = a_hi * b_hi;

        uint64_t middle = p1 + (p0 >> 32) + (p2 & 0xFFFFFFFFULL);
        uint64_t expected = p3 + (middle >> 32) + (p2 >> 32);
#endif

        bool correct = (result == expected);
        all_correct &= correct;

        std::cout << tc.description << ": ";
        std::cout << (correct ? "✓ CORRECTO" : "✗ ERROR") << std::endl;

        if (!correct) {
            std::cout << "  Resultado: 0x" << std::hex << result << std::endl;
            std::cout << "  Esperado:  0x" << std::hex << expected << std::endl;
        }
    }

    if (!all_correct) {
        std::cout << "\n❌ HAY ERRORES EN LA EMULACIÓN" << std::endl;
        return 1;
    }

    std::cout << "\n🎉 EMULACIÓN CORRECTA 🎉" << std::endl;

    // Benchmark de rendimiento
    std::cout << "\n--- BENCHMARK DE RENDIMIENTO ---" << std::endl;

    const int iterations = 5000000;
    std::mt19937_64 rng(42);

    // Preparar datos de prueba
    std::vector<uint128_t> test_values;
    std::vector<uint64_t> multipliers;
    test_values.reserve(1000);
    multipliers.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        test_values.emplace_back(rng(), rng());
        multipliers.push_back(rng());
    }

    // Calentar cache
    volatile uint64_t dummy = 0;
    for (int i = 0; i < 1000; ++i) {
        dummy += test_values[i % 1000].fullmult_times_uint64(multipliers[i % 1000]);
    }

    // Medir tiempo
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        dummy += test_values[i % 1000].fullmult_times_uint64(multipliers[i % 1000]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    double ns_per_op = static_cast<double>(elapsed.count()) / iterations;

    std::cout << "Iteraciones: " << iterations << std::endl;
    std::cout << "Tiempo por operación: " << std::fixed << std::setprecision(2) << ns_per_op
              << " ns" << std::endl;

    // Información de la implementación activa
    std::cout << "\n--- IMPLEMENTACIÓN ACTIVA ---" << std::endl;

#ifdef _MSC_VER
    std::cout << "Compilador: MSVC" << std::endl;
    std::cout << "Implementación: __umulh nativo (REFERENCIA)" << std::endl;
#else
#if defined(__x86_64__) && defined(__BMI2__)
    std::cout << "Implementación: BMI2 _mulx_u64 (emulación __umulh directa)" << std::endl;
#elif defined(__SIZEOF_INT128__)
    std::cout << "Implementación: __uint128_t nativo (emulación __umulh)" << std::endl;
#elif defined(__x86_64__)
    std::cout << "Implementación: Inline assembly mulq (emulación __umulh directa)" << std::endl;
#else
    std::cout << "Implementación: Fallback manual (emulación __umulh)" << std::endl;
#endif

#if defined(__GNUC__) && !defined(__clang__)
    std::cout << "Compilador: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#elif defined(__clang__)
    std::cout << "Compilador: Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
#endif
#endif

    std::cout << "\n--- COMPARACIÓN CON MSVC ---" << std::endl;
    std::cout << "• MSVC __umulh:      ~0.46 ns/op (REFERENCIA)" << std::endl;
    std::cout << "• Nuestra emulación: " << std::fixed << std::setprecision(2) << ns_per_op
              << " ns/op" << std::endl;

    if (ns_per_op < 0.5) {
        std::cout << "• Estado: EXCELENTE - Muy cerca de MSVC 🎯" << std::endl;
    } else if (ns_per_op < 1.0) {
        std::cout << "• Estado: MUY BUENO - Emulación eficiente 🚀" << std::endl;
    } else if (ns_per_op < 2.0) {
        std::cout << "• Estado: BUENO - Rendimiento aceptable ✓" << std::endl;
    } else {
        std::cout << "• Estado: MEJORABLE - Considerar más optimización 📈" << std::endl;
    }

    std::cout << "\n=== ANÁLISIS COMPLETADO ===" << std::endl;
    return 0;
}