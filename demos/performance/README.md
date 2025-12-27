# Performance Demos - int128 Library

Análisis profundo de rendimiento de características y algoritmos específicos.

## 🎯 Propósito

Explorar en detalle el rendimiento de operaciones específicas, optimizaciones y comportamiento bajo diferentes condiciones.

## 📂 Demos Planificados

### 🔜 Por Implementar

#### Algoritmos Específicos

- **`division_algorithms.cpp`** - Comparación de estrategias de división
  - Algoritmo D de Knuth vs alternativas
  - Fast paths (potencias de 2, 10)
  - División por valores pequeños
  - Impact de branch prediction

- **`multiplication_strategies.cpp`** - Estrategias de multiplicación
  - Intrínsecos (mul128) vs manual
  - Karatsuba para valores grandes
  - Optimizaciones de compilador

#### Conversiones

- **`string_conversion_perf.cpp`** - Análisis de conversiones string
  - to_string() en diferentes bases
  - from_string() parsing
  - C-string vs std::string
  - Buffer management overhead

- **`type_conversions.cpp`** - Conversiones entre tipos
  - to<T>() para diferentes tipos
  - Cast operators
  - Implicit vs explicit
  - Float/double conversions accuracy

#### Efectos de Microarquitectura

- **`cache_effects.cpp`** - Impacto de caché
  - Memory layout
  - Alignment effects
  - Cache-friendly patterns
  - False sharing en arrays

- **`branch_prediction.cpp`** - Branch prediction analysis
  - Predictable vs unpredictable branches
  - Fast paths effectiveness
  - Cost of mispredictions

#### Funciones Matemáticas

- **`sqrt_performance.cpp`** - Performance de sqrt()
  - Newton-Raphson iterations
  - Initial guess quality
  - Convergence speed
  - Comparison con uint64_t

- **`gcd_lcm_analysis.cpp`** - GCD/LCM deep-dive
  - Binary GCD algorithm
  - Different input patterns
  - Worst/best cases
  - Extended Euclidean (Bézout)

#### Operaciones Bitwise

- **`bitwise_operations.cpp`** - Operaciones bit a bit
  - Shift operations
  - Count leading/trailing zeros
  - Popcount strategies
  - Bit rotation

## 📊 Métricas Medidas

1. **Tiempo de Ejecución**
   - Nanosegundos por operación
   - Throughput (ops/sec)
   - Latency analysis

2. **Ciclos de CPU**
   - RDTSC measurements
   - Cycles per operation
   - IPC (Instructions per cycle)

3. **Comparaciones**
   - vs uint64_t (baseline)
   - vs __uint128_t nativo
   - vs Boost.Multiprecision
   - vs operaciones sin optimizar

4. **Profiling**
   - Hotspots identification
   - Call graphs
   - Cache misses
   - Branch mispredictions

## 🚀 Compilación

```bash
# Con optimizaciones completas
g++ -std=c++20 -O3 -march=native -I include \
    demos/performance/division_algorithms.cpp \
    -o build/demos/performance/division_algorithms

# Con profiling
g++ -std=c++20 -O3 -march=native -g -I include \
    demos/performance/cache_effects.cpp \
    -o build/demos/performance/cache_effects

# Con diferentes niveles de optimización
for opt in O0 O1 O2 O3; do
    g++ -std=c++20 -$opt -I include \
        demos/performance/string_conversion_perf.cpp \
        -o build/demos/performance/string_perf_$opt
done
```

## 🔬 Metodología

### Benchmarking Best Practices
- Múltiples iteraciones (warmup + medición)
- Outliers detection y filtrado
- Statistical significance (mean, median, stddev)
- Prevent compiler optimizations away (volatile, DoNotOptimize)

### Inputs Representativos
- Small values (< 2^64)
- Medium values (2^64 - 2^96)
- Large values (> 2^96)
- Edge cases (0, 1, max)
- Random distributions

### Environment Control
- CPU governor (performance mode)
- Isolated core si es posible
- Disabled turbo boost para consistency
- Background processes minimizados

## 📈 Visualización de Resultados

Los demos pueden generar:
- Tablas de resultados (stdout)
- CSV para análisis externo
- Gráficos ASCII para quick comparison
- Datos para plotting con Python/gnuplot

## 📝 Contribuciones

Sugerencias para nuevos análisis de performance son bienvenidas, especialmente:
- Casos de uso específicos de aplicaciones reales
- Patrones de acceso interesantes
- Hardware diferente (ARM, AMD vs Intel)
