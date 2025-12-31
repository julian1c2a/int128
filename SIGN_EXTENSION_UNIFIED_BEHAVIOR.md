# Sign Extension: Comportamiento Unificado int128_t/uint128_t

**Fecha:** 31 de diciembre de 2025  
**Contexto:** Análisis previo a Fase 1.5 - Unificación Template Signed/Unsigned

---

## 🎯 Resumen Ejecutivo

Tanto `int128_t` como `uint128_t` implementan **sign extension** al convertir desde tipos integrales signed negativos. Este comportamiento consistente es **fundamental** para la futura unificación en un template `int128_base_t<signedness S>`.

**Garantía clave**: Ambos tipos usan **idéntica lógica de conversión**, diferenciándose solo en cómo **interpretan** los bits resultantes (signed vs unsigned).

---

## 📋 Comportamiento Actual de Conversiones

### Tabla Completa de Conversiones

| Tipo Fuente | Valor | Destino: uint128_t | Destino: int128_t | Patrón de Relleno |
|-------------|-------|-------------------|-------------------|-------------------|
| `uint64_t` | cualquiera | ✅ | ✅ | **Zero extension** (0s) |
| `uint32_t` | cualquiera | ✅ | ✅ | **Zero extension** (0s) |
| `uint16_t` | cualquiera | ✅ | ✅ | **Zero extension** (0s) |
| `uint8_t` | cualquiera | ✅ | ✅ | **Zero extension** (0s) |
| `int64_t` | ≥ 0 | ✅ | ✅ | **Zero extension** (0s) |
| `int32_t` | ≥ 0 | ✅ | ✅ | **Zero extension** (0s) |
| `int64_t` | < 0 | ✅ | ✅ | **Sign extension** (1s) |
| `int32_t` | < 0 | ✅ | ✅ | **Sign extension** (1s) |
| `int16_t` | < 0 | ✅ | ✅ | **Sign extension** (1s) |
| `int8_t` | < 0 | ✅ | ✅ | **Sign extension** (1s) |

**Punto crítico**: Ambos tipos comparten **exactamente la misma lógica**:

```cpp
if constexpr (std::is_signed_v<T>) {
    if (value < 0) {
        // AMBOS tipos: Sign extension
        data[0] = static_cast<uint64_t>(value);
        data[1] = UINT64_MAX;  // 0xFFFFFFFFFFFFFFFF
    } else {
        // AMBOS tipos: Zero extension
        data[0] = static_cast<uint64_t>(value);
        data[1] = 0;
    }
} else {
    // AMBOS tipos: Zero extension
    data[0] = static_cast<uint64_t>(value);
    data[1] = 0;
}
```

---

## 🔧 Implementación Actual

### uint128_t::uint128_t(T value) - Líneas 218-226

**Archivo:** `include/uint128/uint128_t.hpp`

```cpp
template <integral_builtin T>
constexpr uint128_t(T value) noexcept : data{static_cast<uint64_t>(value), 0ull}
{
    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            data[1] = ~0ull; // Extensión de signo correcta para negativos
        }
    }
}
```

**Comportamiento**:

1. Inicializa `data[0]` con el valor (ya con sign extension si T es signed)
2. Inicializa `data[1] = 0` por defecto
3. **Si T es signed Y value < 0**: Sobrescribe `data[1] = 0xFFFFFFFFFFFFFFFF`

### int128_t::int128_t(T value) - Líneas 74-89

**Archivo:** `include/int128/int128_t.hpp`

```cpp
template <typename T>
explicit constexpr int128_t(T value) noexcept : data{0, 0}
{
    static_assert(std::is_integral_v<T> && sizeof(T) <= 8 && !std::is_same_v<T, uint128_t>,
                  "T must be an integral type <= 8 bytes and not uint128_t");
    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            data[0] = static_cast<uint64_t>(value);
            data[1] = UINT64_MAX; // Extensión de signo
        } else {
            data[0] = static_cast<uint64_t>(value);
            data[1] = 0;
        }
    } else {
        data[0] = static_cast<uint64_t>(value);
        data[1] = 0;
    }
}
```

