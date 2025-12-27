# Showcase Demos - int128 Library

Esta carpeta contiene demos **impresionantes** que muestran el poder completo de la biblioteca int128.

## 🌟 Demo Principal

### [main.cpp](main.cpp) - Showcase Interactiva Completa

La demo principal es una presentación interactiva con 7 secciones:

1. **Números Gigantes** 🚀
   - Factorial de 34 (39 dígitos!)
   - Fibonacci hasta F(185)
   - Potencias de 2 hasta 2^127

2. **Magia Bitwise** ✨
   - Popcount (contar bits activados)
   - Encontrar bit más alto (MSB)
   - Swap sin variable temporal

3. **Funciones Matemáticas** 📐
   - GCD de números gigantes
   - LCM
   - Raíz cuadrada

4. **Algoritmos STL** 📚
   - Sort de números de 128 bits
   - Accumulate
   - Transform y filter

5. **Operaciones Seguras** 🛡️
   - Detección de overflow
   - División por cero
   - Verificaciones previas

6. **Rendimiento** ⚡
   - Benchmarks en tiempo real
   - Comparación de operaciones
   - Millones de iteraciones

7. **Formateo** 🎨
   - Múltiples bases (dec, hex, oct)
   - Tablas formateadas
   - Output colorido

### Compilar y Ejecutar

```bash
# Con g++
g++ -std=c++20 -I../../include main.cpp -o showcase
./showcase

# Con CMake
cmake --build . --target showcase_main
./demos/showcase/showcase_main
```

### Output Esperado

La demo produce output **colorido** con:

- ✅ Marcadores de éxito en verde
- ⚠️ Advertencias en amarillo
- ❌ Errores detectados en rojo
- 📊 Tablas formateadas
- 🎯 Banners ASCII art

## 🎭 Demos Adicionales

### Expression Templates (★ NUEVO - Diciembre 2025)

Demostraciones completas de técnicas avanzadas de Expression Templates para eliminar temporales intermedios en operaciones con uint128_t.

#### [expression_templates.cpp](expression_templates.cpp) - Introducción a ET

- Comparación con/sin Expression Templates
- Explicación del método CRTP
- Árbol de expresiones lazy
- Benchmarks de rendimiento
- **Complejidad**: 582 líneas, nivel intermedio

```bash
make demo CATEGORY=showcase DEMO=expression_templates
```

#### [expression_templates_complete.cpp](expression_templates_complete.cpp) - Sistema Completo

- ✅ **10 operaciones binarias**: +, -, *, /, %, &, |, ^, <<, >>
- ✅ **Operaciones unarias**: ~, !
- ✅ **Operaciones multi-asociativas**: sum(), product(), bitwise_or/and()
- ✅ **Optimización CSE**: Common Subexpression Elimination
- ✅ **Operadores de asignación**: +=, -=, *=, /=
- ✅ **constexpr completo**: Evaluación en compile-time
- **Complejidad**: 895 líneas, nivel avanzado

```bash
make demo CATEGORY=showcase DEMO=expression_templates_complete
```

**Features destacados**:

- Desenrollado automático de operaciones n-arias
- Memoización para subexpresiones comunes
- Type-safe con SFINAE
- Benchmarks comparativos

#### [expression_templates_fold.cpp](expression_templates_fold.cpp) - Fold Expressions C++17

- **Fold expressions nativos**: `(... + args)` y `(args + ...)`
- Comparación: implementación manual vs fold nativo
- Operaciones múltiples: suma, producto, OR, AND, XOR
- Evaluación constexpr
- **Complejidad**: 489 líneas, nivel intermedio-avanzado

```bash
make demo CATEGORY=showcase DEMO=expression_templates_fold
```

**Conceptos demostrados**:

```cpp
// Left fold: (... + args)
template<typename... Args>
auto sum_fold(Args... args) {
    return (... + args.eval());  // ¡Una sola línea!
}

// Expande a: ((((arg1 + arg2) + arg3) + arg4) + ...)
```

#### [expression_templates_horner.cpp](expression_templates_horner.cpp) - Evaluación de Polinomios

