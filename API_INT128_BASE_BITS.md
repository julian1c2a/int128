# API Reference - int128_base_bits.hpp

> **Operaciones de bits para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_bits.hpp"

namespace nstd {
    // Conteo de bits
    template<signedness S>
    constexpr int popcount(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int countl_zero(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int countl_one(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int countr_zero(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int countr_one(int128_base_t<S> x) noexcept;
    
    // Propiedades de bits
    template<signedness S>
    constexpr int bit_width(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr bool has_single_bit(int128_base_t<S> x) noexcept;
    
    // Potencias de 2
    template<signedness S>
    constexpr int128_base_t<S> bit_ceil(int128_base_t<S> x) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> bit_floor(int128_base_t<S> x) noexcept;
    
    // Rotaciones
    template<signedness S>
    constexpr int128_base_t<S> rotl(int128_base_t<S> x, int s) noexcept;
    
    template<signedness S>
    constexpr int128_base_t<S> rotr(int128_base_t<S> x, int s) noexcept;
    
    // Byte swap
    template<signedness S>
    constexpr int128_base_t<S> byteswap(int128_base_t<S> x) noexcept;
}
```

---

## Funciones de Conteo

### popcount

```cpp
template<signedness S>
constexpr int popcount(int128_base_t<S> x) noexcept;
```

**Brief:** Cuenta el número de bits en 1.

**Parámetros:**

- `x` - Valor a examinar

**Retorna:** Número de bits en 1 (0-128).

**Ejemplo:**

```cpp
const uint128_t val{0b11110000};
assert(nstd::popcount(val) == 4);
```

---

### countl_zero

```cpp
template<signedness S>
constexpr int countl_zero(int128_base_t<S> x) noexcept;
```

**Brief:** Cuenta ceros consecutivos desde el bit más significativo.

**Parámetros:**

- `x` - Valor a examinar

**Retorna:** Número de ceros a la izquierda (0-128).

**Ejemplo:**

```cpp
const uint128_t val{1};  // Solo bit 0 activo
assert(nstd::countl_zero(val) == 127);

const uint128_t zero{0};
assert(nstd::countl_zero(zero) == 128);
```

---

### countl_one

```cpp
template<signedness S>
constexpr int countl_one(int128_base_t<S> x) noexcept;
```

**Brief:** Cuenta unos consecutivos desde el bit más significativo.

**Parámetros:**

- `x` - Valor a examinar

**Retorna:** Número de unos a la izquierda (0-128).

---

### countr_zero

```cpp
template<signedness S>
constexpr int countr_zero(int128_base_t<S> x) noexcept;
```

**Brief:** Cuenta ceros consecutivos desde el bit menos significativo.

**Parámetros:**

- `x` - Valor a examinar

**Retorna:** Número de ceros a la derecha (0-128).

**Ejemplo:**

```cpp
const uint128_t val{0b1000};  // Bit 3 activo
assert(nstd::countr_zero(val) == 3);
```

---

### countr_one

```cpp
template<signedness S>
constexpr int countr_one(int128_base_t<S> x) noexcept;
```

**Brief:** Cuenta unos consecutivos desde el bit menos significativo.

---

## Propiedades de Bits

### bit_width

```cpp
template<signedness S>
constexpr int bit_width(int128_base_t<S> x) noexcept;
```

**Brief:** Número mínimo de bits necesarios para representar el valor.

**Retorna:** `1 + floor(log2(x))` si `x > 0`, o `0` si `x == 0`.

**Ejemplo:**

```cpp
const uint128_t val{255};   // 0xFF
assert(nstd::bit_width(val) == 8);

const uint128_t val2{256};  // 0x100
assert(nstd::bit_width(val2) == 9);
```

---

### has_single_bit

```cpp
template<signedness S>
constexpr bool has_single_bit(int128_base_t<S> x) noexcept;
```

**Brief:** Verifica si el valor es una potencia de 2.

**Retorna:** `true` si exactamente un bit está activo.

**Ejemplo:**

```cpp
assert(nstd::has_single_bit(uint128_t{64}));   // true: 2^6
assert(!nstd::has_single_bit(uint128_t{65}));  // false
```

---

## Potencias de 2

### bit_ceil

```cpp
template<signedness S>
constexpr int128_base_t<S> bit_ceil(int128_base_t<S> x) noexcept;
```

**Brief:** Menor potencia de 2 ≥ x.

**Ejemplo:**

```cpp
assert(nstd::bit_ceil(uint128_t{5}) == uint128_t{8});
assert(nstd::bit_ceil(uint128_t{8}) == uint128_t{8});
```

---

### bit_floor

```cpp
template<signedness S>
constexpr int128_base_t<S> bit_floor(int128_base_t<S> x) noexcept;
```

**Brief:** Mayor potencia de 2 ≤ x.

**Ejemplo:**

```cpp
assert(nstd::bit_floor(uint128_t{5}) == uint128_t{4});
assert(nstd::bit_floor(uint128_t{8}) == uint128_t{8});
```

---

## Rotaciones

### rotl

```cpp
template<signedness S>
constexpr int128_base_t<S> rotl(int128_base_t<S> x, int s) noexcept;
```

**Brief:** Rotación circular a la izquierda.

**Parámetros:**

- `x` - Valor a rotar
- `s` - Posiciones a rotar (puede ser negativo)

**Ejemplo:**

```cpp
const uint128_t val{0x80000000'00000000'00000000'00000001_u128};
const auto rotated = nstd::rotl(val, 1);  // MSB pasa a LSB
```

---

### rotr

```cpp
template<signedness S>
constexpr int128_base_t<S> rotr(int128_base_t<S> x, int s) noexcept;
```

**Brief:** Rotación circular a la derecha.

---

## Operaciones de Bytes

### byteswap

```cpp
template<signedness S>
constexpr int128_base_t<S> byteswap(int128_base_t<S> x) noexcept;
```

**Brief:** Invierte el orden de los bytes (endianness swap).

**Ejemplo:**

```cpp
const uint128_t val{0x0102030405060708'090A0B0C0D0E0F10_u128};
const auto swapped = nstd::byteswap(val);
// swapped == 0x100F0E0D0C0B0A09'0807060504030201
```

---

## Ver También

- [int128_base_tt.hpp](API_INT128_BASE_TT.md) - Tipo principal
- [int128_base_numeric.hpp](API_INT128_BASE_NUMERIC.md) - Funciones numéricas

---

**Licencia:** Boost Software License 1.0
