# Plan de Refactorización - Fase 19

**Fecha**: 2026-01-07  
**Autor**: Julián Calderón Almendros <julian.calderon.almendros@gmail.com>

## Resumen Ejecutivo

Análisis de los cambios arquitectónicos propuestos para `int128_base.hpp` después de la unificación en Fase 1.5.

---

## Situación Actual

### Estado del operator-()

✅ **YA DOCUMENTADO CORRECTAMENTE** (líneas 495-520)

```cpp
/**
 * @brief Operador de negación aritmética (complemento a 2)
 * 
 * @details Implementa la negación mediante complemento a 2: -x = ~x + 1
 * Este operador funciona idénticamente para signed y unsigned (wrap-around).
 *
 * **Para int128_t (signed):**
 * - -42 → -42 (representación two's complement)
 * - -INT128_MIN → INT128_MIN (overflow, comportamiento definido)
 *
 * **Para uint128_t (unsigned):**
 * - -42 → 2^128 - 42 (aritmética modular)
 * - -0 → 0
 *
 * @note Depende de operator~(), que fue corregido en Fase 17
 */
```

**Conclusión Issue #1**: ✅ **NO REQUIERE CAMBIOS**

---

### Estado del operator*= Optimization

✅ **YA OPTIMIZADO EN FASE 0.5** (líneas 662-689)

```cpp
template <integral_builtin T> constexpr int128_base_t& operator*=(T other) noexcept
{
    const uint64_t b = static_cast<uint64_t>(other);

    // Fast path: valor cabe en 64 bits
    if (data[1] == 0) {
        const uint64_t low_part = intrinsics::umul128(data[0], b, &data[1]);
        data[0] = low_part;
        return *this;
    }

    // General case: multiplicación 128×64
    uint64_t high_part;
    const uint64_t low_part = intrinsics::umul128(data[0], b, &high_part);
    const uint64_t cross_product = data[1] * b;
    data[0] = low_part;
    data[1] = high_part + cross_product;
    return *this;
}
```

**Características implementadas**:

- ✅ Fast-path optimization (data[1] == 0): 1 multiplicación
- ✅ General case optimization: 2 multiplicaciones
- ✅ Performance: 75% mejora en caso común

**Nota sobre sign extension**:

- El código actual NO incluye corrección explícita de signo
- Comentario afirma: "La multiplicación en complemento a 2 funciona automáticamente"
- **Verificación necesaria**: Confirmar que esto es correcto matemáticamente

**Conclusión Issue #NEW**: ✅ **YA IMPLEMENTADO** (pero verificar sign extension)

---

## Issue #2: Operadores Duplicados (PENDIENTE)

### Patrón de Duplicación Identificado

**Ejemplo con operator+=**:

```cpp
// LÍNEAS 523-535: Same-signedness version (DUPLICADO)
constexpr int128_base_t& operator+=(const int128_base_t& other) noexcept
{
    uint64_t temp_low = 0;
    const unsigned char carry = intrinsics::add_u64(data[0], other.data[0], &temp_low);
    data[0] = temp_low;
    uint64_t temp_high = 0;
    intrinsics::addcarry_u64(carry, data[1], other.data[1], &temp_high);
    data[1] = temp_high;
    return *this;
}

// LÍNEAS 587-596: Template version (MANTENERLA)
template <signedness S2>
constexpr int128_base_t& operator+=(const int128_base_t<S2>& other) noexcept
{
    uint64_t temp_low = 0;
    const unsigned char carry = intrinsics::add_u64(data[0], other.low(), &temp_low);
    data[0] = temp_low;
    uint64_t temp_high = 0;
    intrinsics::addcarry_u64(carry, data[1], other.high(), &temp_high);
    data[1] = temp_high;
    return *this;
}
```

**Problema**: Cuando `S2 == S`, el template maneja el caso → versión non-template redundante

### Operadores Afectados

