/**
 * ===========================================================================
 * EJEMPLO: Direcciones IPv6 con uint128_t
 * ===========================================================================
 *
 * Este ejemplo muestra cómo usar uint128_t para manejar direcciones IPv6.
 * Una dirección IPv6 es un entero de 128 bits, perfecto para uint128_t.
 *
 * Funcionalidades demostradas:
 * - Parseo de direcciones IPv6 desde string
 * - Conversión a notación estándar
 * - Operaciones de red (máscaras, subredes)
 * - Verificación de tipos de direcciones
 * - Cálculo de rangos de red
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <int128_simple.hpp>
#include <vector>

using namespace nstd;

class IPv6Address
{
private:
    uint128_t address;

public:
    // Constructor desde uint128_t
    IPv6Address(uint128_t addr = uint128_t{0}) : address(addr) {}

    // Constructor desde string
    static IPv6Address from_string(const std::string &str)
    {
        // Simplificación: parseo básico de formato completo
        // Ejemplo: "2001:0db8:85a3:0000:0000:8a2e:0370:7334"

        uint128_t result{0};
        std::string temp = str;

        // Expandir :: si existe
        size_t double_colon = temp.find("::");
        if (double_colon != std::string::npos)
        {
            // Simplificación: asumimos solo un :: y está al principio
            temp = "0000" + temp.substr(2);
        }

        // Parsear cada componente hexadecimal
        std::istringstream iss(temp);
        std::string segment;
        int position = 7;

        while (std::getline(iss, segment, ':') && position >= 0)
        {
            if (segment.empty())
                segment = "0";

            uint64_t value = std::stoull(segment, nullptr, 16);
            result |= (uint128_t{value} << (position * 16));
            position--;
        }

        return IPv6Address(result);
    }

    // Convertir a string en formato estándar
    std::string to_string() const
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        for (int i = 7; i >= 0; i--)
        {
            uint16_t segment = static_cast<uint16_t>((address >> (i * 16)).low() & 0xFFFF);
            oss << std::setw(4) << segment;
            if (i > 0)
                oss << ":";
        }

        return oss.str();
    }

    // Formato comprimido (eliminar ceros a la izquierda)
    std::string to_compressed_string() const
    {
        std::ostringstream oss;
        oss << std::hex;

        bool first = true;
        for (int i = 7; i >= 0; i--)
        {
            uint16_t segment = static_cast<uint16_t>((address >> (i * 16)).low() & 0xFFFF);
            if (!first)
                oss << ":";
            oss << segment;
            first = false;
        }

        return oss.str();
    }

    // Aplicar máscara de red
    IPv6Address apply_mask(int prefix_length) const
    {
        if (prefix_length >= 128)
            return *this;
        if (prefix_length <= 0)
            return IPv6Address{uint128_t{0}};

        uint128_t mask = (~uint128_t{0}) << (128 - prefix_length);
        return IPv6Address(address & mask);
    }

    // Obtener dirección de red
    IPv6Address network_address(int prefix_length) const
    {
        return apply_mask(prefix_length);
    }

    // Obtener dirección de broadcast (último host en la subred)
    IPv6Address broadcast_address(int prefix_length) const
    {
        uint128_t mask = (~uint128_t{0}) << (128 - prefix_length);
        uint128_t host_mask = ~mask;
        return IPv6Address((address & mask) | host_mask);
    }

    // Verificar si está en una subred
    bool in_subnet(const IPv6Address &network, int prefix_length) const
    {
        IPv6Address my_network = apply_mask(prefix_length);
        IPv6Address other_network = network.apply_mask(prefix_length);
        return my_network.address == other_network.address;
    }

    // Tipos de direcciones IPv6
    bool is_loopback() const
    {
        return address == uint128_t{1}; // ::1
    }

    bool is_link_local() const
    {
        // fe80::/10
        uint128_t prefix = address >> 118;
        return prefix == 0x3fa; // fe80 >> 6
    }

    bool is_multicast() const
    {
        // ff00::/8
        uint128_t prefix = address >> 120;
        return prefix == 0xff;
    }

    bool is_global_unicast() const
    {
        // 2000::/3
        uint128_t prefix = address >> 125;
        return prefix == 1;
    }

    // Obtener siguiente dirección
    IPv6Address next() const
    {
        return IPv6Address{address + uint128_t{1}};
    }

    // Obtener dirección anterior
    IPv6Address prev() const
    {
        return IPv6Address{address - uint128_t{1}};
    }

    // Operadores
    bool operator==(const IPv6Address &other) const
    {
        return address == other.address;
    }

    bool operator<(const IPv6Address &other) const
    {
        return address < other.address;
    }

    uint128_t get_raw() const
    {
        return address;
    }
};

void demo_basic_operations()
{
    std::cout << "\n=== Operaciones Básicas con IPv6 ===\n\n";

    // Crear direcciones
    auto addr1 = IPv6Address::from_string("2001:0db8:85a3:0000:0000:8a2e:0370:7334");
    auto addr2 = IPv6Address::from_string("::1");     // Loopback
    auto addr3 = IPv6Address::from_string("fe80::1"); // Link-local

    std::cout << "Dirección 1 (completa):  " << addr1.to_string() << "\n";
    std::cout << "Dirección 1 (comprimida): " << addr1.to_compressed_string() << "\n\n";

    std::cout << "Dirección 2 (loopback):  " << addr2.to_string() << "\n";
    std::cout << "Es loopback? " << (addr2.is_loopback() ? "Sí" : "No") << "\n\n";

    std::cout << "Dirección 3 (link-local): " << addr3.to_string() << "\n";
    std::cout << "Es link-local? " << (addr3.is_link_local() ? "Sí" : "No") << "\n";
}

void demo_subnet_operations()
{
    std::cout << "\n=== Operaciones de Subred ===\n\n";

    auto addr = IPv6Address::from_string("2001:0db8:85a3:1234:5678:8a2e:0370:7334");
    int prefix = 64;

    std::cout << "Dirección original: " << addr.to_string() << "/" << prefix << "\n\n";

    auto network = addr.network_address(prefix);
    auto broadcast = addr.broadcast_address(prefix);

    std::cout << "Dirección de red:       " << network.to_string() << "\n";
    std::cout << "Último host (broadcast): " << broadcast.to_string() << "\n\n";

    // Calcular número de hosts en la subred
    uint128_t num_hosts = uint128_t(1) << (128 - prefix);
    std::cout << "Número de direcciones en la subred: " << num_hosts << "\n";
    std::cout << "(Suficiente para " << num_hosts << " dispositivos)\n";
}

void demo_range_iteration()
{
    std::cout << "\n=== Iteración de Rango ===\n\n";

    auto start = IPv6Address::from_string("2001:db8::1");
    auto end = IPv6Address::from_string("2001:db8::10");

    std::cout << "Primeras 10 direcciones desde " << start.to_compressed_string() << ":\n\n";

    IPv6Address current = start;
    for (int i = 0; i < 10 && current.get_raw() <= end.get_raw(); ++i)
    {
        std::cout << "  " << (i + 1) << ". " << current.to_compressed_string() << "\n";
        current = current.next();
    }
}

void demo_address_types()
{
    std::cout << "\n=== Tipos de Direcciones IPv6 ===\n\n";

    struct TestAddress
    {
        std::string addr;
        std::string type;
    };

    std::vector<TestAddress> addresses = {
        {"::1", "Loopback"},
        {"fe80::1", "Link-local"},
        {"ff02::1", "Multicast"},
        {"2001:db8::1", "Global Unicast"},
        {"fd00::1", "Unique Local"},
    };

    for (const auto &test : addresses)
    {
        auto addr = IPv6Address::from_string(test.addr);

        std::cout << test.addr << " (" << test.type << "):\n";
        std::cout << "  Completa:      " << addr.to_string() << "\n";
        std::cout << "  Loopback:      " << (addr.is_loopback() ? "✓" : "✗") << "\n";
        std::cout << "  Link-local:    " << (addr.is_link_local() ? "✓" : "✗") << "\n";
        std::cout << "  Multicast:     " << (addr.is_multicast() ? "✓" : "✗") << "\n";
        std::cout << "  Global Unicast: " << (addr.is_global_unicast() ? "✓" : "✗") << "\n\n";
    }
}

void demo_subnet_membership()
{
    std::cout << "\n=== Verificación de Pertenencia a Subred ===\n\n";

    auto network = IPv6Address::from_string("2001:db8::");
    int prefix = 32;

    std::cout << "Red: " << network.to_compressed_string() << "/" << prefix << "\n\n";

    std::vector<std::string> test_addresses = {
        "2001:db8::1",
        "2001:db8:1::1",
        "2001:db8:ffff:ffff:ffff:ffff:ffff:ffff",
        "2001:db9::1",
        "2001:0:0:1::1",
    };

    for (const auto &addr_str : test_addresses)
    {
        auto addr = IPv6Address::from_string(addr_str);
        bool in_net = addr.in_subnet(network, prefix);

        std::cout << "  " << addr_str << ": ";
        std::cout << (in_net ? "✓ En la red" : "✗ Fuera de la red") << "\n";
    }
}

int main()
{
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║   IPv6 ADDRESS MANAGEMENT                                 ║
║   Usando uint128_t                                        ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
)" << "\n";

    std::cout << "IPv6 usa direcciones de 128 bits.\n";
    std::cout << "uint128_t es perfecto para representar direcciones IPv6.\n";

    demo_basic_operations();
    demo_subnet_operations();
    demo_range_iteration();
    demo_address_types();
    demo_subnet_membership();

    std::cout << "\n=== Ventajas de usar uint128_t para IPv6 ===\n\n";
    std::cout << "✓ Representación nativa de 128 bits\n";
    std::cout << "✓ Operaciones bitwise eficientes para máscaras\n";
    std::cout << "✓ Comparaciones y ordenamiento natural\n";
    std::cout << "✓ Aritmética para rangos e iteración\n";
    std::cout << "✓ Sin overhead de strings o arrays\n";

    return 0;
}