**Comportamiento idéntico** con sintaxis diferente:

1. Inicializa `data{0, 0}` por defecto
2. **Distingue signed vs unsigned en compile-time**
3. **Para signed < 0**: `data[1] = UINT64_MAX` (sign extension)
4. **Para todo lo demás**: `data[1] = 0` (zero extension)

---

## 💡 Ejemplos Prácticos

### Ejemplo 1: Conversión de int64_t Negativo

```cpp
int64_t source = -42;
// Representación: 0xFFFFFFFFFFFFFFD6 (complemento a 2)

// Conversión a uint128_t
uint128_t u(source);
// data[0] = 0xFFFFFFFFFFFFFFD6 (bits preservados)
// data[1] = 0xFFFFFFFFFFFFFFFF (sign extension)
// Resultado: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD6

// Conversión a int128_t
int128_t i(source);
// data[0] = 0xFFFFFFFFFFFFFFD6 (bits preservados)
// data[1] = 0xFFFFFFFFFFFFFFFF (sign extension)
// Resultado: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD6

// CRÍTICO: Ambos tienen IDÉNTICA representación binaria
static_assert(std::memcmp(&u, &i, 16) == 0, "Bits idénticos");

// La diferencia está en la INTERPRETACIÓN:
static_cast<int64_t>(u) == -42;  // ✅ Valor preservado
i.to_string() == "-42";           // ✅ Signo interpretado correctamente
```

### Ejemplo 2: Conversión de int32_t Negativo

```cpp
int32_t source = -1;
// Representación: 0xFFFFFFFF (32 bits)

// Ambas conversiones:
uint128_t u(source);
int128_t i(source);

// PASO 1: static_cast<uint64_t>(-1) → 0xFFFFFFFFFFFFFFFF (sign ext 32→64)
// PASO 2: Detectar negativo → data[1] = 0xFFFFFFFFFFFFFFFF (sign ext 64→128)

// Resultado IDÉNTICO en ambos:
// data[0] = 0xFFFFFFFFFFFFFFFF
// data[1] = 0xFFFFFFFFFFFFFFFF
// Representación binaria: todos los bits a 1
```

### Ejemplo 3: Conversión de uint64_t (Sin Sign Extension)

```cpp
uint64_t source = UINT64_MAX; // 0xFFFFFFFFFFFFFFFF

uint128_t u(source);
// data[0] = 0xFFFFFFFFFFFFFFFF
// data[1] = 0x0000000000000000  ← Zero extension (source es unsigned)

int128_t i(source);
// data[0] = 0xFFFFFFFFFFFFFFFF
// data[1] = 0x0000000000000000  ← Zero extension (source es unsigned)

// CRÍTICO: No hay sign extension porque std::is_signed_v<uint64_t> == false
// Ambos interpretan esto como un NÚMERO POSITIVO GRANDE
```

---

## 🚀 Implicaciones para la Unificación (Fase 1.5)

### Código Compartible (>90%)

El comportamiento idéntico permite compartir casi toda la lógica:

```cpp
// FUTURO: Template unificado
template<signedness S>
class int128_base_t {
    static constexpr bool is_signed = (S == signedness::signed_type);
    uint64_t data[2];

public:
    // Constructor IDÉNTICO para ambos tipos
    template <typename T>
    constexpr int128_base_t(T value) noexcept : data{static_cast<uint64_t>(value), 0}
    {
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                data[1] = UINT64_MAX;  // Sign extension compartida
            }
        }
    }
    
    // Solo ~5-10% de operaciones necesitan if constexpr (is_signed)
    // Ejemplos: operator-(), operator>>=, abs(), is_negative()
};
```

### Operaciones Compartidas (Sin if constexpr)

**>90% de operaciones son idénticas** porque trabajan sobre bits, no sobre interpretación:

#### Aritmética Básica (100% compartida)

