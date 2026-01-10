# API Reference - int128_base_limits.hpp

> **Especialización de `nstd::numeric_limits` para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_limits.hpp"

namespace nstd {
    template<signedness S>
    struct numeric_limits<int128_base_t<S>>;
}
```

---

## Especialización para `uint128_t`

```cpp
template<>
struct numeric_limits<uint128_t> {
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool has_denorm = false;
    static constexpr bool has_denorm_loss = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;
    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;
    
    static constexpr int digits = 128;
    static constexpr int digits10 = 38;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    
    static consteval uint128_t min() noexcept;
    static consteval uint128_t max() noexcept;
    static consteval uint128_t lowest() noexcept;
    static consteval uint128_t epsilon() noexcept;
    static consteval uint128_t round_error() noexcept;
    static consteval uint128_t infinity() noexcept;
    static consteval uint128_t quiet_NaN() noexcept;
    static consteval uint128_t signaling_NaN() noexcept;
    static consteval uint128_t denorm_min() noexcept;
};
```

---

## Especialización para `int128_t`

```cpp
template<>
struct numeric_limits<int128_t> {
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool has_denorm = false;
    static constexpr bool has_denorm_loss = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = false;
    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;
    
    static constexpr int digits = 127;
    static constexpr int digits10 = 38;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    
    static consteval int128_t min() noexcept;
    static consteval int128_t max() noexcept;
    static consteval int128_t lowest() noexcept;
    static consteval int128_t epsilon() noexcept;
    static consteval int128_t round_error() noexcept;
    static consteval int128_t infinity() noexcept;
    static consteval int128_t quiet_NaN() noexcept;
    static consteval int128_t signaling_NaN() noexcept;
    static consteval int128_t denorm_min() noexcept;
};
```

---

## Funciones Miembro

### min

```cpp
static consteval uint128_t min() noexcept;  // uint128_t
static consteval int128_t min() noexcept;   // int128_t
```

**Retorna:** Valor mínimo representable.

| Tipo | Valor |
|------|-------|
| `uint128_t` | 0 |
| `int128_t` | -170141183460469231731687303715884105728 (-2¹²⁷) |

---

### max

```cpp
static consteval uint128_t max() noexcept;  // uint128_t
static consteval int128_t max() noexcept;   // int128_t
```

**Retorna:** Valor máximo representable.

| Tipo | Valor |
|------|-------|
| `uint128_t` | 340282366920938463463374607431768211455 (2¹²⁸-1) |
| `int128_t` | 170141183460469231731687303715884105727 (2¹²⁷-1) |

---

### lowest

```cpp
static consteval uint128_t lowest() noexcept;
static consteval int128_t lowest() noexcept;
```

**Retorna:** Igual que `min()` para tipos enteros.

---

### epsilon / round_error / infinity / quiet_NaN / signaling_NaN / denorm_min

```cpp
static consteval T epsilon() noexcept;        // Retorna 0
static consteval T round_error() noexcept;    // Retorna 0
static consteval T infinity() noexcept;       // Retorna 0
static consteval T quiet_NaN() noexcept;      // Retorna 0
static consteval T signaling_NaN() noexcept;  // Retorna 0
static consteval T denorm_min() noexcept;     // Retorna 0
```

**Nota:** Estas funciones retornan 0 porque no aplican a tipos enteros.

---

## Ejemplo

```cpp
#include "int128_base_limits.hpp"
#include <iostream>

int main() {
    using nstd::numeric_limits;
    using nstd::uint128_t;
    using nstd::int128_t;
    
    std::cout << "uint128_t digits: " << numeric_limits<uint128_t>::digits << "\n";
    std::cout << "int128_t digits:  " << numeric_limits<int128_t>::digits << "\n";
    
    std::cout << "uint128_t max: " << numeric_limits<uint128_t>::max().to_string() << "\n";
    std::cout << "int128_t min:  " << numeric_limits<int128_t>::min().to_string() << "\n";
    
    static_assert(numeric_limits<uint128_t>::is_integer);
    static_assert(!numeric_limits<int128_t>::is_modulo);  // signed no es modular
}
```

---

## Ver También

- [int128_base_tt.hpp](API_INT128_BASE_TT.md) - Tipo principal
- [int128_base_traits.hpp](API_INT128_BASE_TRAITS.md) - Type traits

---

**Licencia:** Boost Software License 1.0
