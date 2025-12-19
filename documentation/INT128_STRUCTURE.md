# Estructura del Proyecto int128 (uint128_t + int128_t)

## 📁 Directorio Raíz
```
int128/
├── CMakeLists.txt         # Configuración CMake principal
├── CMakeUserPresets.json  # Presets de usuario para CMake
├── conanfile.txt          # Dependencias Conan
├── Makefile               # Makefile para construcción rápida
├── README.md              # Documentación principal (uint128_t + int128_t)
├── LICENSE.txt            # Licencia del proyecto
└── .clang-format          # Configuración de formato de código
```

**Nota:** Todos los scripts (.sh, .bash, .ps1, .bat, .py) ahora están organizados en `scripts/`

## 📂 Directorios de Código

### `include/` - Headers de la biblioteca

#### 🔗 Header Unificador
```
include/
└── int128.hpp             # ⭐ Header unificador para uint128_t e int128_t
                           # Incluye todos los submódulos de ambos tipos
                           # Uso: #include "int128.hpp"
```

**Nota:** `int128.hpp` es el punto de entrada recomendado que incluye automáticamente todos los headers de uint128_t e int128_t.

#### uint128_t (Unsigned 128-bit)
```
include/uint128/
├── uint128_t.hpp          # Implementación principal de uint128_t
├── uint128_traits.hpp     # Extensiones STL (numeric_limits, hash, concepts)
├── uint128_limits.hpp     # std::numeric_limits specialization
├── uint128_concepts.hpp   # C++20 concepts
├── uint128_algorithm.hpp  # Algoritmos optimizados
└── uint128_numeric.hpp    # Funciones numéricas C++20 uint128_t
```

#### int128_t (Signed 128-bit)
```
include/int128/
├── int128_t.hpp              # Implementación principal signed
├── int128_algorithm.hpp      # Algoritmos (división, módulo signed)
├── int128_arithmetic.hpp     # Operaciones aritméticas signed
├── int128_bitwise.hpp        # Operaciones bitwise
├── int128_comparison.hpp     # Comparaciones signed
├── int128_io.hpp             # Entrada/salida con signo
├── int128_limits.hpp         # std::numeric_limits<int128_t>
├── int128_traits.hpp         # Type traits signed
└── int128_thread_safety.hpp  # ✅ Wrappers thread-safe (4 opciones)
```

**Paridad completa**: int128_t ahora tiene las mismas capacidades que uint128_t, incluyendo thread safety

### `tests/` - Suite de pruebas
```
tests/
├── test_uint128.cpp               # Pruebas básicas de funcionalidad
├── test_user_literals.cpp         # Pruebas de literales (_u128, _U128)
├── test_iostream.cpp              # Pruebas de entrada/salida
├── uint128_extracted_tests.cpp    # Suite completa uint128_t
├── int128_extracted_tests.cpp     # Suite completa int128_t
├── uint128_thread_safety_tests.cpp # Tests thread safety uint128_t
└── int128_thread_safety_tests.cpp  # Tests thread safety int128_t
```

### `benchmarks/` - Benchmarks de rendimiento
```
benchmarks/
├── benchmark_suite.hpp                 # Suite común de benchmarks
├── uint128_extracted_benchmarks.cpp    # Benchmarks uint128_t
├── int128_extracted_benchmarks.cpp     # Benchmarks int128_t
├── uint128_thread_safety_benchmarks.cpp # Thread safety uint128_t
├── int128_thread_safety_benchmarks.cpp  # Thread safety int128_t
├── benchmark_complete.cpp              # Suite completa
├── benchmark_division.cpp              # Benchmarks de división
├── benchmark_optimized.cpp             # Versión optimizada
└── README.md                           # Documentación de benchmarks
```

### `demos/` - Demostraciones y ejemplos
```
demos/
├── demo_int128_thread_safety.cpp  # Demo thread safety
├── demo_constexpr_cstr.cpp        # Demo constexpr
├── demo_mathematical_library.cpp  # Demo matemáticas
├── demo_practical_udl.cpp         # Demo user-defined literals
├── demo_symmetry_complete.cpp     # Demo simetría operadores
├── example_thread_safety.cpp      # Ejemplos thread safety
└── int128.cpp                     # Ejemplo básico int128_t
```

