# Demos - int128 Library

Esta carpeta contiene todas las demostraciones de la biblioteca int128, organizadas en **7 categorías**.

## 📂 Estructura

```
demos/
├── general/            # Demos generales y experimentales
├── tutorials/          # 13 tutoriales paso a paso (aprendizaje)
├── examples/           # Casos de uso del mundo real
├── showcase/           # Demos impresionantes e interactivas
├── comparison/         # Comparaciones con otras librerías
├── performance/        # Análisis profundo de rendimiento
└── integration/        # Integración con ecosistemas externos
```

## 🎯 Guía por Categoría

### 🔧 General - Demos Ad-hoc

**Ubicación**: [`general/`](general/)

Demostraciones experimentales, prototipo y características específicas que no encajan en otras categorías.

- Demo de simetría de operadores
- Conversiones bytes/bitset
- Funciones constexpr con C-strings
- Análisis de bits

**Ideal para:** Investigación, experimentos, prototipos

[Ver detalles →](general/README.md)

### 🎓 Tutorials - Para Aprender

**Ubicación**: [`tutorials/`](tutorials/)

13 tutoriales progresivos que te enseñan desde lo básico hasta características avanzadas:

1. Operaciones básicas (+, -, *, /, %)
2. Operaciones bitwise (&, |, ^, <<, >>)
3. Comparaciones y ordenamiento
4. Conversiones entre tipos
5. Conversión de strings
6. iostream (cin, cout)
7. Formateo avanzado
8. Funciones matemáticas
9. Algoritmos STL
10. Funciones numéricas C++20
11. Thread safety
12. Operaciones seguras
13. User-defined literals

**Ideal para:**

- 🆕 Principiantes que aprenden la biblioteca
- 📚 Referencia rápida de características
- 🎓 Enseñanza y educación

[Ver índice completo →](tutorials/README.md)

### 💼 Examples - Casos de Uso Reales

**Ubicación**: [`examples/`](examples/)

Aplicaciones prácticas que resuelven problemas del mundo real:

- **Gestión de direcciones IPv6**
- **Generación de UUIDs v4** (RFC 4122)
- **Calculadora de enteros grandes** (REPL interactivo)
- **Factorización de números primos**
- **Búsqueda de primos de Mersenne**
- **Demos de thread safety**

**Ideal para:**

- 💡 Inspiración para tus propios proyectos
- 🔨 Código base para aplicaciones reales
- 🎯 Ver best practices en contexto

[Ver catálogo →](examples/README.md)

### 🌟 Showcase - Para Impresionar

**Ubicación**: [`showcase/`](showcase/)

Demos visuales e impresionantes que muestran el poder de la biblioteca:

- **Demo Principal Interactiva**: 7 secciones con output colorido
- **Criptografía**: RSA toy, Miller-Rabin, búsqueda de primos
- **Científico**: Factoriales, Fibonacci, cálculo de π y e
- **Performance**: Benchmarks uint128 vs uint64 en vivo

**Ideal para:**

- 🎪 Presentaciones y demos
- 🎨 Mostrar capacidades visuales
- 🚀 Impresionar con números grandes

[Ver showcase →](showcase/README.md)

### ⚖️ Comparison - Comparar con Otras Librerías

**Ubicación**: [`comparison/`](comparison/)

Comparaciones lado a lado con otras implementaciones:

- **Boost.Multiprecision** (cpp_int, uint128_t)
- **Tipos nativos** (__uint128_t,__int128_t)
- **GMP** (GNU Multiple Precision)
- **Matriz de características** (type traits, concepts, STL)

**Ideal para:**

- 🤔 Decidir qué librería usar
- 📊 Entender trade-offs
- ⚡ Comparar rendimiento

[Ver comparaciones →](comparison/README.md)

### 🔬 Performance - Análisis Profundo

**Ubicación**: [`performance/`](performance/)

Deep-dive en rendimiento de operaciones específicas:

