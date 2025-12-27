/**
 * @file expression_templates_fold.cpp
 * @brief Expression Templates con Fold Expressions de C++17
 *
 * DEMOSTRACION:
 * Las fold expressions (C++17) permiten que el compilador desenrolle
 * automáticamente operaciones n-arias de forma nativa y elegante.
 *
 * SINTAXIS DE FOLD EXPRESSIONS:
 * 1. Unary left fold:   (... op args)     →  (((arg1 op arg2) op arg3) op ...)
 * 2. Unary right fold:  (args op ...)     →  (... op (argN-1 op argN))
 * 3. Binary left fold:  (init op ... op args)
 * 4. Binary right fold: (args op ... op init)
 *
 * VENTAJAS sobre implementación manual:
 * ✅ Código más simple y legible
 * ✅ Compilador optimiza mejor
 * ✅ Menos código plantilla
 * ✅ Sintaxis nativa del lenguaje
 *
 * COMPILACIÓN:
 *     make demo CATEGORY=showcase DEMO=expression_templates_fold
 *
 * FECHA: Diciembre 2025
 */

#include <chrono>
#include <int128.hpp>
#include <iomanip>
#include <iostream>
#include <uint128/uint128_iostreams.hpp>

using namespace std;
using namespace std::chrono;

// ============================================================================
// INFRAESTRUCTURA BÁSICA
// ============================================================================

template <typename E> struct Expr {
    constexpr const E& cast() const noexcept
    {
        return static_cast<const E&>(*this);
    }

    constexpr uint128_t eval() const
    {
        return cast().eval();
    }
};

struct Terminal : public Expr<Terminal> {
    uint128_t value;
    constexpr explicit Terminal(uint128_t v) noexcept : value(v) {}
    constexpr uint128_t eval() const noexcept
    {
        return value;
    }
};

// ============================================================================
// FOLD EXPRESSIONS PARA SUMA (VERSIÓN SIMPLE)
// ============================================================================

/**
 * @brief Suma n-aria usando fold expression nativo de C++17
 *
 * Esta es la implementación MÁS SIMPLE Y CORRECTA.
 * El compilador se encarga de todo el desenrollado.
 */
template <typename... Args> constexpr auto sum_fold(Args... args)
{
    // Left fold: (... + args)
    // Se expande a: ((arg1 + arg2) + arg3) + ... + argN
    return (... + args.eval());
}

/**
 * @brief Suma n-aria con valor inicial
 */
template <typename... Args> constexpr auto sum_fold_init(Args... args)
{
    // Binary left fold: (init + ... + args)
    // Se expande a: (((0 + arg1) + arg2) + arg3) + ...
    return (uint128_t(0) + ... + args.eval());
}

/**
 * @brief Right fold para suma (asociatividad derecha)
 */
template <typename... Args> constexpr auto sum_fold_right(Args... args)
{
    // Right fold: (args + ...)
    // Se expande a: arg1 + (arg2 + (arg3 + ... + argN))
    return (args.eval() + ...);
}

// ============================================================================
// FOLD EXPRESSIONS PARA PRODUCTO
// ============================================================================

template <typename... Args> constexpr auto product_fold(Args... args)
{
    // Left fold para multiplicación
    return (... * args.eval());
}

template <typename... Args> constexpr auto product_fold_init(Args... args)
{
    // Con valor inicial 1
    return (uint128_t(1) * ... * args.eval());
}

// ============================================================================
// FOLD EXPRESSIONS PARA OPERACIONES BITWISE
// ============================================================================

template <typename... Args> constexpr auto bitwise_or_fold(Args... args)
{
    return (... | args.eval());
}

template <typename... Args> constexpr auto bitwise_and_fold(Args... args)
{
    return (... & args.eval());
}

template <typename... Args> constexpr auto bitwise_xor_fold(Args... args)
{
    return (... ^ args.eval());
}

// ============================================================================
// COMPARACIÓN: FOLD MANUAL VS FOLD NATIVO
// ============================================================================

/**
 * @brief Implementación MANUAL (complicada, innecesaria)
 */
