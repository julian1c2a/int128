# Demos - int128 Library

Esta carpeta contiene todas las demostraciones de la biblioteca int128, organizadas en 3 categorías.

## 📂 Estructura

```
demos/
├── tutorials/          # 13 tutoriales paso a paso
├── showcase/           # Demos impresionantes e interactivas
└── examples/           # Casos de uso del mundo real
```

## 🎓 Tutoriales (Para Aprender)

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

## 🌟 Showcase (Para Impresionar)

**Ubicación**: [`showcase/`](showcase/)

Demos visuales e impresionantes que muestran el poder de la biblioteca:

- **Demo Principal Interactiva**: 7 secciones con output colorido
- Números gigantes (factoriales, fibonacci)
- Magia bitwise avanzada
- Benchmarks en tiempo real
- Operaciones seguras con detección de errores

**Ideal para:**
- 🎤 Presentaciones técnicas
- 💼 Evaluación de la biblioteca
- 🎨 Marketing y promoción
- ⚡ Benchmarking

[Ver showcase →](showcase/README.md)

## 💼 Examples (Casos de Uso Real)

**Ubicación**: [`examples/`](examples/)

Implementaciones de casos de uso prácticos del mundo real:

- Thread safety en aplicaciones concurrentes
- Biblioteca matemática completa
- IPv6 address handling (planeado)
- UUID generation (planeado)
- Calculadora de enteros grandes (planeado)
- Prime factorization (planeado)

**Ideal para:**
- 🔧 Implementaciones reales
- 📖 Aprender patrones de uso
- 🚀 Punto de partida para proyectos

[Ver examples →](examples/README.md)

## 🚀 Inicio Rápido

### Opción 1: Tutoriales (Recomendado para nuevos usuarios)

```bash
cd demos/tutorials
g++ -std=c++20 -I../../include 01_basic_operations.cpp -o tutorial01
./tutorial01
```

### Opción 2: Showcase (Para ver algo impresionante)

```bash
cd demos/showcase
g++ -std=c++20 -I../../include main.cpp -o showcase
./showcase
```

### Opción 3: Examples (Para casos prácticos)

```bash
cd demos/examples
g++ -std=c++20 -I../../include -pthread demo_int128_thread_safety.cpp -o thread_demo
./thread_demo
```

## 📊 Mapa de Aprendizaje

```
┌─────────────────────────────────────────────────────────────┐
│  PRINCIPIANTE                                               │
│  └─> tutorials/01-05  (Básicos)                           │
│                                                             │
│  INTERMEDIO                                                 │
│  └─> tutorials/06-10  (Funciones avanzadas)               │
│                                                             │
│  AVANZADO                                                   │
│  └─> tutorials/11-13  (Thread safety, safe ops)           │
│  └─> examples/        (Casos reales)                      │
│                                                             │
│  PRESENTACIÓN                                               │
│  └─> showcase/        (Demostración completa)             │
└─────────────────────────────────────────────────────────────┘
```

## 🎯 Selección Rápida

| Quiero... | Ve a... |
|-----------|---------|
| Aprender desde cero | [`tutorials/01_basic_operations.cpp`](tutorials/01_basic_operations.cpp) |
| Ver algo impresionante | [`showcase/main.cpp`](showcase/main.cpp) |
| Implementar thread safety | [`examples/demo_int128_thread_safety.cpp`](examples/demo_int128_thread_safety.cpp) |
| Usar funciones matemáticas | [`examples/demo_mathematical_library.cpp`](examples/demo_mathematical_library.cpp) |
| Referencia rápida | [`tutorials/README.md`](tutorials/README.md) |

## 🛠️ Compilación

### Compilar todos con CMake:

```bash
mkdir build && cd build
cmake ..
cmake --build .

# Ejecutar
./demos/tutorials/01_basic_operations
./demos/showcase/showcase_main
./demos/examples/demo_int128_thread_safety
```

### Compilar individualmente:

```bash
# Cualquier demo
g++ -std=c++20 -I../include -pthread <archivo.cpp> -o output
./output
```

## 📚 Recursos Adicionales

- **Documentación HTML**: [`documentation/generated/html/index.html`](../documentation/generated/html/index.html)
- **Quick Start Guide**: [`documentation/doxygen/pages/quickstart.md`](../documentation/doxygen/pages/quickstart.md)
- **API Reference**: Generada por Doxygen
- **README Principal**: [`README.md`](../README.md)

## 💡 Tips

1. **Empieza con tutorials/** si eres nuevo
2. **Usa showcase/** para presentaciones
3. **Consulta examples/** para patrones reales
4. **Lee la documentación** para detalles de API
5. **Experimenta** modificando el código!

## 🤝 Contribuir

¿Tienes un ejemplo interesante? ¡Compártelo!

1. Crea tu demo en la carpeta apropiada
2. Sigue el estilo de código existente
3. Documenta bien (comentarios + output esperado)
4. Envía un Pull Request

## ⚙️ Requisitos

- **Compilador**: GCC 11+, Clang 14+, MSVC 19.30+, Intel ICX 2023+
- **Estándar**: C++20 o superior
- **Plataforma**: Windows, Linux, macOS
- **Opcional**: pthread (para demos de threading)

## 📄 Licencia

Boost Software License 1.0 - Ver [`LICENSE.txt`](../LICENSE.txt)

---

**¡Explora, aprende y diviértete! 🚀**
