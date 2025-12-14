# Benchmarks

Este directorio contiene pruebas de rendimiento y benchmarks para los tipos `uint128_t` e `int128_t`.

## Archivos

### `benchmark_test.cpp`
- Benchmarks básicos de operaciones fundamentales
- Comparación de rendimiento entre diferentes implementaciones
- Medición de tiempos de ejecución

### `normalized_benchmark.cpp`
- Benchmarks normalizados y estandarizados
- Comparaciones con tipos nativos (uint64_t, int64_t)
- Métricas de rendimiento detalladas

## Compilación

```bash
# Compilar benchmark individual
g++ -I ../include -std=c++20 -O2 -DNDEBUG benchmark_test.cpp -o benchmark_test.exe

# Compilar todos los benchmarks
g++ -I ../include -std=c++20 -O2 -DNDEBUG *.cpp -o all_benchmarks.exe
```

## Uso

Los benchmarks están diseñados para:
- ✅ Verificar que las optimizaciones (`trivially_copyable` + `standard_layout`) funcionan
- ✅ Comparar rendimiento con tipos nativos
- ✅ Identificar posibles cuellos de botella
- ✅ Validar que el balance óptimo se traduce en mejor rendimiento real