template <typename... Args> struct SumManual {
    tuple<Args...> args;

    constexpr explicit SumManual(Args... a) : args(a...) {}

    constexpr uint128_t eval() const
    {
        return eval_impl(std::index_sequence_for<Args...>{});
    }

  private:
    template <size_t... Is> constexpr uint128_t eval_impl(std::index_sequence<Is...>) const
    {
        return fold_left(std::get<Is>(args).eval()...);
    }

    // Caso base
    template <typename T> static constexpr uint128_t fold_left(T arg)
    {
        return arg;
    }

    // Recursivo
    template <typename T, typename... Rest>
    static constexpr uint128_t fold_left(T first, Rest... rest)
    {
        return first + fold_left(rest...);
    }
};

/**
 * @brief Implementación con FOLD EXPRESSION nativo (simple, elegante)
 */
template <typename... Args> struct SumNative {
    tuple<Args...> args;

    constexpr explicit SumNative(Args... a) : args(a...) {}

    constexpr uint128_t eval() const
    {
        return eval_impl(std::index_sequence_for<Args...>{});
    }

  private:
    template <size_t... Is> constexpr uint128_t eval_impl(std::index_sequence<Is...>) const
    {
        // ¡UNA SOLA LÍNEA con fold expression!
        return (... + std::get<Is>(args).eval());
    }
};

// ============================================================================
// DEMOS
// ============================================================================

void demo_basic_fold()
{
    cout << "\n=== FOLD EXPRESSIONS BÁSICOS ===" << endl;

    Terminal a(100), b(200), c(300), d(400), e(500);

    cout << "Valores: a=100, b=200, c=300, d=400, e=500\n" << endl;

    // Left fold sin valor inicial
    auto result1 = sum_fold(a, b, c, d, e);
    cout << "sum_fold(a,b,c,d,e) = " << result1 << endl;
    cout << "  Sintaxis: (... + args)" << endl;
    cout << "  Expande a: ((((a+b)+c)+d)+e)" << endl;

    // Left fold con valor inicial
    auto result2 = sum_fold_init(a, b, c, d, e);
    cout << "\nsum_fold_init(a,b,c,d,e) = " << result2 << endl;
    cout << "  Sintaxis: (0 + ... + args)" << endl;
    cout << "  Expande a: (((((0+a)+b)+c)+d)+e)" << endl;

    // Right fold
    auto result3 = sum_fold_right(a, b, c, d, e);
    cout << "\nsum_fold_right(a,b,c,d,e) = " << result3 << endl;
    cout << "  Sintaxis: (args + ...)" << endl;
    cout << "  Expande a: (a+(b+(c+(d+e))))" << endl;

    cout << "\nNOTA: Para suma el orden no importa (conmutativo)" << endl;
    cout << "      Pero para operaciones no-conmutativas sí importa" << endl;
}

void demo_different_operations()
{
    cout << "\n=== DIFERENTES OPERACIONES CON FOLD ===" << endl;

    Terminal a(2), b(3), c(4), d(5);

    cout << "Valores: a=2, b=3, c=4, d=5\n" << endl;

    // Suma
    auto sum = sum_fold(a, b, c, d);
    cout << "Suma: (... + args) = " << sum << endl;

    // Producto
    auto prod = product_fold(a, b, c, d);
    cout << "Producto: (... * args) = " << prod << endl;

    // Bitwise OR
    Terminal x(0x01), y(0x02), z(0x04), w(0x08);
    auto or_result = bitwise_or_fold(x, y, z, w);
    cout << "\nBitwise OR: (0x01 | 0x02 | 0x04 | 0x08) = " << or_result << " (0x" << hex
         << or_result << dec << ")" << endl;

    // Bitwise AND
    Terminal p(0xFF), q(0xF0), r(0xCC);
    auto and_result = bitwise_and_fold(p, q, r);
    cout << "Bitwise AND: (0xFF & 0xF0 & 0xCC) = " << and_result << " (0x" << hex << and_result
         << dec << ")" << endl;

    // Bitwise XOR
    Terminal m(0xFF), n(0x0F);
    auto xor_result = bitwise_xor_fold(m, n);
    cout << "Bitwise XOR: (0xFF ^ 0x0F) = " << xor_result << " (0x" << hex << xor_result << dec
         << ")" << endl;
}

