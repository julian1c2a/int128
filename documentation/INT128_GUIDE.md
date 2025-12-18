# int128_t Complete Guide

Guía completa de `int128_t`: entero con signo de 128 bits con aritmética two's complement.

## 📋 Tabla de Contenidos

1. [Descripción General](#descripción-general)
2. [Signed vs Unsigned](#signed-vs-unsigned)
3. [Representación Two's Complement](#representación-twos-complement)
4. [Sign Extension](#sign-extension)
5. [Operaciones Signed](#operaciones-signed)
6. [Benchmarks](#benchmarks)
7. [Comparaciones de Rendimiento](#comparaciones-de-rendimiento)
8. [Uso Práctico](#uso-práctico)

---

## Descripción General

`int128_t` es una implementación de entero con signo de 128 bits que complementa `uint128_t`. Proporciona:

- **Rango**: -2^127 a 2^127-1 (-170,141,183,460,469,231,731,687,303,715,884,105,728 a 170,141,183,460,469,231,731,687,303,715,884,105,727)
- **Representación**: Two's complement (estándar C++)
- **Compatibilidad**: API similar a `int64_t` y otros tipos signed built-in
- **Backends**: Soporte para `__int128_t` (GCC/Clang) y Boost.Multiprecision

### Archivos Principales

```
include/int128/
├── int128_t.hpp              # Implementación principal
├── int128_algorithm.hpp      # Algoritmos (división, módulo)
├── int128_arithmetic.hpp     # Operaciones aritméticas
├── int128_bitwise.hpp        # Operaciones bitwise
├── int128_comparison.hpp     # Comparaciones
├── int128_io.hpp             # Entrada/salida
├── int128_limits.hpp         # std::numeric_limits
└── int128_traits.hpp         # Type traits

tests/
└── int128_extracted_tests.cpp    # Suite de tests completa

benchmarks/
└── int128_extracted_benchmarks.cpp   # Benchmarks exhaustivos
```

---

## Signed vs Unsigned

### Diferencias Clave

| Aspecto | `uint128_t` (Unsigned) | `int128_t` (Signed) |
|---------|------------------------|---------------------|
| **Rango** | 0 a 2^128-1 | -2^127 a 2^127-1 |
| **Bit más significativo** | Valor | Signo (0=positivo, 1=negativo) |
| **Overflow** | Wrap around (módulo 2^128) | Undefined behavior en C++ |
| **División** | Unsigned (siempre positiva) | Signed (con reglas C++) |
| **Comparaciones** | Unsigned (mayor=más bits) | Signed (negativos < positivos) |
| **Shifts** | Lógicos (rellena con 0) | Aritméticos (preserva signo) |
| **Uso típico** | Hashes, IDs, contadores | Aritmética matemática, coordenadas |

### Ejemplo Visual

```cpp
// uint128_t
uint128_t u1 = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;  // Máximo positivo
uint128_t u2 = u1 + 1;  // Wrap to 0

// int128_t  
int128_t i1 = INT128_MAX;  // 2^127 - 1
int128_t i2 = -1;          // Negativo
bool cmp = (i2 < i1);      // true (negativo < positivo)

// Bit patterns idénticos, interpretación diferente
uint128_t u = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
int128_t  i(u);  // Reinterpretación: i == -1
```

---

## Representación Two's Complement

### Definición

El complemento a dos (two's complement) es el estándar de C++ para enteros con signo:

```
Positivo N:  representación binaria directa
Negativo -N: complemento bit a bit de N, más 1
```

### Estructura de int128_t

```cpp
class int128_t {
    std::uint64_t low_;   // 64 bits bajos (menos significativos)
    std::uint64_t high_;  // 64 bits altos (más significativos)
};
```

**Bit de signo**: MSB (bit más significativo) de `high_`

```
Positivo: high_ MSB = 0
Negativo: high_ MSB = 1
```

### Ejemplos de Representación

```cpp
// Caso 1: Cero
int128_t zero(0);
// low_  = 0x0000000000000000
// high_ = 0x0000000000000000

// Caso 2: Positivo pequeño (42)
int128_t positive(42);
// low_  = 0x000000000000002A
// high_ = 0x0000000000000000

// Caso 3: Negativo (-1)
int128_t neg_one(-1);
// low_  = 0xFFFFFFFFFFFFFFFF
// high_ = 0xFFFFFFFFFFFFFFFF
// Todos los bits en 1

// Caso 4: Negativo (-42)
int128_t neg_forty_two(-42);
// low_  = 0xFFFFFFFFFFFFFFD6  (complemento de 42)
// high_ = 0xFFFFFFFFFFFFFFFF  (sign extension)

// Caso 5: INT128_MIN (-2^127)
int128_t min_value = INT128_MIN;
// low_  = 0x0000000000000000
// high_ = 0x8000000000000000  (solo MSB = 1)

// Caso 6: INT128_MAX (2^127 - 1)
int128_t max_value = INT128_MAX;
// low_  = 0xFFFFFFFFFFFFFFFF
// high_ = 0x7FFFFFFFFFFFFFFF  (MSB = 0, resto = 1)
```

### Detectar Signo

```cpp
bool int128_t::is_negative() const noexcept {
    return (high_ & 0x8000000000000000ULL) != 0;
}

bool int128_t::is_positive() const noexcept {
    return !is_negative() && (*this != 0);
}
```

### Negación (Unary Minus)

```cpp
int128_t int128_t::operator-() const {
    // Two's complement: ~x + 1
    int128_t result;
    result.low_ = ~low_;
    result.high_ = ~high_;
    
    // Incrementar
    result.low_++;
    if (result.low_ == 0) {
        result.high_++;  // Carry propagation
    }
    
    return result;
}

// Ejemplos
int128_t a(42);
int128_t b = -a;  // b == -42

int128_t c(-100);
int128_t d = -c;  // d == 100 (negación de negativo)
```

---

## Sign Extension

### ¿Qué es Sign Extension?

Al convertir un entero signed más pequeño a uno más grande, los bits adicionales se llenan con el bit de signo:

```
Positivo: Llenar con 0
Negativo: Llenar con 1 (0xFF...FF)
```

Esto preserva el valor matemático.

### Implementación

```cpp
// Constructor desde int64_t
constexpr int128_t(int64_t value) noexcept 
    : low_(static_cast<uint64_t>(value))
    , high_(value < 0 ? UINT64_MAX : 0)  // Sign extension
{}

// Ejemplos:
int128_t a(42);          // low=42,    high=0x0000...
int128_t b(-42);         // low=...,   high=0xFFFF...

int128_t c(INT64_MAX);   // Positivo grande
int128_t d(INT64_MIN);   // Negativo grande
```

### Caso Especial: int32_t y tipos más pequeños

```cpp
// Constructor desde int32_t
constexpr int128_t(int32_t value) noexcept 
    : low_(static_cast<uint64_t>(static_cast<int64_t>(value)))
    , high_(value < 0 ? UINT64_MAX : 0)
{}

// Proceso:
// 1. int32_t → int64_t (sign extension automática de C++)
// 2. int64_t → uint64_t (reinterpretación de bits)
// 3. high_ se llena según signo

// Ejemplo: -100 (int32_t)
// Paso 1: -100 (int64_t) = 0xFFFFFFFFFFFFFF9C
// Paso 2: low_ = 0xFFFFFFFFFFFFFF9C
// Paso 3: high_ = 0xFFFFFFFFFFFFFFFF (negativo)
```

### Verificación de Sign Extension

```cpp
// Test: Conversión preserva valor
void test_sign_extension() {
    int64_t values[] = {0, 1, -1, 100, -100, INT64_MAX, INT64_MIN};
    
    for (int64_t val : values) {
        int128_t i(val);
        int64_t back = static_cast<int64_t>(i);
        
        assert(val == back);  // Debe ser idéntico
        
        // Verificar high_
        if (val < 0) {
            assert(i.high() == UINT64_MAX);  // Todos 1s
        } else {
            assert(i.high() == 0);           // Todos 0s
        }
    }
}
```

---

## Operaciones Signed

### Aritmética Signed

#### Suma y Resta

```cpp
// La suma/resta es idéntica a unsigned en two's complement
int128_t a(-50), b(100);
int128_t sum = a + b;      // 50
int128_t diff = a - b;     // -150

// Overflow detection
int128_t max = INT128_MAX;
int128_t overflow = max + 1;  // UB en C++ (nuestro impl: wrap)
```

#### Multiplicación

```cpp
// Multiplicación signed: considerar signos
int128_t a(-10), b(20);
int128_t prod = a * b;  // -200

// Algoritmo simplificado:
int128_t multiply(int128_t a, int128_t b) {
    bool neg_result = a.is_negative() ^ b.is_negative();
    
    // Trabajar con valores absolutos
    uint128_t abs_a = a.is_negative() ? -a : a;
    uint128_t abs_b = b.is_negative() ? -b : b;
    
    uint128_t prod = abs_a * abs_b;  // Multiplicación unsigned
    
    return neg_result ? -prod : prod;
}
```

#### División y Módulo

Las reglas de C++ para división signed:

```cpp
// Truncamiento hacia cero
 7 /  3 =  2,   7 %  3 =  1
-7 /  3 = -2,  -7 %  3 = -1
 7 / -3 = -2,   7 % -3 =  1
-7 / -3 =  2,  -7 % -3 = -1

// Invariante: (a/b)*b + (a%b) == a

// Implementación
int128_t divide_signed(int128_t dividend, int128_t divisor) {
    bool neg_result = dividend.is_negative() ^ divisor.is_negative();
    
    uint128_t abs_dividend = dividend.is_negative() ? -dividend : dividend;
    uint128_t abs_divisor = divisor.is_negative() ? -divisor : divisor;
    
    uint128_t quotient = abs_dividend / abs_divisor;
    
    return neg_result ? -quotient : quotient;
}
```

**Caso especial**: `INT128_MIN / -1`

```cpp
// INT128_MIN = -2^127 (solo representable como negativo)
// -INT128_MIN = 2^127 (NO representable en int128_t)
// Resultado: overflow (UB en C++, nuestra impl: return INT128_MIN)

int128_t min = INT128_MIN;
int128_t result = min / -1;  // UB teórico, práctica: min
```

### Comparaciones Signed

```cpp
// Orden signed: negativos < 0 < positivos
int128_t a(-100), b(50);

assert(a < b);   // -100 < 50
assert(b > a);   // 50 > -100
assert(a < 0);   // Negativo < cero

// Implementación de operator<
bool operator<(int128_t a, int128_t b) {
    // Comparar signos primero
    bool a_neg = a.is_negative();
    bool b_neg = b.is_negative();
    
    if (a_neg != b_neg) {
        return a_neg;  // Negativo < positivo
    }
    
    // Mismo signo: comparar magnitud
    if (a.high() != b.high()) {
        return a_neg ? (a.high() > b.high())  // Neg: mayor bits = menor valor
                     : (a.high() < b.high()); // Pos: menor bits = menor valor
    }
    return a_neg ? (a.low() > b.low())
                 : (a.low() < b.low());
}
```

### Shifts Signed

```cpp
// Left shift: mismo que unsigned
int128_t a(10);
int128_t b = a << 2;  // 40 (multiplica por 4)

// Right shift: aritmético (preserva signo)
int128_t c(-100);
int128_t d = c >> 2;  // -25 (divide por 4, redondea hacia -inf)

// Implementación de operator>>
int128_t operator>>(int128_t value, unsigned shift) {
    if (shift >= 128) return value.is_negative() ? -1 : 0;
    
    if (value.is_negative()) {
        // Arithmetic right shift: rellenar con 1s
        uint128_t unsigned_val(value);
        unsigned_val >>= shift;  // Shift lógico
        
        // Rellenar bits altos con 1s
        if (shift < 64) {
            uint64_t mask = (UINT64_MAX << (64 - shift));
            unsigned_val.high_ |= mask;
        } else {
            unsigned_val.high_ = UINT64_MAX;
            unsigned_val.low_ |= (UINT64_MAX << (128 - shift));
        }
        
        return int128_t(unsigned_val);
    } else {
        // Positivo: shift lógico normal
        return int128_t(uint128_t(value) >> shift);
    }
}
```

### Conversión a String

```cpp
// Signed: manejar signo "-"
std::string int128_t::to_string() const {
    if (*this == 0) return "0";
    
    if (is_negative()) {
        // Negar y agregar "-"
        int128_t positive = -*this;
        return "-" + positive.to_string_unsigned();
    }
    
    return to_string_unsigned();  // Reusa lógica unsigned
}

// Ejemplos:
int128_t a(123);   // "123"
int128_t b(-456);  // "-456"
int128_t c(0);     // "0"
int128_t d(INT128_MIN);  // "-170141183460469231731687303715884105728"
```

### Parsing de String

```cpp
// Signed: detectar "-" inicial
int128_t int128_t::from_string(const char* str) {
    if (!str || *str == '\0') return 0;
    
    bool negative = false;
    if (*str == '-') {
        negative = true;
        ++str;
    } else if (*str == '+') {
        ++str;
    }
    
    uint128_t magnitude = uint128_t::from_string(str);
    
    return negative ? -int128_t(magnitude) : int128_t(magnitude);
}

// Ejemplos:
auto a = int128_t::from_string("123");     // 123
auto b = int128_t::from_string("-456");    // -456
auto c = int128_t::from_string("+789");    // 789
```

---

## Benchmarks

### Estructura de Benchmarks

El sistema de benchmarks de `int128_t` es completamente paralelo al de `uint128_t`:

```
benchmarks/
├── uint128_extracted_benchmarks.cpp   # Benchmarks unsigned
└── int128_extracted_benchmarks.cpp    # Benchmarks signed

scripts/
├── build_benchmarks.bash              # Compila AMBOS
├── run_benchmarks.bash                # Ejecuta AMBOS
├── aggregate_benchmark_results.py     # Agrega AMBOS
├── plot_benchmark_results.py          # Grafica AMBOS
└── build_with_backends.bash           # Compila AMBOS con backends

benchmark_results/
├── uint128_benchmarks_*.csv           # Resultados unsigned
├── int128_benchmarks_*.csv            # Resultados signed
├── consolidated_uint128_*.csv/json/md # Reportes uint128
└── consolidated_int128_*.csv/json/md  # Reportes int128
```

### Operaciones Benchmarked

#### 1. Construcción
```cpp
benchmark_construction() {
    // Default constructor
    int128_t a;
    
    // From int64_t (sign extension)
    int128_t b(-12345);
    
    // From two uint64_t
    int128_t c(0xFFFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF);
}
```

#### 2. Aritmética
```cpp
benchmark_addition() {
    int128_t a(1000000), b(-500000);
    int128_t sum = a + b;  // Mixed signs
}

benchmark_multiplication() {
    int128_t a(-1234), b(5678);
    int128_t prod = a * b;  // Negative result
}

benchmark_division() {
    int128_t a(-1000000), b(7);
    int128_t quot = a / b;  // Signed division
    int128_t rem = a % b;   // Signed modulo
}
```

#### 3. Comparaciones
```cpp
benchmark_comparisons() {
    int128_t a(-100), b(50), c(-200);
    
    bool eq = (a == b);   // false
    bool lt = (a < 0);    // true (negative)
    bool gt = (b > c);    // true (positive > negative)
}
```

#### 4. Conversiones
```cpp
benchmark_string_conversion() {
    // To string (con signo)
    int128_t a(-123456789);
    std::string str = a.to_string();  // "-123456789"
    
    // From string
    int128_t b = int128_t::from_string("-987654321");
}
```

### Tipos Comparados

| Tipo | Descripción | Disponibilidad |
|------|-------------|----------------|
| `int128_t` | Nuestra implementación | Siempre |
| `int64_t` | Baseline (nativo) | Siempre |
| `int32_t` | Baseline pequeño | Siempre |
| `__int128_t` | Compilador nativo signed | GCC/Clang (no MSVC) |
| `boost::int128_t` | Boost cpp_int | Con `--with-boost` |
| `boost_int128_gmp` | Boost + GMP backend | Con `--with-gmp` |
| `boost_int128_tommath` | Boost + tommath | Con `--with-tommath` |

### Compilación

```bash
# Básico: solo int128_t vs int64_t
./scripts/build_benchmarks.bash gcc

# Con __int128_t (GCC/Clang automático)
# No requiere flags especiales

# Con Boost
./scripts/build_with_backends.bash gcc --with-boost

# Con todos los backends
./scripts/build_with_backends.bash gcc --all-backends
```

### Ejecución

```bash
# Ejecutar todos los benchmarks (uint128 + int128)
./scripts/run_benchmarks.bash all

# Solo GCC
./scripts/run_benchmarks.bash gcc

# Los resultados se guardan en:
# benchmark_results/int128_benchmarks_GCC_*.csv
# benchmark_results/int128_benchmarks_GCC_*.json
```

### Análisis

```bash
# Agregar resultados (genera reportes separados)
python scripts/aggregate_benchmark_results.py

# Genera:
# - consolidated_uint128_TIMESTAMP.csv/json/md
# - consolidated_int128_TIMESTAMP.csv/json/md

# Graficar (detecta ambos tipos automáticamente)
python scripts/plot_benchmark_results.py

# Genera gráficos con prefijos:
# - uint128_time_*.png, uint128_cycles_*.png, ...
# - int128_time_*.png, int128_cycles_*.png, ...
```

---

## Comparaciones de Rendimiento

### Expectativas Generales

```
Velocidad esperada (más rápido → más lento):
1. int64_t          - Nativo, óptimo
2. __int128_t       - Nativo GCC/Clang, casi óptimo
3. int128_t         - Software puro, bueno
4. boost::int128_t  - Templated, overhead moderado
5. boost + GMP/Tom  - Backends externos, más lento
```

### Operaciones Rápidas vs Lentas

#### Rápidas (< 10 ciclos)
- Construcción default
- Asignación
- Comparaciones simples (==, !=)
- Shifts pequeños
- Operaciones bitwise

#### Moderadas (10-50 ciclos)
- Suma/resta con carry
- Multiplicación de magnitudes pequeñas
- Comparaciones signed (<, >, etc.)
- Construcción desde int64_t

#### Lentas (> 50 ciclos)
- División y módulo (algoritmo complejo)
- Multiplicación de magnitudes grandes
- Conversión a/desde strings
- Operaciones con backends externos

### Factores que Afectan Rendimiento

1. **Sign extension overhead**
   - Constructor desde tipos signed requiere extensión
   - Comparado con unsigned: +2-5 ciclos

2. **Detección de signo**
   - `is_negative()`: Check de un bit (< 1 ciclo)
   - Usado en todas las operaciones signed

3. **Manejo de negativos**
   - División/módulo: requiere abs() antes y después
   - Overhead: ~10-20 ciclos extra vs unsigned

4. **Compilador**
   - GCC/Clang: pueden optimizar con instrucciones nativas
   - MSVC: todo software, ~10-30% más lento

### Ejemplo de Resultados Típicos

```
Operation: Addition
┌─────────────────────┬───────────┬──────────┐
│ Type                │ Time (ns) │ Cycles   │
├─────────────────────┼───────────┼──────────┤
│ int64_t             │    0.8    │    2.5   │
│ __int128_t (GCC)    │    1.2    │    3.8   │
│ int128_t            │    3.5    │   11.2   │
│ boost::int128_t     │    8.2    │   26.3   │
└─────────────────────┴───────────┴──────────┘

Operation: Division
┌─────────────────────┬───────────┬──────────┐
│ Type                │ Time (ns) │ Cycles   │
├─────────────────────┼───────────┼──────────┤
│ int64_t             │    2.1    │    6.7   │
│ __int128_t (GCC)    │    4.5    │   14.4   │
│ int128_t            │   85.3    │  273.0   │
│ boost::int128_t     │  152.7    │  488.6   │
└─────────────────────┴───────────┴──────────┘
```

**Análisis**:
- Suma: int128_t es ~3x más lento que int64_t (aceptable)
- División: int128_t es ~40x más lento (algoritmo complejo)
- __int128_t es ~3x más rápido que int128_t en división (nativo)

---

## Uso Práctico

### Casos de Uso de int128_t

#### 1. Aritmética de Precisión Extendida

```cpp
#include "int128/int128_t.hpp"

// Cálculos financieros con centavos
int128_t balance = 1'000'000'000'000'000LL;  // $1 trillion
int128_t interest = balance * 5 / 100;       // 5% interés
int128_t new_balance = balance + interest;

// Física: velocidades relativistas
int128_t speed_of_light = 299'792'458LL;  // m/s
int128_t distance_light_year = speed_of_light * 365 * 24 * 3600;
```

#### 2. Coordenadas y Offsets con Signo

```cpp
// Videojuegos: coordenadas de mundo grande
struct Position {
    int128_t x, y, z;
};

Position player = {1'000'000'000, -500'000'000, 250'000'000};
Position offset = {-1000, 2000, -500};
Position new_pos = {
    player.x + offset.x,
    player.y + offset.y,
    player.z + offset.z
};

// Comparar posiciones (signed)
bool is_north = player.y > 0;  // Hemisferio norte
bool is_below = player.z < offset.z;
```

#### 3. Diferencias Temporales

```cpp
// Timestamps con rango extendido
int128_t unix_nano_1 = 1'600'000'000'000'000'000LL;  // Sep 2020
int128_t unix_nano_2 = 1'700'000'000'000'000'000LL;  // Nov 2023

// Diferencia (puede ser negativa)
int128_t diff = unix_nano_2 - unix_nano_1;

if (diff > 0) {
    std::cout << "Event 2 happened " << diff << " ns later\n";
} else {
    std::cout << "Event 2 happened " << -diff << " ns earlier\n";
}
```

#### 4. Matemáticas: Factoriales Grandes

```cpp
// Factorial que puede ser negativo (matemática modular)
int128_t factorial(int n) {
    int128_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Hasta ~34! cabe en int128_t
int128_t fact20 = factorial(20);  // 2432902008176640000
```

#### 5. Criptografía: Exponenciación Modular

```cpp
// Modular exponentiation: (base^exp) % mod
int128_t mod_pow(int128_t base, int128_t exp, int128_t mod) {
    int128_t result = 1;
    base %= mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    
    return result;
}

// RSA toy example
int128_t encrypted = mod_pow(message, public_key, modulus);
```

### Mejores Prácticas

#### ✅ DO: Usar int128_t Cuando

- Necesitas rango mayor que int64_t (-2^63 a 2^63-1)
- Trabajas con números negativos y positivos
- Requieres aritmética signed correcta
- Comparaciones signed son importantes
- Sign extension automática es útil

#### ❌ DON'T: Evitar int128_t Cuando

- int64_t es suficiente (más rápido)
- Solo trabajas con positivos (usa uint128_t)
- Rendimiento extremo es crítico (usa __int128_t si disponible)
- División/módulo en bucle muy ajustado (muy lento)

#### 🔧 Optimizaciones

```cpp
// Preferir __int128_t si disponible
#ifdef HAS_INT128_T
    using fast_int128 = __int128_t;
#else
    using fast_int128 = int128_t;
#endif

// Evitar divisiones en bucles
int128_t sum = 0;
for (int i = 0; i < 1000000; ++i) {
    sum += values[i];  // Rápido
}
int128_t avg = sum / 1000000;  // División fuera del bucle

// Reusar resultados de is_negative()
bool neg = value.is_negative();
if (neg) { /* ... */ }
// Mejor que llamar is_negative() múltiples veces

// Comparar con cero es más rápido que comparaciones generales
if (value < 0) { /* ... */ }  // Optimizado
```

### Testing y Debugging

```cpp
// Verificar sign extension
void test_construction() {
    int64_t small = -42;
    int128_t large(small);
    
    // Debe preservar valor
    assert(static_cast<int64_t>(large) == small);
    
    // Debe extender signo
    assert(large.is_negative());
    assert(large.high() == UINT64_MAX);  // All 1s
}

// Verificar límites
void test_limits() {
    int128_t min_val = INT128_MIN;
    int128_t max_val = INT128_MAX;
    
    assert(min_val < 0);
    assert(max_val > 0);
    assert(max_val == -(min_val + 1));
}

// Debug print
void debug_int128(int128_t value) {
    std::cout << "Value: " << value.to_string() << "\n";
    std::cout << "High:  0x" << std::hex << value.high() << "\n";
    std::cout << "Low:   0x" << value.low() << std::dec << "\n";
    std::cout << "Sign:  " << (value.is_negative() ? "negative" : "positive") << "\n";
}
```

---

## Diferencias con uint128_t

| Característica | uint128_t | int128_t |
|----------------|-----------|----------|
| **Rango** | 0 a 2^128-1 | -2^127 a 2^127-1 |
| **Constructor int64_t** | Cast directo | Sign extension |
| **Comparaciones** | Unsigned | Signed (neg < pos) |
| **División** | Unsigned | Signed (trunca a cero) |
| **Right shift** | Lógico (>>>) | Aritmético (>>) |
| **MSB** | Dato | Bit de signo |
| **Negación** | No tiene sentido | Two's complement |
| **to_string()** | Sin signo | Con "-" si negativo |
| **Overflow** | Wrap (módulo) | UB (C++ estándar) |

---

## Recursos Adicionales

### Documentación Relacionada

- [uint128_t Guide](BENCHMARK_GUIDE.md) - Guía de la versión unsigned
- [API Reference](api.md) - Referencia completa de API
- [Benchmark Results](BENCHMARK_SUMMARY.md) - Resultados de rendimiento

### Archivos de Tests

```bash
# Ejecutar tests de int128_t
cd tests
g++ -std=c++20 -I../include int128_extracted_tests.cpp -o int128_tests
./int128_tests
```

### Ejemplos de Código

Ver `tests/int128_extracted_tests.cpp` para ejemplos exhaustivos de:
- Construcción y sign extension
- Operaciones aritméticas signed
- Comparaciones signed
- Conversión a/desde strings con signo
- Casos edge (INT128_MIN, INT128_MAX)

---

## Conclusión

`int128_t` proporciona aritmética signed de 128 bits completa con:

✅ **Representación two's complement estándar**  
✅ **Sign extension automática desde tipos smaller**  
✅ **Semántica signed completa (división, comparaciones, shifts)**  
✅ **API consistente con int64_t y tipos built-in**  
✅ **Benchmarks exhaustivos vs tipos nativos y Boost**  
✅ **Sistema de tests completo**  

**Rendimiento**: Competitivo para suma/resta, razonable para multiplicación, aceptable para división (operación inherentemente lenta).

**Cuándo usar**: Cuando necesitas enteros signed más grandes que int64_t con aritmética correcta.

**Alternativas**:
- `int64_t`: Si rango es suficiente (más rápido)
- `__int128_t`: Si disponible (GCC/Clang, nativo)
- `boost::int128_t`: Si necesitas backends GMP/tommath
- `uint128_t`: Si solo trabajas con positivos

---

*Última actualización: 19 de diciembre de 2025*
