/**
 * Tutorial 13: User-Defined Literals (UDL)
 *
 * Aprenderás:
 * - Sintaxis _u128 y _i128
 * - Ventajas de los literales
 * - Uso en constexpr
 * - Best practices
 */

#include <int128_simple.hpp>
#include <iostream>

using namespace nstd;

// Importar los namespaces de literales
using namespace nstd::int128_literals;
using namespace nstd::int128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 13: User-Defined Literals\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. SINTAXIS BÁSICA
    // ============================================================
    std::cout << "--- 1. Sintaxis de literales ---\n";

    auto unsigned_val = 12345_u128; // uint128_t
    auto signed_val = -67890_i128;  // int128_t

    std::cout << "12345_u128  = " << unsigned_val << " (uint128_t)\n";
    std::cout << "-67890_i128 = " << signed_val << " (int128_t)\n\n";

    // ============================================================
    // 2. COMPARACIÓN CON SINTAXIS TRADICIONAL
    // ============================================================
    std::cout << "--- 2. Comparación con sintaxis tradicional ---\n";

    // Sintaxis tradicional (verbose)
    uint128_t traditional{uint128_t(1000)};

    // Con literal (conciso)
    auto with_literal = 1000_u128;

    std::cout << "Traditional: uint128_t(1000)\n";
    std::cout << "Literal:     1000_u128\n";
    std::cout << "Resultado:   " << (traditional == with_literal ? "iguales" : "diferentes")
              << "\n\n";

    // ============================================================
    // 3. EN EXPRESIONES CONSTEXPR
    // ============================================================
    std::cout << "--- 3. Uso en constexpr ---\n";

    constexpr auto compile_time = 100_u128 * 200_u128;

    std::cout << "constexpr 100_u128 × 200_u128 = " << compile_time << "\n";
    std::cout << "✅ Evaluado en tiempo de compilación!\n\n";

    // ============================================================
    // 4. NÚMEROS GRANDES
    // ============================================================
    std::cout << "--- 4. Números grandes con literales ---\n";

    auto billion = 1000000000_u128;
    auto trillion = billion * 1000;
    auto quadrillion = trillion * 1000;

    std::cout << "Mil millones:     " << billion << "\n";
    std::cout << "Billón:           " << trillion << "\n";
    std::cout << "Mil billones:     " << quadrillion << "\n\n";

    // ============================================================
    // 5. OPERACIONES ARITMÉTICAS
    // ============================================================
    std::cout << "--- 5. Operaciones aritméticas ---\n";

    auto result = 100_u128 + 200_u128 * 3_u128;

    std::cout << "100_u128 + 200_u128 × 3_u128 = " << result << "\n\n";

    // ============================================================
    // 6. SIGNED VS UNSIGNED
    // ============================================================
    std::cout << "--- 6. Signed vs Unsigned ---\n";

    auto pos = 500_i128;
    auto neg = -300_i128;
    auto diff = pos - neg; // 500 - (-300) = 800

    std::cout << "500_i128 - (-300_i128) = " << diff << "\n\n";

    // ============================================================
    // 7. EN ARRAYS Y CONTENEDORES
    // ============================================================
    std::cout << "--- 7. En arrays y contenedores ---\n";

    std::vector<uint128_t> values = {10_u128, 20_u128, 30_u128, 40_u128, 50_u128};

    std::cout << "Vector: ";
    for (const auto& v : values) {
        std::cout << v << " ";
    }
    std::cout << "\n\n";

    // ============================================================
    // 8. HEXADECIMAL Y OCTAL
    // ============================================================
    std::cout << "--- 8. Hexadecimal y octal ---\n";

    auto hex = 0xFF_u128;
    auto oct = 0777_u128;

    std::cout << "0xFF_u128  = " << hex << " (hexadecimal)\n";
    std::cout << "0777_u128  = " << oct << " (octal)\n\n";

    // ============================================================
    // 9. BEST PRACTICES
    // ============================================================
    std::cout << "--- 9. 📘 Best Practices ---\n";
    std::cout << "✅ DO:\n";
    std::cout << "  - Usa _u128 para uint128_t\n";
    std::cout << "  - Usa _i128 para int128_t\n";
    std::cout << "  - Aprovecha constexpr cuando sea posible\n";
    std::cout << "  - Usa literales para claridad del código\n\n";

    std::cout << "❌ DON'T:\n";
    std::cout << "  - No olvides 'using namespace nstd::int128_literals;'\n";
    std::cout << "  - No uses _u128 con valores negativos\n";
    std::cout << "  - No mezcles signed/unsigned sin conversión explícita\n\n";

    // ============================================================
    // 10. EJEMPLO PRÁCTICO: FACTORIAL
    // ============================================================
    std::cout << "--- 10. Ejemplo: Factorial ---\n";

    constexpr auto factorial = []() {
        uint128_t result = 1_u128;
        for (int i = 2; i <= 20; ++i) {
            result *= i;
        }
        return result;
    }();

    std::cout << "20! = " << factorial << "\n";
    std::cout << "✅ Calculado con literales!\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 13 completado\n";
    std::cout << "🎉 ¡Has completado todos los tutoriales!\n";
    std::cout << "========================================\n";
    std::cout << "\nContinúa con:\n";
    std::cout << "- demos/showcase/ - Demos impresionantes\n";
    std::cout << "- demos/examples/ - Casos de uso real\n";

    return 0;
}