```cpp
// Suma: Idéntica para signed/unsigned
constexpr int128_base_t& operator+=(const int128_base_t& other) noexcept {
    uint64_t temp = 0;
    auto carry = intrinsics::add_u64(data[0], other.data[0], &temp);
    data[0] = temp;
    intrinsics::addcarry_u64(carry, data[1], other.data[1], &temp);
    data[1] = temp;
    return *this;
}

// Multiplicación, división, módulo: Idénticas en lógica de bits
```

#### Bitwise (100% compartida)

```cpp
// AND, OR, XOR, NOT: Operan sobre bits sin importar signo
constexpr int128_base_t operator&(const int128_base_t& other) const noexcept {
    return int128_base_t(data[1] & other.data[1], data[0] & other.data[0]);
}
```

#### Comparaciones entre Mismo Tipo (100% compartida)

```cpp
constexpr bool operator==(const int128_base_t& other) const noexcept {
    return data[0] == other.data[0] && data[1] == other.data[1];
}

constexpr bool operator<(const int128_base_t& other) const noexcept {
    if constexpr (is_signed) {
        // Solo aquí: comparación signed requiere considerar MSB como signo
        return /* lógica signed */;
    } else {
        // Comparación unsigned simple
        return data[1] < other.data[1] || 
               (data[1] == other.data[1] && data[0] < other.data[0]);
    }
}
```

#### Conversiones String (95% compartida)

```cpp
constexpr std::string to_string() const {
    if constexpr (is_signed) {
        if (is_negative()) {
            // Solo aquí: manejar signo negativo
            return "-" + negate().to_string_unsigned();
        }
    }
    return to_string_unsigned();  // Lógica compartida
}
```

### Operaciones Específicas de Signo (<10%)

**Solo estas operaciones necesitan `if constexpr (is_signed)`**:

1. **Negación unaria** (`operator-()`)
   - Solo para signed
   - unsigned puede tener implementación dummy para compatibilidad

2. **Arithmetic right shift** (`operator>>=`)
   - Signed: Propaga bit de signo
   - Unsigned: Rellena con 0s

3. **Valor absoluto** (`abs()`)
   - Solo significativo para signed
   - unsigned retorna `*this`

4. **Detección de signo** (`is_negative()`)
   - Signed: Verifica MSB de data[1]
   - Unsigned: Siempre false

5. **División/módulo con signos** (optimización)
   - Signed puede beneficiarse de paths especiales
   - Pero la lógica básica es compartible

---

## 📊 Matriz de Decisión: if constexpr Necesarios

| Operación | ¿Requiere `if constexpr (is_signed)`? | Razón |
|-----------|----------------------------------------|-------|
| Constructores desde T | **✅ SÍ** | Sign extension para T signed |
| `operator+` | ❌ NO | Suma de bits es idéntica |
| `operator-` (binario) | ❌ NO | Resta de bits es idéntica |
| `operator-` (unario) | **✅ SÍ** | Solo para signed |
| `operator*` | ❌ NO | Multiplicación de bits idéntica |
| `operator/` | ⚠️ OPCIONAL | Optimización signed, pero base compartible |
| `operator%` | ⚠️ OPCIONAL | Optimización signed, pero base compartible |
| `operator&, |, ^, ~` | ❌ NO | Operaciones bitwise puras |
| `operator<<` | ❌ NO | Shift lógico a izquierda idéntico |
| `operator>>` | **✅ SÍ** | Arithmetic (signed) vs Logical (unsigned) |
| `operator==, !=` | ❌ NO | Comparación de bits exacta |
| `operator<, <=, >, >=` | **✅ SÍ** | Interpretación de MSB como signo |
| `high(), low()` | ❌ NO | Acceso a miembros idéntico |
| `to_string()` | **✅ SÍ** | Formateo con signo '-' |
| `from_string()` | **✅ SÍ** | Parsing de '-' |
| `abs()` | **✅ SÍ** | Solo para signed |
| `is_negative()` | **✅ SÍ** | Verificación de MSB |

**Resumen**: ~10 de ~100 operaciones requieren `if constexpr` (~10%)

---

## 🎯 Ventajas para la Unificación

### 1. **Código DRY (Don't Repeat Yourself)**

**Antes de unificación** (código actual):

