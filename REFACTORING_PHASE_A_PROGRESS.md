# Progreso de Refactorización - Fase A (Opción A)

**Fecha**: 2026-01-05  
**Autor**: Julián Calderón Almendros  
**Tarea**: Añadir constantes sz_N_Uint64_t, MSULL, LSULL y refactorizar `data[]` accesses

---

## ✅ Completado (Fase A.1 - A.4)

### A.1: Constantes de Tamaño ✅

- ✅ Añadidas constantes después de `data[2]` (líneas ~120-135):
  - `static constexpr size_t sz_N_Uint64_t = 2`
  - `static constexpr size_t MSULL = 1`
  - `static constexpr size_t LSULL = 0`

### A.2: Constructores ✅

- ✅ Constructor por defecto
- ✅ Constructor desde integrales builtin
- ✅ Constructor desde high/low
- ✅ Constructor desde floating point (signed y unsigned)

### A.3: Operadores de Asignación ✅

- ✅ `operator=(const int128_base_t<S2>&)`
- ✅ `operator=(int128_base_t<S2>&&)`
- ✅ `operator=(integral_builtin)`
- ✅ `operator=(floating_point_builtin)`

### A.4: Accessors ✅

- ✅ `high()` → `data[MSULL]`
- ✅ `low()` → `data[LSULL]`
- ✅ `set_high()` → `data[MSULL]`
- ✅ `set_low()` → `data[LSULL]`
- ✅ `operator~()` → `int128_base_t(~data[MSULL], ~data[LSULL])`

### A.5: Operadores Aritméticos ✅

- ✅ `operator+=(int128_base_t<S2>&)` - Suma cross-signedness
- ✅ `operator-=(int128_base_t<S2>&)` - Resta cross-signedness
- ✅ `operator*=(integral_builtin)` - Multiplicación optimizada (Fase 0.5)

---

## ⏳ Pendiente (Fase A.5+)

### A.6: Operadores Bitwise (20+ referencias)

```cpp
// Same-signedness operators
data[0] &= other.data[0];  // → data[LSULL] &= other.data[LSULL];
data[1] &= other.data[1];  // → data[MSULL] &= other.data[MSULL];

// Cross-signedness operators (ya usan .low()/.high(), están OK)
data[0] &= other.low();    // → data[LSULL] &= other.low();
data[1] &= other.high();   // → data[MSULL] &= other.high();
```

**Operadores afectados**:

- `operator&=` (2 versiones)
- `operator|=` (2 versiones)
- `operator^=` (2 versiones)

### A.7: Operadores Shift (30+ referencias)

```cpp
// operator<<=
data[0] = 0;               // → data[LSULL] = 0;
data[1] = 0;               // → data[MSULL] = 0;
data[0] << (shift - 64);   // → data[LSULL] << (shift - 64);
data[1] << shift;          // → data[MSULL] << shift;
// etc.

// operator>>=
data[1] < 0;               // → data[MSULL] < 0; (sign check)
data[0] >> shift;          // → data[LSULL] >> shift;
data[1] >> shift;          // → data[MSULL] >> shift;
// etc.
```

### A.8: Operadores de Comparación (10+ referencias)

```cpp
// operator==
data[0] == other.data[0]   // → data[LSULL] == other.data[LSULL]
data[1] == other.data[1]   // → data[MSULL] == other.data[MSULL]

// operator<
data[1] < other.data[1]    // → data[MSULL] < other.data[MSULL]
data[0] < other.data[0]    // → data[LSULL] < other.data[LSULL]

// operator< con signed/unsigned mixto
static_cast<int64_t>(data[1]) < 0  // → static_cast<int64_t>(data[MSULL]) < 0
```

### A.9: Conversiones (15+ referencias)

```cpp
// operator bool
data[0] != 0 || data[1] != 0  // → data[LSULL] != 0 || data[MSULL] != 0

// operator floating_point
data[1] * 2^64 + data[0]  // → data[MSULL] * 2^64 + data[LSULL]

// to_string() operations
data[0] != 0 || data[1] != 0  // → data[LSULL] != 0 || data[MSULL] != 0
temp.data[0] != 0 || temp.data[1] != 0  // → temp.data[LSULL] || temp.data[MSULL]
```

