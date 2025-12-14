# API Reference - uint128_t Library

## Descripción General

`uint128_t` es una implementación completa de enteros sin signo de 128 bits compatible con C++20, diseñada para proporcionar alta precisión aritmética manteniendo la eficiencia y compatibilidad con la STL.

## Estructura Modular

La biblioteca está organizada en módulos especializados:

- **Core** (`uint128_t.hpp`): Implementación principal
- **Límites** (`uint128_limits.hpp`): std::numeric_limits especialization  
- **Traits** (`uint128_traits.hpp`): Type traits y std::hash
- **Concepts** (`uint128_concepts.hpp`): C++20 concepts
- **Algoritmos** (`uint128_algorithm.hpp`): STL algorithms optimizados
- **Numérico** (`uint128_numeric.hpp`): Funciones numéricas C++20 🆕

## Funciones Numéricas C++20 (`uint128_numeric.hpp`)

### Funciones Estándar

```cpp
// Punto medio entre dos valores (evita overflow)  
uint128_t result = midpoint(uint128_t(100), uint128_t(200));  // 150

// Restringe valor a un rango
uint128_t clamped = clamp(value, uint128_t(0), uint128_t(255));
```

### Manipulación de Bits

```cpp
// Contar bits activados
uint128_t count = popcount(value);

// Contar ceros a la izquierda/derecha
uint128_t leading_zeros = countl_zero(value);
uint128_t trailing_zeros = countr_zero(value);

// Ancho en bits (posición del bit más alto)
uint128_t width = bit_width(value);

// Comprobar si es potencia de 2
bool is_power_of_2 = has_single_bit(value);

// Siguiente/anterior potencia de 2
uint128_t next_pow2 = bit_ceil(value);
uint128_t prev_pow2 = bit_floor(value);
```

### Rotaciones y Utilidades

```cpp
// Rotar bits
uint128_t rotated_left = rotl(value, 5);
uint128_t rotated_right = rotr(value, 3);

// Intercambio de bytes
uint128_t swapped = byteswap(value);

// Utilidades matemáticas
uint128_t sqrt_val = isqrt(value);      // Raíz cuadrada entera
uint128_t power_result = power(a, b);   // Potencia
bool even = is_even(value);             // Paridad
uint128_t diff = abs_diff(a, b);        // Diferencia absoluta
```

## Constructores

### Constructor por Defecto
```cpp
uint128_t num;  // Inicializa a 0
```

### Constructor desde Enteros
```cpp
uint128_t num1(42);           // Desde entero pequeño
uint128_t num2(100, 200);     // high=100, low=200
```

### Constructor desde Cadenas
```cpp
uint128_t big("18446744073709551616");  // Números grandes
```

### Literales de Usuario
```cpp
auto small = 42_u128;
auto big = "999999999999999999999"_U128;
```

## Operadores Aritméticos

| Operador | Descripción    | Ejemplo      |
|----------|----------------|--------------|
| `+`      | Suma           | `a + b`      |
| `-`      | Resta          | `a - b`      |
| `*`      | Multiplicación | `a * b`      |
| `/`      | División       | `a / b`      |
| `%`      | Módulo         | `a % b`      |
| `++`     | Incremento     | `++a`, `a++` |
| `--`     | Decremento     | `--a`, `a--` |

## Operadores de Asignación

| Operador | Descripción          | Ejemplo   |
|----------|----------------------|-----------|
| `+=`     | Suma y asigna        | `a += b`  |
| `-=`     | Resta y asigna       | `a -= b`  |
| `*=`     | Multiplica y asigna  | `a *= b`  |
| `/=`     | Divide y asigna      | `a /= b`  |
| `%=`     | Módulo y asigna      | `a %= b`  |

## Operadores de Comparación

| Operador | Descripción   | Ejemplo   |
|----------|---------------|-----------|
| `==`     | Igualdad      | `a == b`  |
| `!=`     | Desigualdad   | `a != b`  |
| `<`      | Menor que     | `a < b`   |
| `<=`     | Menor o igual | `a <= b`  |
| `>`      | Mayor que     | `a > b`   |
| `>=`     | Mayor o igual | `a >= b`  |

## Operadores Bit a Bit

| Operador | Descripción             | Ejemplo  |
|----------|-------------------------|----------|
| `&`      | AND bit a bit           | `a & b`  |
| `\|`     | OR bit a bit            | `a \| b` |
| `^`      | XOR bit a bit           | `a ^ b`  |
| `~`      | NOT bit a bit           | `~a`     |
| `<<`     | Desplazamiento izquierda| `a << 5` |
| `>>`     | Desplazamiento derecha  | `a >> 3` |

## Métodos de Acceso

### Accessores
```cpp
uint64_t high_part = num.high();  // Obtener parte alta
uint64_t low_part = num.low();    // Obtener parte baja
num.set_high(value);              // Establecer parte alta
num.set_low(value);               // Establecer parte baja
```

### Conversiones
```cpp
std::string str = num.to_string();           // A string
uint128_t parsed = uint128_t::from_string(str); // Desde string
```

## Integración con STL

### numeric_limits
```cpp
auto max_val = std::numeric_limits<uint128_t>::max();
auto min_val = std::numeric_limits<uint128_t>::min();
bool is_integer = std::numeric_limits<uint128_t>::is_integer;
```

### Contenedores Hash
```cpp
std::unordered_map<uint128_t, std::string> map;
map[123_u128] = "value";
```

### Algoritmos
```cpp
std::vector<uint128_t> vec(10);
std::iota(vec.begin(), vec.end(), 1000_u128);
```

## Casos de Uso Prácticos

### Cálculos Financieros
```cpp
const auto WEI_PER_ETHER = "1000000000000000000"_u128;
auto transaction = 50_u128 * WEI_PER_ETHER;
```

### Identificadores Únicos
```cpp
uint128_t uuid_high = generate_random_64bit();
uint128_t uuid_low = generate_random_64bit();
uint128_t uuid(uuid_high, uuid_low);
```

### Contadores de Gran Rango
```cpp
uint128_t counter = 0_u128;
for (auto item : huge_dataset) {
    ++counter;
    process(item, counter);
}
```

## Performance

- **Tamaño**: 16 bytes (2 × uint64_t)
- **Alineación**: 8 bytes
- **Operaciones simples**: ~0.5-1.0 ns por operación
- **Comparaciones**: ~0.6-0.9 ns por operación
- **Operaciones bit a bit**: ~0.001-0.002 ns por operación

## Limitaciones

1. **División y Módulo**: Implementación completa pendiente para números grandes
2. **Conversión desde Float**: No implementada directamente
3. **Serialización Binaria**: No incluida en la implementación actual

## Compatibilidad

- **C++20** o superior requerido
- **MSVC 14.50+**, **GCC 10+**, **Clang 12+**
- **Windows**, **Linux**, **macOS**
- **x64**, **ARM64**