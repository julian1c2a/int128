/**
 * ============================================================================
 * SHOWCASE: PERFORMANCE - Análisis de Rendimiento
 * ============================================================================
 *
 * Este showcase compara el rendimiento de uint128_t con tipos nativos:
 * - Operaciones aritméticas básicas
 * - Operaciones bitwise
 * - Conversiones
 * - Operaciones STL
 * - Memory footprint
 *
 * Permite entender cuándo usar uint128_t vs tipos nativos.
 */

#include <algorithm>
#include <chrono>
#include <int128.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <uint128/uint128_iostreams.hpp>
#include <vector>

using namespace nstd;

using namespace uint128_literals;
using namespace std::chrono;

// Colores ANSI
const char* RESET = "\033[0m";
const char* CYAN = "\033[36m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* MAGENTA = "\033[35m";
const char* RED = "\033[31m";

void print_header(const std::string& title)
{
    std::cout << "\n" << CYAN << "╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << title;
    for (size_t i = title.length(); i < 54; ++i)
        std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝" << RESET << "\n\n";
}

template <typename Func> double benchmark(Func&& func, int iterations)
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000.0; // ms
}

void demo_arithmetic_performance()
{
    print_header("DEMO 1: Rendimiento Aritmético");

    const int ITERATIONS = 1000000;

    std::cout << YELLOW << ">>> Comparación de operaciones básicas (" << ITERATIONS
              << " iteraciones)" << RESET << "\n\n";

    // uint64_t suma
    volatile uint64_t sum64 = 0;
    auto time_add64 = benchmark([&]() { sum64 += 12345; }, ITERATIONS);

    // uint128_t suma
    uint128_t sum128 = 0;
    auto time_add128 = benchmark([&]() { sum128 += 12345_u128; }, ITERATIONS);

    std::cout << "  SUMA:\n";
    std::cout << "    uint64_t:  " << GREEN << std::fixed << std::setprecision(3) << time_add64
              << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_add128 << " ms" << RESET;
    double ratio_add = time_add128 / time_add64;
    std::cout << "  (" << (ratio_add > 1 ? RED : GREEN) << ratio_add << "x" << RESET << ")\n\n";

    // Multiplicación
    volatile uint64_t prod64 = 1;
    auto time_mul64 = benchmark([&]() { prod64 = (prod64 * 123) % 1000000; }, ITERATIONS);

    uint128_t prod128 = 1;
    auto time_mul128 =
        benchmark([&]() { prod128 = (prod128 * 123_u128) % 1000000_u128; }, ITERATIONS);

    std::cout << "  MULTIPLICACIÓN:\n";
    std::cout << "    uint64_t:  " << GREEN << time_mul64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_mul128 << " ms" << RESET;
    double ratio_mul = time_mul128 / time_mul64;
    std::cout << "  (" << (ratio_mul > 1 ? RED : GREEN) << ratio_mul << "x" << RESET << ")\n\n";

    // División
    volatile uint64_t div64 = 1000000;
    auto time_div64 = benchmark([&]() { div64 = div64 / 7 + 1; }, ITERATIONS);

    uint128_t div128 = 1000000_u128;
    auto time_div128 = benchmark([&]() { div128 = div128 / 7_u128 + 1_u128; }, ITERATIONS);

    std::cout << "  DIVISIÓN:\n";
    std::cout << "    uint64_t:  " << GREEN << time_div64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_div128 << " ms" << RESET;
    double ratio_div = time_div128 / time_div64;
    std::cout << "  (" << (ratio_div > 1 ? RED : GREEN) << ratio_div << "x" << RESET << ")\n";
}

