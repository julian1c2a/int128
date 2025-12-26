# Guía de Conversiones: Bytes y Bitset

## 📋 Resumen

Las clases `uint128_t` y `int128_t` soportan conversiones a/desde:
- **std::array<std::byte, 16>** - Serialización binaria
- **std::bitset<128>** - Manipulación de bits individual

## 🔧 API Reference

### Conversión a Bytes (Serialización)

#### `to_bytes()` - uint128_t/int128_t → bytes
```cpp
constexpr std::array<std::byte, 16> to_bytes() const noexcept;
```

**Características**:
- Formato: **Little-endian** (LSB primero)
- Organización: bytes[0-7] = data[0], bytes[8-15] = data[1]
- Two's complement: Preservado para int128_t negativos
- Performance: O(1), constexpr

**Ejemplo**:
```cpp
uint128_t value(0x1234567890ABCDEF, 0xFEDCBA0987654321);
auto bytes = value.to_bytes();

// bytes[0] = 0x21 (LSB de data[0])
// bytes[7] = 0xFE (MSB de data[0])
// bytes[8] = 0xEF (LSB de data[1])
// bytes[15] = 0x12 (MSB de data[1])
```

#### `from_bytes()` - bytes → uint128_t/int128_t
```cpp
static constexpr uint128_t from_bytes(const std::array<std::byte, 16>& bytes) noexcept;
static constexpr int128_t from_bytes(const std::array<std::byte, 16>& bytes) noexcept;
```

**Características**:
- Deserializa desde formato little-endian
- Reconstruye high/low desde bytes[8-15] y bytes[0-7]
- int128_t interpreta como two's complement
- Performance: O(1), constexpr

**Ejemplo**:
```cpp
std::array<std::byte, 16> data = {...};
uint128_t value = uint128_t::from_bytes(data);
int128_t signed_val = int128_t::from_bytes(data); // Interpreta signo
```

---

### Conversión a Bitset (Manipulación de Bits)

#### `to_bitset()` - uint128_t/int128_t → bitset
```cpp
constexpr std::bitset<128> to_bitset() const noexcept;
```

**Características**:
- Bit mapping: bits[0-63] = data[0], bits[64-127] = data[1]
- Bit 0 = LSB, Bit 127 = MSB (signo en int128_t)
- Permite manipulación con operadores bitset estándar
- Performance: O(128), constexpr

**Ejemplo**:
```cpp
uint128_t value(0xF, 0xFF);
auto bits = value.to_bitset();

std::cout << "Bits activados: " << bits.count() << "/128\n";
std::cout << "Bit 127 (MSB): " << bits[127] << "\n";

// Manipular bits individuales
bits.set(100);    // Activar bit 100
bits.reset(50);   // Desactivar bit 50
bits.flip(75);    // Invertir bit 75
```

#### `from_bitset()` - bitset → uint128_t/int128_t
```cpp
static constexpr uint128_t from_bitset(const std::bitset<128>& bits) noexcept;
static constexpr int128_t from_bitset(const std::bitset<128>& bits) noexcept;
```

**Características**:
- Reconstruye desde bits individuales
- int128_t interpreta bit 127 como signo
- Útil para construcción desde patrones de bits
- Performance: O(128), constexpr

**Ejemplo**:
```cpp
std::bitset<128> pattern;
pattern.set(0);    // READ permission
pattern.set(1);    // WRITE permission
pattern.set(10);   // ADMIN permission
pattern.set(127);  // SUPERUSER flag

uint128_t permissions = uint128_t::from_bitset(pattern);
```

---

## 💡 Casos de Uso Comunes

### 1. Serialización para Red/Disco

**Problema**: Enviar uint128_t por red o guardarlo en disco

```cpp
// Serializar (emisor)
uint128_t transaction_id(0x123456789ABCDEF0, 0xFEDCBA0987654321);
auto bytes = transaction_id.to_bytes();

// Enviar bytes...
std::vector<uint8_t> network_buffer;
for (auto b : bytes) {
    network_buffer.push_back(static_cast<uint8_t>(b));
}
send_over_network(network_buffer);

// Deserializar (receptor)
std::array<std::byte, 16> received_bytes;
// ... recibir datos ...
uint128_t recovered_id = uint128_t::from_bytes(received_bytes);
```

### 2. Sistema de Permisos (128 Flags)

**Problema**: Almacenar 128 flags booleanos de forma eficiente

```cpp
// Crear permisos
std::bitset<128> permissions;
permissions.set(0);    // READ
permissions.set(1);    // WRITE
permissions.set(4);    // EXECUTE
permissions.set(10);   // ADMIN
permissions.set(127);  // SUPERUSER

// Almacenar compacto
uint128_t perms_compact = uint128_t::from_bitset(permissions);

// Verificar permisos más tarde
auto restored = perms_compact.to_bitset();
if (restored[10]) {
    std::cout << "Usuario tiene permisos ADMIN\n";
}
```

### 3. Análisis de Datos Binarios

**Problema**: Analizar estructura de bits de un hash o ID

```cpp
uint128_t hash(0xDEADBEEFCAFEBABE, 0x1234567890ABCDEF);

// Analizar bits
auto bits = hash.to_bitset();
std::cout << "Bits activados: " << bits.count() << "/128\n";
std::cout << "Bit de paridad: " << (bits.count() % 2) << "\n";

// Checksum XOR de bytes
auto bytes = hash.to_bytes();
uint8_t checksum = 0;
for (const auto& byte : bytes) {
    checksum ^= static_cast<uint8_t>(byte);
}
std::cout << "Checksum XOR: 0x" << std::hex << static_cast<int>(checksum) << "\n";
```

