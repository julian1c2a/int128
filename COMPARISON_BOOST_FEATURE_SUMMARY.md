# COMPARISON_BOOST - Nueva FEATURE para Benchmarks Comparativos

## 📋 Descripción

La FEATURE `comparison_boost` proporciona un benchmark completo que compara el rendimiento de `uint128_t` con:

1. **Boost.Multiprecision** (`boost::multiprecision::uint128_t`)
2. **unsigned __int128** (GCC/Clang builtin)
3. **__int128** (GCC/Clang builtin signed)

## 🎯 Objetivo

Proporcionar métricas objetivas de rendimiento comparando:
- Nuestra implementación (`uint128_t`)
- Boost.Multiprecision (estándar industrial)
- Tipos builtin de GCC/Clang (`__int128`, `unsigned __int128`)

## 📊 Categorías de Benchmarks

El benchmark compara **5 categorías principales**:

### 1. **Construcción y Asignación** (3 tests)
- Default constructor
- Constructor desde `uint64_t`
- Constructor de copia

### 2. **Aritmética Básica** (5 tests)
- Suma (`+=`, `+`)
- Resta (`-=`, `-`)
- Multiplicación (`*=`, `*`)
- División (`/=`, `/`)
- Módulo (`%=`, `%`)
### 3. **Operaciones Bit a Bit** (5 tests)
- AND (`&=`, `&`)
- OR (`|=`, `|`)
- XOR (`^=`, `^`)
- Shift left (`<<=`, `<<`)
- Shift right (`>>=`, `>>`)
### 4. **Conversiones de String** (3 tests)
- `to_string()` decimal
- `to_string()` hexadecimal
- `from_string()`

### 5. **Funciones Matemáticas** (4 tests)
- GCD (máximo común divisor)
- LCM (mínimo común múltiplo)
- POW (potencia)
- SQRT (raíz cuadrada)

**Total: 20 tests individuales**

## 🔧 Uso con Makefile

### Compilar

```bash
# Compilar con todos los compiladores (GCC + Clang)
make build_benchs TYPE=uint128 FEATURE=comparison_boost

# Compilar solo con GCC
make build_benchs TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc

# Compilar solo Release con Clang
make build_benchs TYPE=uint128 FEATURE=comparison_boost COMPILER=clang MODE=release
```

### Ejecutar

```bash
# Ejecutar con todos los compiladores
make run TYPE=uint128 FEATURE=comparison_boost

# Ejecutar solo GCC Release
make run TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc MODE=release

# Ejecutar Clang Debug
make run TYPE=uint128 FEATURE=comparison_boost COMPILER=clang MODE=debug
```

### Pipeline Completo

```bash
# Build + Run todo
make comparison_boost-full

# Equivalente a:
make build-all-comparison_boost
make run-all-comparison_boost
```

## 📁 Archivos Creados

### Benchmark
- `benchmarks/benchmark_comparison_boost.cpp` (594 líneas)

### Scripts de Compilación
- `scripts/build_uint128_comparison_boost_extracted_benchs.bash`
- `scripts/build_int128_comparison_boost_extracted_benchs.bash` (wrapper)

### Scripts de Ejecución
- `scripts/run_uint128_comparison_boost_extracted_benchs.bash`
- `scripts/run_int128_comparison_boost_extracted_benchs.bash` (wrapper)

## ⚙️ Configuración del Benchmark

```cpp
constexpr int ITERATIONS = 100000;  // Iteraciones por test
constexpr int WARM_UP = 1000;       // Iteraciones de calentamiento
```

## 📈 Interpretación de Resultados

El benchmark muestra tiempos en **nanosegundos** y calcula un **speedup**:

```
Addition (+):
  uint128_t:         12.50 ns
  boost_uint128_t:   45.30 ns  (speedup: 3.62x)
  unsigned __int128: 8.20 ns   (speedup: 0.66x)
```

- **speedup > 1.0**: `uint128_t` es **más rápido** que la alternativa
- **speedup < 1.0**: `uint128_t` es **más lento** que la alternativa
- **speedup ≈ 1.0**: rendimiento **similar**

## 🎯 Resultados Esperados

### uint128_t vs Boost.Multiprecision
- **Construcción**: uint128_t ~2-4× más rápido
- **Aritmética básica**: uint128_t ~2-3× más rápido
- **Bitwise**: uint128_t ~3-5× más rápido
- **String conversions**: Similar o ligeramente más lento
- **Math functions**: Similar

### uint128_t vs unsigned __int128 (GCC/Clang builtin)
- **Construcción**: Similar (ambos optimizados)
- **Aritmética básica**: `__int128` ligeramente más rápido (nativo)
- **Bitwise**: Similar
- **String conversions**: uint128_t más rápido (implementación optimizada)
- **Math functions**: uint128_t disponible (std::gcd, std::lcm work)

## 🔍 Soporte por Compilador

