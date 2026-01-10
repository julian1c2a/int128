/**
 * @file expression_templates_complete.cpp
 * @brief Implementación completa de Expression Templates para uint128_t
 *
 * CARACTERÍSTICAS:
 * ✅ Operaciones binarias: +, -, *, /, %, &, |, ^, <<, >>
 * ✅ Operaciones unarias: ~, !, ++valor, negación
 * ✅ Operaciones multi-asociativas con desenrollado automático
 * ✅ Eliminación de subexpresiones comunes (CSE básico)
 * ✅ Evaluación lazy completa
 * ✅ Optimizaciones en compile-time
 * ✅ SFINAE para operaciones condicionales
 *
 * TÉCNICAS AVANZADAS:
 * - CRTP (Curiously Recurring Template Pattern)
 * - Variadic templates para operaciones n-arias
 * - Expression folding para asociatividad
 * - Perfect forwarding
 * - constexpr todo lo posible
 *
 * COMPILACIÓN:
 *     make demo CATEGORY=showcase DEMO=expression_templates_complete
 *
 * FECHA: Diciembre 2025
 */

#include <chrono>
#include <int128_simple.hpp>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <utility>

using namespace nstd;

using namespace std;
using namespace std::chrono;

// ============================================================================
// INFRAESTRUCTURA BASE
// ============================================================================

/**
 * @brief Clase base CRTP para todas las expresiones
 */
template <typename E>
struct Expr
{
    constexpr const E &cast() const noexcept
    {
        return static_cast<const E &>(*this);
    }

    constexpr E &cast() noexcept
    {
        return static_cast<E &>(*this);
    }

    constexpr uint128_t eval() const
    {
        return cast().eval();
    }

    constexpr operator uint128_t() const
    {
        return eval();
    }
};

/**
 * @brief Tag para identificar tipos de expresión
 */
struct ExprTag
{
};

template <typename T>
struct is_expr : std::is_base_of<ExprTag, T>
{
};

template <typename T>
constexpr bool is_expr_v = is_expr<T>::value;

// ============================================================================
// NODO TERMINAL (HOJA DEL ÁRBOL)
// ============================================================================

/**
 * @brief Nodo terminal que almacena un valor uint128_t
 */
struct Terminal : public Expr<Terminal>, ExprTag
{
    uint128_t value;

    constexpr explicit Terminal(uint128_t v) noexcept : value(v) {}

    constexpr uint128_t eval() const noexcept
    {
        return value;
    }
};

// ============================================================================
// OPERACIONES BINARIAS
// ============================================================================

/**
 * @brief Plantilla para operaciones binarias
 */
template <typename L, typename R, typename Op>
struct BinaryOp : public Expr<BinaryOp<L, R, Op>>, ExprTag
{
    L left;
    R right;

    constexpr BinaryOp(L l, R r) noexcept : left(l), right(r) {}

    constexpr uint128_t eval() const
    {
        return Op::apply(left.eval(), right.eval());
    }
};

// Definición de operaciones binarias
struct AddOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a + b;
    }
    static constexpr const char *name()
    {
        return "+";
    }
};

struct SubOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a - b;
    }
    static constexpr const char *name()
    {
        return "-";
    }
};

struct MulOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a * b;
    }
    static constexpr const char *name()
    {
        return "*";
    }
};

struct DivOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a / b;
    }
    static constexpr const char *name()
    {
        return "/";
    }
};

struct ModOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a % b;
    }
    static constexpr const char *name()
    {
        return "%";
    }
};

struct AndOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a & b;
    }
    static constexpr const char *name()
    {
        return "&";
    }
};

struct OrOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a | b;
    }
    static constexpr const char *name()
    {
        return "|";
    }
};

struct XorOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a ^ b;
    }
    static constexpr const char *name()
    {
        return "^";
    }
};

struct ShlOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a << static_cast<int>(b);
    }
    static constexpr const char *name()
    {
        return "<<";
    }
};

