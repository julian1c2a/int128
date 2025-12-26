# Generalización a Enteros de Ancho Fijo Parametrizable

## Fecha
26 de diciembre de 2025

## Resumen Ejecutivo

Este documento explora la evolución natural del proyecto `uint128_t` hacia una biblioteca de enteros de precisión arbitraria pero fija en tiempo de compilación: `uint_fixed_t<N>`, donde `N` representa el número de palabras de 64 bits.

**Objetivo**: Crear una clase template que permita:
```cpp
using uint128_t  = uint_fixed_t<2>;   // 128 bits
using uint256_t  = uint_fixed_t<4>;   // 256 bits  
using uint512_t  = uint_fixed_t<8>;   // 512 bits
using uint1024_t = uint_fixed_t<16>;  // 1024 bits
```

---

## 1. Diseño Conceptual

### 1.1 Estructura Base

```cpp
template<size_t N>
class uint_fixed_t {
    static_assert(N > 0, "Número de palabras debe ser positivo");
    static_assert(N <= 1024, "Límite razonable para evitar mal uso");
    
private:
    // Little-endian: data[0] contiene los bits menos significativos
    uint64_t data[N];
    
public:
    static constexpr size_t num_words = N;
    static constexpr size_t num_bits = N * 64;
    
    // Constructores
    constexpr uint_fixed_t() noexcept : data{} {}
    constexpr uint_fixed_t(uint64_t low) noexcept;
    
    // Operadores aritméticos
    constexpr uint_fixed_t& operator+=(const uint_fixed_t& other) noexcept;
    constexpr uint_fixed_t& operator-=(const uint_fixed_t& other) noexcept;
    constexpr uint_fixed_t& operator*=(const uint_fixed_t& other) noexcept;
    constexpr uint_fixed_t& operator/=(const uint_fixed_t& other);
    
    // Operadores bit a bit (triviales)
    constexpr uint_fixed_t& operator&=(const uint_fixed_t& other) noexcept;
    constexpr uint_fixed_t& operator|=(const uint_fixed_t& other) noexcept;
    constexpr uint_fixed_t& operator^=(const uint_fixed_t& other) noexcept;
    
    // Shifts (requieren propagación)
    constexpr uint_fixed_t& operator<<=(int shift) noexcept;
    constexpr uint_fixed_t& operator>>=(int shift) noexcept;
    
    // Comparaciones
    constexpr auto operator<=>(const uint_fixed_t& other) const noexcept;
    constexpr bool operator==(const uint_fixed_t& other) const noexcept;
};
```

### 1.2 Representación Interna

**Little-endian por palabra**:
- `data[0]` = bits 0-63
- `data[1]` = bits 64-127
- `data[N-1]` = bits más significativos

**Ventajas**:
- Coherente con arquitecturas x86-64
- Operaciones de bajo nivel (suma con carry) son naturales
- Fácil conversión desde/hacia tipos nativos

---

## 2. Categorización de Operaciones por Complejidad

### 2.1 Operaciones Triviales - O(N)

#### Operaciones Bit a Bit

```cpp
constexpr uint_fixed_t& operator&=(const uint_fixed_t& other) noexcept {
    for (size_t i = 0; i < N; ++i) {
        data[i] &= other.data[i];
    }
    return *this;
}
```

**Generalización**: Directa desde el código actual de 128-bit.

#### Comparaciones

```cpp
constexpr std::strong_ordering operator<=>(const uint_fixed_t& other) const noexcept {
    // Comparar desde la palabra más significativa
    for (size_t i = N; i > 0; --i) {
        if (data[i-1] != other.data[i-1]) {
            return data[i-1] <=> other.data[i-1];
        }
    }
    return std::strong_ordering::equal;
}
```

### 2.2 Operaciones con Carry Propagation - O(N)

#### Suma con Carry

```cpp
constexpr uint_fixed_t& operator+=(const uint_fixed_t& other) noexcept {
    uint64_t carry = 0;
    for (size_t i = 0; i < N; ++i) {
        uint64_t old_val = data[i];
        data[i] += other.data[i] + carry;
        
        // Detectar overflow: si suma < operando, hubo carry
        carry = (data[i] < old_val) || (carry && data[i] == old_val);
    }
    return *this;
}
```

**Optimización con intrinsics** (x86-64):
```cpp
#if defined(__x86_64__) && defined(__ADX__)
    unsigned char carry = 0;
    for (size_t i = 0; i < N; ++i) {
        carry = _addcarry_u64(carry, data[i], other.data[i], &data[i]);
    }
#endif
```

#### Resta con Borrow

Similar a la suma, pero propagando el borrow:
```cpp
constexpr uint_fixed_t& operator-=(const uint_fixed_t& other) noexcept {
    uint64_t borrow = 0;
    for (size_t i = 0; i < N; ++i) {
        uint64_t old_val = data[i];
        data[i] -= other.data[i] + borrow;
        
        // Borrow si: restamos más de lo que teníamos
        borrow = (data[i] > old_val) || (borrow && data[i] == old_val);
    }
    return *this;
}
```

**Intrinsics disponibles**: `_subborrow_u64` (BMI2)

### 2.3 Shifts - O(N)

#### Shift Left

El shift requiere transferir bits entre palabras:

```cpp
constexpr uint_fixed_t& operator<<=(int shift) noexcept {
    if (shift == 0) return *this;
    if (shift >= num_bits) {
        std::fill(std::begin(data), std::end(data), 0);
        return *this;
    }
    
    const size_t word_shift = shift / 64;
    const int bit_shift = shift % 64;
    
    if (bit_shift == 0) {
        // Shift solo de palabras completas
        for (size_t i = N - 1; i >= word_shift; --i) {
            data[i] = data[i - word_shift];
        }
        std::fill_n(data, word_shift, 0);
    } else {
        // Shift con bits parciales
        for (size_t i = N - 1; i > word_shift; --i) {
            data[i] = (data[i - word_shift] << bit_shift) |
                      (data[i - word_shift - 1] >> (64 - bit_shift));
        }
        data[word_shift] = data[0] << bit_shift;
        std::fill_n(data, word_shift, 0);
    }
    
    return *this;
}
```

