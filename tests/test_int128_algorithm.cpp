#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// Incluir el archivo a probar
#include "int128/int128_algorithm.hpp"

void test_basic_algorithms()
{
    std::cout << "🔍 Probando algoritmos básicos...\n";

    std::vector<int128_t> data = {int128_t(-10), int128_t(5), int128_t(-3),
                                  int128_t(20),  int128_t(0), int128_t(15)};

    // Test búsqueda
    std::sort(data.begin(), data.end());
    bool found = int128_algorithm::binary_search_int128(data.begin(), data.end(), int128_t(5));
    assert(found);
    std::cout << "  [OK] binary_search_int128 funciona\n";

    // Test find_if
    auto it = int128_algorithm::find_if_int128(data.begin(), data.end(),
                                               [](const int128_t& x) { return x > int128_t(10); });
    assert(it != data.end() && *it == int128_t(15));
    std::cout << "  [OK] find_if_int128 funciona\n";
}

void test_reduction_algorithms()
{
    std::cout << "[INFO] Probando algoritmos de reducción...\n";

    std::vector<int128_t> data = {int128_t(1), int128_t(-2), int128_t(3), int128_t(-4),
                                  int128_t(5)};

    // Test suma
    int128_t sum = int128_algorithm::sum_int128(data.begin(), data.end());
    assert(sum == int128_t(3)); // 1-2+3-4+5 = 3
    std::cout << "  [OK] sum_int128 = " << sum << "\n";

    // Test producto
    int128_t product = int128_algorithm::product_int128(data.begin(), data.end());
    assert(product == int128_t(120)); // 1*(-2)*3*(-4)*5 = 120
    std::cout << "  [OK] product_int128 = " << product << "\n";
}

void test_specialized_algorithms()
{
    std::cout << "🧮 Probando algoritmos especializados...\n";

    std::vector<int128_t> data = {int128_t(12), int128_t(-18), int128_t(24)};

    // Test GCD (valores absolutos)
    int128_t gcd_result = int128_algorithm::gcd_range(data.begin(), data.end());
    assert(gcd_result == int128_t(6)); // GCD(12, 18, 24) = 6
    std::cout << "  [OK] gcd_range = " << gcd_result << "\n";

    // Test LCM (valores absolutos)
    int128_t lcm_result = int128_algorithm::lcm_range(data.begin(), data.end());
    assert(lcm_result == int128_t(72)); // LCM(12, 18, 24) = 72
    std::cout << "  [OK] lcm_range = " << lcm_result << "\n";
}

void test_sequence_generation()
{
    std::cout << "🔢 Probando generación de secuencias...\n";

    // Test secuencia aritmética
    std::vector<int128_t> arithmetic(5);
    int128_algorithm::generate_arithmetic_sequence(arithmetic.begin(), 5, int128_t(10),
                                                   int128_t(-2));
    // Debería ser: 10, 8, 6, 4, 2
    assert(arithmetic[0] == int128_t(10));
    assert(arithmetic[1] == int128_t(8));
    assert(arithmetic[4] == int128_t(2));
    std::cout << "  [OK] generate_arithmetic_sequence funciona\n";

    // Test secuencia geométrica
    std::vector<int128_t> geometric(4);
    int128_algorithm::generate_geometric_sequence(geometric.begin(), 4, int128_t(2), int128_t(-3));
    // Debería ser: 2, -6, 18, -54
    assert(geometric[0] == int128_t(2));
    assert(geometric[1] == int128_t(-6));
    assert(geometric[2] == int128_t(18));
    assert(geometric[3] == int128_t(-54));
    std::cout << "  [OK] generate_geometric_sequence funciona\n";
}

