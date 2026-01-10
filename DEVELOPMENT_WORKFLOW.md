# Workflow de Desarrollo - int128 Library

> **DOCUMENTO NORMATIVO** - Seguir estos pasos en CADA ciclo de desarrollo
>
> ⏰ **Última actualización:** 2026-01-10

---

## 📋 Tabla de Contenidos

1. [Ciclo de Desarrollo Obligatorio](#ciclo-de-desarrollo-obligatorio)
2. [Niveles de Optimización](#niveles-de-optimización)
3. [Sanitizers](#sanitizers)
4. [Análisis Estático](#análisis-estático)
5. [Benchmarks Comparativos](#benchmarks-comparativos)
6. [Demos](#demos)
7. [Comandos Rápidos](#comandos-rápidos)
8. [WSL/Linux](#wsllinux)

---

## 🔄 Ciclo de Desarrollo Obligatorio

### Regla Principal

**NUNCA** saltarse pasos. El ciclo completo es:

```
┌─────────────────────────────────────────────────────────────────────┐
│  1. MODIFICAR CÓDIGO                                                │
│     └── Aplicar las 14 reglas de coding standards                   │
├─────────────────────────────────────────────────────────────────────┤
│  2. COMPILAR CON TODOS LOS COMPILADORES                             │
│     └── GCC, Clang, Intel, MSVC (los 4 obligatorios)                │
├─────────────────────────────────────────────────────────────────────┤
│  3. EJECUTAR TESTS                                                  │
│     └── Debug Y Release, todos los compiladores                     │
├─────────────────────────────────────────────────────────────────────┤
│  4. SANITIZERS (si hay cambios en memoria/threading)                │
│     └── ASan, UBSan, TSan según corresponda                         │
├─────────────────────────────────────────────────────────────────────┤
│  5. ANÁLISIS ESTÁTICO (periódicamente)                              │
│     └── cppcheck, clang-tidy                                        │
├─────────────────────────────────────────────────────────────────────┤
│  6. BENCHMARKS (si hay cambios de rendimiento)                      │
│     └── Comparar con __int128 y Boost                               │
├─────────────────────────────────────────────────────────────────────┤
│  7. DEMOS (verificar que ejemplos siguen funcionando)               │
│     └── Compilar con GCC (compilador por defecto para demos)        │
└─────────────────────────────────────────────────────────────────────┘
```

### Paso 1: Modificar Código

Aplicar SIEMPRE las 14 reglas de coding standards:

1. ✅ Const correctness (todo lo que pueda ser const, debe serlo)
2. ✅ Inicializar variables al declarar con `{}`
3. ✅ `constexpr`/`consteval` en todas las funciones posibles
4. ✅ No excepciones → usar `std::optional` o `std::pair<error, T>`
5. ✅ Siempre llaves `{}` en if/else/for/while
6. ✅ Template en línea propia, luego firma
7. ✅ K&R brace style (llave en misma línea)
8. ✅ `noexcept` en todas las funciones que no lanzan
9. ✅ Naming: `snake_case_t` para tipos, `_tt` para templates, `_ec_t` para enums
10. ✅ Comportamiento builtin-like (como enteros nativos)
11. ✅ `explicit` en constructores y conversiones
12. ✅ Preferir inmutabilidad
13. ✅ Casts explícitos C++ (`static_cast`, no `(type)`)
14. ✅ `std::` explícito (excepto tipos aritméticos importados con `using`)

### Paso 2: Compilar

```bash
# OBLIGATORIO: Los 4 compiladores
python make.py build uint128 <feature> tests all all
# O con Makefile:
make build_tests TYPE=uint128 FEATURE=<feature> COMPILER=all MODE=all
```

**Compiladores obligatorios:**

- GCC (15.x en MSYS2)
- Clang (19.x en MSYS2)
- Intel ICX (oneAPI)
- MSVC (Visual Studio 2022)

### Paso 3: Ejecutar Tests

```bash
# OBLIGATORIO: Todos los compiladores, debug Y release
python make.py check uint128 <feature> all all
# O con Makefile:
make check TYPE=uint128 FEATURE=<feature> COMPILER=all MODE=all
```

**Criterio de éxito:** 100% tests PASS en los 4 compiladores × 2 modos = 8 combinaciones mínimo

### Paso 4: Sanitizers

**Cuándo usar:**

- Cambios en gestión de memoria → ASan
- Cambios en aritmética/shifts → UBSan
- Cambios en threading → TSan
- Antes de release → ASan + UBSan combinados

```bash
# AddressSanitizer (memoria)
python make.py sanitize uint128 <feature> asan gcc tests

# UndefinedBehaviorSanitizer (UB)
python make.py sanitize uint128 <feature> ubsan gcc tests

# ThreadSanitizer (data races)
python make.py sanitize uint128 thread_safety tsan gcc tests

# Combinado ASan + UBSan
python make.py sanitize uint128 <feature> all gcc tests
```

**Makefile:**

```bash
make sanitize TYPE=uint128 FEATURE=bits COMPILER=gcc SANITIZER=asan
make sanitize TYPE=uint128 FEATURE=thread_safety COMPILER=gcc SANITIZER=tsan
```

### Paso 5: Análisis Estático

**Cuándo usar:**

- Antes de cada commit importante
- Después de refactorizaciones grandes
- Periódicamente (1x por semana mínimo)

```bash
# cppcheck (recomendado, rápido)
python make.py analyze cppcheck headers
python make.py analyze cppcheck tests

# clang-tidy (más exhaustivo, más lento)
python make.py analyze clang-tidy headers

# Todos los analizadores en todo el código
python make.py analyze all all
```

**Makefile:**

```bash
make static-analysis TOOL=cppcheck TARGET=headers
make static-analysis TOOL=clang-tidy TARGET=tests
make static-analysis TOOL=all TARGET=all
```

### Paso 6: Benchmarks

**Cuándo usar:**

- Cambios en operadores aritméticos
- Cambios en división/módulo
- Optimizaciones de rendimiento
- Antes de release

```bash
# Benchmark comparativo completo
python make.py compare gcc release-O3 100000

# Con máxima optimización
python make.py compare gcc release-Ofast 1000000

# Con Clang
python make.py compare clang release-O3 100000
```

**Makefile:**

```bash
make benchmark-compare COMPILER=gcc MODE=release-O3
make benchmark-compare COMPILER=clang MODE=release-Ofast ITERATIONS=1000000
```

**Tipos comparados:**

- Builtin: `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `int8_t`, `int16_t`, `int32_t`, `int64_t`
- Este proyecto: `nstd::uint128_t`, `nstd::int128_t`
- Compiler extension: `unsigned __int128`, `__int128` (GCC/Clang)
- Boost: `boost::multiprecision::uint128_t`

### Paso 7: Demos

**Compilador por defecto para demos: GCC**

```bash
# Compilar un demo específico
python make.py build demos tutorials 01_basic_operations gcc release

# Ejecutar demo
python make.py demo tutorials 01_basic_operations gcc release

# Verificar que todos los demos compilan
python make.py check demos all gcc release
```

**Makefile:**

```bash
make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release
make run_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release
make demo CATEGORY=tutorials DEMO=01_basic_operations  # build + run
make check_demos CATEGORY=all COMPILER=gcc MODE=release
```

---

## ⚡ Niveles de Optimización

### Modos Disponibles

| Modo | GCC/Clang | MSVC | Intel | Uso |
|------|-----------|------|-------|-----|
| `debug` | `-O0 -g` | `/Od /Zi` | `-O0 -g` | Desarrollo, debugging |
| `release` | `-O2` | `/O2` | `-O2` | Producción estándar |
| `release-O1` | `-O1` | `/O1` | `-O1` | Balance tamaño/velocidad |
| `release-O2` | `-O2` | `/O2` | `-O2` | Optimización estándar |
| `release-O3` | `-O3 + extras` | `/Ox` | `-O3` | Máxima velocidad |
| `release-Ofast` | `-Ofast + extras` | `/Ox /fp:fast` | `-Ofast` | Agresivo (puede romper IEEE) |

### Flags Agresivos (release-O3 y release-Ofast)

**GCC:**

```bash
-O3 -fexpensive-optimizations -funroll-loops -ftree-vectorize -march=native
```

**release-Ofast adicional:**

```bash
-Ofast -ffast-math -fexpensive-optimizations -march=native
```

### Cuándo usar cada modo

- **debug**: Desarrollo diario, debugging
- **release / release-O2**: Tests de CI/CD, validación
- **release-O3**: Benchmarks de rendimiento
- **release-Ofast**: Benchmarks máximos (¡cuidado con IEEE compliance!)

---

## 🔬 Sanitizers

### Tabla de Compatibilidad

| Sanitizer | GCC | Clang | Intel | MSVC | Detecta |
|-----------|-----|-------|-------|------|---------|
| `asan` | ✅ | ✅ | ✅ | ✅ | Buffer overflow, use-after-free, leaks |
| `ubsan` | ✅ | ✅ | ✅ | ❌ | Signed overflow, null deref, alignment |
| `tsan` | ✅ | ✅ | ❌ | ❌ | Data races, deadlocks |
| `msan` | ❌ | ✅* | ❌ | ❌ | Uninitialized memory |
| `all` | ✅ | ✅ | ✅ | ❌ | ASan + UBSan combinados |

*MSan solo en Clang Linux

### Variables de Entorno Útiles

```bash
# ASan
export ASAN_OPTIONS=detect_leaks=1:abort_on_error=1:print_stats=1

# UBSan
export UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1

# TSan
export TSAN_OPTIONS=second_deadlock_stack=1:history_size=7
```

---

## 🔍 Análisis Estático

### Herramientas

| Herramienta | Instalación MSYS2 | Velocidad | Exhaustividad |
|-------------|-------------------|-----------|---------------|
| **cppcheck** | `pacman -S cppcheck` | ⚡ Rápido | ⭐⭐⭐ |
| **clang-tidy** | Incluido con Clang | 🐢 Lento | ⭐⭐⭐⭐⭐ |
| **Infer** | Manual | 🐢 Lento | ⭐⭐⭐⭐ |
| **PVS-Studio** | Comercial (gratis OSS) | ⚡ Medio | ⭐⭐⭐⭐⭐ |

### Targets Disponibles

- `headers` - Solo archivos .hpp en include/
- `tests` - Archivos de tests
- `benchs` - Archivos de benchmarks
- `demos` - Archivos de demos
- `all` - Todo el código

### Reportes

Los reportes se guardan en:

```
build/static_analysis/
├── cppcheck_headers_YYYYMMDD_HHMMSS.txt
├── cppcheck_headers_YYYYMMDD_HHMMSS.xml
├── clang_tidy_tests_YYYYMMDD_HHMMSS.txt
└── infer_<target>_YYYYMMDD_HHMMSS/
```

---

## 📊 Benchmarks Comparativos

### Operaciones Medidas

1. **Addition (+)** - Suma
2. **Multiplication (*)** - Multiplicación
3. **Division (/)** - División
4. **Bitwise XOR (^)** - XOR bit a bit
5. **Comparison (<)** - Comparación

### Tipos Comparados

```cpp
// Builtin (baseline)
uint8_t, uint16_t, uint32_t, uint64_t
int8_t, int16_t, int32_t, int64_t

// Este proyecto
nstd::uint128_t
nstd::int128_t

// Compiler extension (GCC/Clang only)
unsigned __int128
__int128

// Boost.Multiprecision
boost::multiprecision::uint128_t
```

### Requisitos

```bash
# Instalar Boost para comparaciones completas
pacman -S mingw-w64-ucrt-x86_64-boost
```

### Formato de Resultados

```
+---------------------------+---------------+------------------+
| Tipo                      | Tiempo (ns)   | Ops/segundo      |
+---------------------------+---------------+------------------+
| uint64_t                  |          0.50 |       2.00e+09   |
| nstd::uint128_t           |          1.20 |       8.33e+08   |
| unsigned __int128         |          0.80 |       1.25e+09   |
| boost::uint128_t          |          2.50 |       4.00e+08   |
+---------------------------+---------------+------------------+
```

---

## 🎮 Demos

### Categorías

| Categoría | Descripción | Cantidad |
|-----------|-------------|----------|
| `tutorials` | Tutoriales paso a paso | 16 |
| `examples` | Casos de uso reales | 9 |
| `showcase` | Demostraciones avanzadas | 4 |
| `general` | Conceptos generales | 6 |
| `comparison` | Comparaciones | - |
| `performance` | Análisis de rendimiento | - |
| `integration` | Integración con otras libs | - |

### Compilador por Defecto

**GCC** es el compilador por defecto para demos:

- Mejor soporte de características C++20
- Mensajes de error más claros
- `__int128` disponible para comparaciones

### Comandos

```bash
# Listar demos disponibles
python make.py list

# Compilar y ejecutar
python make.py demo tutorials 01_basic_operations

# Solo compilar
python make.py build demos tutorials 01_basic_operations gcc release

# Verificar todos los demos
python make.py check demos all gcc release
```

---

## 🚀 Comandos Rápidos

### Ciclo Completo para una Feature

```bash
# 1. Compilar (todos los compiladores)
python make.py build uint128 bits tests all all

# 2. Tests (todos los compiladores)
python make.py check uint128 bits all all

# 3. Sanitizers (GCC)
python make.py sanitize uint128 bits all gcc tests

# 4. Análisis estático
python make.py analyze cppcheck headers

# 5. Benchmarks (si aplica)
python make.py build uint128 bits benchs gcc release-O3
python make.py run uint128 bits gcc release-O3

# 6. Benchmark comparativo
python make.py compare gcc release-O3

# 7. Demos (verificar)
python make.py check demos all gcc release
```

### Pipeline Rápido (desarrollo diario)

```bash
# Solo GCC debug (rápido)
python make.py build uint128 <feature> tests gcc debug
python make.py check uint128 <feature> gcc debug
```

### Pipeline Completo (antes de commit)

```bash
# Todos los compiladores
python make.py build uint128 <feature> tests all all
python make.py check uint128 <feature> all all
python make.py sanitize uint128 <feature> all gcc tests
python make.py analyze cppcheck headers
```

### Pipeline Release

```bash
# Todos + benchmarks + análisis completo
python make.py build uint128 <feature> tests all all
python make.py check uint128 <feature> all all
python make.py sanitize uint128 <feature> all gcc tests
python make.py sanitize uint128 <feature> all clang tests
python make.py analyze all all
python make.py compare gcc release-O3
python make.py compare clang release-O3
python make.py check demos all gcc release
```

---

## 🐧 WSL/Linux

### Objetivo

Validar el proyecto en un entorno Linux real usando WSL (Windows Subsystem for Linux)
con múltiples versiones de compiladores:

- **GCC**: 13, 14, 15
- **Clang**: 18, 19, 20, 21
- **Intel**: icpx (oneAPI)

### Instalación de Compiladores en WSL Ubuntu

#### 1. Usando el Script Automático

```bash
# Desde MSYS2 o PowerShell
python scripts/run_wsl_tests.py setup

# O directamente en WSL
bash scripts/wsl_build_system.bash setup
```

#### 2. Instalación Manual

```bash
# Actualizar sistema
sudo apt update && sudo apt upgrade -y

# Herramientas base
sudo apt install -y build-essential cmake ninja-build git curl wget

# GCC 13, 14, 15
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install -y g++-13 g++-14 g++-15

# Clang 18-21 (desde repositorio LLVM)
wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc
UBUNTU_CODENAME=$(lsb_release -cs)
echo "deb http://apt.llvm.org/$UBUNTU_CODENAME/ llvm-toolchain-$UBUNTU_CODENAME main" | sudo tee /etc/apt/sources.list.d/llvm.list
for ver in 18 19 20 21; do
    echo "deb http://apt.llvm.org/$UBUNTU_CODENAME/ llvm-toolchain-$UBUNTU_CODENAME-$ver main" | sudo tee -a /etc/apt/sources.list.d/llvm.list
done
sudo apt update
sudo apt install -y clang-18 clang-19 clang-20 clang-21

# Intel oneAPI (opcional)
wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
sudo apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
echo "deb https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list
sudo apt update
sudo apt install -y intel-oneapi-compiler-dpcpp-cpp

# Cargar entorno Intel (añadir a ~/.bashrc)
source /opt/intel/oneapi/setvars.sh

# Herramientas de análisis
sudo apt install -y cppcheck libboost-all-dev
```

### Verificar Compiladores

```bash
# Desde MSYS2/PowerShell
python scripts/run_wsl_tests.py detect

# O en WSL directamente
bash scripts/wsl_build_system.bash detect
```

**Salida esperada:**

```
GCC:
  ✓ g++-13: g++ (Ubuntu 13.2.0-4ubuntu3) 13.2.0
  ✓ g++-14: g++ (Ubuntu 14.2.0-1ubuntu1) 14.2.0
  ✓ g++-15: g++ (Ubuntu 15.0.0-0ubuntu1) 15.0.0

Clang:
  ✓ clang++-18: Ubuntu clang version 18.1.8
  ✓ clang++-19: Ubuntu clang version 19.1.0
  ✓ clang++-20: Ubuntu clang version 20.0.0
  ✓ clang++-21: Ubuntu clang version 21.0.0

Intel:
  ✓ icpx: Intel(R) oneAPI DPC++/C++ Compiler 2024.1.0
```

### Ejecutar Tests en WSL

#### Sintaxis Simplificada (Fase 1.66)

El template unificado `int128_base_t<S>` elimina la necesidad del parámetro `TYPE`:

```bash
# ANTES (obsoleto):
bash scripts/wsl_build_and_test.bash int128 tt all all

# AHORA (simplificado):
bash scripts/wsl_build_and_test.bash tt all all
bash scripts/wsl_build_and_test.bash <feature> [compiler] [mode]
```

#### Desde Windows (PowerShell/CMD/MSYS2)

```bash
# Pipeline completo con todos los compiladores
python scripts/run_wsl_tests.py tt all all

# Solo GCC 15 en release
python scripts/run_wsl_tests.py bits gcc-15 release

# Clang 21 en todos los modos
python scripts/run_wsl_tests.py cmath clang-21 all
```

#### Desde WSL Directamente

```bash
# Bash script maestro (recomendado)
bash scripts/wsl_build_and_test.bash tt all all

# Con compilador específico
bash scripts/wsl_build_and_test.bash bits gcc-15 release
bash scripts/wsl_build_and_test.bash cmath clang-21 debug

# Python script maestro (alternativa)
python scripts/wsl_build_and_test.py tt all all
python scripts/wsl_build_and_test.py numeric gcc-14 release
```

#### Features Disponibles

```
tt, t, algorithm, bits, cmath, concepts, format, iostreams,
limits, numeric, ranges, safe, thread_safety, traits
```

#### Compiladores Disponibles

```
gcc-13, gcc-14, gcc-15, clang-18, clang-19, clang-20, clang-21, icpx, all
```

#### Modos Disponibles

```
debug, release, all
```

### Estructura de Scripts WSL (Fase 1.66)

```
scripts/
├── wsl/                           # Scripts modulares por compilador
│   ├── common.bash                # Funciones compartidas Bash
│   ├── common.py                  # Funciones compartidas Python
│   ├── build_gcc13.bash           # GCC 13
│   ├── build_gcc14.bash           # GCC 14
│   ├── build_gcc15.bash           # GCC 15
│   ├── build_clang18.bash         # Clang 18
│   ├── build_clang19.bash         # Clang 19
│   ├── build_clang20.bash         # Clang 20
│   ├── build_clang21.bash         # Clang 21
│   └── build_icpx.bash            # Intel ICPX
├── wsl_build_and_test.bash        # Script maestro Bash
├── wsl_build_and_test.py          # Script maestro Python
└── run_wsl_tests.py               # Puente Windows→WSL
```

### Estructura de Builds WSL

```
build/
└── wsl_build_tests/           # Tests compilados en WSL
    ├── gcc-13/
    │   ├── debug/
    │   └── release/
    ├── gcc-14/
    ├── gcc-15/
    ├── clang-18/
    ├── clang-19/
    ├── clang-20/
    ├── clang-21/
    └── icpx/
```

### Diferencias MSYS2 vs WSL

| Aspecto | MSYS2 (Windows) | WSL (Linux) |
|---------|-----------------|-------------|
| **GCC** | ucrt64/bin/g++ | g++-13, g++-14, g++-15 |
| **Clang** | clang64/bin/clang++ | clang++-18 a clang++-21 |
| **Intel** | icx (Windows) | icpx (Linux) |
| **Binarios** | .exe | Sin extensión |
| **ABI** | Windows | Linux (incompatibles) |
| **Paths** | /c/... | /mnt/c/... |

### Notas Importantes

1. **Los binarios son incompatibles**: Un .exe de Windows no funciona en Linux y viceversa.
   Los scripts limpian `build/` antes de compilar en WSL.

2. **Cargar entorno Intel**: Siempre ejecutar `source /opt/intel/oneapi/setvars.sh`
   antes de usar `icpx`.

3. **Finales de línea**: El script `run_wsl_tests.py` convierte automáticamente
   CRLF → LF para evitar errores `$'\r': command not found`.

4. **Sincronización**: WSL accede al mismo código fuente via `/mnt/c/...`,
   los cambios se reflejan instantáneamente.

---

## 📁 Estructura de Archivos Generados

```
build/
├── build_tests/                    # Ejecutables de tests
│   ├── gcc/
│   │   ├── debug/
│   │   └── release/
│   ├── clang/
│   ├── intel/
│   └── msvc/
├── build_benchs/                   # Ejecutables de benchmarks
├── build_demos/                    # Ejecutables de demos
├── build_tests_sanitized/          # Tests con sanitizers
│   ├── gcc/
│   │   ├── asan/
│   │   ├── ubsan/
│   │   └── tsan/
│   └── clang/
├── benchmark_comparison/           # Resultados de comparaciones
│   ├── gcc/release-O3/
│   └── clang/release-Ofast/
└── static_analysis/                # Reportes de análisis estático
    ├── cppcheck_*.txt
    ├── cppcheck_*.xml
    └── clang_tidy_*.txt
```

---

## ✅ Checklist Pre-Commit

- [ ] Código sigue las 14 reglas de coding standards
- [ ] Compila con GCC (debug + release)
- [ ] Compila con Clang (debug + release)
- [ ] Compila con Intel (debug + release)
- [ ] Compila con MSVC (debug + release)
- [ ] 100% tests PASS en todos los compiladores
- [ ] ASan sin errores
- [ ] UBSan sin errores
- [ ] TSan sin errores (si hay threading)
- [ ] cppcheck sin errores críticos
- [ ] Demos compilan correctamente
- [ ] CHANGELOG.md actualizado
- [ ] Documentación actualizada (si hay cambios de API)

---

**Documento mantenido por:** int128 Project  
**Versión:** 1.0  
**Fecha:** 2026-01-10
