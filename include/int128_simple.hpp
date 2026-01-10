/**
 * @file int128_simple.hpp
 * @brief Header simplificado para demos - Evita conflictos de redefinición
 *
 * Este header incluye solo lo esencial para usar int128_t/uint128_t:
 * - Template principal con todas las operaciones
 * - Traits y limits
 * - I/O streams
 *
 * NO incluye: cmath, numeric, algorithm (tienen redefiniciones conflictivas)
 *
 * @author Julián Calderón Almendros
 * @version 1.66
 * @date 2026-01-11
 */

#ifndef INT128_SIMPLE_HPP
#define INT128_SIMPLE_HPP

// Core template - INCLUYE TODAS LAS OPERACIONES BÁSICAS
#include "int128_base_tt.hpp"

// Type traits y limits
#include "int128_base_limits.hpp"
#include "int128_base_traits.hpp"
#include "int128_base_traits_specializations.hpp"

// I/O
#include "int128_base_iostreams.hpp"

// Nota: Para funciones matemáticas avanzadas (gcd, lcm, sqrt, etc.),
// incluir SOLO UNO de:
//   - int128_base_cmath.hpp (funciones matemáticas estilo cmath)
//   - int128_base_numeric.hpp (funciones numéricas estilo numeric)
// NO AMBOS - tienen definiciones conflictivas.

#endif // INT128_SIMPLE_HPP
