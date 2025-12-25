# Guía Rápida: Benchmarks Comparativos con Boost.Multiprecision

Esta guía te ayudará a compilar y ejecutar los benchmarks comparativos entre `uint128_t` (nuestra implementación) y `boost::multiprecision::uint128_t`.

---

## 📋 Requisitos Previos

### 1. Instalar Boost.Multiprecision

#### En MSYS2 (Windows):
```bash
pacman -S mingw-w64-ucrt-x86_64-boost
```

#### En Ubuntu/Debian (Linux):
```bash
sudo apt-get install libboost-dev
```

#### En macOS:
```bash
brew install boost
```

#### Verificar instalación:
```bash
echo '#include <boost/multiprecision/cpp_int.hpp>' | g++ -std=c++20 -E -x c++ - &>/dev/null && echo "✓ Boost instalado" || echo "✗ Boost no encontrado"
```

### 2. Compilador C++20

Necesitas GCC 10+, Clang 12+, o MSVC 14.50+ con soporte C++20.

---

## 🚀 Ejecución Rápida

### Opción 1: Script Automatizado (Recomendado)

```bash
# Desde la raíz del proyecto
bash scripts/run_boost_comparison.bash
```

Este script:
1. ✅ Verifica que Boost esté instalado
2. ✅ Compila con GCC y Clang (los que estén disponibles)
3. ✅ Ejecuta ambos benchmarks automáticamente
4. ✅ Muestra resultados en terminal

### Opción 2: Compilación Manual

```bash
# Con GCC
g++ -std=c++20 -O3 -I./include \
    benchmarks/benchmark_comparison_boost.cpp \
    -o build/benchmark_comparison_boost_gcc

# Ejecutar
./build/benchmark_comparison_boost_gcc
```

```bash
# Con Clang
clang++ -std=c++20 -O3 -I./include \
    benchmarks/benchmark_comparison_boost.cpp \
    -o build/benchmark_comparison_boost_clang

# Ejecutar
./build/benchmark_comparison_boost_clang
```

---

## 📊 Qué Mide el Benchmark

El benchmark compara las siguientes operaciones (100,000 iteraciones cada una):

### 1. **Construcción y Asignación**
- Default constructor
- Constructor desde `uint64_t`
- Copy constructor

### 2. **Aritmética Básica**
- Suma (`+`)
- Resta (`-`)
- Multiplicación (`*`)
- División (`/`)
- Módulo (`%`)

### 3. **Operaciones Bit a Bit**
- AND (`&`)
- OR (`|`)
- XOR (`^`)
- Left shift (`<<`)
- Right shift (`>>`)

### 4. **Conversiones String**
- `to_string()` decimal
- `to_string_hex()`
- `from_string()`

### 5. **Funciones Matemáticas**
- `gcd()`
- `lcm()`
- `pow()`
- `sqrt()`

---

## 📈 Interpretación de Resultados

### Formato de Salida

```
=== CONSTRUCCIÓN Y ASIGNACIÓN ===
Default constructor:
  uint128_t:        2.15 ns
  boost_uint128_t:  5.23 ns  (speedup: 2.43x)
```

**Cómo leer**:
- **ns**: Nanosegundos por operación (menor es mejor)
- **speedup**: Factor de mejora de nuestra implementación
  - `speedup > 1.0` → `uint128_t` es más rápido
  - `speedup < 1.0` → `boost::multiprecision` es más rápido
  - `speedup ≈ 1.0` → Rendimiento similar

### Resultados Esperados

Basándonos en las optimizaciones de `uint128_t`, esperamos:

| Operación | Speedup Esperado | Notas |
|-----------|------------------|-------|
| **Construcción** | ~2.5× | Trivialmente copyable |
| **Suma/Resta** | ~2.7× | Intrínsecos del compilador |
| **Multiplicación** | ~2.7× | Optimizada para 128-bit |
| **División** | ~2.5× | Knuth Algorithm D |
| **Bitwise** | ~3.0× | Operaciones nativas |
| **Shift** | ~2.7× | Optimizado |
| **String** | ~2.4× | Conversiones optimizadas |
| **GCD** | ~2.5× | Binary GCD |

---

## 🔍 Análisis Detallado

