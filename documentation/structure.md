# Estructura del Proyecto uint128_t

## 📁 Directorio Raíz
```
int128/
├── build_msvc.ps1          # Script principal de construcción (PowerShell)
├── msys_build.sh          # Script principal de construcción (Bash/MSYS2)
├── benchmark.ps1          # Ejecutar benchmarks normalizados  
├── benchmark.sh           # Ejecutar benchmarks normalizados (bash)
├── report.ps1             # Generar reporte de análisis
└── README.md              # Documentación principal
```

## 📂 Directorios de Código

### `include/` - Headers de la biblioteca
```
include/
├── uint128_t.hpp          # Implementación principal de uint128_t
└── uint128_traits.hpp     # Extensiones STL (numeric_limits, hash, concepts)
```

### `tests/` - Suite de pruebas
```
tests/
├── test_uint128.cpp       # Pruebas básicas de funcionalidad
├── test_user_literals.cpp # Pruebas de literales de usuario (_u128, _U128)
├── test_iostream.cpp      # Pruebas de entrada/salida
├── test_iota.cpp          # Pruebas de compatibilidad con std::iota
├── benchmark_test.cpp     # Pruebas de rendimiento básicas
└── normalized_benchmark.cpp # Benchmark normalizado vs uint64_t
```

### `scripts/` - Scripts auxiliares
```
scripts/
├── build.ps1              # Motor de construcción (PowerShell)
├── test.ps1               # Construcción rápida para desarrollo
├── build.sh               # Motor de construcción (Bash)
├── test.sh                # Construcción rápida (Bash)
├── run_benchmarks.ps1     # Ejecutor de benchmarks (PowerShell)
├── run_benchmarks.sh      # Ejecutor de benchmarks (Bash)
└── benchmark_report.ps1   # Generador de reportes de análisis
```

### `build/` - Artefactos de construcción
```
build/
├── debug/                 # Ejecutables en modo debug
├── release/               # Ejecutables optimizados
└── CMakeFiles/           # Archivos de construcción de CMake
```

### `documentation/` - Documentación y resultados
```
documentation/
├── README.md              # Documentación de la API
├── structure.md           # Este archivo - estructura del proyecto
└── benchmarks/           # Resultados de benchmarks
    ├── normalized_benchmark_*.csv  # Resultados normalizados
    ├── arithmetic_operations.csv   # Operaciones aritméticas
    └── comparison_operations.csv   # Operaciones de comparación
```

## 🚀 Flujo de Trabajo

### Desarrollo Rápido
```powershell
# PowerShell
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