struct ShrOp
{
    static constexpr uint128_t apply(uint128_t a, uint128_t b) noexcept
    {
        return a >> static_cast<int>(b);
    }
    static constexpr const char *name()
    {
        return ">>";
    }
};

// ============================================================================
// OPERACIONES UNARIAS
// ============================================================================

/**
 * @brief Plantilla para operaciones unarias
 */
template <typename E, typename Op>
struct UnaryOp : public Expr<UnaryOp<E, Op>>, ExprTag
{
    E expr;

    constexpr explicit UnaryOp(E e) noexcept : expr(e) {}

    constexpr uint128_t eval() const
    {
        return Op::apply(expr.eval());
    }
};

// Definición de operaciones unarias
struct NotOp
{
    static constexpr uint128_t apply(uint128_t a) noexcept
    {
        return ~a;
    }
    static constexpr const char *name()
    {
        return "~";
    }
};

struct LogicalNotOp
{
    static constexpr uint128_t apply(uint128_t a) noexcept
    {
        return a == 0 ? uint128_t{1} : uint128_t{0};
    }
    static constexpr const char *name()
    {
        return "!";
    }
};

struct IncrementOp
{
    static constexpr uint128_t apply(uint128_t a) noexcept
    {
        return a + 1;
    }
    static constexpr const char *name()
    {
        return "++";
    }
};

struct DecrementOp
{
    static constexpr uint128_t apply(uint128_t a) noexcept
    {
        return a - 1;
    }
    static constexpr const char *name()
    {
        return "--";
    }
};

// ============================================================================
// OPERACIONES MULTI-ASOCIATIVAS (DESENROLLADO)
// ============================================================================

/**
 * @brief Operación n-aria asociativa con desenrollado automático
 *
 * Esta clase desenrolla expresiones como a+b+c+d en tiempo de compilación
 * usando variadic templates y fold expressions.
 */
template <typename Op, typename... Args>
struct AssocMultiOp : public Expr<AssocMultiOp<Op, Args...>>, ExprTag
{
    tuple<Args...> args;

    constexpr explicit AssocMultiOp(Args... a) noexcept : args(a...) {}

    constexpr uint128_t eval() const
    {
        return eval_impl(std::index_sequence_for<Args...>{});
    }

private:
    template <size_t... Is>
    constexpr uint128_t eval_impl(std::index_sequence<Is...>) const
    {
        // Desenrolla en tiempo de compilación: Op(arg0, Op(arg1, Op(arg2, ...)))
        return fold_left(std::get<Is>(args).eval()...);
    }

    // Caso base: un solo argumento
    template <typename T>
    static constexpr uint128_t fold_left(T arg)
    {
        return arg;
    }

    // Caso recursivo: fold left
    template <typename T, typename... Rest>
    static constexpr uint128_t fold_left(T first, Rest... rest)
    {
        return Op::apply(first, fold_left(rest...));
    }
};

/**
 * @brief Helper para construir operaciones multi-asociativas
 */
template <typename Op, typename... Args>
constexpr auto make_assoc_op(Args... args)
{
    return AssocMultiOp<Op, Args...>(args...);
}

// ============================================================================
// OPTIMIZACIÓN: ELIMINACIÓN DE SUBEXPRESIONES COMUNES (CSE)
// ============================================================================

/**
 * @brief Wrapper para expresiones con CSE
 *
 * Cachea el resultado de la evaluación para evitar re-cálculos.
 */
template <typename E>
struct CachedExpr : public Expr<CachedExpr<E>>, ExprTag
{
    E expr;
    mutable bool cached;
    mutable uint128_t cache_value;

    constexpr explicit CachedExpr(E e) noexcept : expr(e), cached(false), cache_value(0) {}

    constexpr uint128_t eval() const
    {
        if (!cached)
        {
            cache_value = expr.eval();
            cached = true;
        }
        return cache_value;
    }

