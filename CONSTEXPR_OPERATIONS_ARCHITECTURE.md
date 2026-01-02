# Arquitectura Modular de Operaciones Constexpr - uint128_t

## 📋 Resumen

Esta documentación describe la nueva arquitectura modular para operaciones con constantes en tiempo de compilación del tipo `uint128_t`.

**Ubicación**: `include/uint128/specializations/`  
**Fecha de creación**: 2 de enero de 2026  
**Estado**: 4/6 headers completados

---

## 🎯 Objetivos de la Arquitectura

1. **Separación de responsabilidades**: Cada header tiene un propósito específico
2. **Optimización compile-time**: Todas las funciones son `constexpr` y `noexcept`
3. **Extensibilidad**: Fácil añadir nuevas optimizaciones sin modificar código existente
4. **Reutilización**: Headers se pueden incluir independientemente
5. **Claridad**: Código organizado por funcionalidad, no por implementación

---

## 📁 Headers Creados (4/6)

### 1️⃣ `uint128_power_detection.hpp` ✅

**Namespace**: `uint128_power_detection`

**Propósito**: Detectar si un número es potencia de otro y calcular potencias

**Funciones principales**:

```cpp
// Detección de potencias específicas
constexpr bool is_power_of_2(uint64_t n) noexcept;
constexpr bool is_power_of_3(uint64_t n) noexcept;
constexpr bool is_power_of_5/7/10(uint64_t n) noexcept;

// Template genérico (Base ∈ [2, 63])
template <uint64_t Base>
constexpr bool is_power_of(uint64_t n) noexcept;

// Logaritmos en base específica
constexpr int log2_uint64(uint64_t n) noexcept;
constexpr int log3_uint64(uint64_t n) noexcept;
constexpr int log5/7/10_uint64(uint64_t n) noexcept;

// Template genérico de logaritmo
template <uint64_t Base>
constexpr int log_base(uint64_t n) noexcept;

// Calcular potencias en compile-time
constexpr uint64_t pow2/3/5/7/10(int exp) noexcept;

// Template genérico de potencia
template <uint64_t Base, int Exp>
constexpr uint64_t pow() noexcept;

// Versión runtime
template <uint64_t Base>
constexpr uint64_t pow_runtime(int exp) noexcept;
```

**Características**:

- ✅ Exponenciación rápida para templates
- ✅ Optimizaciones específicas para 2, 3, 5, 7, 10
- ✅ Algoritmo genérico para bases 2-63
- ✅ 100% constexpr y noexcept

---

### 2️⃣ `uint128_divisibility.hpp` ✅

**Namespace**: `uint128_divisibility_details`

**Propósito**: Detectar si un `uint128_t` es múltiplo de una constante

**Macros para uint128_t**:

```cpp
UINT128_DIVISIBILITY_PRIVATE_METHODS  // Incluir en sección private
UINT128_DIVISIBILITY_PUBLIC_METHODS   // Incluir en sección public
```

**Métodos públicos añadidos a uint128_t**:

```cpp
// Paridad
constexpr bool is_even() const noexcept;
constexpr bool is_odd() const noexcept;

// Divisibilidad por constante (Divisor ∈ [2, 63])
template <uint64_t Divisor>
constexpr bool is_multiple_of() const noexcept;
```

**Métodos privados helper**:

```cpp
constexpr bool is_even_helper() const noexcept;
constexpr bool is_odd_helper() const noexcept;
constexpr bool is_multiple_of_3/5/7/10_helper() const noexcept;

template <uint64_t Divisor>
constexpr bool is_multiple_of_helper() const noexcept;
```

**Algoritmo de reducción modular**:

```cpp
// Para uint128_t = (high, low) y módulo M:
// Paso 1: Calcular 2^64 mod M en compile-time
constexpr uint64_t pow2_64_mod_M = compute_2_64_mod(M);

// Paso 2: Reducir a 64 bits
uint64_t reduced = (high % M) * pow2_64_mod_M + (low % M);

// Paso 3: Test final
return (reduced % M) == 0;
```

