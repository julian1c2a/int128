# CHANGELOG - int128 Library

Todos los cambios notables del proyecto se documentan en este archivo.

El formato está basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/),
y este proyecto adhiere a [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## 📋 Normativa de Documentación

### ⏰ Actualización Horaria (OBLIGATORIO)

**REGLA:** Este archivo DEBE actualizarse **cada hora** durante sesiones de desarrollo activo.

**Formato de entrada horaria:**

```markdown
### [HH:MM] YYYY-MM-DD - Resumen breve
- Cambio 1
- Cambio 2
```

**Beneficios:**

- ✅ Preserva contexto entre sesiones
- ✅ Facilita revisión de cambios
- ✅ Evita pérdida de información

### 🔗 Documentos Interconectados

Este proyecto mantiene documentación sincronizada. Al actualizar un documento, verifica si los relacionados necesitan cambios:

| Documento | Propósito | Actualizar cuando... |
|-----------|-----------|----------------------|
| **[CHANGELOG.md](CHANGELOG.md)** | Historial de cambios | Cualquier cambio significativo |
| **[TODO.md](TODO.md)** | Roadmap y tareas | Cambios de estado de fases/tareas |
| **[README.md](README.md)** | Documentación usuario | Nuevas features o API changes |
| **[PROMPT.md](PROMPT.md)** | Convenciones de código | Nuevas reglas o patrones |
| **[API_INT128_BASE_T.md](API_INT128_BASE_T.md)** | Referencia API | Cambios en firmas de funciones |
| **[PORTABILITY_GUIDE.md](PORTABILITY_GUIDE.md)** | Guía portabilidad | Nuevos compiladores/plataformas |
| **[DEV_ENV_VARS.md](DEV_ENV_VARS.md)** | Entorno desarrollo | Cambios en configuración |

### 📊 Matriz de Dependencias

```
CHANGELOG.md ──────┬──> TODO.md (estado de fases)
                   ├──> README.md (features nuevas)
                   └──> API_*.md (cambios API)

TODO.md ───────────┬──> CHANGELOG.md (completar tareas)
                   └──> README.md (roadmap público)

PROMPT.md ─────────┬──> Todos (convenciones aplican a todo)
                   └──> copilot-instructions.md (reglas AI)

README.md ─────────┬──> CHANGELOG.md (historial)
                   └──> TODO.md (próximos pasos)
```

---

## [Unreleased] - Fase 1.66 Validación Multi-Compilador

### 🔄 Sesión Actual

#### [12:00] 2026-01-11 - Limpieza TYPE + Scripts Directos + Makefile Actualizado

- ✅ **Scripts simplificados (eliminado TYPE de todos):**
  - `build_tests_generic.bash` - hardcoded `int128_` en lugar de `${TYPE}_`
  - `build_benchs_generic.bash` - hardcoded `int128_` en lugar de `${TYPE}_`
  - `check_generic.bash` - removida compatibilidad legacy TYPE
  - `run_generic.bash` - removida compatibilidad legacy TYPE
  - `build_generic.bash` - simplificado solo para demos (~220 líneas)
- ✅ **Nuevos scripts directos creados:**
  - `check_direct.bash` - ejecuta test directamente con `exec "$EXE_PATH"`
  - `run_direct.bash` - ejecuta benchmark directamente con `exec "$EXE_PATH"`
- ✅ **Makefile actualizado:**
  - Eliminado TYPE de feature_shortcuts (`build-all-*`, `check-all-*`, `run-all-*`)
  - Eliminado TYPE de target `sanitize`
  - Actualizado help con nueva sintaxis sin TYPE
  - Título actualizado: "INT128 - Build System (Unified Template)"
- ✅ **Benchmarks probados con nuevos scripts:**
  - `tt`, `format`, `iostreams`, `bits`, `algorithm`, `cmath`, `numeric`, `concepts`
  - Todos compilan y ejecutan correctamente
- ⚠️ **`comparison_boost`** requiere GMP instalado (dependencia externa)
- **Sintaxis nueva:**

  ```bash
  bash scripts/build_benchs_generic.bash <feature> [compiler] [mode]
  bash scripts/run_direct.bash <feature> [compiler] [mode]
  bash scripts/build_tests_generic.bash <feature> [compiler] [mode]
  bash scripts/check_direct.bash <feature> [compiler] [mode]
  ```

---

#### [10:00] 2026-01-11 - Unificación de Benchmarks + Bugs Documentados

- ✅ **Benchmarks unificados creados (7 de 14):**
  - `int128_tt_extracted_benchs.cpp` - Con comparativa Boost
  - `int128_bits_extracted_benchs.cpp` - popcount, clz, ctz, rotl, rotr
  - `int128_algorithm_extracted_benchs.cpp` - iota, accumulate, transform, sort
  - `int128_cmath_extracted_benchs.cpp` - gcd, lcm, sqrt, pow, bezout
  - `int128_numeric_extracted_benchs.cpp` - midpoint, iota, inner_product
  - `int128_limits_extracted_benchs.cpp` - numeric_limits
  - `int128_traits_extracted_benchs.cpp` - type_traits
- 🐛 **Bugs descubiertos en especializaciones std::**:
  - `std::numeric_limits<uint128_t>::digits` → 0 (debería ser 128)
  - `std::is_integral_v<uint128_t>` → false (debería ser true)
  - `std::is_arithmetic_v<uint128_t>` → false
  - Variantes `nstd::` funcionan correctamente
- 📋 **TODO.md actualizado** con:
  - Tabla de estado de unificaciones de benchmarks
  - Sección de bugs conocidos en especializaciones std::
  - Nota sobre comparison_boost e interop pendientes de añadir int128_t

---

### 📜 Sesiones Anteriores

#### [17:30] 2026-01-10 - VALIDACIÓN WINDOWS COMPLETADA: 13/13 Features

- ✅ **Windows (MSYS2) - 26/26 configuraciones GCC+Clang:**
  - Scripts `build_generic.bash`, `check_generic.bash`, `run_generic.bash` actualizados
  - Removido parámetro TYPE redundante (igual que WSL)
  - Fix: OSTYPE check para `"cygwin"` (no `"msys"`)
  - Fix: `MSYS2_ARG_CONV_EXCL="*"` para MSVC
  - Fix: Flags separados GCC vs Clang para `-Ofast`
- ✅ **Resultados de tests (GCC + Clang release):**

| Feature | GCC | Clang |
|---------|-----|-------|
| tt | ✅ PASS | ✅ PASS |
| traits | ✅ PASS | ✅ PASS |
| limits | ✅ PASS | ✅ PASS |
| concepts | ✅ PASS | ✅ PASS |
| bits | ✅ PASS | ✅ PASS |
| numeric | ✅ PASS | ✅ PASS |
| algorithm | ✅ PASS | ✅ PASS |
| cmath | ✅ PASS | ✅ PASS |
| iostreams | ✅ PASS | ✅ PASS |
| format | ✅ PASS | ✅ PASS |
| ranges | ✅ PASS | ✅ PASS |
| safe | ✅ PASS | ✅ PASS |
| thread_safety | ✅ PASS | ✅ PASS |

- 📋 **Total Windows:** 26/26 (GCC 13 + Clang 13)
- 📋 **Total WSL:** 16/16 (8 compiladores × 2 modos)
- 📋 **Total Global:** 42/42 configuraciones validadas

#### [16:00] 2026-01-10 - FASE 1.66 COMPLETADA: Validación Multi-Compilador WSL

- ✅ **Validación Windows (MSYS2):** 24/24 configuraciones
  - GCC 15.2 (UCRT64): debug ✅, release ✅
  - Clang 19.1 (CLANG64): debug ✅, release ✅
  - Intel ICX 2025: debug ✅, release ✅
  - MSVC 2026: debug ✅, release ✅
- ✅ **Validación WSL (Ubuntu):** 16/16 configuraciones
  - GCC 13.3, 14.2, 15.0.1: debug ✅, release ✅
  - Clang 18.1.8, 19.1.7, 20.1.2, 21.1.8: debug ✅, release ✅
  - Intel ICPX 2025.3.1: debug ✅, release ✅
- ✅ **Reorganización scripts WSL:**
  - `scripts/wsl/` - Scripts modulares por compilador
  - `scripts/wsl/common.bash` - Funciones compartidas
  - `scripts/wsl_build_and_test.bash` - Script maestro simplificado
- ✅ **Nueva sintaxis simplificada:**
  - Antes: `bash wsl_build_and_test.bash int128 tt all all` (TYPE redundante)
  - Ahora: `bash wsl_build_and_test.bash tt all all` (solo FEATURE)
- ✅ **Scripts Python actualizados:**
  - `scripts/wsl/common.py` - Módulo común Python
  - `scripts/wsl_build_and_test.py` - Script maestro Python
  - `scripts/run_wsl_tests.py` - Puente Windows→WSL actualizado
- ✅ **Bug fix:** `((passed++))` → `passed=$((passed + 1))` (evita exit 1 con set -e)
- 📋 **Total validaciones:** 40/40 (100%)

#### [14:30] 2026-01-10 - FASE 1.6 COMPLETADA: Limpieza total de directorios

- ✅ **Directorios eliminados:**
  - `tests_new/` - 14 archivos template (contenido fusionado en tests/)
  - `experimental/` - 12 archivos (duplicados de tests_new/)
  - `include_legacy/` - 19 headers (backup obsoleto)
- ✅ **Archivos de raíz eliminados:**
  - 62 archivos `.cpp` de debugging/testing temporal
  - 3 archivos `.hpp` temporales (`int128_base.hpp`, `uint128_bits_*.hpp`)
  - 4 archivos temporales (`temp_*.sh`, `output.txt`, `debug_*.bash`)
- ✅ **Tests fusionados (14 features):**
  - `int128_algorithm_extracted_tests.cpp`
  - `int128_base_tt_extracted_tests.cpp`
  - `int128_bits_extracted_tests.cpp`
  - `int128_cmath_extracted_tests.cpp`
  - `int128_concepts_extracted_tests.cpp`
  - `int128_format_extracted_tests.cpp`
  - `int128_iostreams_extracted_tests.cpp`
  - `int128_limits_extracted_tests.cpp`
  - `int128_numeric_extracted_tests.cpp`
  - `int128_ranges_extracted_tests.cpp`
  - `int128_safe_extracted_tests.cpp`
  - `int128_t_extracted_tests.cpp`
  - `int128_thread_safety_extracted_tests.cpp`
  - `int128_traits_extracted_tests.cpp`
- 📋 **Estructura final limpia:** include/, tests/, benchs/, demos/, scripts/

#### [12:10] 2026-01-10 - FASE A COMPLETADA: include_new/ → include/ (#003)

- ✅ **Migración de directorios ejecutada:**
  - `include/` → `include_legacy/` (backup)
  - `include_new/` → `include/` (nuevo canónico)
  - `int128.hpp` copiado a include/ (wrapper de conveniencia)
- ✅ **Comparación de archivos:**
  - 15 headers idénticos entre ambos directorios
  - `int128_base_algorithm.hpp`: include_new/ tiene licencia Boost completa (+34 líneas)
  - `intrinsics/compiler_detection.hpp`: include_new/ tiene detección de OS/ABI (+100 líneas)
  - `intrinsics/arithmetic_operations.hpp`: include_new/ usa `INTRINSICS_USES_MSVC_ABI` (más robusto)
- ✅ **Compilación verificada:** `int128_base_tt_extracted_tests.cpp` compila OK con GCC
- 📋 **Próximo:** Fase B (tests_new/ → tests/)

#### [12:00] 2026-01-10 - Grafo de Documentación y Plan de Integración (#001-#002)

- ✅ **DOCUMENTATION_GRAPH.md**: Creado grafo completo de 114 archivos .md
  - Mapa de dependencias entre documentos
  - Categorización: Primarios, API, Soporte, Análisis
  - Protocolo de actualización con checklist
  - Contador de actividad por sesión
- 📋 **Plan de Integración de Directorios** documentado:
  - `include_new/` → `include/` (Fase A) ✅ **COMPLETADA**
  - `tests_new/` → `tests/` (Fase B)
  - `experimental/` → `tests/experimental/` (Fase C)
- 🔗 **Ver:** [DOCUMENTATION_GRAPH.md](DOCUMENTATION_GRAPH.md) para mapa completo

---

### 📜 Sesiones Anteriores

#### [11:00] 2026-01-08 - Licencias Boost expandidas en todos los headers

- ✅ `type_traits.hpp`: Añadida licencia Boost completa (faltaba)
- ✅ `intrinsics/arithmetic_operations.hpp`: Expandida licencia abreviada → completa
- ✅ `intrinsics/bit_operations.hpp`: Expandida licencia abreviada → completa
- ✅ `intrinsics/compiler_detection.hpp`: Expandida licencia abreviada → completa
- ✅ `intrinsics/fallback_portable.hpp`: Expandida licencia abreviada → completa
- ✅ `intrinsics/byte_operations.hpp`: Expandida licencia abreviada → completa
- 📋 **Total archivos con licencia completa:** 17/17 en `include_new/`

#### [10:30] 2026-01-08 - FEATURES cmath, iostreams y format COMPLETOS

- ✅ **FEATURE cmath**: `int128_base_cmath.hpp` completo (18/18 tests OK)
  - gcd (Stein binary algorithm)
  - lcm (con protección overflow)
  - pow (exponenciación binaria)
  - sqrt (Newton-Raphson)
  - abs, min, max, clamp, sign, midpoint, divmod
  - **bezout_coeffs** (Algoritmo Extendido de Euclides) ✨
- ✅ **FEATURE iostreams**: `int128_base_iostreams.hpp` verificado (25/25 tests OK)
  - operator<< con manipuladores (hex, oct, dec, setw, setfill...)
  - operator>> con auto-detección de base
  - Funciones de conveniencia: format(), hex(), oct(), dec()
- ✅ **FEATURE format**: `int128_base_format.hpp` completo (25/25 tests OK) ✨
  - format() con control total de flags, alineación, fill
  - hex(), oct(), dec(), dec_signed()
  - bin() para formato binario con prefijo
  - with_separators() para separadores de miles
  - format_like_iostream() imita estado de ostream
- 📋 **Total Fase 1.5:** 11/13 headers completados, 419 tests pasando

**Próximos headers:** safe, thread_safety

#### [23:30] 2026-01-07 - FEATURES concepts, bits, numeric, algorithm COMPLETOS

- ✅ **FEATURE concepts**: `int128_base_concepts.hpp` creado (59/59 tests OK)
- ✅ **FEATURE bits**: `int128_base_bits.hpp` creado (77/77 tests OK)
- ✅ **FEATURE numeric**: `int128_base_numeric.hpp` creado (87/87 tests OK)
- ✅ **FEATURE algorithm**: `int128_base_algorithm.hpp` creado (47/47 tests OK)
- ✅ Renombrado `uint128_traits_specializations.hpp` → `int128_base_traits_specializations.hpp`
- ✅ Actualizada documentación (TODO.md, CHANGELOG.md)
- 📋 **Total Fase 1.5:** 8/13 headers completados, 336 tests pasando

**Próximos headers:** cmath, iostreams, format, safe, thread_safety

#### [19:00] 2026-01-07 - Migracion FEATURE limits y nomenclatura

- ✅ Creada normativa actualizacion horaria CHANGELOG
- ✅ Sistema de interconexion documentos *.md implementado
- ✅ Actualizado PROMPT.md con seccion Fase 1.5 y sanitizadores
- ✅ Nueva nomenclatura: headers ahora son `int128_base_[feature].hpp`
- ✅ **FEATURE limits**: `int128_base_limits.hpp` creado y testeado (10/10 OK)
- 📋 Proxima: FEATURE cmath

**Nomenclatura actualizada:**

- `[type_base]` -> `[signedness]` (unsigned/signed)
- Futuro: `[limbs]` (2/4/8 para 128/256/512 bits)
- Headers: `int128_base_[feature].hpp`

#### [17:00] 2026-01-07 - Auditoria y Normativa

- Auditoria completa de `include_new/` vs `include/`
- Creacion de normativa de actualizacion horaria del CHANGELOG
- Sistema de interconexion de documentos *.md
- Estado: Template `int128_base_tt.hpp` tiene core completo, faltan features modulares

#### Estado de Migracion de Features

| Feature | En `include/` | En `include_new/` | Estado |
|---------|---------------|-------------------|--------|
| Core (`_tt.hpp`) | ✅ uint128 + int128 | ✅ Template unificado | ✅ Listo |
| `_limits.hpp` | ✅ | ✅ `int128_base_limits.hpp` | ✅ **COMPLETO** |
| `_traits.hpp` | ✅ | ✅ `int128_base_traits*.hpp` | ✅ **COMPLETO** |
| `_concepts.hpp` | ✅ | ✅ `int128_base_concepts.hpp` | ✅ **COMPLETO** |
| `_bits.hpp` | ✅ | ✅ `int128_base_bits.hpp` | ✅ **COMPLETO** |
| `_numeric.hpp` | ✅ | ✅ `int128_base_numeric.hpp` | ✅ **COMPLETO** |
| `_algorithm.hpp` | ✅ | ✅ `int128_base_algorithm.hpp` | ✅ **COMPLETO** |
| `_cmath.hpp` | ✅ | ✅ `int128_base_cmath.hpp` | ✅ **COMPLETO** |
| `_iostreams.hpp` | ✅ | ✅ `int128_base_iostreams.hpp` | ✅ **COMPLETO** |
| `_format.hpp` | ✅ | ✅ `int128_base_format.hpp` | ✅ **COMPLETO** |
| `_safe.hpp` | ✅ | ❌ | 📋 Pendiente |
| `_thread_safety.hpp` | ✅ | ❌ | 📋 Pendiente |
| `_ranges.hpp` | ✅ | ❌ | 📋 Pendiente (bajo prioridad) |

---

## [1.0.0] - 2026-01-05

### 🎉 Versión Release - Biblioteca Production-Ready

**Estado:** Biblioteca completa y lista para producción con 29/29 tests pasando ✅

### 🔥 Cambios Críticos (Fase 17)

#### Fixed - Operador `~` (Negación Bitwise)

**Bug crítico corregido** que rompía toda la cadena de negación:

- **Problema:** `operator~()` tenía los argumentos del constructor invertidos

  ```cpp
  // INCORRECTO (antes):
  return int128_base_t(~data[0], ~data[1]); // Pasaba (~low, ~high) ❌
  
  // CORRECTO (ahora):
  return int128_base_t(~data[1], ~data[0]); // Pasa (high, low) ✅
  ```

- **Impacto:** Este bug rompía:
  - `operator-()` → usaba complemento a 2 con `~(*this) + 1`
  - `abs()` → usaba `is_negative() ? -(*this) : *this`
  - `to_string()` → usaba `abs()` para valores negativos

- **Síntoma:** Números negativos se mostraban como valores basura gigantes:

  ```cpp
  int128_t x(-42);
  x.to_string(); // Antes: "-340282366920938463444927863358058659882" ❌
                 // Ahora: "-42" ✅
  ```

- **Referencia:** Commit Fase 17, archivo `include_new/int128_base.hpp` línea 411

### 🧹 Mejoras de Calidad de Código

#### Changed - Reemplazo de Literales Hexadecimales

**Portabilidad mejorada** reemplazando 12 ocurrencias de valores hexadecimales mágicos:

| Antes | Después | Ubicación |
|-------|---------|-----------|
| `0xFFFFFFFFFFFFFFFFull` | `std::numeric_limits<uint64_t>::max()` | parse_base(), max(), UINT128_MAX |
| `0x7FFFFFFFFFFFFFFFull` | `std::numeric_limits<int64_t>::max()` | max(), INT128_MAX |
| `0x8000000000000000ull` | `static_cast<uint64_t>(std::numeric_limits<int64_t>::min())` | min(), INT128_MIN |

**Beneficios:**

- ✅ Código más portable entre plataformas
- ✅ Autodocumentado (claridad de intención)
- ✅ Eliminación de "números mágicos"
- ✅ Mejor conformidad con Modern C++

**Archivos modificados:**

- `include_new/int128_base.hpp` líneas: 1522-1523, 1652, 1664, 1668, 1718, 1721, 1724

### 📚 Documentación

#### Added - Comentarios Doxygen Completos

- ✅ Header principal con metadatos del proyecto
- ✅ Documentación completa de `enum signedness`
- ✅ Documentación exhaustiva de `template int128_base_t`
- ✅ Comentarios detallados en operadores clave:
  - `operator~()` con nota del fix crítico
  - `operator-()` explicando complemento a 2
  - `to_string()` y `to_string(int base)` con ejemplos
  - `abs()` con comportamiento signed/unsigned
- ✅ Tags Doxygen: `@brief`, `@param`, `@return`, `@details`, `@note`, `@par`, `@code`, `@see`

#### Changed - Actualización de Doxyfile

- `PROJECT_NUMBER` actualizado a `1.0.0`
- Configuración lista para generar documentación HTML completa

### 🧪 Testing

#### Nuevos Tests

- ✅ **test_tostring_debug.cpp** - Diagnóstico del bug operator~()
  - Test 1: Valores positivos
  - Test 2: Negación de valores negativos
  - Test 3: Parsing de strings con signo
  - Test 4: Función abs() con negativos
  - Test 5: Doble negación

#### Estado de Tests - 29/29 PASS ✅

| Test File | Tests | Estado |
|-----------|-------|--------|
| test_tostring_debug.cpp | 5/5 | ✅ PASS |
| test_parse_direct.cpp | 6/6 | ✅ PASS |
| test_parse_notostring.cpp | 6/6 | ✅ PASS |
| test_literals_fixed.cpp | 7/7 | ✅ PASS |
| test_constants.cpp | 5/5 | ✅ PASS |
| **TOTAL** | **29/29** | **✅ 100%** |

### 📦 Características Completas

#### Core Features (100% Completo)

- ✅ Template unificado `int128_base_t<signedness S>`
- ✅ Type aliases: `uint128_t`, `int128_t`
- ✅ Two's complement para signed, binario puro para unsigned
- ✅ Constructores: default, desde integrales, desde floating point, copia/movimiento
- ✅ Conversión explícita entre signed ↔ unsigned

#### Operadores (100% Completo)

- ✅ Aritméticos: `+`, `-`, `*`, `/`, `%`, negación unaria `-x`
- ✅ Comparación: `==`, `!=`, `<`, `<=`, `>`, `>=`, `<=>` (C++20)
- ✅ Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
- ✅ Asignación: `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
- ✅ Incremento/Decremento: `++x`, `x++`, `--x`, `x--`
- ✅ Simetría completa: friend operators para `T op uint128_t` y `uint128_t op T`

#### Conversiones (100% Completo)

- ✅ **to_string()**: Base 10 optimizada con `divrem_by_10()`
- ✅ **to_string(int base)**: Bases 2-36 (binario, octal, decimal, hex, etc.)
- ✅ **parse()**: Detección automática de base (0x, 0b, 0 prefixes)
- ✅ **parse_base()**: Parsing en base específica con manejo de errores
- ✅ **to/from floating point**: `float`, `double`, `long double`
- ✅ **Separadores de dígitos**: Soporte para `'` en parsing (`"1'234'567"`)

#### Literales UDL (100% Completo)

- ✅ Sufijos básicos: `42_u128`, `42_i128`
- ✅ Sufijos hexadecimales: `0xDEADBEEF_u128`, `0xFF_i128`
- ✅ Sufijos binarios: `0b11110000_u128`
- ✅ Sufijos octales: `0777_u128`
- ✅ String literals: `"12345"_U128`, `"0xFF"_U128`
- ✅ Factory functions: `make_uint128()`, `make_int128_signed()`, `make_int128_checked()`

#### Constantes Globales

- ✅ `UINT128_MAX` = 2^128 - 1 (340,282,366,920,938,463,463,374,607,431,768,211,455)
- ✅ `INT128_MAX` = 2^127 - 1 (170,141,183,460,469,231,731,687,303,715,884,105,727)
- ✅ `INT128_MIN` = -2^127 (-170,141,183,460,469,231,731,687,303,715,884,105,728)

#### Funciones Auxiliares (100% Completo)

- ✅ `high()`, `low()`: Acceso a palabras de 64 bits
- ✅ `set_high()`, `set_low()`: Modificación de palabras
- ✅ `is_zero()`: Verificación rápida de cero
- ✅ `is_negative()`: Verificación de signo (solo signed)
- ✅ `abs()`: Valor absoluto
- ✅ `min()`, `max()`: Constantes de límites

#### Optimizaciones

- ✅ Intrínsecos del compilador: `_umul128`, `_udiv128`, `__builtin_clzll`, etc.
- ✅ `divrem_by_10()` optimizado para to_string() rápido
- ✅ Detección automática de plataforma (MSVC, GCC, Clang, Intel)
- ✅ Constexpr everywhere: evaluación en compile-time cuando es posible

### 🔧 Compatibilidad

#### Compiladores Soportados

- ✅ **GCC** 15.2+ (UCRT64, MSYS2)
- ✅ **Clang** 19.1+ (CLANG64, MSYS2)
- ⚠️ **Intel oneAPI** ICX (compilación básica, requiere setup especial)
- ⚠️ **MSVC** 2022 (Visual Studio 17.12+, soporte limitado)

#### Plataformas Testeadas

- ✅ **Windows x86_64** (MSYS2) - 100% testeado
- 📋 **Linux x86_64** - Debería funcionar (no testeado)
- 📋 **macOS x86_64/ARM64** - Debería funcionar (no testeado)

#### Requisitos

- **C++ Standard:** C++20 o superior
- **Características C++20 usadas:**
  - `constexpr` extendido
  - `requires` clauses
  - `std::numeric_limits` para portabilidad
  - Three-way comparison `operator<=>`

### 📖 Documentación Disponible

- ✅ `README.md` - Guía completa de uso
- ✅ `PROMPT.md` - Especificación técnica y convenciones
- ✅ `TODO.md` - Roadmap y próximas fases
- ✅ `PORTABILITY_GUIDE.md` - Guía de portabilidad
- ✅ `DEV_ENV_VARS.md` - Configuración de entornos de compilación
- ✅ `CHANGELOG.md` - Este archivo

### 🎯 Próximos Pasos (Fase 1.5)

Ver `TODO.md` para el plan detallado de unificación de template signed/unsigned
con el objetivo de reducir el código de 4500 a 2500 líneas (45% menos).

---

## Leyenda de Cambios

- **Added:** Nuevas características
- **Changed:** Cambios en funcionalidad existente
- **Deprecated:** Características que serán removidas
- **Removed:** Características removidas
- **Fixed:** Correcciones de bugs
- **Security:** Correcciones de seguridad

---

**Mantenedores:** int128 Project Contributors  
**Licencia:** Boost Software License 1.0  
**Repositorio:** [Pendiente]  
**Documentación:** Ver `README.md` y comentarios Doxygen en código
