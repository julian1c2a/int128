/**
 * @file expression_templates.cpp
 * @brief Demostración de Expression Templates para uint128_t
 *
 * PROBLEMA:
 * Con tipos definidos por usuario como uint128_t, la expresión:
 *     e = a + b + c + d;
 * crea 3 temporales intermedios:
 *     temp1 = a + b          // temporal 1
 *     temp2 = temp1 + c      // temporal 2
 *     temp3 = temp2 + d      // temporal 3
 *     e = temp3              // copia final
 *
 * SOLUCIÓN:
 * Expression Templates usan metaprogramación para construir un árbol de
 * expresiones sin evaluar. La evaluación ocurre en una sola pasada cuando
 * se asigna al resultado final, eliminando todos los temporales.
 *
 * TÉCNICAS DEMOSTRADAS:
 * 1. Clases de expresión (Expr, BinOp, UnOp)
 * 2. Lazy evaluation con operadores sobrecargados
 * 3. Template metaprogramming para árbol de expresiones
 * 4. Benchmarks: sin ET vs con ET
 * 5. CRTP (Curiously Recurring Template Pattern)
 *
 * COMPILACIÓN:
 *     g++ -std=c++20 -O3 -I include demos/showcase/expression_templates.cpp
 *
 * AUTOR: Generated for int128 library demo
 * FECHA: Diciembre 2025
 */

#include <chrono>
#include <int128.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <uint128/uint128_iostreams.hpp> // operator<< para cout

using namespace std;
using namespace std::chrono;

// ============================================================================
// EXPRESSION TEMPLATES INFRASTRUCTURE
// ============================================================================

/**
 * @brief Clase base CRTP para todas las expresiones
 *
 * Usa el patrón CRTP (Curiously Recurring Template Pattern) para
 * polimorfismo estático sin overhead de funciones virtuales.
 */
template <typename E> struct Expr {
    // Cast al tipo derivado (CRTP)
    constexpr const E& derived() const noexcept
    {
        return static_cast<const E&>(*this);
    }

    // Evaluación de la expresión
    constexpr uint128_t eval() const
    {
        return derived().eval();
    }

    // Conversión implícita a uint128_t
    constexpr operator uint128_t() const
    {
        return eval();
    }
};

// ============================================================================
// WRAPPER PARA uint128_t (LEAF NODE EN EL ÁRBOL DE EXPRESIONES)
// ============================================================================

/**
 * @brief Wrapper que convierte uint128_t en una expresión
 *
 * Este es un nodo hoja en el árbol de expresiones.
 * Almacena el valor directamente (no referencia) para evitar problemas con temporales.
 */
struct Uint128Expr : public Expr<Uint128Expr> {
    uint128_t value; // Copia el valor, no almacena referencia

    constexpr explicit Uint128Expr(const uint128_t& v) noexcept : value(v) {}

    constexpr uint128_t eval() const
    {
        return value;
    }
};

// ============================================================================
// OPERACIONES BINARIAS (NODOS INTERNOS DEL ÁRBOL)
// ============================================================================

/**
 * @brief Clase template para operaciones binarias
 *
 * Almacena referencias a las subexpresiones izquierda y derecha,
 * y aplica la operación Op cuando se evalúa.
 *
 * @tparam E1 Tipo de la expresión izquierda
 * @tparam E2 Tipo de la expresión derecha
 * @tparam Op Operación a aplicar (suma, resta, multiplicación, etc.)
 */
template <typename E1, typename E2, typename Op> struct BinOp : public Expr<BinOp<E1, E2, Op>> {
    const E1& left;
    const E2& right;

    constexpr BinOp(const E1& l, const E2& r) noexcept : left(l), right(r) {}

    constexpr uint128_t eval() const
    {
        return Op::apply(left.eval(), right.eval());
    }
};

// ============================================================================
// OPERACIONES UNARIAS
// ============================================================================

/**
 * @brief Clase template para operaciones unarias
 *
 * @tparam E Tipo de la expresión
 * @tparam Op Operación a aplicar (negación, complemento, etc.)
 */