- **Algoritmos de división** (Knuth D, fast paths)
- **Estrategias de multiplicación** (intrínsecos, Karatsuba)
- **Conversiones string** (to_string, parsing)
- **Efectos de caché** (alignment, memory layout)
- **Branch prediction** (fast paths, mispredictions)

**Ideal para:**

- 🔍 Entender optimizaciones
- 📈 Análisis de bottlenecks
- 🎯 Microarquitectura insights

[Ver análisis →](performance/README.md)

### 🔌 Integration - Ecosistema Externo

**Ubicación**: [`integration/`](integration/)

Integración con bibliotecas y sistemas populares:

- **Serialización**: JSON (nlohmann), Protobuf, Cereal
- **Bases de datos**: SQLite, PostgreSQL
- **Networking**: Boost.Asio, gRPC
- **Message queues**: ZeroMQ, Kafka
- **Crypto**: OpenSSL, libsodium

**Ideal para:**

- 🌐 Aplicaciones del mundo real
- 🔗 Interoperabilidad
- 📦 Integrar en tu stack

[Ver integraciones →](integration/README.md)

---

## 🚀 Quick Start

### Compilar una Demo Específica

```bash
# Sintaxis genérica
bash scripts/build_demo.bash [category] [demo_name] [compiler] [mode]

# Ejemplos por categoría
bash scripts/build_demo.bash tutorials 01_basic_operations gcc release
bash scripts/build_demo.bash examples ipv6_address clang debug
bash scripts/build_demo.bash showcase main gcc release
bash scripts/build_demo.bash general bit_analysis gcc debug
bash scripts/build_demo.bash comparison boost_vs_int128 clang release
bash scripts/build_demo.bash performance division_algorithms intel release
bash scripts/build_demo.bash integration json_nlohmann gcc debug
```

### ⚠️ Compilación Manual - Flags Especiales para Thread Safety

Si compilas manualmente con `g++` o `clang++`, las demos con thread safety **requieren flags adicionales**:

```bash
# Demos normales (sin threading)
/ucrt64/bin/g++ -std=c++20 -Iinclude demos/tutorials/01_basic_operations.cpp -o build/demo

# ⚠️ Demos con thread safety REQUIEREN: -pthread -latomic
/ucrt64/bin/g++ -std=c++20 -Iinclude -pthread demos/examples/demo_int128_thread_safety.cpp -latomic -o build/demo
/ucrt64/bin/g++ -std=c++20 -Iinclude -pthread demos/examples/example_thread_safety.cpp -latomic -o build/demo
```

**Demos que requieren `-pthread -latomic`:**

- `demos/examples/demo_int128_thread_safety.cpp`
- `demos/examples/example_thread_safety.cpp`

**Por qué**: Las operaciones atómicas de 128 bits en x86_64 requieren libatomic y soporte de threading.

### Ejecutar Demos

```bash
# Sintaxis genérica
bash scripts/run_demo.bash [category] [demo_name] [args...]

# Ejemplos
bash scripts/run_demo.bash tutorials 01_basic_operations
bash scripts/run_demo.bash showcase main
bash scripts/run_demo.bash examples uuid_generation 100  # genera 100 UUIDs
```

### Compilar Todas las Demos

```bash
# Todas las categorías con todos los compiladores
bash scripts/build_all_demos.bash all gcc debug
bash scripts/build_all_demos.bash all clang release

# Solo una categoría
bash scripts/build_all_demos.bash tutorials gcc release
bash scripts/build_all_demos.bash examples all debug
```

### Atajos con Makefile

```bash
# Compilar demo específica
make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release

# Compilar categoría completa
make tutorials COMPILER=gcc MODE=release
make examples COMPILER=clang MODE=debug

# Compilar todas las demos
make build_all_demos COMPILER=all MODE=release

# Ejecutar demo
make run_demo CATEGORY=showcase DEMO=main

# Verificar que todas compilan
make test_demos

# Generar catálogo
make catalog_demos
```

---

