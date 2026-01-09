/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 */

/**
 * @file byte_operations.hpp
 * @brief Operaciones sobre bytes (bswap, rotl, rotr)
 *
 * Proporciona implementaciones optimizadas de operaciones sobre bytes
 * para todos los compiladores y arquitecturas soportados.
 *
 * @author Julián Calderón Almendros <julian.calderon.almendros@gmail.com>
 * @version 1.0.0
 * @date 2026-01-05
 * @copyright Boost Software License 1.0
 */

#ifndef INTRINSICS_BYTE_OPERATIONS_HPP
#define INTRINSICS_BYTE_OPERATIONS_HPP

#include "compiler_detection.hpp"
#include "fallback_portable.hpp"
#include <cstdint>

// Incluir headers de intrínsecos según ABI
#if INTRINSICS_USES_MSVC_ABI
#include <intrin.h>
#endif

namespace intrinsics
{

// ============================================================================
// BSWAP - Byte Swap (invertir orden de bytes)
// ============================================================================

/**
 * @brief Invierte el orden de los bytes en un uint64_t
 *
 * @param x Valor cuyos bytes se van a intercambiar
 * @return x con bytes en orden inverso
 *
 * Útil para conversiones big-endian ↔ little-endian.
 */
inline constexpr uint64_t bswap64(uint64_t x) noexcept
{
#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows: usar intrínsecos de MSVC
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return fallback::bswap64_portable(x);
    } else {
        return _byteswap_uint64(x);
    }
#elif INTRINSICS_COMPILER_CLANG || INTRINSICS_COMPILER_GCC
    // GCC/Clang: __builtin_bswap64
#if INTRINSICS_ARCH_ARM64 || INTRINSICS_ARCH_ARM32
    // ARM: REV instruction para byte reverse
    return __builtin_bswap64(x);
#elif INTRINSICS_ARCH_RISCV64 || INTRINSICS_ARCH_RISCV32
    // RISC-V: extensión B (Zbb) tiene REV8 para byte reverse
    return __builtin_bswap64(x);
#else
    return __builtin_bswap64(x);
#endif
#else
    // Fallback: C++ puro portable
    return fallback::bswap64_portable(x);
#endif
}

/**
 * @brief Bswap para uint32_t
 */
inline constexpr uint32_t bswap32(uint32_t x) noexcept
{
#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) |
               ((x & 0x000000FF) << 24);
    } else {
        return _byteswap_ulong(x);
    }
#elif INTRINSICS_USES_GNU_ABI
    return __builtin_bswap32(x);
#else
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) |
           ((x & 0x000000FF) << 24);
#endif
}

/**
 * @brief Bswap para uint16_t
 */
inline constexpr uint16_t bswap16(uint16_t x) noexcept
{
#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return static_cast<uint16_t>((x >> 8) | (x << 8));
    } else {
        return _byteswap_ushort(x);
    }
#elif INTRINSICS_USES_GNU_ABI
    return __builtin_bswap16(x);
#else
    return static_cast<uint16_t>((x >> 8) | (x << 8));
#endif
}

// ============================================================================
// ROTL/ROTR - Rotaciones de bits
// ============================================================================

/**
 * @brief Rotación a la izquierda de 64 bits
 *
 * @param x Valor a rotar
 * @param s Número de posiciones (0-63)
 * @return x rotado s posiciones a la izquierda
 */
inline constexpr uint64_t rotl64(uint64_t x, int s) noexcept
{
    s &= 63; // s %= 64
    if (s == 0)
        return x;

#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows: usar _rotl64
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return (x << s) | (x >> (64 - s));
    } else {
        return _rotl64(x, s);
    }
#elif INTRINSICS_USES_GNU_ABI
    // GCC/Clang/Intel(Linux): el compilador optimiza esto a ROL instruction
    return (x << s) | (x >> (64 - s));
#else
    return fallback::rotl64_portable(x, s);
#endif
}

/**
 * @brief Rotación a la derecha de 64 bits
 *
 * @param x Valor a rotar
 * @param s Número de posiciones (0-63)
 * @return x rotado s posiciones a la derecha
 */
inline constexpr uint64_t rotr64(uint64_t x, int s) noexcept
{
    s &= 63; // s %= 64
    if (s == 0)
        return x;

#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows: usar _rotr64
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return (x >> s) | (x << (64 - s));
    } else {
        return _rotr64(x, s);
    }
#elif INTRINSICS_USES_GNU_ABI
    // GCC/Clang/Intel(Linux): el compilador optimiza esto a ROR instruction
    return (x >> s) | (x << (64 - s));
#else
    return fallback::rotr64_portable(x, s);
#endif
}

/**
 * @brief Rotación a la izquierda de 32 bits
 */
inline constexpr uint32_t rotl32(uint32_t x, int s) noexcept
{
    s &= 31;
    if (s == 0)
        return x;

#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return (x << s) | (x >> (32 - s));
    } else {
        return _rotl(x, s);
    }
#else
    return (x << s) | (x >> (32 - s));
#endif
}

/**
 * @brief Rotación a la derecha de 32 bits
 */
inline constexpr uint32_t rotr32(uint32_t x, int s) noexcept
{
    s &= 31;
    if (s == 0)
        return x;

#if INTRINSICS_USES_MSVC_ABI
    // MSVC y Intel ICX en Windows
    if (INTRINSICS_IS_CONSTANT_EVALUATED()) {
        return (x >> s) | (x << (32 - s));
    } else {
        return _rotr(x, s);
    }
#else
    return (x >> s) | (x << (32 - s));
#endif
}

} // namespace intrinsics

// ============================================================================
// NOTAS DE ARQUITECTURA
// ============================================================================

/**
 * @page byte_ops_arch Notas de Arquitectura para Operaciones de Bytes
 *
 * ## x86-64 (Intel/AMD)
 * - BSWAP: Instruction nativa (486+)
 * - ROL/ROR: Instructions nativas (8086+)
 *
 * ## ARM64 (AArch64)
 * - REV: Byte reverse instruction
 * - ROR: Rotate right instruction
 * - ROL: Simulado como ROR con (32/64 - shift)
 *
 * ## ARM32 (AArch32)
 * - REV: Byte reverse instruction (ARMv6+)
 * - ROR: Rotate right instruction (ARMv4+)
 *
 * ## RISC-V
 * - Extensión B (Zbb): REV8 para byte reverse
 * - Extensión B (Zbb): ROL/ROR instructions
 * - Si no disponible: usar fallback
 *
 * ## PowerPC
 * - ROTLDI/ROTRDI: Rotate left/right doubleword immediate
 * - Byte reverse: Combinar shifts
 *
 * ## Intel Compiler (icpx)
 * - Soporta todos los intrínsecos de MSVC y GCC
 * - Optimizaciones vectoriales para operaciones en bloque
 */

#endif // INTRINSICS_BYTE_OPERATIONS_HPP