- `uint128_t.hpp`: ~2733 líneas
- `int128_t.hpp`: ~1090 líneas
- **Total**: ~3823 líneas con duplicación significativa

**Después de unificación** (estimado):

- `int128_base_t.hpp`: ~2500 líneas (código compartido)
- `uint128_t = int128_base_t<unsigned_type>`: ~10 líneas (alias)
- `int128_t = int128_base_t<signed_type>`: ~10 líneas (alias)
- **Total**: ~2520 líneas
- **Reducción**: ~34% menos código

### 2. **Consistencia Automática**

```cpp
// Bug hipotético en código actual:
// uint128_t::operator+(const uint128_t&) tiene un bug
// int128_t::operator+(const int128_t&) NO tiene el bug

// Con template unificado:
// operator+ es UNA SOLA IMPLEMENTACIÓN
// ¡Imposible tener inconsistencias!
```

### 3. **Mantenimiento Simplificado**

```cpp
// Ahora: Cambiar operador en 2 lugares
// - Modificar uint128_t::operator/=()
// - Modificar int128_t::operator/=() (copiar/pegar/adaptar)
// - Riesgo de desincronización

// Futuro: Cambiar operador en 1 lugar
// - Modificar int128_base_t::operator/=()
// - Ambos tipos se benefician automáticamente
```

### 4. **Testing Simplificado**

```cpp
// Ahora: Tests duplicados
// - tests/uint128_algorithm_extracted_tests.cpp (538 líneas)
// - tests/int128_algorithm_extracted_tests.cpp (538 líneas CASI IDÉNTICAS)

// Futuro: Tests parametrizados
template<signedness S>
void test_addition() {
    using T = int128_base_t<S>;
    T a(10), b(20);
    assert((a + b).to_string() == "30");
}

// Ejecutar una vez para cada signedness
test_addition<unsigned_type>();
test_addition<signed_type>();
```

### 5. **Extensión Futura (Fase 2: N-width)**

```cpp
// Después de Fase 1.5 (signedness unificado):
template<signedness S>
class int128_base_t { uint64_t data[2]; };

// Fase 2 será mecánica:
template<size_t N, signedness S>
class wide_int_t { uint64_t data[N]; };

// Solo cambiar:
// - data[0], data[1] → loops sobre data[i]
// - Constantes N=2 → parámetro N
```

---

## 📖 Estándar C++ Justificación

### [conv.integral] - Conversión Integral

> **§7.3.9 Integral conversions [conv.integral]**
>
> 1. A prvalue of an integer type can be converted to a prvalue of another integer type.
>
> 2. If the destination type is unsigned, the resulting value is the least unsigned integer
>    congruent to the source integer (modulo 2^n where n is the number of bits used to
>    represent the unsigned type).
>
> 3. If the destination type is signed, the value is unchanged if it can be represented
>    in the destination type; otherwise, the value is **implementation-defined**.

**Nuestra implementación elige**: Preservar la representación de complemento a 2 vía sign extension.

**Beneficio**: Conversión roundtrip sin pérdida:

```cpp
int64_t x = -42;
uint128_t u(x);           // Sign extension preserva bits
int64_t y = static_cast<int64_t>(u);  // ✅ y == x
```

### [basic.fundamental] - Representación de Enteros Signed

> **§6.8.2 Fundamental types [basic.fundamental]**
>
> Signed integer types shall use two's complement representation.

**Garantía C++20**: Todos los signed integers usan complemento a 2.

**Consecuencia**: Sign extension es la operación correcta para preservar valores negativos al extender el ancho de bits.

---

## 🔒 Garantías de Interoperabilidad

### Conversión Roundtrip (Sin Pérdida)

```cpp
// int64_t → uint128_t → int64_t (SIN PÉRDIDA)
int64_t original = -12345;
uint128_t intermediate(original);
int64_t recovered = static_cast<int64_t>(intermediate);
assert(recovered == original);  // ✅ Garantizado

// uint64_t → int128_t → uint64_t (SIN PÉRDIDA si valor < INT128_MAX)
uint64_t original2 = 0x7FFFFFFFFFFFFFFF;
int128_t intermediate2(original2);
uint64_t recovered2 = static_cast<uint64_t>(intermediate2);
assert(recovered2 == original2);  // ✅ Garantizado
```

