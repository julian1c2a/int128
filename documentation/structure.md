# Estructura del Proyecto int128 (uint128_t + int128_t)

## 📁 Directorio Raíz
```
int128/
├── build_msvc.ps1          # Script principal de construcción (PowerShell)
├── msys_build.sh          # Script principal de construcción (Bash/MSYS2)
├── benchmark.ps1          # Ejecutar benchmarks normalizados  
├── benchmark.sh           # Ejecutar benchmarks normalizados (bash)
├── report.ps1             # Generar reporte de análisis
└── README.md              # Documentación principal (uint128_t + int128_t)
```

## 📂 Directorios de Código

### `include/` - Headers de la biblioteca

#### uint128_t (Unsigned 128-bit)
```
include/
├── uint128_t.hpp          # Implementación principal de uint128_t
├── uint128_traits.hpp     # Extensiones STL (numeric_limits, hash, concepts)
├── uint128_limits.hpp     # std::numeric_limits specialization
├── uint128_concepts.hpp   # C++20 concepts
├── uint128_algorithm.hpp  # Algoritmos optimizados
├── uint128_numeric.hpp    # Funciones numéricas C++20 uint128_t
├── test_user_literals.cpp # Pruebas de literales de usuario (_u128, _U128)
├── test_iostream.cpp      # Pruebas de entrada/salida
├── test_iota.cpp          # Pruebas de compatibilidad con std::iota
├── benchmark_test.cpp     # Pruebas de rendimiento básicas
├── normalized_benchmark.cpp # Benchmark normalizado vs uint64_t
├── uint128_extracted_tests.cpp # Suite completa uint128_t
├── uint128_thread_safety_tests.cpp # Tests de thread safety
└── int128_extracted_tests.cpp  # Suite completa int128_t (signed)
```

### `benchmarks/` - Benchmarks de rendimiento
```
benchmarks/
├── uint128_extracted_benchmarks.cpp # Benchmarks exhaustivos uint128_t
│                                     # (vs uint64_t, __uint128_t, Boost)
└── int128_extracted_benchmarks.cpp  # Benchmarks exhaustivos int128_t
                                      # (vs int64_t, __int128_t, Boost)
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
├── test_uint128.cpp       # Pruebas básicas de funcionalidad
├── test_user_literals.cpp # Pruebas de literales de usuario (_u128, _U128)
├── test_iostream.cpp      # Pruebas de entrada/salida
├── build_benchmarks.bash  # Compilar benchmarks (uint128 + int128, 4 compiladores)
├── run_benchmarks.bash    # Ejecutar benchmarks (uint128 + int128, 4 compiladores)
├── build_with_backends.bash # Compilar con backends opcionales (GMP, tommath)
├── aggregate_benchmark_results.py # Agregar y analizar resultados
├── plot_benchmark_results.py      # Generar gráficos matplotlib
├── test_thread_safety.bash        # Tests de thread safety (uint128_tas
└── normalized_benchmark.cpp # Benchmark normalizado vs uint64_t
```

### `scripts/` - Scripts auxiliares
├── benchmarks/            # Ejecutables de benchmarks
│   ├── uint128_benchmarks_{gcc,clang,msvc,intel}
│   ├── int128_benchmarks_{gcc,clang,msvc,intel}
│   └── *_benchmark_*      # Variantes con backends
```
scripts/
├── build.ps1              # Motor de construcción (PowerShell)
├── test.ps1               # Construcción rápida para desarrollo
├── api.md                 # Referencia de API completa
├── INT128_GUIDE.md        # 📘 Guía completa int128_t (NEW)
│                          #    - Two's complement, sign extension
│                          #    - Signed vs unsigned
│                          #    - Benchmarks y rendimiento
├── BENCHMARK_GUIDE.md     # Guía de benchmarks uint128_t
├── BENCHMARK_IMPROVEMENTS.md # Mejoras de benchmarks implementadas
├── BENCHMARK_SUMMARY.md   # Resumen ejecutivo de benchmarks
├── THREAD_SAFETY_ANALYSIS.md # Análisis thread safety uint128_t
├── THREAD_SAFETY_USAGE.md    # Guía de uso thread-safe wrappers
└── benchmarks/           # Resultados antiguos de benchmarks
    ├── normalized_benchmark_*.csv
    ├── arithmetic_operations.csv
    └── comparison_operations.csv
```

