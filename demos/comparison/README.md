# Comparison Demos - int128 Library

Comparaciones lado a lado de `uint128_t`/`int128_t` con otras implementaciones y librerías.

## 🎯 Propósito

Demostrar las ventajas, trade-offs y diferencias de rendimiento de nuestra implementación comparada con:
- **Boost.Multiprecision** (`cpp_int`, `uint128_t`, etc.)
- **Tipos nativos** (`__uint128_t`, `__int128_t` en GCC/Clang)
- **GMP** (GNU Multiple Precision Library)
- **Otras librerías** de enteros de precisión arbitraria

## 📂 Demos Planificados

### 🔜 Por Implementar

- **`boost_vs_int128.cpp`** - Comparación completa con Boost.Multiprecision
  - Operaciones aritméticas
  - Conversiones string
  - Rendimiento
  - Integración STL

- **`native_uint128_vs_int128.cpp`** - Comparación con `__uint128_t` nativo
  - Solo disponible en GCC/Clang
  - Comparación de rendimiento
  - Diferencias de API

- **`gmp_vs_int128.cpp`** - Comparación con GMP
  - Precisión arbitraria vs fija
  - Trade-offs de rendimiento
  - Casos de uso óptimos

- **`feature_matrix.cpp`** - Matriz de características
  - Type traits
  - C++20 concepts
  - STL compatibility
  - Thread safety

## 🚀 Compilación

```bash
# Boost comparison (requiere Boost instalado)
g++ -std=c++20 -O2 -I include -I/path/to/boost \
    demos/comparison/boost_vs_int128.cpp \
    -o build/demos/comparison/boost_vs_int128

# GMP comparison (requiere GMP instalado)
g++ -std=c++20 -O2 -I include \
    demos/comparison/gmp_vs_int128.cpp \
    -o build/demos/comparison/gmp_vs_int128 \
    -lgmp
```

## 📊 Aspectos a Comparar

1. **Performance**
   - Operaciones aritméticas básicas
   - División/módulo
   - Conversiones string
   - Funciones matemáticas

2. **Características del Lenguaje**
   - Type traits (`std::is_integral`, etc.)
   - C++20 concepts
   - STL algorithms compatibility
   - Range-based operations

3. **API y Usabilidad**
   - Facilidad de uso
   - Documentación
   - Portabilidad
   - Dependencias

4. **Memory Footprint**
   - Tamaño del tipo
   - Overhead de operaciones
   - Stack vs heap allocation

## 📝 Contribuciones

Si tienes acceso a otras librerías o implementaciones, ¡contribuciones bienvenidas!
