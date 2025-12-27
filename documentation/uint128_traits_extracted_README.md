# uint128_traits.hpp - Tests y Benchmarks Extraídos

Documentación completa de tests y benchmarks individualizados para cada función/especialización de `uint128_traits.hpp`.

## 📋 Contenido

### Archivos Creados

#### Tests
- **`tests/uint128_traits_extracted_tests.cpp`** - Suite completa de tests extraídos
  - Tests individuales para cada trait
  - Validación de type traits fundamentales
  - Verificación de trivialidad y standard layout
  - Tests de common_type
  - Tests de std::hash
  - Tests de numeric_limits

#### Benchmarks
- **`benchs/uint128_traits_extracted_benchs.cpp`** - Suite completa de benchmarks
  - Mide tanto **tiempo (ns)** como **ciclos de reloj**
  - Benchmarks de verificación de traits (compile-time)
  - Benchmarks de operaciones de copia (runtime)
  - Benchmarks de hash computation
  - Benchmarks de inserción/búsqueda en unordered_map
  - Benchmarks de numeric_limits

#### Scripts de Compilación
- **`scripts/compile_uint128_traits_extracted.sh`** - Script Bash para MSYS2
- **`scripts/compile_uint128_traits_extracted.bat`** - Script Batch para Windows

Ambos compilan con **4 compiladores**:
1. **G++** (UCRT64) - `/c/msys64/ucrt64/bin/`
2. **Clang** (CLANG64) - `/c/msys64/clang64/bin/`
3. **Intel OneAPI** - `C:\Program Files (x86)\Intel\oneAPI\`
4. **MSVC** - `C:\Program Files\Microsoft Visual Studio\`

#### Scripts de Ejecución
- **`scripts/run_uint128_traits_benchmarks.sh`** - Script Bash completo
- **`scripts/run_uint128_traits_benchmarks.bat`** - Script Batch completo

Realizan:
1. Ejecución de tests de validación
2. Ejecución de benchmarks con todos los compiladores
3. Consolidación de resultados en CSV
4. Generación de reporte comparativo

## 🚀 Uso

### Compilación

#### Desde MSYS2 (Bash):
```bash
cd /c/msys64/ucrt64/home/julian/CppProjects/int128
chmod +x scripts/compile_uint128_traits_extracted.sh
./scripts/compile_uint128_traits_extracted.sh
```

#### Desde Windows (CMD/PowerShell):
```cmd
cd C:\msys64\ucrt64\home\julian\CppProjects\int128
scripts\compile_uint128_traits_extracted.bat
```

### Ejecución de Benchmarks

#### Desde MSYS2 (Bash):
```bash
chmod +x scripts/run_uint128_traits_benchmarks.sh
./scripts/run_uint128_traits_benchmarks.sh
```

#### Desde Windows (CMD/PowerShell):
```cmd
scripts\run_uint128_traits_benchmarks.bat
```

### Ejecución Manual de Tests

```bash
# GCC
./build/uint128_traits_extracted/uint128_traits_tests_gcc.exe

# Clang
./build/uint128_traits_extracted/uint128_traits_tests_clang.exe

# Intel
./build/uint128_traits_extracted/uint128_traits_tests_intel.exe

# MSVC
./build/uint128_traits_extracted/uint128_traits_tests_msvc.exe
```

### Ejecución Manual de Benchmarks

```bash
# GCC
./build/uint128_traits_extracted/uint128_traits_benchs_gcc.exe

# Clang
./build/uint128_traits_extracted/uint128_traits_benchs_clang.exe

# Intel
./build/uint128_traits_extracted/uint128_traits_benchs_intel.exe