template <typename E, typename Op> struct UnOp : public Expr<UnOp<E, Op>> {
    const E& expr;

    constexpr explicit UnOp(const E& e) noexcept : expr(e) {}

    constexpr uint128_t eval() const
    {
        return Op::apply(expr.eval());
    }
};

// ============================================================================
// DEFINICIÓN DE OPERACIONES
// ============================================================================

struct AddOp {
    static constexpr uint128_t apply(const uint128_t& a, const uint128_t& b)
    {
        return a + b;
    }
};

struct SubOp {
    static constexpr uint128_t apply(const uint128_t& a, const uint128_t& b)
    {
        return a - b;
    }
};

struct MulOp {
    static constexpr uint128_t apply(const uint128_t& a, const uint128_t& b)
    {
        return a * b;
    }
};

struct DivOp {
    static constexpr uint128_t apply(const uint128_t& a, const uint128_t& b)
    {
        return a / b;
    }
};

struct ModOp {
    static constexpr uint128_t apply(const uint128_t& a, const uint128_t& b)
    {
        return a % b;
    }
};

// NOTA: uint128_t no tiene operador unario -, así que lo eliminamos
// struct NegOp eliminada

struct BitNotOp {
    static constexpr uint128_t apply(const uint128_t& a)
    {
        return ~a;
    }
};

// ============================================================================
// OPERADORES SOBRECARGADOS (CONSTRUYEN EL ÁRBOL DE EXPRESIONES)
// ============================================================================

// Suma: Expr + Expr
template <typename E1, typename E2> constexpr auto operator+(const Expr<E1>& a, const Expr<E2>& b)
{
    return BinOp<E1, E2, AddOp>(a.derived(), b.derived());
}

// Suma: Expr + uint128_t
template <typename E> constexpr auto operator+(const Expr<E>& a, const uint128_t& b)
{
    return BinOp<E, Uint128Expr, AddOp>(a.derived(), Uint128Expr(b));
}

// Suma: uint128_t + Expr
template <typename E> constexpr auto operator+(const uint128_t& a, const Expr<E>& b)
{
    return BinOp<Uint128Expr, E, AddOp>(Uint128Expr(a), b.derived());
}

// Resta: Expr - Expr
template <typename E1, typename E2> constexpr auto operator-(const Expr<E1>& a, const Expr<E2>& b)
{
    return BinOp<E1, E2, SubOp>(a.derived(), b.derived());
}

// Resta: Expr - uint128_t
template <typename E> constexpr auto operator-(const Expr<E>& a, const uint128_t& b)
{
    return BinOp<E, Uint128Expr, SubOp>(a.derived(), Uint128Expr(b));
}

// Resta: uint128_t - Expr
template <typename E> constexpr auto operator-(const uint128_t& a, const Expr<E>& b)
{
    return BinOp<Uint128Expr, E, SubOp>(Uint128Expr(a), b.derived());
}

// Multiplicación: Expr * Expr
template <typename E1, typename E2> constexpr auto operator*(const Expr<E1>& a, const Expr<E2>& b)
{
    return BinOp<E1, E2, MulOp>(a.derived(), b.derived());
}

// División: Expr / Expr
template <typename E1, typename E2> constexpr auto operator/(const Expr<E1>& a, const Expr<E2>& b)
{
    return BinOp<E1, E2, DivOp>(a.derived(), b.derived());
}

// Módulo: Expr % Expr
template <typename E1, typename E2> constexpr auto operator%(const Expr<E1>& a, const Expr<E2>& b)
{
    return BinOp<E1, E2, ModOp>(a.derived(), b.derived());
}

// Nota: Operador unario - eliminado porque uint128_t no lo soporta

// Complemento: ~Expr
template <typename E> constexpr auto operator~(const Expr<E>& a)
{
    return UnOp<E, BitNotOp>(a.derived());
}

// ============================================================================
// CLASE WRAPPER CONVENIENTE
// ============================================================================

/**
 * @brief Clase conveniente que wrappea uint128_t con soporte para ET
 *
 * Esta clase permite usar Expression Templates de forma transparente.
 * Se puede usar como un uint128_t normal, pero internamente construye
 * árboles de expresiones para optimización.
 *
 * Hereda de Expr para participar en Expression Templates sin ambigüedad.
 */