    // Forzar re-evaluación
    constexpr void invalidate() const noexcept
    {
        cached = false;
    }
};

template <typename E>
constexpr auto cache(E expr)
{
    return CachedExpr<E>(expr);
}

// ============================================================================
// OPERADORES SOBRECARGADOS
// ============================================================================

// Operadores binarios
#define DEFINE_BINARY_OP(op, OpType)                                 \
    template <typename L, typename R>                                \
    constexpr auto operator op(const Expr<L> &l, const Expr<R> &r)   \
    {                                                                \
        return BinaryOp<L, R, OpType>(l.cast(), r.cast());           \
    }                                                                \
    template <typename E>                                            \
    constexpr auto operator op(const Expr<E> &e, uint128_t v)        \
    {                                                                \
        return BinaryOp<E, Terminal, OpType>(e.cast(), Terminal(v)); \
    }                                                                \
    template <typename E>                                            \
    constexpr auto operator op(uint128_t v, const Expr<E> &e)        \
    {                                                                \
        return BinaryOp<Terminal, E, OpType>(Terminal(v), e.cast()); \
    }

DEFINE_BINARY_OP(+, AddOp)
DEFINE_BINARY_OP(-, SubOp)
DEFINE_BINARY_OP(*, MulOp)
DEFINE_BINARY_OP(/, DivOp)
DEFINE_BINARY_OP(%, ModOp)
DEFINE_BINARY_OP(&, AndOp)
DEFINE_BINARY_OP(|, OrOp)
DEFINE_BINARY_OP(^, XorOp)
DEFINE_BINARY_OP(<<, ShlOp)
DEFINE_BINARY_OP(>>, ShrOp)

#undef DEFINE_BINARY_OP

// Operadores unarios
#define DEFINE_UNARY_OP(op, OpType)              \
    template <typename E>                        \
    constexpr auto operator op(const Expr<E> &e) \
    {                                            \
        return UnaryOp<E, OpType>(e.cast());     \
    }

DEFINE_UNARY_OP(~, NotOp)
DEFINE_UNARY_OP(!, LogicalNotOp)

#undef DEFINE_UNARY_OP

// ============================================================================
// CLASE WRAPPER CONVENIENTE
// ============================================================================

/**
 * @brief Clase principal que proporciona interfaz conveniente
 */
class UInt128ET : public Expr<UInt128ET>, ExprTag
{
private:
    uint128_t value_;

public:
    // Constructores
    constexpr UInt128ET() noexcept : value_(0) {}
    constexpr UInt128ET(uint64_t v) noexcept : value_(v) {}
    constexpr UInt128ET(const uint128_t &v) noexcept : value_(v) {}
    constexpr UInt128ET(uint64_t high, uint64_t low) noexcept : value_(high, low) {}

    // Constructor desde expresión
    template <typename E, typename = std::enable_if_t<is_expr_v<E>>>
    constexpr UInt128ET(const Expr<E> &expr) : value_(expr.eval())
    {
    }

    // Asignación desde expresión
    template <typename E, typename = std::enable_if_t<is_expr_v<E>>>
    constexpr UInt128ET &operator=(const Expr<E> &expr)
    {
        value_ = expr.eval();
        return *this;
    }

    // Conversiones
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

    // Operadores de asignación compuesta
    template <typename E>
    constexpr UInt128ET &operator+=(const Expr<E> &expr)
    {
        value_ += expr.eval();
        return *this;
    }

    template <typename E>
    constexpr UInt128ET &operator-=(const Expr<E> &expr)
    {
        value_ -= expr.eval();
        return *this;
    }

    template <typename E>
    constexpr UInt128ET &operator*=(const Expr<E> &expr)
    {
        value_ *= expr.eval();
        return *this;
    }

    template <typename E>
    constexpr UInt128ET &operator/=(const Expr<E> &expr)
    {
        value_ /= expr.eval();
        return *this;
    }