# MSVC
./build/uint128_traits_extracted/uint128_traits_benchs_msvc.exe
```

## 📊 Tests Incluidos

### Type Traits Fundamentales
1. **`test_is_integral`** - Verifica `std::is_integral_v<uint128_t>`
2. **`test_is_arithmetic`** - Verifica `std::is_arithmetic_v<uint128_t>`
3. **`test_is_unsigned`** - Verifica `std::is_unsigned_v<uint128_t>`
4. **`test_is_signed`** - Verifica `!std::is_signed_v<uint128_t>`

### Traits de Trivialidad
5. **`test_is_trivially_copyable`** - Verifica copia trivial
6. **`test_is_trivially_copy_constructible`** - Verifica constructor de copia trivial
7. **`test_is_trivially_move_constructible`** - Verifica constructor de movimiento trivial
8. **`test_is_trivially_copy_assignable`** - Verifica asignación de copia trivial
9. **`test_is_trivially_move_assignable`** - Verifica asignación de movimiento trivial
10. **`test_is_trivially_destructible`** - Verifica destructor trivial
11. **`test_is_standard_layout`** - Verifica standard layout

### Transformación de Tipos
12. **`test_make_unsigned`** - Verifica `std::make_unsigned_t<uint128_t>`

### Common Type
13. **`test_common_type_uint64`** - Verifica `common_type<uint128_t, uint64_t>`
14. **`test_common_type_uint32`** - Verifica `common_type<uint128_t, uint32_t>`
15. **`test_common_type_int`** - Verifica `common_type<uint128_t, int>`

### Hash
16. **`test_hash`** - Verifica `std::hash<uint128_t>` y uso en unordered_map

### Numeric Limits
17. **`test_numeric_limits`** - Verifica especialización de `std::numeric_limits<uint128_t>`

## ⚡ Benchmarks Incluidos

### Traits (Compile-Time Overhead)
1. **`bench_is_integral`** - Overhead de verificación
2. **`bench_is_arithmetic`** - Overhead de verificación
3. **`bench_is_unsigned`** - Overhead de verificación
4. **`bench_is_trivially_copyable`** - Overhead de verificación
5. **`bench_common_type`** - Overhead de resolución de tipo

### Operaciones Runtime
6. **`bench_copy_uint128`** - Rendimiento de copia real (trivial copy)
7. **`bench_hash_computation`** - Cálculo de hash
8. **`bench_hash_map_insert`** - Inserción en unordered_map
9. **`bench_hash_map_lookup`** - Búsqueda en unordered_map

### Numeric Limits
10. **`bench_numeric_limits`** - Query de propiedades
11. **`bench_numeric_limits_minmax`** - Obtención de min/max

## 📈 Métricas

Cada benchmark proporciona:
- **Tiempo** - Nanosegundos por operación (ns/op)
- **Ciclos de Reloj** - Ciclos de CPU por operación (cycles/op)
- **Iteraciones** - Número de iteraciones realizadas

## 📂 Resultados

Los resultados se guardan en:
```
benchmark_results/uint128_traits_extracted/
├── tests_gcc_TIMESTAMP.txt
├── tests_clang_TIMESTAMP.txt
├── tests_intel_TIMESTAMP.txt
├── tests_msvc_TIMESTAMP.txt
├── benchmarks_gcc_TIMESTAMP.txt
├── benchmarks_clang_TIMESTAMP.txt
├── benchmarks_intel_TIMESTAMP.txt
├── benchmarks_msvc_TIMESTAMP.txt
├── summary_TIMESTAMP.csv         <- CSV consolidado
└── report_TIMESTAMP.txt          <- Reporte completo
```

### Formato CSV

```csv
Compiler,Benchmark,Time(ns),Cycles,Iterations
GCC,is_integral,0.123,3.45,10000000
Clang,is_integral,0.118,3.21,10000000
...
```

## 🔧 Opciones de Compilación

### G++ (UCRT64)
```bash
-std=c++20 -O3 -march=native -Wall -Wextra
```

### Clang (CLANG64)
```bash
-std=c++20 -O3 -march=native -Wall -Wextra
```

### Intel OneAPI
```bash
-std=c++20 -O3 -xHost -Wall
```

### MSVC
```cmd
/std:c++20 /O2 /EHsc /W4
```

## 📝 Notas

1. **RDTSC**: Los benchmarks usan el contador de ciclos del procesador (RDTSC/`__rdtsc()`)
2. **Precisión**: Los tiempos se miden con `std::chrono::high_resolution_clock`
3. **Compiladores**: Asegúrate de tener todos los compiladores instalados y configurados
4. **Rutas**: Las rutas están configuradas para la estructura del proyecto

## ✅ Validación

El script de ejecución (`run_uint128_traits_benchmarks`) primero ejecuta los tests de validación antes de los benchmarks. Si algún test falla, se muestra una advertencia pero continúa con los benchmarks.

## 🎯 Funciones Extraídas de uint128_traits.hpp

### Especializaciones de Type Traits en `namespace std`:
- `is_integral<uint128_t>`
- `is_arithmetic<uint128_t>`
- `is_unsigned<uint128_t>`
- `is_signed<uint128_t>`
- `is_trivially_copyable<uint128_t>`
- `is_trivially_copy_constructible<uint128_t>`
- `is_trivially_move_constructible<uint128_t>`
- `is_trivially_copy_assignable<uint128_t>`
- `is_trivially_move_assignable<uint128_t>`
- `is_trivially_destructible<uint128_t>`
- `is_standard_layout<uint128_t>`
- `make_unsigned<uint128_t>`
- `common_type<uint128_t, T>` (con múltiples tipos T)
- `hash<uint128_t>`

### Verificadas con numeric_limits:
- `numeric_limits<uint128_t>::is_specialized`
- `numeric_limits<uint128_t>::is_signed`
- `numeric_limits<uint128_t>::is_integer`
- `numeric_limits<uint128_t>::digits`
- `numeric_limits<uint128_t>::min()`
- `numeric_limits<uint128_t>::max()`

## 🔗 Referencias

- Header principal: `include/uint128/uint128_traits.hpp`
- Implementación base: `include/uint128/uint128_t.hpp`
- Documentación completa: `README.md`
