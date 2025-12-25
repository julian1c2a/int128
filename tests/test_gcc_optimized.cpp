#include "include/uint128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

// Función de referencia CORRECTA para verificar correctitud
uint64_t reference_fullmult_times_uint64(uint64_t high, uint64_t low, uint64_t multiplier)
{
    // Implementación usando __uint128_t si está disponible para máxima precisión
#if defined(__SIZEOF_INT128__)
    __uint128_t value = (static_cast<__uint128_t>(high) << 64) | low;

    // Para obtener la parte alta de uint128_t * uint64_t necesitamos aritmética de precisión
    // extendida Separamos en dos multiplicaciones: high*multiplier y low*multiplier
    __uint128_t low_result = static_cast<__uint128_t>(low) * multiplier;
    __uint128_t high_result = static_cast<__uint128_t>(high) * multiplier;

    // Parte alta de low_result contribuye a la parte media
    uint64_t low_high = static_cast<uint64_t>(low_result >> 64);

    // Parte baja de high_result también contribuye a la parte media
    uint64_t high_low = static_cast<uint64_t>(high_result & 0xFFFFFFFFFFFFFFFFULL);

    // Sumar con carry
    uint64_t sum = low_high + high_low;
    uint64_t carry = (sum < low_high) ? 1 : 0;

    // La parte alta final
    uint64_t high_high = static_cast<uint64_t>(high_result >> 64);

    return high_high + carry;
#else
    // Fallback manual aritmético correctamente implementado
    auto mul64x64 = [](uint64_t a, uint64_t b) -> std::pair<uint64_t, uint64_t> {
        uint64_t a_low = a & 0xFFFFFFFFULL;
        uint64_t a_high = a >> 32;
        uint64_t b_low = b & 0xFFFFFFFFULL;
        uint64_t b_high = b >> 32;

        uint64_t p0 = a_low * b_low;
        uint64_t p1 = a_low * b_high;
        uint64_t p2 = a_high * b_low;
        uint64_t p3 = a_high * b_high;

        uint64_t middle = p1 + (p0 >> 32) + (p2 & 0xFFFFFFFFULL);

        uint64_t result_low = (middle << 32) | (p0 & 0xFFFFFFFFULL);
        uint64_t result_high = p3 + (middle >> 32) + (p2 >> 32);

        return {result_high, result_low};
    };

    // Calcular low * multiplier
    auto low_mul = mul64x64(low, multiplier);

    // Calcular high * multiplier
    auto high_mul = mul64x64(high, multiplier);

    // El resultado final es: high_mul.high + carry desde (low_mul.high + high_mul.low)
    uint64_t intermediate_sum = low_mul.first + high_mul.second;
    uint64_t carry = (intermediate_sum < low_mul.first) ? 1 : 0;

    return high_mul.first + carry;
#endif
}

int main()
{
    std::cout << "=== BENCHMARK INTRÍNSECOS GCC/CLANG OPTIMIZADO ===" << std::endl;

    // Verificar correctitud con casos de prueba
    std::cout << "\n--- VERIFICACIÓN DE CORRECTITUD ---" << std::endl;

    struct TestCase {
        uint64_t high, low, multiplier;
        const char* description;
    };

    TestCase test_cases[] = {
        {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 2ULL, "Caso crítico: (2^128-1)*2"},
        {0x8000000000000000ULL, 0x0000000000000000ULL, 2ULL, "2^127 * 2 = 2^128"},
        {0x0000000000000001ULL, 0x0000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, "2^64 * (2^64-1)"},
        {0xFFFFFFFFFFFFFFFFULL, 0x0000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL,
         "Máximo alto * máximo"},
        {0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL, 0x123456789ABCDEF0ULL, "Valores aleatorios"},
        {0, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "Solo parte baja máxima"},
        {0, 0, 0xFFFFFFFFFFFFFFFFULL, "Multiplicación por cero"},
        {0x1ULL, 0x0ULL, 0x1ULL, "Casos simples"},
    };

    bool all_correct = true;

    for (const auto& tc : test_cases) {
        uint128_t value(tc.high, tc.low);
        uint64_t optimized_result = value.fullmult_times_uint64(tc.multiplier);
        uint64_t reference_result = reference_fullmult_times_uint64(tc.high, tc.low, tc.multiplier);

        bool correct = (optimized_result == reference_result);
        all_correct &= correct;

        std::cout << tc.description << ": ";
        std::cout << (correct ? "[OK] CORRECTO" : "[ERROR] ERROR") << std::endl;

        if (!correct) {
            std::cout << "  Optimizado: 0x" << std::hex << optimized_result << std::endl;
            std::cout << "  Referencia: 0x" << std::hex << reference_result << std::endl;
        }
    }

    if (all_correct) {
        std::cout << "\n🎉 TODAS LAS PRUEBAS CORRECTAS 🎉" << std::endl;
    } else {
        std::cout << "\n[FAIL] HAY ERRORES EN LA IMPLEMENTACIÓN" << std::endl;
        return 1;
    }

    // Benchmark de rendimiento
    std::cout << "\n--- BENCHMARK DE RENDIMIENTO ---" << std::endl;

    const int iterations = 10000000;
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
    std::cout << "Tiempo total: " << elapsed.count() << " ns" << std::endl;
    std::cout << "Tiempo por operación: " << std::fixed << std::setprecision(2) << ns_per_op
              << " ns" << std::endl;

    std::cout << "Rendimiento: ";
    if (ns_per_op < 1.0) {
        std::cout << "EXCELENTE (<1ns)";
    } else if (ns_per_op < 2.0) {
        std::cout << "MUY BUENO (<2ns)";
    } else if (ns_per_op < 5.0) {
        std::cout << "BUENO (<5ns)";
    } else {
        std::cout << "ACEPTABLE (≥5ns)";
    }
    std::cout << std::endl;

    // Información de compilación
    std::cout << "\n--- INFORMACIÓN DE COMPILACIÓN ---" << std::endl;

#if defined(__GNUC__) && !defined(__clang__)
    std::cout << "Compilador: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "."
              << __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(__clang__)
    std::cout << "Compilador: Clang " << __clang_major__ << "." << __clang_minor__ << "."
              << __clang_patchlevel__ << std::endl;
#endif

#if defined(__x86_64__)
    std::cout << "Arquitectura: x86_64" << std::endl;
#endif

#if defined(__BMI2__)
    std::cout << "BMI2: Disponible [OK]" << std::endl;
#else
    std::cout << "BMI2: No disponible [ERROR]" << std::endl;
#endif

#if defined(__SIZEOF_INT128__)
    std::cout << "__uint128_t: Disponible [OK]" << std::endl;
#else
    std::cout << "__uint128_t: No disponible [ERROR]" << std::endl;
#endif

    std::cout << "\n=== BENCHMARK COMPLETADO ===" << std::endl;
    return 0;
}