## 📊 Catálogo Completo

Para ver la lista completa de todas las demos disponibles con descripciones detalladas:

👉 **[Ver DEMOS_CATALOG.md](../DEMOS_CATALOG.md)**

Este catálogo se genera automáticamente con:

```bash
bash scripts/catalog_demos.bash
# o
make catalog_demos
```

---

## 🧭 Navegación

| Categoría | Cantidad | Para... | README |
|-----------|----------|---------|--------|
| **General** | 6+ | Experimentación y características ad-hoc | [→](general/README.md) |
| **Tutorials** | 13 | Aprender paso a paso | [→](tutorials/README.md) |
| **Examples** | 9+ | Aplicaciones reales | [→](examples/README.md) |
| **Showcase** | 4 | Presentaciones impresionantes | [→](showcase/README.md) |
| **Comparison** | 0 (TBD) | Comparar con otras librerías | [→](comparison/README.md) |
| **Performance** | 0 (TBD) | Análisis profundo | [→](performance/README.md) |
| **Integration** | 0 (TBD) | Integrar con ecosistemas | [→](integration/README.md) |

---

## 🔍 Recomendaciones por Nivel

### 🆕 Principiante

1. Empieza con [tutorials/](tutorials/) en orden secuencial (01 → 13)
2. Experimenta modificando las [examples/](examples/) simples
3. Ejecuta el [showcase/main.cpp](showcase/main.cpp) para ver posibilidades

### 🔧 Usuario Intermedio

1. Estudia [examples/](examples/) de casos de uso avanzados
2. Lee [comparison/](comparison/) para entender trade-offs
3. Analiza [performance/](performance/) para optimización

### 🚀 Usuario Avanzado

1. Explora [integration/](integration/) para conectar con tu stack
2. Contribuye nuevas demos a las categorías
3. Extiende [general/](general/) con tus propios experimentos

---

## 📝 Contribuir Nuevas Demos

¿Tienes una demo útil? ¡Agrégala!

1. **Elige la categoría correcta** según la tabla arriba
2. **Crea tu archivo** `.cpp` en `demos/[category]/`
3. **Documenta** tu demo con comentarios explicativos
4. **Verifica** que compile con `bash scripts/test_demos.bash`
5. **Actualiza** el catálogo: `bash scripts/catalog_demos.bash`
6. **Haz un PR** con tu contribución

**Naming conventions:**

- Tutorials: `XX_descriptive_name.cpp` (XX = número secuencial)
- Otros: `descriptive_name.cpp` (snake_case)

---

## 🏗️ Arquitectura del Sistema

Las demos siguen la misma filosofía que `tests/` y `benchs/`:

```
demos/
├── [category]/                    # 7 categorías
│   ├── README.md                 # Guía de la categoría
│   ├── demo1.cpp
│   ├── demo2.cpp
│   └── ...
├── README.md (este archivo)      # Índice principal
└── ../scripts/                   # Scripts genéricos
    ├── build_demo.bash           # Compilar demo específica
    ├── run_demo.bash             # Ejecutar demo
    ├── build_all_demos.bash      # Compilar todas las demos
    ├── test_demos.bash           # Verificar compilación
    └── catalog_demos.bash        # Generar catálogo
```

**Variables del sistema:**

- `[category]` = general | tutorials | examples | showcase | comparison | performance | integration
- `[demo_name]` = nombre del archivo sin `.cpp`
- `[compiler]` = gcc | clang | intel | msvc | all
- `[mode]` = debug | release | all

---

## 📚 Recursos Adicionales

- **Documentación completa**: [../README.md](../README.md)
- **Guía de compilación**: [../PROMPT.md](../PROMPT.md)
- **Estado del proyecto**: [../PROYECTO_COMPLETO.md](../PROYECTO_COMPLETO.md)
- **Benchmark results**: [../RESULTADOS_BENCHMARK.md](../RESULTADOS_BENCHMARK.md)

---

**¡Feliz coding!** 🎉
