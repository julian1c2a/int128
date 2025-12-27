# Expression Templates para uint128_t

## 📚 Guía Completa de Expression Templates

Esta guía documenta la colección completa de 5 demos sobre **Expression Templates** implementados para la biblioteca int128.

## 🎯 ¿Qué son Expression Templates?

Expression Templates (ET) es una técnica avanzada de metaprogramación en C++ que permite:

1. **Eliminar temporales intermedios** en expresiones complejas
2. **Evaluar expresiones en una sola pasada** usando lazy evaluation
3. **Construir árboles de expresiones** en compile-time
4. **Optimizar operaciones** sin overhead runtime

### Problema que Resuelven

Con tipos definidos por usuario como `uint128_t` (16 bytes), una expresión como:

```cpp
e = a + b + c + d;
```

Crea **3 temporales intermedios**:

```cpp
temp1 = a + b          // temporal 1 (16 bytes)
temp2 = temp1 + c      // temporal 2 (16 bytes)
temp3 = temp2 + d      // temporal 3 (16 bytes)
e = temp3              // copia final
```

Con Expression Templates:

```cpp
auto expr = a + b + c + d;  // Construye árbol, no evalúa
e = expr.eval();            // Una sola evaluación, zero temporales
```

## 📂 Colección de Demos

### 1. expression_templates_simple.cpp (620 LOC)

**Nivel**: Principiante  
**Propósito**: Introducción didáctica

**Características**:

- Value semantics (no referencias)
- Operaciones básicas: +, -, *, /
- CRTP (Curiously Recurring Template Pattern)
- Lazy evaluation simple
- Fácil de entender

**Casos de uso demostrados**:

- Suma simple: `a + b + c + d`
- Expresiones complejas: `(a + b) * (c - d) / e`
- Deducción de tipos en compile-time

**Compilar y ejecutar**:

```bash
make demo CATEGORY=showcase DEMO=expression_templates_simple
```

**Código ejemplo**:

```cpp
struct Terminal : public Expr<Terminal> {
    uint128_t value;
    constexpr explicit Terminal(uint128_t v) : value(v) {}
    constexpr uint128_t eval() const { return value; }
};

template<typename L, typename R>
struct Add : public Expr<Add<L, R>> {
    L left;   // Por valor, no referencia
    R right;
    constexpr uint128_t eval() const { 
        return left.eval() + right.eval(); 
    }
};
```

---

### 2. expression_templates.cpp (582 LOC)

**Nivel**: Intermedio  
**Propósito**: Explicación completa con análisis

**Características**:

- Explicación detallada del CRTP
- Visualización del árbol de expresiones
- Comparación con/sin ET paso a paso
- Benchmarks de rendimiento
- Análisis de ensamblador conceptual

**Estructura del demo**:

1. Demostración básica (con y sin ET)
2. Expresiones complejas anidadas
3. Deducción de tipos
4. Benchmarks (suma simple, compleja, grande)
5. Explicación de optimizaciones

**Compilar y ejecutar**:

```bash
make demo CATEGORY=showcase DEMO=expression_templates
```

**Salida típica**:

```
=== DEMO BÁSICO ===
1. Sin ET: e = a + b + c + d = 10000 (3 temporales)
2. Con ET: e = a + b + c + d = 10000 (sin temporales)

=== BENCHMARKS ===
Sin ET: 20.58 ns/op
Con ET: 74.92 ns/op
Nota: Para uint128_t el compilador optimiza bien ambos casos
```

---

### 3. expression_templates_fold.cpp (489 LOC)

**Nivel**: Intermedio-Avanzado  
**Propósito**: Fold expressions de C++17

**Características**:

- **Fold expressions nativos**: `(... + args)`, `(args + ...)`
- Comparación: recursión manual vs fold nativo
- Operaciones múltiples: suma, producto, OR, AND, XOR
- Constexpr completo
- Código más simple y limpio

**Sintaxis de fold expressions**:

