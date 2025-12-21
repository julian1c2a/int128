# Guía Rápida: uint128_traits Tests y Benchmarks

## 🚀 Inicio Rápido (3 pasos)

### Paso 1: Ejecutar Todo Automáticamente

El método más simple es usar el script maestro:

```bash
# Desde MSYS2/Git Bash
cd /c/msys64/ucrt64/home/julian/CppProjects/int128
./scripts/master_uint128_traits_all.sh
```

```cmd
REM Desde Windows CMD
cd C:\msys64\ucrt64\home\julian\CppProjects\int128
scripts\master_uint128_traits_all.bat
```

Esto hará automáticamente:
1. ✅ Compilar con G++, Clang, Intel, MSVC
2. ✅ Ejecutar todos los tests
3. ✅ Ejecutar todos los benchmarks
4. ✅ Consolidar resultados
5. ✅ Generar reportes

### Paso 2: Ver Resultados

Los resultados están en:
```
benchmark_results/uint128_traits_extracted/
```

**Archivos importantes:**
- `summary_[timestamp].csv` - Todos los datos en CSV
- `report_[timestamp].txt` - Reporte completo
- `benchmarks_gcc_[timestamp].txt` - Resultados de GCC
- `benchmarks_clang_[timestamp].txt` - Resultados de Clang
- `benchmarks_intel_[timestamp].txt` - Resultados de Intel
- `benchmarks_msvc_[timestamp].txt` - Resultados de MSVC

### Paso 3: Analizar

Ver el CSV consolidado:
```bash
cat benchmark_results/uint128_traits_extracted/summary_*.csv | column -t -s,
```

Ver el reporte:
```bash
cat benchmark_results/uint128_traits_extracted/report_*.txt | less
```

## 📊 Ejemplo de Salida

### Tests
```
====================================================
  uint128_traits.hpp - Tests Extraídos Individuales
====================================================

=== TEST: is_integral ===
  std::is_integral_v<uint128_t> = 1
  ✅ PASS

=== TEST: is_arithmetic ===
  std::is_arithmetic_v<uint128_t> = 1
  ✅ PASS

...

====================================================
  RESUMEN: 18/18 tests pasados
====================================================
```

### Benchmarks
```
============================================================
  uint128_traits.hpp - Benchmarks Extraídos Individuales
============================================================

=== is_integral ===
  Iterations: 10000000
  Time:       0.123 ns/op
  Cycles:     3.45 cycles/op

=== hash_computation ===
  Iterations: 1000000
  Time:       12.34 ns/op
  Cycles:     45.67 cycles/op

...

============================================================
                    RESUMEN CSV
============================================================
Benchmark,Time(ns),Cycles,Iterations
is_integral,0.123,3.45,10000000
is_arithmetic,0.118,3.21,10000000
...
```

### CSV Consolidado
```csv
Compiler,Benchmark,Time(ns),Cycles,Iterations
GCC,is_integral,0.123,3.45,10000000
GCC,is_arithmetic,0.118,3.21,10000000
GCC,hash_computation,12.34,45.67,1000000
Clang,is_integral,0.115,3.12,10000000
Clang,is_arithmetic,0.112,3.05,10000000
Clang,hash_computation,11.89,43.21,1000000
Intel,is_integral,0.110,3.01,10000000
Intel,is_arithmetic,0.108,2.95,10000000
Intel,hash_computation,11.23,40.87,1000000
MSVC,is_integral,0.125,3.50,10000000
MSVC,is_arithmetic,0.122,3.42,10000000
MSVC,hash_computation,12.78,46.54,1000000
```

## 🔧 Opciones Avanzadas

### Solo Compilar

```bash
./scripts/compile_uint128_traits_extracted.sh
```

### Solo Ejecutar (sin compilar)

```bash
./scripts/run_uint128_traits_benchmarks.sh
```

### Ejecutar un solo compilador

