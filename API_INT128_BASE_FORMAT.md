# API Reference - int128_base_format.hpp

> **Funciones de formateo avanzado para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_format.hpp"

namespace nstd {
    // Formateo con flags
    template<signedness S>
    std::string format(int128_base_t<S> value, 
                       std::ios_base::fmtflags flags = std::ios_base::dec,
                       std::streamsize width = 0,
                       char fill = ' ');
    
    // Formateo específico por base
    template<signedness S>
    std::string hex(int128_base_t<S> value, bool uppercase = false, bool with_prefix = false);
    
    template<signedness S>
    std::string oct(int128_base_t<S> value, bool with_prefix = false);
    
    template<signedness S>
    std::string dec(int128_base_t<S> value);
    
    template<signedness S>
    std::string dec_signed(int128_base_t<S> value);
    
    template<signedness S>
    std::string bin(int128_base_t<S> value, bool with_prefix = false);
    
    // Con separadores
    template<signedness S>
    std::string with_separators(int128_base_t<S> value, char separator = '\'', int group_size = 3);
    
    // Imitar iostream
    template<signedness S>
    std::string format_like_iostream(int128_base_t<S> value, const std::ostream& os);
}
```

---

## Funciones de Formateo

### format

```cpp
template<signedness S>
std::string format(int128_base_t<S> value,
                   std::ios_base::fmtflags flags = std::ios_base::dec,
                   std::streamsize width = 0,
                   char fill = ' ');
```

**Brief:** Formatea un valor con control completo de flags, ancho y relleno.

**Parámetros:**

- `value` - Valor a formatear
- `flags` - Flags de formato (`std::ios_base::hex`, `dec`, `oct`, `left`, `right`, etc.)
- `width` - Ancho mínimo
- `fill` - Carácter de relleno

**Ejemplo:**

```cpp
using namespace nstd;
const uint128_t val{255};

auto s1 = format(val, std::ios_base::hex | std::ios_base::uppercase);
// "FF"

auto s2 = format(val, std::ios_base::dec, 10, '0');
// "0000000255"

auto s3 = format(val, std::ios_base::hex | std::ios_base::showbase);
// "0xff"
```

---

## Formateo por Base

### hex

```cpp
template<signedness S>
std::string hex(int128_base_t<S> value, bool uppercase = false, bool with_prefix = false);
```

**Brief:** Formato hexadecimal.

**Ejemplo:**

```cpp
const uint128_t val{255};
assert(nstd::hex(val) == "ff");
assert(nstd::hex(val, true) == "FF");
assert(nstd::hex(val, false, true) == "0xff");
assert(nstd::hex(val, true, true) == "0xFF");
```

---

### oct

```cpp
template<signedness S>
std::string oct(int128_base_t<S> value, bool with_prefix = false);
```

**Brief:** Formato octal.

**Ejemplo:**

```cpp
const uint128_t val{64};
assert(nstd::oct(val) == "100");
assert(nstd::oct(val, true) == "0100");
```

---

### dec

```cpp
template<signedness S>
std::string dec(int128_base_t<S> value);
```

**Brief:** Formato decimal (siempre sin signo).

---

### dec_signed

```cpp
template<signedness S>
std::string dec_signed(int128_base_t<S> value);
```

**Brief:** Formato decimal con signo (para int128_t).

**Ejemplo:**

```cpp
const int128_t val{-42};
assert(nstd::dec_signed(val) == "-42");
```

---

### bin

```cpp
template<signedness S>
std::string bin(int128_base_t<S> value, bool with_prefix = false);
```

**Brief:** Formato binario.

**Ejemplo:**

```cpp
const uint128_t val{42};
assert(nstd::bin(val) == "101010");
assert(nstd::bin(val, true) == "0b101010");
```

---

## Con Separadores

### with_separators

```cpp
template<signedness S>
std::string with_separators(int128_base_t<S> value, char separator = '\'', int group_size = 3);
```

**Brief:** Formato decimal con separadores de miles.

**Parámetros:**

- `value` - Valor a formatear
- `separator` - Carácter separador (por defecto `'`)
- `group_size` - Dígitos por grupo (por defecto 3)

**Ejemplo:**

```cpp
const uint128_t big{"123456789012345678901234567890"};
const auto s = nstd::with_separators(big);
// "123'456'789'012'345'678'901'234'567'890"

const auto s2 = nstd::with_separators(big, ',');
// "123,456,789,012,345,678,901,234,567,890"

const auto s3 = nstd::with_separators(big, '_', 4);
// "1234_5678_9012_3456_7890_1234_5678_90"
```

---

## Imitar iostream

### format_like_iostream

```cpp
template<signedness S>
std::string format_like_iostream(int128_base_t<S> value, const std::ostream& os);
```

**Brief:** Formatea usando el estado actual de un ostream.

**Ejemplo:**

```cpp
std::ostringstream oss;
oss << std::hex << std::uppercase << std::setw(20) << std::setfill('0');

const uint128_t val{0xDEADBEEF};
const auto s = nstd::format_like_iostream(val, oss);
// "000000000000DEADBEEF"
```

---

## Ver También

- [int128_base_iostreams.hpp](API_INT128_BASE_IOSTREAMS.md) - Operadores de stream
- [int128_base_tt.hpp](API_INT128_BASE_TT.md) - `to_string()` básico

---

**Licencia:** Boost Software License 1.0
