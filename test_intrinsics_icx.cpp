/**
 * @file test_intrinsics_icx.cpp
 * @brief Test de compilación de intrínsecos con Intel ICX
 */

#include "include_new/intrinsics/arithmetic_operations.hpp"
#include "include_new/intrinsics/bit_operations.hpp"
#include "include_new/intrinsics/byte_operations.hpp"
#include "include_new/intrinsics/compiler_detection.hpp"

#include <cstdint>
#include <iostream>

int main()
{
    std::cout << "=== Test Intrinsics con Intel ICX ===" << std::endl;

    // Mostrar detección de compilador
    std::cout << "\n--- Deteccion de compilador ---" << std::endl;
    std::cout << "INTRINSICS_COMPILER_INTEL: " << INTRINSICS_COMPILER_INTEL << std::endl;
    std::cout << "INTRINSICS_COMPILER_MSVC:  " << INTRINSICS_COMPILER_MSVC << std::endl;
    std::cout << "INTRINSICS_COMPILER_GCC:   " << INTRINSICS_COMPILER_GCC << std::endl;
    std::cout << "INTRINSICS_COMPILER_CLANG: " << INTRINSICS_COMPILER_CLANG << std::endl;

    // Mostrar detección de SO
    std::cout << "\n--- Deteccion de SO ---" << std::endl;
    std::cout << "INTRINSICS_OS_WINDOWS: " << INTRINSICS_OS_WINDOWS << std::endl;
    std::cout << "INTRINSICS_OS_LINUX:   " << INTRINSICS_OS_LINUX << std::endl;
    std::cout << "INTRINSICS_OS_MACOS:   " << INTRINSICS_OS_MACOS << std::endl;

    // Mostrar detección de ABI
    std::cout << "\n--- Deteccion de ABI ---" << std::endl;
    std::cout << "INTRINSICS_USES_MSVC_ABI: " << INTRINSICS_USES_MSVC_ABI << std::endl;
    std::cout << "INTRINSICS_USES_GNU_ABI:  " << INTRINSICS_USES_GNU_ABI << std::endl;

    // Mostrar arquitectura
    std::cout << "\n--- Arquitectura ---" << std::endl;
    std::cout << "INTRINSICS_ARCH_X86_64: " << INTRINSICS_ARCH_X86_64 << std::endl;

    // Test de operaciones aritméticas
    std::cout << "\n--- Test operaciones aritmeticas ---" << std::endl;

    uint64_t a = 0xFFFFFFFFFFFFFFFFULL;
    uint64_t b = 1;
    uint64_t result = 0;

    // Test add_u64 con overflow
    unsigned char carry = intrinsics::add_u64(a, b, &result);
    std::cout << "add_u64(MAX, 1) = " << result << ", carry = " << (int)carry << std::endl;
    if (carry == 1 && result == 0) {
        std::cout << "  [OK] Overflow detectado correctamente" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en deteccion de overflow" << std::endl;
        return 1;
    }

    // Test addcarry_u64
    carry = intrinsics::addcarry_u64(1, a, 0, &result);
    std::cout << "addcarry_u64(1, MAX, 0) = " << result << ", carry = " << (int)carry << std::endl;
    if (carry == 1 && result == 0) {
        std::cout << "  [OK] Carry propagado correctamente" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en propagacion de carry" << std::endl;
        return 1;
    }

    // Test sub_u64
    unsigned char borrow = intrinsics::sub_u64(0, 1, &result);
    std::cout << "sub_u64(0, 1) = " << result << ", borrow = " << (int)borrow << std::endl;
    if (borrow == 1 && result == 0xFFFFFFFFFFFFFFFFULL) {
        std::cout << "  [OK] Underflow detectado correctamente" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en deteccion de underflow" << std::endl;
        return 1;
    }

    // Test umul128
    std::cout << "\n--- Test multiplicacion 128 bits ---" << std::endl;
    uint64_t hi = 0;
    uint64_t lo = intrinsics::umul128(0x100000000ULL, 0x100000000ULL, &hi);
    std::cout << "umul128(2^32, 2^32) = hi:" << hi << ", lo:" << lo << std::endl;
    if (hi == 1 && lo == 0) {
        std::cout << "  [OK] Multiplicacion correcta (2^64)" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en multiplicacion" << std::endl;
        return 1;
    }

    // Test operaciones de bits
    std::cout << "\n--- Test operaciones de bits ---" << std::endl;

    int pc = intrinsics::popcount64(0xFFFFFFFFFFFFFFFFULL);
    std::cout << "popcount64(MAX) = " << pc << std::endl;
    if (pc == 64) {
        std::cout << "  [OK] Popcount correcto" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en popcount" << std::endl;
        return 1;
    }

    int clz = intrinsics::clz64(1ULL << 63);
    std::cout << "clz64(1 << 63) = " << clz << std::endl;
    if (clz == 0) {
        std::cout << "  [OK] CLZ correcto" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en CLZ" << std::endl;
        return 1;
    }

    int ctz = intrinsics::ctz64(1ULL << 63);
    std::cout << "ctz64(1 << 63) = " << ctz << std::endl;
    if (ctz == 63) {
        std::cout << "  [OK] CTZ correcto" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en CTZ" << std::endl;
        return 1;
    }

    // Test operaciones de bytes
    std::cout << "\n--- Test operaciones de bytes ---" << std::endl;

    uint64_t swapped = intrinsics::bswap64(0x0102030405060708ULL);
    std::cout << "bswap64(0x0102030405060708) = 0x" << std::hex << swapped << std::dec << std::endl;
    if (swapped == 0x0807060504030201ULL) {
        std::cout << "  [OK] Bswap correcto" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en bswap" << std::endl;
        return 1;
    }

    uint64_t rotated = intrinsics::rotl64(1ULL, 63);
    std::cout << "rotl64(1, 63) = 0x" << std::hex << rotated << std::dec << std::endl;
    if (rotated == (1ULL << 63)) {
        std::cout << "  [OK] Rotl correcto" << std::endl;
    } else {
        std::cout << "  [FAIL] Error en rotl" << std::endl;
        return 1;
    }

    std::cout << "\n=== TODOS LOS TESTS PASARON ===" << std::endl;
    return 0;
}