**Optimizaciones**:

- ✅ Paridad: Check del LSB (bit menos significativo)
- ✅ Múltiplo de 10: `is_even() && is_multiple_of<5>()`
- ✅ Reducción modular para otros casos

---

### 3️⃣ `uint128_factorization_helpers.hpp` ✅

**Namespace**: `uint128_factorization_details`

**Propósito**: Encontrar factores pequeños mediante examen rápido

**Macros para uint128_t**:

```cpp
UINT128_FACTORIZATION_PRIVATE_METHODS  // Incluir en sección private
UINT128_FACTORIZATION_PUBLIC_METHODS   // Incluir en sección public
```

**Métodos públicos añadidos a uint128_t**:

```cpp
// Extraer máxima potencia de 2
constexpr std::pair<int, uint128_t> extract_power_of_2() const noexcept;

// Extraer máxima potencia de primo (Prime ∈ [2, 61])
template <uint64_t Prime>
constexpr std::pair<int, uint128_t> extract_power_of() const noexcept;

// Buscar primer factor primo ≤ 61
constexpr uint64_t find_small_prime_factor() const noexcept;

// Test rápido de primalidad (solo verifica factores ≤ 61)
constexpr bool is_prime_candidate() const noexcept;
```

**Datos auxiliares**:

```cpp
// Primos menores que 128
constexpr uint64_t small_primes[] = {
       2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 
      31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 
      73,  79,  83,  89,  97, 101, 103, 107, 109, 113, 
     127
};
**Ejemplo de uso**:

```cpp
uint128_t val(24);  // 24 = 2^3 * 3
auto [exp, quotient] = val.extract_power_of_2();
// exp = 3, quotient = 3

auto factor = quotient.find_small_prime_factor();
// factor = 3
```

---

### 4️⃣ `uint128_multiply_const.hpp` ✅

**Namespace**: `uint128_multiply_const_details`

**Propósito**: Multiplicar por constantes con optimizaciones

**Macros para uint128_t**:

```cpp
UINT128_MULTIPLY_CONST_PRIVATE_METHODS  // Incluir en sección private
UINT128_MULTIPLY_CONST_PUBLIC_METHODS   // Incluir en sección public
```

**Métodos públicos añadidos a uint128_t**:

```cpp
// Multiplicación por constante (N ∈ [2, 63])
template <uint64_t N>
constexpr uint128_t multiply_by() const noexcept;

// Multiplicación por potencia de 2: x * 2^Exp
template <int Exp>
constexpr uint128_t multiply_by_power_of_2() const noexcept;

// Multiplicación por potencia de primo: x * Base^Exp
template <uint64_t Base, int Exp>
constexpr uint128_t multiply_by_power() const noexcept;
```

**Optimizaciones específicas**:

```cpp
// Potencias de 2 (N = 2, 4, 8, 16, 32, 64)
x * 2^k = x << k

// Multiplicación por 3
x * 3 = (x << 1) + x

// Multiplicación por 5
x * 5 = (x << 2) + x

// Multiplicación por 7
x * 7 = (x << 3) - x

// Multiplicación por 9
x * 9 = (x << 3) + x

// Multiplicación por 10
x * 10 = (x << 3) + (x << 1)

// Multiplicación por 11
x * 11 = (x << 3) + (x << 1) + x

// Multiplicación por 15
x * 15 = (x << 4) - x
```

**Funciones auxiliares**:

```cpp
namespace uint128_multiply_const_details {
    constexpr uint128_t multiply_by_3/5/7/9/10/11/15(const uint128_t&);
}
```

**Ejemplo de uso**:

```cpp
uint128_t val(100);