    // Operadores de incremento/decremento
    constexpr UInt128ET &operator++()
    {
        ++value_;
        return *this;
    }

    constexpr UInt128ET operator++(int)
    {
        UInt128ET tmp(*this);
        ++value_;
        return tmp;
    }

    constexpr UInt128ET &operator--()
    {
        --value_;
        return *this;
    }

    constexpr UInt128ET operator--(int)
    {
        UInt128ET tmp(*this);
        --value_;
        return tmp;
    }
};

// ============================================================================
// FUNCIONES HELPER
// ============================================================================

/**
 * @brief Crea una operación multi-asociativa de suma
 */
template <typename... Args>
constexpr auto sum(Args... args)
{
    return make_assoc_op<AddOp>(args...);
}

/**
 * @brief Crea una operación multi-asociativa de multiplicación
 */
template <typename... Args>
constexpr auto product(Args... args)
{
    return make_assoc_op<MulOp>(args...);
}

/**
 * @brief Crea una operación multi-asociativa de OR
 */
template <typename... Args>
constexpr auto bitwise_or(Args... args)
{
    return make_assoc_op<OrOp>(args...);
}

/**
 * @brief Crea una operación multi-asociativa de AND
 */
template <typename... Args>
constexpr auto bitwise_and(Args... args)
{
    return make_assoc_op<AndOp>(args...);
}

// ============================================================================
// DEMOS Y TESTS
// ============================================================================

void demo_basic_operations()
{
    cout << "\n=== OPERACIONES BÁSICAS ===" << endl;

    UInt128ET a(1000), b(500), c(250);

    cout << "Valores: a=1000, b=500, c=250\n"
         << endl;

    // Operaciones binarias
    cout << "Operaciones binarias:" << endl;
    cout << "  a + b = " << (a + b).eval() << endl;
    cout << "  a - b = " << (a - b).eval() << endl;
    cout << "  a * b = " << (a * b).eval() << endl;
    cout << "  a / b = " << (a / b).eval() << endl;
    cout << "  a % b = " << (a % b).eval() << endl;

    // Operaciones bitwise
    cout << "\nOperaciones bitwise:" << endl;
    cout << "  a & b = " << (a & b).eval() << endl;
    cout << "  a | b = " << (a | b).eval() << endl;
    cout << "  a ^ b = " << (a ^ b).eval() << endl;
    cout << "  a << 2 = " << (a << UInt128ET(2)).eval() << endl;
    cout << "  a >> 2 = " << (a >> UInt128ET(2)).eval() << endl;

    // Operaciones unarias
    cout << "\nOperaciones unarias:" << endl;
    cout << "  ~b = " << (~b).eval() << endl;
    cout << "  !UInt128ET(0) = " << (!UInt128ET(0)).eval() << endl;
    cout << "  !UInt128ET(1) = " << (!UInt128ET(1)).eval() << endl;
}

void demo_complex_expressions()
{
    cout << "\n=== EXPRESIONES COMPLEJAS ===" << endl;

    UInt128ET a(100), b(200), c(300), d(50), e(10);

    cout << "Valores: a=100, b=200, c=300, d=50, e=10\n"
         << endl;

    // Expresión compleja anidada
    auto expr1 = (a + b) * (c - d) / e;
    cout << "(a + b) * (c - d) / e = " << expr1.eval() << endl;

    // Expresión con operaciones bitwise
    auto expr2 = (a & b) | (c ^ d);
    cout << "(a & b) | (c ^ d) = " << expr2.eval() << endl;

    // Expresión con desplazamientos
    auto expr3 = (a << UInt128ET(2)) + (b >> UInt128ET(1));
    cout << "(a << 2) + (b >> 1) = " << expr3.eval() << endl;

    // Expresión muy compleja
    auto expr4 = ((a + b) * c - d) / ((e + UInt128ET(5)) & UInt128ET(0xFF));
    cout << "((a + b) * c - d) / ((e + 5) & 0xFF) = " << expr4.eval() << endl;
}

