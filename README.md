# int128 - Implementación de enteros de 128 bits

[![CI - Build & Test](https://github.com/YOUR_USERNAME/int128/actions/workflows/ci.yml/badge.svg)](https://github.com/YOUR_USERNAME/int128/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/License-Boost%201.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)

Una implementación completa y eficiente de enteros de 128 bits (signed y unsigned) para C++20, compatible con MSVC, GCC y Clang.

## 🎯 Dos Tipos Complementarios

- **`uint128_t`**: Entero **sin signo** de 128 bits (0 a 2^128-1)
- **`int128_t`**: Entero **con signo** de 128 bits (-2^127 a 2^127-1)

## 🌍 Plataformas Soportadas

### ✅ Plataforma Principal - Windows (MSYS2) - 24/24 Validaciones

| Compilador | Debug | Release | Versión |
|------------|-------|---------|---------|
| **GCC (UCRT64)** | ✅ PASS | ✅ PASS | 15.2 |
| **Clang (CLANG64)** | ✅ PASS | ✅ PASS | 19.1 |
| **Intel ICX** | ✅ PASS | ✅ PASS | 2025 |
| **MSVC** | ✅ PASS | ✅ PASS | 2026 (v18) |

### ✅ WSL (Ubuntu) - 16/16 Validaciones

| Compilador | Debug | Release | Versión |
|------------|-------|---------|---------|
| **GCC 13** | ✅ PASS | ✅ PASS | 13.3.0 |
| **GCC 14** | ✅ PASS | ✅ PASS | 14.2.0 |
| **GCC 15** | ✅ PASS | ✅ PASS | 15.0.1 |
| **Clang 18** | ✅ PASS | ✅ PASS | 18.1.8 |
| **Clang 19** | ✅ PASS | ✅ PASS | 19.1.7 |
| **Clang 20** | ✅ PASS | ✅ PASS | 20.1.2 |
| **Clang 21** | ✅ PASS | ✅ PASS | 21.1.8 |
| **Intel ICPX** | ✅ PASS | ✅ PASS | 2025.3.1 |

**Total validaciones: 40/40 (100%)** - Fase 1.66 completada ✅

### 📋 Otras Plataformas (Sin Testear - Debería Funcionar)

La biblioteca usa C++20 estándar portable. Debería compilar sin problemas en:

- **Linux x86_64**: GCC 10+, Clang 12+, Intel ICX
- **macOS x86_64/ARM64**: Clang (Apple), GCC (Homebrew)
- **ARM 32/64-bit**: GCC, Clang (con fallback a código genérico)
- **RISC-V 32/64-bit**: GCC, Clang (código genérico)

**Nota**: Los intrínsecos de optimización son específicos de x86_64. En otras arquitecturas,
la biblioteca usa automáticamente implementaciones genéricas portables.

## 📦 Arquitectura del Proyecto

```
int128/
├── include/                   # Headers de la biblioteca (template unificado)
│   ├── int128.hpp                        # Header principal (incluye todo)
│   ├── int128_base_tt.hpp                # Template principal int128_base_t<S>
│   ├── int128_base_limits.hpp            # std::numeric_limits especializado
│   ├── int128_base_traits_specializations.hpp # Type traits STL
│   ├── int128_base_traits.hpp            # common_type y verificaciones
│   ├── int128_base_concepts.hpp          # Concepts C++20
│   ├── int128_base_bits.hpp              # Operaciones de bits
│   ├── int128_base_numeric.hpp           # Funciones numéricas
│   ├── int128_base_algorithm.hpp         # Algoritmos STL-like
│   ├── int128_base_cmath.hpp             # Funciones matemáticas
│   ├── int128_base_iostreams.hpp         # Stream I/O
│   ├── int128_base_format.hpp            # Formateo avanzado
│   ├── type_traits.hpp                   # integral_builtin concept
│   ├── intrinsics/                       # Operaciones de bajo nivel
│   │   ├── arithmetic_operations.hpp     # umul128, div128, add/sub con carry
│   │   ├── bit_operations.hpp            # clz, ctz, popcount
│   │   ├── compiler_detection.hpp        # Detección MSVC/GCC/Clang/Intel
│   │   └── fallback_portable.hpp         # Implementaciones portables
│   └── specializations/                  # Optimizaciones modulares
│       ├── uint128_div_const.hpp
│       └── uint128_divisibility.hpp
├── tests/                     # Tests unitarios (14 archivos por feature)
│   ├── int128_base_tt_extracted_tests.cpp       # Tests template principal
│   ├── int128_bits_extracted_tests.cpp
│   ├── int128_cmath_extracted_tests.cpp
│   ├── int128_concepts_extracted_tests.cpp
│   ├── int128_format_extracted_tests.cpp
│   └── ... (14 features total)
├── benchs/                    # Benchmarks de rendimiento
├── demos/                     # Ejemplos ejecutables
│   ├── tutorials/             # 16 tutoriales (01-13 + extras)
│   ├── showcase/              # Demostraciones avanzadas
│   ├── examples/              # Casos de uso reales
│   └── general/               # Conceptos generales
├── scripts/                   # Scripts de build
│   ├── wsl/                   # Scripts modulares WSL
│   │   ├── common.bash        # Funciones compartidas Bash
│   │   ├── common.py          # Funciones compartidas Python
│   │   ├── build_gcc13.bash ... build_icpx.bash
│   │   └── ...
│   ├── wsl_build_and_test.bash    # Script maestro WSL (Bash)
│   ├── wsl_build_and_test.py      # Script maestro WSL (Python)
│   ├── run_wsl_tests.py           # Puente Windows→WSL
│   ├── build_generic.bash         # Build genérico Bash
│   ├── build_generic.py           # Build genérico Python
│   └── env_setup/                 # Entornos de compilación
├── documentation/             # Documentación generada
├── build/                     # Archivos compilados
├── CMakeLists.txt
├── Makefile
├── make.py                    # Sistema de build Python
└── README.md
```

## 🛠️ Sistema de Build

### Scripts de Build

- `build_msvc.ps1` - Script PowerShell principal (recomendado para Windows)
- `build.ps1` - Script PowerShell original  
- `build.sh` - Script bash para entornos Unix-like (con cygpath)
- `msys_build.sh` - Script bash optimizado para MSYS2

### Uso del Sistema de Build

#### En PowerShell (Recomendado)

```powershell
# Compilar y ejecutar un test específico (debug)
.\build_msvc.ps1 user_literals_test debug

# Compilar en modo release
.\build_msvc.ps1 user_literals_test release
```

#### En terminal MSYS2/Bash

```bash
# Compilar y ejecutar un test específico
./msys_build.sh user_literals_test debug

# Compilar en modo release  
./msys_build.sh user_literals_test release
```

### 📦 Compilación Directa (Tests, Benchmarks, Demos)

#### Tests (27 archivos)

```bash
# Compilar cualquier test
/ucrt64/bin/g++ -std=c++20 -Iinclude tests/nombre_test.cpp -o build/test

# Ejemplos:
/ucrt64/bin/g++ -std=c++20 -Iinclude tests/uint128_extracted_tests.cpp -o build/uint128_test
/ucrt64/bin/g++ -std=c++20 -Iinclude tests/final_traits_test.cpp -o build/traits_test
```

#### Benchmarks (29 archivos)

```bash
# Compilar benchmarks estándar
/ucrt64/bin/g++ -std=c++20 -Iinclude benchs/nombre_bench.cpp -o build/bench

# Benchmarks con thread safety REQUIEREN flags adicionales
/ucrt64/bin/g++ -std=c++20 -Iinclude -pthread benchs/uint128_thread_safety_benchs.cpp -latomic -o build/bench
```

#### Demos (40 archivos)

```bash
# Compilar demos normales
/ucrt64/bin/g++ -std=c++20 -Iinclude demos/tutorials/01_basic_operations.cpp -o build/demo

# ⚠️ IMPORTANTE: Demos con thread safety REQUIEREN flags adicionales
/ucrt64/bin/g++ -std=c++20 -Iinclude -pthread demos/examples/demo_int128_thread_safety.cpp -latomic -o build/demo

# Demos que requieren -pthread -latomic:
# - demos/examples/demo_int128_thread_safety.cpp
# - demos/examples/example_thread_safety.cpp
```

**Nota**: Los flags `-pthread` y `-latomic` son necesarios para operaciones atómicas de 128 bits en x86_64.

### Estructura de Compilación (uint128_t y int128_t)

- [x] Constructores (default, copia, move, desde integrales)
- [x] Operadores aritméticos (`+`, `-`, `*`, `/`, `%`)
- [x] Operadores de comparación (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- [x] Operadores bit a bit (`&`, `|`, `^`, `<<`, `>>`, `~`)
- [x] Operadores de asignación (`+=`, `-=`, `*=`, etc.)
- [x] Operadores de incremento/decremento (`++`, `--`)
- [x] **int128_t**: Sign extension, two's complement, aritmética signed

### ✅ Funcionalidades Básicas

- [x] Constructores (default, copia, move, desde integrales)
- [x] Operadores aritméticos (`+`, `-`, `*`, `/`, `%`)
- [x] Operadores de comparación (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- [x] Operadores bit a bit (`&`, `|`, `^`, `<<`, `>>`, `~`)
- [x] Operadores de asignación (`+=`, `-=`, `*=`, etc.)
- [x] Operadores de incremento/decremento (`++`, `--`)

### ✅ Extensiones STL

- [x] `std::numeric_limits<uint128_t>` y `std::numeric_limits<int128_t>`
- [x] `std::hash<uint128_t>` para contenedores unordered
- [x] Concepts personalizados (`uint128_concepts`)
- [x] Compatibilidad con `std::iota`

### ✅ Thread Safety

- [x] **uint128_t**: Thread-safe para lectura concurrente + 4 wrappers + Tests ✅ + Benchmarks ✅
- [x] **int128_t**: Thread-safe para lectura concurrente + 4 wrappers + Tests ✅ + Benchmarks ✅ **COMPLETO**

### ✅ Funcionalidades Avanzadas

- [x] Constructor desde cadenas: `uint128_t("12345")`
- [x] Método `from_string()` estático
- [x] Literales de usuario: `42_u128`, `"123456789"_U128`
- [x] Operadores simétricos (friend functions)
- [x] Operador de inserción para streams (`operator<<`)

### ✅ Conversiones de Tipo

#### Conversiones Numéricas

- [x] **A tipos integrales**: `static_cast<uint64_t>(num)`, `static_cast<int>(num)`
- [x] **A tipos flotantes**: `static_cast<float>(num)`, `static_cast<double>(num)`, `static_cast<long double>(num)`
- [x] **A bool**: `static_cast<bool>(num)` - true si != 0
- [x] **int128_t**: Método `to<T>()` para todas las conversiones anteriores

```cpp
uint128_t big(0x1234567890ABCDEF, 0xFEDCBA0987654321);

// Conversión a flotantes (high * 2^64 + low)
double d = static_cast<double>(big);
float f = static_cast<float>(big);
long double ld = static_cast<long double>(big);

// int128_t usa .to<T>()
int128_t signed_val(-12345);
double d2 = signed_val.to<double>();  // Maneja signo correctamente
```

#### Conversión a/desde Bytes (std::array<std::byte, 16>)

- [x] **to_bytes()**: Serialización a bytes (little-endian)
- [x] **from_bytes()**: Deserialización desde bytes
- [x] **Casos de uso**: Serialización de red, almacenamiento en disco, checksum

```cpp
uint128_t data(0x1234567890ABCDEF, 0xFEDCBA0987654321);

// Serializar a bytes (little-endian)
std::array<std::byte, 16> bytes = data.to_bytes();
// bytes[0] = 0x21, bytes[1] = 0x43, ..., bytes[15] = 0x12

// Deserializar desde bytes
uint128_t recovered = uint128_t::from_bytes(bytes);
assert(data == recovered);

// Funciona también con int128_t (preserva two's complement)
int128_t negative(-12345);
auto neg_bytes = negative.to_bytes();
int128_t restored = int128_t::from_bytes(neg_bytes);
```

#### Conversión a/desde Bitset (std::bitset<128>)

- [x] **to_bitset()**: Conversión a bitset para manipulación de bits
- [x] **from_bitset()**: Construcción desde bitset
- [x] **Casos de uso**: Máscaras de bits, flags, permisos, análisis binario

```cpp
uint128_t value(0xF, 0xFF);

// Convertir a bitset
std::bitset<128> bits = value.to_bitset();
std::cout << "Bits activados: " << bits.count() << "/128\n";

// Manipular bits individuales
bits.set(127);    // Activar MSB
bits.reset(0);    // Desactivar LSB

// Convertir de vuelta
uint128_t modified = uint128_t::from_bitset(bits);

// Sistema de permisos con 128 flags
std::bitset<128> permissions;
permissions.set(0);   // READ
permissions.set(1);   // WRITE
permissions.set(10);  // ADMIN
uint128_t perms_compact = uint128_t::from_bitset(permissions);
```

#### Demo Completo

Ver [`demos/demo_bytes_bitset.cpp`](demos/demo_bytes_bitset.cpp) para ejemplos prácticos de:

- Serialización/deserialización para red o disco
- Sistema de permisos con 128 flags
- Análisis de datos binarios y checksums
- Conversión entre representaciones (two's complement)
- Operaciones con máscaras de bits

### ✅ Casos de Uso Prácticos

- [x] Cálculos financieros de alta precisión
- [x] Operaciones criptográficas
- [x] Contadores de gran rango
- [x] IDs únicos de 128 bits
- [x] Compatibilidad con algoritmos STL

## 📊 Benchmarks

### Sistema de Benchmarks Completo

El proyecto incluye un **sistema avanzado de benchmarks** con las siguientes capacidades:

#### 🆚 Benchmarks Comparativos con Boost.Multiprecision

**Nuevo**: Benchmarks head-to-head con `boost::multiprecision::uint128_t`

```bash
# Ejecutar benchmarks comparativos (requiere Boost instalado)
bash scripts/run_boost_comparison.bash
```

**Qué compara**:

- ✅ **Construcción y asignación** (default, desde uint64_t, copy)
- ✅ **Aritmética básica** (+, -, *, /, %)
- ✅ **Operaciones bit a bit** (&, |, ^, <<, >>)
- ✅ **Conversiones string** (to_string, from_string, hex)
- ✅ **Funciones matemáticas** (gcd, lcm, pow, sqrt)

**Resultados esperados**: uint128_t ~2-3× más rápido en operaciones 128-bit fijas

**Usando Makefile**:

```bash
# Compilar y ejecutar con GCC
make build_benchs TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc MODE=release
make run TYPE=uint128 FEATURE=comparison_boost COMPILER=gcc MODE=release

# Pipeline completo (build + run)
make comparison_boost-full
```

**Documentación completa**:

- [COMPARISON_BOOST_FEATURE_SUMMARY.md](COMPARISON_BOOST_FEATURE_SUMMARY.md) - Documentación de la FEATURE
- [BOOST_COMPARISON_ANALYSIS.md](BOOST_COMPARISON_ANALYSIS.md) - Análisis detallado de features y performance
- [BOOST_COMPARISON_QUICKSTART.md](BOOST_COMPARISON_QUICKSTART.md) - Guía rápida de uso

**Comparación de características**:

| Característica | uint128_t | Boost.Multiprecision | unsigned __int128 |
|---------------|-----------|---------------------|-------------------|
| **Performance (128-bit)** | ✅ ~2-3× más rápido | ⚠️ Genérico | ✅ ~10-20% más rápido |
| **std::is_integral** | ✅ true | ❌ false | ❌ false |
| **C++20 concepts** | ✅ Completo | ❌ No | ❌ No |
| **Thread safety** | ✅ 4 wrappers | ❌ No | ❌ No |
| **Precisión arbitraria** | ❌ Solo 128-bit | ✅ Ilimitada | ❌ Solo 128-bit |
| **Portabilidad** | ✅ Todos | ✅ Todos | ⚠️ Solo GCC/Clang |

#### 🆚 Benchmark int128_t vs uint128_t

Comparación de performance entre tipos con signo y sin signo para medir el overhead del manejo de signo:

```bash
# Compilar y ejecutar
bash scripts/build_benchmark_int128_vs_uint128.bash gcc release
bash scripts/run_benchmark_int128_vs_uint128.bash gcc release
```

**6 categorías evaluadas** (30+ tests):

1. Construcción y asignación (4 tests)
2. Aritmética básica (6 tests: +, -, *, /, %, negación)
3. Operaciones bitwise (5 tests: &, |, ^, <<, >>)
4. Conversiones string (6 tests: positivos y negativos)
5. Funciones matemáticas (5 tests: gcd, lcm, pow, sqrt, abs)
6. Comparaciones (3 tests: ==, <, >)

**Overhead típico**: 5-10% en operaciones aritméticas, 0-2% en bitwise

**Documentación completa**: [BENCHMARK_INT128_VS_UINT128.md](BENCHMARK_INT128_VS_UINT128.md)

#### � FEATURE interop - Interoperabilidad uint128_t ↔ int128_t

**Suite unificada para validación de interoperabilidad** entre tipos signed y unsigned:

```bash
# Usando el Makefile con FEATURE=interop
make build_benchs TYPE=uint128 FEATURE=interop COMPILER=gcc MODE=release
make run TYPE=uint128 FEATURE=interop COMPILER=gcc MODE=release
make build_tests TYPE=uint128 FEATURE=interop COMPILER=gcc MODE=release
make check TYPE=uint128 FEATURE=interop COMPILER=gcc MODE=release

# Pipeline completo (build + check + run)
make interop-full COMPILER=gcc MODE=release
```

**Incluye**:

- **Benchmark int128_vs_uint128**: Medición de overhead signed (30+ tests)
- **Tests de interoperabilidad**: ✅ **17/17 tests PASSING (100%)**
  - Conversiones explícitas uint128_t ↔ int128_t
  - Operaciones aritméticas y bitwise mixtas
  - **Type traits**: `std::common_type`, `std::make_signed`, `std::make_unsigned`, `std::is_integral`
  - **Funciones numéricas**: gcd/lcm personalizado
  - Casos límite y asignaciones seguras

**Ventajas**:

- ✅ **Unificación**: Un solo comando para toda la validación
- ✅ **Consistencia**: Mismo patrón que otras FEATURES del Makefile
- ✅ **Atajos automáticos**: `make interop-full` ejecuta todo
- ✅ **Flexible**: Funciona con TYPE=uint128 o TYPE=int128
- ✅ **Completo**: Cobertura 100% de operaciones mixtas

**Documentación**:

- [INTEROP_FEATURE_SUMMARY.md](INTEROP_FEATURE_SUMMARY.md) - Resumen general
- [TYPE_TRAITS_IMPLEMENTATION.md](TYPE_TRAITS_IMPLEMENTATION.md) - Type traits y gcd/lcm

#### �📈 Benchmarks Internos

- ✅ **Comparación de tipos (uint128_t)**:
  - `uint128_t` (nuestra implementación)
  - `uint64_t` y `uint32_t` (baseline)
  - `__uint128_t` (tipo nativo GCC/Clang)
- ✅ **Comparación de tipos (int128_t)**:
  - `int128_t` (nuestra implementación signed)
  - `int64_t` y `int32_t` (baseline signed)
  - `__int128_t` (tipo nativo GCC/Clang)
  - `boost::int128_tmplementación)
  - `uint64_t` y `uint32_t` (baseline)
  - `__uint128_t` (tipo nativo GCC/Clang)
  - `boost::multiprecision` (cpp_int, GMP, tommath)
- ✅ **Mediciones duales**:
  - Tiempo (nanosegundos)
  - Ciclos de CPU (RDTSC)
- ✅ **Visualización profesional**:
  - Gráficos matplotlib (PNG)
  - Reportes Markdown
  - CSV/JSON export
- ✅ **Backends opcionales**:
  - Boost.Multiprecision pure
  - Boost + GMP backcompila y ejecuta uint128 + int128)
./scripts/build_benchmarks.bash gcc
./scripts/run_benchmarks.bash gcc

# Benchmark con todos los backends (Boost + GMP + tommath)

./scripts/build_with_backends.bash gcc --all-backends
./build/build_benchs/gcc/release/uint128_benchmark_gcc
./build/build_benchs/gcc/release/int128_benchmark_gcc

# Agregar y analizar resultados (genera reportes separados)

python scripts/aggregate_benchmark_results.py

# Generar gráficos (detecta uint128 e int128 automáticamente)

python scripts/plot_benchmark_results.py

# Ver resultados

cat benchmark_results/benchmark_report_uint128_*.md
cat benchmark_results/benchmark_report_int128_*
**uint128_t (unsigned)**:

- [BENCHMARK_GUIDE.md](documentation/BENCHMARK_GUIDE.md) - Guía completa uint128
- [BENCHMARK_IMPROVEMENTS.md](documentation/BENCHMARK_IMPROVEMENTS.md) - Mejoras implementadas
- [BENCHMARK_SUMMARY.md](documentation/BENCHMARK_SUMMARY.md) - Resumen ejecutivo
- **int128_t (signed)**:
  - [INT128_GUIDE.md](documentation/INT128_GUIDE.md) - 📘 **Guía completa int128_t** (NEW)
    - Two's complement y sign extension
    - Aritmética signed vs unsigned
   uint128_benchmarks_*.csv` - Resultados raw uint128 por compilador
- `int128_benchmarks_*.csv` - Resultados raw int128 por compilador
- `consolidated_uint128_*.csv/json/md` - Resultados agregados uint128
- `consolidated_int128_*.csv/json/md` - Resultados agregados int128
- `plots/` - Gráficos PNG separados por tipo:
  - `uint128_time_*.png`, `uint128_cycles_*.png`, etc.
  - `int128_time_*.png`, `int128_cycles_*.png`, etc.
cat benchmark_results/benchmark_summary.md
ls benchmark_results/plots/

```

#### 📚 Documentación

- **uint128_t (unsigned)**:
  - [BENCHMARK_GUIDE.md](documentation/BENCHMARK_GUIDE.md) - Guía completa uint128
  - [BENCHMARK_IMPROVEMENTS.md](documentation/BENCHMARK_IMPROVEMENTS.md) - Mejoras implementadas
  - [BENCHMARK_SUMMARY.md](documentation/BENCHMARK_SUMMARY.md) - Resumen ejecutivo
  - [THREAD_SAFETY_ANALYSIS.md](documentation/THREAD_SAFETY_ANALYSIS.md) - Análisis thread safety
  - [THREAD_SAFETY_USAGE.md](documentation/THREAD_SAFETY_USAGE.md) - Guía de wrappers thread-safe
- **int128_t (signed)**: 
  - [INT128_GUIDE.md](documentation/INT128_GUIDE.md) - 📘 **Guía completa int128_t**
    - Two's complement y sign extension
    - Aritmética signed vs unsigned
    - Benchmarks y comparaciones de rendimiento
    - Casos de uso prácticos
- **Thread Safety**:
  - [THREAD_SAFETY_STATUS.md](documentation/THREAD_SAFETY_STATUS.md) - 🔒 **Estado thread safety**
    - uint128_t: ✅ Completo (4 wrappers disponibles)
    - int128_t: ⏳ Pendiente (implementación trivial)

## 🎮 Demos y Ejemplos Interactivos

El proyecto incluye un **sistema completo de 35 demos** organizados en 7 categorías, demostrando las capacidades de `uint128_t` e `int128_t` en escenarios reales.

### 📂 Estructura de Demos

```

demos/
├── general/          - 6 demos de conceptos generales y análisis
├── tutorials/        - 16 demos tutoriales paso a paso (01-13 + extras)
├── examples/         - 9 demos de casos de uso reales
├── showcase/         - 4 demos de demostraciones avanzadas
├── comparison/       - Comparaciones con otras bibliotecas (vacío)
├── performance/      - Análisis de rendimiento (vacío)
└── integration/      - Ejemplos de integración (vacío)

```

### 🚀 Sistema de Build de Demos

#### Compilar un Demo Individual

```bash
# Usando scripts bash (recomendado)
bash scripts/build_demo.bash <category> <demo_name> [compiler] [mode]

# Ejemplos
bash scripts/build_demo.bash tutorials 01_basic_operations gcc release
bash scripts/build_demo.bash examples ipv6_address clang debug

# Usando Python (alternativa)
python make.py build demos <category> <demo_name> [compiler] [mode]
python make.py build demos tutorials 01_basic_operations gcc release

# Usando Makefile
make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release
```

#### Ejecutar un Demo

```bash
# Scripts bash
bash scripts/run_demo.bash <category> <demo_name> [compiler] [mode] [args...]

# Ejemplos
bash scripts/run_demo.bash tutorials 01_basic_operations
bash scripts/run_demo.bash examples big_integer_calculator gcc release

# Python (con argumentos)
python make.py run demos examples big_integer_calculator gcc release --help

# Makefile (atajo: compilar + ejecutar)
make demo CATEGORY=tutorials DEMO=01_basic_operations
```

#### Verificar Compilación de Todos los Demos

```bash
# Verificar todos los demos
python make.py check demos all gcc release

# Verificar una categoría específica
python make.py check demos tutorials gcc release

# Salida: Matriz mostrando 35/35 demos OK
# -> Total checks: 35
# [OK] Passed: 35
```

### 📚 Tutoriales (16 demos)

Ubicación: `demos/tutorials/`

Progresión paso a paso desde conceptos básicos hasta operaciones avanzadas:

| Demo | Descripción | LOC |
|------|-------------|-----|
| **01_basic_operations.cpp** | Constructores y operaciones básicas | 150 |
| **02_bitwise_operations.cpp** | Operaciones bit a bit (&, \|, ^, <<, >>) | 180 |
| **03_comparisons.cpp** | Comparaciones y ordenamiento | 200 |
| **04_conversions.cpp** | Conversión desde/hacia otros tipos | 250 |
| **05_string_conversion.cpp** | Conversiones string (decimal, hex, oct, bin) | 170 |
| **06_iostream.cpp** | Entrada/salida con streams | 140 |
| **07_format.cpp** | Formateo con std::format (C++20) | 160 |
| **08_cmath_functions.cpp** | Funciones matemáticas (sqrt, pow, gcd) | 190 |
| **09_algorithms_stl.cpp** | Algoritmos STL (sort, find, accumulate) | 220 |
| **10_numeric_functions.cpp** | Funciones numéricas (iota, accumulate) | 180 |
| **11_thread_safety.cpp** | Wrappers thread-safe y sincronización | 200 |
| **12_safe_operations.cpp** | Detección de overflow/underflow | 170 |
| **13_udl_literals.cpp** | Literales de usuario (_u128,_U128) | 130 |
| **demo_practical_udl.cpp** | UDL en casos prácticos | 150 |
| **test_simple.cpp** | Tests simples de funcionalidad | 100 |
| **test_uint128_only.cpp** | Tests específicos uint128_t | 120 |

**Total**: 16 demos, ~2,710 líneas de código

```bash
# Compilar todos los tutoriales
bash scripts/build_all_demos.bash tutorials gcc release

# Ejecutar un tutorial específico
bash scripts/run_demo.bash tutorials 01_basic_operations
```

### 🎭 Showcases Avanzados (4 demos)

Ubicación: `demos/showcase/`

Demostraciones interactivas avanzadas:

#### 1. **main.cpp** - Demo Principal Interactivo (640 LOC)

Demo principal con 7 secciones navegables:

- Operaciones aritméticas básicas
- Operaciones bit a bit
- Conversiones y formateo
- Uso en contenedores STL
- Algoritmos STL (sort, find, accumulate)
- Operaciones seguras (overflow detection)
- Funciones matemáticas (sqrt, pow, gcd, lcm)

```bash
make demo CATEGORY=showcase DEMO=main
```

#### 2. **showcase_cryptography.cpp** - Aplicaciones Criptográficas (580 LOC)

- **RSA toy implementation** (cifrado/descifrado educativo)
- **Test de primalidad Miller-Rabin** (5 iteraciones)
- **Búsqueda de primos** cerca de 10^9
- **Exponenciación modular** rápida
- **Primos de Mersenne** (M_p = 2^p - 1)
- **Funciones hash** simples con aritmética modular

⚠️ Solo para fines educativos - usar OpenSSL en producción

#### 3. **showcase_scientific.cpp** - Computación Científica (620 LOC)

- **Combinatoria avanzada**:
  - Factoriales grandes (hasta 34!)
  - Coeficientes binomiales C(n, k)
  - Triángulo de Pascal
  - Números de Catalan
- **Secuencias numéricas**:
  - Fibonacci (hasta overflow de uint128_t)
  - Números de Lucas
  - Crecimiento exponencial
- **Aproximaciones matemáticas**:
  - Aproximación de π (método de Leibniz)
  - Aproximación de e (serie de Taylor)
  - Convergencia iterativa

#### 4. **showcase_performance.cpp** - Análisis de Rendimiento

Benchmarks comparativos **uint128_t vs uint64_t** (1M iteraciones):

- **Operaciones aritméticas**: suma, multiplicación, división
- **Operaciones bitwise**: AND, shift left, popcount
- **Algoritmos STL**: sort, accumulate, find
- **Análisis de memoria**: sizeof, alignment, cache line
- **Recomendaciones de uso** basadas en overhead medido

Resultados típicos:

- Suma: 2-3× más lento
- Multiplicación: 50-80× más lento (esperado)
- División: 10-20× más lento
- Bitwise: 2-10× más lento

#### 5. **Expression Templates** - Técnicas Avanzadas de Metaprogramación ★ NUEVO

**Colección completa de 5 demos sobre Expression Templates** para eliminar temporales intermedios en operaciones con uint128_t.

##### [expression_templates_simple.cpp](demos/showcase/expression_templates_simple.cpp) (620 LOC)

- **Nivel**: Principiante
- Implementación didáctica con value semantics
- Operaciones básicas: +, -, *, /
- Introducción a CRTP y lazy evaluation
- Ideal para aprender el concepto

##### [expression_templates.cpp](demos/showcase/expression_templates.cpp) (582 LOC)

- **Nivel**: Intermedio
- Explicación detallada del método CRTP
- Comparación con/sin Expression Templates
- Árbol de expresiones visualizado
- Benchmarks de rendimiento

##### [expression_templates_fold.cpp](demos/showcase/expression_templates_fold.cpp) (489 LOC)

- **Nivel**: Intermedio-Avanzado
- **Fold expressions C++17**: `(... + args)` y `(args + ...)`
- Comparación: implementación manual vs fold nativo
- Operaciones múltiples: sum, product, OR, AND, XOR
- Evaluación constexpr completa

```cpp
// Ejemplo de fold expression
template<typename... Args>
auto sum_fold(Args... args) {
    return (... + args.eval());  // ¡Una sola línea!
}
// Expande a: ((((arg1 + arg2) + arg3) + arg4) + ...)
```

##### [expression_templates_complete.cpp](demos/showcase/expression_templates_complete.cpp) (895 LOC)

- **Nivel**: Avanzado
- ✅ **10 operaciones binarias**: +, -, *, /, %, &, |, ^, <<, >>
- ✅ **Operaciones unarias**: ~, !
- ✅ **Multi-asociativas**: sum(), product(), bitwise_or/and()
- ✅ **CSE (Common Subexpression Elimination)**: Optimización de subexpresiones comunes
- ✅ **constexpr completo**: Evaluación en compile-time
- ✅ Type-safe con SFINAE

##### [expression_templates_horner.cpp](demos/showcase/expression_templates_horner.cpp) (644 LOC)

- **Nivel**: Avanzado
- **Método de Horner** optimizado con ET para evaluación polinomial
- Composición recursiva: `(a*x + b)` → `(a*x + b)*y + c` → ...
- Zero temporales en evaluación de polinomios de grado arbitrario
- Visualización del árbol de expresiones
- Benchmarks para polinomios de grado 3, 5 y 10

```cpp
// Ejemplo de composición incremental sin temporales
Terminal x(3), y(4), z(2);
auto expr1 = 2*x + 5;              // No evalúa, construye árbol
auto expr2 = expr1 * y + 7;        // Extiende el árbol
auto expr3 = expr2 * z + 1;        // Continúa extendiendo
uint128_t result = expr3.eval();   // ¡Evalúa todo de una vez!
```

**Resumen de Expression Templates**:

| Demo | LOC | Nivel | Características Principales |
|------|-----|-------|----------------------------|
| simple | 620 | Principiante | Value semantics, operaciones básicas |
| expression_templates | 582 | Intermedio | Introducción CRTP, lazy evaluation |
| fold | 489 | Intermedio-Avanzado | Fold expressions C++17 nativos |
| complete | 895 | Avanzado | Sistema completo con CSE y multi-assoc |
| horner | 644 | Avanzado | Polinomios con método de Horner |

**Total**: 5 demos, ~3,230 líneas de código

```bash
# Compilar y ejecutar demos de Expression Templates
make demo CATEGORY=showcase DEMO=expression_templates_simple
make demo CATEGORY=showcase DEMO=expression_templates_fold
make demo CATEGORY=showcase DEMO=expression_templates_horner
make demo CATEGORY=showcase DEMO=expression_templates_complete
```

**Beneficios de Expression Templates**:

- ✅ Eliminación de temporales intermedios (16 bytes cada uno)
- ✅ Una sola evaluación al final del árbol de expresiones
- ✅ Zero overhead con optimizaciones del compilador
- ✅ Sintaxis natural (igual que operaciones normales)
- ✅ Funciona con constexpr para compile-time
- ✅ Type-safe en compile-time

#### 6. **demo_bytes_bitset.cpp** - Conversiones Bytes y Bitset

Demo completo de nuevas capacidades de serialización y manipulación:

**5 casos de uso prácticos**:

1. **Serialización/Deserialización**: Conversión a bytes para red o disco
2. **Sistema de permisos**: 128 flags en un solo uint128_t
3. **Análisis binario**: Conteo de bits, checksums XOR
4. **Representación two's complement**: Visualización de números negativos
5. **Máscaras de bits**: Operaciones con patrones de bits

```bash
g++ -std=c++20 -I include demos/demo_bytes_bitset.cpp -o demos/demo_bytes_bitset.exe
./demos/demo_bytes_bitset.exe
```

**Funciones demostradas**:

- `to_bytes()` / `from_bytes()` - Serialización a std::array<std::byte, 16>
- `to_bitset()` / `from_bitset()` - Conversión a std::bitset<128>
- Manipulación de bits individuales
- Verificación de checksums
- Little-endian byte order

### 🔧 Ejemplos de Uso Real (9 demos)

Ubicación: `demos/examples/`

Implementaciones completas de casos de uso prácticos:

| Demo | Descripción | LOC |
|------|-------------|-----|
| **ipv6_address.cpp** | Gestión completa de direcciones IPv6 | 320 |
| **uuid_generation.cpp** | Generación y manejo de UUIDs (RFC 4122) | 280 |
| **big_integer_calculator.cpp** | REPL interactivo para aritmética 128-bit | 410 |
| **prime_factorization.cpp** | Factorización de primos con Pollard's Rho | 350 |
| **mersenne_primes.cpp** | Búsqueda de primos de Mersenne (M_p = 2^p - 1) | 290 |
| **demo_int128_thread_safety.cpp** | Wrappers thread-safe para int128_t | 350 |
| **demo_mathematical_library.cpp** | Biblioteca matemática avanzada | 195 |
| **example_thread_safety.cpp** | Ejemplos completos de thread safety | 254 |
| **int128.cpp** | Casos de uso con int128_t (signed) | 340 |

**Total**: 9 demos, ~2,789 líneas de código

#### 1. **ipv6_address.cpp** - Gestión de Direcciones IPv6 (320 LOC)

Clase completa `IPv6Address` con:

- Parsing desde strings (formato estándar y comprimido)
- Conversión a strings (::ffff:192.0.2.1)
- Operaciones de subred (apply_mask, in_subnet)
- Iteración de rangos (operator++, operator--)
- Detección de tipos (loopback, link-local, multicast)

```bash
make demo CATEGORY=examples DEMO=ipv6_address
```

#### 2. **uuid_generation.cpp** - Generación de UUIDs (280 LOC)

Clase `UUID` compatible con RFC 4122:

- Generación UUID v4 (aleatorio)
- Parsing desde strings (550e8400-e29b-41d4-a716-446655440000)
- Conversión a strings con guiones
- Extracción de versión y variante
- Operadores de comparación y ordenamiento
- UUID nil (00000000-0000-0000-0000-000000000000)

#### 3. **big_integer_calculator.cpp** - Calculadora Interactiva (410 LOC)

REPL completo con:

- **Operaciones básicas**: add, sub, mul, div, mod, pow
- **Funciones especiales**: factorial, fibonacci, gcd, lcm
- **Memoria**: guardar/recuperar valores
- **Formatos**: entrada decimal/hex, salida dec/hex/oct/bin
- **Interactivo**: comandos tipo CLI

```bash
make demo CATEGORY=examples DEMO=big_integer_calculator
> fact 30
30! = 265252859812191058636308480000000
```

#### 4. **prime_factorization.cpp** - Factorización de Primos (350 LOC)

Implementación completa de factorización:

- Algoritmo de división por tentativa optimizado
- Test de primalidad probabilístico
- Pollard's Rho para factores grandes
- Verificación de resultados (producto = original)
- Detección de cuadrados perfectos
- Análisis de potencias de 2
- Ejemplos con números grandes (>10^15)

#### 5. **mersenne_primes.cpp** - Primos de Mersenne (290 LOC)

Exploración de números de Mersenne (M_p = 2^p - 1):

- Generación eficiente con desplazamientos de bits
- Test de primalidad Lucas-Lehmer (simplificado)
- Tabla de primos de Mersenne conocidos (M_2 a M_127)
- Análisis de crecimiento exponencial
- Detección de números compuestos
- Relación con números perfectos (Teorema de Euclides-Euler)
- Visualización de magnitudes

#### 6. **demo_int128_thread_safety.cpp** - Thread Safety Wrappers (350 LOC)

Wrappers thread-safe para operaciones 128-bit:

- `SafeInt128`: Wrapper con std::mutex
- `AtomicInt128`: Wrapper con std::atomic (si disponible)
- Operaciones thread-safe: add, sub, mul, div
- Lectura/escritura atómica
- Ejemplos de race conditions y su prevención
- Benchmarks de overhead de sincronización

#### 7. **demo_mathematical_library.cpp** - Biblioteca Matemática (195 LOC)

Funciones matemáticas avanzadas para uint128_t:

- **NOTA**: Muchas funciones aún no implementadas (sqrt, cbrt, exp, log, sin, cos)
- Operaciones básicas implementadas: abs, min, max, clamp
- Funciones auxiliares: is_power_of_two, count_leading_zeros
- Framework para extensión futura
- Ejemplos de uso y casos de prueba

#### 8. **example_thread_safety.cpp** - Ejemplos Thread Safety (254 LOC)

Casos completos de uso concurrente:

- Counter concurrente con std::atomic
- Suma paralela con múltiples threads
- Producer-consumer con queue thread-safe
- Comparación: mutex vs lock-free
- Medición de performance con diferentes niveles de contención
- Ejemplos de deadlock y su prevención

#### 9. **int128.cpp** - Casos de Uso con int128_t (340 LOC)

Ejemplos prácticos usando int128_t (signed):

- Aritmética con valores negativos
- Overflow y underflow detection
- Conversiones signed/unsigned
- Operaciones mixtas (int128_t con uint128_t)
- Comparaciones con negativos
- Formateo e I/O con signos

### 🧩 Demos Generales (6 demos)

Ubicación: `demos/general/`

Análisis y demostraciones técnicas:

| Demo | Descripción | LOC |
|------|-------------|-----|
| **analysis_summary.cpp** | Resumen de análisis de la biblioteca | 156 |
| **bit_analysis.cpp** | Análisis detallado de representación de bits | 189 |
| **demo_bytes_bitset.cpp** | Conversiones bytes/bitset con endianness | 198 |
| **demo_constexpr_cstr.cpp** | Validación de constexpr con C-strings | 95 |
| **demo_symmetry_complete.cpp** | Pruebas de simetría de operadores | 287 |
| **demo_symmetry_fixed.cpp** | Corrección de asimetrías detectadas | 234 |

**Total**: 6 demos, ~1,159 líneas de código

#### 1. **analysis_summary.cpp** - Resumen de Análisis (156 LOC)

Análisis global de la biblioteca:

- Estadísticas de implementación (operadores, funciones, traits)
- Análisis de conformidad con estándares (std::numeric_limits, type_traits)
- Resumen de features (I/O, conversiones, operadores bitwise)
- Compatibilidad de compiladores
- Métricas de cobertura

#### 2. **bit_analysis.cpp** - Análisis de Bits (189 LOC)

Exploración detallada de representación interna:

- Visualización de bits (formato binario con separadores)
- Análisis little-endian vs big-endian
- Estructura interna (low 64 bits, high 64 bits)
- Operaciones bit a bit (set, clear, flip, test)
- Máscaras y patrones comunes
- Ejemplos con números especiales (0, max, potencias de 2)

#### 3. **demo_bytes_bitset.cpp** - Conversiones Bytes/Bitset (198 LOC)

Sistema completo de conversiones:

- `uint128_t` ↔ `std::array<uint8_t, 16>` (bytes)
- `uint128_t` ↔ `std::bitset<128>` (bits)
- Soporte para little-endian y big-endian
- Validación de reversibilidad (roundtrip)
- Casos de prueba exhaustivos
- Utilidades de visualización

#### 4. **demo_constexpr_cstr.cpp** - Validación Constexpr (95 LOC)

Pruebas de evaluación en tiempo de compilación:

- Parsing de C-strings en constexpr context
- Validación de operaciones constexpr
- Comparación compile-time vs runtime
- Casos límite y manejo de errores
- Ejemplos con literales UDL

#### 5. **demo_symmetry_complete.cpp** - Pruebas de Simetría (287 LOC)

Testing exhaustivo de simetría de operadores:

- Operadores aritméticos: +, -, *, /, %
- Operadores bitwise: &, |, ^
- Operadores de comparación: ==, !=, <, <=, >, >=
- Operadores de desplazamiento: <<, >>
- Mixtos: uint128_t vs tipos nativos (int, long, unsigned)
- Detección de asimetrías y bugs

#### 6. **demo_symmetry_fixed.cpp** - Corrección de Asimetrías (234 LOC)

Implementación de correcciones:

- Friend operators para simetría completa
- Sobrecarga de operadores mixtos
- Validación de correcciones aplicadas
- Comparación antes/después
- Test regression para evitar regresiones futuras

### � Catálogo Completo de Demos

Para una referencia rápida de todos los 35 demos disponibles, consulta el catálogo auto-generado:

```bash
# Generar catálogo actualizado
bash scripts/catalog_demos.bash

# Ver catálogo
cat DEMOS_CATALOG.md
```

El catálogo incluye:

- Lista completa por categoría con líneas de código
- Descripción breve de cada demo
- Comandos de compilación sugeridos
- Totales y estadísticas

---

## Sistema de Construccion para Demos

El sistema de construccion unificado soporta compilar, ejecutar y verificar demos usando **tres interfaces diferentes**: bash scripts, Python scripts, y Make.

### Opcion 1: Scripts Bash (recomendado para uso interactivo)

```bash
# Compilar un demo individual
bash scripts/build_generic.bash demos tutorials 01_basic_operations

# Ejecutar un demo compilado
bash scripts/run_generic.bash demos tutorials 01_basic_operations arg1 arg2

# Compilar y ejecutar todos los demos
bash scripts/build_all_demos.bash

# Verificar todos los demos (compilacion + estadisticas)
bash scripts/check_generic.bash demos
```

### Opcion 2: Scripts Python (recomendado para automatizacion)

```bash
# Compilar un demo individual
python scripts/build_generic.py demos tutorials 01_basic_operations

# Ejecutar un demo compilado con argumentos
python scripts/run_generic.py demos tutorials 01_basic_operations arg1 arg2

# Verificar todos los demos (matriz de resultados)
python scripts/check_generic.py demos
```

### Opcion 3: Make (recomendado para integracion con IDEs)

```bash
# Compilar y ejecutar un demo en un solo comando
make demo CATEGORY=tutorials DEMO=01_basic_operations

# Compilar un demo sin ejecutarlo
make build_demo CATEGORY=tutorials DEMO=01_basic_operations

# Ejecutar un demo ya compilado
make run_demo CATEGORY=tutorials DEMO=01_basic_operations

# Verificar todos los demos
make check_demos
```

### make.py - Interfaz Unificada (nueva en diciembre 2025)

El script `make.py` proporciona una interfaz tipo Make con comandos intuitivos:

```bash
# Ver ayuda con todos los comandos disponibles
python make.py help

# Listar todos los tests, benchs y demos
python make.py list

# Compilar y ejecutar un demo
python make.py demo tutorials/01_basic_operations

# Compilar sin ejecutar
python make.py build demos/tutorials/01_basic_operations

# Ejecutar un demo ya compilado
python make.py run demos/tutorials/01_basic_operations arg1 arg2

# Verificar todos los demos (matriz de compilacion)
python make.py check demos
```

### Deteccion Automatica de Threading

**Todos los scripts de compilacion** (bash, Python, make.py) detectan automaticamente cuando un demo usa threads/atomic y anaden las flags necesarias:

- Deteccion de `<thread>`, `std::thread`, `pthread_*`
- Deteccion de `<atomic>`, `std::atomic`, `thread_safety.hpp`
- Anade automaticamente: `-pthread -latomic`
- **Sin necesidad de configuracion manual**

Ejemplos de demos con threading:

- `demos/examples/demo_int128_thread_safety.cpp`
- `demos/examples/example_thread_safety.cpp`
- `demos/showcase/showcase_thread_safety.cpp`

### Resultados de Verificacion

Ejecutar `python scripts/check_generic.py demos` muestra una matriz con el estado de compilacion de cada demo:

```
========================================
RESULTADOS DE COMPILACION - DEMOS
========================================

[✓] general/analysis_summary.cpp
[✓] general/bit_analysis.cpp
[✓] general/demo_bytes_bitset.cpp
...
[✓] showcase/showcase_thread_safety.cpp

========================================
RESUMEN: 35/35 demos compilaron exitosamente (100%)
========================================
```

**Estado actual**: ✅ **35/35 demos compilando correctamente** (100% de exito)

---

### 🧪 Tests y Benchmarks Extraídos de uint128_traits.hpp

**Sistema completo de tests y benchmarks individualizados** para cada función/especialización de `uint128_traits.hpp`:

#### 📝 Características

- ✅ **18 tests individuales** - Una función de test por cada trait/especialización
- ✅ **11 benchmarks individuales** - Medición precisa de cada operación
- ✅ **4 compiladores** - GCC (UCRT64), Clang (CLANG64), Intel OneAPI, MSVC
- ✅ **Métricas duales** - Tiempo (nanosegundos) + Ciclos de reloj (RDTSC)
- ✅ **Automatización completa** - Scripts para compilar, ejecutar y analizar

#### 📂 Archivos

- **Tests**: `tests/uint128_traits_extracted_tests.cpp`
- **Benchmarks**: `benchs/uint128_traits_extracted_benchs.cpp`
- **Scripts de compilación**: `scripts/compile_uint128_traits_extracted.{sh,bat}`
- **Scripts de ejecución**: `scripts/run_uint128_traits_benchmarks.{sh,bat}`
- **Script maestro**: `scripts/master_uint128_traits_all.{sh,bat}` - Ejecuta todo automáticamente

#### 🚀 Uso Rápido

```bash
# Ejecutar TODO automáticamente (compilar + tests + benchmarks + reportes)
./scripts/master_uint128_traits_all.sh
```

```cmd
REM Versión Windows
scripts\master_uint128_traits_all.bat
```

#### 📊 Tests Incluidos

1. **Type Traits Fundamentales**: `is_integral`, `is_arithmetic`, `is_unsigned`, `is_signed`
2. **Traits de Trivialidad**: `is_trivially_copyable`, `is_trivially_copy_constructible`, etc.
3. **Transformación de Tipos**: `make_unsigned`
4. **Common Type**: `common_type<uint128_t, T>` con múltiples tipos T
5. **Hash**: `std::hash<uint128_t>` y uso en `unordered_map`
6. **Numeric Limits**: Todas las propiedades de `std::numeric_limits<uint128_t>`

#### ⚡ Benchmarks Incluidos

1. **Compile-Time Traits** (5) - Overhead de verificación de traits
2. **Runtime Operations** (4) - Copia trivial, hash computation, map insert/lookup
3. **Numeric Limits** (2) - Query de propiedades y min/max

#### 📚 Documentación

- [uint128_traits_extracted_README.md](documentation/uint128_traits_extracted_README.md) - 📘 **Documentación completa**
- [UINT128_TRAITS_EXTRACTED_SUMMARY.md](UINT128_TRAITS_EXTRACTED_SUMMARY.md) - Resumen de implementación
- [QUICKSTART_UINT128_TRAITS.md](QUICKSTART_UINT128_TRAITS.md) - Guía rápida de uso

#### 📈 Resultados

Los resultados se guardan en `benchmark_results/`:

- `summary_[timestamp].csv` - CSV consolidado con todos los resultados
- `report_[timestamp].txt` - Reporte completo con análisis
- `benchmarks_[compiler]_[timestamp].txt` - Resultados por compilador
- `tests_[compiler]_[timestamp].txt` - Logs de tests por compilador

---

### 🔄 Tests de Interoperabilidad uint128_t ↔ int128_t

**Suite completa de tests para operaciones mixtas entre tipos signed y unsigned**:

#### 📝 Características

- ✅ **17 tests exhaustivos** - Validación completa de interoperabilidad
- ✅ **9 categorías** - Conversiones, aritmética, comparaciones, type traits, bitwise, límites, asignaciones, compuestas, math
- ✅ **4 compiladores** - GCC, Clang, Intel, MSVC
- ✅ **Output con colores** - Verde (pass) / Rojo (fail)
- ✅ **Automatización completa** - Scripts de build y ejecución

#### 📂 Archivos

- **Tests**: `tests/test_interoperability_uint128_int128.cpp`
- **Scripts build**: `scripts/build_test_interoperability.bash`
- **Scripts run**: `scripts/run_test_interoperability.bash`

#### 🚀 Uso Rápido

```bash
# Compilar tests
bash scripts/build_test_interoperability.bash gcc release

# Ejecutar tests
bash scripts/run_test_interoperability.bash gcc release
```

#### 📊 Tests Incluidos

1. **Conversiones explícitas** (2) - Casts seguros uint128_t ↔ int128_t
2. **Aritmética mixta** (4) - `+`, `-`, `*`, `/` con explicit cast
3. **Comparaciones** (2) - `==`, `!=`, `<`, `>`, `<=`, `>=`
4. **Type traits** (2) - `std::common_type`, `make_signed/unsigned`
5. **Bitwise** (1) - `&`, `|`, `^` entre tipos
6. **Casos límite** (3) - Zero, negativos, valores grandes
7. **Asignaciones** (1) - Verificación de rangos seguros
8. **Compuestas** (1) - Expresiones complejas
9. **Math functions** (1) - `std::gcd`, `std::lcm`

#### 📚 Documentación

- [INTEROPERABILITY_TESTS.md](INTEROPERABILITY_TESTS.md) - 📘 **Documentación completa**

#### ⚠️ Puntos Clave

- **Conversiones explícitas requeridas**: Las operaciones mixtas necesitan cast explícito
- **std::common_type funciona**: Está correctamente definido entre ambos tipos
- **Negativos → complemento a 2**: int128_t negativo se convierte a uint128_t vía complemento a 2
- **make_signed/unsigned**: Conversión bidireccional entre tipos

---

### 🧬 int128_traits.hpp - Tests y Benchmarks Extraídos

Suite completa de pruebas y benchmarks para `int128_traits.hpp` (tipo signed).

- ✅ **19 tests individuales** - Una función de test por cada trait/especialización
- ✅ **11 benchmarks individuales** - Medición precisa de cada operación
- ✅ **4 compiladores** - GCC (UCRT64), Clang (CLANG64), Intel OneAPI, MSVC
- ✅ **Métricas duales** - Tiempo (nanosegundos) + Ciclos de reloj (RDTSC)
- ✅ **Automatización completa** - Scripts para compilar, ejecutar y analizar

#### 📂 Archivos

- **Tests**: `tests/int128_traits_extracted_tests.cpp`
- **Benchmarks**: `benchs/int128_traits_extracted_benchs.cpp`
- **Scripts de compilación**: `scripts/compile_int128_traits_extracted.{sh,bat}`
- **Scripts de ejecución**: `scripts/run_int128_traits_benchmarks.{sh,bat}`
- **Script maestro**: `scripts/master_int128_traits_all.{sh,bat}` - Ejecuta todo automáticamente

#### 🚀 Uso Rápido

```bash
# Ejecutar TODO automáticamente (compilar + tests + benchmarks + reportes)
./scripts/master_int128_traits_all.sh
```

```cmd
REM Versión Windows
scripts\master_int128_traits_all.bat
```

#### 📊 Tests Incluidos

1. **Type Traits Fundamentales**: `is_integral`, `is_arithmetic`, `is_unsigned` (false), `is_signed` (true)
2. **Traits de Trivialidad**: `is_trivially_copyable`, `is_trivially_copy_constructible`, `is_trivial` (false), etc.
3. **Transformación de Tipos**: `make_signed` (devuelve int128_t), `make_unsigned` (devuelve uint128_t)
4. **Common Type**: `common_type<int128_t, T>` con múltiples tipos T, `common_type<int128_t, uint128_t>` (devuelve uint128_t)
5. **Hash**: `std::hash<int128_t>` con valores signed
6. **POD y Layout**: Verificación de `is_pod` y `is_standard_layout`

#### ⚡ Benchmarks Incluidos

1. **Type Traits** (4) - Overhead de verificación: `is_integral`, `is_arithmetic`, `is_signed`, `is_trivially_copyable`
2. **Hash Operations** (2) - Hash computation y hash con valores variables (incluye negativos)
3. **Copy/Move Operations** (2) - Rendimiento de copia y movimiento triviales
4. **Type Transformations** (2) - Uso de `common_type` y `make_unsigned` en contextos reales

#### 📚 Documentación

- [INT128_TRAITS_EXTRACTED_TESTS.md](documentation/INT128_TRAITS_EXTRACTED_TESTS.md) - 📘 **Documentación completa**

#### 📈 Resultados

Los resultados se guardan en `benchmark_results/`:

- `summary_[timestamp].csv` - CSV consolidado con todos los resultados
- `report_[timestamp].txt` - Reporte completo con análisis
- `benchmarks_[compiler]_[timestamp].txt` - Resultados por compilador
- `tests_[compiler]_[timestamp].txt` - Logs de tests por compilador

---

### 🎨 Formatting - uint128_format.hpp & int128_format.hpp ✅

Funciones avanzadas de formateo para uint128_t e int128_t con control total sobre el output.

#### 📋 Características

- ✅ **uint128_t**: 9 tests, formateo sin signo (hex, oct, dec)
- ✅ **int128_t**: 10 tests, formateo con signo (incluye showpos)
- ✅ **4 compiladores** - GCC, Clang (Intel/MSVC disponibles)
- ✅ **8 scripts modernos** - Sistema extractado completo
- ✅ **Bases múltiples** - Decimal (10), Hexadecimal (16), Octal (8)
- ✅ **Width y fill** - Padding con cualquier carácter
- ✅ **Alignment** - Left, Right, Internal (relleno después del prefijo)
- ✅ **Flags** - showbase (0x, 0), uppercase (A-F), showpos (signo +)

#### 🚀 API Completa

**uint128_format namespace:**

```cpp
#include "uint128/uint128_format.hpp"

// Formato completo con control total
std::string format(uint128_t value, 
                   int base = 10,           // 10, 16, 8
                   int width = 0,           // ancho mínimo
                   char fill = ' ',         // carácter de relleno
                   bool show_base = false,  // prefijo 0x/0
                   bool uppercase = false,  // A-F mayúsculas
                   bool left_align = false, // alinear izquierda
                   bool internal_align = false); // padding después prefijo

// Wrappers de conveniencia
std::string hex(uint128_t value, int width = 0, bool show_base = false, 
                bool uppercase = false, char fill = ' ');
std::string oct(uint128_t value, int width = 0, bool show_base = false, char fill = ' ');
std::string dec(uint128_t value, int width = 0, char fill = ' ', bool left_align = false);

// Formato como iostream (con fmtflags)
std::string format_like_iostream(uint128_t value, std::ios_base::fmtflags flags,
                                   int width = 0, char fill = ' ');
```

**int128_format namespace (con showpos):**

```cpp
#include "int128/int128_format.hpp"

// Formato completo con soporte para signo (nota: show_pos es el 5to parámetro)
std::string format(int128_t value, 
                   int base = 10, int width = 0, char fill = ' ',
                   bool show_base = false,
                   bool show_pos = false,   // mostrar '+' para positivos
                   bool uppercase = false,
                   bool left_align = false,
                   bool internal_align = false);

// Wrappers (misma firma que uint128_format)
std::string hex/oct/dec(...);  // Igual que uint128_format
```

#### 💡 Ejemplos

```cpp
// uint128_t formatting
uint128_t value(0xFF);
uint128_format::dec(value);                            // "255"
uint128_format::hex(value, 0, true, true);            // "0xFF"
uint128_format::dec(value, 10, '*');                  // "*******255"
uint128_format::hex(value, 10, true, true, '0');      // "0x000000FF"

// int128_t formatting (con signo)
int128_t pos(100), neg(-42);
int128_format::dec(neg);                               // "-42"
int128_format::format(pos, 10, 0, ' ', false, true);  // "+100" (showpos)
int128_format::hex(neg, 0, true);                     // two's complement
```

#### 📂 Archivos

- **Headers**: `include/uint128/uint128_format.hpp` (202 líneas), `include/int128/int128_format.hpp` (251 líneas)
- **Tests**: `tests/uint128_format_tests.cpp` (9 tests), `tests/int128_format_tests.cpp` (10 tests)
- **Benchmarks**: `benchs/uint128_format_benchmarks.cpp`, `benchs/int128_format_benchmarks.cpp`
- **Scripts**: 8 scripts modernos (build/check/run × uint128/int128 × tests/benchs)

#### ✅ Estado: COMPLETO

- ✅ **Tests**: 8/8 PASSED (GCC/Clang debug+release)
- ✅ **Benchmarks**: Ejecutados (uint128: ~264ms/50k ops dec, ~194ms hex; int128: ~259ms dec, ~253ms negative)
- ✅ **Relación con iostreams**: Complementa operator<</>> con funciones standalone
- ✅ **12/12 Features Completas** - **PROYECTO FINALIZADO** 🎉

---

#### 📊 Resultados Generales

Los resultados de benchmarks generales se almacenan en `benchmark_results/`:

- `benchmark_*.csv` - Resultados raw por compilador
- `consolidated_*.csv` - Resultados agregados
- `benchmark_summary.md` - Reporte en Markdown
- `plots/` - Gráficos PNG (time, cycles, speedup, etc.)

## �️ Arquitectura Modular

uint128_t utiliza una **arquitectura modular** para mejor organización y mantenibilidad:

### 📁 Módulos Principales

#### Core (`uint128_t.hpp`)

- Implementación principal de la clase uint128_t
- Todas las operaciones aritméticas, bitwise y de comparación
- Optimizaciones con intrínsecos del compilador
- Conversiones string y literales de usuario

#### Límites (`uint128_limits.hpp`)

- Especialización completa de `std::numeric_limits<uint128_t>`
- Constantes numéricas y propiedades del tipo

#### Traits (`uint128_traits.hpp`)

- Type traits: `std::is_integral`, `std::is_unsigned`, etc.
- `std::common_type` especializaciones
- `std::hash<uint128_t>` para containers

#### Conceptos (`uint128_concepts.hpp`)

- Conceptos C++20 personalizados
- Metaprogramación avanzada
- Verificaciones automáticas de compatibilidad

#### Algoritmos (`uint128_algorithm.hpp`)

- Algoritmos STL optimizados para uint128_t
- Funciones especializadas (GCD/LCM de rangos, estadísticas)
- Templates con concepts para type safety

#### Numérico (`uint128_numeric.hpp`) 🆕

- **Funciones C++20**: `std::midpoint`, `std::clamp`
- **Manipulación de bits**: `std::popcount`, `std::countl_zero`, `std::bit_width`
- **Potencias de 2**: `std::has_single_bit`, `std::bit_ceil`, `std::bit_floor`
- **Rotaciones**: `std::rotl`, `std::rotr`
- **Utilidades**: `isqrt`, `factorial`, `power`

### 🎯 Uso Recomendado

```cpp
// Para uso completo, simplemente incluir:
#include "uint128_simple_traits.hpp"  // Incluye todos los módulos

// O incluir módulos específicos según necesidades:
#include "uint128_t.hpp"              // Solo funcionalidad básica
#include "uint128_limits.hpp"         // + numeric_limits
#include "uint128_concepts.hpp"       // + conceptos C++20
// etc...
```

## �🎯 Type Traits y STL Compatibility

uint128_t incluye **completa compatibilidad con la biblioteca estándar de C++**:

```cpp
// Para usar traits completos, incluir:
#include "uint128_simple_traits.hpp"

// Type traits funcionan como esperado:
static_assert(std::is_integral_v<uint128_t>);
static_assert(std::is_unsigned_v<uint128_t>);
static_assert(std::numeric_limits<uint128_t>::digits == 128);

// C++20 concepts
template<std::unsigned_integral T>
void process(T value) { /* ... */ }
process(uint128_t(42)); // ✅ Funciona

// STL algorithms
### uint128_t (Unsigned)

```cpp
#include "uint128_t.hpp"

// Construcción básica
uint128_t a = 42;
uint128_t b(100, 200);  // high=100, low=200

// Literales de usuario
auto x = 1234_u128;
auto big = "18446744073709551616"_U128;

// Aritmética (sin signo)
auto result = a + b * x;

// Strings
uint128_t num("999999999999999999");
std::cout << num << std::endl;

// STL algorithms
std::vector<uint128_t> vec(10);
std::iota(vec.begin(), vec.end(), 1000_u128);
```

### int128_t (Signed)

```cpp
#include "int128/int128_t.hpp"

// Construcción con signo
int128_t a = -42;           // Sign extension automática
int128_t b(100, 200);       // high=100, low=200

// Aritmética signed
int128_t sum = a + 100;     // -42 + 100 = 58
int128_t prod = a * -2;     // -42 * -2 = 84
int128_t quot = -100 / 7;   // División signed (trunca a cero)

// Comparaciones signed (negativos < positivos)
assert(a < 0);              // true
assert(a < b);              // true (negativo < positivo)

// Strings con signo
int128_t neg("-999999999999999999");
std::cout << neg.to_string() << std::endl;  // "-999999999999999999"

// Detectar signo
if (a.is_negative()) {
    std::cout << "Negative value\n";
}

// Ver [INT128_GUIDE.md](documentation/INT128_GUIDE.md) para más detalles
```

### 🔢 Mathematical Functions (C++20 Standard Library Compatible)

#### uint128_cmath.hpp - Unsigned Math Functions

```cpp
#include "int128/uint128_cmath.hpp"

// Greatest Common Divisor (Binary GCD - Stein's algorithm)
uint128_t g = std::gcd(uint128_t(48), uint128_t(18));  // 6

// Least Common Multiple (optimized: avoids overflow)
uint128_t l = std::lcm(uint128_t(12), uint128_t(18));  // 36

// Power function (fast exponentiation by squaring)
uint128_t p = std::pow(uint128_t(2), uint128_t(10));   // 1024

// Square root (Newton's method)
uint128_t s = std::sqrt(uint128_t(100));                // 10

// Min/Max
uint128_t min_val = std::min(uint128_t(5), uint128_t(10));  // 5
uint128_t max_val = std::max(uint128_t(5), uint128_t(10));  // 10

// Extended GCD (for cryptography - modular inverse)
auto [gcd, x, y] = std::bezout_coeffs(uint128_t(17), uint128_t(13));
// Returns gcd and coefficients x, y such that: 17*x + 13*y = gcd

// ✅ All functions tested 100% with GCC, Clang, MSVC, Intel
// ✅ 96 comprehensive tests, all passing
// ✅ Constexpr support (GCC/Clang)
```

**Documentation:** [UINT128_CMATH_COMPLETION_SUMMARY.md](documentation/UINT128_CMATH_COMPLETION_SUMMARY.md)

#### int128_cmath.hpp - Signed Math Functions

```cpp
#include "int128/int128_cmath.hpp"

// GCD with negative inputs (handles sign automatically)
int128_t g = std::gcd(int128_t(-48), int128_t(18));    // 6

// LCM with negative inputs (returns absolute value)
int128_t l = std::lcm(int128_t(-12), int128_t(18));    // 36

// Absolute value
int128_t abs_val = std::abs(int128_t(-42));             // 42

// Power with negative base (handles odd/even exponents)
int128_t p1 = std::pow(int128_t(-2), int128_t(3));     // -8 (odd exponent)
int128_t p2 = std::pow(int128_t(-2), int128_t(4));     // 16 (even exponent)

// Square root (requires non-negative)
int128_t s = std::sqrt(int128_t(100));                  // 10

// Sign function (returns -1, 0, or +1)
int128_t sign = std::sign(int128_t(-42));               // -1

// Min/Max with negative values
int128_t min_val = std::min(int128_t(-5), int128_t(10));  // -5
int128_t max_val = std::max(int128_t(-5), int128_t(10));  // 10

// Clamp to range (bounds checking)
int128_t clamped = std::clamp(int128_t(15), int128_t(0), int128_t(10));  // 10

// Midpoint (overflow-safe: a + (b-a)/2)
int128_t mid = std::midpoint(int128_t(10), int128_t(20));  // 15

// ✅ All functions tested 100% with GCC, Clang, MSVC, Intel
// ✅ 141 comprehensive tests (GCC/Clang), 139 tests (MSVC/Intel)
// ✅ Constexpr support for sign, min, max, clamp (GCC/Clang)
```

**Documentation:**

- [INT128_CMATH_TESTING_COMPLETE.md](documentation/INT128_CMATH_TESTING_COMPLETE.md) - Full test results
- [INT128_VS_UINT128_CMATH_COMPARISON.md](documentation/INT128_VS_UINT128_CMATH_COMPARISON.md) - Comparative analysis
- [INT128_CMATH_COMPLETION_SUMMARY.md](documentation/INT128_CMATH_COMPLETION_SUMMARY.md) - Executive summary

**Key Features:**

- ✅ **Standard Library Compatible**: Drop-in replacements for `std::gcd`, `std::lcm`, etc.
- ✅ **Performance Optimized**: Binary GCD (O(log n)), fast exponentiation (O(log n))
- ✅ **Overflow Safe**: LCM and midpoint implementations prevent overflow
- ✅ **Sign Aware**: int128_cmath handles negative values correctly
- ✅ **Cryptography Ready**: Extended GCD (bezout_coeffs) for modular arithmetic
- ✅ **Cross-Compiler**: 100% success with GCC, Clang, MSVC, Intel oneAPI
- ✅ **237 Total Tests**: 96 (uint128) + 141 (int128) comprehensive validation

---

### 📘 Basic Usage Examples

#### uint128_t (Unsigned)

```cpp
#include "uint128_t.hpp"

// Construcción básica
uint128_t a = 42;
uint128_t b(100, 200);  // high=100, low=200

// Literales de usuario
auto x = 1234_u128;
auto big = "18446744073709551616"_U128;

// Aritmética
auto result = a + b * x;

// Strings
uint128_t num("999999999999999999");
std::cout << num << std::endl;

// STL algorithms
std::vector<uint128_t> vec(10);
std::iota(vec.begin(), vec.end(), 1000_u128);
```

## ⚡ Performance

- **Tamaño**: Exactamente 16 bytes (2 × uint64_t)
- **Alineación**: Optimizada para arquitecturas de 64 bits
- **Operaciones**: Implementadas con intrínsecos del compilador cuando está disponible
- **Constexpr**: Soporte para evaluación en tiempo de compilación

## 🔗 Compatibilidad

- **Compiladores**: MSVC 14.50+, GCC 10+, Clang 12+
- **Estándares**: C++20 o superior
- **Plataformas**: Windows, Linux, macOS
- **Arquitecturas**: x64, ARM64

## � Documentación HTML Generada

El proyecto utiliza **Doxygen** para generar documentación completa en formato HTML.

### 🔨 Generación de Documentación

```bash
# En MSYS2/Linux/macOS
bash scripts/generate_docs.bash

# En Windows PowerShell
powershell -ExecutionPolicy Bypass -File scripts/generate_docs.ps1
```

El script:

1. ✅ Verifica que Doxygen esté instalado
2. ✅ Genera documentación HTML en `documentation/generated/`
3. ✅ Genera 265 archivos HTML + 184 gráficos SVG (~10.68 MB)
4. ✅ Ofrece abrir automáticamente en el navegador

### 📂 Ubicación de la Documentación

```
documentation/
├── generated/              # 📄 HTML generado por Doxygen (NO commitear)
│   ├── html/
│   │   ├── index.html      # 🏠 Página principal
│   │   ├── annotated.html  # Lista de clases
│   │   └── ...             # 265+ archivos HTML
│   └── ...
├── doxygen/                # 📝 Páginas fuente Markdown
│   └── pages/
│       ├── quickstart.md   # Guía de inicio rápido
│       ├── examples.md     # Ejemplos de uso
│       └── architecture.md # Arquitectura interna
└── benchmarks/             # 📊 Resultados guardados de benchmarks
```

### 🌐 Visualización

**Opción 1** - Abrir directamente:

```bash
# Windows
start documentation/generated/html/index.html

# macOS
open documentation/generated/html/index.html

# Linux
xdg-open documentation/generated/html/index.html
```

**Opción 2** - Servidor HTTP local:

```bash
# Python 3
cd documentation/generated/html
python -m http.server 8000

# Abrir en navegador: http://localhost:8000
```

### ⚙️ Configuración Doxygen

- **Archivo**: `Doxyfile` (raíz del proyecto)
- **Idioma**: Español (`OUTPUT_LANGUAGE = Spanish`)
- **Formato**: HTML con JavaScript search
- **Gráficos**: Diagramas de colaboración y herencia (SVG)
- **Navegación**: Árbol de archivos, índices alfabéticos

### 📝 .gitignore

`documentation/generated/` está **excluido del control de versiones** para evitar:

- ❌ Commits innecesarios de 10.68 MB (1346 archivos)
- ❌ Conflictos de merge en archivos generados
- ❌ Histórico inflado del repositorio

**Regenerar documentación después de clonar**:

```bash
git clone <repository>
cd int128
bash scripts/generate_docs.bash  # Genera documentation/generated/
```

### 📚 Contenido Documentado

La documentación incluye:

- ✅ **API completa** de `uint128_t` y `int128_t`
- ✅ **Type traits** y especializaciones STL
- ✅ **Funciones matemáticas** (cmath, algorithm)
- ✅ **Guías de uso** (Quick Start, Examples)
- ✅ **Arquitectura interna** y optimizaciones
- ✅ **Benchmarks** y análisis de rendimiento

---

## 🔥 Fase 17 - Mejoras de Calidad (5 enero 2026)

### ⚠️ Corrección Crítica: operator~()

**Problema descubierto:** Orden de argumentos invertido en el constructor del operador de negación bit a bit.

```cpp
// ❌ ANTES (INCORRECTO):
constexpr int128_base_t operator~() const noexcept {
    return int128_base_t(~data[0], ~data[1]);  // low, high (INVERTIDO)
}

// ✅ DESPUÉS (CORRECTO):
constexpr int128_base_t operator~() const noexcept {
    return int128_base_t(~data[1], ~data[0]);  // high, low (ORDEN CORRECTO)
}
```

**Impacto en cascada:**

Este bug afectaba a **tres funciones críticas** que dependen de `operator~()`:

1. **`operator-()`** (negación aritmética): Utiliza `~value + 1` para implementar complemento a 2
2. **`abs()`**: Necesita negar valores negativos correctamente
3. **`to_string()`**: Convierte negativos a positivos antes de generar la cadena

**Síntoma observado:**

```cpp
int128_t neg(-42);
std::cout << neg.to_string();  // Mostraba basura en lugar de "-42"
```

**Estado:** ✅ Corregido y validado con **29/29 tests PASS** + **50+ edge cases PASS**

---

### ✨ Mejora: Migración a std::numeric_limits

**Cambio:** Reemplazo de números mágicos hexadecimales por constantes tipo-safe.

```cpp
// ❌ ANTES (números mágicos):
uint64_t mask = 0xFFFFFFFFFFFFFFFF;

// ✅ DESPUÉS (self-documenting):
uint64_t mask = std::numeric_limits<uint64_t>::max();
```

**Beneficios:**

- ✅ **Legibilidad mejorada**: El código es auto-documentado
- ✅ **Portabilidad**: Independiente de la plataforma
- ✅ **Type-safety**: El compilador verifica tipos correctos
- ✅ **Mantenibilidad**: Cambios en tipos más fáciles

**Reemplazos realizados:** 12 ocurrencias en `int128_base.hpp`

---

## 📚 Documentación del Proyecto

### 🗺️ Grafo de Documentación

Ver **[DOCUMENTATION_GRAPH.md](DOCUMENTATION_GRAPH.md)** para el mapa completo de interconexiones
entre los 114 archivos de documentación del proyecto. Este grafo define:

- Dependencias entre documentos
- Protocolo de actualización
- Plan de integración de directorios (`include_new/` → `include/`, etc.)

### Documentos Principales

| Documento | Descripción | Estado |
|-----------|-------------|--------|
| **[DOCUMENTATION_GRAPH.md](DOCUMENTATION_GRAPH.md)** | Mapa de dependencias entre documentos | ✅ Nuevo |
| **[API_INT128_BASE_TT.md](API_INT128_BASE_TT.md)** | Referencia API del template principal | ✅ Completo |
| **[CHANGELOG.md](CHANGELOG.md)** | Historial de versiones y cambios críticos | ✅ Actualizado |
| **[TODO.md](TODO.md)** | Roadmap y estado de tareas | 🔄 Actualizado |

### Referencias API (estilo cppreference)

| Documento | Header | Contenido |
|-----------|--------|-----------|
| [API_INT128_BASE_LIMITS.md](API_INT128_BASE_LIMITS.md) | `int128_base_limits.hpp` | `std::numeric_limits` |
| [API_INT128_BASE_TRAITS.md](API_INT128_BASE_TRAITS.md) | `int128_base_traits*.hpp` | Type traits STL |
| [API_INT128_BASE_CONCEPTS.md](API_INT128_BASE_CONCEPTS.md) | `int128_base_concepts.hpp` | Concepts C++20 |
| [API_INT128_BASE_BITS.md](API_INT128_BASE_BITS.md) | `int128_base_bits.hpp` | Operaciones de bits |
| [API_INT128_BASE_NUMERIC.md](API_INT128_BASE_NUMERIC.md) | `int128_base_numeric.hpp` | Algoritmos numéricos |
| [API_INT128_BASE_ALGORITHM.md](API_INT128_BASE_ALGORITHM.md) | `int128_base_algorithm.hpp` | Algoritmos STL |
| [API_INT128_BASE_CMATH.md](API_INT128_BASE_CMATH.md) | `int128_base_cmath.hpp` | Funciones matemáticas |
| [API_INT128_BASE_IOSTREAMS.md](API_INT128_BASE_IOSTREAMS.md) | `int128_base_iostreams.hpp` | Stream I/O |
| [API_INT128_BASE_FORMAT.md](API_INT128_BASE_FORMAT.md) | `int128_base_format.hpp` | Formateo avanzado |

### Documentos Técnicos

| Documento | Descripción | Estado |
|-----------|-------------|--------|
| **[TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md](TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md)** | Demostración matemática multiplicación | ✅ Completo |
| **[documentation/generated/html/index.html](documentation/generated/html/index.html)** | Documentación Doxygen | ✅ Generado |

### API Reference - Contenido

**[API_INT128_BASE_T.md](API_INT128_BASE_T.md)** contiene:

- **20 secciones organizadas** con todas las firmas de funciones
- **Enumeraciones**: `signedness`, `parse_error`
- **Type aliases**: `uint128_t`, `int128_t`
- **Constantes globales**: `UINT128_MAX`, `INT128_MAX`, `INT128_MIN`
- **Constructores**: 12+ sobrecargas (default, integrales, pares, strings, conversiones)
- **Operadores**: Completa matriz de sobrecarga (aritméticos, bitwise, comparación, shift)
- **Métodos auxiliares**: `to_string()`, `abs()`, `is_negative()`, `divrem()`, etc.
- **Funciones estáticas**: `parse()`, `parse_base()`, `min()`, `max()`
- **Stream I/O**: `operator<<`, `operator>>`
- **UDL operators**: `_u128`, `_U128`, `_i128`, `_I128`
- **Factory functions**: `make_int128<S>()`, `make_uint128()`, etc.
- **Anotaciones de Fase 17**: Operadores corregidos claramente marcados con ⚠️

### Demostración Matemática

**[TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md](TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md)** demuestra:

```
∀ n, m ∈ ℤ : repr(n × m) = repr(n) × repr(m)  (mod 2^N)
```

**Conclusión práctica:** La multiplicación funciona idénticamente para tipos signed y unsigned en complemento a 2, permitiendo una implementación unificada sin ramas condicionales por signo.

---

## ✅ Estado de Testing

### Core Tests: 29/29 PASS

| Test File | Tests | Status | Descripción |
|-----------|-------|--------|-------------|
| test_tostring_debug.cpp | 5/5 | ✅ PASS | Validación de Fase 17 (operator~, to_string) |
| test_parse_direct.cpp | 6/6 | ✅ PASS | Parsing con separadores, hex, manejo de errores |
| test_parse_notostring.cpp | 6/6 | ✅ PASS | Verificación de valores mínimos/máximos |
| test_literals_fixed.cpp | 7/7 | ✅ PASS | Factory functions y constexpr |
| test_constants.cpp | 5/5 | ✅ PASS | Constantes globales (MAX/MIN) |

### Edge Case Tests: 50+ PASS

| Test File | Coverage | Status |
|-----------|----------|--------|
| **test_tostring_edges.cpp** | **Comprehensive boundary testing** | ✅ **PASS** |

**Categorías de edge cases testeadas:**

- INT128_MIN/MAX validation
- UINT128_MAX validation  
- Zero region testing (-1, 0, +1)
- Powers of 2 (2^64, 2^96, 2^120, 2^127)
- Base conversion (bases 2-36)
- Round-trip parsing validation
- Operator chain testing

**Total Coverage:** ✅ Todos los caminos críticos validados

---

## 🛠️ Compatibilidad de Compiladores

### Windows x86_64 (MSYS2) - Completamente Testeado

| Compiler | Version | Status | Tests | Notas |
|----------|---------|--------|-------|-------|
| **GCC (UCRT64)** | 15.2.0+ | ✅ Tested | 29/29 + 50+ | Compilador principal de desarrollo |
| **Clang (CLANG64)** | 19.1.0+ | ✅ Tested | 29/29 + 50+ | Compatibilidad completa |
| **MSVC** | **19.50.35721** | ✅ **Available** | **Verificado** | **x64 Optimizing Compiler confirmado** ✨ |
| **Intel ICX** | oneAPI 2024 | ⚠️ Partial | Básico | Algunos intrínsecos pueden diferir |

**Nota sobre MSVC:** Compilador Microsoft C/C++ 19.50.35721 para x64 disponible y funcional en el entorno de desarrollo.

### Otras Plataformas (Sin Testear - Código Portable)

- **Linux x86_64**: GCC 10+, Clang 12+, Intel ICX (código estándar C++20)
- **macOS x86_64/ARM64**: Clang (Apple), GCC (Homebrew)
- **ARM 32/64-bit**: GCC, Clang (con fallback a código genérico)
- **RISC-V 32/64-bit**: GCC, Clang (implementaciones portables)

**Portabilidad:** Los intrínsecos de optimización son opcionales. En plataformas sin soporte específico, la biblioteca usa automáticamente implementaciones C++20 estándar.

---

## 📝 Licencia

**Boost Software License 1.0**

Copyright (c) 2025 uint128_t Contributors

Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:

The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT.**

Ver [Boost Software License](https://www.boost.org/LICENSE_1_0.txt) para el texto completo de la licencia.
