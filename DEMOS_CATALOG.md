# 📚 Catálogo de Demos - int128 Library

> **Generado automáticamente** - No editar manualmente  
> Ejecuta `bash scripts/catalog_demos.bash` para actualizar

Este documento lista todas las demos disponibles organizadas por categoría.

## 📊 Estadísticas Generales

- **Total de demos**: 35
- **Total de líneas de código**: ~4878 LOC
- **Categorías**: 7
- **Última actualización**: 2025-12-27 12:35:08

---

## 🔧 General - Demos Ad-hoc

**Descripción**: Demostraciones experimentales y características específicas  
**Ubicación**: [`demos/general/`](demos/general/)  
**Total de demos**: 6

| Demo | Descripción | LOC | Características |
|------|-------------|-----|-----------------|
| [`analysis_summary.cpp`](demos/general/analysis_summary.cpp) | Sin descripción | 46 | iostreams |
| [`bit_analysis.cpp`](demos/general/bit_analysis.cpp) | Count bits manually | 37 | iostreams |
| [`demo_bytes_bitset.cpp`](demos/general/demo_bytes_bitset.cpp) | ============================================================================= | 181 | iostreams |
| [`demo_constexpr_cstr.cpp`](demos/general/demo_constexpr_cstr.cpp) | Ejemplo de uso en tiempo de compilación | 131 | iostreams, constexpr |
| [`demo_symmetry_complete.cpp`](demos/general/demo_symmetry_complete.cpp) | Suma simétrica | 102 | algorithm, iostreams |
| [`demo_symmetry_fixed.cpp`](demos/general/demo_symmetry_fixed.cpp) | Suma simétrica - usando conversión explícita para evitar ambigüedad | 119 | algorithm, iostreams |

### 🚀 Quick Start - general

```bash
# Compilar una demo específica
bash scripts/build_demo.bash general <demo_name> gcc release

# Compilar todas las demos de esta categoría
bash scripts/build_all_demos.bash general gcc release

# Ejecutar una demo
bash scripts/run_demo.bash general <demo_name>
```

---

## 🎓 Tutorials - Paso a Paso

**Descripción**: 13 tutoriales progresivos desde básico hasta avanzado  
**Ubicación**: [`demos/tutorials/`](demos/tutorials/)  
**Total de demos**: 16

| Demo | Descripción | LOC | Características |
|------|-------------|-----|-----------------|
| [`01_basic_operations.cpp`](demos/tutorials/01_basic_operations.cpp) | ============================================================ | 154 | iostreams |
| [`02_bitwise_operations.cpp`](demos/tutorials/02_bitwise_operations.cpp) | Helper para imprimir en binario (últimos 16 bits) | 139 | iostreams |
| [`03_comparisons.cpp`](demos/tutorials/03_comparisons.cpp) | ============================================================ | 136 | algorithm, iostreams |
| [`04_conversions.cpp`](demos/tutorials/04_conversions.cpp) | ============================================================ | 151 | iostreams |
| [`05_string_conversion.cpp`](demos/tutorials/05_string_conversion.cpp) | ============================================================ | 72 | iostreams |
| [`06_iostream.cpp`](demos/tutorials/06_iostream.cpp) | ============================================================ | 83 | iostreams |
| [`07_format.cpp`](demos/tutorials/07_format.cpp) | Usar los literales definidos por el usuario | 47 | iostreams |
| [`08_cmath_functions.cpp`](demos/tutorials/08_cmath_functions.cpp) | ============================================================ | 59 | iostreams |
| [`09_algorithms_stl.cpp`](demos/tutorials/09_algorithms_stl.cpp) | ============================================================ | 80 | algorithm, numeric, iostreams |
| [`10_numeric_functions.cpp`](demos/tutorials/10_numeric_functions.cpp) | ============================================================ | 52 | algorithm, iostreams |
| [`11_thread_safety.cpp`](demos/tutorials/11_thread_safety.cpp) | ============================================================ | 76 | thread_safety, iostreams |
| [`12_safe_operations.cpp`](demos/tutorials/12_safe_operations.cpp) | ============================================================ | 121 | iostreams |
| [`13_udl_literals.cpp`](demos/tutorials/13_udl_literals.cpp) | Importar los namespaces de literales | 91 | iostreams, constexpr |
| [`demo_practical_udl.cpp`](demos/tutorials/demo_practical_udl.cpp) | Simular claves y valores hash usando literales UDL | 105 | iostreams |
| [`test_simple.cpp`](demos/tutorials/test_simple.cpp) | Test 1: Crear números básicos | 23 | iostreams |
| [`test_uint128_only.cpp`](demos/tutorials/test_uint128_only.cpp) | Test 1: Crear números básicos | 21 | iostreams |

### 🚀 Quick Start - tutorials

```bash
# Compilar una demo específica
bash scripts/build_demo.bash tutorials <demo_name> gcc release

# Compilar todas las demos de esta categoría
bash scripts/build_all_demos.bash tutorials gcc release

# Ejecutar una demo
bash scripts/run_demo.bash tutorials <demo_name>
```

---

## 💼 Examples - Casos de Uso Reales

**Descripción**: Aplicaciones prácticas del mundo real  
**Ubicación**: [`demos/examples/`](demos/examples/)  
**Total de demos**: 9

