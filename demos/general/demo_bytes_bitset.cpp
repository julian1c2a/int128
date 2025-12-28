/**
 * @file demo_bytes_bitset.cpp
 * @brief Demostración de conversiones a bytes y bitset con casos de uso prácticos
 *
 * Este demo muestra cómo usar las conversiones a std::array<std::byte, 16> y
 * std::bitset<128> para tareas comunes como serialización, manipulación de bits,
 * y análisis de datos binarios.
 */

#include "../include/int128.hpp"
#include <array>
#include <bitset>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace nstd;

// =============================================================================
// UTILIDADES PARA VISUALIZACIÓN
// =============================================================================

void print_bytes(const std::array<std::byte, 16>& bytes, const std::string& label)
{
    std::cout << label << ":\n  ";
    for (size_t i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i])
                  << " ";
        if ((i + 1) % 8 == 0)
            std::cout << "\n  ";
    }
    std::cout << std::dec << "\n";
}

void print_bitset_analysis(const std::bitset<128>& bits)
{
    std::cout << "  Bits activados: " << bits.count() << "/128\n";
    std::cout << "  Bit más alto (127): " << bits[127] << "\n";
    std::cout << "  Bit más bajo (0): " << bits[0] << "\n";

    // Mostrar primeros 32 bits
    std::cout << "  Bits [31:0]: ";
    for (int i = 31; i >= 0; --i) {
        std::cout << bits[i];
        if (i % 8 == 0 && i > 0)
            std::cout << " ";
    }
    std::cout << "\n";
}

// =============================================================================
// CASO DE USO 1: SERIALIZACIÓN PARA RED O DISCO
// =============================================================================

void demo_serialization()
{
    std::cout << "═══════════════════════════════════════════════════════════\n";
    std::cout << "CASO DE USO 1: Serialización y Deserialización\n";
    std::cout << "═══════════════════════════════════════════════════════════\n\n";

    // Crear datos
    uint128_t transaction_id(0x123456789ABCDEF0, 0xFEDCBA0987654321);
    std::cout << "1. Preparando transacción con ID: \n";
    std::cout << "   Hex: 0x" << std::hex << transaction_id.high() << std::setw(16)
              << std::setfill('0') << transaction_id.low() << std::dec << "\n\n";

    // Serializar a bytes
    auto serialized = transaction_id.to_bytes();
    print_bytes(serialized, "2. Datos serializados (little-endian)");

    // Simular envío/almacenamiento (copiar bytes)
    std::vector<std::byte> network_buffer(serialized.begin(), serialized.end());
    std::cout << "3. Datos enviados por red: " << network_buffer.size() << " bytes\n\n";

    // Deserializar
    std::array<std::byte, 16> received_data;
    std::copy(network_buffer.begin(), network_buffer.end(), received_data.begin());
    uint128_t recovered_id = uint128_t::from_bytes(received_data);

    std::cout << "4. ID recuperado: 0x" << std::hex << recovered_id.high() << std::setw(16)
              << std::setfill('0') << recovered_id.low() << std::dec << "\n";
    std::cout << "   ✓ Verificación: " << (transaction_id == recovered_id ? "EXITOSA" : "FALLIDA")
              << "\n\n";
}

// =============================================================================
// CASO DE USO 2: MANIPULACIÓN DE BITS (FLAGS Y PERMISOS)
// =============================================================================

void demo_bit_manipulation()
{
    std::cout << "═══════════════════════════════════════════════════════════\n";
    std::cout << "CASO DE USO 2: Sistema de Permisos con 128 Flags\n";
    std::cout << "═══════════════════════════════════════════════════════════\n\n";

    // Crear bitset de permisos
    std::bitset<128> permissions;

    // Activar permisos específicos
    permissions.set(0);   // READ
    permissions.set(1);   // WRITE
    permissions.set(4);   // EXECUTE
    permissions.set(10);  // ADMIN
    permissions.set(127); // SUPERUSER

    std::cout << "1. Permisos configurados:\n";
    std::cout << "   - Bit 0 (READ): ✓\n";
    std::cout << "   - Bit 1 (WRITE): ✓\n";
    std::cout << "   - Bit 4 (EXECUTE): ✓\n";
    std::cout << "   - Bit 10 (ADMIN): ✓\n";
    std::cout << "   - Bit 127 (SUPERUSER): ✓\n\n";

    // Convertir a uint128_t para almacenamiento eficiente
    uint128_t permissions_compact = uint128_t::from_bitset(permissions);
    std::cout << "2. Permisos compactados en uint128_t:\n";
    std::cout << "   High: 0x" << std::hex << permissions_compact.high() << "\n";
    std::cout << "   Low:  0x" << permissions_compact.low() << std::dec << "\n\n";

    // Recuperar y verificar un permiso específico
    auto restored_permissions = permissions_compact.to_bitset();
    std::cout << "3. Verificación de permisos:\n";
    std::cout << "   ¿Tiene permiso ADMIN (bit 10)? " << (restored_permissions[10] ? "SÍ" : "NO")
              << "\n";
    std::cout << "   ¿Tiene permiso DELETE (bit 2)? " << (restored_permissions[2] ? "SÍ" : "NO")
              << "\n\n";
}

