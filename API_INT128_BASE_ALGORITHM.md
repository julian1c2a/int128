# API Reference - int128_base_algorithm.hpp

> **Algoritmos STL-compatible para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_algorithm.hpp"

namespace nstd {
    // Búsqueda
    template<std::forward_iterator It, signedness S>
    constexpr It find(It first, It last, int128_base_t<S> value);
    
    template<std::forward_iterator It, signedness S>
    constexpr It binary_search_it(It first, It last, int128_base_t<S> value);
    
    template<std::forward_iterator It, signedness S>
    constexpr bool binary_search(It first, It last, int128_base_t<S> value);
    
    // Conteo
    template<std::input_iterator It, signedness S>
    constexpr std::iter_difference_t<It> count(It first, It last, int128_base_t<S> value);
    
    // Min/Max en rangos
    template<std::forward_iterator It>
    constexpr It min_element(It first, It last);
    
    template<std::forward_iterator It>
    constexpr It max_element(It first, It last);
    
    template<std::forward_iterator It>
    constexpr std::pair<It, It> minmax_element(It first, It last);
    
    // Ordenamiento
    template<std::random_access_iterator It>
    constexpr void sort(It first, It last);
    
    template<std::random_access_iterator It, typename Compare>
    constexpr void sort(It first, It last, Compare comp);
    
    template<std::bidirectional_iterator It>
    constexpr void reverse(It first, It last);
    
    // Verificación de orden
    template<std::forward_iterator It>
    constexpr bool is_sorted(It first, It last);
    
    // Transformación
    template<std::input_iterator It, std::output_iterator<...> OutIt, typename UnaryOp>
    constexpr OutIt transform(It first, It last, OutIt d_first, UnaryOp op);
    
    // Generación
    template<std::forward_iterator It, typename Generator>
    constexpr void generate(It first, It last, Generator gen);
    
    template<std::output_iterator<...> It, signedness S>
    constexpr It fill_n(It first, std::size_t count, int128_base_t<S> value);
    
    // Copia condicional
    template<std::input_iterator It, std::output_iterator<...> OutIt, typename Pred>
    constexpr OutIt copy_if(It first, It last, OutIt d_first, Pred pred);
    
    // Unique
    template<std::forward_iterator It>
    constexpr It unique(It first, It last);
    
    // Secuencias especiales
    template<signedness S>
    std::vector<int128_base_t<S>> generate_range(int128_base_t<S> start, int128_base_t<S> end, 
                                                  int128_base_t<S> step = int128_base_t<S>{1});
    
    template<signedness S>
    std::vector<int128_base_t<S>> generate_powers_of_2(int start_exp, int end_exp);
}
```

---

## Funciones de Búsqueda

### find

```cpp
template<std::forward_iterator It, signedness S>
constexpr It find(It first, It last, int128_base_t<S> value);
```

**Brief:** Búsqueda lineal de un valor.

**Retorna:** Iterador al primer elemento igual a `value`, o `last` si no se encuentra.

---

### binary_search_it

```cpp
template<std::forward_iterator It, signedness S>
constexpr It binary_search_it(It first, It last, int128_base_t<S> value);
```

**Brief:** Búsqueda binaria que retorna iterador.

**Requisito:** El rango debe estar ordenado.

**Retorna:** Iterador al elemento encontrado, o `last` si no existe.

---

### binary_search

```cpp
template<std::forward_iterator It, signedness S>
constexpr bool binary_search(It first, It last, int128_base_t<S> value);
```

**Brief:** Búsqueda binaria que retorna bool.

---

## Funciones de Conteo

### count

```cpp
template<std::input_iterator It, signedness S>
constexpr std::iter_difference_t<It> count(It first, It last, int128_base_t<S> value);
```

**Brief:** Cuenta ocurrencias de un valor.

**Ejemplo:**

```cpp
std::vector<uint128_t> v{1, 2, 2, 3, 2, 4};
assert(nstd::count(v.begin(), v.end(), uint128_t{2}) == 3);
```

---

## Funciones Min/Max

### min_element / max_element

```cpp
template<std::forward_iterator It>
constexpr It min_element(It first, It last);