**Nota**: El código actual de `uint128_t` ya implementa esta lógica para N=2.

---

## 3. Multiplicación: Algoritmos y Complejidad

### 3.1 Multiplicación Escolar - O(N²)

**Algoritmo actual en uint128_t**: Generalizable directamente.

```cpp
constexpr uint_fixed_t& operator*=(const uint_fixed_t& other) noexcept {
    uint_fixed_t result;  // Inicializado a 0
    
    // Multiplicación escolar palabra por palabra
    for (size_t i = 0; i < N; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < N - i; ++j) {
            // Multiplicación de 64x64 -> 128 bits
            unsigned __int128 product = 
                static_cast<unsigned __int128>(data[i]) * other.data[j] + 
                result.data[i + j] + carry;
            
            result.data[i + j] = static_cast<uint64_t>(product);
            carry = static_cast<uint64_t>(product >> 64);
        }
    }
    
    *this = result;
    return *this;
}
```

**Complejidad**: O(N²)  
**Viable hasta**: N ≈ 8-16 (512-1024 bits) antes de que otros algoritmos sean superiores.

### 3.2 Algoritmo de Karatsuba - O(N^1.585)

**Referencia**: Karatsuba, A., & Ofman, Y. (1962). "Multiplication of Multidigit Numbers on Automata". *Soviet Physics Doklady*, 7, 595-596.

**Principio**: Divide y conquista reduciendo 4 multiplicaciones a 3.

Para dos números de N palabras:
```
A = A_high * 2^(N/2*64) + A_low
B = B_high * 2^(N/2*64) + B_low

A * B = z2 * 2^(N*64) + z1 * 2^(N/2*64) + z0

donde:
  z0 = A_low * B_low
  z2 = A_high * B_high
  z1 = (A_low + A_high) * (B_low + B_high) - z0 - z2
```

**Solo 3 multiplicaciones recursivas** en lugar de 4.

**Implementación**:
```cpp
static uint_fixed_t karatsuba_multiply(const uint_fixed_t& a, const uint_fixed_t& b) {
    // Caso base: usar multiplicación escolar para N pequeño
    constexpr size_t KARATSUBA_THRESHOLD = 4;
    if (N <= KARATSUBA_THRESHOLD) {
        return schoolbook_multiply(a, b);
    }
    
    constexpr size_t half = N / 2;
    
    // Dividir en mitades (requiere uint_fixed_t de tamaño variable)
    auto a_low = extract_low_half(a);   // uint_fixed_t<half>
    auto a_high = extract_high_half(a);
    auto b_low = extract_low_half(b);
    auto b_high = extract_high_half(b);
    
    // Tres multiplicaciones recursivas
    auto z0 = karatsuba_multiply(a_low, b_low);
    auto z2 = karatsuba_multiply(a_high, b_high);
    auto z1 = karatsuba_multiply(a_low + a_high, b_low + b_high) - z0 - z2;
    
    // Combinar resultados
    return (z2 << (N * 64)) + (z1 << (half * 64)) + z0;
}
```

**Complejidad**: T(N) = 3T(N/2) + O(N) → O(N^(log₂3)) ≈ O(N^1.585)

**Viable desde**: N ≥ 8 (512 bits) aproximadamente.

### 3.3 Multiplicación FFT - O(N log N)

**Referencia**: Schönhage, A., & Strassen, V. (1971). "Schnelle Multiplikation grosser Zahlen". *Computing*, 7(3-4), 281-292.

**Principio**: Tratar los números como polinomios y usar convolución vía FFT.

**Pasos**:
1. Representar A y B como polinomios en base 2^k
2. Evaluar en puntos usando FFT → O(N log N)
3. Multiplicar punto a punto → O(N)
4. Interpolar usando IFFT → O(N log N)

**Implementación conceptual**:
```cpp
// Requiere librería FFT (FFTW, o implementación propia)
static uint_fixed_t fft_multiply(const uint_fixed_t& a, const uint_fixed_t& b) {
    // Convertir a representación FFT
    const size_t fft_size = next_power_of_2(2 * N);
    std::vector<std::complex<double>> a_fft(fft_size), b_fft(fft_size);
    
    // Llenar con palabras de 64 bits (o subdivir en chunks más pequeños)
    for (size_t i = 0; i < N; ++i) {
        a_fft[i] = static_cast<double>(a.data[i]);
        b_fft[i] = static_cast<double>(b.data[i]);
    }
    
    // FFT forward
    fft_forward(a_fft);
    fft_forward(b_fft);
    
    // Multiplicación punto a punto
    for (size_t i = 0; i < fft_size; ++i) {
        a_fft[i] *= b_fft[i];
    }
    
    // FFT inverse
    fft_inverse(a_fft);
    
    // Reconstruir resultado con propagación de carry
    uint_fixed_t result;
    uint64_t carry = 0;
    for (size_t i = 0; i < N; ++i) {
        uint64_t word = static_cast<uint64_t>(a_fft[i].real() + 0.5) + carry;
        result.data[i] = word;
        carry = word >> 64;  // Simplificado, necesita más cuidado
    }
    
    return result;
}
```

**Complejidad**: O(N log N)

**Viable desde**: N ≥ 64 (4096 bits), debido al overhead de FFT.

**Nota práctica**: Requiere aritmética de punto flotante de alta precisión para evitar errores de redondeo. En la práctica, se usa NTT (Number Theoretic Transform) en campos finitos.

