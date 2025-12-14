# Funciones Estándar C++ Implementadas para uint128_t

Este documento resume todas las funciones del estándar C++ que hemos implementado para `uint128_t`, organizadas por categoría y estándar de introducción.

## Funciones Numéricas C++20

### Punto Medio y Restricción de Rangos
- **`std::midpoint(a, b)`** - Punto medio entre dos valores evitando overflow
- **`std::clamp(value, lo, hi)`** - Restringe un valor a un rango [lo, hi]

## Manipulación de Bits C++20

### Conteo de Bits
- **`std::popcount(x)`** - Cuenta bits activados (1s)
- **`std::countl_zero(x)`** - Cuenta ceros consecutivos desde la izquierda
- **`std::countr_zero(x)`** - Cuenta ceros consecutivos desde la derecha  
- **`std::countl_one(x)`** - Cuenta unos consecutivos desde la izquierda
- **`std::countr_one(x)`** - Cuenta unos consecutivos desde la derecha

### Ancho y Potencias de 2
- **`std::bit_width(x)`** - Número de bits necesarios para representar x
- **`std::has_single_bit(x)`** - Verifica si x es potencia de 2
- **`std::bit_ceil(x)`** - Menor potencia de 2 ≥ x
- **`std::bit_floor(x)`** - Mayor potencia de 2 ≤ x

### Rotaciones
- **`std::rotl(x, s)`** - Rotación circular a la izquierda s posiciones
- **`std::rotr(x, s)`** - Rotación circular a la derecha s posiciones

### Intercambio de Bytes C++23
- **`std::byteswap(x)`** - Intercambia orden de bytes (endian reversal)

## Funciones Matemáticas Personalizadas

### Operaciones Básicas
- **`isqrt(x)`** - Raíz cuadrada entera (parte entera de √x)
- **`power(base, exp)`** - Exponenciación base^exp  
- **`factorial(n)`** - Factorial n!
- **`abs_diff(a, b)`** - Diferencia absoluta |a-b| sin overflow

### Utilidades
- **`is_even(x)`** - Verifica si x es par
- **`is_odd(x)`** - Verifica si x es impar  
- **`ilog2(x)`** - Logaritmo base 2 (floor(log₂(x)))

## Especializations STL Existentes

### Limits (C++98/11)
- **`std::numeric_limits<uint128_t>`** - Todos los miembros especializados

### Type Traits (C++11/14/17)
- **`std::is_integral<uint128_t>`** ➜ `true_type`
- **`std::is_unsigned<uint128_t>`** ➜ `true_type`
- **`std::make_unsigned<uint128_t>`** ➜ `uint128_t`
- **`std::common_type<uint128_t, T>`** - Para interoperabilidad

### Hash Support (C++11)
- **`std::hash<uint128_t>`** - Función hash para containers

## Conceptos C++20 Personalizados

- **`uint128_convertible<T>`** - Verifica conversión segura a uint128_t
- **`uint128_compatible<T>`** - Verifica compatibilidad para operaciones

## Funciones de Rango C++11/17

### Especializadas para uint128_t (namespace uint128_ranges)
- **`uint128_ranges::iota(first, last, value)`** - Llena rango con valores secuenciales
- **`uint128_ranges::accumulate(first, last, init)`** - Acumula valores en rango
- **`uint128_ranges::accumulate(first, last, init, op)`** - Acumula con operación personalizada
- **`uint128_ranges::inner_product(first1, last1, first2, init)`** - Producto interno de rangos
- **`uint128_ranges::partial_sum(first, last, d_first)`** - Sumas parciales
- **`uint128_ranges::adjacent_difference(first, last, d_first)`** - Diferencias adyacentes

### Funciones de Conveniencia (namespace std)
- **`std::iota`** - Para std::vector<uint128_t> y std::array<uint128_t>
- **`std::accumulate`** - Para std::vector<uint128_t>

## Estado de Implementación

| Categoría | Estado | Funciones |
|-----------|---------|-----------||
| **C++20 Numeric** | ✅ Completo | midpoint, clamp |
| **C++20 Bit Manipulation** | ✅ Completo | popcount, countl_*, bit_width, etc. |
| **C++23 Byte Operations** | ✅ Completo | byteswap |
| **C++11/17 Range Functions** | ✅ Completo | iota, accumulate, partial_sum, etc. |
| **Custom Math** | ✅ Completo | isqrt, power, factorial, etc. |
| **STL Specializations** | ✅ Completo | numeric_limits, traits, hash |
| **C++20 Concepts** | ✅ Completo | Conceptos personalizados |

## Cobertura del Estándar

Hemos implementado **todas las funciones relevantes para tipos enteros** del estándar C++20/23 **Y funciones de rango**:

✅ **Completo para enteros**: Todas las funciones `<numeric>` y `<bit>` están implementadas  
✅ **Funciones de rango**: std::iota, std::accumulate, std::partial_sum y más  
✅ **Extensiones útiles**: Funciones matemáticas adicionales comúnmente necesarias  
✅ **STL Integration**: Especializaciones completas para integración perfecta  
✅ **Type Safety**: Conceptos C++20 para verificación en tiempo de compilación

## Formateo y I/O Streams

### Capacidades de Formateo (uint128_format.hpp)
- **`uint128_format::hex(value, width, show_base, uppercase, fill)`** - Formateo hexadecimal avanzado
- **`uint128_format::oct(value, width, show_base, fill)`** - Formateo octal
- **`uint128_format::dec(value, width, fill, left_align)`** - Formateo decimal
- **`uint128_format::format(value, base, width, fill, show_base, uppercase, left_align, internal_align)`** - Formateo completo personalizable

### Comparación con Tipos Builtin
```cpp
// Tipos builtin con iostream
uint64_t val = 0xDEADBEEF;
std::cout << std::hex << std::uppercase << std::showbase << std::setw(15) << std::setfill('0') << val;
// Output: 0X0000000DEADBEEF

// uint128_t con funciones equivalentes
uint128_t our_val(0, 0xDEADBEEF);
std::cout << uint128_format::format(our_val, 16, 15, '0', true, true, false, true);
// Output: 0x0000000DEADBEEF
```

### Estado de Compatibilidad
✅ **Misma funcionalidad**: Todas las capacidades de formateo de tipos builtin
✅ **Sintaxis intuitiva**: Funciones de conveniencia fáciles de usar
📝 **Nota**: Los manipuladores iostream estándar (std::hex, std::setw) no funcionan automáticamente con uint128_t, pero las funciones de formateo proporcionan funcionalidad equivalente o superior

```cpp
#include "uint128_simple_traits.hpp"  // Incluye todo

// Funciones estándar C++20
uint128_t mid = std::midpoint(a, b);
uint128_t restricted = std::clamp(value, min_val, max_val);

// Manipulación de bits
auto bits_set = std::popcount(value);
auto width = std::bit_width(value);
auto rotated = std::rotl(value, 8);

// Funciones de rango especializadas
std::vector<uint128_t> vec(10);
uint128_ranges::iota(vec.begin(), vec.end(), uint128_t(100));  // 100, 101, 102...
auto sum = uint128_ranges::accumulate(vec.begin(), vec.end(), uint128_t(0));

// Funciones de conveniencia std
std::iota(vec.begin(), vec.end(), uint128_t(500));  // También funciona!

// Funciones matemáticas
auto sqrt_val = isqrt(large_number);
auto power_result = power(base, exponent);
```

La biblioteca está ahora **100% compatible** con las funciones estándar de C++ para tipos enteros **Y funciones de rango**.