### Ver Análisis Completo de Features

Lee el documento completo de comparación:
```bash
cat BOOST_COMPARISON_ANALYSIS.md
```

Este documento incluye:
- ✅ Comparación detallada de type traits
- ✅ Soporte de C++20 concepts
- ✅ Compatibilidad STL
- ✅ Thread safety
- ✅ Características exclusivas de cada implementación
- ✅ Recomendaciones de cuándo usar cada una

### Características Clave Comparadas

| Característica | uint128_t | Boost.Multiprecision |
|---------------|-----------|---------------------|
| **Performance (128-bit)** | ✅ Optimizado | ⚠️ Genérico |
| **std::is_integral** | ✅ true | ❌ false |
| **C++20 concepts** | ✅ Completo | ❌ No |
| **Thread safety** | ✅ 4 wrappers | ❌ No |
| **std::popcount** | ✅ Funciona | ❌ No |
| **Precisión arbitraria** | ❌ Solo 128-bit | ✅ Ilimitada |
| **Rational numbers** | ❌ No | ✅ Sí |

---

## 🎯 Casos de Uso Recomendados

### Usar uint128_t (Nuestra Implementación) para:

✅ **Performance crítico** en 128-bit fijo
- UUIDs, identificadores únicos
- Contadores de gran rango
- Hashes de 128-bit
- Claves criptográficas de 128-bit

✅ **Integración STL completa**
- Templates con `std::integral` concepts
- Uso con `std::iota`, `std::accumulate`
- Contenedores con `std::hash` automático
- Funciones `std::bit` (popcount, etc.)

✅ **Thread safety built-in**
- Contadores atómicos
- Sincronización con mutex/rwlock/spinlock

### Usar Boost.Multiprecision para:

✅ **Precisión arbitraria** (> 128 bits)
- Factorial de números grandes
- Números primos enormes
- Cálculos con miles de dígitos

✅ **Tipos numéricos especiales**
- Números racionales (fracciones exactas)
- Fixed-point de alta precisión
- Números complejos

✅ **Flexibilidad de backends**
- GMP para máxima velocidad
- cpp_int para portabilidad
- tommath como alternativa

---

## 🐛 Solución de Problemas

### Error: "boost/multiprecision/cpp_int.hpp: No such file or directory"

**Causa**: Boost no está instalado o no está en el path.

**Solución**:
```bash
# MSYS2
pacman -S mingw-w64-ucrt-x86_64-boost

# Ubuntu/Debian
sudo apt-get install libboost-dev

# Verificar
echo '#include <boost/multiprecision/cpp_int.hpp>' | g++ -std=c++20 -E -x c++ - &>/dev/null && echo "OK"
```

### Error de compilación: "std::is_integral_v<boost::...> no es true"

**Esto es esperado**: Boost.Multiprecision no define sus tipos como `std::is_integral`.

El benchmark está diseñado para mostrar esta diferencia. No es un error.

### Benchmark ejecuta muy rápido (< 1 segundo)

**Ajustar iteraciones**: Edita `benchmark_comparison_boost.cpp`:
```cpp
constexpr int ITERATIONS = 1000000;  // Cambiar de 100000 a 1000000
```

---

## 📚 Referencias Adicionales

- **Análisis detallado**: [BOOST_COMPARISON_ANALYSIS.md](BOOST_COMPARISON_ANALYSIS.md)
- **Código fuente**: [benchmark_comparison_boost.cpp](benchmarks/benchmark_comparison_boost.cpp)
- **Boost docs**: https://www.boost.org/doc/libs/release/libs/multiprecision/
- **Nuestra documentación**: [README.md](README.md)

---

## 💡 Próximos Pasos

Después de ejecutar los benchmarks:

1. **Analiza los resultados** y compara con las expectativas
2. **Lee el análisis completo** en `BOOST_COMPARISON_ANALYSIS.md`
3. **Decide qué implementación usar** según tus necesidades:
   - Performance 128-bit → `uint128_t`
   - Precisión arbitraria → `Boost.Multiprecision`
4. **Experimenta** modificando el benchmark para tus casos de uso específicos

---

*Guía generada el 25 de diciembre de 2025*  
*Versión: 1.0*
