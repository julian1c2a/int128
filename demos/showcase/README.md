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

### Crypto Demo (Próximamente)
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
