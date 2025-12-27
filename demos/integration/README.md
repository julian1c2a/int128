# Integration Demos - int128 Library

Ejemplos de integración de `uint128_t`/`int128_t` con bibliotecas y ecosistemas populares.

## 🎯 Propósito

Demostrar cómo usar int128 en aplicaciones del mundo real integrándose con:
- Bibliotecas de serialización
- Bases de datos
- Frameworks de red
- Sistemas de mensajería
- Otras herramientas comunes

## 📂 Demos Planificados

### 🔜 Serialización

#### **`json_nlohmann.cpp`** - JSON con nlohmann/json
```cpp
// Serialización/deserialización de uint128_t a JSON
#include <nlohmann/json.hpp>
#include "int128.hpp"

// Custom serializer para uint128_t
namespace nlohmann {
    template<>
    struct adl_serializer<uint128_t> {
        static void to_json(json& j, const uint128_t& value) {
            j = value.to_string();  // Como string para preservar precisión
        }
        
        static void from_json(const json& j, uint128_t& value) {
            value = uint128_t::from_string(j.get<std::string>());
        }
    };
}
```

#### **`cereal_serialization.cpp`** - Serialización con Cereal
- Serialización binaria
- Serialización JSON/XML
- Archives portables

#### **`protobuf_example.cpp`** - Protocol Buffers
- Definir mensajes con uint128
- Conversión a/desde bytes
- Interoperabilidad con otros lenguajes

### 🔜 Bases de Datos

#### **`sqlite_storage.cpp`** - SQLite integration
```cpp
// Almacenar uint128_t en SQLite
// Opciones:
// 1. Como TEXT (precisión completa, más espacio)
// 2. Como BLOB (16 bytes, eficiente)
// 3. Como dos INTEGER (high/low 64-bit)

void store_uint128(sqlite3* db, uint128_t value) {
    // Opción 1: TEXT
    sqlite3_bind_text(stmt, 1, value.to_string().c_str(), -1, SQLITE_TRANSIENT);
    
    // Opción 2: BLOB
    auto bytes = value.to_bytes();
    sqlite3_bind_blob(stmt, 1, bytes.data(), bytes.size(), SQLITE_TRANSIENT);
}
```

#### **`postgresql_example.cpp`** - PostgreSQL
- Usar tipo NUMERIC o BYTEA
- Stored procedures
- Conversiones eficientes

### 🔜 Networking

#### **`asio_example.cpp`** - Boost.Asio
```cpp
// Enviar/recibir uint128_t por red
#include <boost/asio.hpp>

void send_uint128(tcp::socket& socket, uint128_t value) {
    // Network byte order (big-endian)
    std::array<uint8_t, 16> bytes = value.to_bytes();
    std::reverse(bytes.begin(), bytes.end());  // host to network
    boost::asio::write(socket, boost::asio::buffer(bytes));
}
```

#### **`grpc_service.cpp`** - gRPC Service
- Definir servicio con uint128
- Cliente/servidor
- Streaming

### 🔜 Message Queues

#### **`zeromq_example.cpp`** - ZeroMQ
- Pub/Sub con uint128_t
- Request/Reply
- Serialización eficiente

#### **`kafka_producer.cpp`** - Apache Kafka
- Producer/Consumer
- Key serialization
- Avro schema

### 🔜 File Formats

#### **`csv_export.cpp`** - CSV import/export
- Leer/escribir CSV con uint128 columns
- Handling de números grandes
- Formatos customizables

#### **`parquet_example.cpp`** - Apache Parquet
- Columnar storage
- Compression
- Big data analytics

### 🔜 Crypto & Security

#### **`openssl_integration.cpp`** - OpenSSL
- Big number conversions
- Hash functions
- RSA key generation (toy example)

#### **`libsodium_example.cpp`** - libsodium
- Crypto-secure random uint128
- Hash-based operations

## 🚀 Compilación

Cada demo tiene sus propias dependencias. Ejemplos:

```bash
# nlohmann/json (header-only)
g++ -std=c++20 -O2 -I include -I/path/to/nlohmann \
    demos/integration/json_nlohmann.cpp \
    -o build/demos/integration/json_nlohmann

# SQLite
g++ -std=c++20 -O2 -I include \
    demos/integration/sqlite_storage.cpp \
    -o build/demos/integration/sqlite_storage \
    -lsqlite3

# Boost.Asio
g++ -std=c++20 -O2 -I include -I/path/to/boost \
    demos/integration/asio_example.cpp \
    -o build/demos/integration/asio_example \
    -pthread

# gRPC (más complejo, requiere protoc)
protoc --cpp_out=. int128_service.proto
g++ -std=c++20 -O2 -I include \
    demos/integration/grpc_service.cpp \
    int128_service.pb.cc \
    -o build/demos/integration/grpc_service \
    -lgrpc++ -lprotobuf
```

## 📋 Requisitos por Demo

| Demo | Biblioteca | Instalación | Licencia |
|------|-----------|-------------|----------|
| json_nlohmann | nlohmann/json | Header-only | MIT |
| cereal | Cereal | Header-only | BSD-3 |
| sqlite | SQLite3 | Sistema/apt | Public Domain |
| asio | Boost.Asio | apt/vcpkg | Boost |
| grpc | gRPC | apt/vcpkg | Apache 2.0 |
| zeromq | ZeroMQ | apt/vcpkg | MPL-2.0 |
| openssl | OpenSSL | Sistema/apt | Apache-style |

## 🎯 Patrones Comunes

### 1. Serialización a String (Precisión Completa)
```cpp
// Para JSON, XML, text protocols
std::string serialize(uint128_t value) {
    return value.to_string();  // Base 10 por defecto
}

uint128_t deserialize(const std::string& str) {
    return uint128_t::from_string(str);
}
```

### 2. Serialización Binaria (Eficiencia)
```cpp
// Para protobuf, binary formats, network
std::array<std::byte, 16> serialize_binary(uint128_t value) {
    return value.to_bytes();  // Little-endian
}

uint128_t deserialize_binary(const std::array<std::byte, 16>& bytes) {
    return uint128_t::from_bytes(bytes);
}
```

### 3. Conversión High/Low (Compatibility)
```cpp
// Para sistemas que soportan solo 64-bit
struct Uint128Parts {
    uint64_t high;
    uint64_t low;
};

Uint128Parts split(uint128_t value) {
    return { value.high(), value.low() };
}

uint128_t combine(Uint128Parts parts) {
    return uint128_t(parts.high, parts.low);
}
```

## 📝 Notas de Implementación

### Endianness
- `to_bytes()` usa **little-endian** por defecto
- Para network byte order (big-endian), invertir bytes
- Documentar claramente en cada integración

### Precisión vs Eficiencia
- String serialization: Precisión completa, más bytes
- Binary serialization: Eficiente, requiere endianness handling
- Split high/low: Compatible, puede requerir dos campos

### Error Handling
- Overflow en conversiones (uint128 → uint64)
- Parse errors en deserialización
- Validación de ranges en APIs externas

## 🤝 Contribuciones

¡Demos de integración con otras bibliotecas son especialmente bienvenidos! 

Prioridades:
1. Bibliotecas populares (Boost, etc.)
2. Protocolos estándar (JSON, Protobuf)
3. Bases de datos comunes (PostgreSQL, MySQL)
4. Casos de uso reales de la comunidad