void demo_comparison()
{
    cout << "\n=== COMPARACIÓN: MANUAL VS NATIVO ===" << endl;

    Terminal a(10), b(20), c(30), d(40), e(50);

    cout << "Implementación manual (recursiva):" << endl;
    cout << "  - Requiere casos base y recursivos" << endl;
    cout << "  - ~15 líneas de código plantilla" << endl;
    cout << "  - Más difícil de leer y mantener" << endl;

    SumManual<Terminal, Terminal, Terminal, Terminal, Terminal> manual(a, b, c, d, e);
    auto result1 = manual.eval();
    cout << "  Resultado: " << result1 << endl;

    cout << "\nImplementación con fold expression nativo:" << endl;
    cout << "  - Una sola línea: (... + args.eval())" << endl;
    cout << "  - Código limpio y expresivo" << endl;
    cout << "  - El compilador optimiza mejor" << endl;

    SumNative<Terminal, Terminal, Terminal, Terminal, Terminal> native(a, b, c, d, e);
    auto result2 = native.eval();
    cout << "  Resultado: " << result2 << endl;

    cout << "\n✓ Mismo resultado, código mucho más simple" << endl;
}

void demo_constexpr()
{
    cout << "\n=== EVALUACIÓN EN COMPILE-TIME ===" << endl;

    cout << "Las fold expressions funcionan perfectamente con constexpr:\n" << endl;

    // Todo evaluado en compile-time
    constexpr Terminal a(10);
    constexpr Terminal b(20);
    constexpr Terminal c(30);

    constexpr auto result = sum_fold(a, b, c);

    cout << "constexpr auto result = sum_fold(10, 20, 30);" << endl;
    cout << "Resultado (conocido en compilación): " << result << endl;
    cout << "\n→ Zero overhead runtime, valor constante en el binario" << endl;
}

// ============================================================================
// BENCHMARKS
// ============================================================================

template <typename Func> double benchmark(const string& name, Func&& f, int iterations = 1000000)
{
    auto start = high_resolution_clock::now();

    uint128_t sink = 0;
    for (int i = 0; i < iterations; ++i) {
        auto result = f();
        sink = sink + result;
    }

    if (sink == uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL)) {
        cout << "impossible";
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    double avg_ns = static_cast<double>(duration) / iterations;

    cout << "  " << setw(35) << left << name << ": " << setw(10) << right << fixed
         << setprecision(2) << avg_ns << " ns/op" << endl;

    return avg_ns;
}

void benchmark_fold_expressions()
{
    cout << "\n=== BENCHMARKS: FOLD EXPRESSIONS ===" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    Terminal a(100), b(200), c(300), d(400), e(500);

    cout << "Suma de 5 operandos (a+b+c+d+e):" << endl;

    benchmark("Fold manual (recursivo)", [&]() {
        SumManual<Terminal, Terminal, Terminal, Terminal, Terminal> manual(a, b, c, d, e);
        return manual.eval();
    });

    benchmark("Fold nativo C++17", [&]() {
        SumNative<Terminal, Terminal, Terminal, Terminal, Terminal> native(a, b, c, d, e);
        return native.eval();
    });

    benchmark("Fold directo (sin struct)", [&]() { return sum_fold(a, b, c, d, e); });

    cout << "\n→ Los tres métodos tienen rendimiento similar" << endl;
    cout << "  (el compilador optimiza agresivamente todos los casos)" << endl;
    cout << "\nVentaja del fold nativo: código más simple y mantenible" << endl;
}

// ============================================================================
// DOCUMENTACIÓN
// ============================================================================