void demo_bitwise_performance()
{
    print_header("DEMO 2: Operaciones Bitwise");

    const int ITERATIONS = 1000000;

    std::cout << YELLOW << ">>> Comparación de operaciones bitwise" << RESET << "\n\n";

    // AND
    volatile uint64_t and64 = 0xFFFFFFFFFFFFFFFF;
    auto time_and64 = benchmark([&]() { and64 = and64 & 0x123456789ABCDEF0; }, ITERATIONS);

    uint128_t and128 = (uint128_t(0xFFFFFFFFFFFFFFFF) << 64) | 0xFFFFFFFFFFFFFFFF;
    auto time_and128 = benchmark([&]() { and128 = and128 & 0x123456789ABCDEF0_u128; }, ITERATIONS);

    std::cout << "  AND (&):\n";
    std::cout << "    uint64_t:  " << GREEN << std::fixed << std::setprecision(3) << time_and64
              << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_and128 << " ms" << RESET;
    std::cout << "  (" << (time_and128 > time_and64 ? RED : GREEN) << (time_and128 / time_and64)
              << "x" << RESET << ")\n\n";

    // Shift
    volatile uint64_t shl64 = 1;
    auto time_shl64 = benchmark([&]() { shl64 = (shl64 << 1) | 1; }, ITERATIONS);

    uint128_t shl128 = 1_u128;
    auto time_shl128 = benchmark([&]() { shl128 = (shl128 << 1) | 1_u128; }, ITERATIONS);

    std::cout << "  SHIFT (<<):\n";
    std::cout << "    uint64_t:  " << GREEN << time_shl64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_shl128 << " ms" << RESET;
    std::cout << "  (" << (time_shl128 > time_shl64 ? RED : GREEN) << (time_shl128 / time_shl64)
              << "x" << RESET << ")\n\n";

    // Popcount (contar bits)
    uint64_t test64 = 0xAAAAAAAAAAAAAAAA;
    auto time_pop64 = benchmark(
        [&]() {
            volatile int count = __builtin_popcountll(test64);
            (void)count;
        },
        ITERATIONS);

    uint128_t test128 = (uint128_t(0xAAAAAAAAAAAAAAAA) << 64) | 0xAAAAAAAAAAAAAAAA;
    auto time_pop128 = benchmark(
        [&]() {
            uint64_t low = test128.low();
            uint64_t high = test128.high();
            volatile int count = __builtin_popcountll(low) + __builtin_popcountll(high);
            (void)count;
        },
        ITERATIONS);

    std::cout << "  POPCOUNT:\n";
    std::cout << "    uint64_t:  " << GREEN << time_pop64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_pop128 << " ms" << RESET;
    std::cout << "  (" << (time_pop128 > time_pop64 ? RED : GREEN) << (time_pop128 / time_pop64)
              << "x" << RESET << ")\n";
}

void demo_stl_performance()
{
    print_header("DEMO 3: Operaciones STL");

    const int SIZE = 10000;

    std::cout << YELLOW << ">>> Comparación de algoritmos STL" << RESET << "\n\n";

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    // Crear vectores
    std::vector<uint64_t> vec64(SIZE);
    std::vector<uint128_t> vec128(SIZE);

    for (int i = 0; i < SIZE; ++i) {
        uint64_t val = dist(gen);
        vec64[i] = val;
        vec128[i] = uint128_t(val);
    }

    // Sort
    auto vec64_copy = vec64;
    auto time_sort64 = benchmark([&]() { std::sort(vec64_copy.begin(), vec64_copy.end()); }, 10);

    auto vec128_copy = vec128;
    auto time_sort128 = benchmark([&]() { std::sort(vec128_copy.begin(), vec128_copy.end()); }, 10);

    std::cout << "  SORT (" << SIZE << " elementos, 10 iteraciones):\n";
    std::cout << "    uint64_t:  " << GREEN << std::fixed << std::setprecision(2) << time_sort64
              << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_sort128 << " ms" << RESET;
    std::cout << "  (" << (time_sort128 > time_sort64 ? RED : GREEN) << (time_sort128 / time_sort64)
              << "x" << RESET << ")\n\n";

    // Accumulate
    auto time_acc64 = benchmark(
        [&]() {
            volatile uint64_t sum = std::accumulate(vec64.begin(), vec64.end(), 0ULL);
            (void)sum;
        },
        100);

    auto time_acc128 = benchmark(
        [&]() {
            uint128_t sum = std::accumulate(vec128.begin(), vec128.end(), 0_u128);
            (void)sum;
        },
        100);

    std::cout << "  ACCUMULATE (100 iteraciones):\n";
    std::cout << "    uint64_t:  " << GREEN << time_acc64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_acc128 << " ms" << RESET;
    std::cout << "  (" << (time_acc128 > time_acc64 ? RED : GREEN) << (time_acc128 / time_acc64)
              << "x" << RESET << ")\n\n";

    // Find
    uint64_t target64 = vec64[SIZE / 2];
    auto time_find64 = benchmark(
        [&]() {
            volatile auto it = std::find(vec64.begin(), vec64.end(), target64);
            (void)it;
        },
        1000);

    uint128_t target128 = vec128[SIZE / 2];
    auto time_find128 = benchmark(
        [&]() {
            volatile auto it = std::find(vec128.begin(), vec128.end(), target128);
            (void)it;
        },
        1000);

    std::cout << "  FIND (1000 iteraciones):\n";
    std::cout << "    uint64_t:  " << GREEN << time_find64 << " ms" << RESET << "\n";
    std::cout << "    uint128_t: " << GREEN << time_find128 << " ms" << RESET;
    std::cout << "  (" << (time_find128 > time_find64 ? RED : GREEN) << (time_find128 / time_find64)
              << "x" << RESET << ")\n";
}

