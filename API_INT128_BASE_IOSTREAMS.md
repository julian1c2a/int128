# API Reference - int128_base_iostreams.hpp

> **Operadores de entrada/salida para tipos de 128 bits**
> **Namespace:** `nstd`

---

## Synopsis

```cpp
#include "int128_base_iostreams.hpp"

namespace nstd {
    // Operadores de stream
    template<signedness S>
    std::ostream& operator<<(std::ostream& os, int128_base_t<S> value);
    
    template<signedness S>
    std::istream& operator>>(std::istream& is, int128_base_t<S>& value);
    
    // Funciones de conveniencia
    template<signedness S>
    std::string format(int128_base_t<S> value);
    
    template<signedness S>
    std::string hex(int128_base_t<S> value);
    
    template<signedness S>
    std::string oct(int128_base_t<S> value);
    
    template<signedness S>
    std::string dec(int128_base_t<S> value);
}
```

---

## Operadores de Stream

### operator<<

```cpp
template<signedness S>
std::ostream& operator<<(std::ostream& os, int128_base_t<S> value);
```

**Brief:** Escribe un valor a un stream de salida.

**Respeta manipuladores:**

- `std::hex`, `std::oct`, `std::dec` - Base numérica
- `std::uppercase` - Letras mayúsculas en hex
- `std::showbase` - Prefijo `0x`, `0`, etc.
- `std::setw()` - Ancho mínimo
- `std::setfill()` - Carácter de relleno
- `std::left`, `std::right`, `std::internal` - Alineación
- `std::showpos` - Mostrar signo `+` para positivos

**Ejemplo:**

```cpp
using namespace nstd;
const uint128_t val{255};

std::cout << val << "\n";                        // "255"
std::cout << std::hex << val << "\n";            // "ff"
std::cout << std::hex << std::uppercase << val;  // "FF"
std::cout << std::hex << std::showbase << val;   // "0xff"

std::cout << std::setw(10) << std::setfill('0') << val;  // "0000000255"
```

---

### operator>>

```cpp
template<signedness S>
std::istream& operator>>(std::istream& is, int128_base_t<S>& value);
```

**Brief:** Lee un valor desde un stream de entrada.

**Comportamiento:**

- Ignora whitespace inicial
- Detecta automáticamente la base:
  - `0x` o `0X` → hexadecimal
  - `0b` o `0B` → binario
  - `0` → octal
  - otro → decimal
- Soporta signos `-` y `+` para int128_t
- Soporta separadores `'` y `_`

**Ejemplo:**

```cpp
using namespace nstd;

std::istringstream iss("42");
uint128_t val;
iss >> val;
assert(val == uint128_t{42});

std::istringstream iss2("0xFF");
iss2 >> val;
assert(val == uint128_t{255});

std::istringstream iss3("-123");
int128_t signed_val;
iss3 >> signed_val;
assert(signed_val == int128_t{-123});
```

---

## Funciones de Conveniencia

### format

```cpp
template<signedness S>
std::string format(int128_base_t<S> value);
```

**Brief:** Convierte a string decimal.

**Equivalente a:** `value.to_string()`

---

### hex

```cpp
template<signedness S>
std::string hex(int128_base_t<S> value);
```

**Brief:** Convierte a string hexadecimal (sin prefijo).

**Ejemplo:**

```cpp
const uint128_t val{255};
assert(nstd::hex(val) == "ff");
```

---

### oct

```cpp
template<signedness S>
std::string oct(int128_base_t<S> value);
```

**Brief:** Convierte a string octal (sin prefijo).

---

### dec

```cpp
template<signedness S>
std::string dec(int128_base_t<S> value);
```

**Brief:** Convierte a string decimal.

---

## Ejemplos Avanzados

### Tabla formateada

```cpp
#include <iomanip>
#include "int128_base_iostreams.hpp"

using namespace nstd;

void print_table() {
    std::cout << std::setw(20) << "Decimal"
              << std::setw(20) << "Hexadecimal"
              << std::setw(20) << "Binary\n";
    
    for (uint128_t i{1}; i < uint128_t{16}; i = i * uint128_t{2}) {
        std::cout << std::dec << std::setw(20) << i
                  << std::hex << std::setw(20) << i
                  << std::setw(20) << i.to_string(2) << "\n";
    }
}
```

### Lectura de múltiples formatos

```cpp
std::string input = "100 0x64 0144 0b1100100";
std::istringstream iss(input);

uint128_t a, b, c, d;
iss >> a >> b >> c >> d;
// Todos valen 100:
assert(a == b && b == c && c == d);
```

---

## Ver También

- [int128_base_format.hpp](API_INT128_BASE_FORMAT.md) - Formateo avanzado
- [int128_base_tt.hpp](API_INT128_BASE_TT.md) - `to_string()` y `parse()`

---

**Licencia:** Boost Software License 1.0