- **Método de Horner** optimizado con ET
- Composición recursiva: `(a*x + b)` → `(a*x + b)*y + c` → ...
- Zero temporales en evaluación polinomial
- Polinomios de grado arbitrario
- **Complejidad**: 644 líneas, nivel avanzado

```bash
make demo CATEGORY=showcase DEMO=expression_templates_horner
```

**Ejemplo de uso**:

```cpp
// P(x) = 3x² + 2x + 5
// Horner: ((3x + 2)x + 5)
Terminal x(10);
auto expr = ((Terminal(3) * x + 2) * x + 5);
uint128_t result = expr.eval();  // Una sola evaluación
```

**Características especiales**:

- Composición incremental sin temporales
- Visualización del árbol de expresiones
- Benchmarks para polinomios de grado 3, 5 y 10
- Demostración de evaluación compile-time

#### [expression_templates_simple.cpp](expression_templates_simple.cpp) - Versión Simplificada

- Implementación didáctica con value semantics
- Más fácil de entender para principiantes
- Operaciones básicas (+, -, *, /)
- **Complejidad**: 620 líneas, nivel principiante

```bash
make demo CATEGORY=showcase DEMO=expression_templates_simple
```

**Resumen de Expression Templates**:

| Demo | Líneas | Nivel | Características Principales |
|------|--------|-------|----------------------------|
| simple | 620 | Principiante | Value semantics, operaciones básicas |
| expression_templates | 582 | Intermedio | Introducción CRTP, lazy evaluation |
| fold | 489 | Intermedio-Avanzado | Fold expressions C++17 nativos |
| complete | 895 | Avanzado | Sistema completo con CSE y multi-assoc |
| horner | 644 | Avanzado | Polinomios con método de Horner |

### Crypto Demo

```bash
demos/showcase/showcase_cryptography.cpp
```

- Aritmética modular de 128 bits
- Ejemplo de RSA toy
- Generación de números primos grandes

### Scientific Computing (Próximamente)

```bash
demos/showcase/showcase_scientific.cpp
```

- Cálculos astronómicos
- Constantes físicas de alta precisión
- Simulaciones numéricas

### Performance Comparison (Próximamente)

```bash
demos/showcase/showcase_performance.cpp
```

- Comparación vs __uint128_t (GCC builtin)
- Comparación vs Boost.Multiprecision
- Gráficos de rendimiento

## 💡 Características Destacadas

- **Output Colorido**: Usa códigos ANSI para terminal
- **Interactivo**: Pausa entre secciones
- **Educativo**: Explica cada operación
- **Completo**: Cubre toda la API
- **Impresionante**: Números realmente grandes!

## 🚀 Trucos para Presentaciones

1. **Maximiza tu terminal** para ver todo el arte ASCII
2. **Usa un tema oscuro** para mejor contraste de colores
3. **Corre en terminal real** (no VSCode integrado) para colores completos
4. **Ajusta velocidad** modificando los pause() en el código

## 📊 Ejemplo de Output

```
╔══════════════════════════════════════════════════════════════╗
║ DEMO 1: Números Realmente GRANDES                            ║
╚══════════════════════════════════════════════════════════════╝

>>> Factoriales

  ✓ 34!: 295232799039604140847618609643520000000
  Dígitos: 39

>>> Fibonacci

  ✓ Fibonacci(185): 205697230343233228174223751303346572685
  Calculados: 186 términos
```

## 🎯 Público Objetivo

- **Desarrolladores** evaluando la biblioteca
- **Presentaciones** técnicas
- **Educación** sobre enteros de 128 bits
- **Marketing** de la biblioteca
- **Benchmarking** y comparaciones

## 🔧 Personalización

Modifica las constantes en [main.cpp](main.cpp):

```cpp
const int iterations = 1000000;  // Más/menos iteraciones
#define SKIP_PAUSE  // Deshabilitar pausas
```

## 📚 Referencias

- [Tutoriales](../tutorials/) - Aprende paso a paso
- [Examples](../examples/) - Casos de uso real
- [Documentación](../../documentation/generated/html/) - API completa

---

**¡Disfruta la demo! 🎉**
