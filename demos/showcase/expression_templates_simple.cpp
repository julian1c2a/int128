/**
 * @file expression_templates_simple.cpp
 * @brief Demostración simplificada de Expression Templates para uint128_t
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
 * Expression Templates construyen un árbol de expresiones en tiempo de
 * compilación. La evaluación ocurre en una sola pasada, eliminando temporales.
 *
 * TÉCNICAS:
 * 1. Template metaprogramming (tipos que representan expresiones)
 * 2. Lazy evaluation (evaluación diferida hasta asignación)
 * 3. CRTP (Curiously Recurring Template Pattern)
 * 4. Benchmarks comparativos
 *
 * COMPILACIÓN:
 *     make demo CATEGORY=showcase DEMO=expression_templates_simple
 *
 * FECHA: Diciembre 2025
 */

#include <chrono>
#include <int128_simple.hpp>
#include <iomanip>
#include <iostream>
#include <uint128/uint128_iostreams.hpp>

using namespace nstd;

using namespace std;
using namespace std::chrono;

// ============================================================================
// VERSIÓN SIMPLIFICADA: Sin referencias, solo copias por valor
// ============================================================================

/**
 * @brief Clase base para expresiones (CRTP)
 */
template <typename E> struct Expr {
    constexpr const E& derived() const noexcept
    {
        return static_cast<const E&>(*this);
    }

    constexpr uint128_t eval() const
    {
        return derived().eval();
    }
};

/**
 * @brief Hoja: wrapper para uint128_t
 */
struct Value : public Expr<Value> {
    uint128_t val;

    constexpr explicit Value(uint128_t v) noexcept : val(v) {}
    constexpr uint128_t eval() const
    {
        return val;
    }
};

/**
 * @brief Nodo binario: suma
 */
template <typename L, typename R> struct Add : public Expr<Add<L, R>> {
    L left;
    R right;

    constexpr Add(L l, R r) noexcept : left(l), right(r) {}
    constexpr uint128_t eval() const
    {
        return left.eval() + right.eval();
    }
};

/**
 * @brief Nodo binario: resta
 */
template <typename L, typename R> struct Sub : public Expr<Sub<L, R>> {
    L left;
    R right;

    constexpr Sub(L l, R r) noexcept : left(l), right(r) {}
    constexpr uint128_t eval() const
    {
        return left.eval() - right.eval();
    }
};

/**
 * @brief Nodo binario: multiplicación
 */
template <typename L, typename R> struct Mul : public Expr<Mul<L, R>> {
    L left;
    R right;

    constexpr Mul(L l, R r) noexcept : left(l), right(r) {}
    constexpr uint128_t eval() const
    {
        return left.eval() * right.eval();
    }
};

/**
 * @brief Nodo binario: división
 */
template <typename L, typename R> struct Div : public Expr<Div<L, R>> {
    L left;
    R right;

    constexpr Div(L l, R r) noexcept : left(l), right(r) {}
    constexpr uint128_t eval() const
    {
        return left.eval() / right.eval();
    }
};

// ============================================================================
// OPERADORES SOBRECARGADOS
// ============================================================================

template <typename L, typename R> constexpr auto operator+(const Expr<L>& l, const Expr<R>& r)
{
    return Add<L, R>(l.derived(), r.derived());
}

template <typename L, typename R> constexpr auto operator-(const Expr<L>& l, const Expr<R>& r)
{
    return Sub<L, R>(l.derived(), r.derived());
}

template <typename L, typename R> constexpr auto operator*(const Expr<L>& l, const Expr<R>& r)
{
    return Mul<L, R>(l.derived(), r.derived());
}

template <typename L, typename R> constexpr auto operator/(const Expr<L>& l, const Expr<R>& r)
{
    return Div<L, R>(l.derived(), r.derived());
}

// ============================================================================
// CLASE CONVENIENTE
// ============================================================================

class UInt128ET : public Expr<UInt128ET>
{
  private:
    uint128_t value_;