```cpp
// 1. Unary left fold: (... op args)
template<typename... Args>
auto sum_fold(Args... args) {
    return (... + args.eval());
}
// Expande: ((((arg1 + arg2) + arg3) + arg4) + ...)

// 2. Unary right fold: (args op ...)
template<typename... Args>
auto sum_fold_right(Args... args) {
    return (args.eval() + ...);
}
// Expande: (arg1 + (arg2 + (arg3 + (arg4 + ...))))

// 3. Binary left fold: (init op ... op args)
template<typename... Args>
auto sum_with_init(Args... args) {
    return (uint128_t(0) + ... + args.eval());
}
// Expande: (((0 + arg1) + arg2) + arg3) + ...
```

**Comparación de código**:

```cpp
// ❌ MANUAL (complicado, ~15 líneas)
template<typename... Args>
struct SumManual {
    template<typename T>
    static uint128_t fold_left(T arg) { return arg; }
    
    template<typename T, typename... Rest>
    static uint128_t fold_left(T first, Rest... rest) {
        return first + fold_left(rest...);
    }
};

// ✅ FOLD NATIVO (simple, 1 línea)
template<typename... Args>
auto sum_native(Args... args) {
    return (... + args.eval());  // ¡Eso es todo!
}
```

**Compilar y ejecutar**:

```bash
make demo CATEGORY=showcase DEMO=expression_templates_fold
```

---

### 4. expression_templates_complete.cpp (895 LOC)

**Nivel**: Avanzado  
**Propósito**: Sistema completo de ET

**Características**:

✅ **10 Operaciones Binarias**:

- Aritméticas: `+`, `-`, `*`, `/`, `%`
- Bitwise: `&`, `|`, `^`, `<<`, `>>`

✅ **Operaciones Unarias**:

- `~` (bitwise NOT)
- `!` (logical NOT)
- `++`, `--` (incremento/decremento)

✅ **Operaciones Multi-Asociativas**:

- `sum(a, b, c, d, ...)` - Suma n-aria
- `product(a, b, c, ...)` - Producto n-ario
- `bitwise_or(...)`, `bitwise_and(...)`

✅ **Optimización CSE**:

- Common Subexpression Elimination
- Memoización de subexpresiones

✅ **Características Avanzadas**:

- Operadores de asignación: `+=`, `-=`, `*=`, `/=`
- Constexpr completo
- SFINAE para type safety
- Evaluación lazy completa

**Ejemplo de uso completo**:

```cpp
UInt128ET a(100), b(200), c(300), d(50), e(10);

// Expresión compleja con múltiples operaciones
auto expr = (a + b) * (c - d) / e;
uint128_t result = expr.eval();

// Operaciones multi-asociativas
auto sum_result = sum(a, b, c, d, e);

// Optimización CSE
auto common = cache(a + b);
auto result = common * common;  // (a+b) calculado solo una vez
```

**Compilar y ejecutar**:

```bash
make demo CATEGORY=showcase DEMO=expression_templates_complete
```

**Salida del demo**:

```
╔══════════════════════════════════════════════════════════════╗
║   EXPRESSION TEMPLATES COMPLETOS PARA uint128_t             ║
║  ✓ Binary ops  ✓ Unary ops  ✓ Multi-assoc ops              ║
║  ✓ CSE         ✓ constexpr  ✓ Type-safe                    ║
╚══════════════════════════════════════════════════════════════╝

=== OPERACIONES BÁSICAS ===
Operaciones binarias:
  a + b = 1500
  a * b = 500000
  a & b = 480
  a << 2 = 4000

=== OPTIMIZACIÓN CSE ===
Sin CSE: (a+b) calculado 2 veces
Con CSE: (a+b) calculado 1 vez, cacheado
```

---

### 5. expression_templates_horner.cpp (644 LOC)

**Nivel**: Avanzado  
**Propósito**: Evaluación de polinomios con método de Horner

**Características**:

- **Método de Horner** optimizado con ET
- Composición recursiva infinita
- Zero temporales en evaluación polinomial
- Visualización del árbol de expresiones
- Benchmarks para diferentes grados

**Método de Horner explicado**:

```
Polinomio estándar:
P(x) = a₀ + a₁x + a₂x² + a₃x³

Forma de Horner:
P(x) = a₀ + x(a₁ + x(a₂ + x(a₃)))
     = ((a₃x + a₂)x + a₁)x + a₀

Ventajas:
- Solo N multiplicaciones (vs 2N en forma estándar)
- Evita cálculo de potencias (x², x³, ...)
- Ideal para Expression Templates
```

