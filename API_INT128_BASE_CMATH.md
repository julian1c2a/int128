# API Reference - int128_base_cmath.hpp

> **Funciones matemáticas para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_cmath.hpp"

namespace nstd {
    // GCD y LCM
    template<signedness S>
    constexpr int128_base_t<S> gcd(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> lcm(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    // Exponenciación y raíces
    template<signedness S>
    constexpr int128_base_t<S> pow(int128_base_t<S> base, unsigned exp) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> sqrt(int128_base_t<S> x) noexcept;
    
    // Valor absoluto y signo
    template<signedness S>
    constexpr int128_base_t<S> abs(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int sign(int128_base_t<S> x) noexcept;
    
    // Comparación y clamp
    template<signedness S>
    constexpr int128_base_t<S> min(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> max(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> clamp(int128_base_t<S> v, int128_base_t<S> lo, int128_base_t<S> hi) noexcept;
    
    // Punto medio y división
    template<signedness S>
    constexpr int128_base_t<S> midpoint(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    template<signedness S>
    constexpr std::pair<int128_base_t<S>, int128_base_t<S>> 
    divmod(int128_base_t<S> a, int128_base_t<S> b) noexcept;
    
    // Coeficientes de Bézout
    template<signedness S>
    struct bezout_result {
        int128_base_t<S> gcd;
        int128_base_t<signedness::signed_type> x;
        int128_base_t<signedness::signed_type> y;
    };
    
    template<signedness S>
    constexpr bezout_result<S> bezout_coeffs(int128_base_t<S> a, int128_base_t<S> b) noexcept;
}
```

---

## Funciones GCD/LCM

### gcd

```cpp
template<signedness S>
constexpr int128_base_t<S> gcd(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** Calcula el máximo común divisor usando el algoritmo binario de Stein.

**Parámetros:**

- `a`, `b` - Valores para calcular GCD

**Retorna:** Máximo común divisor de `|a|` y `|b|`.

**Ejemplo:**

```cpp
const uint128_t a{48};
const uint128_t b{18};
assert(nstd::gcd(a, b) == uint128_t{6});
```

---

### lcm

```cpp
template<signedness S>
constexpr int128_base_t<S> lcm(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** Calcula el mínimo común múltiplo.

**Retorna:** `|a * b| / gcd(a, b)` o 0 si alguno es 0.

**Ejemplo:**

```cpp
const uint128_t a{12};
const uint128_t b{18};
assert(nstd::lcm(a, b) == uint128_t{36});
```

---

## Exponenciación y Raíces

### pow

```cpp
template<signedness S>
constexpr int128_base_t<S> pow(int128_base_t<S> base, unsigned exp) noexcept;
```

**Brief:** Exponenciación binaria (square-and-multiply).

**Parámetros:**

- `base` - Base de la potencia
- `exp` - Exponente (unsigned)

**Retorna:** `base^exp`

**Ejemplo:**

```cpp
const uint128_t base{2};
assert(nstd::pow(base, 64) == (uint128_t{1} << 64));
```

---

### sqrt

```cpp
template<signedness S>
constexpr int128_base_t<S> sqrt(int128_base_t<S> x) noexcept;
```

**Brief:** Raíz cuadrada entera usando Newton-Raphson.

**Retorna:** `floor(sqrt(x))` para `x >= 0`, o 0 para valores negativos.

**Ejemplo:**

```cpp
const uint128_t val{100};
assert(nstd::sqrt(val) == uint128_t{10});

const uint128_t val2{101};
assert(nstd::sqrt(val2) == uint128_t{10});  // floor
```

---

## Valor Absoluto y Signo

### abs

```cpp
template<signedness S>
constexpr int128_base_t<S> abs(int128_base_t<S> x) noexcept;
```

**Brief:** Valor absoluto.

**Retorna:** `|x|` (para unsigned, retorna el mismo valor).

---

### sign

```cpp
template<signedness S>
constexpr int sign(int128_base_t<S> x) noexcept;
```

**Brief:** Función signo.

**Retorna:**

- `-1` si `x < 0`
- `0` si `x == 0`
- `1` si `x > 0`

---

## Comparación y Clamp

### min / max

```cpp
template<signedness S>
constexpr int128_base_t<S> min(int128_base_t<S> a, int128_base_t<S> b) noexcept;

template<signedness S>
constexpr int128_base_t<S> max(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** Retorna el menor/mayor de dos valores.

---

### clamp

```cpp
template<signedness S>
constexpr int128_base_t<S> clamp(int128_base_t<S> v, int128_base_t<S> lo, int128_base_t<S> hi) noexcept;
```

**Brief:** Limita un valor a un rango.

**Retorna:** `v` si `lo <= v <= hi`, `lo` si `v < lo`, `hi` si `v > hi`.

---

## Punto Medio y División

### midpoint

```cpp
template<signedness S>
constexpr int128_base_t<S> midpoint(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** Punto medio sin overflow.

**Retorna:** `(a + b) / 2` calculado sin riesgo de overflow.

---

### divmod

```cpp
template<signedness S>
constexpr std::pair<int128_base_t<S>, int128_base_t<S>> 
divmod(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** División con resto en una operación.

**Retorna:** `{cociente, resto}`

---

## Coeficientes de Bézout

### bezout_coeffs

```cpp
template<signedness S>
constexpr bezout_result<S> bezout_coeffs(int128_base_t<S> a, int128_base_t<S> b) noexcept;
```

**Brief:** Calcula los coeficientes de Bézout usando el algoritmo extendido de Euclides.

**Retorna:** Estructura con `{gcd, x, y}` tal que `a*x + b*y = gcd(a,b)`.

**Ejemplo:**

```cpp
const auto [g, x, y] = nstd::bezout_coeffs(uint128_t{240}, uint128_t{46});
// g = 2, x = -9, y = 47
// 240 * (-9) + 46 * 47 = 2 ✓
```

---

## Ver También

- [int128_base_bits.hpp](API_INT128_BASE_BITS.md) - Operaciones de bits
- [int128_base_numeric.hpp](API_INT128_BASE_NUMERIC.md) - Funciones numéricas adicionales

---

**Licencia:** Boost Software License 1.0
