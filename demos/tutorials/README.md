# Tutoriales de int128

Esta carpeta contiene 13 tutoriales paso a paso para aprender a usar `uint128_t` e `int128_t`.

## 📚 Índice de Tutoriales

| # | Nombre | Descripción | Conceptos clave |
|---|--------|-------------|-----------------|
| 01 | [basic_operations.cpp](01_basic_operations.cpp) | Operaciones aritméticas fundamentales | +, -, *, /, %, ++, --, +=, -=, overflow |
| 02 | [bitwise_operations.cpp](02_bitwise_operations.cpp) | Operaciones a nivel de bits | &, \|, ^, ~, <<, >>, máscaras, flags |
| 03 | [comparisons.cpp](03_comparisons.cpp) | Comparaciones y ordenamiento | ==, !=, <, >, <=, >=, sort, find |
| 04 | [conversions.cpp](04_conversions.cpp) | Conversiones entre tipos | static_cast, narrowing, high/low words |
| 05 | [string_conversion.cpp](05_string_conversion.cpp) | Conversión desde/hacia strings | iostream, hex, oct, dec, parsing |
| 06 | [iostream.cpp](06_iostream.cpp) | Entrada/salida con streams | cin, cout, manipuladores, archivos |
| 07 | [format.cpp](07_format.cpp) | Formateo avanzado | UDL, hex, oct, width, padding |
| 08 | [cmath_functions.cpp](08_cmath_functions.cpp) | Funciones matemáticas | abs, gcd, lcm, sqrt, pow |
| 09 | [algorithms_stl.cpp](09_algorithms_stl.cpp) | Algoritmos STL | sort, find, accumulate, transform |
| 10 | [numeric_functions.cpp](10_numeric_functions.cpp) | Funciones numéricas C++20 | midpoint, clamp, min, max |
| 11 | [thread_safety.cpp](11_thread_safety.cpp) | Seguridad en multithreading | mutex, atomic, race conditions |
| 12 | [safe_operations.cpp](12_safe_operations.cpp) | Operaciones con detección de errores | overflow, underflow, std::optional |
| 13 | [udl_literals.cpp](13_udl_literals.cpp) | User-Defined Literals | _u128, _i128, constexpr |

## 🚀 Cómo usar estos tutoriales

### Orden recomendado

1. **Principiantes**: Empieza por 01 y sigue en orden hasta 07
2. **Intermedios**: Empieza en 08 si ya conoces las operaciones básicas
3. **Avanzados**: Ve directamente a 11-13 para multithreading y operaciones seguras

### Compilar un tutorial

#### Con CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
./demos/tutorials/01_basic_operations
```

#### Con g++ directamente:
```bash
g++ -std=c++20 -I../../include 01_basic_operations.cpp -o 01_basic_operations
./01_basic_operations
```

#### Con MSVC:
```powershell
cl /std:c++20 /EHsc /I..\..\include 01_basic_operations.cpp
01_basic_operations.exe
```

### Experimentar

Cada tutorial incluye:
- ✅ **Código comentado línea por línea**
- ✅ **Output esperado** en comentarios
- ✅ **Ejemplos prácticos**
- ✅ **Warnings sobre pitfalls comunes**

**Tip**: Modifica los valores y recompila para ver cómo cambian los resultados.

## 📖 Conceptos por tutorial

### Tutorial 01: Operaciones Básicas
- Creación de números de 128 bits
- Suma, resta, multiplicación, división, módulo
- Operadores compuestos (+=, -=, *=, /=)
- Incremento/decremento (++, --)
- ⚠️ **Pitfall**: Unsigned underflow

### Tutorial 02: Operaciones Bitwise
- AND (&), OR (|), XOR (^), NOT (~)
- Desplazamientos (<<, >>)
- Manipulación de bits individuales
- Sistema de flags
- Swap sin variable temporal

### Tutorial 03: Comparaciones
- Operadores de comparación
- Comparación con tipos nativos
- Signed vs unsigned
- Ordenamiento con std::sort
- Búsqueda binaria

### Tutorial 04: Conversiones
- Desde/hacia tipos nativos
- Narrowing y pérdida de datos
- Verificación de rangos
- Extracción de high/low words
- Conversión segura

### Tutorial 05: String Conversion
- uint128_t/int128_t a string
- Parsing desde string
- Hexadecimal, octal, decimal
- Números muy grandes
- Formato personalizado

### Tutorial 06: iostream
- cin y cout
- Manipuladores (hex, oct, showbase)
- Ancho y relleno
- Tablas formateadas
- stringstream

### Tutorial 07: Formateo Avanzado
- User-defined literals (_u128, _i128)
- Conversiones a múltiples bases
- Width y padding
- Mayúsculas/minúsculas

### Tutorial 08: Funciones Matemáticas
- abs (valor absoluto)
- gcd (máximo común divisor)
- lcm (mínimo común múltiplo)
- sqrt (raíz cuadrada)
- pow (potencia)

### Tutorial 09: Algoritmos STL
- std::sort (ordenamiento)
- std::find (búsqueda)
- std::accumulate (suma total)
- std::transform (transformación)
- std::copy_if (filtrado)

### Tutorial 10: Funciones Numéricas C++20
- std::midpoint (punto medio seguro)
- std::clamp (acotar valores)
- std::min/max de múltiples valores
- std::exchange

### Tutorial 11: Thread Safety
- ⚠️ Race conditions
- Protección con std::mutex
- std::atomic<uint128_t>
- Lecturas/escrituras thread-safe
- Best practices

### Tutorial 12: Safe Operations
- Detección de overflow
- Detección de underflow
- División por cero
- Operaciones con std::optional
- Conversiones seguras

### Tutorial 13: User-Defined Literals
- Sintaxis _u128 y _i128
- Uso en constexpr
- Ventajas vs sintaxis tradicional
- Best practices
- Ejemplos prácticos

## 🎯 Próximos pasos

Después de completar estos tutoriales:

1. **Explora las demos showcase**: `demos/showcase/`
   - Demos impresionantes de todas las capacidades
   - Comparaciones de rendimiento
   - Aplicaciones criptográficas

2. **Revisa los ejemplos de uso real**: `demos/examples/`
   - IPv6 addresses
   - UUID generation
   - Calculadora de enteros grandes
   - Factorización de primos

3. **Lee la documentación completa**: `documentation/generated/html/index.html`
   - API reference completa
   - Guías adicionales
   - Benchmarks

## 💡 Tips generales

- **Siempre especifica el tipo**: Usa `_u128` o `_i128` para claridad
- **Verifica rangos**: Especialmente al convertir a tipos menores
- **Cuidado con unsigned**: No pueden ser negativos, causan wrap around
- **Usa safe operations**: Para código crítico, detecta overflow/underflow
- **Thread-safe**: Protege con mutex o usa atomic cuando sea necesario

## 📚 Referencias

- [README principal](../../README.md)
- [Documentación Doxygen](../../documentation/generated/html/index.html)
- [Quick Start Guide](../../documentation/doxygen/pages/quickstart.md)

## 🐛 Encontraste un bug?

Si encuentras algún problema en los tutoriales:
1. Verifica que estás usando C++20
2. Asegúrate de incluir `int128.hpp` correctamente
3. Compila con optimizaciones (-O2 o -O3) para mejor rendimiento

---

**¡Feliz aprendizaje! 🚀**