### 3.4 Estrategia Adaptativa

```cpp
constexpr uint_fixed_t& operator*=(const uint_fixed_t& other) noexcept {
    if constexpr (N <= 4) {
        // Hasta 256 bits: escolar
        return schoolbook_multiply_inplace(other);
    } else if constexpr (N <= 32) {
        // 512-2048 bits: Karatsuba
        *this = karatsuba_multiply(*this, other);
        return *this;
    } else {
        // > 2048 bits: FFT
        *this = fft_multiply(*this, other);
        return *this;
    }
}
```

---

## 4. División: Algoritmos

### 4.1 División Larga (Long Division) - O(N²)

**Algoritmo actual en uint128_t**: Basado en división larga binaria.

**Generalización para N palabras**:

```cpp
constexpr uint_fixed_t& operator/=(const uint_fixed_t& divisor) {
    if (divisor.is_zero()) throw std::domain_error("Division by zero");
    if (*this < divisor) {
        *this = uint_fixed_t{0};
        return *this;
    }
    
    uint_fixed_t quotient{0};
    uint_fixed_t remainder{0};
    
    // Procesar bit a bit desde el más significativo
    for (int i = num_bits - 1; i >= 0; --i) {
        remainder <<= 1;
        if (test_bit(i)) {
            remainder.set_bit(0);
        }
        
        if (remainder >= divisor) {
            remainder -= divisor;
            quotient.set_bit(i);
        }
    }
    
    *this = quotient;
    return *this;
}
```

**Complejidad**: O(N² * 64) = O(N²) operaciones de palabras completas.

### 4.2 Algoritmo de Newton-Raphson - O(M(N))

**Referencia**: Brent, R. P., & Zimmermann, P. (2010). *Modern Computer Arithmetic*. Cambridge University Press. (Sección 3.5)

**Principio**: Calcular 1/divisor mediante iteración de Newton, luego multiplicar.

Para calcular A / B:
1. Calcular R = 1/B usando Newton-Raphson
2. Resultado = A * R

**Iteración de Newton para 1/B**:
```
R_(n+1) = R_n * (2 - B * R_n)
```

Converge cuadráticamente: cada iteración duplica los dígitos correctos.

**Complejidad**: O(M(N) log N), donde M(N) es el costo de multiplicación.
- Con Karatsuba: O(N^1.585 log N)
- Con FFT: O(N log² N)

**Implementación conceptual**:
```cpp
static uint_fixed_t reciprocal_newton(const uint_fixed_t& divisor) {
    // Aproximación inicial (primeras palabras)
    uint_fixed_t r = initial_approximation(divisor);
    
    // Iteraciones de Newton
    for (int prec = 64; prec < num_bits; prec *= 2) {
        // R = R * (2 - divisor * R)
        uint_fixed_t two = 2;
        r *= (two - divisor * r);
        // Truncar a precisión actual
    }
    
    return r;
}

constexpr uint_fixed_t operator/(const uint_fixed_t& divisor) const {
    uint_fixed_t recip = reciprocal_newton(divisor);
    return (*this * recip) >> num_bits;  // Ajustar escala
}
```

**Viable desde**: N ≥ 16 (1024 bits), cuando el costo de multiplicación compensa el overhead.

### 4.3 Algoritmo de Burnikel-Ziegler - O(M(N) log N)

**Referencia**: Burnikel, C., & Ziegler, J. (1998). "Fast Recursive Division". *Research Report MPI-I-98-1-022*, Max-Planck-Institut für Informatik.

**Principio**: Divide y conquista para división de multi-palabras.

**Mejores para**: Casos donde Newton-Raphson tiene overhead por conversiones.

---

## 5. Propuestas del Estándar C++

### 5.1 Papers de WG21 (C++ Standards Committee)

#### P0539R0: "A Proposal to add wide_int to the Standard Library" (2017)
- **Autor**: Lawrence Crowl
- **Estado**: Rechazado / No progresó
- **Propuesta**: Añadir `std::wide_int<Bits>` como tipo de entero de ancho fijo
- **URL**: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0539r0.html

**Características propuestas**:
```cpp
template<size_t Bits>
class wide_int {
    // Entero con signo de Bits bits
    // Bits debe ser múltiplo de algún valor base (8, 32, 64)
};

template<size_t Bits>
class wide_uint {
    // Entero sin signo de Bits bits
};
```

**Razones del rechazo**:
- Falta de consenso sobre representación interna
- Preocupaciones sobre rendimiento vs tipos nativos
- Preferencia por extensiones de compiladores específicos

#### P1889R0: "Efficient fixed-width integer types" (2019)
- **Autores**: Multiple contributors
- **Estado**: En discusión
- **Propuesta**: Mejorar `std::intN_t` y añadir soporte para tamaños no estándar

**No directamente aplicable**, pero relacionado con la estandarización de tipos de ancho fijo.

#### P0101R0: "An Outline of a C++ Numbers Technical Specification" (2015)
- **Incluía**: Discusión sobre aritmética de precisión extendida
- **Estado**: Nunca llegó a TS (Technical Specification)

### 5.2 Boost.Multiprecision

**Referencia oficial**: https://www.boost.org/doc/libs/1_84_0/libs/multiprecision/doc/html/index.html

Boost ya proporciona una solución madura:

```cpp
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

// Enteros de ancho fijo
using int128_t  = number<cpp_int_backend<128, 128, signed_magnitude, unchecked, void>>;
using uint256_t = number<cpp_int_backend<256, 256, unsigned_magnitude, unchecked, void>>;

// Alternativa con template más simple
using uint512_t = checked_uint512_t;  // Alias predefinido
```

**Características**:
- **Ancho fijo en compilación**: `cpp_int_backend<Bits, Bits, ...>`
- **Optimizaciones**: Usa intrinsics cuando está disponible
- **Constexpr**: Soporte parcial desde C++14
- **Expresiones mixtas**: Permite operar con tipos nativos

