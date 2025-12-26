// Test de conversiones a bytes y bitset
#include "../include/int128.hpp"
#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << "=== Tests de Conversión uint128_t/int128_t ↔ bytes/bitset ===\n\n";

    // =========================================================================
    // TEST 1: uint128_t → bytes → uint128_t
    // =========================================================================
    std::cout << "Test 1: uint128_t → bytes → uint128_t (round-trip)\n";
    uint128_t u1(0x1234567890ABCDEF, 0xFEDCBA0987654321);

    auto bytes1 = u1.to_bytes();
    std::cout << "  Original: high=0x" << std::hex << u1.high() << " low=0x" << u1.low() << std::dec
              << "\n";
    std::cout << "  Bytes (primeros 8): ";
    for (int i = 0; i < 8; ++i) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(bytes1[i]) << " ";
    }
    std::cout << std::dec << "\n";

    uint128_t u1_restored = uint128_t::from_bytes(bytes1);
    std::cout << "  Restaurado: high=0x" << std::hex << u1_restored.high() << " low=0x"
              << u1_restored.low() << std::dec << "\n";
    assert(u1 == u1_restored);
    std::cout << "  ✓ Round-trip exitoso\n\n";

    // =========================================================================
    // TEST 2: uint128_t → bitset → uint128_t
    // =========================================================================
    std::cout << "Test 2: uint128_t → bitset → uint128_t (round-trip)\n";
    uint128_t u2(0xF, 0xFF);

    auto bitset2 = u2.to_bitset();
    std::cout << "  Original: high=0x" << std::hex << u2.high() << " low=0x" << u2.low() << std::dec
              << "\n";
    std::cout << "  Bitset LSB (bits 0-15): ";
    for (int i = 0; i < 16; ++i) {
        std::cout << bitset2[i];
    }
    std::cout << "\n";

    uint128_t u2_restored = uint128_t::from_bitset(bitset2);
    std::cout << "  Restaurado: high=0x" << std::hex << u2_restored.high() << " low=0x"
              << u2_restored.low() << std::dec << "\n";
    assert(u2 == u2_restored);
    std::cout << "  ✓ Round-trip exitoso\n\n";

    // =========================================================================
    // TEST 3: int128_t positivo → bytes → int128_t
    // =========================================================================
    std::cout << "Test 3: int128_t positivo → bytes → int128_t\n";
    int128_t i1(12345);

    auto bytes3 = i1.to_bytes();
    std::cout << "  Original: " << i1.to_string() << "\n";

    int128_t i1_restored = int128_t::from_bytes(bytes3);
    std::cout << "  Restaurado: " << i1_restored.to_string() << "\n";
    assert(i1 == i1_restored);
    std::cout << "  ✓ Round-trip exitoso\n\n";

    // =========================================================================
    // TEST 4: int128_t negativo → bytes → int128_t
    // =========================================================================
    std::cout << "Test 4: int128_t negativo → bytes → int128_t (two's complement)\n";
    int128_t i2(-12345);

    auto bytes4 = i2.to_bytes();
    std::cout << "  Original: " << i2.to_string() << "\n";
    std::cout << "  Bytes[15] (MSB): 0x" << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(bytes4[15]) << std::dec << " (debe ser 0xFF para negativo)\n";

    int128_t i2_restored = int128_t::from_bytes(bytes4);
    std::cout << "  Restaurado: " << i2_restored.to_string() << "\n";
    assert(i2 == i2_restored);
    std::cout << "  ✓ Round-trip exitoso\n\n";

    // =========================================================================
    // TEST 5: int128_t → bitset → int128_t
    // =========================================================================
    std::cout << "Test 5: int128_t negativo → bitset → int128_t\n";
    int128_t i3(-1);

    auto bitset5 = i3.to_bitset();
    std::cout << "  Original: " << i3.to_string() << "\n";
    std::cout << "  Bitset count: " << bitset5.count() << " (debe ser 128 para -1)\n";
    std::cout << "  Bit 127 (signo): " << bitset5[127] << "\n";

    int128_t i3_restored = int128_t::from_bitset(bitset5);
    std::cout << "  Restaurado: " << i3_restored.to_string() << "\n";
    assert(i3 == i3_restored);
    std::cout << "  ✓ Round-trip exitoso\n\n";

    // =========================================================================
    // TEST 6: Manipulación directa de bitset
    // =========================================================================
    std::cout << "Test 6: Construir desde bitset manipulado\n";
    std::bitset<128> custom_bits;
    custom_bits.set(0);   // Bit 0 = 1
    custom_bits.set(64);  // Bit 64 = 1
    custom_bits.set(127); // Bit 127 = 1 (signo negativo)

    int128_t i4 = int128_t::from_bitset(custom_bits);
    std::cout << "  Bits activos: 0, 64, 127\n";
    std::cout << "  Valor resultante: " << i4.to_string() << "\n";
    std::cout << "  Es negativo: " << (i4.is_negative() ? "sí" : "no") << "\n";
    std::cout << "  ✓ Conversión desde bitset manual exitosa\n\n";

    // =========================================================================
    // TEST 7: Bytes en little-endian
    // =========================================================================
    std::cout << "Test 7: Verificar orden little-endian en bytes\n";
    uint128_t u3(0, 0x0102030405060708);
    auto bytes7 = u3.to_bytes();

    std::cout << "  Valor: 0x0102030405060708\n";
    std::cout << "  bytes[0] = 0x" << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(bytes7[0]) << " (debe ser 0x08)\n";
    std::cout << "  bytes[7] = 0x" << std::setw(2) << std::setfill('0')
              << static_cast<int>(bytes7[7]) << std::dec << " (debe ser 0x01)\n";

    assert(bytes7[0] == std::byte{0x08});
    assert(bytes7[7] == std::byte{0x01});
    std::cout << "  ✓ Orden little-endian correcto\n\n";

    std::cout << "✓ Todos los tests de conversión bytes/bitset ejecutados correctamente\n";
    return 0;
}
