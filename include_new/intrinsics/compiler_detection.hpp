/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 */

/**
 * @file compiler_detection.hpp
 * @brief Detección de compilador, arquitectura y capacidades
 *
 * Este header proporciona macros para detectar el compilador en uso,
 * la arquitectura objetivo y las capacidades disponibles.
 *
 * @author Julián Calderón Almendros <julian.calderon.almendros@gmail.com>
 * @version 1.0.0
 * @date 2026-01-05
 * @copyright Boost Software License 1.0
 */

#ifndef INTRINSICS_COMPILER_DETECTION_HPP
#define INTRINSICS_COMPILER_DETECTION_HPP

// ============================================================================
// DETECCIÓN DE COMPILADOR
// ============================================================================

// Detectar compilador antes de arquitectura (orden importa)
#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__ICL) || defined(__ICC)
#define INTRINSICS_COMPILER_INTEL 1
#elif defined(_MSC_VER)
#define INTRINSICS_COMPILER_MSVC 1
#elif defined(__clang__)
#define INTRINSICS_COMPILER_CLANG 1
#elif defined(__GNUC__)
#define INTRINSICS_COMPILER_GCC 1
#else
#define INTRINSICS_COMPILER_UNKNOWN 1
#endif

// Definir 0 para los no detectados
#ifndef INTRINSICS_COMPILER_INTEL
#define INTRINSICS_COMPILER_INTEL 0
#endif
#ifndef INTRINSICS_COMPILER_MSVC
#define INTRINSICS_COMPILER_MSVC 0
#endif
#ifndef INTRINSICS_COMPILER_CLANG
#define INTRINSICS_COMPILER_CLANG 0
#endif
#ifndef INTRINSICS_COMPILER_GCC
#define INTRINSICS_COMPILER_GCC 0
#endif
#ifndef INTRINSICS_COMPILER_UNKNOWN
#define INTRINSICS_COMPILER_UNKNOWN 0
#endif

// ============================================================================
// DETECCIÓN DE SISTEMA OPERATIVO
// ============================================================================

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define INTRINSICS_OS_WINDOWS 1
#elif defined(__linux__) || defined(__linux) || defined(linux)
#define INTRINSICS_OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define INTRINSICS_OS_MACOS 1
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#define INTRINSICS_OS_BSD 1
#elif defined(__unix__) || defined(__unix)
#define INTRINSICS_OS_UNIX 1
#else
#define INTRINSICS_OS_UNKNOWN 1
#endif

// Definir 0 para sistemas operativos no detectados
#ifndef INTRINSICS_OS_WINDOWS
#define INTRINSICS_OS_WINDOWS 0
#endif
#ifndef INTRINSICS_OS_LINUX
#define INTRINSICS_OS_LINUX 0
#endif
#ifndef INTRINSICS_OS_MACOS
#define INTRINSICS_OS_MACOS 0
#endif
#ifndef INTRINSICS_OS_BSD
#define INTRINSICS_OS_BSD 0
#endif
#ifndef INTRINSICS_OS_UNIX
#define INTRINSICS_OS_UNIX 0
#endif
#ifndef INTRINSICS_OS_UNKNOWN
#define INTRINSICS_OS_UNKNOWN 0
#endif

// ============================================================================
// DETECCIÓN DE ABI (Application Binary Interface)
// ============================================================================

/**
 * @brief Detecta si el compilador usa ABI de MSVC
 *
 * Intel ICX en Windows usa el ABI y las bibliotecas de MSVC, por lo que
 * debe usar los intrínsecos de MSVC (_addcarry_u64, etc.) en lugar de
 * los builtins de GCC (__builtin_addcll, etc.).
 *
 * Esta macro es true cuando:
 * - Compilador es MSVC
 * - Compilador es Intel en Windows (usa MSVC ABI)
 */
#if INTRINSICS_COMPILER_MSVC || (INTRINSICS_COMPILER_INTEL && INTRINSICS_OS_WINDOWS)
#define INTRINSICS_USES_MSVC_ABI 1
#else
#define INTRINSICS_USES_MSVC_ABI 0
#endif

/**
 * @brief Detecta si el compilador usa ABI de GCC/Clang
 *
 * GCC, Clang, y Intel ICX en Linux/macOS usan builtins estilo GCC.
 */
#if (INTRINSICS_COMPILER_GCC || INTRINSICS_COMPILER_CLANG ||                                       \
     (INTRINSICS_COMPILER_INTEL && !INTRINSICS_OS_WINDOWS))
#define INTRINSICS_USES_GNU_ABI 1
#else
#define INTRINSICS_USES_GNU_ABI 0
#endif

