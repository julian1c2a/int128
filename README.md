# uint128_t - Implementación de enteros de 128 bits

Una implementación completa y eficiente de enteros sin signo de 128 bits para C++20, compatible con MSVC, GCC y Clang.

## 📁 Estructura del Proyecto

```
int128/
├── include/                    # Headers públicos (arquitectura modular)
│   ├── uint128_t.hpp          # Implementación principal
│   ├── uint128_limits.hpp     # std::numeric_limits especializations
│   ├── uint128_traits.hpp     # Type traits y std::hash
│   ├── uint128_concepts.hpp   # C++20 concepts personalizados
│   ├── uint128_algorithm.hpp  # Algoritmos optimizados
│   ├── uint128_numeric.hpp    # Funciones numéricas C++20 (midpoint, clamp, etc.)
│   └── uint128_simple_traits.hpp # Header de conveniencia (incluye todo)
├── tests/                     # Tests unitarios
│   ├── basic_test.cpp
│   ├── final_traits_test.cpp  # Test completo de type traits
│   ├── test_numeric_functions.cpp # Test funciones numéricas C++20
│   └── ...
├── sources/                   # Código fuente de bibliotecas compiladas
├── build/                     # Archivos compilados
│   ├── test/
│   │   ├── debug/            # Tests compilados en modo debug
│   │   └── release/          # Tests compilados en modo release
│   └── lib/                  # Bibliotecas compiladas (futuro)
├── documentation/            # Documentación
│   ├── benchmarks/          # Resultados de benchmarks (.csv)
│   ├── api.md              # Documentación de API
│   └── examples.md         # Ejemplos de uso
├── TRAITS_SUMMARY.md        # Resumen completo de type traits
├── TODO.md                  # Estado del proyecto y roadmap
├── LICENSE.txt              # Licencia Boost Software License 1.0  
└── scripts/                 # Scripts de build y utilidades
```

## 🛠️ Sistema de Build

### Scripts de Build

- `build_msvc.ps1` - Script PowerShell principal (recomendado para Windows)
- `build.ps1` - Script PowerShell original  
- `build.sh` - Script bash para entornos Unix-like (con cygpath)
- `msys_build.sh` - Script bash optimizado para MSYS2

### Uso del Sistema de Build

#### En PowerShell (Recomendado):
```powershell
# Compilar y ejecutar un test específico (debug)
.\build_msvc.ps1 user_literals_test debug

# Compilar en modo release
.\build_msvc.ps1 user_literals_test release
```

#### En terminal MSYS2/Bash:
```bash
# Compilar y ejecutar un test específico
./msys_build.sh user_literals_test debug

# Compilar en modo release  
./msys_build.sh user_literals_test release
```

### Estructura de Compilación

- **Debug**: `/Od /Zi /MDd /DDEBUG`
- **Release**: `/O2 /MD /DNDEBUG`
- **Output**: `build/test/{debug|release}/test_name.exe`

## 🔧 Características Implementadas

### ✅ Funcionalidades Básicas
- [x] Constructores (default, copia, move, desde integrales)
- [x] Operadores aritméticos (`+`, `-`, `*`, `/`, `%`)
- [x] Operadores de comparación (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- [x] Operadores bit a bit (`&`, `|`, `^`, `<<`, `>>`, `~`)
- [x] Operadores de asignación (`+=`, `-=`, `*=`, etc.)
- [x] Operadores de incremento/decremento (`++`, `--`)

### ✅ Extensiones STL
- [x] `std::numeric_limits<uint128_t>`
- [x] `std::hash<uint128_t>` para contenedores unordered
- [x] Concepts personalizados (`uint128_concepts`)
- [x] Compatibilidad con `std::iota`

### ✅ Funcionalidades Avanzadas
- [x] Constructor desde cadenas: `uint128_t("12345")`
- [x] Método `from_string()` estático
- [x] Literales de usuario: `42_u128`, `"123456789"_U128`
- [x] Operadores simétricos (friend functions)
- [x] Operador de inserción para streams (`operator<<`)

### ✅ Casos de Uso Prácticos
- [x] Cálculos financieros de alta precisión
- [x] Operaciones criptográficas
- [x] Contadores de gran rango
- [x] IDs únicos de 128 bits
- [x] Compatibilidad con algoritmos STL

## 📊 Benchmarks

### Sistema de Benchmarks Completo

El proyecto incluye un **sistema avanzado de benchmarks** con las siguientes capacidades:

#### 🎯 Características
- ✅ **Múltiples compiladores**: GCC, Clang, MSVC, Intel (icpx/icx)
- ✅ **Comparación de tipos**:
  - `uint128_t` (nuestra implementación)
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
  - Boost + GMP backend
  - Boost + tommath backend

#### 📈 Ejecutar Benchmarks

```bash
# Benchmark básico (solo tipos nativos)
./scripts/build_benchmarks.bash gcc
./scripts/run_benchmarks.bash gcc

# Benchmark con todos los backends
./scripts/build_with_backends.bash gcc --all-backends
./build/benchmarks/benchmark_gcc

# Generar gráficos
python scripts/plot_benchmark_results.py

# Ver resultados
cat benchmark_results/benchmark_summary.md
ls benchmark_results/plots/
```

#### 📚 Documentación

- [BENCHMARK_IMPROVEMENTS.md](documentation/BENCHMARK_IMPROVEMENTS.md) - Guía completa de mejoras
- [BENCHMARK_GUIDE.md](documentation/BENCHMARK_GUIDE.md) - Manual de uso
- [BENCHMARK_SUMMARY.md](documentation/BENCHMARK_SUMMARY.md) - Resumen ejecutivo

#### 📊 Resultados

Los resultados se almacenan en `benchmark_results/`:

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
std::vector<uint128_t> vec = {3, 1, 4, 1, 5};
std::sort(vec.begin(), vec.end()); // ✅ Funciona

// Hash support para containers
std::unordered_set<uint128_t> set;
set.insert(uint128_t(123456789));
```

**Traits implementados:**
- ✅ `std::numeric_limits` - Límites y propiedades numéricas
- ✅ `std::is_integral` - Reconocido como tipo integral  
- ✅ `std::is_unsigned` - Reconocido como tipo sin signo
- ✅ `std::make_unsigned` - Transformación de tipo
- ✅ `std::common_type` - Tipos comunes con enteros estándar
- ✅ `std::hash` - Soporte para containers hash-based

Ver [TRAITS_SUMMARY.md](TRAITS_SUMMARY.md) para detalles completos.

## 🚀 Ejemplos de Uso

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

## 📝 Licencia

**Boost Software License 1.0**

Copyright (c) 2025 uint128_t Contributors

Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:

The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT.**

Ver [Boost Software License](https://www.boost.org/LICENSE_1_0.txt) para el texto completo de la licencia.