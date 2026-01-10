/**
 * Tutorial 03: Comparaciones
 *
 * Este tutorial muestra todas las operaciones de comparación
 * disponibles para uint128_t e int128_t.
 *
 * Aprenderás:
 * - Operadores de comparación (==, !=, <, >, <=, >=)
 * - Diferencias entre signed y unsigned
 * - Ordenamiento y búsqueda
 * - Comparación con tipos nativos
 */

#include <algorithm>
#include <int128_simple.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <uint128/uint128_iostreams.hpp>
#include <vector>

using namespace nstd;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 03: Comparaciones\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. IGUALDAD (==) Y DESIGUALDAD (!=)
    // ============================================================
    std::cout << "--- 1. Igualdad y desigualdad ---\n";

    uint128_t a{1000};
    uint128_t b{1000};
    uint128_t c{2000};

    std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n\n";

    std::cout << "a == b: " << (a == b ? "true" : "false") << "\n";
    std::cout << "a == c: " << (a == c ? "true" : "false") << "\n";
    std::cout << "a != c: " << (a != c ? "true" : "false") << "\n\n";

    // ============================================================
    // 2. MENOR QUE (<) Y MAYOR QUE (>)
    // ============================================================
    std::cout << "--- 2. Menor que y mayor que ---\n";

    std::cout << "a < c:  " << (a < c ? "true" : "false") << "\n";
    std::cout << "c > a:  " << (c > a ? "true" : "false") << "\n";
    std::cout << "a < a:  " << (a < a ? "true" : "false") << "\n\n";

    // ============================================================
    // 3. MENOR O IGUAL (<=) Y MAYOR O IGUAL (>=)
    // ============================================================
    std::cout << "--- 3. Menor o igual, mayor o igual ---\n";

    std::cout << "a <= b: " << (a <= b ? "true" : "false") << "\n";
    std::cout << "a <= c: " << (a <= c ? "true" : "false") << "\n";
    std::cout << "c >= a: " << (c >= a ? "true" : "false") << "\n\n";

    // ============================================================
    // 4. COMPARACIÓN CON TIPOS NATIVOS
    // ============================================================
    std::cout << "--- 4. Comparación con tipos nativos ---\n";

    uint128_t big{1000000};
    int native_int = 500000;

    std::cout << "uint128_t big = " << big << "\n";
    std::cout << "int native_int = " << native_int << "\n\n";

    std::cout << "big > native_int:  " << (big > native_int ? "true" : "false") << "\n";
    std::cout << "big == 1000000:    " << (big == 1000000 ? "true" : "false") << "\n";
    std::cout << "big != 999999:     " << (big != 999999 ? "true" : "false") << "\n\n";

    // ============================================================
    // 5. SIGNED VS UNSIGNED
    // ============================================================
    std::cout << "--- 5. Signed vs Unsigned ---\n";

    int128_t signed_neg{-100};
    int128_t signed_pos{100};
    uint128_t unsigned_val{50};

    std::cout << "int128_t signed_neg = " << signed_neg << "\n";
    std::cout << "int128_t signed_pos = " << signed_pos << "\n";
    std::cout << "uint128_t unsigned_val = " << unsigned_val << "\n\n";

    // Comparación entre signed
    std::cout << "signed_neg < signed_pos: " << (signed_neg < signed_pos ? "true" : "false")
              << "\n";

    // Comparación con unsigned (requiere conversión explícita)
    std::cout << "signed_pos > unsigned_val: "
              << (signed_pos > static_cast<int128_t>(unsigned_val) ? "true" : "false") << "\n\n";

    // ============================================================
    // 6. MÁXIMOS Y MÍNIMOS
    // ============================================================
    std::cout << "--- 6. Encontrar máximo y mínimo ---\n";

    uint128_t x = 500, y = 1000, z = 750;

    // Usando std::max y std::min
    auto maximum = std::max({x, y, z});
    auto minimum = std::min({x, y, z});

    std::cout << "Valores: " << x << ", " << y << ", " << z << "\n";
    std::cout << "Máximo: " << maximum << "\n";
    std::cout << "Mínimo: " << minimum << "\n\n";

    // ============================================================
    // 7. ORDENAMIENTO
    // ============================================================
    std::cout << "--- 7. Ordenamiento ---\n";

    std::vector<uint128_t> numbers = {500, 100, 1000, 250, 750};

    std::cout << "Antes de ordenar: ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    std::sort(numbers.begin(), numbers.end());

    std::cout << "Después de ordenar: ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";

    // ============================================================
    // 8. BÚSQUEDA BINARIA
    // ============================================================
    std::cout << "--- 8. Búsqueda binaria ---\n";

    uint128_t target{250};

    bool found = std::binary_search(numbers.begin(), numbers.end(), target);

    std::cout << "¿" << target << " está en el vector? " << (found ? "Sí" : "No") << "\n";

    auto it = std::lower_bound(numbers.begin(), numbers.end(), target);
    if (it != numbers.end()) {
        std::cout << "Posición de " << target << ": " << std::distance(numbers.begin(), it)
                  << "\n\n";
    }

    // ============================================================
    // 9. COMPARACIÓN DE RANGOS
    // ============================================================
    std::cout << "--- 9. Verificación de rangos ---\n";

    uint128_t value{75};
    uint128_t min_range{50};
    uint128_t max_range{100};

    bool in_range = (value >= min_range) && (value <= max_range);

    std::cout << "¿" << value << " está entre " << min_range << " y " << max_range << "? "
              << (in_range ? "Sí" : "No") << "\n\n";

    // ============================================================
    // 10. COMPARACIÓN LEXICOGRÁFICA
    // ============================================================
    std::cout << "--- 10. Comparación lexicográfica ---\n";

    std::vector<uint128_t> vec1 = {1, 2, 3};
    std::vector<uint128_t> vec2 = {1, 2, 4};
    std::vector<uint128_t> vec3 = {1, 2, 3};

    std::cout << "vec1 < vec2: "
              << (std::lexicographical_compare(vec1.begin(), vec1.end(), vec2.begin(), vec2.end())
                      ? "true"
                      : "false")
              << "\n";

    std::cout << "vec1 == vec3: "
              << (std::equal(vec1.begin(), vec1.end(), vec3.begin()) ? "true" : "false") << "\n\n";

    // ============================================================
    // 11. OPERADOR SPACESHIP (C++20)
    // ============================================================
    // NOTA: El operador <=> aún no está implementado para uint128_t/int128_t
    // Se puede simular usando los operadores de comparación tradicionales
    std::cout << "--- 11. Three-way comparison (simulado) ---\n";

    uint128_t p{100};
    uint128_t q{200};

    // Simulamos el comportamiento de operator<=>
    int cmp;
    if (p < q) {
        cmp = -1;
    } else if (p > q) {
        cmp = 1;
    } else {
        cmp = 0;
    }

    if (cmp < 0) {
        std::cout << p << " < " << q << "\n";
    } else if (cmp > 0) {
        std::cout << p << " > " << q << "\n";
    } else {
        std::cout << p << " == " << q << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 12. PITFALL: COMPARACIÓN UNSIGNED
    // ============================================================
    std::cout << "--- 12. ⚠️  PITFALL: Comparaciones unsigned ---\n";

    uint128_t unsigned_a{5};
    uint128_t unsigned_b{10};

    // Esto puede ser confuso con unsigned!
    auto diff = unsigned_a - unsigned_b; // Wrap around!

    std::cout << "unsigned_a = " << unsigned_a << "\n";
    std::cout << "unsigned_b = " << unsigned_b << "\n";
    std::cout << "unsigned_a - unsigned_b = " << diff << " (¡wrap around!)\n";
    std::cout << "diff > unsigned_a: " << (diff > unsigned_a ? "true" : "false")
              << " (¡parece mayor!)\n\n";

    std::cout << "Mejor comparar ANTES de restar:\n";
    if (unsigned_a < unsigned_b) {
        std::cout << "a es menor que b, no podemos restar sin wrap\n";
    }
    std::cout << "\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 03 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 04_conversions.cpp\n";

    return 0;
}