class UInt128ET : public Expr<UInt128ET>
{
  private:
    uint128_t value_;

  public:
    // Constructores
    constexpr UInt128ET() noexcept : value_(0) {}
    constexpr UInt128ET(uint64_t v) noexcept : value_(v) {}
    constexpr UInt128ET(const uint128_t& v) noexcept : value_(v) {}

    // Constructor desde expresión (aquí ocurre la evaluación)
    template <typename E> constexpr UInt128ET(const Expr<E>& expr) : value_(expr.eval()) {}

    // Asignación desde expresión
    template <typename E> constexpr UInt128ET& operator=(const Expr<E>& expr)
    {
        value_ = expr.eval();
        return *this;
    }

    // Conversión a uint128_t
    constexpr operator uint128_t() const
    {
        return value_;
    }
    constexpr uint128_t value() const
    {
        return value_;
    }

    // Método eval() requerido por Expr<UInt128ET>
    constexpr uint128_t eval() const
    {
        return value_;
    }

    // Operadores de asignación compuesta
    constexpr UInt128ET& operator+=(const UInt128ET& other)
    {
        value_ += other.value_;
        return *this;
    }

    constexpr UInt128ET& operator-=(const UInt128ET& other)
    {
        value_ -= other.value_;
        return *this;
    }

    constexpr UInt128ET& operator*=(const UInt128ET& other)
    {
        value_ *= other.value_;
        return *this;
    }
};

// Los operadores ya están definidos en Expr<E1> op Expr<E2>
// Como UInt128ET hereda de Expr<UInt128ET>, participa automáticamente

// ============================================================================
// FUNCIONES DE DEMOSTRACIÓN
// ============================================================================

void demo_basic_usage()
{
    cout << "\n=== DEMOSTRACIÓN BÁSICA ===" << endl;
    cout << "Sin Expression Templates, 'e = a + b + c + d' crea 3 temporales." << endl;
    cout << "Con Expression Templates, se evalúa en una sola pasada.\n" << endl;

    uint128_t a = 1000;
    uint128_t b = 2000;
    uint128_t c = 3000;
    uint128_t d = 4000;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;

    // Forma tradicional (crea temporales)
    cout << "\n--- Sin Expression Templates ---" << endl;
    uint128_t result1 = a + b + c + d;
    cout << "e = a + b + c + d = " << result1 << endl;
    cout << "(Creó 3 temporales intermedios)" << endl;

    // Con Expression Templates
    cout << "\n--- Con Expression Templates ---" << endl;
    auto expr = Uint128Expr(a) + Uint128Expr(b) + Uint128Expr(c) + Uint128Expr(d);
    uint128_t result2 = expr.eval();
    cout << "e = a + b + c + d = " << result2 << endl;
    cout << "(Sin temporales intermedios, evaluación en una pasada)" << endl;

    // Usando la clase wrapper conveniente
    cout << "\n--- Con UInt128ET (wrapper conveniente) ---" << endl;
    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d);
    UInt128ET e_et = a_et + b_et + c_et + d_et;
    cout << "e = a + b + c + d = " << e_et.value() << endl;
    cout << "(Sintaxis transparente, optimización automática)" << endl;
}

void demo_complex_expressions()
{
    cout << "\n=== EXPRESIONES COMPLEJAS ===" << endl;

    UInt128ET a(100), b(200), c(300), d(400), e(500);

    // Expresión compleja: (a + b) * (c - d) + e
    cout << "\na = " << a.value() << ", b = " << b.value() << ", c = " << c.value()
         << ", d = " << d.value() << ", e = " << e.value() << endl;

    UInt128ET result = (a + b) * (c - d) + e;
    cout << "\nResultado de (a + b) * (c - d) + e = " << result.value() << endl;

    // Verificación manual
    uint128_t check =
        (uint128_t(100) + uint128_t(200)) * (uint128_t(300) - uint128_t(400)) + uint128_t(500);
    cout << "Verificación manual: " << check << endl;
    cout << "Coincide: " << (result.value() == check ? "SÍ ✓" : "NO ✗") << endl;

    // Expresión anidada: ((a + b) * c - d) / e
    UInt128ET result2 = ((a + b) * c - d) / (e + UInt128ET(50));
    cout << "\nResultado de ((a + b) * c - d) / (e + 50) = " << result2.value() << endl;
}

