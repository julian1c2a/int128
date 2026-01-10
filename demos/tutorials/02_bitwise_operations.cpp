/**
 * Tutorial 02: Operaciones Bitwise
 *
 * Este tutorial muestra operaciones a nivel de bits con uint128_t.
 *
 * Aprenderás:
 * - AND, OR, XOR, NOT bitwise
 * - Desplazamientos izquierda y derecha
 * - Manipulación de bits individuales
 * - Máscaras de bits
 * - Aplicaciones prácticas
 */

#include <int128_simple.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iomanip>
#include <iostream>

using namespace nstd;

// Helper para imprimir en binario (últimos 16 bits)
void print_binary_low16(const uint128_t& n, const std::string& label)
{
    uint64_t low_word = static_cast<uint64_t>(n);
    std::cout << label << ": ";
    for (int i = 15; i >= 0; --i) {
        std::cout << ((low_word >> i) & 1);
        if (i % 4 == 0 && i != 0)
            std::cout << "_";
    }
    std::cout << " (bin) = " << n << " (dec)\n";
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 02: Operaciones Bitwise\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. AND BITWISE (&)
    // ============================================================
    std::cout << "--- 1. AND bitwise (&) ---\n";
    std::cout << "Resultado: 1 solo si ambos bits son 1\n\n";

    uint128_t a = 0b1111'0000; // 240
    uint128_t b = 0b1010'1010; // 170

    print_binary_low16(a, "a        ");
    print_binary_low16(b, "b        ");

    auto and_result = a & b; // 0b1010'0000 = 160
    print_binary_low16(and_result, "a & b    ");
    std::cout << "\n";

    // ============================================================
    // 2. OR BITWISE (|)
    // ============================================================
    std::cout << "--- 2. OR bitwise (|) ---\n";
    std::cout << "Resultado: 1 si al menos un bit es 1\n\n";

    auto or_result = a | b; // 0b1111'1010 = 250
    print_binary_low16(or_result, "a | b    ");
    std::cout << "\n";

    // ============================================================
    // 3. XOR BITWISE (^)
    // ============================================================
    std::cout << "--- 3. XOR bitwise (^) ---\n";
    std::cout << "Resultado: 1 si los bits son diferentes\n\n";

    auto xor_result = a ^ b; // 0b0101'1010 = 90
    print_binary_low16(xor_result, "a ^ b    ");
    std::cout << "\n";

    // ============================================================
    // 4. NOT BITWISE (~)
    // ============================================================
    std::cout << "--- 4. NOT bitwise (~) ---\n";
    std::cout << "Invierte todos los bits (0→1, 1→0)\n\n";

    uint128_t small = 0b0000'1111; // 15
    auto not_result = ~small;

    print_binary_low16(small, "small    ");
    print_binary_low16(not_result, "~small   ");
    std::cout << "Nota: ~small invierte TODOS los 128 bits!\n\n";

    // ============================================================
    // 5. DESPLAZAMIENTO IZQUIERDA (<<)
    // ============================================================
    std::cout << "--- 5. Desplazamiento izquierda (<<) ---\n";
    std::cout << "Multiplica por potencias de 2\n\n";

    uint128_t n{5}; // 0b101

    std::cout << "n       = " << n << "\n";
    std::cout << "n << 1  = " << (n << 1) << " (× 2)\n";
    std::cout << "n << 2  = " << (n << 2) << " (× 4)\n";
    std::cout << "n << 3  = " << (n << 3) << " (× 8)\n";
    std::cout << "n << 10 = " << (n << 10) << " (× 1024)\n\n";

    // ============================================================
    // 6. DESPLAZAMIENTO DERECHA (>>)
    // ============================================================
    std::cout << "--- 6. Desplazamiento derecha (>>) ---\n";
    std::cout << "Divide por potencias de 2 (trunca)\n\n";

    uint128_t m{1000};

    std::cout << "m       = " << m << "\n";
    std::cout << "m >> 1  = " << (m >> 1) << " (÷ 2)\n";
    std::cout << "m >> 2  = " << (m >> 2) << " (÷ 4)\n";
    std::cout << "m >> 3  = " << (m >> 3) << " (÷ 8)\n";
    std::cout << "m >> 10 = " << (m >> 10) << " (÷ 1024)\n\n";

    // ============================================================
    // 7. MÁSCARAS DE BITS
    // ============================================================
    std::cout << "--- 7. Máscaras de bits ---\n";

    // Extraer los 8 bits inferiores
    uint128_t value = 0xABCD; // 43981
    uint128_t mask_low8 = 0xFF;
    auto low_byte = value & mask_low8;

    std::cout << std::hex << std::uppercase;
    std::cout << "value     = 0x" << value << "\n";
    std::cout << "mask_low8 = 0x" << mask_low8 << "\n";
    std::cout << "low_byte  = 0x" << low_byte << "\n";
    std::cout << std::dec << std::nouppercase << "\n";

    // ============================================================
    // 8. MANIPULACIÓN DE BITS INDIVIDUALES
    // ============================================================
    std::cout << "--- 8. Manipulación de bits individuales ---\n";

    uint128_t flags{0};

    // Activar bit 5
    flags |= (uint128_t(1) << 5);
    std::cout << "Después de activar bit 5:   " << flags << "\n";

    // Activar bit 10
    flags |= (uint128_t(1) << 10);
    std::cout << "Después de activar bit 10:  " << flags << "\n";

    // Verificar si bit 5 está activo
    bool bit5_set = (flags & (uint128_t(1) << 5)) != 0;
    std::cout << "¿Bit 5 activo? " << (bit5_set ? "Sí" : "No") << "\n";

    // Desactivar bit 5
    flags &= ~(uint128_t(1) << 5);
    std::cout << "Después de desactivar bit 5: " << flags << "\n";

    // Toggle (invertir) bit 10
    flags ^= (uint128_t(1) << 10);
    std::cout << "Después de toggle bit 10:    " << flags << "\n\n";

    // ============================================================
    // 9. APLICACIÓN: FLAGS/PERMISOS
    // ============================================================
    std::cout << "--- 9. Aplicación: Sistema de flags ---\n";

    const uint128_t FLAG_READ = 1 << 0;    // 0b001
    const uint128_t FLAG_WRITE = 1 << 1;   // 0b010
    const uint128_t FLAG_EXECUTE = 1 << 2; // 0b100

    uint128_t permissions{0};

    // Otorgar permisos de lectura y escritura
    permissions |= FLAG_READ;
    permissions |= FLAG_WRITE;

    std::cout << "Permisos: ";
    if (permissions & FLAG_READ)
        std::cout << "READ ";
    if (permissions & FLAG_WRITE)
        std::cout << "WRITE ";
    if (permissions & FLAG_EXECUTE)
        std::cout << "EXECUTE ";
    std::cout << "\n";

    // Revocar escritura
    permissions &= ~FLAG_WRITE;

    std::cout << "Después de revocar WRITE: ";
    if (permissions & FLAG_READ)
        std::cout << "READ ";
    if (permissions & FLAG_WRITE)
        std::cout << "WRITE ";
    if (permissions & FLAG_EXECUTE)
        std::cout << "EXECUTE ";
    std::cout << "\n\n";

    // ============================================================
    // 10. APLICACIÓN: SWAP SIN VARIABLE TEMPORAL
    // ============================================================
    std::cout << "--- 10. Aplicación: Swap usando XOR ---\n";

    uint128_t x{123};
    uint128_t y{456};

    std::cout << "Antes:   x = " << x << ", y = " << y << "\n";

    // Swap sin variable temporal usando XOR
    x ^= y;
    y ^= x;
    x ^= y;

    std::cout << "Después: x = " << x << ", y = " << y << "\n\n";

    // ============================================================
    // 11. POTENCIAS DE 2
    // ============================================================
    std::cout << "--- 11. Potencias de 2 con desplazamientos ---\n";

    std::cout << "2^0  = " << (uint128_t(1) << 0) << "\n";
    std::cout << "2^10 = " << (uint128_t(1) << 10) << "\n";
    std::cout << "2^20 = " << (uint128_t(1) << 20) << "\n";
    std::cout << "2^64 = " << (uint128_t(1) << 64) << "\n";
    std::cout << "2^100 = " << (uint128_t(1) << 100) << "\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 02 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 03_comparisons.cpp\n";

    return 0;
}

