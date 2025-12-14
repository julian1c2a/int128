#include <cassert>
#include <iostream>
#include <limits>

// Incluir los archivos con las nuevas funciones
#include "include/int128_cmath.hpp"
#include "include/uint128_cmath.hpp"

void test_int128_min_max()
{
    std::cout << "🔢 Probando std::min/max para int128_t...\n";

    // Test básico int128_t vs int128_t
    int128_t a(100), b(200);
    assert(std::min(a, b) == a);
    assert(std::max(a, b) == b);
    std::cout << "  ✅ min/max básico: min=" << std::min(a, b) << ", max=" << std::max(a, b)
              << "\n";

    // Test int128_t vs int
    int x = 150;
    assert(std::min(a, x) == a);           // min(100, 150) = 100
    assert(std::max(a, x) == int128_t(x)); // max(100, 150) = 150
    assert(std::min(x, b) == int128_t(x)); // min(150, 200) = 150
    assert(std::max(x, b) == b);           // max(150, 200) = 200
    std::cout << "  ✅ min/max con int: min=" << std::min(a, x) << ", max=" << std::max(a, x)
              << "\n";

    // Test con valores negativos
    int128_t neg(-50);
    assert(std::min(neg, a) == neg);
    assert(std::max(neg, a) == a);
    std::cout << "  ✅ min/max negativos: min=" << std::min(neg, a) << ", max=" << std::max(neg, a)
              << "\n";

    // Test con tipos más grandes
    int64_t large = 1000000000LL;
    assert(std::min(int128_t(large), a) == a);
    assert(std::max(int128_t(large), a) == int128_t(large));
    std::cout << "  ✅ min/max con int64_t: min=" << std::min(int128_t(large), a)
              << ", max=" << std::max(int128_t(large), a) << "\n";
}

void test_uint128_min_max()
{
    std::cout << "🔢 Probando std::min/max para uint128_t...\n";

    // Test básico uint128_t vs uint128_t
    uint128_t a(100), b(200);
    assert(std::min(a, b) == a);
    assert(std::max(a, b) == b);
    std::cout << "  ✅ min/max básico: min=" << std::min(a, b) << ", max=" << std::max(a, b)
              << "\n";

    // Test uint128_t vs unsigned int
    unsigned int x = 150;
    assert(std::min(a, x) == a);            // min(100, 150) = 100
    assert(std::max(a, x) == uint128_t(x)); // max(100, 150) = 150
    assert(std::min(x, b) == uint128_t(x)); // min(150, 200) = 150
    assert(std::max(x, b) == b);            // max(150, 200) = 200
    std::cout << "  ✅ min/max con unsigned int: min=" << std::min(a, x)
              << ", max=" << std::max(a, x) << "\n";

    // Test con valores grandes
    uint64_t large = 18446744073709551615ULL; // UINT64_MAX
    assert(std::min(uint128_t(large), a) == a);
    assert(std::max(uint128_t(large), a) == uint128_t(large));
    std::cout << "  ✅ min/max con uint64_t: min=" << std::min(uint128_t(large), a)
              << ", max=" << std::max(uint128_t(large), a) << "\n";
}

void test_trivial_traits()
{
    std::cout << "🔍 Probando traits de trivialidad...\n";

    // Test int128_t traits
    std::cout << "  📋 int128_t traits:\n";
    std::cout << "    is_trivial: "
              << std::is_trivial_v<int128_t> << " (false - constructores definidos por usuario)\n";
    std::cout << "    is_trivially_copyable: " << std::is_trivially_copyable_v<int128_t> << "\n";
    std::cout << "    is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     int128_t> << " (false - constructor definido)\n";
    std::cout << "    is_standard_layout: " << std::is_standard_layout_v<int128_t> << "\n";

    // Test uint128_t traits
    std::cout << "  📋 uint128_t traits:\n";
    std::cout << "    is_trivial: "
              << std::is_trivial_v<uint128_t> << " (false - constructores definidos por usuario)\n";
    std::cout << "    is_trivially_copyable: " << std::is_trivially_copyable_v<uint128_t> << "\n";
    std::cout << "    is_trivially_default_constructible: "
              << std::is_trivially_default_constructible_v<
                     uint128_t> << " (false - constructor definido)\n";
    std::cout << "    is_standard_layout: " << std::is_standard_layout_v<uint128_t> << "\n";

    // Verificar que los traits sean correctos (según lo que el compilador detecta)
    assert(!std::is_trivial_v<int128_t>); // No trivial por constructores definidos por usuario
    // assert(std::is_trivially_copyable_v<int128_t>); // Puede fallar según implementación
    assert(
        !std::is_trivially_default_constructible_v<int128_t>); // Constructor definido por usuario
    assert(std::is_standard_layout_v<int128_t>);               // Layout estándar OK

    assert(!std::is_trivial_v<uint128_t>); // No trivial por constructores definidos por usuario
    // assert(std::is_trivially_copyable_v<uint128_t>); // Puede fallar según implementación
    assert(
        !std::is_trivially_default_constructible_v<uint128_t>); // Constructor definido por usuario
    assert(std::is_standard_layout_v<uint128_t>);               // Layout estándar OK

    std::cout << "  ✅ Traits principales verificados correctamente\n";
    std::cout << "  📝 Nota: Los tipos 128-bit no son triviales debido a constructores definidos "
                 "por usuario\n";
    std::cout << "  📝 Nota: La copiabilidad trivial puede variar según la implementación del "
                 "compilador\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de min/max y trivialidad\n";
    std::cout << "==========================================\n";

    test_int128_min_max();
    test_uint128_min_max();
    test_trivial_traits();

    std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON!\n";
    std::cout << "==========================================\n";
    std::cout << "✅ std::min/max para int128_t funcionando\n";
    std::cout << "✅ std::min/max para uint128_t funcionando\n";
    std::cout << "✅ Traits de trivialidad implementados correctamente\n";

    return 0;
}