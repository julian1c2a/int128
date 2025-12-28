/**
 * Tutorial 09: Algoritmos STL
 *
 * Aprenderás:
 * - sort, find, count
 * - accumulate, transform
 * - min/max_element
 * - Uso de uint128_t con algoritmos estándar
 */

#include <algorithm>
#include <int128.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <numeric>
#include <vector>

using namespace nstd;

using namespace uint128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 09: Algoritmos STL\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. SORT (ORDENAMIENTO)
    // ============================================================
    std::cout << "--- 1. Ordenamiento con std::sort ---\n";

    std::vector<uint128_t> numbers = {500_u128, 100_u128, 1000_u128, 250_u128, 750_u128};

    std::cout << "Antes:   ";
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << "\n";

    std::sort(numbers.begin(), numbers.end());

    std::cout << "Después: ";
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << "\n\n";

    // ============================================================
    // 2. FIND (BÚSQUEDA)
    // ============================================================
    std::cout << "--- 2. Búsqueda con std::find ---\n";

    auto target = 250_u128;
    auto it = std::find(numbers.begin(), numbers.end(), target);

    if (it != numbers.end()) {
        std::cout << "Encontrado " << target << " en posición "
                  << std::distance(numbers.begin(), it) << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 3. COUNT (CONTAR)
    // ============================================================
    std::cout << "--- 3. Contar con std::count ---\n";

    std::vector<uint128_t> data = {10_u128, 20_u128, 10_u128, 30_u128, 10_u128};
    auto count = std::count(data.begin(), data.end(), 10_u128);

    std::cout << "Ocurrencias de 10: " << count << "\n\n";

    // ============================================================
    // 4. ACCUMULATE (SUMA TOTAL)
    // ============================================================
    std::cout << "--- 4. Suma total con std::accumulate ---\n";

    auto sum = std::accumulate(numbers.begin(), numbers.end(), 0_u128);

    std::cout << "Suma total: " << sum << "\n\n";

    // ============================================================
    // 5. MIN/MAX ELEMENT
    // ============================================================
    std::cout << "--- 5. Mínimo y máximo ---\n";

    auto min_it = std::min_element(numbers.begin(), numbers.end());
    auto max_it = std::max_element(numbers.begin(), numbers.end());

    std::cout << "Mínimo: " << *min_it << "\n";
    std::cout << "Máximo: " << *max_it << "\n\n";

    // ============================================================
    // 6. TRANSFORM
    // ============================================================
    std::cout << "--- 6. Transformar con std::transform ---\n";

    std::vector<uint128_t> input = {1_u128, 2_u128, 3_u128, 4_u128, 5_u128};
    std::vector<uint128_t> output;

    // Multiplicar por 10
    std::transform(input.begin(), input.end(), std::back_inserter(output),
                   [](uint128_t n) { return n * 10; });

    std::cout << "Original:     ";
    for (const auto& n : input)
        std::cout << n << " ";
    std::cout << "\n";

    std::cout << "Transformado: ";
    for (const auto& n : output)
        std::cout << n << " ";
    std::cout << "\n\n";

    // ============================================================
    // 7. FILTER CON COPY_IF
    // ============================================================
    std::cout << "--- 7. Filtrar con std::copy_if ---\n";

    std::vector<uint128_t> all = {10_u128, 25_u128, 30_u128, 45_u128, 50_u128};
    std::vector<uint128_t> filtered;

    // Solo valores > 30
    std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                 [](uint128_t n) { return n > 30; });

    std::cout << "Filtrados (> 30): ";
    for (const auto& n : filtered)
        std::cout << n << " ";
    std::cout << "\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 09 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 10_numeric_functions.cpp\n";

    return 0;
}