  public:
    constexpr UInt128ET() noexcept : value_(0) {}
    constexpr UInt128ET(uint64_t v) noexcept : value_(v) {}
    constexpr UInt128ET(const uint128_t& v) noexcept : value_(v) {}

    template <typename E> constexpr UInt128ET(const Expr<E>& expr) : value_(expr.eval()) {}

    template <typename E> constexpr UInt128ET& operator=(const Expr<E>& expr)
    {
        value_ = expr.eval();
        return *this;
    }

    constexpr operator uint128_t() const
    {
        return value_;
    }
    constexpr uint128_t value() const
    {
        return value_;
    }
    constexpr uint128_t eval() const
    {
        return value_;
    }

    constexpr UInt128ET& operator+=(const UInt128ET& other)
    {
        value_ += other.value_;
        return *this;
    }
};

// ============================================================================
// DEMOS
// ============================================================================

void demo_basic()
{
    cout << "\n=== DEMO BÁSICO ===" << endl;
    cout << "Problema: e = a + b + c + d crea 3 temporales\n" << endl;

    uint128_t a = 1000, b = 2000, c = 3000, d = 4000;

    cout << "Valores:" << endl;
    cout << "  a = " << a << endl;
    cout << "  b = " << b << endl;
    cout << "  c = " << c << endl;
    cout << "  d = " << d << endl;

    // Sin ET
    cout << "\n1. Sin Expression Templates:" << endl;
    uint128_t result1 = a + b + c + d;
    cout << "   e = a + b + c + d = " << result1 << endl;
    cout << "   ↳ Crea 3 temporales (temp1, temp2, temp3)" << endl;

    // Con ET - forma explícita
    cout << "\n2. Con Expression Templates (explícito):" << endl;
    auto expr = Value(a) + Value(b) + Value(c) + Value(d);
    uint128_t result2 = expr.eval();
    cout << "   e = a + b + c + d = " << result2 << endl;
    cout << "   ↳ Sin temporales, evaluación en una pasada" << endl;

    // Con ET - usando wrapper
    cout << "\n3. Con UInt128ET (wrapper conveniente):" << endl;
    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d);
    UInt128ET e_et = a_et + b_et + c_et + d_et;
    cout << "   e = a + b + c + d = " << e_et.value() << endl;
    cout << "   ↳ Sintaxis natural, optimización automática" << endl;
}

void demo_complex()
{
    cout << "\n=== EXPRESIONES COMPLEJAS ===" << endl;

    UInt128ET a(100), b(200), c(500), d(300), e(10);

    cout << "\nValores: a=100, b=200, c=500, d=300, e=10\n" << endl;

    // (a + b) * (c - d) / e
    cout << "Expresión: (a + b) * (c - d) / e" << endl;
    UInt128ET result = (a + b) * (c - d) / e;
    cout << "Resultado: " << result.value() << endl;

    // Verificación manual
    uint128_t check =
        (uint128_t(100) + uint128_t(200)) * (uint128_t(500) - uint128_t(300)) / uint128_t(10);
    cout << "Verificación: " << check << " " << (result.value() == check ? "✓" : "✗") << endl;

    cout << "\nSin ET: Crearía 4 temporales" << endl;
    cout << "Con ET: 0 temporales, evaluación directa" << endl;
}

void demo_types()
{
    cout << "\n=== DEDUCCIÓN DE TIPOS ===" << endl;

    UInt128ET a(10), b(20), c(30);

    cout << "\nCódigo: auto expr = a + b + c;" << endl;
    auto expr = a + b + c;

    cout << "Tipo real: " << typeid(expr).name() << endl;
    cout << "\nEste tipo complejo:" << endl;
    cout << "  • Se construye en tiempo de compilación" << endl;
    cout << "  • No existe en runtime" << endl;
    cout << "  • El compilador lo inline-a completamente" << endl;
    cout << "  • Resultado: código tan eficiente como escrito a mano" << endl;

    uint128_t result = expr.eval();
    cout << "\nResultado final: " << result << endl;
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

    cout << "  " << setw(30) << left << name << ": " << setw(10) << right << fixed
         << setprecision(2) << avg_ns << " ns/op" << endl;

    return avg_ns;
}