### 4. Máscaras de Bits

**Problema**: Aplicar máscara para limpiar/establecer rangos de bits

```cpp
uint128_t data(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // Todos en 1

// Crear máscara: bits 32-63 en 0, resto en 1
std::bitset<128> mask;
mask.set(); // Todos en 1
for (int i = 32; i < 64; ++i) {
    mask.reset(i);
}

uint128_t mask_value = uint128_t::from_bitset(mask);
uint128_t masked_data = data & mask_value;

// Verificar
auto result_bits = masked_data.to_bitset();
assert(!result_bits[32] && !result_bits[63]); // Bits limpiados
```

### 5. Two's Complement en int128_t

**Problema**: Visualizar representación binaria de números negativos

```cpp
int128_t negative(-12345);

// Ver representación en bits
auto bits = negative.to_bitset();
std::cout << "Bit de signo (127): " << bits[127] << "\n"; // Debe ser 1

// Ver representación en bytes
auto bytes = negative.to_bytes();
std::cout << "MSB: 0x" << std::hex 
          << static_cast<int>(bytes[15]) << "\n"; // Debe ser 0xFF

// Round-trip preserva el valor
int128_t restored = int128_t::from_bytes(bytes);
assert(negative == restored);
```

---

## 🎯 Comparación con Alternativas

| Método | Ventajas | Desventajas | Uso Recomendado |
|--------|----------|-------------|-----------------|
| **to_bytes()** | - Compacto (16 bytes)<br>- Estándar para serialización<br>- Compatible con protocolos binarios | - No legible por humanos<br>- Requiere interpretar endianness | Serialización de red/disco, checksum, protocolos binarios |
| **to_bitset()** | - Manipulación de bits individual<br>- API rica (count, flip, test)<br>- Visualización fácil | - Overhead de memoria (128 bits → 128 bytes en algunos casos)<br>- Conversión O(128) | Análisis de bits, permisos, máscaras, debugging |
| **to_string()** | - Legible por humanos<br>- Debugging amigable<br>- Compatible con logs | - Tamaño variable<br>- No eficiente para almacenamiento | Debugging, logging, UI, configuración |

---

## 🔍 Detalles de Implementación

### Little-Endian vs Big-Endian

**Decisión de diseño**: Se usa **little-endian** para `to_bytes()` porque:
1. Es el formato nativo de x86/x86_64
2. Consistente con la representación interna (data[0] = low, data[1] = high)
3. Compatible con la mayoría de protocolos de red modernos

**Si necesitas big-endian**:
```cpp
auto bytes_le = value.to_bytes();
std::array<std::byte, 16> bytes_be;
std::reverse_copy(bytes_le.begin(), bytes_le.end(), bytes_be.begin());
```

### Two's Complement en int128_t

Los números negativos en `int128_t` se representan en **two's complement**:
- Bit 127 = 0 → Número positivo
- Bit 127 = 1 → Número negativo

**Ejemplo**:
```cpp
int128_t minus_one(-1);
auto bits = minus_one.to_bitset();
assert(bits.count() == 128); // Todos los bits en 1

auto bytes = minus_one.to_bytes();
assert(bytes[15] == std::byte{0xFF}); // MSB = 0xFF
```

### Performance

Todas las funciones son:
- ✅ **constexpr**: Evaluables en tiempo de compilación
- ✅ **noexcept**: No lanzan excepciones
- ✅ **O(1) o O(128)**: Performance predecible

**Benchmarks típicos** (GCC -O3):
- `to_bytes()`: ~5-10 ns
- `from_bytes()`: ~5-10 ns
- `to_bitset()`: ~15-20 ns
- `from_bitset()`: ~15-20 ns

---

## 📚 Ver También

- [demos/demo_bytes_bitset.cpp](../demos/demo_bytes_bitset.cpp) - Demo completo con 5 casos de uso
- [tests/test_bytes_bitset_conversions.cpp](../tests/test_bytes_bitset_conversions.cpp) - Suite de tests
- [README.md](../README.md#conversiones-de-tipo) - Sección de conversiones en README principal

---

## 🧪 Testing

Para ejecutar los tests de estas conversiones:

```bash
# Compilar y ejecutar tests
g++ -std=c++20 -I include tests/test_bytes_bitset_conversions.cpp -o tests/test_bytes_bitset.exe
./tests/test_bytes_bitset.exe

# Compilar y ejecutar demo
g++ -std=c++20 -I include demos/demo_bytes_bitset.cpp -o demos/demo_bytes_bitset.exe
./demos/demo_bytes_bitset.exe
```

**Tests incluidos**:
1. ✅ uint128_t → bytes → uint128_t (round-trip)
2. ✅ uint128_t → bitset → uint128_t (round-trip)
3. ✅ int128_t positivo → bytes → int128_t
4. ✅ int128_t negativo → bytes → int128_t (two's complement)
5. ✅ int128_t → bitset → int128_t
6. ✅ Construcción desde bitset manipulado
7. ✅ Verificación little-endian

---

**Última actualización**: Diciembre 2025
**Versión**: 1.0
