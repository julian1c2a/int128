# DOCUMENTATION_GRAPH.md - Grafo de Referencias de Documentación

> **Propósito:** Mapa completo de interconexiones entre archivos de documentación
> **Última actualización:** 2026-01-10 (Contador de actividad: #001)
> **Mantenedor:** Julián Calderón Almendros

---

## 📋 Índice

1. [Visión General](#visión-general)
2. [Grafo de Dependencias Principal](#grafo-de-dependencias-principal)
3. [Nodos del Grafo](#nodos-del-grafo)
4. [Categorías de Documentos](#categorías-de-documentos)
5. [Plan de Integración de Directorios](#plan-de-integración-de-directorios)
6. [Protocolo de Actualización](#protocolo-de-actualización)
7. [Contador de Actividad](#contador-de-actividad)

---

## Visión General

Este proyecto mantiene **114 archivos .md** organizados en un grafo de dependencias.
Cuando se actualiza un documento, deben verificarse y potencialmente actualizarse
los documentos conectados.

### Regla de Oro

> **Cuando actualices un documento, recorre sus aristas en el grafo y verifica
> si los documentos conectados necesitan cambios.**

---

## Grafo de Dependencias Principal

```
                              ┌─────────────────┐
                              │   README.md     │ ◄── Punto de entrada público
                              │   (usuario)     │
                              └────────┬────────┘
                                       │
              ┌────────────────────────┼────────────────────────┐
              │                        │                        │
              ▼                        ▼                        ▼
    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
    │  CHANGELOG.md   │    │    TODO.md      │    │   PROMPT.md     │
    │  (historial)    │◄──►│   (roadmap)     │    │  (convenciones) │
    └────────┬────────┘    └────────┬────────┘    └────────┬────────┘
             │                      │                      │
             │         ┌────────────┴────────────┐         │
             │         │                         │         │
             ▼         ▼                         ▼         ▼
    ┌─────────────────────────────────────────────────────────────┐
    │                    API_*.md (10 archivos)                    │
    │  ┌──────────────┬──────────────┬──────────────┬────────────┐ │
    │  │ API_INT128_  │ API_INT128_  │ API_INT128_  │ API_INT128_│ │
    │  │ BASE_TT.md   │ BASE_BITS.md │ BASE_CMATH.md│ BASE_*.md  │ │
    │  └──────────────┴──────────────┴──────────────┴────────────┘ │
    └─────────────────────────────────────────────────────────────┘
             │
             │
             ▼
    ┌─────────────────────────────────────────────────────────────┐
    │               Documentos de Soporte Técnico                  │
    │  ┌────────────────┬────────────────┬────────────────┐       │
    │  │ DEV_ENV_VARS   │ PORTABILITY_   │ TROUBLESHOOTING│       │
    │  │    .md         │   GUIDE.md     │     .md        │       │
    │  └────────────────┴────────────────┴────────────────┘       │
    └─────────────────────────────────────────────────────────────┘
```

---

## Nodos del Grafo

### 🔴 Nodos Críticos (Requieren actualización frecuente)

| Documento | Propósito | Actualizar cuando... | Conexiones |
|-----------|-----------|----------------------|------------|
| **[CHANGELOG.md](CHANGELOG.md)** | Historial de cambios | Cualquier cambio significativo | → TODO, README, API_* |
| **[TODO.md](TODO.md)** | Roadmap y estado | Cambios de fase/tarea | → CHANGELOG, README |
| **[README.md](README.md)** | Doc pública | Nuevas features/API | → CHANGELOG, TODO, API_* |

### 🟡 Nodos de Referencia API (cppreference style)

| Documento | Header Correspondiente | Synopsis |
|-----------|------------------------|----------|
| [API_INT128_BASE_TT.md](API_INT128_BASE_TT.md) | `int128_base_tt.hpp` | Template principal `int128_base_t<S>` |
| [API_INT128_BASE_LIMITS.md](API_INT128_BASE_LIMITS.md) | `int128_base_limits.hpp` | `std::numeric_limits` |
| [API_INT128_BASE_TRAITS.md](API_INT128_BASE_TRAITS.md) | `int128_base_traits*.hpp` | Type traits STL |
| [API_INT128_BASE_CONCEPTS.md](API_INT128_BASE_CONCEPTS.md) | `int128_base_concepts.hpp` | Concepts C++20 |
| [API_INT128_BASE_BITS.md](API_INT128_BASE_BITS.md) | `int128_base_bits.hpp` | Operaciones de bits |
| [API_INT128_BASE_NUMERIC.md](API_INT128_BASE_NUMERIC.md) | `int128_base_numeric.hpp` | Algoritmos numéricos |
| [API_INT128_BASE_ALGORITHM.md](API_INT128_BASE_ALGORITHM.md) | `int128_base_algorithm.hpp` | Algoritmos STL |
| [API_INT128_BASE_CMATH.md](API_INT128_BASE_CMATH.md) | `int128_base_cmath.hpp` | Funciones matemáticas |
| [API_INT128_BASE_IOSTREAMS.md](API_INT128_BASE_IOSTREAMS.md) | `int128_base_iostreams.hpp` | Stream I/O |
| [API_INT128_BASE_FORMAT.md](API_INT128_BASE_FORMAT.md) | `int128_base_format.hpp` | Formateo avanzado |

### 🟢 Nodos de Soporte Técnico

| Documento | Propósito |
|-----------|-----------|
| [DEV_ENV_VARS.md](DEV_ENV_VARS.md) | Variables de entorno de compiladores |
| [PORTABILITY_GUIDE.md](PORTABILITY_GUIDE.md) | Guía de portabilidad multiplataforma |
| [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | Solución de problemas comunes |
| [COMPILER_REQUIREMENTS.md](COMPILER_REQUIREMENTS.md) | Requisitos de compiladores |
| [PROMPT.md](PROMPT.md) | Convenciones de código (para AI/dev) |

### 🔵 Nodos de Análisis y Reportes

| Documento | Contenido |
|-----------|-----------|
| [BENCHMARK_INT128_VS_UINT128.md](BENCHMARK_INT128_VS_UINT128.md) | Comparación rendimiento |
| [BOOST_COMPARISON_ANALYSIS.md](BOOST_COMPARISON_ANALYSIS.md) | vs Boost.Multiprecision |
| [THREAD_SAFETY_SUMMARY.md](THREAD_SAFETY_SUMMARY.md) | Análisis thread safety |
| [KARATSUBA_MULTIPLICATION_ANALYSIS.md](KARATSUBA_MULTIPLICATION_ANALYSIS.md) | Análisis algoritmo |

---

## Categorías de Documentos

### 1. Documentos Primarios (siempre actualizados)

```
README.md ←→ CHANGELOG.md ←→ TODO.md
     ↓            ↓             ↓
   Usuario    Historial     Roadmap
```

### 2. Referencia API (estilo cppreference)

```
API_INT128_BASE_TT.md (núcleo)
    ├── API_INT128_BASE_LIMITS.md
    ├── API_INT128_BASE_TRAITS.md
    ├── API_INT128_BASE_CONCEPTS.md
    ├── API_INT128_BASE_BITS.md
    ├── API_INT128_BASE_NUMERIC.md
    ├── API_INT128_BASE_ALGORITHM.md
    ├── API_INT128_BASE_CMATH.md
    ├── API_INT128_BASE_IOSTREAMS.md
    └── API_INT128_BASE_FORMAT.md
```

### 3. Documentos de Feature/Completion

| Prefijo | Tipo |
|---------|------|
| `*_SUMMARY.md` | Resúmenes de implementación |
| `*_COMPLETED.md` | Features completadas |
| `*_ANALYSIS.md` | Análisis técnicos |
| `*_REPORT.md` | Reportes de estado |

### 4. Documentos de Proceso

| Documento | Uso |
|-----------|-----|
| `PHASE_*.md` | Documentación de fases |
| `REFACTORING_*.md` | Planes de refactorización |
| `MIGRATION_*.md` | Guías de migración |

---

## Plan de Integración de Directorios

### Estado Actual

```
Directorio         Estado              Destino Final
─────────────────────────────────────────────────────
include/           ✅ Canónico         → Mantener
include_new/       🔄 En migración     → Renombrar a include/
tests/             ✅ Canónico         → Mantener (ampliar)
tests_new/         🔄 Pendiente        → Integrar en tests/
experimental/      🔄 Pendiente        → Integrar en tests/
```

### Plan de Migración

#### Fase A: Consolidación de `include_new/` → `include/`

```bash
# 1. Verificar que include_new/ tiene versión completa
# 2. Backup de include/ actual
# 3. Migración atómica:
mv include/ include_legacy/
mv include_new/ include/

# 4. Actualizar #include paths en tests y demos
# 5. Verificar compilación completa
# 6. Eliminar include_legacy/ cuando todo OK
```

**Headers en `include_new/` (17 archivos):**

- `int128_base_tt.hpp` - Template principal
- `int128_base_***.hpp` - 13 módulos de features
- `type_traits.hpp` - Concepts base
- `intrinsics/` - 5 archivos de operaciones de bajo nivel
- `specializations/` - Optimizaciones modulares

**Headers en `include/` (19 archivos actuales):**

- Mismo contenido + `int128.hpp` (wrapper legacy)

#### Fase B: Integración de `tests_new/` → `tests/`

```
tests_new/                    →  tests/
├── test_*_template.cpp            └── template/ (nuevo subdir)
├── int128_base_safe_tests.cpp     └── int128_safe_extracted_tests.cpp
└── int128_base_thread_safety_tests.cpp → int128_thread_safety_extracted_tests.cpp
```

**Archivos en `tests_new/` (14):**

```
int128_base_safe_tests.cpp
int128_base_thread_safety_tests.cpp
test_algorithm_template.cpp
test_bits_template.cpp
test_cmath_template.cpp
test_concepts_template.cpp
test_format_template.cpp
test_iostreams_template.cpp
test_limits_template.cpp
test_numeric_template.cpp
test_ranges_template.cpp
test_safe_template.cpp
test_thread_safety_template.cpp
test_traits_template.cpp
```

#### Fase C: Integración de `experimental/` → `tests/`

```
experimental/                 →  tests/experimental/
├── test_*_template.cpp            (mismo nombre)
```

**Archivos en `experimental/` (12):**

```
test_algorithm_template.cpp
test_bits_template.cpp
test_cmath_template.cpp
test_concepts_template.cpp
test_format_template.cpp
test_iostreams_template.cpp
test_limits_template.cpp
test_numeric_template.cpp
test_ranges_template.cpp
test_safe_template.cpp
test_thread_safety_template.cpp
test_traits_template.cpp
```

---

## Protocolo de Actualización

### Al modificar cualquier documento

1. **Identificar nodo en el grafo**
2. **Verificar conexiones salientes**
3. **Actualizar documentos conectados si es necesario**
4. **Incrementar contador de actividad**
5. **Registrar en CHANGELOG.md**

### Checklist de Actualización

```markdown
□ Documento modificado: _______________
□ Fecha/hora: _______________
□ Documentos conectados verificados:
  □ README.md (si afecta usuario)
  □ CHANGELOG.md (siempre)
  □ TODO.md (si afecta roadmap)
  □ API_*.md (si afecta signatures)
□ Contador incrementado en DOCUMENTATION_GRAPH.md
```

---

## Contador de Actividad

### Sesión Actual: 2026-01-10

| # | Hora | Acción | Documentos Afectados |
|---|------|--------|---------------------|
| 001 | 12:00 | Creación grafo documentación | DOCUMENTATION_GRAPH.md, CHANGELOG.md |
| 002 | 12:05 | Actualización README y TODO | README.md, TODO.md (plan Fase 1.6) |
| 003 | 12:10 | **FASE A COMPLETADA** include_new/ → include/ | CHANGELOG.md, TODO.md |

### Historial de Sesiones

| Fecha | Sesión | Actividades | Total Cambios |
|-------|--------|-------------|---------------|
| 2026-01-10 | #1 | Grafo docs, **Fase A completada** | 3 |
| 2026-01-08 | - | Licencias Boost, API docs | 15+ |
| 2026-01-07 | - | Features: concepts, bits, numeric, algorithm | 20+ |

---

## Referencias

- [CHANGELOG.md](CHANGELOG.md) - Historial detallado
- [TODO.md](TODO.md) - Estado de fases
- [PROMPT.md](PROMPT.md) - Convenciones de código
- [README.md](README.md) - Documentación de usuario

---

**Licencia:** Boost Software License 1.0  
**Autor:** Julián Calderón Almendros <julian.calderon.almendros@gmail.com>