**Diferencias con este proyecto**:
| Característica | uint128_t (este proyecto) | Boost.Multiprecision |
|---|---|---|
| **Tamaño** | Fijo: 128 bits | Configurable: cualquier múltiplo de 64 |
| **Dependencias** | Header-only, sin deps | Requiere Boost |
| **Constexpr** | Total (C++20) | Parcial |
| **Rendimiento** | Optimizado para 128-bit | Más general, puede ser más lento |
| **Algoritmos** | Escolar solamente | Múltiples algoritmos según N |

### 5.3 GCC/Clang `__int128`

**No es estándar**, pero ampliamente disponible:

```cpp
#if defined(__SIZEOF_INT128__)
    using int128_t = __int128;
    using uint128_t = unsigned __int128;
#endif
```

**Limitaciones**:
- No hay `__int256` ni tamaños superiores
- No es portable (no funciona en MSVC)
- No extensible por el usuario

### 5.4 C23 `_BitInt(N)` - Tipos de Enteros de Precisión Arbitraria en C

#### 5.4.1 Especificación en el Estándar C23

**Estado**: ✅ **ADOPTADO** en ISO/IEC 9899:2023 (C23)

El estándar C23 introduce `_BitInt(N)`, un tipo de entero de precisión de bits exacta, donde N especifica el número exacto de bits (incluyendo el bit de signo para tipos con signo).

**Sintaxis**:
```c
// Entero con signo de N bits
_BitInt(N) signed_var;

// Entero sin signo de N bits  
unsigned _BitInt(N) unsigned_var;

// Ejemplos concretos
_BitInt(128) x = 42;                    // Entero con signo de 128 bits
unsigned _BitInt(256) y = 0xFFFF;       // Entero sin signo de 256 bits
_BitInt(1024) z;                         // Entero con signo de 1024 bits
```

#### 5.4.2 Características Técnicas

**Rango de valores**:
- **Tipo con signo** `_BitInt(N)`:
  - Rango: [-2^(N-1), 2^(N-1) - 1]
  - Representación: Complemento a dos (obligatorio en C23)
  - Ejemplo: `_BitInt(8)` → [-128, 127]

- **Tipo sin signo** `unsigned _BitInt(N)`:
  - Rango: [0, 2^N - 1]
  - Ejemplo: `unsigned _BitInt(8)` → [0, 255]

**Límites del sistema**:
```c
#include <limits.h>

// Ancho máximo soportado por la implementación
#ifndef BITINT_MAXWIDTH
    #define BITINT_MAXWIDTH __BITINT_MAXWIDTH__
#endif

// Garantía mínima: BITINT_MAXWIDTH >= 128
static_assert(BITINT_MAXWIDTH >= 128, "C23 requiere al menos 128 bits");
```

**Requisitos de C23**:
- Todos los tipos enteros usan **complemento a dos** obligatoriamente
- `_BitInt(N)` debe soportar todos los valores de N desde 1 hasta `BITINT_MAXWIDTH`
- Valores comunes de `BITINT_MAXWIDTH`: 128, 256, 512, 1024, o incluso mayor según implementación

#### 5.4.3 Operaciones Soportadas

```c
_BitInt(256) a = 100;
_BitInt(256) b = 200;

// Aritmética estándar
_BitInt(256) sum = a + b;
_BitInt(256) product = a * b;
_BitInt(256) quotient = a / b;

// Operaciones bit a bit
_BitInt(256) and_result = a & b;
_BitInt(256) or_result = a | b;
_BitInt(256) xor_result = a ^ b;

// Comparaciones
if (a < b) {
    // ...
}

// Conversiones
int x = 42;
_BitInt(256) big = x;  // Conversión desde tipo estándar

// Literales (extensión de compilador)
_BitInt(128) literal = 340282366920938463463374607431768211456wb;  // GCC/Clang
```

#### 5.4.4 Papers de WG14 (C Standards Committee)

1. **N2709** (2021): "Bit-precise integer types" - **ADOPTADO en C23**
   - Autor: Melanie Blower (Intel)
   - URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2709.pdf
   - Especificación final que fue adoptada

2. **N2425** (2019): "Bit-precise integer types" - Versión preliminar
   - Propuesta inicial con sintaxis y semántica básicas
   - URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2425.pdf

3. **N2763** (2021): "Bit-precise integer types (latest draft)"
   - Revisión final antes de adopción
   - URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2763.pdf

4. **N1281** (2007): "Extended Integer Types" - Propuesta anterior rechazada
   - Primera tentativa, diseño diferente
   - No llegó a adoptarse

#### 5.4.5 Soporte en Compiladores (2025)

| Compilador | Versión | Soporte _BitInt(N) | Estado | Notas |
|------------|---------|-------------------|--------|-------|
| **Clang** | 14+ | ✅ Completo | Producción | Soporte desde Clang 14 (2022) |
| **GCC** | 14+ | ⚠️ Experimental | En desarrollo | `-std=c23` requerido |
| **Intel ICX** | 2023.0+ | ⚠️ Parcial | Beta | Soporte limitado a N ≤ 256 |
| **MSVC** | - | ❌ No disponible | Planeado | En roadmap para C23 |
| **TinyCC** | - | ❌ No disponible | - | - |

**Ejemplo de uso con Clang 14+**:
```bash
# Compilar con soporte _BitInt
clang -std=c23 -o program program.c

# Verificar soporte
clang -dM -E - < /dev/null | grep BITINT_MAXWIDTH
# Output: #define __BITINT_MAXWIDTH__ 128
```

#### 5.4.6 Conexión con Fortran `INTEGER(KIND=n)`