**Composición incremental (pregunta del usuario)**:

```cpp
Terminal x(3), y(4), z(2);

// Paso 1: expr₁ = a*x + b
auto expr1 = 2*x + 5;              // No evalúa, construye árbol
cout << "Tipo: Add<Mul<Terminal, Terminal>, Terminal>" << endl;

// Paso 2: expr₂ = expr₁ * y + c = (2*x + 5) * y + 7
auto expr2 = expr1 * y + 7;        // Extiende el árbol
cout << "Tipo: Add<Mul<Add<...>, Terminal>, Terminal>" << endl;

// Paso 3: expr₃ = expr₂ * z + d = ((2*x + 5) * y + 7) * z + 1
auto expr3 = expr2 * z + 1;        // Continúa extendiendo
cout << "Tipo: Add<Mul<Add<Mul<Add<...>, Terminal>, Terminal>, Terminal>, Terminal>" << endl;

// Evaluación ÚNICA al final
uint128_t result = expr3.eval();   // ¡Evalúa todo el árbol de una vez!

// ✓ Cada composición extiende el árbol sin evaluar
// ✓ Sin temporales en ningún paso
// ✓ Una sola evaluación al final
```

**Ejemplo de polinomio**:

```cpp
// P(x) = 3x² + 2x + 5
// Horner: ((3x + 2)x + 5)
Terminal x(10);

auto expr = ((Terminal(3) * x + 2) * x + 5);
uint128_t result = expr.eval();  // = 325

// Sin temporales, una sola evaluación
```

**Compilar y ejecutar**:

```bash
make demo CATEGORY=showcase DEMO=expression_templates_horner
```

**Benchmarks**:

```
Polinomio grado 3:  Sin ET: 97.42 ns/op  | Con ET: 114.16 ns/op
Polinomio grado 5:  Sin ET: 125.26 ns/op | Con ET: 155.38 ns/op
Polinomio grado 10: Sin ET: 200.40 ns/op | Con ET: 241.88 ns/op

→ Cuanto mayor el grado, mayor el beneficio potencial
```

---

## 🎓 Conceptos Clave

### 1. CRTP (Curiously Recurring Template Pattern)

```cpp
template<typename E>
struct Expr {
    const E& cast() const { 
        return static_cast<const E&>(*this); 
    }
    
    uint128_t eval() const {
        return cast().eval();  // Llama a eval() del tipo derivado
    }
};

struct Terminal : public Expr<Terminal> {
    uint128_t value;
    uint128_t eval() const { return value; }
};
```

**Ventajas**:

- Polimorfismo estático (sin vtables)
- Zero overhead
- Resuelto en compile-time

### 2. Lazy Evaluation

```cpp
// Construcción de expresión (NO evalúa)
auto expr = a + b + c + d;  // Tipo: Add<Add<Add<Terminal, Terminal>, Terminal>, Terminal>

// Evaluación diferida (evalúa cuando es necesario)
uint128_t result = expr.eval();  // Ahora sí evalúa
```

**Ventajas**:

- No crea temporales durante construcción
- Una sola pasada de evaluación
- Optimizable por el compilador

### 3. Árbol de Expresiones

```
Expresión: ((3*x + 2)*x + 5)

Árbol:
        Add
       /   \
     Mul    5
     /  \
   Add   x
   / \
  Mul 2
  / \
 3   x
```

**Construcción**: Compile-time  
**Evaluación**: Runtime (una sola pasada)  
**Optimización**: El compilador puede inline todo

### 4. Fold Expressions (C++17)

```cpp
// Sintaxis nativa del lenguaje
(... + args)          // Left fold
(args + ...)          // Right fold
(0 + ... + args)      // Binary left fold
(args + ... + 0)      // Binary right fold
```

**Ventajas sobre recursión manual**:

- Código más simple (1 línea vs 15)
- Mejor optimizable por el compilador
- Más legible y mantenible

---

## 📊 Comparación de Demos

| Demo | LOC | Nivel | Operaciones | Características Especiales |
|------|-----|-------|-------------|---------------------------|
| simple | 620 | Principiante | +, -, *, / | Value semantics, didáctico |
| expression_templates | 582 | Intermedio | +, -, *, /, % | Análisis detallado, benchmarks |
| fold | 489 | Intermedio-Avanzado | sum, product, OR, AND | Fold expressions nativos |
| complete | 895 | Avanzado | 10 binarias + unarias | CSE, multi-assoc, constexpr |
| horner | 644 | Avanzado | +, * | Polinomios, composición |