// ============================================================================
// DETECCIÓN DE ARQUITECTURA
// ============================================================================

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
#define INTRINSICS_ARCH_X86_64 1
#elif defined(__i386__) || defined(_M_IX86)
#define INTRINSICS_ARCH_X86_32 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define INTRINSICS_ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
#define INTRINSICS_ARCH_ARM32 1
#elif defined(__riscv)
#if __riscv_xlen == 64
#define INTRINSICS_ARCH_RISCV64 1
#else
#define INTRINSICS_ARCH_RISCV32 1
#endif
#elif defined(__powerpc64__) || defined(__ppc64__)
#define INTRINSICS_ARCH_PPC64 1
#else
#define INTRINSICS_ARCH_UNKNOWN 1
#endif

// Definir 0 para arquitecturas no detectadas
#ifndef INTRINSICS_ARCH_X86_64
#define INTRINSICS_ARCH_X86_64 0
#endif
#ifndef INTRINSICS_ARCH_X86_32
#define INTRINSICS_ARCH_X86_32 0
#endif
#ifndef INTRINSICS_ARCH_ARM64
#define INTRINSICS_ARCH_ARM64 0
#endif
#ifndef INTRINSICS_ARCH_ARM32
#define INTRINSICS_ARCH_ARM32 0
#endif
#ifndef INTRINSICS_ARCH_RISCV64
#define INTRINSICS_ARCH_RISCV64 0
#endif
#ifndef INTRINSICS_ARCH_RISCV32
#define INTRINSICS_ARCH_RISCV32 0
#endif
#ifndef INTRINSICS_ARCH_PPC64
#define INTRINSICS_ARCH_PPC64 0
#endif
#ifndef INTRINSICS_ARCH_UNKNOWN
#define INTRINSICS_ARCH_UNKNOWN 0
#endif

// ============================================================================
// DETECCIÓN DE CAPACIDADES (BUILTINS)
// ============================================================================

// GCC/Clang/Intel (en Linux/macOS) tienen __has_builtin
#ifdef __has_builtin
#define INTRINSICS_HAS_BUILTIN(x) __has_builtin(x)
#else
#define INTRINSICS_HAS_BUILTIN(x) 0
#endif

// Detectar builtins específicos - SOLO para compiladores con GNU ABI
// Intel ICX en Windows NO tiene estos builtins
#if INTRINSICS_HAS_BUILTIN(__builtin_popcountll)
#define INTRINSICS_HAS_BUILTIN_POPCOUNT 1
#elif INTRINSICS_USES_GNU_ABI
#define INTRINSICS_HAS_BUILTIN_POPCOUNT 1
#else
#define INTRINSICS_HAS_BUILTIN_POPCOUNT 0
#endif

#if INTRINSICS_HAS_BUILTIN(__builtin_clzll)
#define INTRINSICS_HAS_BUILTIN_CLZ 1
#elif INTRINSICS_USES_GNU_ABI
#define INTRINSICS_HAS_BUILTIN_CLZ 1
#else
#define INTRINSICS_HAS_BUILTIN_CLZ 0
#endif

#if INTRINSICS_HAS_BUILTIN(__builtin_ctzll)
#define INTRINSICS_HAS_BUILTIN_CTZ 1
#elif INTRINSICS_USES_GNU_ABI
#define INTRINSICS_HAS_BUILTIN_CTZ 1
#else
#define INTRINSICS_HAS_BUILTIN_CTZ 0
#endif

#if INTRINSICS_HAS_BUILTIN(__builtin_bswap64)
#define INTRINSICS_HAS_BUILTIN_BSWAP 1
#elif INTRINSICS_USES_GNU_ABI
#define INTRINSICS_HAS_BUILTIN_BSWAP 1
#else
#define INTRINSICS_HAS_BUILTIN_BSWAP 0
#endif

// Detectar __builtin_addcll / __builtin_subcll (GCC/Clang x86-64, NO Intel Windows)
#if INTRINSICS_HAS_BUILTIN(__builtin_addcll) && INTRINSICS_ARCH_X86_64 && !INTRINSICS_USES_MSVC_ABI
#define INTRINSICS_HAS_BUILTIN_ADDC 1
#else
#define INTRINSICS_HAS_BUILTIN_ADDC 0
#endif

// ============================================================================
// DETECCIÓN DE HEADERS INTRÍNSECOS
// ============================================================================