### `scripts/` - Scripts de construcción y testing
```
scripts/
├── build.ps1                      # Motor construcción (PowerShell)
├── build.sh                       # Motor construcción (Bash)
├── test.ps1                       # Tests rápidos (PowerShell)
├── test.sh                        # Tests rápidos (Bash)
├── benchmark.bash                 # Ejecutar benchmarks
├── benchmark.ps1                  # Ejecutar benchmarks (PowerShell)
├── build_msvc.ps1                 # Construcción MSVC
├── build_simple.bat               # Construcción simple
├── msys_build.sh                  # Construcción MSYS2
├── test_compilers.sh              # Test múltiples compiladores
├── report.ps1                     # Generar reportes
├── vcvars.py                      # Activar variables MSVC
├── vcvarsall.py                   # Variables MSVC completas
├── verify.py                      # Verificar configuración
├── run_*.sh                       # Scripts de ejecución varios
├── run_*.ps1                      # Scripts PowerShell varios
├── build_benchmarks.bash          # Compilar benchmarks
├── run_benchmarks.bash            # Ejecutar benchmarks
├── build_with_backends.bash       # Compilar con backends
├── aggregate_benchmark_results.py # Agregar resultados
├── plot_benchmark_results.py      # Generar gráficos
└── test_thread_safety.bash        # Tests thread safety
```

### `benchmark_results/` - Resultados de benchmarks
```
benchmark_results/
├── uint128_benchmarks_GCC_15_2.csv       # Resultados por compilador
├── uint128_benchmarks_GCC_15_2.json
├── benchmark_msvc.csv                    # Resultados MSVC
├── benchmark_output_msvc.txt             # Output detallado
├── summary.txt                           # Resumen de resultados
└── benchmark_results_*.csv               # Resultados históricos
```

### `build/` - Archivos de construcción y ejecutables
```
build/
├── build_tests/                   # Ejecutables de tests
│   ├── msvc/
│   │   ├── debug/
│   │   └── release/
│   │       ├── uint128_extracted_tests.exe
│   │       └── int128_thread_safety_tests.exe
│   ├── gcc/
│   │   ├── debug/
│   │   └── release/
│   └── clang/
│       ├── debug/
│       └── release/
├── build_benchmarks/              # Ejecutables de benchmarks
│   ├── msvc/
│   │   └── release/
│   │       ├── uint128_thread_safety_bench.exe
│   │       └── int128_thread_safety_bench.exe
│   ├── gcc/
│   │   └── release/
│   └── clang/
│       └── release/
├── build_demos/                   # Ejecutables de demos
│   └── msvc/
│       └── release/
│           └── demo_int128_thread_safety.exe
└── [otros archivos CMake]         # cmake_install.cmake, etc.
```

### `documentation/` - Documentación del proyecto
```
documentation/
├── api.md                        # Referencia de API completa
├── INT128_STRUCTURE.md          # Este archivo - estructura del proyecto
├── INT128_GUIDE.md              # Guía completa int128_t
├── BENCHMARK_GUIDE.md           # Guía de benchmarks uint128_t
├── THREAD_SAFETY_ANALYSIS.md    # Análisis thread safety
└── THREAD_SAFETY_USAGE.md       # Guía wrappers thread-safe
```

### `debugging/` - Herramientas de depuración
```
debugging/
├── debug_basic.cpp               # Tests básicos de depuración
├── debug_bezout.cpp              # Debug algoritmo Bezout
├── debug_bits_detailed.cpp       # Análisis detallado de bits
└── [otros archivos debug]
```

### `external/` - Dependencias externas
```
external/
└── [bibliotecas de terceros]
```

### `sources/` - Código fuente adicional
```
sources/
└── [implementaciones complementarias]
```

## 🎯 Características por Tipo

#### uint128_t (Unsigned)
- **uint128_t completo**: Aritmética, lógica, comparaciones unsigned
- **Literales de usuario**: `123_u128`, `0xFF_U128`
- **Constructores de string**: `uint128_t("12345")`
- **Integración STL**: `std::iota`, `std::hash`, `std::numeric_limits`
- **Thread safety**: Wrappers thread-safe (Mutex, RW-lock, SpinLock, Atomic)
- **Backends opcionales**: Boost.Multiprecision (pure, GMP, tommath)
- **__uint128_t support**: Detección y uso automático (GCC/Clang)

#### int128_t (Signed)
- **int128_t completo**: Aritmética signed con two's complement
- **Sign extension**: Automática desde int64_t, int32_t, etc.
- **Comparaciones signed**: Orden correcto (negativos < 0 < positivos)
- **División/módulo signed**: Semántica C++ estándar
- **Conversión a/desde string**: Con soporte de signo "-"
- **__int128_t support**: Detección y uso automático (GCC/Clang)
- **Thread safety**: Implementado (4 opciones)

### 🔧 Compiladores Soportados
- **MSVC 19.50+** (Visual Studio 2022)
- **GCC 11+** (con `-std=c++20`)
- **Clang 13+** (con `-std=c++20`)
- **Intel oneAPI** (icpx/icx)