### A.10: Funciones Helper (30+ referencias)

```cpp
// count_leading_zeros
data[1] != 0;              // → data[MSULL] != 0;
intrinsics::clz64(data[1]) // → intrinsics::clz64(data[MSULL])
intrinsics::clz64(data[0]) // → intrinsics::clz64(data[LSULL])

// is_negative
static_cast<int64_t>(data[1]) < 0  // → static_cast<int64_t>(data[MSULL]) < 0

// divrem
divisor.data[0] == 0 && divisor.data[1] == 0  // → divisor.data[LSULL] == 0 && divisor.data[MSULL] == 0
data[0] == 0 && data[1] == 0                  // → data[LSULL] == 0 && data[MSULL] == 0
// ... (muchas más en divrem_64bit y divrem_binary)
```

---

## 📊 Estadísticas

| Categoría | Referencias `data[0]` | Referencias `data[1]` | Total | Estado |
|-----------|----------------------|----------------------|-------|--------|
| Constructores | ~10 | ~10 | ~20 | ✅ Completo |
| Operadores asignación | ~15 | ~15 | ~30 | ✅ Completo |
| Accessors | 2 | 2 | 4 | ✅ Completo |
| Operadores aritméticos | ~8 | ~8 | ~16 | ✅ Completo |
| Operadores bitwise | ~12 | ~12 | ~24 | ⏳ Pendiente |
| Operadores shift | ~15 | ~15 | ~30 | ⏳ Pendiente |
| Operadores comparación | ~8 | ~8 | ~16 | ⏳ Pendiente |
| Conversiones | ~10 | ~5 | ~15 | ⏳ Pendiente |
| Helper functions | ~20 | ~20 | ~40 | ⏳ Pendiente |
| **TOTAL** | **~100** | **~95** | **~195** | **~30% completo** |

---

## 🎯 Estrategia para Completar

### Opción 1: Continuar Manualmente (Estimado: 1-2 horas)

- Refactorizar sección por sección usando `multi_replace_string_in_file`
- Ventaja: Control total, validación inmediata
- Desventaja: Tedioso, propenso a errores humanos

### Opción 2: Script Python Automatizado (Estimado: 30 minutos)

```python
# scripts/refactor_data_to_constants.py
import re

def refactor_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    
    # Regex patterns
    replacements = [
        (r'\bdata\[0\]', 'data[LSULL]'),
        (r'\bdata\[1\]', 'data[MSULL]'),
        (r'other\.data\[0\]', 'other.data[LSULL]'),
        (r'other\.data\[1\]', 'other.data[MSULL]'),
        (r'divisor\.data\[0\]', 'divisor.data[LSULL]'),
        (r'divisor\.data\[1\]', 'divisor.data[MSULL]'),
        (r'temp\.data\[0\]', 'temp.data[LSULL]'),
        (r'temp\.data\[1\]', 'temp.data[MSULL]'),
    ]
    
    for pattern, replacement in replacements:
        content = re.sub(pattern, replacement, content)
    
    with open(filepath, 'w') as f:
        f.write(content)

refactor_file('include_new/int128_base.hpp')
```

**Ventaja**: Rápido, consistente  
**Desventaja**: Puede reemplazar comentarios o documentación que no debería cambiar

### Opción 3: Híbrido (Recomendado)

1. Script automatizado para mayoría de casos (~70% restante)
2. Revisión manual de cambios críticos:
   - Comentarios de documentación
   - Código en constexpr functions
   - Casos edge en divrem()
3. Validación con compilación y tests

---

## 🚀 Recomendación

**Usar Opción 3 Híbrido**:

1. **Crear script Python** que refactorice automáticamente
2. **Ejecutar script** sobre `int128_base.hpp`
3. **Revisar diff** manualmente para verificar no rompimos nada
4. **Compilar y testear**: Ejecutar tests para validar cambios

**Ventaja principal**: Ahorra tiempo, mantiene calidad

**¿Proceder con script automatizado?** (Y/N)