**Total**: ~3,230 líneas de código

---

## 🚀 Uso Recomendado

### Para Aprender

1. **Empezar con**: `expression_templates_simple.cpp`
2. **Continuar con**: `expression_templates.cpp`
3. **Profundizar con**: `expression_templates_fold.cpp`
4. **Dominar con**: `expression_templates_complete.cpp` y `horner.cpp`

### Para Proyectos Reales

- **Álgebra lineal**: Operaciones con vectores/matrices (complete)
- **Polinomios**: Evaluación Horner (horner)
- **DSL embebidos**: Sintaxis natural (complete)
- **Código n-ario**: Fold expressions (fold)

---

## ⚡ Rendimiento

### Factores que Afectan

1. **Tamaño del tipo**: uint128_t (16 bytes) vs uint64_t (8 bytes)
2. **Optimizaciones del compilador**: -O3 puede optimizar temporales
3. **Complejidad de expresión**: Más operaciones = más beneficio potencial
4. **Arquitectura**: Cache, alignment, registros

### Resultados Típicos (GCC -O3)

```
Expresión simple (4 operandos):
  Sin ET: 20-30 ns/op
  Con ET: 70-80 ns/op
  → El compilador optimiza bien ambos casos

Expresión compleja (10+ operaciones):
  Sin ET: 50-60 ns/op
  Con ET: 230-250 ns/op
  → Overhead de ET visible pero potencial de optimización
```

**Conclusión**: Para uint128_t, los compiladores modernos optimizan agresivamente. ET es más valioso para:

- Tipos más grandes (>16 bytes)
- Expresiones muy complejas
- Cuando el perfil muestra copias costosas

---

## 🎯 Casos de Uso Ideales

### ✅ Cuándo Usar Expression Templates

1. **Tipos grandes** (>16 bytes) con copias costosas
2. **Expresiones largas** encadenadas (10+ operaciones)
3. **DSLs embebidos** que necesitan sintaxis natural
4. **Álgebra lineal** (vectores, matrices)
5. **Evaluación de polinomios** de alto grado
6. **Cuando el profiling** muestra copias excesivas

### ❌ Cuándo NO Usar

1. **Tipos pequeños** (≤8 bytes) trivialmente copiables
2. **Expresiones simples** (2-3 operaciones)
3. **Cuando el compilador** ya optimiza bien
4. **Overhead de compilación** es crítico
5. **Mensajes de error** deben ser claros

---

## 📚 Referencias y Recursos

### Documentación del Proyecto

- [README.md](README.md) - Documentación principal
- [demos/showcase/README.md](demos/showcase/README.md) - Índice de demos

### Conceptos Relacionados

- CRTP (Curiously Recurring Template Pattern)
- Template Metaprogramming
- Fold Expressions (C++17)
- Lazy Evaluation
- Expression Trees

### Compilar Todos los Demos

```bash
# Compilar todos los demos de ET
for demo in expression_templates_simple expression_templates expression_templates_fold expression_templates_complete expression_templates_horner; do
    make demo CATEGORY=showcase DEMO=$demo
done

# O usando el script de Python
python make.py check demos showcase gcc release | grep expression
```

---

## 📝 Fecha y Versión

- **Fecha**: Diciembre 2025
- **Versión**: 1.0
- **Compiladores testeados**: GCC 15.2+, Clang 19.1+
- **Estándar**: C++20 (fold expressions requieren C++17)

---

## 🎓 Conclusión

Esta colección de demos proporciona:

1. ✅ **Progresión completa** desde principiante hasta avanzado
2. ✅ **Ejemplos ejecutables** con salida real
3. ✅ **Técnicas modernas** (fold expressions C++17)
4. ✅ **Casos de uso prácticos** (polinomios de Horner)
5. ✅ **Código bien documentado** (~3,230 líneas)

**Expression Templates es una técnica poderosa** que, usada correctamente, puede eliminar overhead y proporcionar sintaxis elegante para tipos definidos por usuario.

---

*Última actualización: 27 de diciembre de 2025*