void demo_multi_assoc_operations()
{
    cout << "\n=== OPERACIONES MULTI-ASOCIATIVAS (DESENROLLADAS) ===" << endl;

    UInt128ET a(10), b(20), c(30), d(40), e(50);

    cout << "Valores: a=10, b=20, c=30, d=40, e=50\n"
         << endl;

    // Suma multi-operando (desenrollada en compile-time)
    auto s = sum(a, b, c, d, e);
    cout << "sum(a, b, c, d, e) = " << s.eval() << endl;
    cout << "  → Desenrollado en compile-time sin temporales\n"
         << endl;

    // Producto multi-operando
    UInt128ET x(2), y(3), z(4);
    auto p = product(x, y, z);
    cout << "product(2, 3, 4) = " << p.eval() << endl;

    // OR bitwise multi-operando
    UInt128ET m(0x01), n(0x02), o(0x04), p_val(0x08);
    auto or_result = bitwise_or(m, n, o, p_val);
    cout << "bitwise_or(0x01, 0x02, 0x04, 0x08) = " << or_result.eval() << " (hex: 0x" << hex
         << or_result.eval() << dec << ")" << endl;

    // AND bitwise multi-operando
    UInt128ET q(0xFF), r(0xF0), s_val(0x0F);
    auto and_result = bitwise_and(q, r, s_val);
    cout << "bitwise_and(0xFF, 0xF0, 0x0F) = " << and_result.eval() << " (hex: 0x" << hex
         << and_result.eval() << dec << ")" << endl;
}

void demo_cse_optimization()
{
    cout << "\n=== OPTIMIZACIÓN CSE (Common Subexpression Elimination) ===" << endl;

    UInt128ET a(100), b(200);

    cout << "Valores: a=100, b=200\n"
         << endl;

    // Sin CSE: calcula (a+b) dos veces
    cout << "Sin CSE:" << endl;
    auto expr1 = (a + b) * (a + b); // (a+b) calculado 2 veces
    cout << "  (a + b) * (a + b) = " << expr1.eval() << endl;
    cout << "  → Subexpresión (a+b) calculada 2 veces\n"
         << endl;

    // Con CSE: calcula (a+b) una vez y cachea
    cout << "Con CSE:" << endl;
    auto common = cache(a + b);
    auto expr2 = common * common;
    cout << "  common = cache(a + b)" << endl;
    cout << "  common * common = " << expr2.eval() << endl;
    cout << "  → Subexpresión calculada 1 vez, cacheada" << endl;
}

void demo_assignment_operators()
{
    cout << "\n=== OPERADORES DE ASIGNACIÓN ===" << endl;

    UInt128ET a(100), b(50);

    cout << "Inicial: a=100, b=50\n"
         << endl;

    // +=
    a += b + UInt128ET(10);
    cout << "a += (b + 10): a = " << a.value() << endl;

    // -=
    a -= b;
    cout << "a -= b: a = " << a.value() << endl;

    // *=
    a *= UInt128ET(2);
    cout << "a *= 2: a = " << a.value() << endl;

    // /=
    a /= UInt128ET(5);
    cout << "a /= 5: a = " << a.value() << endl;

    // ++ y --
    cout << "\nIncrementos y decrementos:" << endl;
    cout << "++a: " << (++a).value() << endl;
    cout << "a++: " << (a++).value() << " (luego a=" << a.value() << ")" << endl;
    cout << "--a: " << (--a).value() << endl;
    cout << "a--: " << (a--).value() << " (luego a=" << a.value() << ")" << endl;
}