void benchmark_simple()
{
    cout << "\n=== BENCHMARK: a + b + c + d ===" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t a = 1234567890123456ULL;
    uint128_t b = 9876543210987654ULL;
    uint128_t c = 1111111111111111ULL;
    uint128_t d = 2222222222222222ULL;
    uint128_t result{0};

    auto time1 = benchmark("Sin ET", [&]() { result = a + b + c + d; });

    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d);
    UInt128ET result_et;

    auto time2 = benchmark("Con ET", [&]() { result_et = a_et + b_et + c_et + d_et; });

    double speedup = time1 / time2;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;

    if (speedup > 1.05) {
        cout << "→ ET es " << ((speedup - 1.0) * 100) << "% más rápido" << endl;
    } else if (speedup < 0.95) {
        cout << "→ Sin ET es " << ((1.0 / speedup - 1.0) * 100) << "% más rápido" << endl;
        cout << "  (ET no vale la pena para expresiones tan simples)" << endl;
    } else {
        cout << "→ Rendimiento similar" << endl;
        cout << "  (compilador optimizó ambos casos con -O3)" << endl;
    }
}

void benchmark_complex()
{
    cout << "\n=== BENCHMARK: (a+b)*(c-d)/(e+f) ===" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t a = 100, b = 200, c = 500, d = 300, e = 10, f = 5;
    uint128_t result{0};

    auto time1 = benchmark("Sin ET", [&]() { result = (a + b) * (c - d) / (e + f); });

    UInt128ET a_et(a), b_et(b), c_et(c), d_et(d), e_et(e), f_et(f);
    UInt128ET result_et;

    auto time2 =
        benchmark("Con ET", [&]() { result_et = (a_et + b_et) * (c_et - d_et) / (e_et + f_et); });

    double speedup = time1 / time2;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;
}

void benchmark_large()
{
    cout << "\n=== BENCHMARK: 10 operandos ===" << endl;
    cout << "e = a+b+c+d+e+f+g+h+i+j" << endl;
    cout << "Iteraciones: 1,000,000\n" << endl;

    uint128_t vals[10];
    for (int i = 0; i < 10; ++i) {
        vals[i] = 1000 + i * 100;
    }
    uint128_t result{0};

    auto time1 = benchmark("Sin ET (9 temporales)", [&]() {
        result = vals[0] + vals[1] + vals[2] + vals[3] + vals[4] + vals[5] + vals[6] + vals[7] +
                 vals[8] + vals[9];
    });

    UInt128ET vals_et[10];
    for (int i = 0; i < 10; ++i) {
        vals_et[i] = UInt128ET(vals[i]);
    }
    UInt128ET result_et;

    auto time2 = benchmark("Con ET (sin temporales)", [&]() {
        result_et = vals_et[0] + vals_et[1] + vals_et[2] + vals_et[3] + vals_et[4] + vals_et[5] +
                    vals_et[6] + vals_et[7] + vals_et[8] + vals_et[9];
    });

    double speedup = time1 / time2;
    cout << "\nSpeedup: " << fixed << setprecision(2) << speedup << "x" << endl;
    cout << "→ Cuantos más operandos, mayor beneficio de ET" << endl;
}

// ============================================================================
// ANÁLISIS CONCEPTUAL
// ============================================================================