void demo_memory_footprint()
{
    print_header("DEMO 4: Uso de Memoria");

    std::cout << YELLOW << ">>> Tamaño de tipos" << RESET << "\n\n";

    std::cout << "  sizeof(uint8_t):   " << GREEN << sizeof(uint8_t) << " bytes" << RESET << "\n";
    std::cout << "  sizeof(uint16_t):  " << GREEN << sizeof(uint16_t) << " bytes" << RESET << "\n";
    std::cout << "  sizeof(uint32_t):  " << GREEN << sizeof(uint32_t) << " bytes" << RESET << "\n";
    std::cout << "  sizeof(uint64_t):  " << GREEN << sizeof(uint64_t) << " bytes" << RESET << "\n";
    std::cout << "  sizeof(uint128_t): " << YELLOW << sizeof(uint128_t) << " bytes" << RESET
              << "\n\n";

    std::cout << YELLOW << ">>> Alineación" << RESET << "\n\n";

    std::cout << "  alignof(uint64_t):  " << GREEN << alignof(uint64_t) << " bytes" << RESET
              << "\n";
    std::cout << "  alignof(uint128_t): " << GREEN << alignof(uint128_t) << " bytes" << RESET
              << "\n\n";

    std::cout << YELLOW << ">>> Uso de memoria en vectores" << RESET << "\n\n";

    std::vector<int> sizes = {100, 1000, 10000, 100000};

    for (int size : sizes) {
        size_t mem64 = size * sizeof(uint64_t);
        size_t mem128 = size * sizeof(uint128_t);

        std::cout << "  Vector de " << std::setw(6) << size << " elementos:\n";
        std::cout << "    uint64_t:  " << GREEN << std::setw(8) << mem64 << " bytes" << RESET
                  << "\n";
        std::cout << "    uint128_t: " << YELLOW << std::setw(8) << mem128 << " bytes" << RESET;
        std::cout << "  (2x)\n\n";
    }
}

void demo_recommendations()
{
    print_header("DEMO 5: Recomendaciones de Uso");

    std::cout << GREEN << "✓ USAR uint128_t cuando:" << RESET << "\n\n";
    std::cout << "  • Necesitas valores > 2^64 - 1\n";
    std::cout << "  • Trabajas con direcciones IPv6\n";
    std::cout << "  • Implementas UUIDs (128 bits)\n";
    std::cout << "  • Calculas hashes de 128 bits\n";
    std::cout << "  • Necesitas precisión en cálculos científicos\n";
    std::cout << "  • Factoriales, combinatoria grande\n\n";

    std::cout << RED << "✗ NO usar uint128_t cuando:" << RESET << "\n\n";
    std::cout << "  • Los valores caben en uint64_t\n";
    std::cout << "  • Performance crítica en operaciones simples\n";
    std::cout << "  • Memoria es muy limitada\n";
    std::cout << "  • Solo necesitas 64 bits o menos\n\n";

    std::cout << YELLOW << "⚖  TRADE-OFFS:" << RESET << "\n\n";
    std::cout << "  • uint128_t es ~2-5x más lento que uint64_t\n";
    std::cout << "  • Usa 2x memoria (16 bytes vs 8 bytes)\n";
    std::cout << "  • Pero permite cálculos imposibles con uint64_t\n";
    std::cout << "  • La pérdida de performance suele ser aceptable\n";
    std::cout << "  • Para la mayoría de aplicaciones, no es cuello de botella\n";
}

int main()
{
    std::cout << CYAN << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║   ██████╗ ███████╗██████╗ ███████╗                           ║
    ║   ██╔══██╗██╔════╝██╔══██╗██╔════╝                           ║
    ║   ██████╔╝█████╗  ██████╔╝█████╗                             ║
    ║   ██╔═══╝ ██╔══╝  ██╔══██╗██╔══╝                             ║
    ║   ██║     ███████╗██║  ██║██║                                ║
    ║   ╚═╝     ╚══════╝╚═╝  ╚═╝╚═╝                                ║
    ║                                                               ║
    ║              SHOWCASE: Análisis de Rendimiento                ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Esta demo compara uint128_t con tipos nativos.\n";
    std::cout << "Permite entender cuándo usar uint128_t es apropiado.\n";

    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_arithmetic_performance();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_bitwise_performance();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_stl_performance();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_memory_footprint();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_recommendations();

    std::cout << "\n"
              << CYAN << R"(
╔═══════════════════════════════════════════════════════════════╗
║                   ⚡ DEMO COMPLETADA ⚡                       ║
╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Conclusión:\n";
    std::cout << "  • uint128_t es más lento que uint64_t (2-5x)\n";
    std::cout << "  • Pero permite cálculos imposibles con tipos nativos\n";
    std::cout << "  • Usa cuando NECESITAS 128 bits, no por defecto\n";

    return 0;
}

