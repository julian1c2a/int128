# int128_traits.hpp - Pruebas y Benchmarks Extraídos

## Resumen

Este documento describe las pruebas y benchmarks extraídos para cada especialización de `int128_traits.hpp`. Se han creado tests individuales y benchmarks de rendimiento que se compilan con 4 compiladores diferentes: GCC, Clang, Intel OneAPI y MSVC.

## Archivos Creados

### Tests
- **tests/int128_traits_extracted_tests.cpp**: Contiene 19 pruebas individuales para verificar el correcto funcionamiento de cada especialización de type traits.

### Benchmarks
- **benchmarks/int128_traits_extracted_benchs.cpp**: Contiene 11 benchmarks que miden:
  - Tiempo de ejecución en nanosegundos
  - Ciclos de CPU usando RDTSC

### Scripts de Compilación
- **scripts/compile_int128_traits_extracted.sh**: Compila tests y benchmarks con los 4 compiladores (Bash)
- **scripts/compile_int128_traits_extracted.bat**: Compila tests y benchmarks con los 4 compiladores (Windows)

### Scripts de Ejecución
- **scripts/run_int128_traits_benchmarks.sh**: Ejecuta todos los tests y benchmarks, genera reportes consolidados (Bash)
- **scripts/run_int128_traits_benchmarks.bat**: Ejecuta todos los tests y benchmarks, genera reportes consolidados (Windows)

### Scripts Maestros
- **scripts/master_int128_traits_all.sh**: Ejecuta todo el proceso automáticamente (Bash)
- **scripts/master_int128_traits_all.bat**: Ejecuta todo el proceso automáticamente (Windows)

## Estructura de Directorios

```
build/
├── build_tests/
│   ├── gcc/release/          # Tests compilados con GCC
│   ├── clang/release/        # Tests compilados con Clang
│   ├── intel/release/        # Tests compilados con Intel
│   └── msvc/release/         # Tests compilados con MSVC
└── build_benchmarks/
    ├── gcc/release/          # Benchmarks compilados con GCC
    ├── clang/release/        # Benchmarks compilados con Clang
    ├── intel/release/        # Benchmarks compilados con Intel
    └── msvc/release/         # Benchmarks compilados con MSVC

benchmark_results/            # Resultados de las ejecuciones
├── tests_gcc_TIMESTAMP.txt
├── tests_clang_TIMESTAMP.txt
├── benchmarks_gcc_TIMESTAMP.txt
├── benchmarks_clang_TIMESTAMP.txt
├── summary_TIMESTAMP.csv
└── report_TIMESTAMP.txt
```

## Tests Incluidos

### Type Traits Fundamentales (4 tests)
1. `test_is_integral()` - Verifica que int128_t sea reconocido como integral
2. `test_is_arithmetic()` - Verifica que int128_t sea reconocido como aritmético
3. `test_is_unsigned()` - Verifica que int128_t NO sea unsigned
4. `test_is_signed()` - Verifica que int128_t sea signed

### Traits de Trivialidad (10 tests)
5. `test_is_trivially_copyable()` - Verifica copia trivial
6. `test_is_trivially_default_constructible()` - Constructor por defecto (false)
7. `test_is_trivially_copy_constructible()` - Constructor de copia trivial
8. `test_is_trivially_move_constructible()` - Constructor de movimiento trivial
9. `test_is_trivially_copy_assignable()` - Asignación por copia trivial
10. `test_is_trivially_move_assignable()` - Asignación por movimiento trivial
11. `test_is_trivially_destructible()` - Destructor trivial
12. `test_is_trivial()` - Tipo trivial (false)
13. `test_is_standard_layout()` - Layout estándar
14. `test_is_pod()` - Tipo POD

### Transformación de Tipos (2 tests)
15. `test_make_signed()` - Verifica make_signed devuelve int128_t
16. `test_make_unsigned()` - Verifica make_unsigned devuelve uint128_t

### Common Type (2 tests)
17. `test_common_type_standard()` - Common type con tipos estándar
18. `test_common_type_uint128()` - Common type con uint128_t

### Hash (1 test)
19. `test_hash()` - Verifica función hash

## Benchmarks Incluidos