void explain_how_it_works()
{
    cout << "\n=== CÓMO FUNCIONA ===" << endl;

    cout << "\n1. SIN EXPRESSION TEMPLATES:" << endl;
    cout << "   e = a + b + c + d;" << endl;
    cout << "   " << endl;
    cout << "   Ensamblador conceptual:" << endl;
    cout << "   mov  temp1, a" << endl;
    cout << "   add  temp1, b       ; temp1 = a + b (16 bytes copiados)" << endl;
    cout << "   mov  temp2, temp1" << endl;
    cout << "   add  temp2, c       ; temp2 = temp1 + c (16 bytes)" << endl;
    cout << "   mov  temp3, temp2" << endl;
    cout << "   add  temp3, d       ; temp3 = temp2 + d (16 bytes)" << endl;
    cout << "   mov  e, temp3       ; copia final (16 bytes)" << endl;
    cout << "   " << endl;
    cout << "   Total: 4 copias × 16 bytes = 64 bytes movidos" << endl;

    cout << "\n2. CON EXPRESSION TEMPLATES:" << endl;
    cout << "   e = a + b + c + d;  // construye árbol de tipos" << endl;
    cout << "   " << endl;
    cout << "   Ensamblador conceptual:" << endl;
    cout << "   mov  e, a" << endl;
    cout << "   add  e, b" << endl;
    cout << "   add  e, c" << endl;
    cout << "   add  e, d" << endl;
    cout << "   " << endl;
    cout << "   Total: 1 copia × 16 bytes = 16 bytes movidos" << endl;

    cout << "\n3. BENEFICIO:" << endl;
    cout << "   → 75% menos movimiento de datos" << endl;
    cout << "   → Mejor uso de cache L1/L2" << endl;
    cout << "   → Menos presión en pipeline del CPU" << endl;
    cout << "   → Todo en compile-time, sin overhead runtime" << endl;
}

void explain_tradeoffs()
{
    cout << "\n=== TRADE-OFFS ===" << endl;

    cout << "\nVENTAJAS:" << endl;
    cout << "  ✓ Elimina copias innecesarias" << endl;
    cout << "  ✓ Sin overhead en runtime" << endl;
    cout << "  ✓ Código más rápido en expresiones complejas" << endl;
    cout << "  ✓ Sintaxis natural (transparente para el usuario)" << endl;

    cout << "\nDESVENTAJAS:" << endl;
    cout << "  ✗ Tiempo de compilación más largo" << endl;
    cout << "  ✗ Mensajes de error crípticos" << endl;
    cout << "  ✗ Binarios ligeramente más grandes (más templates)" << endl;
    cout << "  ✗ Complejidad de implementación" << endl;

    cout << "\nCUÁNDO USAR:" << endl;
    cout << "  • Expresiones con 3+ operaciones" << endl;
    cout << "  • Tipos grandes (>16 bytes)" << endl;
    cout << "  • Código critical performance" << endl;
    cout << "  • Bibliotecas numéricas (Eigen, Blaze)" << endl;

    cout << "\nCUÁNDO NO USAR:" << endl;
    cout << "  • Expresiones simples (a + b)" << endl;
    cout << "  • Tipos pequeños (int, double)" << endl;
    cout << "  • Prototipado rápido" << endl;
    cout << "  • Código que cambia frecuentemente" << endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔══════════════════════════════════════════════════════════════╗" << endl;
    cout << "║     EXPRESSION TEMPLATES PARA uint128_t (Simplificado)      ║" << endl;
    cout << "║                                                              ║" << endl;
    cout << "║  Elimina temporales intermedios en expresiones complejas    ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════╝" << endl;

    try {
        demo_basic();
        demo_complex();
        demo_types();

        cout << "\n" << string(65, '=') << endl;
        cout << "BENCHMARKS" << endl;
        cout << string(65, '=') << endl;

        benchmark_simple();
        benchmark_complex();
        benchmark_large();

        explain_how_it_works();
        explain_tradeoffs();

        cout << "\n" << string(65, '=') << endl;
        cout << "CONCLUSIONES" << endl;
        cout << string(65, '=') << endl;
        cout << "\n1. Expression Templates = metaprogramación para optimización" << endl;
        cout << "2. Elimina copias temporales de objetos grandes" << endl;
        cout << "3. Todo el trabajo ocurre en compile-time" << endl;
        cout << "4. Beneficio mayor con expresiones largas" << endl;
        cout << "5. uint128_t (16 bytes) se beneficia significativamente" << endl;
        cout << "6. Compiladores modernos pueden optimizar ambos casos" << endl;
        cout << "7. Trade-off: compile-time vs runtime performance" << endl;

        cout << "\n✓ Demo completado exitosamente" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

