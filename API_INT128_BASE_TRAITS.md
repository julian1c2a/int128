# API Reference - int128_base_traits.hpp

> **Type traits para tipos de 128 bits**
> **Namespace:** `std` (especializaciones) y `nstd`

---

## Synopsis

```cpp
#include "int128_base_traits.hpp"

// Especializaciones en namespace std
namespace std {
    template<nstd::signedness S>
    struct is_integral<nstd::int128_base_t<S>> : std::true_type {};
    
    template<nstd::signedness S>
    struct is_arithmetic<nstd::int128_base_t<S>> : std::true_type {};
    
    template<>
    struct is_signed<nstd::uint128_t> : std::false_type {};
    
    template<>
    struct is_signed<nstd::int128_t> : std::true_type {};
    
    template<>
    struct is_unsigned<nstd::uint128_t> : std::true_type {};
    
    template<>
    struct is_unsigned<nstd::int128_t> : std::false_type {};
    
    template<>
    struct make_signed<nstd::uint128_t> { using type = nstd::int128_t; };
    
    template<>
    struct make_signed<nstd::int128_t> { using type = nstd::int128_t; };
    
    template<>
    struct make_unsigned<nstd::uint128_t> { using type = nstd::uint128_t; };
    
    template<>
    struct make_unsigned<nstd::int128_t> { using type = nstd::uint128_t; };
    
    // common_type especializaciones
    template<nstd::signedness S, typename T>
    struct common_type<nstd::int128_base_t<S>, T>;
    
    // hash especialización
    template<nstd::signedness S>
    struct hash<nstd::int128_base_t<S>>;
}
```

---

## Especializaciones de Type Traits

### is_integral

```cpp
static_assert(std::is_integral_v<nstd::uint128_t>);  // true
static_assert(std::is_integral_v<nstd::int128_t>);   // true
```

**Brief:** Ambos tipos de 128 bits son integrales.

---

### is_arithmetic

```cpp
static_assert(std::is_arithmetic_v<nstd::uint128_t>);  // true
static_assert(std::is_arithmetic_v<nstd::int128_t>);   // true
```

---

### is_signed / is_unsigned

```cpp
static_assert(!std::is_signed_v<nstd::uint128_t>);   // uint128 NO es signed
static_assert(std::is_signed_v<nstd::int128_t>);     // int128 ES signed

static_assert(std::is_unsigned_v<nstd::uint128_t>);  // uint128 ES unsigned
static_assert(!std::is_unsigned_v<nstd::int128_t>);  // int128 NO es unsigned
```

---

### make_signed / make_unsigned

```cpp
using S1 = std::make_signed_t<nstd::uint128_t>;   // nstd::int128_t
using S2 = std::make_signed_t<nstd::int128_t>;    // nstd::int128_t

using U1 = std::make_unsigned_t<nstd::uint128_t>; // nstd::uint128_t
using U2 = std::make_unsigned_t<nstd::int128_t>;  // nstd::uint128_t
```

**Ejemplo:**

```cpp
template<typename T>
auto to_signed(T val) {
    return static_cast<std::make_signed_t<T>>(val);
}

nstd::uint128_t u{42};
nstd::int128_t s = to_signed(u);  // Funciona
```

---

## common_type Especializaciones

```cpp
// int128 con tipos builtin
using CT1 = std::common_type_t<nstd::uint128_t, uint64_t>;  // nstd::uint128_t
using CT2 = std::common_type_t<nstd::int128_t, int64_t>;    // nstd::int128_t
using CT3 = std::common_type_t<nstd::uint128_t, int>;       // nstd::uint128_t

// Entre ambos tipos de 128 bits
using CT4 = std::common_type_t<nstd::uint128_t, nstd::int128_t>;  // nstd::int128_t
```

**Regla:** `int128_t` es el tipo común entre signed y unsigned de 128 bits.

---

## Hash Especialización

```cpp
std::hash<nstd::uint128_t> hasher;
nstd::uint128_t val{42};
std::size_t h = hasher(val);
```

**Brief:** Permite usar tipos de 128 bits como claves en `std::unordered_map` y `std::unordered_set`.

**Ejemplo:**

```cpp
#include <unordered_map>
#include "int128_base_traits.hpp"

std::unordered_map<nstd::uint128_t, std::string> map;
map[nstd::uint128_t{1}] = "one";
map[nstd::uint128_t{2}] = "two";

std::unordered_set<nstd::int128_t> set;
set.insert(nstd::int128_t{-42});
```

---

## Verificaciones Estáticas

El header incluye verificaciones automáticas:

```cpp
// Type traits básicos
static_assert(std::is_integral_v<uint128_t>);
static_assert(std::is_integral_v<int128_t>);
static_assert(std::is_arithmetic_v<uint128_t>);
static_assert(std::is_arithmetic_v<int128_t>);

// Signedness
static_assert(!std::is_signed_v<uint128_t>);
static_assert(std::is_signed_v<int128_t>);
static_assert(std::is_unsigned_v<uint128_t>);
static_assert(!std::is_unsigned_v<int128_t>);

// Make signed/unsigned
static_assert(std::same_as<std::make_signed_t<uint128_t>, int128_t>);
static_assert(std::same_as<std::make_unsigned_t<int128_t>, uint128_t>);
```

---

## Ver También

- [int128_base_concepts.hpp](API_INT128_BASE_CONCEPTS.md) - Conceptos C++20
- [type_traits.hpp](API_TYPE_TRAITS.md) - Traits auxiliares

---

**Licencia:** Boost Software License 1.0
