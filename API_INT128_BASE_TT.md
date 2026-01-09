# API Reference - int128_base_t

> **Template unificado para enteros de 128 bits signed y unsigned**  
> **Versión:** 1.0.0  
> **Fecha:** 2026-01-05  
> **Namespace:** `nstd`

---

## 📋 Tabla de Contenidos

1. [Enumeraciones](#enumeraciones)
2. [Type Aliases](#type-aliases)
3. [Constantes Globales](#constantes-globales)
4. [Template Principal](#template-principal)
5. [Constructores](#constructores)
6. [Operadores de Asignación](#operadores-de-asignación)
7. [Métodos de Acceso](#métodos-de-acceso)
8. [Operadores Unarios](#operadores-unarios)
9. [Operadores Aritméticos](#operadores-aritméticos)
10. [Operadores de División/Módulo](#operadores-de-división-módulo)
11. [Operadores Bitwise](#operadores-bitwise)
12. [Operadores de Shift](#operadores-de-shift)
13. [Operadores de Comparación](#operadores-de-comparación)
14. [Funciones Puras de Incremento/Decremento](#funciones-puras-de-incremento-decremento)
15. [Operadores de Incremento/Decremento](#operadores-de-incremento-decremento)
16. [Conversiones Explícitas](#conversiones-explícitas)
17. [Métodos Auxiliares](#métodos-auxiliares)
18. [Funciones Estáticas](#funciones-estáticas)
19. [Stream I/O](#stream-io)
20. [Literales UDL](#literales-udl)
21. [Funciones Factory](#funciones-factory)

---

## Enumeraciones

### signedness

```cpp
enum class signedness : bool {
    unsigned_type = false,  // uint128_t
    signed_type = true      // int128_t
};
```

**Descripción:** Enum para distinguir entre tipos signed y unsigned en el template.

---

### parse_error

```cpp
enum class parse_error : uint8_t {
    success = 0,
    null_pointer,
    empty_string,
    invalid_base,
    invalid_character,
    overflow,
    unknown_error
};
```

**Descripción:** Códigos de error para la función `parse()`.

---

## Type Aliases

```cpp
using uint128_t = int128_base_t<signedness::unsigned_type>;
using int128_t = int128_base_t<signedness::signed_type>;
```

**Descripción:** Aliases convenientes para los tipos concretos de 128 bits.

---

## Constantes Globales

```cpp
inline constexpr uint128_t UINT128_MAX =
    uint128_t(std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max());

inline constexpr int128_t INT128_MAX =
    int128_t(reinterpret_cast<uint64_t>(std::numeric_limits<int64_t>::max()), std::numeric_limits<uint64_t>::max());

inline constexpr int128_t INT128_MIN =
    int128_t(reinterpret_cast<uint64_t>(std::numeric_limits<int64_t>::min()), 0ULL);
```

**Descripción:** Constantes globales representando valores extremos.

**Valores:**

- `UINT128_MAX`: 340282366920938463463374607431768211455 (2^128 - 1)
- `INT128_MAX`: 170141183460469231731687303715884105727 (2^127 - 1)
- `INT128_MIN`: -170141183460469231731687303715884105728 (-2^127)

---

## Template Principal

```cpp
template <signedness S>
class int128_base_t {
    static constexpr bool is_signed = (S == signedness::signed_type);
    static constexpr int BITS = 128;
    static constexpr int BYTES = BITS / CHAR_BIT;
    
private:
    uint64_t data[2];  // data[0] = low 64 bits, data[1] = high 64 bits
    
public:
    // ... (ver secciones siguientes)
};
```

**Descripción:** Clase template unificada que implementa enteros de 128 bits con signedness parametrizable.

**Miembros privados:**

- `data[2]`: Array de dos uint64_t representando los 128 bits en formato little-endian

**Constantes estáticas:**

- `is_signed`: true para int128_t, false para uint128_t
- `BITS`: Siempre 128
- `BYTES`: Siempre 16

---

## Constructores

### Constructor por defecto

```cpp
constexpr int128_base_t() noexcept;
```

**Descripción:** Construye valor cero.

---

### Constructor desde integral builtin

```cpp
template <integral_builtin T>
explicit constexpr int128_base_t(T value) noexcept;
```

**Descripción:** Construye desde cualquier tipo integral estándar (int8_t, uint64_t, etc.).  
**Sign extension:** Automático para tipos signed cuando `S == signedness::signed_type`.

---

### Constructor desde par (high, low)

```cpp
template <integral_builtin T1, integral_builtin T2>
explicit constexpr int128_base_t(T1 high, T2 low) noexcept;
```

**Descripción:** Construye desde dos valores de 64 bits (high y low).

---

### Constructor desde floating point

```cpp
template <floating_point_builtin T>
explicit constexpr int128_base_t(T value) noexcept;
```

**Descripción:** Construye desde float, double o long double.  
**Rango válido:** Valores fuera del rango válido producen comportamiento indefinido.

---

### Constructores de copia y movimiento

```cpp
constexpr int128_base_t(const int128_base_t&) = default;
constexpr int128_base_t(int128_base_t&&) = default;
```

---

### Constructor de conversión entre signedness

```cpp
template <signedness S2>
explicit constexpr int128_base_t(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
explicit constexpr int128_base_t(int128_base_t<S2>&& other) noexcept;
```

**Descripción:** Permite conversión explícita entre uint128_t ↔ int128_t.

---

### Constructores desde string

```cpp
explicit int128_base_t(const char* str);
explicit int128_base_t(const char* str, int base);
explicit int128_base_t(const std::string& str);
explicit int128_base_t(const std::string& str, int base);
explicit int128_base_t(std::string_view str);
explicit int128_base_t(std::string_view str, int base);
```

**Descripción:** Construye desde string con parsing automático.  
**Lanza:** `std::invalid_argument` si el string no es válido.  
**Base:** 2-36 (auto-detección con prefijos `0x`, `0b`, `0` si no se especifica).

---

## Operadores de Asignación

### Asignación por defecto

```cpp
constexpr int128_base_t& operator=(const int128_base_t&) = default;
constexpr int128_base_t& operator=(int128_base_t&&) = default;
```

---

### Asignación desde otro signedness

```cpp
template <signedness S2>
constexpr int128_base_t& operator=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t& operator=(int128_base_t<S2>&& other) noexcept;
```

---

### Asignación desde integral builtin

```cpp
template <integral_builtin T>
constexpr int128_base_t& operator=(T other) noexcept;
```

---

### Asignación desde floating point

```cpp
template <floating_point_builtin T>
constexpr int128_base_t& operator=(T value) noexcept;
```

---

## Métodos de Acceso

```cpp
constexpr uint64_t high() const noexcept;
constexpr uint64_t low() const noexcept;

template <typename T>
constexpr void set_high(T value) noexcept;

template <typename T>
constexpr void set_low(T value) noexcept;
```

**Descripción:** Acceso a las dos mitades de 64 bits.

---

## Operadores Unarios

### Negación bitwise (NOT)

```cpp
constexpr int128_base_t operator~() const noexcept;
```

**⚠️ FIX Fase 17:** Constructor corregido (antes intercambiaba high/low incorrectamente).

---

### Negación aritmética

```cpp
constexpr int128_base_t operator-() const noexcept;
```

**Descripción:** Complemento a 2 (sea el tipo signed o unsigned).  
**Requiere:** `S == signedness::signed_type` para comportamiento matemático habitual.

---

## Operadores Aritméticos

### Operadores con mismo tipo

### Operadores con otro signedness

```cpp
template <signedness S2>
constexpr int128_base_t& operator+=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator+(const int128_base_t<S2>& other) const noexcept;

template <signedness S2>
constexpr int128_base_t& operator-=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator-(const int128_base_t<S2>& other) const noexcept;

template <signedness S2>
constexpr int128_base_t& operator*=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator*(const int128_base_t<S2>& other) const noexcept;
```

---

### Operadores con integral builtin

```cpp
template <integral_builtin T>
constexpr int128_base_t& operator+=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator+(T other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator-=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator-(T other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator*=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator*(T other) const noexcept;
```

---

## Operadores de División/Módulo

### Con mismo tipo o distinto signedness template <signedness S2>

```cpp
```cpp
template <signedness S2>
constexpr int128_base_t& operator/=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator/(const int128_base_t<S2>& other) const noexcept;

template <signedness S2>
constexpr int128_base_t& operator%=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator%(const int128_base_t<S2>& other) const noexcept;
```

---

### Con integral builtin

```cpp
template <integral_builtin T>
constexpr int128_base_t& operator/=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator/(T other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator%=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator%(T other) const noexcept;
```

---

### Con otro signedness

---

## Operadores Bitwise

### AND (&)

```cpp
template <signedness S2>
constexpr int128_base_t& operator&=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator&(const int128_base_t<S2>& other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator&=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator&(T other) const noexcept;
```

---

### OR (|)

```cpp
template <signedness S2>
constexpr int128_base_t& operator|=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator|(const int128_base_t<S2>& other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator|=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator|(T other) const noexcept;
```

---

### XOR (^)

```cpp
template <signedness S2>
constexpr int128_base_t& operator^=(const int128_base_t<S2>& other) noexcept;

template <signedness S2>
constexpr int128_base_t operator^(const int128_base_t<S2>& other) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator^=(T other) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator^(T other) const noexcept;
```

---

## Operadores de Shift

### Left Shift (<<)

```cpp
template <integral_builtin T>
constexpr int128_base_t& operator<<=(T shift) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator<<(T shift) const noexcept;
```

---

### Right Shift (>>)

```cpp
constexpr int128_base_t& operator>>=(int shift) noexcept;
constexpr int128_base_t operator>>(int shift) const noexcept;

template <integral_builtin T>
constexpr int128_base_t& operator>>=(T shift) noexcept;

template <integral_builtin T>
constexpr int128_base_t operator>>(T shift) const noexcept;
```

**Nota:** Para tipos signed, >> realiza arithmetic shift (sign extension).  
Para unsigned, >> realiza logical shift (ceros).

---

## Operadores de Comparación

### Con mismo tipo

```cpp
constexpr bool operator==(const int128_base_t& other) const noexcept;
constexpr bool operator!=(const int128_base_t& other) const noexcept;
constexpr bool operator<(const int128_base_t& other) const noexcept;
constexpr std::strong_ordering operator<=>(const int128_base_t& other) const noexcept;
```

---

### Friend operators con integral builtin

```cpp
template <integral_builtin T>
friend constexpr bool operator==(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator==(T lhs, const int128_base_t& rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator!=(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator!=(T lhs, const int128_base_t& rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator<(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator<(T lhs, const int128_base_t& rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator<=(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator<=(T lhs, const int128_base_t& rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator>(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator>(T lhs, const int128_base_t& rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator>=(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator>=(T lhs, const int128_base_t& rhs) noexcept;
```

---

### Friend operators con otro signedness

```cpp
template <signedness S2>
friend constexpr bool operator==(const int128_base_t& lhs, const int128_base_t<S2>& rhs) noexcept;

template <signedness S2>
friend constexpr bool operator!=(const int128_base_t& lhs, const int128_base_t<S2>& rhs) noexcept;
```

---

## Funciones Puras de Incremento/Decremento

### incr() - Incremento con Semántica de Valor

```cpp
constexpr int128_base_t incr() const noexcept;
```

**Descripción:** Retorna un nuevo `int128_base_t` con valor incrementado en 1.  
**Semántica:** Función **pura** (no modifica el objeto actual).  
**Optimización:** Delega a `operator+=` que usa intrínsecos optimizados.  
**Portabilidad:** Funciona en x86-64, ARM64, RISC-V con intrínsecos específicos.

**Intrínsecos usados:**

- **x86-64**: `ADC` (add with carry) via `_addcarry_u64` (MSVC/Intel) o `__builtin_add_overflow` (GCC/Clang)
- **ARM64**: `ADDS` (add with set flags)
- **RISC-V**: `ADD + SLTU` (overflow detection)

**Ejemplo:**

```cpp
int128_t x(42);
int128_t y = x.incr();  // y = 43, x permanece = 42

// Equivalente a: y = x + 1;
// Pero con intención más explícita (valor puro, no mutación)
```

**Nota:** Prefiere `incr()` sobre `operator++` cuando necesites semántica de valor puro.

---

### decr() - Decremento con Semántica de Valor

```cpp
constexpr int128_base_t decr() const noexcept;
```

**Descripción:** Retorna un nuevo `int128_base_t` con valor decrementado en 1.  
**Semántica:** Función **pura** (no modifica el objeto actual).  
**Optimización:** Delega a `operator-=` que usa intrínsecos optimizados.  
**Portabilidad:** Misma optimización que `incr()` pero para resta.

**Intrínsecos usados:**

- **x86-64**: `SBB` (subtract with borrow) via `_subborrow_u64` o `__builtin_sub_overflow`
- **ARM64**: `SUBS` (subtract with set flags)
- **RISC-V**: `SUB + SLTU` (underflow detection)

**Ejemplo:**

```cpp
uint128_t x(100);
uint128_t y = x.decr();  // y = 99, x permanece = 100

// Equivalente a: y = x - 1;
// Pero con intención más explícita (valor puro, no mutación)
```

**Nota:** Prefiere `decr()` sobre `operator--` cuando necesites semántica de valor puro.

---

## Operadores de Incremento/Decremento

```cpp
constexpr int128_base_t& operator++() noexcept;       // Pre-incremento
constexpr int128_base_t operator++(int) noexcept;    // Post-incremento

constexpr int128_base_t& operator--() noexcept;       // Pre-decremento
constexpr int128_base_t operator--(int) noexcept;    // Post-decremento
```

---

## Conversiones Explícitas

### A bool

```cpp
explicit constexpr operator bool() const noexcept;
```

**Retorna:** `true` si el valor != 0, `false` en caso contrario.

---

### A tipos aritméticos

```cpp
template <arithmetic_builtin T>
explicit constexpr operator T() const noexcept;
```

**Descripción:** Conversión explícita a int, uint64_t, float, double, etc.  
**Truncamiento:** Valores fuera del rango del tipo destino se truncan.

---

## Métodos Auxiliares

### Conversión a string

```cpp
std::string to_string() const;
std::string to_string(int base) const;
```

**Descripción:** Convierte a string decimal (base 10) o en la base especificada (2-36).  
**Base:** 2 (binario), 8 (octal), 10 (decimal), 16 (hexadecimal), 36 (máximo).  
**⚠️ FIX Fase 17:** Ahora funciona correctamente con negativos.

---

### Valor absoluto

```cpp
constexpr int128_base_t abs() const noexcept;
```

**Descripción:** Retorna el valor absoluto (para tipos signed existe un posible cambio, para los unsigned es la identidad).  
**⚠️ FIX Fase 17:** Ahora funciona correctamente (dependía de operator~() bugueado).

---

### Comprobación de signo

```cpp
constexpr bool is_negative() const noexcept
    requires(is_signed);
```

**Retorna:** `true` si el valor es negativo (MSB de data[1] == 0b1).

---

### División con resto por 10

```cpp
constexpr std::pair<int128_base_t, uint64_t> divrem_by_10() const noexcept;
```

**Descripción:** Optimización para conversión a string decimal.  
**Retorna:** `{cociente, resto}` donde resto ∈ [0, 9].

---

### División con resto general

```cpp
constexpr std::pair<int128_base_t, int128_base_t> divrem(const int128_base_t& divisor) const noexcept;
```

**Descripción:** Calcula `{cociente, resto}` en una sola operación.  
**Algoritmo:** Usa división binaria optimizada (no Knuth D para 128 bits).

---

### Leading zeros

```cpp
constexpr int leading_zeros() const noexcept;
```

**Retorna:** Número de ceros a la izquierda (0-128).

---

## Funciones Estáticas

### parse

```cpp
static constexpr std::pair<parse_error, int128_base_t> 
parse(const char* str) noexcept;
```

**Descripción:** Parsea string con auto-detección de base.  
**Prefijos:** `0x` (hex), `0b` (binario), `0` (octal), ninguno (decimal).  
**Separadores:** Ignora `'`, `_`, espacios.  
**Retorna:** `{error, valor}` donde error == `parse_error::success` si OK.

---

### parse con base explícita

```cpp
static constexpr std::pair<parse_error, int128_base_t> 
parse_base(const char* str, int base) noexcept;
```

**Base:** 2-36.

---

### min / max

```cpp
static consteval int128_base_t min() noexcept;
static consteval int128_base_t max() noexcept;
```

**Descripción:** Retorna valores mínimo y máximo representables.

**Valores:**

- `uint128_t::min()` = 0
- `uint128_t::max()` = UINT128_MAX
- `int128_t::min()` = INT128_MIN
- `int128_t::max()` = INT128_MAX

---

## Stream I/O

```cpp
friend std::ostream& operator<<(std::ostream& os, const int128_base_t& value);
friend std::istream& operator>>(std::istream& is, int128_base_t& value);
```

**Descripción:** Permite usar `std::cout << value` y `std::cin >> value`.

---

## Literales UDL

### Literales unsigned

```cpp
constexpr uint128_t operator""_u128(const char* str) noexcept;
constexpr uint128_t operator""_U128(const char* str) noexcept;
```

**Uso:**

```cpp
auto x = 42_u128;
auto big = "123456789012345678901234567890"_U128;
```

---

### Literales signed

```cpp
constexpr int128_t operator""_i128(const char* str) noexcept;
constexpr int128_t operator""_I128(const char* str) noexcept;
```

**Uso:**

```cpp
auto neg = -42_i128;
auto big_neg = "-123456789012345678901234567890"_I128;
```

---

## Funciones Factory

### make_int128

```cpp
template <signedness S>
constexpr int128_base_t<S> make_int128(const char* str) noexcept;
```

**Descripción:** Construye int128_t o uint128_t desde string constexpr.  
**Retorna:** Valor parseado o 0 si hay error.

---

### make_int128_checked

```cpp
template <signedness S>
constexpr std::pair<parse_error, int128_base_t<S>> 
make_int128_checked(const char* str) noexcept;
```

**Descripción:** Versión con verificación de errores.

---

### Wrappers específicos

```cpp
inline constexpr uint128_t make_uint128(const char* str) noexcept;
inline constexpr int128_t make_int128_signed(const char* str) noexcept;
```

---

## 📝 Notas de Mantenimiento

**Este archivo contiene solo firmas, no implementaciones.**

- **Actualización:** Mantener sincronizado con `int128_base.hpp`
- **Última actualización:** 2026-01-05 (Fase 18)
- **Versión biblioteca:** 1.0.0
- **Tests:** 29/29 PASS ✅

---

## 🔗 Ver También

- [CHANGELOG.md](CHANGELOG.md) - Historial de cambios
- [README.md](README.md) - Guía de usuario
- [int128_base.hpp](include_new/int128_base.hpp) - Implementación completa
- [documentation/generated/html/index.html](documentation/generated/html/index.html) - Documentación Doxygen

---

**Copyright © 2026 int128 Project**  
**Licencia:** Boost Software License 1.0