// Usar <intrin.h> para MSVC y también para Intel ICX en Windows (MSVC ABI)
#if INTRINSICS_USES_MSVC_ABI
#define INTRINSICS_HAS_INTRIN_H 1
#define INTRINSICS_HAS_X86INTRIN_H 0
#define INTRINSICS_HAS_ARM_NEON_H 0
#elif INTRINSICS_ARCH_X86_64 || INTRINSICS_ARCH_X86_32
#define INTRINSICS_HAS_INTRIN_H 0
#define INTRINSICS_HAS_X86INTRIN_H 1
#define INTRINSICS_HAS_ARM_NEON_H 0
#elif INTRINSICS_ARCH_ARM64 || INTRINSICS_ARCH_ARM32
#define INTRINSICS_HAS_INTRIN_H 0
#define INTRINSICS_HAS_X86INTRIN_H 0
#define INTRINSICS_HAS_ARM_NEON_H 1
#else
#define INTRINSICS_HAS_INTRIN_H 0
#define INTRINSICS_HAS_X86INTRIN_H 0
#define INTRINSICS_HAS_ARM_NEON_H 0
#endif

// ============================================================================
// INCLUIR HEADERS NECESARIOS
// ============================================================================

#if INTRINSICS_HAS_INTRIN_H
#include <intrin.h>
#endif

#if INTRINSICS_HAS_X86INTRIN_H
// GCC/Clang en x86-64: <x86intrin.h> incluye todo lo necesario
// Pero los builtins suelen funcionar sin incluir nada
#endif

#if INTRINSICS_HAS_ARM_NEON_H
#include <arm_neon.h>
#endif

// ============================================================================
// DETECCIÓN DE C++20 std::is_constant_evaluated
// ============================================================================

#include <type_traits>

#if defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811L
#define INTRINSICS_HAS_IS_CONSTANT_EVALUATED 1
#else
#define INTRINSICS_HAS_IS_CONSTANT_EVALUATED 0
#endif

// ============================================================================
// MACRO HELPER PARA CONSTEXPR
// ============================================================================

/**
 * @def INTRINSICS_IS_CONSTANT_EVALUATED()
 * @brief Verifica si la evaluación es en tiempo de compilación
 */
#if INTRINSICS_HAS_IS_CONSTANT_EVALUATED
#define INTRINSICS_IS_CONSTANT_EVALUATED() std::is_constant_evaluated()
#else
// Fallback: siempre retorna false (seguro pero menos optimizado)
#define INTRINSICS_IS_CONSTANT_EVALUATED() false
#endif

// ============================================================================
// INFORMACIÓN DE RESUMEN
// ============================================================================

/**
 * @page compiler_detection Detección de Compilador y Arquitectura
 *
 * ## Compiladores Detectados
 * - INTRINSICS_COMPILER_INTEL: Intel oneAPI (icx, icpx, icc)
 * - INTRINSICS_COMPILER_MSVC: Microsoft Visual C++
 * - INTRINSICS_COMPILER_CLANG: Clang/LLVM
 * - INTRINSICS_COMPILER_GCC: GNU Compiler Collection
 * - INTRINSICS_COMPILER_UNKNOWN: Compilador no reconocido
 *
 * ## Arquitecturas Soportadas
 * - INTRINSICS_ARCH_X86_64: x86-64 (AMD64, x64)
 * - INTRINSICS_ARCH_X86_32: x86 32-bit (i386, i686)
 * - INTRINSICS_ARCH_ARM64: ARM 64-bit (AArch64)
 * - INTRINSICS_ARCH_ARM32: ARM 32-bit
 * - INTRINSICS_ARCH_RISCV64: RISC-V 64-bit
 * - INTRINSICS_ARCH_RISCV32: RISC-V 32-bit
 * - INTRINSICS_ARCH_PPC64: PowerPC 64-bit
 * - INTRINSICS_ARCH_UNKNOWN: Arquitectura no reconocida
 *
 * ## Capacidades Detectadas
 * - INTRINSICS_HAS_BUILTIN_POPCOUNT: __builtin_popcountll
 * - INTRINSICS_HAS_BUILTIN_CLZ: __builtin_clzll
 * - INTRINSICS_HAS_BUILTIN_CTZ: __builtin_ctzll
 * - INTRINSICS_HAS_BUILTIN_BSWAP: __builtin_bswap64
 * - INTRINSICS_HAS_BUILTIN_ADDC: __builtin_addcll
 *
 * ## Uso
 * @code
 * #if INTRINSICS_COMPILER_MSVC
 *     // Código específico de MSVC
 * #elif INTRINSICS_COMPILER_INTEL
 *     // Código específico de Intel
 * #else
 *     // Fallback genérico
 * #endif
 * @endcode
 */

#endif // INTRINSICS_COMPILER_DETECTION_HPP
