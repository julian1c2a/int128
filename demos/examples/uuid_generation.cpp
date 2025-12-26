/**
 * ===========================================================================
 * EJEMPLO: Generación y Manipulación de UUIDs (RFC 4122)
 * ===========================================================================
 *
 * Los UUIDs (Universally Unique Identifiers) son identificadores de 128 bits.
 * uint128_t es perfecto para almacenar y manipular UUIDs eficientemente.
 *
 * Este ejemplo implementa:
 * - UUID v4 (aleatorio)
 * - Parseo desde string
 * - Formato canónico (8-4-4-4-12)
 * - Operaciones de comparación y ordenamiento
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <uint128/uint128_iostreams.hpp>
#include <uint128/uint128_t.hpp>
#include <vector>

class UUID
{
  private:
    uint128_t value;

  public:
    // Constructor desde uint128_t
    UUID(uint128_t val = 0) : value(val) {}

    // Generar UUID v4 (aleatorio)
    static UUID generate_v4()
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist;

        uint64_t high = dist(gen);
        uint64_t low = dist(gen);

        // Aplicar bits de versión y variante según RFC 4122
        // Versión 4 (aleatorio): bits 12-15 del high = 0100
        high = (high & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;

        // Variante RFC 4122: bits 62-63 del low = 10
        low = (low & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

        return UUID((uint128_t(high) << 64) | low);
    }

    // Parsear desde string formato: "550e8400-e29b-41d4-a716-446655440000"
    static UUID from_string(const std::string& str)
    {
        std::string hex_only;
        for (char c : str) {
            if (std::isxdigit(c)) {
                hex_only += c;
            }
        }

        if (hex_only.length() != 32) {
            throw std::invalid_argument("UUID inválido");
        }

        // Parsear en dos partes de 64 bits
        uint64_t high = std::stoull(hex_only.substr(0, 16), nullptr, 16);
        uint64_t low = std::stoull(hex_only.substr(16, 16), nullptr, 16);

        return UUID((uint128_t(high) << 64) | low);
    }

    // Convertir a string formato canónico
    std::string to_string() const
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        uint64_t high = (value >> 64).low();
        uint64_t low = value.low();

        // Formato: 8-4-4-4-12
        oss << std::setw(8) << (high >> 32);
        oss << "-";
        oss << std::setw(4) << ((high >> 16) & 0xFFFF);
        oss << "-";
        oss << std::setw(4) << (high & 0xFFFF);
        oss << "-";
        oss << std::setw(4) << (low >> 48);
        oss << "-";
        oss << std::setw(12) << (low & 0xFFFFFFFFFFFFULL);

        return oss.str();
    }

    // Obtener versión del UUID
    int get_version() const
    {
        uint64_t high = (value >> 64).low();
        return (high >> 12) & 0xF;
    }

    // Obtener variante del UUID
    int get_variant() const
    {
        uint64_t low = value.low();
        if ((low & 0x8000000000000000ULL) == 0)
            return 0; // NCS
        if ((low & 0x4000000000000000ULL) == 0)
            return 1; // RFC 4122
        if ((low & 0x2000000000000000ULL) == 0)
            return 2; // Microsoft
        return 3;     // Reservado
    }

    // Operadores de comparación
    bool operator==(const UUID& other) const
    {
        return value == other.value;
    }

    bool operator!=(const UUID& other) const
    {
        return value != other.value;
    }

    bool operator<(const UUID& other) const
    {
        return value < other.value;
    }

    // UUID nulo (todos ceros)
    bool is_nil() const
    {
        return value == 0;
    }

    uint128_t get_raw() const
    {
        return value;
    }
};

void demo_generation()
{
    std::cout << "\n=== Generación de UUIDs ===\n\n";

    std::cout << "Generando 5 UUIDs v4 aleatorios:\n\n";

    for (int i = 0; i < 5; ++i) {
        UUID uuid = UUID::generate_v4();
        std::cout << "  " << (i + 1) << ". " << uuid.to_string();
        std::cout << " (v" << uuid.get_version() << ")\n";
    }

    std::cout << "\nNota: Los UUIDs v4 tienen una probabilidad de colisión\n";
    std::cout << "      extremadamente baja (~2^-122 para 2 UUIDs).\n";
}

void demo_parsing()
{
    std::cout << "\n=== Parseo desde String ===\n\n";

    std::vector<std::string> uuid_strings = {
        "550e8400-e29b-41d4-a716-446655440000",
        "6ba7b810-9dad-11d1-80b4-00c04fd430c8",
        "00000000-0000-0000-0000-000000000000",
    };

    for (const auto& str : uuid_strings) {
        try {
            UUID uuid = UUID::from_string(str);
            std::cout << "String: " << str << "\n";
            std::cout << "  Parseado: " << uuid.to_string() << "\n";
            std::cout << "  Versión: " << uuid.get_version() << "\n";
            std::cout << "  Variante: " << uuid.get_variant() << "\n";
            std::cout << "  Es nil: " << (uuid.is_nil() ? "Sí" : "No") << "\n\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n\n";
        }
    }
}

void demo_comparison()
{
    std::cout << "\n=== Comparación y Ordenamiento ===\n\n";

    // Generar varios UUIDs
    std::vector<UUID> uuids;
    for (int i = 0; i < 10; ++i) {
        uuids.push_back(UUID::generate_v4());
    }

    std::cout << "UUIDs generados (desordenados):\n";
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "  " << uuids[i].to_string() << "\n";
    }

    // Ordenar
    std::sort(uuids.begin(), uuids.end());

    std::cout << "\nUUIDs ordenados (primeros 5):\n";
    for (size_t i = 0; i < 5; ++i) {
        std::cout << "  " << uuids[i].to_string() << "\n";
    }

    // Verificar unicidad
    std::cout << "\n¿Todos únicos? ";
    auto last = std::unique(uuids.begin(), uuids.end());
    bool all_unique = (last == uuids.end());
    std::cout << (all_unique ? "✓ Sí" : "✗ No") << "\n";
}

void demo_storage_efficiency()
{
    std::cout << "\n=== Eficiencia de Almacenamiento ===\n\n";

    std::cout << "Comparación de representaciones:\n\n";

    UUID uuid = UUID::generate_v4();

    // Como uint128_t
    size_t size_uint128 = sizeof(uint128_t);
    std::cout << "  uint128_t:     " << size_uint128 << " bytes\n";

    // Como string
    std::string uuid_str = uuid.to_string();
    size_t size_string = uuid_str.length() + sizeof(std::string);
    std::cout << "  std::string:   ~" << size_string << " bytes\n";
    std::cout << "                  (más heap allocation overhead)\n\n";

    std::cout << "Ahorro: uint128_t es " << (size_string / size_uint128) << "x más eficiente\n";

    std::cout << "\nEn un vector de 1,000,000 UUIDs:\n";
    std::cout << "  uint128_t: ~" << (1000000 * size_uint128 / 1024 / 1024) << " MB\n";
    std::cout << "  string:    ~" << (1000000 * size_string / 1024 / 1024) << " MB\n";
}

void demo_practical_use()
{
    std::cout << "\n=== Uso Práctico: Base de Datos de Usuarios ===\n\n";

    struct User {
        UUID id;
        std::string name;

        User(const std::string& n) : id(UUID::generate_v4()), name(n) {}
    };

    std::vector<User> users = {
        User("Alice"), User("Bob"), User("Charlie"), User("Diana"), User("Eve"),
    };

    std::cout << "Usuarios registrados:\n\n";
    std::cout << std::left;
    std::cout << std::setw(40) << "UUID" << "Nombre\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& user : users) {
        std::cout << std::setw(40) << user.id.to_string() << user.name << "\n";
    }

    // Buscar por UUID
    std::cout << "\nBuscar usuario por UUID...\n";
    UUID target = users[2].id;

    auto it = std::find_if(users.begin(), users.end(),
                           [&target](const User& u) { return u.id == target; });

    if (it != users.end()) {
        std::cout << "✓ Usuario encontrado: " << it->name << "\n";
        std::cout << "  UUID: " << it->id.to_string() << "\n";
    }
}

int main()
{
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║   UUID GENERATION AND MANIPULATION                        ║
║   Usando uint128_t (RFC 4122)                             ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
)" << "\n";

    std::cout << "UUIDs son identificadores únicos de 128 bits.\n";
    std::cout << "uint128_t proporciona almacenamiento y operaciones eficientes.\n";

    demo_generation();
    demo_parsing();
    demo_comparison();
    demo_storage_efficiency();
    demo_practical_use();

    std::cout << "\n=== Ventajas de uint128_t para UUIDs ===\n\n";
    std::cout << "✓ Almacenamiento compacto (16 bytes)\n";
    std::cout << "✓ Comparación y ordenamiento rápidos\n";
    std::cout << "✓ Sin overhead de strings\n";
    std::cout << "✓ Operaciones bitwise eficientes\n";
    std::cout << "✓ Compatible con bases de datos (BINARY(16))\n";

    return 0;
}