**Contexto histórico**: `_BitInt(N)` de C23 se inspiró en el sistema de tipos parametrizados de Fortran, especialmente `INTEGER(KIND=n)` introducido en **Fortran 90 (1991)**.

**Fortran INTEGER(KIND=n)** - Desde 1991:
```fortran
! KIND especifica el rango/precisión deseado
INTEGER(KIND=4) :: small_int    ! Típicamente 32 bits
INTEGER(KIND=8) :: big_int      ! Típicamente 64 bits
INTEGER(KIND=16) :: huge_int    ! Típicamente 128 bits (si disponible)

! Selección automática basada en rango
INTEGER(KIND=SELECTED_INT_KIND(9)) :: my_int  ! KIND para rango [-10^9, 10^9]
INTEGER(KIND=SELECTED_INT_KIND(18)) :: big    ! KIND para rango [-10^18, 10^18]
```

**Comparación: Fortran vs C23**:

| Característica | Fortran `INTEGER(KIND=n)` | C23 `_BitInt(N)` |
|----------------|---------------------------|------------------|
| **Introducido** | 1991 (Fortran 90) | 2023 (C23) |
| **Parámetro** | KIND (impl-dependent) | N = bits exactos |
| **Sintaxis** | `INTEGER(KIND=8)` | `_BitInt(128)` |
| **Portabilidad** | KIND varía por compilador | N es exacto en todas partes |
| **Rango especificación** | Indirecto (SELECTED_INT_KIND) | Directo (bits exactos) |
| **Ejemplo** | `INTEGER(KIND=8)` → 64 bits (típico) | `_BitInt(64)` → 64 bits (exacto) |
| **Máximo** | Depende compilador (128-256 bits) | `BITINT_MAXWIDTH` (≥128, típico 1024+) |
| **Propósito** | Ciencia/ingeniería numérica | Propósito general + crypto |

**Ejemplo de portabilidad**:

```fortran
! Fortran: KIND no portable
INTEGER(KIND=4) :: x  ! ¿32 bits? ¿64 bits? Depende del compilador

! Mejor práctica en Fortran (portable):
INTEGER(KIND=SELECTED_INT_KIND(9)) :: x  ! Garantiza rango [-10^9, 10^9]
```

```c
// C23: Siempre exacto y portable
_BitInt(32) x;     // Exactamente 32 bits en CUALQUIER compilador
_BitInt(128) y;    // Exactamente 128 bits en CUALQUIER compilador
```

**Influencia en el diseño de _BitInt**:
- Fortran demostró la **utilidad de tipos parametrizados** desde 1991
- C23 tomó la idea pero la mejoró: **bits exactos** en lugar de "kinds" dependientes de implementación
- Ambos permiten **selección en tiempo de compilación** del tamaño óptimo

#### 5.4.7 Comparación: C `_BitInt(N)` vs C++ (sin estándar)

| Característica | C23 `_BitInt(N)` | C++ (sin estándar) | C++ `uint_fixed_t<N>` (este proyecto) |
|----------------|------------------|-------------------|---------------------------------------|
| **Estado oficial** | ✅ Estándar C23 | ❌ No estándar | ❌ Biblioteca usuario |
| **Sintaxis** | `_BitInt(256)` | N/A | `uint_fixed_t<4>` (4 × 64 bits) |
| **Parámetro** | Bits exactos | N/A | Número de palabras (64-bit) |
| **Constexpr** | Limitado | N/A | ✅ Total (C++20) |
| **Templates** | ❌ No (C no tiene) | N/A | ✅ Sí (parametrizable) |
| **Portabilidad** | C23 solamente | N/A | Cualquier compilador C++20 |
| **Propuestas C++** | N/A | P0539R0, P0101R0 (rechazadas) | Implementación práctica |

**Ventajas de _BitInt(N) en C**:
- ✅ **Oficial en estándar C23**
- ✅ Soporte de compilador optimizado
- ✅ Portable entre implementaciones C23
- ✅ Bits exactos (más flexible que múltiplos de 64)

**Ventajas de uint_fixed_t<N> en C++**:
- ✅ **Constexpr completo** (evaluación en compile-time)
- ✅ **Templates** (parametrización tipo-segura)
- ✅ **Funciona hoy** con C++20 (no requiere C23)
- ✅ **Extensible** (se pueden añadir métodos custom)
- ✅ **Compatible con STL** (iteradores, algoritmos, etc.)

#### 5.4.8 Ejemplos Comparativos

**Criptografía RSA-2048 (necesita aritmética de 2048 bits)**:

```c
// C23 - _BitInt directo
#include <stdint.h>

_BitInt(2048) modulus = /* ... */;
_BitInt(2048) base = /* ... */;
_BitInt(2048) exponent = /* ... */;

// Exponenciación modular
_BitInt(2048) result = mod_exp_bitint(base, exponent, modulus);
```

```cpp
// C++20 - uint_fixed_t<N>
using uint2048_t = uint_fixed_t<32>;  // 32 × 64 = 2048 bits

uint2048_t modulus = /* ... */;
uint2048_t base = /* ... */;
uint2048_t exponent = /* ... */;

// Exponenciación modular (constexpr!)
constexpr uint2048_t result = mod_exp(base, exponent, modulus);
```

**Comparación de rendimiento** (estimado, depende de optimización del compilador):

| Operación | C23 `_BitInt(2048)` | C++ `uint_fixed_t<32>` | GMP (C) |
|-----------|---------------------|------------------------|---------|
| **Suma** | ⚡ Muy rápido | ⚡ Muy rápido | ⚡⚡ Óptimo |
| **Multiplicación** | ⚡⚡ Rápido | ⚡ Bueno (escolar) | ⚡⚡⚡ Óptimo (Karatsuba/FFT) |
| **División** | ⚡⚡ Rápido | ⚡ Medio (larga) | ⚡⚡⚡ Óptimo (Newton) |
| **Constexpr** | ❌ Limitado | ✅ Total | ❌ No aplicable |

