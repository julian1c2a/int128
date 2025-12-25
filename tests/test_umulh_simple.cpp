#include "include/uint128_t.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== TEST SIMPLIFICADO DE EMULACIÓN __umulh ===" << std::endl;

    // Test básico: caso conocido (2^128-1)*2=1 (overflow)
    std::cout << "\n--- PRUEBA CASO CRÍTICO ---" << std::endl;
    uint128_t max_value(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    uint64_t result_critical = max_value.fullmult_times_uint64(2);
    std::cout << "Caso crítico (2^128-1)*2 = " << result_critical
              << (result_critical == 1 ? " [OK] CORRECTO" : " [ERROR] ERROR") << std::endl;

    // Test de rendimiento rápido
    std::cout << "\n--- BENCHMARK RÁPIDO ---" << std::endl;

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

    std::cout << "Tiempo por operación: " << std::fixed << std::setprecision(2) << ns_per_op
              << " ns" << std::endl;

    // Información de implementación
    std::cout << "\n--- IMPLEMENTACIÓN DETECTADA ---" << std::endl;

#ifdef _MSC_VER
    std::cout << "MSVC: Usando __umulh nativo" << std::endl;
#else
    std::cout << "GCC/Clang: Usando emulación optimizada" << std::endl;

#if defined(__x86_64__) && defined(__BMI2__)
    std::cout << "  → BMI2 _mulx_u64 disponible" << std::endl;
#elif defined(__SIZEOF_INT128__)
    std::cout << "  → __uint128_t nativo disponible" << std::endl;
#elif defined(__x86_64__)
    std::cout << "  → Inline assembly mulq disponible" << std::endl;
#else
    std::cout << "  → Fallback manual" << std::endl;
#endif

#if defined(__GNUC__) && !defined(__clang__)
    std::cout << "  → GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#elif defined(__clang__)
    std::cout << "  → Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
#endif
#endif

    // Evaluación del rendimiento
    std::cout << "\n--- EVALUACIÓN ---" << std::endl;
    std::cout << "MSVC __umulh (referencia): ~0.46 ns" << std::endl;
    std::cout << "Nuestra emulación:         " << std::fixed << std::setprecision(2) << ns_per_op
              << " ns" << std::endl;

    if (ns_per_op < 0.6) {
        std::cout << "🎯 EXCELENTE: Muy cerca del rendimiento de MSVC" << std::endl;
    } else if (ns_per_op < 1.5) {
        std::cout << "🚀 MUY BUENO: Emulación eficiente lograda" << std::endl;
    } else if (ns_per_op < 3.0) {
        std::cout << "[OK] BUENO: Rendimiento aceptable" << std::endl;
    } else {
        std::cout << "📈 MEJORABLE: Espacio para optimización" << std::endl;
    }

    double ratio = ns_per_op / 0.46;
    std::cout << "Factor vs MSVC: " << std::fixed << std::setprecision(1) << ratio << "x"
              << std::endl;

    std::cout << "\n=== ANÁLISIS COMPLETADO ===" << std::endl;
    return 0;
}