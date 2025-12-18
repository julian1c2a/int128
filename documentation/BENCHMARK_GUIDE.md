# uint128_t Comprehensive Benchmark Suite

Sistema completo de benchmarks para `uint128_t` con comparación contra tipos built-in y Boost.Multiprecision.

## Descripción

Este sistema de benchmarks evalúa el rendimiento de `uint128_t` en todas las operaciones principales:

- **Construcción**: Constructores por defecto, desde uint64_t, desde dos uint64_t
- **Aritmética**: Suma, resta, multiplicación, división, módulo
- **Bitwise**: AND, OR, XOR, NOT, shifts (izquierda/derecha)
- **Comparaciones**: ==, !=, <, <=, >, >=
- **Conversiones**: To/from strings (decimal, hexadecimal)

### Comparaciones

Los benchmarks comparan `uint128_t` con:

1. **Tipos built-in**: `uint64_t`, `uint32_t`
2. **Boost.Multiprecision**: `cpp_int` (puro C++, sin backends GMP/tommath)

### Compiladores

Los benchmarks se ejecutan con los tres compiladores principales:

- **GCC** (GNU Compiler Collection)
- **Clang** (LLVM)
- **MSVC** (Microsoft Visual C++)

## Estructura de Archivos

```
benchmarks/
├── uint128_extracted_benchmarks.cpp   # Archivo principal de benchmarks
└── README.md                           # Este archivo

scripts/
├── build_benchmarks.bash              # Compilación multi-compilador
├── run_benchmarks.bash                # Ejecución multi-compilador
├── aggregate_benchmark_results.py     # Agregación de resultados
└── benchmark.bash                     # Script wrapper (build+run)

benchmark_results/
├── uint128_benchmarks_GCC_*.csv       # Resultados GCC (CSV)
├── uint128_benchmarks_GCC_*.json      # Resultados GCC (JSON)
├── uint128_benchmarks_Clang_*.csv     # Resultados Clang (CSV)
├── uint128_benchmarks_Clang_*.json    # Resultados Clang (JSON)
├── uint128_benchmarks_MSVC_*.csv      # Resultados MSVC (CSV)
├── uint128_benchmarks_MSVC_*.json     # Resultados MSVC (JSON)
├── consolidated_benchmarks_*.csv      # Resultados consolidados (CSV)
├── consolidated_benchmarks_*.json     # Resultados consolidados (JSON)
└── benchmark_report_*.md              # Reporte de análisis (Markdown)
```

## Uso

### Opción 1: Script Wrapper (Recomendado)

```bash
# Compilar y ejecutar todo
./scripts/benchmark.bash all

# Solo compilar
./scripts/benchmark.bash build

# Solo ejecutar (si ya está compilado)
./scripts/benchmark.bash run

# Limpiar ejecutables
./scripts/benchmark.bash clean
```

### Opción 2: Scripts Individuales

```bash
# Compilar con todos los compiladores
./scripts/build_benchmarks.bash all

# Compilar con un compilador específico
./scripts/build_benchmarks.bash gcc
./scripts/build_benchmarks.bash clang
./scripts/build_benchmarks.bash msvc

# Ejecutar todos los benchmarks
./scripts/run_benchmarks.bash all

# Ejecutar benchmark específico
./scripts/run_benchmarks.bash gcc
./scripts/run_benchmarks.bash clang
./scripts/run_benchmarks.bash msvc

# Agregar resultados manualmente
python scripts/aggregate_benchmark_results.py
```

### Opción 3: Ejecutar Directamente

```bash
# Compilar
g++ -std=c++20 -O3 -march=native -I./include \
    benchmarks/uint128_extracted_benchmarks.cpp \
    -o build/uint128_benchmarks_gcc

# Ejecutar
./build/uint128_benchmarks_gcc
```

## Habilitar Boost (Opcional)

Para incluir comparaciones con Boost.Multiprecision:

1. Instalar Boost (si no está instalado):
   ```bash
   # MSYS2/UCRT64
   pacman -S mingw-w64-ucrt-x86_64-boost
   
   # Ubuntu/Debian
   sudo apt-get install libboost-all-dev
   
   # macOS
   brew install boost
   ```

2. Modificar `scripts/build_benchmarks.bash`:
   ```bash
   # Descomentar estas líneas:
   BOOST_FLAGS="-DHAVE_BOOST"
   # O si Boost está en ubicación custom:
   BOOST_FLAGS="-DHAVE_BOOST -I/path/to/boost/include"
   ```

3. Recompilar:
   ```bash
   ./scripts/benchmark.bash build
   ```