### `benchmark_results/` - Resultados de benchmarks (NEW)
```
benchmark_results/
├── uint128_benchmarks_{GCC,Clang,MSVC,Intel}_*.csv  # Raw uint128
├── uint128_benchmarks_{GCC,Clang,MSVC,Intel}_*.json
├── int128_benchmarks_{GCC,Clang,MSVC,Intel}_*.csv   # Raw int128
├── int128_benchmarks_{GCC,Clang,MSVC,Intel}_*.json
├── consolidated_uint128_*.{csv,json,md}  # Reportes agregados uint128
├── consolidated_int128_*.{csv,json,md}   # Reportes agregados int128
└── plots/                                # Gráficos matplotlib
    ├── uint128_time_*.png
    ├── uint128_cycles_*.png
    ├── uint128_compiler_*.png
    ├── uint128_speedup_heatmap.png
    ├── uint128_operations_overview.png
    ├── int128_time_*.png          # Gráficos signed
    ├── int128_cycles_*.png
    ├── int128_compiler_*.png

#### uint128_t (Unsigned)
- **uint128_t completo**: Aritmética, lógica, comparaciones unsigned
- **Literales de usuario**: `123_u128`, `0xFF_U128`
- **Constructores de string**: `uint128_t("12345")`
- **Integración STL**: `std::iota`, `std::hash`, `std::numeric_limits`
- **Thread safety**: Wrappers thread-safe (Mutex, RW-lock, SpinLock, Atomic)
- **Backends opcionales**: Boost.Multiprecision (pure, GMP, tommath)
- **__uint128_t support**: Detección y uso automático (GCC/Clang)

#### int128_t (Signed) - NEW
- **int128_t completo**: Aritmética signed con two's complement
- **Sign extension**: Automática desde int64_t, int32_t, etc.
- **Comparaciones signed**: Orden correcto (negativos < 0 < positivos)
- **División/módulo signed**: Semántica C++ estándar
- **Conversión a/desde string**: Con soporte de signo "-"
- **__int128_t support**: Detección y uso automático (GCC/Clang)
- **Thread safety**: NO implementado (pendiente)

### 🔧 Compiladores Soportados
- **MSVC 19.50+** (Visual Studio 2022)
- **GCC 11+** (con `-std=c++20`)
- **Clang 13+** (con `-std=c++20`)
- **Intel oneAPI** (icpx/icx)

### 📊 Sistema de Benchmarks Completo
- **Múltiples compiladores**: GCC, Clang, MSVC, Intel
- **Dos tipos**: uint128_t (unsigned) + int128_t (signed)
- **Comparaciones**: vs tipos nativos, __uint128_t/__int128_t, Boost
- **Mediciones duales**: Tiempo (ns) + Ciclos de CPU (RDTSC)
- **Visualización**: Gráficos matplotlib, reportes Markdown, CSV/JSON export
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

# 2. Compilar con backends (Boost + GMP + tommath)
./scripts/build_with_backends.bash gcc --all-backends
# Compila ambos con todos los backends

# 3. Ejecutar benchmarks
./scripts/run_benchmarks.bash gcc
# Ejecuta ambos, reporta "N successful, M failed"

# 4. Agregar y analizar resultados
python scripts/aggregate_benchmark_results.py
# Genera: consolidated_uint128_*.{csv,json,md}
#         consolidated_int128_*.{csv,json,md}

# 5. Generar gráficos
python scripts/plot_benchmark_results.py
# Genera: plots/uint128_*.png
#         plots/int128_*.png
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
### int128_t (Signed)
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
.\build_msvc.ps1 test_uint128 debug

# Bash (MSYS2)
./msys_build.sh test_uint128 debug
```

### Benchmarks y Análisis
```powershell
# Ejecutar benchmarks normalizados
.\benchmark.ps1

# Generar reporte de análisis
.\report.ps1
```

### Construcción de Release
```powershell
.\build_msvc.ps1 test_uint128 release
```

## 🎯 Características Clave

### ✅ Funcionalidad Implementada
- **uint128_t completo**: Aritmética, lógica, comparaciones
- **Literales de usuario**: `123_u128`, `0xFF_U128`
- **Constructores de string**: `uint128_t("12345")`
- **Integración STL**: `std::iota`, `std::hash`, `std::numeric_limits`
- **Sistema de construcción dual**: PowerShell + Bash
- **Benchmarks normalizados**: Comparación vs tipos nativos

### 🔧 Compiladores Soportados
- **MSVC 19.50+** (Visual Studio 2022)
- **GCC 11+** (con `-std=c++20`)
- **Clang 13+** (con `-std=c++20`)

### 📊 Métricas de Rendimiento
- **Operaciones bitwise**: 67% más rápido que uint64_t
- **Aritmética básica**: 33% más rápido que uint64_t  
- **Comparaciones**: Overhead de 67-233% (esperado)

## 💡 Uso Típico

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
```

## 🔄 Normalización de Benchmarks

El sistema genera archivos CSV con:
- **Información del sistema**: Compilador, timestamp, arquitectura
- **Métricas estadísticas**: Mediana, media, desviación estándar
- **Comparación relativa**: Performance vs uint64_t nativo
- **Cross-platform**: Resultados comparables entre máquinas

Esta estructura permite desarrollo eficiente, testing robusto y análisis de rendimiento normalizado.