// Multiplicación optimizada
auto r1 = val.multiply_by<10>();           // 1000 (usa shifts)
auto r2 = val.multiply_by_power_of_2<3>(); // 800 (shift left 3)
auto r3 = val.multiply_by_power<3, 2>();   // 900 (100 * 9)
```

---

## 📋 Headers Pendientes (2/6)

### 5️⃣ `uint128_mod_helpers.hpp` ✅ **YA EXISTE** (refactorizar)

**Estado actual**: Contiene lógica de módulo optimizado

**Acciones necesarias**:

1. ✅ Extraer funciones de detección de potencias → `uint128_power_detection.hpp` (ya hecho)
2. ⚠️ Mantener solo métodos `mod_helper<Radix>()` y especializaciones
3. ⚠️ Incluir los nuevos headers para reutilizar funciones
4. ⚠️ Eliminar código duplicado

**Macros existentes**:

```cpp
UINT128_MOD_HELPERS_PRIVATE_METHODS  // Ya definida
```

**Métodos públicos en uint128_t**:

```cpp
template <uint64_t Rad>
constexpr uint128_t mod() const noexcept;

template <int n>
constexpr uint128_t mod_pot2/3/5/7/10() const noexcept;
```

---

### 6️⃣ `uint128_div_const.hpp` 🔄 **PENDIENTE**

**Propósito**: División optimizada por constantes

**Métodos planeados**:

```cpp
// División por constante (Divisor ∈ [2, 63])
template <uint64_t Divisor>
constexpr uint128_t divide_by() const noexcept;

// División por potencia de 2: x / 2^Exp
template <int Exp>
constexpr uint128_t divide_by_power_of_2() const noexcept;

// División por potencia de primo: x / Base^Exp
template <uint64_t Base, int Exp>
constexpr uint128_t divide_by_power() const noexcept;

// División por potencia de 10 (mover desde knuth_D_divrem)
constexpr std::pair<uint128_t, uint128_t> divide_by_10() const noexcept;
constexpr std::pair<uint128_t, uint128_t> divide_by_100() const noexcept;
// etc.
```

**Funciones a mover desde `uint128_t.hpp`**:

```cpp
// Actualmente en sección privada (líneas ~1950-2050)
std::pair<uint128_t, uint128_t> divide_by_10/100/1000() const noexcept;
std::pair<uint128_t, uint128_t> divide_by_3/9/27() const noexcept;
std::pair<uint128_t, uint128_t> divide_by_5/25/125() const noexcept;

std::optional<std::pair<uint128_t, uint128_t>>
try_divide_by_power_of_10/3/5(const uint128_t& divisor) const noexcept;
```

**Optimizaciones planeadas**:

```cpp
// Potencias de 2: shift right
x / 2^k = x >> k

// Potencias de 10: Descomposición 2^k * 5^k
x / 10 = x / (2 * 5) = (x / 2) / 5

// Multiplicación por inverso modular (para primos)
x / p ≈ x * (2^128 / p) >> 128
```

---

## 🔧 Integración en uint128_t.hpp

### Paso 1: Incluir los headers

```cpp
// En uint128_t.hpp (después de línea 40)
#include "specializations/uint128_power_detection.hpp"
#include "specializations/uint128_divisibility.hpp"
#include "specializations/uint128_factorization_helpers.hpp"
#include "specializations/uint128_multiply_const.hpp"
#include "specializations/uint128_mod_helpers.hpp"      // Ya existe
#include "specializations/uint128_div_const.hpp"        // Pendiente
```

### Paso 2: Añadir macros en la clase

```cpp
class uint128_t {
    // ... código existente ...

private:
    // Métodos helper de divisibilidad
    UINT128_DIVISIBILITY_PRIVATE_METHODS
    
    // Métodos helper de factorización
    UINT128_FACTORIZATION_PRIVATE_METHODS
    
    // Métodos helper de multiplicación constante
    UINT128_MULTIPLY_CONST_PRIVATE_METHODS
    
    // Métodos helper de módulo (ya existe)
    UINT128_MOD_HELPERS_PRIVATE_METHODS
    
    // Métodos helper de división constante (pendiente)
    // UINT128_DIV_CONST_PRIVATE_METHODS

public:
    // API pública de divisibilidad
    UINT128_DIVISIBILITY_PUBLIC_METHODS
    
