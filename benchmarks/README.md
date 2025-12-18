# Benchmarks para uint128_t

## 🎯 Sistema Completo de Benchmarks

Este directorio contiene un **sistema completo de benchmarks** para `uint128_t` que compara el rendimiento con tipos built-in y Boost.Multiprecision.

## 📊 Archivo Principal: `uint128_extracted_benchmarks.cpp`

Sistema exhaustivo con:

- ✅ **Construcción**: default, desde uint64_t, desde dos uint64_t
- ✅ **Aritmética**: suma, resta, multiplicación, división, módulo
- ✅ **Bitwise**: AND, OR, XOR, NOT, shifts (left/right)
- ✅ **Comparaciones**: ==, !=, <, <=, >, >=
- ✅ **Conversiones**: to/from strings (decimal, hexadecimal)
- ✅ **Multi-tipo**: uint128_t, uint64_t, uint32_t, Boost (opcional)
- ✅ **Multi-compilador**: GCC, Clang, MSVC
- ✅ **Export**: CSV y JSON por compilador
- ✅ **Análisis**: Reportes consolidados y Markdown

## 🚀 Uso Rápido

```bash
# Desde la raíz del proyecto - Todo en un comando:
./benchmark.bash all

# Opciones individuales:
./benchmark.bash build  # Solo compilar
./benchmark.bash run    # Solo ejecutar
./benchmark.bash clean  # Limpiar
```

## 📁 Resultados Generados

Los resultados se guardan automáticamente en `benchmark_results/`:

- `uint128_benchmarks_GCC_*.csv/json` - Resultados GCC
- `uint128_benchmarks_Clang_*.csv/json` - Resultados Clang
- `uint128_benchmarks_MSVC_*.csv/json` - Resultados MSVC
- `consolidated_benchmarks_*.csv/json` - Todos los resultados
- `benchmark_report_*.md` - Reporte de análisis completo

## 📖 Documentación Completa

Ver [../documentation/BENCHMARK_GUIDE.md](../documentation/BENCHMARK_GUIDE.md) para guía completa de uso, interpretación de resultados y troubleshooting.

---

## 📝 Archivos Antiguos (Referencia Histórica)

Los siguientes archivos son benchmarks antiguos (ahora obsoletos por `uint128_extracted_benchmarks.cpp`):

### `benchmark_test.cpp`
- Benchmarks básicos de operaciones fundamentales
- Comparación de rendimiento entre diferentes implementaciones

### `normalized_benchmark.cpp`
- Benchmarks normalizados y estandarizados
- Comparaciones con tipos nativos (uint64_t, int64_t)

**Nota**: Se mantienen por referencia histórica. El nuevo sistema es más completo.