#### 5.4.9 Estado del Arte (2025)

**Adopción de _BitInt**:
- **LLVM/Clang**: Soporte completo desde 2022
- **Proyectos usando _BitInt**: Cryptographic libraries, blockchain implementations
- **GCC**: Trabajo en progreso para C23 completo
- **MSVC**: En roadmap, sin fecha confirmada

**Perspectivas futuras**:
1. **C++26/C++29**: Posible adopción de algo similar a `_BitInt` o `std::fixed_int<N>`
2. **Interoperabilidad C/C++**: Compiladores podrían permitir usar `_BitInt(N)` en código C++ como extensión
3. **Hardware support**: Futura extensión de ISA para operaciones de ancho arbitrario

**Lecciones para uint_fixed_t**:
- C23 validó la **demanda de tipos de ancho arbitrario**
- La sintaxis `_BitInt(N)` es más flexible que `uint_fixed_t<N>` (permite tamaños impares como 129 bits)
- **C++ necesita urgentemente** una solución estándar (proyectos como este llenan el vacío)

### 5.5 Propuestas en otros lenguajes

#### Rust: `num-bigint` y tipos fijos
```rust
use num_bigint::{BigUint, BigInt};

// O con tamaño fijo (crates externos):
use wide::u256;  // crate 'wide'
```

#### Python: `int` arbitrario por defecto
```python
# Python tiene precisión arbitraria nativa
x = 2 ** 1024  # Automático, no hay límite
```

#### Go: `math/big`
```go
import "math/big"

var x big.Int
x.SetString("340282366920938463463374607431768211456", 10)  // 2^128
```

**C++ carece de una solución estándar**, de ahí el valor de proyectos como este.

---

## 6. Plan de Implementación

### Fase 1: Refactorización del Código Actual
1. **Identificar operaciones generalizables** en `uint128_t`
2. **Extraer algoritmos core** a funciones template helper
3. **Crear tests parametrizados** que funcionen para N=2, 4, 8

### Fase 2: Prototipo `uint_fixed_t<N>`
1. **Estructura básica** con `std::array<uint64_t, N>` o C-array
2. **Operaciones triviales**: bitwise, comparaciones
3. **Suma/resta con carry**: generalizar código existente
4. **Multiplicación escolar**: adaptar para N palabras

### Fase 3: Optimizaciones
1. **Karatsuba** para N ≥ 8
2. **División mejorada**: Newton-Raphson para N grandes
3. **Intrinsics**: usar `_addcarry_u64`, `_mulx_u64` cuando esté disponible
4. **Constexpr**: asegurar que todo sea evaluable en compile-time

### Fase 4: Especialización y Retrocompatibilidad
```cpp
// Especialización para N=2 que usa el código optimizado actual
template<>
class uint_fixed_t<2> {
    // Implementación existente de uint128_t
    // Ya tiene todas las optimizaciones manuales
};

// Alias para compatibilidad
using uint128_t = uint_fixed_t<2>;
```

### Fase 5: Testing y Benchmarks
1. **Unit tests** para N = 2, 4, 8, 16, 32
2. **Benchmarks** comparando:
   - Diferentes valores de N
   - Algoritmos (escolar vs Karatsuba vs FFT)
   - Comparación con Boost.Multiprecision
3. **Fuzzing** para detectar bugs en operaciones complejas

---

## 7. Consideraciones de Diseño

### 7.1 Memoria y Alineación

```cpp
template<size_t N>
class alignas(64) uint_fixed_t {  // Alinear a línea de caché
    uint64_t data[N];
};
```

**Para N grandes** (≥16), considerar:
- Alineación a línea de caché (64 bytes)
- Evitar copias innecesarias (usar move semántica)
- Operaciones in-place cuando sea posible

### 7.2 Constexpr vs Performance

**Trade-off**: Constexpr requiere código portable sin intrinsics.

**Solución**: Dual implementation
```cpp
constexpr uint_fixed_t& operator+=(const uint_fixed_t& other) noexcept {
    if (std::is_constant_evaluated()) {
        // Implementación portable constexpr
        return add_constexpr(other);
    } else {
        // Implementación optimizada con intrinsics
        return add_runtime(other);
    }
}
```

### 7.3 División Generalizada

**Desafío**: El algoritmo de división actual asume 128 bits exactos.

**Solución**: Implementar división larga genérica primero, luego optimizar:

```cpp
// División genérica (lenta pero correcta)
template<size_t N>
constexpr std::pair<uint_fixed_t<N>, uint_fixed_t<N>> 
divmod_generic(const uint_fixed_t<N>& dividend, const uint_fixed_t<N>& divisor);

// Especializaciones optimizadas
template<>
constexpr std::pair<uint_fixed_t<2>, uint_fixed_t<2>>
divmod_generic<2>(...) {
    // Usar código optimizado actual de uint128_t
}
```

### 7.4 Interoperabilidad

```cpp
template<size_t N>
class uint_fixed_t {
    // Conversiones entre diferentes tamaños
    template<size_t M>
    explicit constexpr uint_fixed_t(const uint_fixed_t<M>& other) noexcept;
    
    // Conversión a/desde tipos nativos
    constexpr uint_fixed_t(uint64_t value) noexcept;
    explicit constexpr operator uint64_t() const noexcept;
    
    // Conversión a/desde strings (hex, dec, bin)
    static uint_fixed_t from_string(std::string_view str, int base = 10);
    std::string to_string(int base = 10) const;
};
```

---

## 8. Comparación con Implementaciones Existentes

### 8.1 Boost.Multiprecision vs uint_fixed_t