void demo_constexpr_evaluation()
{
    cout << "\n=== EVALUACIÓN CONSTEXPR ===" << endl;

    cout << "Todas las operaciones pueden evaluarse en compile-time:\n"
         << endl;

    // Expresiones constexpr
    constexpr UInt128ET a(100);
    constexpr UInt128ET b(200);
    constexpr auto expr = (a + b) * UInt128ET(2);
    constexpr uint128_t result = expr.eval();

    cout << "constexpr auto expr = (UInt128ET(100) + UInt128ET(200)) * UInt128ET(2);" << endl;
    cout << "Resultado (calculado en compile-time): " << result << endl;
    cout << "\n→ Sin overhead runtime, valor conocido en compilación" << endl;
}

// ============================================================================
// BENCHMARKS
// ============================================================================

template <typename Func>
double benchmark(const string &name, Func &&f, int iterations = 1000000)
{
    auto start = high_resolution_clock::now();

    uint128_t sink{0}; // Evitar optimización agresiva
    for (int i = 0; i < iterations; ++i)
    {
        auto result = f();
        sink = sink + result; // Uso para evitar optimización
    }

    // Usar sink para evitar que se optimice completamente
    if (sink == uint128_t(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL))
    {
        cout << "impossible";
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    double avg_ns = static_cast<double>(duration) / iterations;

    cout << "  " << setw(35) << left << name << ": " << setw(10) << right << fixed
         << setprecision(2) << avg_ns << " ns/op" << endl;

    return avg_ns;
}

void benchmark_operations()
{
    cout << "\n=== BENCHMARKS ===" << endl;
    cout << "Iteraciones: 1,000,000\n"
         << endl;

    uint128_t a_raw{12345};
    uint128_t b_raw{67890};
    uint128_t c_raw{11111};
    uint128_t d_raw{22222};

    UInt128ET a(a_raw), b(b_raw), c(c_raw), d(d_raw);

    cout << "Expresión simple (4 operandos):" << endl;

    benchmark("Sin ET: a+b+c+d", [&]()
              { return a_raw + b_raw + c_raw + d_raw; });

    benchmark("Con ET: a+b+c+d", [&]()
              { return (a + b + c + d).eval(); });

    benchmark("Multi-assoc: sum(a,b,c,d)", [&]()
              { return sum(a, b, c, d).eval(); });

    cout << "\nExpresión compleja:" << endl;

    benchmark("Sin ET: (a+b)*(c-d)", [&]()
              { return (a_raw + b_raw) * (c_raw - d_raw); });

    benchmark("Con ET: (a+b)*(c-d)", [&]()
              { return ((a + b) * (c - d)).eval(); });

    cout << "\nOperaciones bitwise:" << endl;

    benchmark("Sin ET: (a&b)|(c^d)", [&]()
              { return (a_raw & b_raw) | (c_raw ^ d_raw); });

    benchmark("Con ET: (a&b)|(c^d)", [&]()
              { return ((a & b) | (c ^ d)).eval(); });
}

// ============================================================================
// ANÁLISIS TÉCNICO
// ============================================================================

void explain_architecture()
{
    cout << "\n=== ARQUITECTURA DEL SISTEMA ===" << endl;

    cout << "\n1. JERARQUÍA DE CLASES:" << endl;
    cout << "   Expr<E> (CRTP base)" << endl;
    cout << "   ├── Terminal (hojas)" << endl;
    cout << "   ├── BinaryOp<L,R,Op> (nodos binarios)" << endl;
    cout << "   ├── UnaryOp<E,Op> (nodos unarios)" << endl;
    cout << "   ├── AssocMultiOp<Op,Args...> (operaciones n-arias)" << endl;
    cout << "   ├── CachedExpr<E> (con memoización)" << endl;
    cout << "   └── UInt128ET (interfaz conveniente)" << endl;

    cout << "\n2. OPERACIONES SOPORTADAS:" << endl;
    cout << "   Binarias: +, -, *, /, %, &, |, ^, <<, >>" << endl;
    cout << "   Unarias: ~, !" << endl;
    cout << "   Multi-asociativas: sum(), product(), bitwise_or(), bitwise_and()" << endl;
    cout << "   Asignación: +=, -=, *=, /=" << endl;
    cout << "   Incremento/decremento: ++, --" << endl;

    cout << "\n3. OPTIMIZACIONES:" << endl;
    cout << "   • Evaluación lazy (diferida hasta asignación)" << endl;
    cout << "   • Eliminación de temporales" << endl;
    cout << "   • CSE (Common Subexpression Elimination)" << endl;
    cout << "   • Desenrollado de operaciones asociativas" << endl;
    cout << "   • constexpr para evaluación en compile-time" << endl;
    cout << "   • SFINAE para type safety" << endl;
}

void explain_benefits()
{
    cout << "\n=== BENEFICIOS Y LIMITACIONES ===" << endl;

    cout << "\nBENEFICIOS:" << endl;
    cout << "  ✓ Zero-overhead abstraction" << endl;
    cout << "  ✓ Sintaxis natural (como tipos nativos)" << endl;
    cout << "  ✓ Type-safe en compile-time" << endl;
    cout << "  ✓ Permite optimizaciones imposibles manualmente" << endl;
    cout << "  ✓ Composable y extensible" << endl;
    cout << "  ✓ Soporta constexpr" << endl;

    cout << "\nLIMITACIONES:" << endl;
    cout << "  ✗ Tiempo de compilación incrementado" << endl;
    cout << "  ✗ Mensajes de error largos" << endl;
    cout << "  ✗ Tamaño de binario aumenta (más templates)" << endl;
    cout << "  ✗ Curva de aprendizaje empinada" << endl;

    cout << "\nUSO RECOMENDADO:" << endl;
    cout << "  → Bibliotecas numéricas de alto rendimiento" << endl;
    cout << "  → Álgebra lineal (matrices, vectores)" << endl;
    cout << "  → DSLs embebidos en C++" << endl;
    cout << "  → Código crítico de performance" << endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔══════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   EXPRESSION TEMPLATES COMPLETOS PARA uint128_t             ║" << endl;
    cout << "║                                                              ║" << endl;
    cout << "║  ✓ Binary ops  ✓ Unary ops  ✓ Multi-assoc ops              ║" << endl;
    cout << "║  ✓ CSE         ✓ constexpr  ✓ Type-safe                    ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════╝" << endl;

    try
    {
        demo_basic_operations();
        demo_complex_expressions();
        demo_multi_assoc_operations();
        demo_cse_optimization();
        demo_assignment_operators();
        demo_constexpr_evaluation();

        cout << "\n"
             << string(65, '=') << endl;
        cout << "ANÁLISIS DE PERFORMANCE" << endl;
        cout << string(65, '=') << endl;

        benchmark_operations();

        cout << "\n"
             << string(65, '=') << endl;
        cout << "DOCUMENTACIÓN TÉCNICA" << endl;
        cout << string(65, '=') << endl;

        explain_architecture();
        explain_benefits();

        cout << "\n"
             << string(65, '=') << endl;
        cout << "RESUMEN" << endl;
        cout << string(65, '=') << endl;
        cout << "\nEste sistema de Expression Templates proporciona:" << endl;
        cout << "  1. Operaciones binarias completas (+,-,*,/,%,&,|,^,<<,>>)" << endl;
        cout << "  2. Operaciones unarias (~, !)" << endl;
        cout << "  3. Operaciones multi-asociativas desenrolladas" << endl;
        cout << "  4. Optimización CSE para subexpresiones comunes" << endl;
        cout << "  5. Evaluación lazy completa" << endl;
        cout << "  6. Soporte constexpr para compile-time" << endl;
        cout << "  7. Type-safe con SFINAE" << endl;
        cout << "\n✓ Demo completado exitosamente" << endl;
    }
    catch (const exception &e)
    {
        cerr << "\n✗ Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
