# API Reference - int128_base_concepts.hpp

> **Conceptos C++20 para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_concepts.hpp"

namespace nstd {
    // Conceptos básicos
    template<typename T>
    concept int128_type = /* ... */;
    
    template<typename T>
    concept uint128_type = /* ... */;
    
    template<typename T>
    concept signed_int128_type = /* ... */;
    
    // Conceptos de conversión
    template<typename T>
    concept int128_convertible = /* ... */;
    
    template<typename T>
    concept int128_compatible = /* ... */;
    
    // Conceptos bitwise
    template<typename T>
    concept int128_bitwise_compatible = /* ... */;
    
    template<typename T>
    concept valid_shift_type = /* ... */;
}
```

---

## Conceptos Básicos

### int128_type

```cpp
template<typename T>
concept int128_type = std::same_as<std::remove_cv_t<T>, uint128_t> ||
                      std::same_as<std::remove_cv_t<T>, int128_t>;
```

**Brief:** Verifica si T es cualquier tipo de 128 bits (signed o unsigned).

**Ejemplo:**

```cpp
static_assert(nstd::int128_type<uint128_t>);
static_assert(nstd::int128_type<int128_t>);
static_assert(nstd::int128_type<const uint128_t>);
static_assert(!nstd::int128_type<uint64_t>);
```

---

### uint128_type

```cpp
template<typename T>
concept uint128_type = std::same_as<std::remove_cv_t<T>, uint128_t>;
```

**Brief:** Verifica si T es específicamente uint128_t.

---

### signed_int128_type

```cpp
template<typename T>
concept signed_int128_type = std::same_as<std::remove_cv_t<T>, int128_t>;
```

**Brief:** Verifica si T es específicamente int128_t.

---

## Conceptos de Conversión

### int128_convertible

```cpp
template<typename T>
concept int128_convertible = 
    integral_builtin<T> ||
    std::same_as<std::remove_cv_t<T>, uint128_t> ||
    std::same_as<std::remove_cv_t<T>, int128_t>;
```

**Brief:** Tipos que pueden convertirse a/desde tipos de 128 bits.

**Incluye:**

- Todos los tipos integrales built-in (int, uint64_t, etc.)
- uint128_t
- int128_t

---

### int128_compatible

```cpp
template<typename T>
concept int128_compatible = 
    int128_convertible<T> ||
    floating_point_builtin<T>;
```

**Brief:** Tipos compatibles con operaciones de 128 bits.

**Incluye:**

- Todo lo de int128_convertible
- Tipos de punto flotante (float, double, long double)

---

## Conceptos Bitwise

### int128_bitwise_compatible

```cpp
template<typename T>
concept int128_bitwise_compatible = 
    integral_builtin<T> || int128_type<T>;
```

**Brief:** Tipos válidos para operaciones bitwise con int128.

---

### valid_shift_type

```cpp
template<typename T>
concept valid_shift_type = 
    std::integral<T> && !std::same_as<std::remove_cv_t<T>, bool>;
```

**Brief:** Tipos válidos para operaciones de shift (<<, >>).

**Excluye:** `bool` (para evitar conversiones implícitas problemáticas).

---

## Verificaciones Automáticas

El header incluye `static_assert` que verifican la integración con la STL:

```cpp
// Verificaciones de conceptos estándar
static_assert(std::integral<uint128_t>);
static_assert(std::integral<int128_t>);
static_assert(std::unsigned_integral<uint128_t>);
static_assert(std::signed_integral<int128_t>);
static_assert(std::regular<uint128_t>);
static_assert(std::regular<int128_t>);
static_assert(std::totally_ordered<uint128_t>);
static_assert(std::totally_ordered<int128_t>);

// Verificaciones de conceptos personalizados
static_assert(int128_type<uint128_t>);
static_assert(int128_type<int128_t>);
static_assert(uint128_type<uint128_t>);
static_assert(!uint128_type<int128_t>);
static_assert(signed_int128_type<int128_t>);
static_assert(!signed_int128_type<uint128_t>);
```

---

## Uso en Templates

```cpp
// Función que acepta cualquier tipo de 128 bits
template<nstd::int128_type T>
constexpr T double_value(T x) {
    return x * T{2};
}

// Función solo para uint128_t
template<nstd::uint128_type T>
constexpr T square(T x) {
    return x * x;
}

// Función que acepta cualquier tipo compatible
template<nstd::int128_compatible T>
constexpr uint128_t to_uint128(T x) {
    return uint128_t{x};
}
```

---

## Ver También

- [int128_base_traits.hpp](API_INT128_BASE_TRAITS.md) - Type traits
- [type_traits.hpp](API_TYPE_TRAITS.md) - Traits auxiliares

---

**Licencia:** Boost Software License 1.0
