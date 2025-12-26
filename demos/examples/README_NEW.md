# Examples - Casos de Uso Reales

Esta carpeta contiene ejemplos completos que demuestran cómo usar la biblioteca int128 en aplicaciones del mundo real.

## ✅ Ejemplos Implementados

### 1. **ipv6_address.cpp** - Gestión de Direcciones IPv6
Manejo completo de direcciones IPv6 usando uint128_t.

**Características:**
- Parseo de direcciones IPv6 desde string
- Conversión a notación estándar y comprimida
- Operaciones de subred (máscaras, rangos)
- Verificación de tipos (loopback, link-local, multicast)
- Iteración de rangos de direcciones

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include ipv6_address.cpp -o ipv6_address
./ipv6_address
```

---

### 2. **uuid_generation.cpp** - UUIDs (RFC 4122)
Generación y manipulación de identificadores únicos universales.

**Características:**
- Generación de UUID v4 (aleatorio)
- Parseo desde formato canónico (8-4-4-4-12)
- Operaciones de comparación y ordenamiento
- Almacenamiento eficiente (16 bytes vs strings)
- Uso práctico en bases de datos

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include uuid_generation.cpp -o uuid_generation
./uuid_generation
```

---

### 3. **big_integer_calculator.cpp** - Calculadora Interactiva
Calculadora de línea de comandos para enteros grandes.

**Características:**
- Operaciones: suma, resta, multiplicación, división, potencia
- Funciones especiales: factorial, fibonacci, gcd, lcm
- Memoria para almacenar resultados
- Conversión entre bases (decimal, hex)
- Interfaz interactiva

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include big_integer_calculator.cpp -o calculator
./calculator
```

**Modo demo:**
```bash
./calculator --demo
```

---

### 4. **prime_factorization.cpp** - Factorización Prima
Factorización de números grandes en sus factores primos.

**Características:**
- Algoritmo de división por tentativa
- Optimizaciones para números pares y pequeños
- Verificación de resultados
- Análisis de números perfectos y potencias
- Test de primalidad

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include prime_factorization.cpp -o factorize
./factorize
```

---

### 5. **mersenne_primes.cpp** - Primos de Mersenne
Búsqueda y análisis de primos de Mersenne (2^p - 1).

**Características:**
- Generación de números de Mersenne
- Test de primalidad Lucas-Lehmer
- Relación con números perfectos
- Análisis de crecimiento exponencial
- Búsqueda sistemática

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include mersenne_primes.cpp -o mersenne
./mersenne
```

---

### 6. **demo_int128_thread_safety.cpp** - Thread Safety
Demuestra el uso seguro de int128 en contextos multithreading.

**Características:**
- Operaciones atómicas simuladas
- Sincronización con mutexes
- Ejemplo de contador compartido

**Compilar y ejecutar:**
```bash
g++ -std=c++20 -I../../include -pthread demo_int128_thread_safety.cpp -o thread_demo
./thread_demo
```

---

### 7. **demo_mathematical_library.cpp** - Funciones Matemáticas
Uso de funciones matemáticas avanzadas con int128.

**Características:**
- GCD, LCM
- Exponenciación
- Raíces cuadradas

---

### 8. **example_thread_safety.cpp** - Thread Safety 2
Otro ejemplo de thread safety con diferentes escenarios.

---

### 9. **int128.cpp** - Demo General
Demostración general de las capacidades básicas.

---

## 📊 Comparación de Ejemplos

| Ejemplo | Caso de Uso | Complejidad | Interactivo |
|---------|-------------|-------------|-------------|
| ipv6_address | Redes, Networking | Media | No |
| uuid_generation | Bases de datos, IDs | Baja | No |
| big_integer_calculator | Educación, Herramientas | Media | ✓ Sí |
| prime_factorization | Matemáticas, Cripto | Alta | No |
| mersenne_primes | Teoría de números | Alta | No |
| thread_safety demos | Programación concurrente | Media | No |

## 🚀 Compilación Rápida

### Todos los ejemplos con CMake
```bash
cd ../../
cmake -B build
cmake --build build
# Ejecutables en: build/demos/examples/
```

### Individual con g++
```bash
g++ -std=c++20 -I../../include <archivo>.cpp -o <nombre>
# Para ejemplos con threads: agregar -pthread
```

## 📋 Requisitos

- **C++20** o superior
- **Compilador:** GCC 10+, Clang 13+, MSVC 2019+
- **Para thread safety:** Flag `-pthread`

## 💡 Cómo Agregar un Ejemplo

1. **Crea tu archivo** `.cpp` en esta carpeta
2. **Incluye los headers** necesarios:
   ```cpp
   #include <uint128/uint128_t.hpp>
   #include <uint128/uint128_iostreams.hpp>
   ```
3. **Documenta el caso de uso** con comentarios claros
4. **Agrega ejemplos de output** esperado
5. **Prueba la compilación** individualmente
6. **Actualiza este README** agregando tu ejemplo

## 📝 Template para Nuevos Ejemplos

```cpp
/**
 * ===========================================================================
 * EJEMPLO: [Nombre del Ejemplo]
 * ===========================================================================
 *
 * [Descripción breve del caso de uso]
 *
 * Funcionalidades demostradas:
 * - [Característica 1]
 * - [Característica 2]
 * - [Característica 3]
 */

#include <uint128/uint128_t.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <iostream>

using namespace uint128_literals;

int main() {
    std::cout << "=== [Título del Ejemplo] ===\n\n";
    
    // Demo 1: [Descripción]
    uint128_t example = 12345_u128;
    std::cout << "Resultado: " << example << "\n";
    
    // Demo 2: [Descripción]
    // ...
    
    return 0;
}
```

## 🎯 Ideas para Futuros Ejemplos

¿Quieres contribuir? Aquí hay ideas para ejemplos adicionales:

- **Hashing de 128 bits** - Implementar funciones hash personalizadas
- **Simulaciones científicas** - Monte Carlo, física de partículas
- **Análisis financiero** - Cálculos de alta precisión para finanzas
- **Codificación de datos** - Base64, codificaciones personalizadas
- **Algoritmos genéticos** - Uso de uint128_t como genoma
- **Blockchain toy** - Hashes y proof-of-work educativos
- **Generadores PRNG** - Mersenne Twister de 128 bits
- **Compresión de datos** - Algoritmos que usan enteros grandes

## 🔗 Recursos Relacionados

- [Documentación API](../../documentation/generated/html/)
- [Tutoriales](../tutorials/)
- [Showcases](../showcase/)
- [README Principal](../../README.md)

---

**Nota:** Todos los ejemplos son educativos. Para aplicaciones de producción en criptografía, usa bibliotecas especializadas como OpenSSL o Crypto++.