void demo_type_deduction()
{
    cout << "\n=== DEDUCCIÓN DE TIPOS (COMPILE-TIME) ===" << endl;

    UInt128ET a(10), b(20), c(30);

    // El tipo de la expresión NO es uint128_t, es un árbol de templates
    auto expr = a + b + c;

    cout << "\nauto expr = a + b + c;" << endl;
    cout << "Tipo de expr (con typeid): " << typeid(expr).name() << endl;
    cout << "\nEste tipo complejo es optimizado por el compilador." << endl;
    cout << "Al evaluar: expr.eval() o asignar a uint128_t," << endl;
    cout << "el compilador inline-a toda la expresión sin temporales." << endl;

    uint128_t result = expr; // Aquí ocurre la evaluación
    cout << "\nResultado: " << result << endl;
}

// ============================================================================
// BENCHMARKS
// ============================================================================

template <typename Func> double benchmark(const string& name, Func&& f, int iterations = 1000000)
{
    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        f();
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    double avg_ns = static_cast<double>(duration) / iterations;

    cout << setw(40) << left << name << ": " << setw(10) << right << fixed << setprecision(2)
         << avg_ns << " ns/op" << endl;

    return avg_ns;
}

void benchmark_simple_addition()
{
    cout << "\n=== BENCHMARK: SUMA SIMPLE (a + b + c + d) ===" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t a = 12345678901234567890ULL;
    uint128_t b = 98765432109876543210ULL;
    uint128_t c = 11111111111111111111ULL;
    uint128_t d = 22222222222222222222ULL;

    uint128_t result = 0;

    // Sin Expression Templates
    auto time_no_et = benchmark("Sin ET (temporales)", [&]() { result = a + b + c + d; });

    // Con Expression Templates usando wrapper
    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d);
    UInt128ET result_et;

    auto time_with_et =
        benchmark("Con ET (sin temporales)", [&]() { result_et = a_et + b_et + c_et + d_et; });

    double speedup = time_no_et / time_with_et;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;

    if (speedup > 1.0) {
        cout << "Expression Templates son " << ((speedup - 1.0) * 100) << "% más rápidos" << endl;
    } else if (speedup < 1.0) {
        cout << "Sin ET es " << ((1.0 / speedup - 1.0) * 100)
             << "% más rápido (overhead de ET no vale la pena para expresiones simples)" << endl;
    } else {
        cout << "Rendimiento similar (compilador optimizó ambos casos)" << endl;
    }
}

void benchmark_complex_expression()
{
    cout << "\n=== BENCHMARK: EXPRESIÓN COMPLEJA ===" << endl;
    cout << "e = (a + b) * (c - d) + (e / f) - g" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t a = 1000, b = 2000, c = 5000, d = 3000;
    uint128_t e = 100000, f = 10, g = 500;
    uint128_t result = 0;

    // Sin Expression Templates
    auto time_no_et = benchmark("Sin ET", [&]() { result = (a + b) * (c - d) + (e / f) - g; });

    // Con Expression Templates
    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d);
    UInt128ET e_et(e), f_et(f), g_et(g);
    UInt128ET result_et;

    auto time_with_et = benchmark(
        "Con ET", [&]() { result_et = (a_et + b_et) * (c_et - d_et) + (e_et / f_et) - g_et; });

    double speedup = time_no_et / time_with_et;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;
}