template<std::forward_iterator It>
constexpr It max_element(It first, It last);
```

**Brief:** Encuentra el elemento mínimo/máximo en un rango.

---

### minmax_element

```cpp
template<std::forward_iterator It>
constexpr std::pair<It, It> minmax_element(It first, It last);
```

**Brief:** Encuentra ambos en una sola pasada.

**Retorna:** `{iterador_al_min, iterador_al_max}`

---

## Funciones de Ordenamiento

### sort

```cpp
template<std::random_access_iterator It>
constexpr void sort(It first, It last);

template<std::random_access_iterator It, typename Compare>
constexpr void sort(It first, It last, Compare comp);
```

**Brief:** Ordena elementos (quicksort/introsort).

**Ejemplo:**

```cpp
std::vector<uint128_t> v{5, 2, 8, 1, 9};
nstd::sort(v.begin(), v.end());
// v = {1, 2, 5, 8, 9}

// Orden descendente
nstd::sort(v.begin(), v.end(), std::greater<>{});
```

---

### reverse

```cpp
template<std::bidirectional_iterator It>
constexpr void reverse(It first, It last);
```

**Brief:** Invierte el orden de los elementos.

---

### is_sorted

```cpp
template<std::forward_iterator It>
constexpr bool is_sorted(It first, It last);
```

**Brief:** Verifica si el rango está ordenado.

---

## Funciones de Transformación

### transform

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt, typename UnaryOp>
constexpr OutIt transform(It first, It last, OutIt d_first, UnaryOp op);
```

**Brief:** Aplica una operación a cada elemento.

**Ejemplo:**

```cpp
std::vector<uint128_t> in{1, 2, 3};
std::vector<uint128_t> out(3);
nstd::transform(in.begin(), in.end(), out.begin(), 
    [](uint128_t x) { return x * x; });
// out = {1, 4, 9}
```

---

## Funciones de Generación

### generate

```cpp
template<std::forward_iterator It, typename Generator>
constexpr void generate(It first, It last, Generator gen);
```

**Brief:** Llena un rango con valores generados.

---

### fill_n

```cpp
template<std::output_iterator<...> It, signedness S>
constexpr It fill_n(It first, std::size_t count, int128_base_t<S> value);
```

**Brief:** Llena N elementos con un valor.

---

### generate_range

```cpp
template<signedness S>
std::vector<int128_base_t<S>> generate_range(int128_base_t<S> start, int128_base_t<S> end,
                                              int128_base_t<S> step = int128_base_t<S>{1});
```

**Brief:** Genera una secuencia aritmética [start, end).

**Ejemplo:**

```cpp
auto seq = nstd::generate_range(uint128_t{0}, uint128_t{10}, uint128_t{2});
// seq = {0, 2, 4, 6, 8}
```

---

### generate_powers_of_2

```cpp
template<signedness S>
std::vector<int128_base_t<S>> generate_powers_of_2(int start_exp, int end_exp);
```

**Brief:** Genera potencias de 2.

**Ejemplo:**

```cpp
auto powers = nstd::generate_powers_of_2<signedness::unsigned_type>(0, 5);
// powers = {1, 2, 4, 8, 16}
```

---

## Funciones de Filtrado

### copy_if

```cpp
template<std::input_iterator It, std::output_iterator<...> OutIt, typename Pred>
constexpr OutIt copy_if(It first, It last, OutIt d_first, Pred pred);
```

**Brief:** Copia elementos que satisfacen un predicado.

---

### unique

```cpp
template<std::forward_iterator It>
constexpr It unique(It first, It last);
```

**Brief:** Elimina duplicados consecutivos.

**Retorna:** Iterador al nuevo final lógico.

---

## Ver También

- [int128_base_numeric.hpp](API_INT128_BASE_NUMERIC.md) - Operaciones numéricas
- [int128_base_cmath.hpp](API_INT128_BASE_CMATH.md) - Funciones matemáticas

---

**Licencia:** Boost Software License 1.0
