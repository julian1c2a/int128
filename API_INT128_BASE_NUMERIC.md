# API Reference - int128_base_numeric.hpp

> **Funciones numéricas estilo `<numeric>` para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_numeric.hpp"

namespace nstd {
    // Acumulación
    template<std::input_iterator It, signedness S>
    constexpr int128_base_t<S> accumulate(It first, It last, int128_base_t<S> init) noexcept;
    
    template<std::input_iterator It, signedness S, typename BinaryOp>
    constexpr int128_base_t<S> accumulate(It first, It last, int128_base_t<S> init, BinaryOp op);
    
    // Producto interno
    template<std::input_iterator It1, std::input_iterator It2, signedness S>
    constexpr int128_base_t<S> inner_product(It1 first1, It1 last1, It2 first2, int128_base_t<S> init) noexcept;
    
    // Sumas parciales
    template<std::input_iterator It, std::output_iterator<int128_base_t<signedness::unsigned_type>> OutIt>
    constexpr OutIt partial_sum(It first, It last, OutIt d_first);
    
    // Diferencias adyacentes
    template<std::input_iterator It, std::output_iterator<int128_base_t<signedness::unsigned_type>> OutIt>
    constexpr OutIt adjacent_difference(It first, It last, OutIt d_first);
    
    // Iota
    template<std::forward_iterator It, signedness S>
    constexpr void iota(It first, It last, int128_base_t<S> value);
    
    // Reduce
    template<std::input_iterator It, signedness S>
    constexpr int128_base_t<S> reduce(It first, It last, int128_base_t<S> init) noexcept;
    
    template<std::input_iterator It, signedness S, typename BinaryOp>
    constexpr int128_base_t<S> reduce(It first, It last, int128_base_t<S> init, BinaryOp op);
    
    // Transform reduce
    template<std::input_iterator It, signedness S, typename UnaryOp, typename BinaryOp>
    constexpr int128_base_t<S> transform_reduce(It first, It last, int128_base_t<S> init, 
                                                 BinaryOp reduce_op, UnaryOp transform_op);
    
    // Exclusive/Inclusive scan
    template<std::input_iterator It, std::output_iterator<int128_base_t<signedness::unsigned_type>> OutIt, signedness S>
    constexpr OutIt exclusive_scan(It first, It last, OutIt d_first, int128_base_t<S> init);
    
    template<std::input_iterator It, std::output_iterator<int128_base_t<signedness::unsigned_type>> OutIt>
    constexpr OutIt inclusive_scan(It first, It last, OutIt d_first);
}
```

---

## Funciones de Acumulación

### accumulate

```cpp
template<std::input_iterator It, signedness S>
constexpr int128_base_t<S> accumulate(It first, It last, int128_base_t<S> init) noexcept;
```

**Brief:** Suma todos los elementos de un rango.

**Parámetros:**

- `first`, `last` - Rango de entrada
- `init` - Valor inicial

**Retorna:** `init + *first + *(first+1) + ... + *(last-1)`

**Ejemplo:**

```cpp
std::vector<uint64_t> v{1, 2, 3, 4, 5};
const auto sum = nstd::accumulate(v.begin(), v.end(), uint128_t{0});
assert(sum == uint128_t{15});
```

---

### accumulate (con operación)

```cpp
template<std::input_iterator It, signedness S, typename BinaryOp>
constexpr int128_base_t<S> accumulate(It first, It last, int128_base_t<S> init, BinaryOp op);
```

**Brief:** Acumula elementos usando una operación personalizada.

**Ejemplo:**

```cpp
std::vector<uint128_t> v{2, 3, 4};
const auto product = nstd::accumulate(v.begin(), v.end(), uint128_t{1},
    [](uint128_t a, uint128_t b) { return a * b; });
assert(product == uint128_t{24});
```

---

## Producto Interno

### inner_product

```cpp
template<std::input_iterator It1, std::input_iterator It2, signedness S>
constexpr int128_base_t<S> inner_product(It1 first1, It1 last1, It2 first2, int128_base_t<S> init) noexcept;
```

**Brief:** Calcula el producto interno de dos rangos.

**Retorna:** `init + (*first1 * *first2) + (*(first1+1) * *(first2+1)) + ...`

**Ejemplo:**

```cpp
std::vector<uint64_t> a{1, 2, 3};
std::vector<uint64_t> b{4, 5, 6};
const auto dot = nstd::inner_product(a.begin(), a.end(), b.begin(), uint128_t{0});
// 1*4 + 2*5 + 3*6 = 32
assert(dot == uint128_t{32});
```

---

## Sumas Parciales

### partial_sum

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt>
constexpr OutIt partial_sum(It first, It last, OutIt d_first);
```

**Brief:** Genera sumas parciales acumulativas.

**Ejemplo:**

```cpp
std::vector<uint128_t> in{1, 2, 3, 4};
std::vector<uint128_t> out(4);
nstd::partial_sum(in.begin(), in.end(), out.begin());
// out = {1, 3, 6, 10}
```

---

## Diferencias Adyacentes

### adjacent_difference

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt>
constexpr OutIt adjacent_difference(It first, It last, OutIt d_first);
```

**Brief:** Calcula diferencias entre elementos consecutivos.

**Ejemplo:**

```cpp
std::vector<uint128_t> in{1, 4, 7, 11};
std::vector<uint128_t> out(4);
nstd::adjacent_difference(in.begin(), in.end(), out.begin());
// out = {1, 3, 3, 4}
```

---

## Iota

### iota

```cpp
template<std::forward_iterator It, signedness S>
constexpr void iota(It first, It last, int128_base_t<S> value);
```

**Brief:** Llena un rango con valores incrementales.

**Ejemplo:**

```cpp
std::vector<uint128_t> v(5);
nstd::iota(v.begin(), v.end(), uint128_t{10});
// v = {10, 11, 12, 13, 14}
```

---

## Reduce

### reduce

```cpp
template<std::input_iterator It, signedness S>
constexpr int128_base_t<S> reduce(It first, It last, int128_base_t<S> init) noexcept;
```

**Brief:** Reduce un rango a un solo valor (similar a accumulate pero permite reordenamiento).

---

### transform_reduce

```cpp
template<std::input_iterator It, signedness S, typename UnaryOp, typename BinaryOp>
constexpr int128_base_t<S> transform_reduce(It first, It last, int128_base_t<S> init,
                                             BinaryOp reduce_op, UnaryOp transform_op);
```

**Brief:** Transforma y reduce en una sola pasada.

**Ejemplo:**

```cpp
std::vector<uint64_t> v{1, 2, 3};
// Suma de cuadrados
const auto sum_sq = nstd::transform_reduce(v.begin(), v.end(), uint128_t{0},
    std::plus<>{},
    [](uint64_t x) { return uint128_t{x * x}; });
assert(sum_sq == uint128_t{14});  // 1 + 4 + 9
```

---

## Scans

### exclusive_scan

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt, signedness S>
constexpr OutIt exclusive_scan(It first, It last, OutIt d_first, int128_base_t<S> init);
```

**Brief:** Scan exclusivo (cada elemento es la suma de todos los anteriores, excluyéndose).

---

### inclusive_scan

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt>
constexpr OutIt inclusive_scan(It first, It last, OutIt d_first);
```

**Brief:** Scan inclusivo (cada elemento es la suma incluyéndose a sí mismo).

---

## Ver También

- [int128_base_algorithm.hpp](API_INT128_BASE_ALGORITHM.md) - Algoritmos
- [int128_base_cmath.hpp](API_INT128_BASE_CMATH.md) - Funciones matemáticas

---

**Licencia:** Boost Software License 1.0