    // API pública de factorización
    UINT128_FACTORIZATION_PUBLIC_METHODS
    
    // API pública de multiplicación constante
    UINT128_MULTIPLY_CONST_PUBLIC_METHODS
    
    // API pública de división constante (pendiente)
    // UINT128_DIV_CONST_PUBLIC_METHODS
};
```

---

## 📊 Resumen de Funcionalidad

### Métodos añadidos a uint128_t (API completa)

| Categoría | Método | Template | Descripción |
|-----------|--------|----------|-------------|
| **Paridad** | `is_even()` | - | Verifica si es par |
| | `is_odd()` | - | Verifica si es impar |
| **Divisibilidad** | `is_multiple_of<N>()` | N ∈ [2, 63] | Verifica divisibilidad |
| **Factorización** | `extract_power_of_2()` | - | Extrae potencia de 2 |
| | `extract_power_of<P>()` | P ∈ [2, 61] | Extrae potencia de primo |
| | `find_small_prime_factor()` | - | Busca factor ≤ 61 |
| | `is_prime_candidate()` | - | Test rápido de primalidad |
| **Multiplicación** | `multiply_by<N>()` | N ∈ [2, 63] | Multiplica por constante |
| | `multiply_by_power_of_2<E>()` | E ∈ [0, 127] | Multiplica por 2^E |
| | `multiply_by_power<B, E>()` | B ∈ [2, 61] | Multiplica por B^E |
| **Módulo** | `mod<R>()` | R ∈ [2, 63] | Módulo por constante |
| | `mod_pot2/3/5/7/10<n>()` | n ≥ 1 | Módulo por potencia |

### Funciones auxiliares (namespaces)

| Namespace | Funciones | Uso |
|-----------|-----------|-----|
| `uint128_power_detection` | `is_power_of<B>()`, `pow<B,E>()`, `log_base<B>()` | Detección y cálculo de potencias |
| `uint128_divisibility_details` | `compute_2_64_mod()` | Reducción modular |
| `uint128_factorization_details` | `small_primes[]` | Lista de primos 2-61 |
| `uint128_multiply_const_details` | `multiply_by_3/5/7/9/10/11/15()` | Multiplicaciones optimizadas |

---

## 🎯 Ventajas de la Nueva Arquitectura

### ✅ Separación de Responsabilidades

- Cada header tiene un propósito claro y único
- Fácil encontrar y modificar funcionalidad específica
- No hay dependencias circulares

### ✅ Optimización Compile-Time

- Todas las funciones son `constexpr` y `noexcept`
- Cálculos en tiempo de compilación cuando es posible
- Zero runtime overhead para constantes conocidas

### ✅ Extensibilidad

- Añadir nuevas optimizaciones no requiere modificar código existente
- Plantillas genéricas permiten extensión para más bases/divisores
- Headers independientes permiten incluir solo lo necesario

### ✅ Reutilización

- Funciones de detección de potencias usadas en múltiples contextos
- Reducción modular reutilizada en divisibilidad y módulo
- Algoritmos genéricos con especializaciones optimizadas

### ✅ Mantenibilidad

- Código organizado por funcionalidad lógica
- Documentación exhaustiva en cada header
- Ejemplos de uso en comentarios

---

## 📝 Próximos Pasos

1. ✅ **Crear `uint128_multiply_const.hpp`** (Completado)
2. ⚠️ **Crear `uint128_div_const.hpp`** (Pendiente)
3. ⚠️ **Refactorizar `uint128_mod_helpers.hpp`** (Integrar nuevos headers)
4. ⚠️ **Actualizar `uint128_t.hpp`** (Incluir headers y macros)
5. ⚠️ **Tests comprehensivos** (Validar todas las optimizaciones)
6. ⚠️ **Benchmarks** (Comparar con implementaciones anteriores)
7. ⚠️ **Replicar para int128_t** (Adaptar para tipo signed)

---

**Última actualización**: 2 de enero de 2026  
**Estado**: 4/6 headers completados (66%)  
**Autor**: Copilot + Usuario