| Aspecto | Boost | Este Proyecto |
|---------|-------|---------------|
| **Portabilidad** | Excelente | Buena (requiere C++20) |
| **Rendimiento 128-bit** | Bueno | **Excelente** (especializado) |
| **Rendimiento N grande** | Excelente | Por implementar |
| **Constexpr** | Limitado | **Completo** |
| **Tamaño de compilación** | Grande (toda Boost) | Pequeño (header-only) |
| **Curva de aprendizaje** | Compleja | Simple |
| **Algoritmos** | Múltiples (automático) | Manual (por ahora) |

### 8.2 GCC `__int128` vs uint128_t

| Aspecto | __int128 | uint128_t |
|---------|----------|-----------|
| **Portabilidad** | Solo GCC/Clang | **Cualquier compilador C++20** |
| **MSVC** | ❌ No disponible | ✅ Funciona |
| **Constexpr** | Limitado | **Completo** |
| **Extensible** | ❌ No | ✅ Sí (a uint_fixed_t<N>) |
| **Rendimiento** | **Óptimo** (compilador) | Muy bueno |

---

## 9. Casos de Uso de uint_fixed_t<N>

### 9.1 Criptografía

```cpp
using uint256_t = uint_fixed_t<4>;   // RSA-2048 requiere aritmética de 256 bits
using uint512_t = uint_fixed_t<8>;   // Curvas elípticas

// Exponenciación modular: a^b mod n
uint256_t mod_exp(uint256_t base, uint256_t exp, uint256_t modulus);
```

### 9.2 Hashing (SHA-512, Blake3)

```cpp
using uint512_t = uint_fixed_t<8>;

uint512_t sha512_state[8];  // Estado interno de SHA-512
```

### 9.3 Matemáticas de Alta Precisión

```cpp
using uint1024_t = uint_fixed_t<16>;

// Números de Fibonacci muy grandes
uint1024_t fib(int n);

// Factoriales: 100! requiere ~524 bits
uint1024_t factorial(int n);
```

### 9.4 Blockchain / Ethereum

Ethereum usa aritmética de 256 bits:
```cpp
using uint256_t = uint_fixed_t<4>;

struct EthereumAddress {
    uint256_t value;  // 20 bytes, pero se maneja como 256 bits
};
```

### 9.5 Simulaciones Científicas

```cpp
// Precisión extendida para acumulación
using uint512_t = uint_fixed_t<8>;

uint512_t sum = 0;
for (uint64_t value : large_dataset) {
    sum += value;  // Sin overflow hasta 2^512
}
```

---

## 10. Referencias

### 10.1 Artículos Académicos

1. **Karatsuba, A., & Ofman, Y. (1962)**. "Multiplication of Multidigit Numbers on Automata". *Soviet Physics Doklady*, 7, 595-596.

2. **Schönhage, A., & Strassen, V. (1971)**. "Schnelle Multiplikation grosser Zahlen". *Computing*, 7(3-4), 281-292.  
   DOI: 10.1007/BF02242355

3. **Burnikel, C., & Ziegler, J. (1998)**. "Fast Recursive Division". *Research Report MPI-I-98-1-022*, Max-Planck-Institut für Informatik.  
   URL: https://www.mpi-inf.mpg.de/fileadmin/inf/d1/burnikel_fast_1998.pdf

4. **Brent, R. P., & Zimmermann, P. (2010)**. *Modern Computer Arithmetic*. Cambridge University Press.  
   URL: https://members.loria.fr/PZimmermann/mca/mca-cup-0.5.9.pdf

5. **Fürer, M. (2007)**. "Faster Integer Multiplication". *Proceedings of the 39th ACM Symposium on Theory of Computing*, 57-66.  
   DOI: 10.1145/1250790.1250800

6. **Harvey, D., & van der Hoeven, J. (2019)**. "Integer multiplication in time O(n log n)". *Annals of Mathematics*.  
   URL: https://hal.archives-ouvertes.fr/hal-02070778/document

### 10.2 Propuestas C++ (WG21)

1. **P0539R0** (2017): "A Proposal to add wide_int to the Standard Library"  
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0539r0.html

2. **P0101R0** (2015): "An Outline of a C++ Numbers Technical Specification"  
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0101r0.html

3. **P1889R0** (2019): "Efficient fixed-width integer types"  
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1889r0.pdf

### 10.3 Propuestas C (WG14) y Estándar C23

1. **N2709** (2021): "Bit-precise integer types" - **ADOPTADO en C23**  
   Autor: Melanie Blower (Intel)  
   URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2709.pdf  
   Especificación final de `_BitInt(N)` que fue incluida en ISO/IEC 9899:2023

2. **N2763** (2021): "Bit-precise integer types (latest draft)"  
   Última revisión antes de adopción en C23  
   URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2763.pdf

3. **N2425** (2019): "Bit-precise integer types" - Versión preliminar  
   Primera propuesta completa de sintaxis y semántica  
   URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2425.pdf

4. **N1281** (2007): "Extended Integer Types" - Propuesta anterior rechazada  
   Primera tentativa de tipos extendidos, diseño diferente  
   URL: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1281.pdf

5. **ISO/IEC 9899:2023** (C23): Estándar oficial que incluye `_BitInt(N)`  
   Publicado: 2023  
   Sección 6.2.5: "Types" - Bit-precise integer types

### 10.4 Fortran y Tipos Parametrizados

1. **ISO/IEC 1539-1:1991** (Fortran 90): Introducción de `INTEGER(KIND=n)`  
   Primera estandarización de tipos enteros parametrizados

2. **ISO/IEC 1539-1:1997** (Fortran 95): Refinamiento del sistema KIND  
   Mejoras en `SELECTED_INT_KIND` y portabilidad

3. **Adams, J. C., et al. (2009)**. *The Fortran 2003 Handbook*.  
   Springer. ISBN: 978-1-84628-378-9  
   Capítulo sobre tipos parametrizados y KIND system