| Compilador | uint128_t | Boost | unsigned __int128 | __int128 |
|------------|-----------|-------|-------------------|----------|
| **GCC**    | ✅        | ✅    | ✅                | ✅       |
| **Clang**  | ✅        | ✅    | ✅                | ✅       |
| **Intel**  | ✅        | ✅    | ❌                | ❌       |
| **MSVC**   | ✅        | ✅    | ❌                | ❌       |

**Nota**: El benchmark incluye detección automática de `__int128`:
```cpp
#if defined(__GNUC__) || defined(__clang__)
#define HAS_INT128_BUILTIN 1
// ...tests with __int128...
#endif
```

## 📦 Dependencias

### Requeridas
- **Boost.Multiprecision**: Headers de Boost (header-only)
  ```bash
  # MSYS2
  pacman -S mingw-w64-ucrt-x86_64-boost
  
  # Ubuntu
  sudo apt-get install libboost-all-dev
  
  # macOS
  brew install boost
  ```

### Opcionales (ya instaladas según usuario)
- **GMP** (GNU Multiple Precision)
- **tommath** (LibTomMath)

El script de compilación detecta automáticamente si GMP/tommath están disponibles y las enlaza.

## 🚀 Ejemplo de Uso Rápido

```bash
# 1. Compilar (solo GCC Release para máximo rendimiento)
make build_benchs TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc MODE=release

# 2. Ejecutar
make run TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc MODE=release

# 3. Ver resultados en pantalla
# Output: Tiempos en ns + speedups para cada test
```

## 📊 Output de Ejemplo

```
==============================================================================
BENCHMARK COMPARATIVO: uint128_t vs Boost.Multiprecision vs __int128
==============================================================================
Iteraciones: 100000 (warm-up: 1000)
Nota: speedup > 1.0 significa uint128_t es más rápido
✅ Compilador con soporte para __int128 (GCC/Clang)

=== CONSTRUCCIÓN Y ASIGNACIÓN ===
Default constructor:
  uint128_t:         2.15 ns
  boost_uint128_t:   8.30 ns  (speedup: 3.86x)
  unsigned __int128: 1.80 ns  (speedup: 0.84x)

From uint64_t:
  uint128_t:         2.50 ns
  boost_uint128_t:   9.20 ns  (speedup: 3.68x)
  unsigned __int128: 2.10 ns  (speedup: 0.84x)
...
```

## 🎓 Casos de Uso

### ✅ Cuándo usar uint128_t
- Necesitas **std::is_integral** = true
- Necesitas trabajar con **STL algorithms** (std::accumulate, std::iota)
- Necesitas **C++20 concepts** (std::integral, std::unsigned_integral)
- Necesitas **std::popcount**, **std::bit_width**, etc.
- Necesitas **thread safety** (atomic wrappers)

### ✅ Cuándo usar Boost.Multiprecision
- Necesitas **precisión arbitraria** (> 128 bits)
- Necesitas **números racionales** (fracciones exactas)
- Necesitas **números complejos** de alta precisión
- Necesitas **backends específicos** (GMP, tommath, cpp_dec_float)

### ✅ Cuándo usar unsigned __int128
- Solo necesitas **128-bit aritmética básica**
- Estás seguro de usar **GCC/Clang únicamente**
- No necesitas **std::is_integral** = true
- No necesitas **I/O streams** (cout, cin)
- Prioridad: **máximo rendimiento** en aritmética simple

## 🔗 Documentación Relacionada

- [BOOST_COMPARISON_ANALYSIS.md](BOOST_COMPARISON_ANALYSIS.md) - Análisis completo de features
- [BOOST_COMPARISON_QUICKSTART.md](BOOST_COMPARISON_QUICKSTART.md) - Guía rápida de uso
- [README.md](README.md#-benchmarks-comparativos-con-boostmultiprecision) - Sección de benchmarks

## ✅ Checklist de Implementación

- [x] Benchmark completo (5 categorías, 20 tests)
- [x] Soporte para Boost.Multiprecision
- [x] Soporte para `unsigned __int128` (GCC/Clang)
- [x] Soporte para `__int128` (GCC/Clang signed)
- [x] Detección automática de `__int128` con `#if defined(__GNUC__) || defined(__clang__)`
- [x] Scripts de compilación (`build_*_comparison_boost_extracted_benchs.bash`)
- [x] Scripts de ejecución (`run_*_comparison_boost_extracted_benchs.bash`)
- [x] Integración con Makefile (FEATURE=comparison_boost)
- [x] Documentación completa
- [x] 100,000 iteraciones + warm-up
- [x] Output formateado con speedups

## 🎉 Resumen

La FEATURE `comparison_boost` es una **herramienta completa** para:
- Evaluar objetivamente el rendimiento de `uint128_t`
- Comparar con estándares industriales (Boost)
- Comparar con tipos builtin de compiladores (GCC/Clang)
- Identificar fortalezas y debilidades
- Justificar elección de implementación según caso de uso

**Total: 20 tests × 3 implementaciones = 60 comparaciones por ejecución**