void test_signed_specific_algorithms()
{
    std::cout << "➕➖ Probando algoritmos específicos para signados...\n";

    std::vector<int128_t> data = {int128_t(-5), int128_t(3), int128_t(-10), int128_t(8),
                                  int128_t(0)};

    // Test partición por signo
    auto partition_point = int128_algorithm::partition_by_sign(data.begin(), data.end());
    // Verificar que los negativos están antes del punto de partición
    bool all_negative_before = std::all_of(data.begin(), partition_point,
                                           [](const int128_t& x) { return x < int128_t(0); });
    bool all_non_negative_after = std::all_of(partition_point, data.end(),
                                              [](const int128_t& x) { return x >= int128_t(0); });
    assert(all_negative_before && all_non_negative_after);
    std::cout << "  [OK] partition_by_sign funciona\n";

    // Test valor absoluto máximo
    int128_t max_abs = int128_algorithm::max_abs_value(data.begin(), data.end());
    assert(max_abs == int128_t(-10)); // El de mayor valor absoluto es -10
    std::cout << "  [OK] max_abs_value = " << max_abs << "\n";
}

void test_statistics()
{
    std::cout << "📈 Probando cálculo de estadísticas...\n";

    std::vector<int128_t> data = {int128_t(-5), int128_t(0), int128_t(10), int128_t(-2),
                                  int128_t(7)};

    auto stats = int128_algorithm::calculate_stats(data.begin(), data.end());

    assert(stats.count == 5);
    assert(stats.min_value == int128_t(-5));
    assert(stats.max_value == int128_t(10));
    assert(stats.sum == int128_t(10)); // -5+0+10-2+7 = 10
    assert(stats.negative_count == 2); // -5, -2
    assert(stats.positive_count == 2); // 10, 7
    assert(stats.zero_count == 1);     // 0
    assert(stats.mean() == 2.0);       // 10/5 = 2.0

    std::cout << "  [OK] calculate_stats funciona\n";
    std::cout << "    Min: " << stats.min_value << ", Max: " << stats.max_value << "\n";
    std::cout << "    Sum: " << stats.sum << ", Mean: " << stats.mean() << "\n";
    std::cout << "    Negativos: " << stats.negative_count
              << ", Positivos: " << stats.positive_count << ", Ceros: " << stats.zero_count << "\n";
}

void test_transformations()
{
    std::cout << "🔄 Probando transformaciones...\n";

    std::vector<int128_t> input = {int128_t(-3), int128_t(4), int128_t(-5)};
    std::vector<int128_t> output(3);

    // Transformar a valores absolutos
    int128_algorithm::transform_int128(input.begin(), input.end(), output.begin(),
                                       [](const int128_t& x) { return x.abs(); });

    assert(output[0] == int128_t(3));
    assert(output[1] == int128_t(4));
    assert(output[2] == int128_t(5));
    std::cout << "  [OK] transform_int128 (abs) funciona\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de int128_algorithm.hpp\n";
    std::cout << "=========================================\n\n";

    try {
        test_basic_algorithms();
        std::cout << "\n";

        test_reduction_algorithms();
        std::cout << "\n";

        test_specialized_algorithms();
        std::cout << "\n";

        test_sequence_generation();
        std::cout << "\n";

        test_signed_specific_algorithms();
        std::cout << "\n";

        test_statistics();
        std::cout << "\n";

        test_transformations();
        std::cout << "\n";

        std::cout << "🎉 ¡TODOS LOS TESTS DE ALGORITHM PASARON!\n";
        std::cout << "=========================================\n";
        std::cout << "[OK] Algoritmos de búsqueda funcionando\n";
        std::cout << "[OK] Algoritmos de reducción funcionando\n";
        std::cout << "[OK] Algoritmos especializados funcionando\n";
        std::cout << "[OK] Generación de secuencias funcionando\n";
        std::cout << "[OK] Algoritmos específicos de signados funcionando\n";
        std::cout << "[OK] Cálculo de estadísticas funcionando\n";
        std::cout << "[OK] Transformaciones funcionando\n";

        return 0;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error en tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "[FAIL] Error desconocido en tests" << std::endl;
        return 1;
    }
}