void benchmark_large_expressions()
{
    cout << "\n=== BENCHMARK: EXPRESIÓN GRANDE (10 operandos) ===" << endl;
    cout << "e = a + b + c + d + e + f + g + h + i + j" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t vals[10];
    for (int i = 0; i < 10; ++i) {
        vals[i] = 1000 + i * 100;
    }

    uint128_t result = 0;

    // Sin Expression Templates (9 temporales)
    auto time_no_et = benchmark("Sin ET (9 temporales)", [&]() {
        result = vals[0] + vals[1] + vals[2] + vals[3] + vals[4] + vals[5] + vals[6] + vals[7] +
                 vals[8] + vals[9];
    });

    // Con Expression Templates
    UInt128ET vals_et[10];
    for (int i = 0; i < 10; ++i) {
        vals_et[i] = UInt128ET(vals[i]);
    }
    UInt128ET result_et;

    auto time_with_et = benchmark("Con ET (sin temporales)", [&]() {
        result_et = vals_et[0] + vals_et[1] + vals_et[2] + vals_et[3] + vals_et[4] + vals_et[5] +
                    vals_et[6] + vals_et[7] + vals_et[8] + vals_et[9];
    });

    double speedup = time_no_et / time_with_et;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;
    cout << "\nNOTA: Cuantos más operandos, mayor beneficio de ET" << endl;
    cout << "      (menos temporales = menos copias de 16 bytes)" << endl;
}

// ============================================================================
// ANÁLISIS DE ENSAMBLADOR (CONCEPTUAL)
// ============================================================================

void explain_assembly_optimization()
{
    cout << "\n=== ANÁLISIS DE ENSAMBLADOR (CONCEPTUAL) ===" << endl;
    cout << "\nSin Expression Templates:" << endl;
    cout << "  e = a + b + c + d;" << endl;
    cout << "  // Ensamblador (simplificado):" << endl;
    cout << "  mov  temp1, a" << endl;
    cout << "  add  temp1, b      ; temp1 = a + b" << endl;
    cout << "  mov  temp2, temp1" << endl;
    cout << "  add  temp2, c      ; temp2 = temp1 + c" << endl;
    cout << "  mov  temp3, temp2" << endl;
    cout << "  add  temp3, d      ; temp3 = temp2 + d" << endl;
    cout << "  mov  e, temp3      ; copia final" << endl;
    cout << "  // 7 operaciones (4 mov + 3 add)" << endl;

    cout << "\nCon Expression Templates:" << endl;
    cout << "  e = a + b + c + d; // expresión lazy" << endl;
    cout << "  // Ensamblador (simplificado):" << endl;
    cout << "  mov  e, a" << endl;
    cout << "  add  e, b" << endl;
    cout << "  add  e, c" << endl;
    cout << "  add  e, d" << endl;
    cout << "  // 4 operaciones (1 mov + 3 add)" << endl;

    cout << "\nReducción: 43% menos instrucciones" << endl;
    cout << "Beneficio: Menos escrituras en memoria, mejor uso de cache" << endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔══════════════════════════════════════════════════════════════╗" << endl;
    cout << "║       EXPRESSION TEMPLATES PARA uint128_t                    ║" << endl;
    cout << "║                                                              ║" << endl;
    cout << "║  Técnica avanzada de C++ para eliminar temporales           ║" << endl;
    cout << "║  intermedios en expresiones aritméticas complejas            ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════╝" << endl;

    try {
        demo_basic_usage();
        demo_complex_expressions();
        demo_type_deduction();

        cout << "\n" << string(65, '=') << endl;
        cout << "BENCHMARKS" << endl;
        cout << string(65, '=') << endl;

        benchmark_simple_addition();
        benchmark_complex_expression();
        benchmark_large_expressions();

        explain_assembly_optimization();

        cout << "\n" << string(65, '=') << endl;
        cout << "CONCLUSIONES" << endl;
        cout << string(65, '=') << endl;
        cout << "\n1. Expression Templates eliminan copias innecesarias" << endl;
        cout << "2. Mayor beneficio con expresiones largas/complejas" << endl;
        cout << "3. Sin overhead en runtime (todo en compile-time)" << endl;
        cout << "4. Compiladores modernos con -O3 pueden optimizar ambos casos" << endl;
        cout << "5. Para uint128_t (16 bytes), el beneficio es notable" << endl;
        cout << "6. Trade-off: tiempo de compilación vs tiempo de ejecución" << endl;

        cout << "\n✓ Demo completado exitosamente" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