### Type Traits (4 benchmarks)
1. `bench_is_integral()` - Rendimiento de is_integral
2. `bench_is_arithmetic()` - Rendimiento de is_arithmetic
3. `bench_is_signed()` - Rendimiento de is_signed
4. `bench_is_trivially_copyable()` - Rendimiento de is_trivially_copyable

### Hash (2 benchmarks)
5. `bench_hash_computation()` - Rendimiento del cálculo de hash
6. `bench_hash_varying_values()` - Hash con valores variables

### Operaciones de Copia/Movimiento (2 benchmarks)
7. `bench_copy_int128()` - Rendimiento de copia
8. `bench_move_int128()` - Rendimiento de movimiento

### Transformaciones de Tipo (2 benchmarks)
9. `bench_common_type_usage()` - Uso de common_type
10. `bench_make_unsigned_usage()` - Uso de make_unsigned

## Uso

### Compilación y Ejecución Completa (Recomendado)

**Bash:**
```bash
./scripts/master_int128_traits_all.sh
```

**Windows:**
```cmd
scripts\master_int128_traits_all.bat
```

### Solo Compilación

**Bash:**
```bash
./scripts/compile_int128_traits_extracted.sh
```

**Windows:**
```cmd
scripts\compile_int128_traits_extracted.bat
```

### Solo Ejecución

**Bash:**
```bash
./scripts/run_int128_traits_benchmarks.sh
```

**Windows:**
```cmd
scripts\run_int128_traits_benchmarks.bat
```

## Compiladores

### GCC (UCRT64)
- Ruta: `C:\msys64\ucrt64\bin\g++.exe`
- Flags: `-std=c++20 -O3 -march=native`

### Clang (CLANG64)
- Ruta: `C:\msys64\clang64\bin\clang++.exe`
- Flags: `-std=c++20 -O3 -march=native`

### Intel OneAPI
- Ruta: `C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\icx.exe`
- Flags: `-std=c++20 -O3`

### MSVC
- Ruta: `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe`
- Flags: `/std:c++20 /O2 /EHsc`

## Métricas de Benchmark

Cada benchmark mide dos métricas:

1. **Tiempo (nanosegundos)**: Usando `std::chrono::high_resolution_clock`
2. **Ciclos de CPU**: Usando la instrucción RDTSC (Read Time-Stamp Counter)

### Configuración
- Iteraciones: 10,000,000
- Warmup: 1,000 iteraciones
- Resultados promediados

## Resultados

Los resultados se guardan en `benchmark_results/` con timestamp:

- **tests_[compilador]_[timestamp].txt**: Salida de los tests
- **benchmarks_[compilador]_[timestamp].txt**: Salida de los benchmarks
- **summary_[timestamp].csv**: Resumen consolidado en formato CSV
- **report_[timestamp].txt**: Reporte completo en texto

### Formato CSV

```
Compilador,Benchmark,Tiempo_ns,Ciclos
GCC,is_integral,0.12345,3
Clang,hash_computation,5.67890,18
...
```

## Notas Técnicas

### Características Destacadas de int128_t

1. **Tipo Integral con Signo**: int128_t es reconocido como un tipo integral con signo
2. **Trivialmente Copiable**: Puede copiarse con memcpy
3. **Standard Layout**: Compatible con estructuras C
4. **POD Type**: Plain Old Data type
5. **Hash Optimizado**: Combina high y low con XOR y desplazamiento

### Diferencias con uint128_t

- **Signo**: int128_t es signed, uint128_t es unsigned
- **Common Type**: common_type<int128_t, uint128_t> = uint128_t
- **Rango**: int128_t: -2^127 a 2^127-1, uint128_t: 0 a 2^128-1

## Depuración

Si los tests o benchmarks fallan:

1. Verificar que los compiladores estén instalados y en el PATH
2. Revisar los logs de compilación en `/tmp/` (Bash) o stderr (Windows)
3. Examinar los archivos de resultados en `benchmark_results/`
4. Verificar que las rutas de los compiladores sean correctas en los scripts

## Referencias

- Archivo principal: `include/int128/int128_traits.hpp`
- Tipo base: `include/int128/int128_t.hpp`
- Estándar: C++20
- Documentación adicional: Ver comentarios en los archivos fuente