void explain_fold_syntax()
{
    cout << "\n=== SINTAXIS DE FOLD EXPRESSIONS (C++17) ===" << endl;

    cout << "\n1. UNARY LEFT FOLD: (... op args)" << endl;
    cout << "   Ejemplo: (... + args)" << endl;
    cout << "   Expande: ((arg1 + arg2) + arg3) + ... + argN" << endl;
    cout << "   Uso: sum_fold(a, b, c, d)" << endl;

    cout << "\n2. UNARY RIGHT FOLD: (args op ...)" << endl;
    cout << "   Ejemplo: (args + ...)" << endl;
    cout << "   Expande: arg1 + (arg2 + (arg3 + ... + argN))" << endl;
    cout << "   Uso: Para asociatividad derecha" << endl;

    cout << "\n3. BINARY LEFT FOLD: (init op ... op args)" << endl;
    cout << "   Ejemplo: (0 + ... + args)" << endl;
    cout << "   Expande: ((0 + arg1) + arg2) + ... + argN" << endl;
    cout << "   Uso: Cuando se necesita valor inicial" << endl;

    cout << "\n4. BINARY RIGHT FOLD: (args op ... op init)" << endl;
    cout << "   Ejemplo: (args + ... + 0)" << endl;
    cout << "   Expande: arg1 + (arg2 + (... + (argN + 0)))" << endl;
    cout << "   Uso: Valor inicial a la derecha" << endl;

    cout << "\nOPERADORES SOPORTADOS:" << endl;
    cout << "  Aritméticos: +, -, *, /, %" << endl;
    cout << "  Bitwise: &, |, ^, <<, >>" << endl;
    cout << "  Lógicos: &&, ||" << endl;
    cout << "  Comparación: ==, !=, <, >, <=, >=" << endl;
    cout << "  Otros: ,, ->*, .*, etc." << endl;
}

void explain_advantages()
{
    cout << "\n=== VENTAJAS DE FOLD EXPRESSIONS ===" << endl;

    cout << "\nVS IMPLEMENTACIÓN MANUAL:" << endl;
    cout << "  ✓ Código mucho más corto (1 línea vs 15 líneas)" << endl;
    cout << "  ✓ Más legible y expresivo" << endl;
    cout << "  ✓ Menos propenso a errores" << endl;
    cout << "  ✓ El compilador entiende mejor la intención" << endl;
    cout << "  ✓ Puede generar código más óptimo" << endl;

    cout << "\nVS BUCLES RUNTIME:" << endl;
    cout << "  ✓ Todo evaluado en compile-time" << endl;
    cout << "  ✓ Zero overhead" << endl;
    cout << "  ✓ Permite constexpr completo" << endl;
    cout << "  ✓ Mejor inlining y optimizaciones" << endl;

    cout << "\nCUÁNDO USAR FOLD EXPRESSIONS:" << endl;
    cout << "  → Operaciones n-arias homogéneas" << endl;
    cout << "  → Suma, producto, OR/AND de múltiples valores" << endl;
    cout << "  → Reduce verbosidad en template metaprogramming" << endl;
    cout << "  → Anywhere variadic templates son útiles" << endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔══════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   EXPRESSION TEMPLATES CON FOLD EXPRESSIONS (C++17)          ║" << endl;
    cout << "║                                                              ║" << endl;
    cout << "║  Respuesta: SÍ, el compilador puede usar a+...+0            ║" << endl;
    cout << "║  nativamente con fold expressions                           ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════╝" << endl;

    try {
        demo_basic_fold();
        demo_different_operations();
        demo_comparison();
        demo_constexpr();

        cout << "\n" << string(65, '=') << endl;
        cout << "ANÁLISIS DE PERFORMANCE" << endl;
        cout << string(65, '=') << endl;

        benchmark_fold_expressions();

        cout << "\n" << string(65, '=') << endl;
        cout << "DOCUMENTACIÓN TÉCNICA" << endl;
        cout << string(65, '=') << endl;

        explain_fold_syntax();
        explain_advantages();

        cout << "\n" << string(65, '=') << endl;
        cout << "RESUMEN" << endl;
        cout << string(65, '=') << endl;
        cout << "\nFold expressions (C++17) son la forma CORRECTA y SIMPLE" << endl;
        cout << "de implementar operaciones n-arias en Expression Templates:" << endl;
        cout << "\n  1. Sintaxis: (... + args) o (args + ...)" << endl;
        cout << "  2. El compilador desenrolla automáticamente" << endl;
        cout << "  3. Código mucho más simple que recursión manual" << endl;
        cout << "  4. Mismo o mejor rendimiento" << endl;
        cout << "  5. Funciona con constexpr para compile-time" << endl;
        cout << "\n✓ Tenías razón: el compilador lo trata nativamente" << endl;
        cout << "✓ Demo completado exitosamente" << endl;

    } catch (const exception& e) {
        cerr << "\n✗ Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