### 📊 Sistema de Benchmarks Completo
- **Múltiples compiladores**: GCC, Clang, MSVC, Intel oneAPI
- **Dos tipos**: uint128_t (unsigned) + int128_t (signed)
- **Comparaciones**: vs tipos nativos, __uint128_t/__int128_t, Boost
- **Mediciones duales**: Tiempo (ns) + Ciclos de CPU (RDTSC)
- **Visualización**: Gráficos matplotlib, reportes Markdown, CSV/JSON export
- **Organización**: Ejecutables en `build/build_benchmarks/[compiler]/[mode]/`
### uint128_t (Unsigned)
```cpp
#include "include/uint128_t.hpp"
using namespace uint128_literals;

// Literales de usuario
auto big_num = 0xFFFFFFFFFFFFFFFF_u128;
auto decimal = 12345678901234567890_U128;

// Constructores de string
uint128_t from_string("123456789012345678901234567890");

// Uso con STL
std::vector<uint128_t> values(100);
std::iota(values.begin(), values.end(), 1_u128);

// ThrSistema de Benchmarks

### Características
- **Múltiples compiladores**: GCC, Clang, MSVC, Intel (4 compiladores)
- **Dos tipos completos**: uint128_t (unsigned) + int128_t (signed)
- **Compilación paralela**: Scripts compilan ambos tipos simultáneamente
- **Ejecución paralela**: Scripts ejecutan ambos tipos con tracking individual
- **Agregación separada**: Reportes independientes por tipo
- **Visualización dual**: Gráficos con prefijos (uint128_*, int128_*)

### Flujo de Trabajo

```bash
# 1. Compilar benchmarks (básico)
./scripts/build_benchmarks.bash gcc
# Compila: uint128_benchmarks_gcc + int128_benchmarks_gcc
# Output: build/build_benchmarks/gcc/release/

# 2. Compilar con backends (Boost + GMP + tommath)
./scripts/build_with_backends.bash gcc --all-backends
# Compila ambos con todos los backends
# Output: build/build_benchmarks/gcc/release/

# 3. Ejecutar benchmarks
./scripts/run_benchmarks.bash gcc
# Ejecuta ambos desde build/build_benchmarks/gcc/release/
# Reporta "N successful, M failed"

# 4. Agregar y analizar resultados
python scripts/aggregate_benchmark_results.py
# Genera: benchmark_results/consolidated_uint128_*.{csv,json,md}
#         benchmark_results/consolidated_int128_*.{csv,json,md}

# 5. Generar gráficos
python scripts/plot_benchmark_results.py
# Genera: benchmark_results/plots/uint128_*.png
#         benchmark_results/plots/int128_*.png
```

### Resultados Generados
- **CSV**: Resultados raw por operación, tipo, compilador
- **JSON**: Mismo contenido en formato JSON
- **Markdown**: Reportes con tablas, análisis, top performers
- **PNG**: Gráficos de tiempo, ciclos, speedup, comparaciones

## 📚 Documentación Clave

### uint128_t (Unsigned)
- [BENCHMARK_GUIDE.md](BENCHMARK_GUIDE.md) - Guía completa de benchmarks
- [THREAD_SAFETY_ANALYSIS.md](THREAD_SAFETY_ANALYSIS.md) - Análisis thread safety
- [THREAD_SAFETY_USAGE.md](THREAD_SAFETY_USAGE.md) - Guía de wrappers thread-safe

### int128_t (Signed)
- [INT128_GUIDE.md](INT128_GUIDE.md) - 📘 **Guía completa signed**
  - Two's complement y representación interna
  - Sign extension explicada
  - Aritmética signed vs unsigned
  - Benchmarks y comparaciones de rendimiento
  - Casos de uso prácticos

Esta estructura permite desarrollo eficiente, testing robusto y análisis de rendimiento completo para **ambos tipos** (unsigned y signed)

## 💡 Ejemplos de Uso

### Opción 1: Header Unificador (Recomendado)
```cpp
#include "int128.hpp"  // ⭐ Incluye todo: uint128_t + int128_t
using namespace uint128_literals;

// uint128_t (unsigned)
auto big_num = 0xFFFFFFFFFFFFFFFF_u128;
uint128_t from_string("123456789012345678901234567890");

// int128_t (signed)
int128_t negative(-42);
int128_t positive(100);

// Uso mixto
std::vector<uint128_t> unsigned_values(100);
std::vector<int128_t> signed_values(100);
```

### Opción 2: Headers Individuales

#### uint128_t (Unsigned)
```cpp
#include "include/uint128/uint128_t.hpp"
using namespace uint128_literals;