| Demo | Descripción | LOC | Características |
|------|-------------|-----|-----------------|
| [`big_integer_calculator.cpp`](demos/examples/big_integer_calculator.cpp) | Cálculos matemáticos | 241 | iostreams |
| [`demo_int128_thread_safety.cpp`](demos/examples/demo_int128_thread_safety.cpp) | ========================= Demo 1: ThreadSafeInt128 (Mutex) ========================= | 257 | thread_safety, iostreams |
| [`demo_mathematical_library.cpp`](demos/examples/demo_mathematical_library.cpp) | Demostrar GCD y LCM | 137 | iostreams |
| [`example_thread_safety.cpp`](demos/examples/example_thread_safety.cpp) | ========================= Example 1: Global Counter ========================= | 178 | iostreams, constexpr |
| [`int128.cpp`](demos/examples/int128.cpp) | Prueba: Crear un entero de 128 bits | 14 | iostreams |
| [`ipv6_address.cpp`](demos/examples/ipv6_address.cpp) | Constructor desde uint128_t | 242 | algorithm, iostreams |
| [`mersenne_primes.cpp`](demos/examples/mersenne_primes.cpp) | Test de primalidad simple (para p pequeño) | 206 | iostreams |
| [`prime_factorization.cpp`](demos/examples/prime_factorization.cpp) | Verificar si un número es primo (test simple) | 222 | iostreams |
| [`uuid_generation.cpp`](demos/examples/uuid_generation.cpp) | Constructor desde uint128_t | 230 | algorithm, iostreams |

### 🚀 Quick Start - examples

```bash
# Compilar una demo específica
bash scripts/build_demo.bash examples <demo_name> gcc release

# Compilar todas las demos de esta categoría
bash scripts/build_all_demos.bash examples gcc release

# Ejecutar una demo
bash scripts/run_demo.bash examples <demo_name>
```

---

## 🌟 Showcase - Demos Impresionantes

**Descripción**: Presentaciones visuales e interactivas  
**Ubicación**: [`demos/showcase/`](demos/showcase/)  
**Total de demos**: 4

| Demo | Descripción | LOC | Características |
|------|-------------|-----|-----------------|
| [`main.cpp`](demos/showcase/main.cpp) | ANSI color codes para output atractivo | 320 | algorithm, iostreams |
| [`showcase_cryptography.cpp`](demos/showcase/showcase_cryptography.cpp) | Colores ANSI | 253 | iostreams |
| [`showcase_performance.cpp`](demos/showcase/showcase_performance.cpp) | Colores ANSI | 286 | algorithm, numeric, iostreams |
| [`showcase_scientific.cpp`](demos/showcase/showcase_scientific.cpp) | Colores ANSI | 266 | iostreams |

### 🚀 Quick Start - showcase

```bash
# Compilar una demo específica
bash scripts/build_demo.bash showcase <demo_name> gcc release

# Compilar todas las demos de esta categoría
bash scripts/build_all_demos.bash showcase gcc release

# Ejecutar una demo
bash scripts/run_demo.bash showcase <demo_name>
```

---

## 🛠️ Comandos Útiles

### Compilación

```bash
# Una demo específica
bash scripts/build_demo.bash <category> <demo_name> [compiler] [mode]

# Todas las demos de una categoría
bash scripts/build_all_demos.bash <category> [compiler] [mode]

# Todas las demos de todas las categorías
bash scripts/build_all_demos.bash all gcc release
```

### Ejecución

```bash
# Ejecutar una demo
bash scripts/run_demo.bash <category> <demo_name> [compiler] [mode] [args...]

# Ejemplo con argumentos
bash scripts/run_demo.bash examples uuid_generation gcc release 100
```

### Testing

```bash
# Verificar que todas compilan
bash scripts/test_demos.bash

# Verificar solo una categoría
bash scripts/test_demos.bash tutorials

# Verificar con compilador específico
bash scripts/test_demos.bash all clang release
```

### Regenerar Catálogo

```bash
# Actualizar este archivo
bash scripts/catalog_demos.bash
```

---

## 📖 Documentación Relacionada

- **README Principal**: [README.md](README.md)
- **Guía de Demos**: [demos/README.md](demos/README.md)
- **Guía de Compilación**: [PROMPT.md](PROMPT.md)
- **Estado del Proyecto**: [PROYECTO_COMPLETO.md](PROYECTO_COMPLETO.md)

---

## 🎯 Recomendaciones

### 🆕 Si eres nuevo:
1. Empieza con [tutorials/01_basic_operations.cpp](demos/tutorials/01_basic_operations.cpp)
2. Sigue los tutoriales en orden secuencial (01 → 13)
3. Experimenta con [examples/](demos/examples/) simples

### 🔧 Si buscas ejemplos prácticos:
1. Explora [examples/](demos/examples/) para casos reales
2. Revisa [showcase/](demos/showcase/) para ver posibilidades
3. Consulta [comparison/](demos/comparison/) para decidir entre librerías

### 🚀 Si necesitas optimización:
1. Analiza [performance/](demos/performance/) para entender benchmarks
2. Estudia algoritmos de división y multiplicación
3. Revisa efectos de caché y branch prediction

### 🔌 Si integras con otros sistemas:
1. Consulta [integration/](demos/integration/) para tu caso de uso
2. Verifica ejemplos de serialización (JSON, Protobuf)
3. Revisa integración con bases de datos

---

**📅 Última actualización**: Generado automáticamente el $(date '+%Y-%m-%d %H:%M:%S')