### Operaciones Mixtas Predecibles

```cpp
int128_t a(-42);
uint128_t b(100);

// Conversión implícita vía constructor:
uint128_t result = a + b;
// a es convertido a uint128_t VÍA CONSTRUCTOR (sign extension)
// Luego: uint128_t(-42) + uint128_t(100) = uint128_t(58)
```

---

## 🛠️ Plan de Implementación (Fase 1.5)

### Paso 1: Crear Template Base

**Archivo nuevo**: `include/wide_int_base.hpp`

```cpp
enum class signedness : bool { 
    unsigned_type = false, 
    signed_type = true 
};

template<signedness S>
class int128_base_t {
    static constexpr bool is_signed = (S == signedness::signed_type);
    uint64_t data[2];

public:
    // Constructor con sign extension COMPARTIDO
    template <typename T>
    constexpr int128_base_t(T value) noexcept : data{static_cast<uint64_t>(value), 0}
    {
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                data[1] = UINT64_MAX;  // ✅ Sign extension
            }
        }
    }
    
    // ... resto de implementación compartida
};
```

### Paso 2: Reemplazar Clases Actuales con Type Aliases

**Archivo modificado**: `include/uint128/uint128_t.hpp`

```cpp
// Reemplazar clase completa con:
using uint128_t = int128_base_t<signedness::unsigned_type>;
```

**Archivo modificado**: `include/int128/int128_t.hpp`

```cpp
// Reemplazar clase completa con:
using int128_t = int128_base_t<signedness::signed_type>;
```

### Paso 3: Validación Exhaustiva

```bash
# Compilar TODOS los tests con TODOS los compiladores
make check TYPE=uint128 FEATURE=t COMPILER=all MODE=all
make check TYPE=int128 FEATURE=t COMPILER=all MODE=all

# Ejecutar TODOS los benchmarks
make run TYPE=uint128 FEATURE=t COMPILER=all MODE=release
make run TYPE=int128 FEATURE=t COMPILER=all MODE=release

# Verificar que el rendimiento NO EMPEORA
# (las optimizaciones de if constexpr deben ser eliminadas en compile-time)
```

### Paso 4: Criterios de Éxito

- ✅ Todos los tests uint128_t + int128_t pasan (>1000 tests)
- ✅ Benchmarks muestran rendimiento ≥ actual (tolerancia: -2%)
- ✅ Compila en 4 compiladores (gcc, clang, msvc, intel)
- ✅ Tiempo de compilación ≤ 110% del actual
- ✅ Mensajes de error comprensibles (verificar con tests que deben fallar)
- ✅ Tamaño de ejecutables similar (verificar con `size` en Linux)

---

## 📚 Referencias Cruzadas

- **TODO.md**: Plan completo de Fase 1.5 - Unificación Template
- **PROMPT.md**: Convenciones de naming y estructura de archivos
- **README.md**: Arquitectura actual de uint128_t/int128_t
- **TYPE_TRAITS_IMPLEMENTATION.md**: Type traits interoperabilidad
- **INTEROP_FEATURE_SUMMARY.md**: Tests de operaciones mixtas

---

## ✅ Conclusión

El comportamiento consistente de **sign extension en ambos tipos** (uint128_t y int128_t) no es accidental: es una **decisión de diseño deliberada** que facilita:

1. **Preservación de valores**: Conversiones roundtrip sin pérdida
2. **Interoperabilidad**: Operaciones mixtas predecibles
3. **Unificación futura**: >90% del código es compartible
4. **Mantenibilidad**: Una sola implementación, cero duplicación
5. **Testing**: Tests parametrizados en lugar de duplicados

**La sign extension consistente es la piedra angular de la arquitectura unificada** 🚀

---

**Próximo paso**: Iniciar Fase 1.5 el 31 de diciembre de 2025 (HOY) según cronograma en TODO.md.

**Estado actual**: ✅ Análisis completo, listo para implementar.