| Operador | Lines (same-signedness) | Lines (template S2) | Duplicado |
|----------|-------------------------|---------------------|-----------|
| **operator+=** | 523-535 | 587-596 | ✅ |
| **operator+** | 537-541 | 598-603 | ✅ |
| **operator-=** | 543-555 | 605-614 | ✅ |
| **operator-** | 557-561 | 616-621 | ✅ |
| **operator*=** | 628-658 | 698-702 | ✅ |
| **operator*** | 660-664 | 704-709 | ✅ (non-const) |
| **operator/=** | 715-720 | 773-777 | ✅ |
| **operator/** | 722-726 | 779-784 | ✅ |
| **operator%=** | 728-733 | 786-790 | ✅ |
| **operator%** | 735-739 | 792-797 | ✅ |
| **operator&=** | 803-808 | 810-815 | ✅ |
| **operator&** | 817-822 | 824-829 | ✅ |
| **operator\|=** | 840-845 | 847-852 | ✅ |

**Total estimado**: ~15-20 operadores duplicados

### Estrategia de Eliminación

**Para cada operador duplicado**:

1. **Localizar ambas versiones** (same-signedness + template)
2. **Verificar equivalencia semántica**:
   - Same-signedness: `other.data[0]`, `other.data[1]`
   - Template: `other.low()`, `other.high()`
   - Ambos acceden a los mismos datos
3. **Eliminar versión same-signedness**
4. **Mantener template version** (maneja ambos casos S2 == S y S2 != S)

**Ejemplo de refactorización**:

```cpp
// ANTES: Dos versiones
constexpr int128_base_t& operator+=(const int128_base_t& other) noexcept { ... }
template <signedness S2>
constexpr int128_base_t& operator+=(const int128_base_t<S2>& other) noexcept { ... }

// DESPUÉS: Solo template
template <signedness S2>
constexpr int128_base_t& operator+=(const int128_base_t<S2>& other) noexcept { ... }
```

**Beneficios**:

- ✅ Reduce código duplicado (~1500 líneas)
- ✅ Mantenimiento más simple (un solo lugar para cambios)
- ✅ Template maneja automáticamente S2 == S

**Riesgos**:

- ⚠️ Cambios en resolución de sobrecarga
- ⚠️ Posibles warnings de templates innecesarios cuando S2 == S
- ⚠️ Requiere testing exhaustivo

---

## Issue #3: Friend Operators Cross-Signedness (PENDIENTE)

### Estado Actual

**Friend operators existentes** (líneas 1086+):

```cpp
// Para tipos builtin (CORRECTO)
template <integral_builtin T>
friend constexpr bool operator==(const int128_base_t& lhs, T rhs) noexcept;

template <integral_builtin T>
friend constexpr bool operator==(T lhs, const int128_base_t& rhs) noexcept;
```

**Friend operators FALTANTES**: Cross-signedness (int128_t ↔ uint128_t)

### Problema Identificado

**Usuario señala**: Template debe estar en LHS (parámetro externo) para conversiones simétricas

**Pattern incorrecto** (si existe):

```cpp
// MAL: Template en RHS
template <signedness S2>
friend constexpr bool operator==(const int128_base_t& lhs, const int128_base_t<S2>& rhs) noexcept;
```

**Pattern correcto** (a implementar):

```cpp
// BIEN: Template en LHS
template <signedness S1>
friend constexpr bool operator==(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept;
```

### Friend Operators a Añadir

**Comparaciones básicas**:

```cpp
template <signedness S1>
friend constexpr bool operator==(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept
{
    return lhs.data[0] == rhs.data[0] && lhs.data[1] == rhs.data[1];
}

template <signedness S1>
friend constexpr bool operator!=(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept
{
    return !(lhs == rhs);
}
```

**Comparaciones ordenadas** (requiere lógica signed/unsigned):

```cpp
template <signedness S1>
friend constexpr bool operator<(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept
{
    // Manejar comparación signed/unsigned correctamente
    if constexpr (int128_base_t<S1>::is_signed && !is_signed) {
        // signed < unsigned
        if (lhs.is_negative()) return true;  // Negative signed always < unsigned
        return lhs.to_uint128() < rhs;
    }
    else if constexpr (!int128_base_t<S1>::is_signed && is_signed) {
        // unsigned < signed
        if (rhs.is_negative()) return false; // Unsigned never < negative signed
        return lhs < rhs.to_uint128();
    }
    else {
        // Same signedness
        if constexpr (is_signed) {
            // Comparación signed
            if (lhs.data[1] != rhs.data[1]) {
                return static_cast<int64_t>(lhs.data[1]) < static_cast<int64_t>(rhs.data[1]);
            }
            return lhs.data[0] < rhs.data[0];
        } else {
            // Comparación unsigned
            if (lhs.data[1] != rhs.data[1]) {
                return lhs.data[1] < rhs.data[1];
            }
            return lhs.data[0] < rhs.data[0];
        }
    }
}
```

**Operadores restantes**: <=, >, >= (derivados de < y ==)

**Total a añadir**: ~6 friend operators × 2 sobrecargas cada uno = ~12 funciones

---

## Sign Extension en Multiplicación

### Issue Descubierto

ORIGINAL.hpp incluye corrección explícita de signo:

```cpp
// ORIGINAL (líneas 1620-1658)
if constexpr (std::is_signed_v<T>) {
    if (other < 0) {
        data[1] -= data[0];  // ← Corrección de signo
    }
}
```

**Actual int128_base.hpp** (líneas 662-689):

```cpp
// NO incluye corrección de signo
// Comentario afirma: "multiplicación en complemento a 2 funciona automáticamente"
```

### Verificación Matemática Necesaria

**Hipótesis actual**: Multiplicación en complemento a 2 es idéntica para signed/unsigned

**Teorema (Two's Complement Multiplication)**:

```
∀ n, m ∈ ℤ : repr(n × m) = repr(n) × repr(m)  (mod 2^N)
```

**Implicación**: Multiplicación de bits es IDÉNTICA para signed/unsigned

**Conclusión**:

- ✅ Si el teorema es correcto → NO se necesita corrección de signo
- ❌ Si hay casos edge → Necesitamos corrección explícita como ORIGINAL

**Acción requerida**:

1. Verificar teorema matemáticamente
2. Si es correcto → documentar por qué no necesita corrección
3. Si es incorrecto → añadir corrección como ORIGINAL

---

## Plan de Implementación

### Fase 1: Eliminación de Operadores Duplicados (~40-60 operaciones)

**Orden de eliminación**:

1. Operadores aritméticos: +, -, *, /, % (~10 pares)
2. Operadores bitwise: &, |, ^, <<, >> (~10 pares)
3. Operadores de comparación: ==, !=, <, <=, >, >= (~6 pares)

**Para cada operador**:

```bash
# 1. Localizar ambas versiones
grep -n "operator+=" int128_base.hpp

# 2. Leer contexto
read_file lines X-Y

# 3. Verificar equivalencia
# 4. Eliminar versión non-template con replace_string_in_file
# 5. Verificar compilación
g++ -std=c++20 -Iinclude_new tests/test_basic.cpp
```

### Fase 2: Añadir Friend Operators Cross-Signedness (~15-25 operaciones)

**Ubicación**: Después de línea 1070 (sección de comparaciones)

**Operadores a añadir**:

```cpp
// ============================================================================
// COMPARACIONES CROSS-SIGNEDNESS (int128_t ↔ uint128_t)
// ============================================================================

template <signedness S1>
friend constexpr bool operator==(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }

template <signedness S1>
friend constexpr bool operator!=(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }

template <signedness S1>
friend constexpr bool operator<(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }

template <signedness S1>
friend constexpr bool operator<=(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }

template <signedness S1>
friend constexpr bool operator>(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }

template <signedness S1>
friend constexpr bool operator>=(const int128_base_t<S1>& lhs, const int128_base_t& rhs) noexcept { ... }
```

### Fase 3: Verificar Sign Extension (~5-10 operaciones)

**Opciones**:

1. **Si teorema es correcto**:
   - Añadir comentario explicando teorema
   - Referenciar prueba matemática
   - Documentar en TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md

2. **Si teorema tiene edge cases**:
   - Añadir corrección como ORIGINAL
   - Documentar casos donde se necesita

### Fase 4: Testing Exhaustivo (~10-15 operaciones)

**Tests a ejecutar**:

```bash
# 1. Tests básicos
g++ -std=c++20 -Iinclude_new test_tostring_debug.cpp -o build/test1.exe && .\build\test1.exe

# 2. Tests de parsing
g++ -std=c++20 -Iinclude_new test_parse_direct.cpp -o build/test3.exe && .\build\test3.exe

# 3. Tests de literales
g++ -std=c++20 -Iinclude_new test_literals_fixed.cpp -o build/test5.exe && .\build\test5.exe

# 4. Tests de constantes
g++ -std=c++20 -Iinclude_new test_constants.cpp -o build/test6.exe && .\build\test6.exe

# 5. Tests edge case
g++ -std=c++20 -Iinclude_new test_tostring_edges.cpp -o build/test2.exe && .\build\test2.exe
```

**Criterio de éxito**: 79+ tests, 100% pass rate

### Fase 5: Actualización de Documentación (~10-15 operaciones)

**Archivos a actualizar**:

1. **CHANGELOG.md**: Nueva versión 1.1.0

   ```markdown
   ## [1.1.0] - 2026-01-07
   
   ### Refactorización Arquitectónica
   - Eliminados ~1500 líneas de operadores duplicados
   - Añadidos friend operators para int128_t ↔ uint128_t
   - Verificada optimización operator*= (ya implementada en Fase 0.5)
   
   ### Mejoras de Documentación
   - operator-() documentado para unsigned (ya existía)
   - Añadida prueba matemática de multiplicación two's complement
   
   ### Performance
   - Mantenido: operator*= con optimización fast-path (75% mejora)
   ```

2. **API_INT128_BASE_T.md**: Actualizar firmas de operadores

3. **README.md**: Notar cambios arquitectónicos

4. **VERIFICATION_REPORT.md**: Nueva versión

5. **REFACTORING_PHASE_19.md**: Resumen de cambios

---

## Estimación de Esfuerzo

| Fase | Operaciones | Tiempo estimado |
|------|-------------|-----------------|
| Fase 1: Eliminar duplicados | 40-60 | 1-2 horas |
| Fase 2: Friend operators | 15-25 | 30-60 min |
| Fase 3: Sign extension | 5-10 | 15-30 min |
| Fase 4: Testing | 10-15 | 30-45 min |
| Fase 5: Documentación | 10-15 | 30-45 min |
| **TOTAL** | **80-125** | **3-5 horas** |

---

## Criterios de Éxito

### Código

- ✅ Eliminados todos los operadores same-signedness duplicados
- ✅ Añadidos 6 friend operators cross-signedness
- ✅ Verificada corrección de sign extension en multiplicación
- ✅ Reducción neta: ~1250 líneas (1500 eliminadas - 250 añadidas)

### Testing

- ✅ 79+ tests compilando sin warnings
- ✅ 100% pass rate mantenido
- ✅ Sin regresiones en funcionalidad

### Documentación

- ✅ CHANGELOG.md actualizado a versión 1.1.0
- ✅ API_INT128_BASE_T.md con firmas actualizadas
- ✅ REFACTORING_PHASE_19.md documentando cambios
- ✅ TWOS_COMPLEMENT_MULTIPLICATION_PROOF.md (si aplica)

---

## Conclusiones

### Issues Originales - Estado Final

1. **Issue #1 (operator-() docs)**: ✅ **YA RESUELTO** - No requiere cambios
2. **Issue #2 (operadores duplicados)**: ⏳ **PENDIENTE** - Requiere ~40-60 operaciones
3. **Issue #3 (friend operators)**: ⏳ **PENDIENTE** - Requiere ~15-25 operaciones

### Issue Nuevo

1. **operator*= optimization**: ✅ **YA IMPLEMENTADO** - Verificar sign extension

### Recomendación

**Proceder con Fases 1-5** eliminando duplicados, añadiendo friends, y verificando sign extension.

**Beneficio estimado**:

- Reducción de código: ~1250 líneas
- Mejor mantenibilidad: Un solo lugar por operador
- Funcionalidad completa: Cross-signedness operadores

---

**Fin del plan de refactorización**
