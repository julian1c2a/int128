#include "../include/uint128_ranges_clean.hpp"
#include "../include/uint128_t.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

void test_iota()
{
    std::cout << "🧪 Probando uint128_ranges::iota..." << std::endl;

    // Test con vector
    std::vector<uint128_t> vec(5);
    uint128_ranges::iota(vec.begin(), vec.end(), uint128_t(100));

    // Verificar valores: 100, 101, 102, 103, 104
    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == uint128_t(100 + i));
        std::cout << "  vec[" << i << "] = " << vec[i] << std::endl;
    }

    // Test con array usando std::iota (función de conveniencia)
    std::array<uint128_t, 3> arr;
    std::iota(arr.begin(), arr.end(), uint128_t(1000));

    std::cout << "  Array iota desde 1000: ";
    for (const auto& val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    assert(arr[0] == uint128_t(1000));
    assert(arr[1] == uint128_t(1001));
    assert(arr[2] == uint128_t(1002));

    std::cout << "✅ iota funcionando correctamente" << std::endl;
}

void test_accumulate()
{
    std::cout << "\n🧪 Probando uint128_ranges::accumulate..." << std::endl;

    std::vector<uint128_t> vec = {uint128_t(10), uint128_t(20), uint128_t(30), uint128_t(40)};

    // Suma simple
    auto sum = uint128_ranges::accumulate(vec.begin(), vec.end(), uint128_t(0));
    std::cout << "  Suma: " << sum << std::endl;
    assert(sum == uint128_t(100));

    // Suma con valor inicial
    auto sum_with_init = uint128_ranges::accumulate(vec.begin(), vec.end(), uint128_t(50));
    std::cout << "  Suma con inicial 50: " << sum_with_init << std::endl;
    assert(sum_with_init == uint128_t(150));

    // Producto usando lambda
    auto product = uint128_ranges::accumulate(vec.begin(), vec.end(), uint128_t(1),
                                              [](uint128_t a, uint128_t b) { return a * b; });
    std::cout << "  Producto: " << product << std::endl;
    assert(product == uint128_t(240000)); // 10*20*30*40 = 240000

    // Usar función de conveniencia std::accumulate
    auto sum_std = std::accumulate(vec.begin(), vec.end(), uint128_t(0));
    std::cout << "  Suma std: " << sum_std << std::endl;
    assert(sum_std == uint128_t(100));

    std::cout << "✅ accumulate funcionando correctamente" << std::endl;
}

void test_partial_sum()
{
    std::cout << "\n🧪 Probando uint128_ranges::partial_sum..." << std::endl;

    std::vector<uint128_t> input = {uint128_t(1), uint128_t(2), uint128_t(3), uint128_t(4),
                                    uint128_t(5)};
    std::vector<uint128_t> output(input.size());

    uint128_ranges::partial_sum(input.begin(), input.end(), output.begin());

    std::cout << "  Input:  ";
    for (const auto& val : input)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "  Output: ";
    for (const auto& val : output)
        std::cout << val << " ";
    std::cout << std::endl;

    // Verificar sumas parciales: 1, 3, 6, 10, 15
    assert(output[0] == uint128_t(1));
    assert(output[1] == uint128_t(3));
    assert(output[2] == uint128_t(6));
    assert(output[3] == uint128_t(10));
    assert(output[4] == uint128_t(15));

    std::cout << "✅ partial_sum funcionando correctamente" << std::endl;
}

void test_adjacent_difference()
{
    std::cout << "\n🧪 Probando uint128_ranges::adjacent_difference..." << std::endl;

    std::vector<uint128_t> input = {uint128_t(10), uint128_t(15), uint128_t(12), uint128_t(20)};
    std::vector<uint128_t> output(input.size());

    uint128_ranges::adjacent_difference(input.begin(), input.end(), output.begin());

    std::cout << "  Input:  ";
    for (const auto& val : input)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "  Output: ";
    for (const auto& val : output)
        std::cout << val << " ";
    std::cout << std::endl;

    // Verificar diferencias: 10, 5, muy_grande, 8
    assert(output[0] == uint128_t(10)); // Primer elemento se mantiene
    assert(output[1] == uint128_t(5));  // 15-10 = 5
    // output[2] será un número muy grande debido a uint128_t(12) - uint128_t(15)
    assert(output[3] == uint128_t(8)); // 20-12 = 8

    std::cout << "✅ adjacent_difference funcionando correctamente" << std::endl;
}

void test_inner_product()
{
    std::cout << "\n🧪 Probando uint128_ranges::inner_product..." << std::endl;

    std::vector<uint128_t> vec1 = {uint128_t(2), uint128_t(3), uint128_t(4)};
    std::vector<uint128_t> vec2 = {uint128_t(5), uint128_t(6), uint128_t(7)};

    auto result =
        uint128_ranges::inner_product(vec1.begin(), vec1.end(), vec2.begin(), uint128_t(0));

    std::cout << "  Vec1: ";
    for (const auto& val : vec1)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "  Vec2: ";
    for (const auto& val : vec2)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "  Producto interno: " << result << std::endl;

    // 2*5 + 3*6 + 4*7 = 10 + 18 + 28 = 56
    assert(result == uint128_t(56));

    std::cout << "✅ inner_product funcionando correctamente" << std::endl;
}

int main()
{
    std::cout << "=== TESTS DE FUNCIONES DE RANGO PARA uint128_t ===" << std::endl;

    try {
        test_iota();
        test_accumulate();
        test_partial_sum();
        test_adjacent_difference();
        test_inner_product();

        std::cout << "\n🎉 ¡Todas las funciones de rango funcionan correctamente!" << std::endl;
        std::cout << "✅ uint128_ranges::iota, accumulate, partial_sum," << std::endl;
        std::cout << "✅ adjacent_difference, inner_product" << std::endl;
        std::cout << "✅ Funciones de conveniencia std:: también funcionan" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}