// Literales de usuario
auto big_num = 0xFFFFFFFFFFFFFFFF_u128;
auto decimal = 12345678901234567890_U128;

// Constructores de string
uint128_t from_string("123456789012345678901234567890");

// Uso con STL
std::vector<uint128_t> values(100);
std::iota(values.begin(), values.end(), 1_u128);
```

#### int128_t (Signed)
```cpp
#include "include/int128/int128_t.hpp"

// Construcción con signo (sign extension automática)
int128_t negative(-42);
int128_t positive(100);

// Aritmética signed
int128_t sum = negative + positive;  // -42 + 100 = 58
int128_t prod = negative * -2;       // -42 * -2 = 84

// División signed (trunca a cero)
int128_t quot = -100 / 7;  // -14 (no -15)
int128_t rem = -100 % 7;   // -2  (signo de dividendo)

// Comparaciones signed
assert(negative < 0);        // true
assert(negative < positive); // true (negativo < positivo)

// String con signo
int128_t from_str("-999999999999999999");
std::cout << from_str.to_string() << "\n";  // "-999999999999999999"
```

## 🚀 Guía de Construcción

### Construcción Básica
```powershell
# PowerShell (Windows) - Tests
.\scripts\build_msvc.ps1 test_uint128 debug
# Output: build/build_tests/msvc/debug/test_uint128.exe

# Bash (MSYS2) - Tests
./scripts/msys_build.sh test_uint128 debug
# Output: build/build_tests/gcc/debug/test_uint128
```

### Benchmarks
```powershell
# Ejecutar benchmarks normalizados (PowerShell)
.\scripts\benchmark.ps1
# Output: benchmark_results/

# Ejecutar benchmarks (Bash)
./scripts/run_benchmarks.bash gcc
# Ejecutables desde: build/build_benchmarks/gcc/release/

# Generar reporte de análisis
.\scripts\report.ps1
```

### Demos
```bash
# Compilar y ejecutar demos
# Ejecutables en: build/build_demos/[compiler]/[mode]/
./build/build_demos/msvc/release/demo_int128_thread_safety.exe
```

## 🎯 Características Clave

### ✅ Funcionalidad Implementada
- **uint128_t completo**: Aritmética, lógica, comparaciones unsigned
- **int128_t completo**: Aritmética signed con two's complement
- **Literales de usuario**: `123_u128`, `0xFF_U128`
- **Constructores de string**: `uint128_t("12345")`, `int128_t("-12345")`
- **Integración STL**: `std::iota`, `std::hash`, `std::numeric_limits`
- **Thread safety**: Wrappers thread-safe para ambos tipos
- **Sistema de construcción**: CMake + PowerShell + Bash
- **Benchmarks completos**: Comparación vs tipos nativos y Boost

### 🔧 Compiladores Soportados
- **MSVC 19.50+** (Visual Studio 2022)
- **GCC 11+** (con `-std=c++20`)
- **Clang 13+** (con `-std=c++20`)
- **Intel oneAPI** (icpx/icx)

### 📊 Métricas de Rendimiento
- **Operaciones bitwise**: 67% más rápido que uint64_t
- **Aritmética básica**: 33% más rápido que uint64_t  
- **Comparaciones**: Overhead de 67-233% (esperado para 128-bit)
- **Thread safety**: Benchmarks específicos para operaciones concurrentes

## 📐 Organización de Ejecutables

La nueva estructura organiza los ejecutables por propósito:

```
build/
├── build_tests/[compiler]/[mode]/         # Tests unitarios
├── build_benchmarks/[compiler]/[mode]/    # Benchmarks de rendimiento
└── build_demos/[compiler]/[mode]/         # Demostraciones y ejemplos
```

Donde:
- `[compiler]` = msvc, gcc, clang, intel
- `[mode]` = debug, release

Esta estructura permite:
- **Separación clara** entre tests, benchmarks y demos
- **Múltiples compiladores** en paralelo sin conflictos
- **Builds incrementales** eficientes
- **Ejecución organizada** desde PATH con scripts/

## 🔄 Sistema de Benchmarks

El sistema genera resultados en `benchmark_results/`:
- **CSV/JSON**: Datos raw por operación, compilador y tipo
- **Markdown**: Reportes con análisis y tablas comparativas
- **Gráficos PNG**: Visualización matplotlib de rendimiento
- **Información del sistema**: Compilador, timestamp, arquitectura
- **Métricas estadísticas**: Mediana, media, desviación estándar
- **Comparación relativa**: Performance vs tipos nativos
- **Cross-platform**: Resultados comparables entre máquinas

---

Esta estructura permite desarrollo eficiente, testing robusto, análisis de rendimiento completo y organización clara de ejecutables para **ambos tipos** (unsigned y signed).