// =============================================================================
// CASO DE USO 3: ANÁLISIS DE DATOS BINARIOS
// =============================================================================

void demo_binary_analysis()
{
    std::cout << "═══════════════════════════════════════════════════════════\n";
    std::cout << "CASO DE USO 3: Análisis de Datos Binarios\n";
    std::cout << "═══════════════════════════════════════════════════════════\n\n";

    // Datos de ejemplo (hash SHA-128 simulado)
    uint128_t hash(0xDEADBEEFCAFEBABE, 0x1234567890ABCDEF);

    std::cout << "1. Hash original:\n";
    std::cout << "   0x" << std::hex << hash.high() << std::setw(16) << std::setfill('0')
              << hash.low() << std::dec << "\n\n";

    // Convertir a bitset para análisis
    auto hash_bits = hash.to_bitset();

    std::cout << "2. Análisis de bits:\n";
    print_bitset_analysis(hash_bits);
    std::cout << "\n";

    // Calcular checksum simple (XOR de todos los bytes)
    auto hash_bytes = hash.to_bytes();
    uint8_t checksum = 0;
    for (const auto& byte : hash_bytes) {
        checksum ^= static_cast<uint8_t>(byte);
    }

    std::cout << "3. Checksum XOR: 0x" << std::hex << static_cast<int>(checksum) << std::dec
              << "\n\n";
}

// =============================================================================
// CASO DE USO 4: CONVERSIÓN ENTRE REPRESENTACIONES
// =============================================================================

void demo_representation_conversion()
{
    std::cout << "═══════════════════════════════════════════════════════════\n";
    std::cout << "CASO DE USO 4: Conversión entre Representaciones\n";
    std::cout << "═══════════════════════════════════════════════════════════\n\n";

    // Número con signo negativo (construir manualmente para evitar literal)
    int128_t signed_value = -int128_t(uint128_t(0xABCDEF, 0x1234567890ABCDEF));

    std::cout << "1. Valor con signo: " << signed_value.to_string() << "\n\n";

    // Ver representación two's complement en bitset
    auto bits = signed_value.to_bitset();
    std::cout << "2. Representación two's complement (bitset):\n";
    print_bitset_analysis(bits);
    std::cout << "\n";

    // Ver representación en bytes
    auto bytes = signed_value.to_bytes();
    print_bytes(bytes, "3. Representación en bytes (little-endian)");

    // Verificar que el MSB está en 1 (número negativo)
    std::cout << "4. Verificación del bit de signo:\n";
    std::cout << "   MSB del byte más alto: "
              << ((static_cast<uint8_t>(bytes[15]) & 0x80) ? "1" : "0") << " (número negativo)\n\n";

    // Convertir de vuelta
    int128_t recovered = int128_t::from_bytes(bytes);
    std::cout << "5. Valor recuperado: " << recovered.to_string() << "\n";
    std::cout << "   ✓ Verificación: " << (signed_value == recovered ? "EXITOSA" : "FALLIDA")
              << "\n\n";
}

// =============================================================================
// CASO DE USO 5: MÁSCARA DE BITS
// =============================================================================

void demo_bit_masking()
{
    std::cout << "═══════════════════════════════════════════════════════════\n";
    std::cout << "CASO DE USO 5: Operaciones con Máscaras de Bits\n";
    std::cout << "═══════════════════════════════════════════════════════════\n\n";

    uint128_t data(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // Todos los bits en 1

    std::cout << "1. Dato original: todos los bits en 1\n";
    std::cout << "   Bits activados: " << data.to_bitset().count() << "/128\n\n";

    // Crear máscara para limpiar bits 32-63
    std::bitset<128> mask;
    mask.set(); // Todos en 1
    for (int i = 32; i < 64; ++i) {
        mask.reset(i); // Bits 32-63 en 0
    }

    uint128_t mask_value = uint128_t::from_bitset(mask);
    uint128_t masked_data = data & mask_value;

    std::cout << "2. Aplicar máscara (limpiar bits 32-63):\n";
    std::cout << "   Bits activados después: " << masked_data.to_bitset().count() << "/128\n";
    std::cout << "   Low: 0x" << std::hex << masked_data.low() << std::dec << "\n\n";

    // Verificar que bits 32-63 están en 0
    auto result_bits = masked_data.to_bitset();
    bool mask_successful = true;
    for (int i = 32; i < 64; ++i) {
        if (result_bits[i]) {
            mask_successful = false;
            break;
        }
    }

    std::cout << "3. Verificación de máscara: " << (mask_successful ? "✓ EXITOSA" : "✗ FALLIDA")
              << "\n\n";
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║  DEMO: Conversiones a Bytes y Bitset - Casos Prácticos   ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    try {
        demo_serialization();
        demo_bit_manipulation();
        demo_binary_analysis();
        demo_representation_conversion();
        demo_bit_masking();

        std::cout << "═══════════════════════════════════════════════════════════\n";
        std::cout << "✓ Todos los demos ejecutados exitosamente\n";
        std::cout << "═══════════════════════════════════════════════════════════\n\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