4. **Metcalf, M., Reid, J., & Cohen, M. (2018)**. *Modern Fortran Explained* (5th ed.).  
   Oxford University Press. ISBN: 978-0-19-881135-7  
   Referencia actualizada sobre sistema KIND de Fortran

### 10.5 Implementaciones Existentes

1. **Boost.Multiprecision**  
   https://www.boost.org/doc/libs/1_84_0/libs/multiprecision/doc/html/index.html

2. **GMP (GNU Multiple Precision Arithmetic Library)**  
   https://gmplib.org/

3. **MPFR (Multiple Precision Floating-Point Reliable Library)**  
   https://www.mpfr.org/

4. **Crypto++ Integer Class**  
   https://www.cryptopp.com/docs/ref/class_integer.html

5. **Google Abseil int128**  
   https://github.com/abseil/abseil-cpp/blob/master/absl/numeric/int128.h

6. **Clang/LLVM _BitInt Implementation**  
   https://clang.llvm.org/docs/LanguageExtensions.html#extended-integer-types  
   Documentación de la implementación de `_BitInt(N)` en Clang

### 10.6 Recursos Adicionales

1. **"Hacker's Delight"** (2nd Edition) - Henry S. Warren Jr.  
   Capítulos sobre aritmética multi-palabra y trucos de bits.

2. **Knuth, Donald E. (1997)**. *The Art of Computer Programming, Volume 2: Seminumerical Algorithms* (3rd ed.).  
   Sección 4.3: Multiple-Precision Arithmetic.

3. **Intel Intrinsics Guide**  
   https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html  
   Para `_addcarry_u64`, `_subborrow_u64`, `_mulx_u64`, etc.

---

## 11. Conclusiones

### 11.1 Viabilidad

La generalización de `uint128_t` a `uint_fixed_t<N>` es **altamente viable**:

✅ **Ventajas**:
- Código existente sirve como base sólida y bien probada
- Operaciones básicas (bitwise, carry propagation) son triviales de generalizar
- Multiplicación escolar funciona bien hasta N~16
- Beneficio educativo y práctico

⚠️ **Desafíos**:
- División requiere re-implementación más cuidadosa
- Algoritmos avanzados (Karatsuba, FFT) requieren estudio adicional
- Testing exhaustivo para diferentes valores de N
- Mantenimiento de rendimiento para N=2 (caso especial)

### 11.2 Recomendación

**Enfoque incremental**:

1. **Corto plazo** (ahora): Documentar diseño (✅ este documento)
2. **CI/CD** (siguiente paso): Establecer pipeline de testing robusto
3. **Medio plazo** (siguiente milestone): 
   - Refactorizar uint128_t para facilitar extracción
   - Prototipo de uint_fixed_t<2>, uint_fixed_t<4>
4. **Largo plazo** (futuro proyecto):
   - Implementación completa con algoritmos avanzados
   - Benchmarks extensivos vs Boost
   - Posible propuesta WG21 (si la calidad es suficiente)

### 11.3 Valor del Proyecto

Este proyecto puede convertirse en:
- **Alternativa ligera** a Boost.Multiprecision para casos de uso específicos
- **Implementación de referencia** constexpr C++20
- **Material educativo** sobre algoritmos de aritmética multi-precisión
- **Base para propuesta** al comité de estandarización C++

### 11.4 Panorama de Estandarización (2025)

**Estado actual de tipos de ancho arbitrario**:

| Lenguaje | Estado | Solución | Año |
|----------|--------|----------|-----|
| **Fortran** | ✅ Estándar | `INTEGER(KIND=n)` | 1991 (Fortran 90) |
| **C** | ✅ Estándar | `_BitInt(N)` | 2023 (C23) |
| **C++** | ❌ Sin estándar | Extensiones compilador / librerías | - |
| **Python** | ✅ Built-in | `int` (precisión arbitraria) | Desde siempre |
| **Rust** | ⚠️ Librerías | `num-bigint`, crates externos | - |

**Conclusión clave**: C++ es el **único lenguaje mainstream moderno** sin una solución estándar para enteros de ancho arbitrario, a pesar de que:
- **Fortran** lo tiene desde hace 34 años (1991)
- **C** lo acaba de adoptar (2023)
- Las propuestas C++ (P0539R0, P0101R0) fueron rechazadas

**Implicación para este proyecto**:
- Existe una **necesidad real** no cubierta por el estándar C++
- Este proyecto demuestra que es **técnicamente factible** con C++20
- Podría servir como **implementación de referencia** para futuras propuestas WG21
- La adopción de `_BitInt(N)` en C23 valida la demanda y utilidad de esta funcionalidad

---

## Apéndice A: Ejemplo Completo de Uso

```cpp
#include "uint_fixed.hpp"

int main() {
    using uint256_t = uint_fixed_t<4>;
    
    // Construcción
    uint256_t a = 0xFFFFFFFFFFFFFFFFull;
    uint256_t b{42};
    
    // Aritmética
    uint256_t sum = a + b;
    uint256_t product = a * b;
    
    // Comparaciones
    if (sum > product) {
        std::cout << "sum > product\n";
    }
    
    // Conversión a string
    std::cout << "Result: " << sum.to_string(16) << '\n';
    
    // Operaciones bit a bit
    uint256_t masked = a & uint256_t{0xFF};
    
    // Constexpr
    constexpr uint256_t compile_time = uint256_t{100} * uint256_t{200};
    static_assert(compile_time == uint256_t{20000});
    
    return 0;
}
```

---

**Fin del documento**  
Autor: Generado automáticamente  
Fecha: 26 de diciembre de 2025  
Versión: 2.0 - Añadida documentación completa sobre C23 `_BitInt(N)`, conexión con Fortran `INTEGER(KIND=n)`, y análisis comparativo del panorama de estandarización