## Formatos de Salida

### CSV

Archivo CSV con todas las mediciones:

```csv
Operation,Type,Compiler,Optimization,Time_ns,Cycles,Iterations,Ops_per_sec,Timestamp
addition,uint128_t,GCC-13.2,O3,5.234,13.20,1000000,191057365,2024-01-15 10:30:45
addition,uint64_t,GCC-13.2,O3,0.521,1.30,1000000,1919386796,2024-01-15 10:30:45
...
```

**Columnas:**
- `Operation`: Nombre de la operación (ej: addition, multiplication)
- `Type`: Tipo de dato (uint128_t, uint64_t, boost_uint128)
- `Compiler`: Compilador y versión
- `Optimization`: Nivel de optimización
- `Time_ns`: Tiempo promedio por operación (nanosegundos)
- `Cycles`: Ciclos de CPU promedio por operación
- `Iterations`: Número de iteraciones ejecutadas
- `Ops_per_sec`: Operaciones por segundo
- `Timestamp`: Fecha y hora de ejecución

**Nota sobre Cycles**: Se utiliza la instrucción `RDTSC` (x86/x64) para medir ciclos de CPU. Esta métrica es independiente de la frecuencia del CPU y más precisa para comparaciones de rendimiento entre diferentes sistemas.

### JSON

Archivo JSON estructurado:

```json
{
  "metadata": {
    "generated_at": "2024-01-15T10:30:45.123456",
    "total_results": 150
  },
  "benchmark_results": [
    {
      "operation": "addition",
      "type": "uint128_t",
      "compiler": "GCC-13.2",
      "optimization": "O3",
      "time_ns": 5.234,
      "iterations": 1000000,
      "ops_per_sec": 191057365,
      "timestamp": "2024-01-15 10:30:45"
    },
    ...
  ]
}
```

### Markdown Report

El script `aggregate_benchmark_results.py` genera un reporte detallado en Markdown que incluye:

1. **Estadísticas por Compilador**: Número de resultados y operaciones únicas
2. **Comparación de Rendimiento**: Speedup relativo vs. uint64_t
3. **Resultados Detallados**: Por operación, tipo y compilador
4. **Top Performers**: Las 10 operaciones más rápidas
5. **Operaciones Lentas**: Las 10 operaciones más lentas
6. **Comparación de Compiladores**: Ganador por operación

Ver ejemplo en: `benchmark_results/benchmark_report_*.md`

## Interpretación de Resultados

### Time_ns (Tiempo en nanosegundos)

- **Menor es mejor**
- Típicamente:
  - Operaciones bitwise: < 10 ns
  - Aritmética simple: 5-50 ns
  - Multiplicación: 10-100 ns
  - División: 100-1000 ns
  - Conversiones de strings: 1000-10000 ns

### Ops_per_sec (Operaciones por segundo)

- **Mayor es mejor**
- Indica cuántas operaciones se pueden realizar en un segundo
- Útil para estimar throughput en aplicaciones reales

### Speedup Relativo

En el reporte consolidado, el speedup se calcula como:

```
Speedup = Time(uint64_t) / Time(Type)
```

- **Speedup > 1.0**: Más lento que uint64_t
- **Speedup = 1.0**: Igual que uint64_t
- **Speedup < 1.0**: Más rápido que uint64_t (inusual)

Ejemplo:
- `uint128_t addition`: Speedup 10.0x → 10 veces más lento que uint64_t
- `uint128_t bitwise_and`: Speedup 2.0x → 2 veces más lento que uint64_t

## Optimización de Benchmarks

Los benchmarks están optimizados con:

- **-O3**: Optimización máxima
- **-march=native**: Instrucciones específicas del CPU
- **Warm-up**: 10% de iteraciones antes de medir
- **volatile**: Evita que el compilador elimine código
- **Múltiples iteraciones**: 1,000,000 para operaciones rápidas, 100,000 para división

### Prevención de Optimizaciones

```cpp
volatile uint128_t result = a + b;
(void)result;
```

Esto asegura que:
1. El compilador no elimina la operación
2. Los valores no son promovidos a constantes en compile-time
3. Los resultados son realmente calculados

## Añadir Nuevos Benchmarks

Para añadir un nuevo benchmark:

1. Crear función en `uint128_extracted_benchmarks.cpp`:

```cpp
void benchmark_nueva_operacion()
{
    std::cout << "Benchmarking Nueva Operación..." << std::endl;
    
    uint128_t a(rng(), rng()), b(rng(), rng());
    
    benchmark_operation("nueva_op", "uint128_t", [&]() {
        volatile uint128_t result = /* tu operación */;
        (void)result;
    });
    
    // Comparación con uint64_t
    uint64_t a64 = rng(), b64 = rng();
    benchmark_operation("nueva_op", "uint64_t", [&]() {
        volatile uint64_t result = /* equivalente uint64_t */;
        (void)result;
    });
    
    // Boost (opcional)
#ifdef HAVE_BOOST
    boost_uint128 ab(rng()), bb(rng());
    benchmark_operation("nueva_op", "boost_uint128", [&]() {
        volatile boost_uint128 result = /* equivalente boost */;
        (void)result;
    });
#endif
}
```

2. Llamar en `main()`:

```cpp
int main()
{
    // ... benchmarks existentes ...
    benchmark_nueva_operacion();
    // ...
}
```

3. Recompilar y ejecutar:

```bash
./scripts/benchmark.bash all
```

## Troubleshooting

### Error: "cl.exe not found"

**Solución**: Activar MSVC environment:

```bash
# El script debería hacerlo automáticamente, pero si falla:
source vcvarsall.py
./scripts/build_benchmarks.bash msvc
```

### Error: "Boost headers not found"

**Solución**: Instalar Boost o deshabilitar comparaciones:

```bash
# Opción 1: Instalar Boost
pacman -S mingw-w64-ucrt-x86_64-boost

# Opción 2: Compilar sin Boost (automático por defecto)
./scripts/build_benchmarks.bash all
```

### Resultados Inconsistentes

**Causas posibles**:
- CPU throttling (temperatura)
- Procesos en background
- Power saving mode

**Solución**:
- Cerrar aplicaciones innecesarias
- Ejecutar múltiples veces y promediar
- Deshabilitar power saving

```bash
# Ejecutar 3 veces para estabilidad
for i in {1..3}; do
    ./scripts/run_benchmarks.bash all
done
```

### Benchmarks Muy Lentos

**Posible causa**: Compilación sin optimización

**Verificación**:
```bash
# Los scripts usan -O3 automáticamente
# Verificar que aparezca "Optimization: O3" en resultados
```

## Análisis Avanzado

### Comparar Compiladores

```bash
# Ver qué compilador es más rápido por operación
grep "addition,uint128_t" benchmark_results/*.csv | sort -t, -k5 -n
```

### Operaciones Críticas

Para identificar operaciones críticas en tu aplicación:

1. Ejecutar benchmarks completos
2. Revisar `benchmark_report_*.md`
3. Identificar operaciones lentas (> 100 ns)
4. Optimizar esas operaciones específicamente

### Regresiones de Rendimiento

Comparar resultados entre versiones:

```bash
# Guardar baseline
cp benchmark_results/consolidated_benchmarks_*.csv baseline.csv

# Después de cambios
./scripts/benchmark.bash all
python scripts/compare_benchmarks.py baseline.csv benchmark_results/consolidated_benchmarks_*.csv
```

*(El script `compare_benchmarks.py` es futuro trabajo)*

## Performance Tips

### Mejores Prácticas

1. **Bitwise >> Aritmética**: Usa operaciones bitwise cuando sea posible
2. **Evitar División**: La división es ~10-100x más lenta que multiplicación
3. **Inline pequeño**: Operaciones pequeñas se benefician de inline
4. **Alineación**: uint128_t usa 16 bytes, alinear a 16 bytes ayuda

### Código Optimizado

```cpp
// Rápido: Bitwise
result = a & mask;        // ~5 ns

// Medio: Aritmética simple
result = a + b;           // ~10 ns
result = a * b;           // ~50 ns

// Lento: División
result = a / b;           // ~500 ns

// Muy lento: Strings
str = a.to_string();      // ~5000 ns
```

## Referencias

- [uint128_t Implementation](../include/uint128/uint128_t.hpp)
- [Test Suite](../tests/uint128_extracted_tests.cpp)
- [Boost.Multiprecision](https://www.boost.org/doc/libs/release/libs/multiprecision/)
- [Google Benchmark](https://github.com/google/benchmark) (alternativa avanzada)

## Contribuir

Para añadir nuevos benchmarks o mejorar los existentes:

1. Seguir el patrón establecido en `uint128_extracted_benchmarks.cpp`
2. Incluir comparaciones con uint64_t
3. Documentar nuevas operaciones en este README
4. Probar con los tres compiladores
5. Actualizar scripts si es necesario

## Licencia

Mismo que el proyecto principal (ver [LICENSE.txt](../LICENSE.txt))

---

**Última actualización**: 2024-01-15  
**Versión**: 1.0.0