```bash
# Solo tests con GCC
./build/uint128_traits_extracted/uint128_traits_tests_gcc.exe

# Solo benchmarks con GCC
./build/uint128_traits_extracted/uint128_traits_benchs_gcc.exe
```

### Guardar resultados con nombre personalizado

```bash
./build/uint128_traits_extracted/uint128_traits_benchs_gcc.exe > mi_resultado.txt
```

## 📈 Análisis de Resultados

### Comparar tiempos entre compiladores

```bash
# Extraer solo tiempos de un benchmark específico
grep "hash_computation" benchmark_results/uint128_traits_extracted/summary_*.csv
```

### Encontrar el compilador más rápido

```bash
# Ordenar por tiempo (columna 3)
tail -n +2 benchmark_results/uint128_traits_extracted/summary_*.csv | sort -t, -k3 -n | head -n 5
```

### Gráfico simple en terminal

```bash
# Usar gnuplot o similar
gnuplot -e "set datafile separator ','; 
            set xlabel 'Benchmark'; 
            set ylabel 'Time (ns)'; 
            plot 'summary_*.csv' using 3:xtic(2) with boxes title 'Time'"
```

## 🐛 Solución de Problemas

### Error: compilador no encontrado

Verifica las rutas en los scripts:
- G++: `/c/msys64/ucrt64/bin/g++.exe`
- Clang: `/c/msys64/clang64/bin/clang++.exe`
- Intel: `C:\Program Files (x86)\Intel\oneAPI\`
- MSVC: `C:\Program Files\Microsoft Visual Studio\`

### Error: permiso denegado

```bash
chmod +x scripts/*.sh
```

### Error en tests

Revisa los logs:
```bash
cat benchmark_results/uint128_traits_extracted/tests_gcc_*.txt
```

### Benchmarks muy lentos

Reduce las iteraciones en el archivo:
```cpp
// En uint128_traits_extracted_benchs.cpp
const size_t ITERATIONS = 1'000'000;  // Reducir si es muy lento
```

## 📝 Personalización

### Añadir un nuevo test

1. Edita `tests/uint128_traits_extracted_tests.cpp`
2. Añade tu función de test:
```cpp
bool test_mi_nuevo_test()
{
    std::cout << "\n=== TEST: mi_nuevo_test ===\n";
    // Tu código aquí
    std::cout << "  ✅ PASS\n";
    return true;
}
```
3. Añádelo al main:
```cpp
RUN_TEST(test_mi_nuevo_test);
```
4. Recompila y ejecuta

### Añadir un nuevo benchmark

1. Edita `benchmarks/uint128_traits_extracted_benchs.cpp`
2. Añade tu función de benchmark:
```cpp
BenchmarkResult bench_mi_operacion()
{
    const size_t ITERATIONS = 1'000'000;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    uint64_t start_cycles = RDTSC();
    
    // Tu código a medir aquí
    for (size_t i = 0; i < ITERATIONS; ++i) {
        // operación a medir
    }
    
    uint64_t end_cycles = RDTSC();
    auto end_time = std::chrono::high_resolution_clock::now();
    
    double elapsed_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
    uint64_t cycles = end_cycles - start_cycles;
    
    return {"mi_operacion", elapsed_ns / ITERATIONS, cycles / ITERATIONS, ITERATIONS};
}
```
3. Añádelo al main
4. Recompila y ejecuta

## 🎯 Próximos Pasos

1. ✅ Ejecutar el script maestro
2. ✅ Revisar resultados
3. ✅ Analizar CSV consolidado
4. ✅ Identificar el compilador más rápido para cada operación
5. ✅ Optimizar si es necesario

## 📚 Más Información

- [README completo](documentation/uint128_traits_extracted_README.md)
- [Resumen de implementación](UINT128_TRAITS_EXTRACTED_SUMMARY.md)
- [API Reference](documentation/API.md)

---

**¿Listo?** Ejecuta:
```bash
./scripts/master_uint128_traits_all.sh
```

**¡Y disfruta de los resultados!** 🚀
