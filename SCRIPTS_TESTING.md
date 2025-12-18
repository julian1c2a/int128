# Scripts de Compilación y Ejecución - uint128_extracted_tests

## Tests Integrados

El archivo `tests/uint128_extracted_tests.cpp` ahora incluye **84 tests en total**:
- **66 tests originales** extraídos de la documentación
- **18 tests adicionales** de robustez (copy/move, UDL, streams, edge cases, constexpr)

## Scripts Disponibles

Los scripts están ubicados en el directorio `scripts/`. También hay un wrapper conveniente `test.bash` en la raíz.

### 0. `test.bash` - Atajo Rápido (Wrapper) 🚀
Wrapper conveniente en la raíz del proyecto que ejecuta `scripts/test_extracted.bash`.

**Uso:**
```bash
./test.bash [gcc|clang|msvc|all]
```

Este es el método más rápido para ejecutar tests desde la raíz.

---

### 1. `scripts/build_extracted_tests.bash` - Compilar tests
Compila los tests de `tests/uint128_extracted_tests.cpp` con uno o todos los compiladores.

**Uso:**
```bash
scripts/build_extracted_tests.bash [gcc|clang|msvc|all]
```

**Ejemplos:**
```bash
scripts/build_extracted_tests.bash gcc     # Solo GCC
scripts/build_extracted_tests.bash clang   # Solo Clang
scripts/build_extracted_tests.bash msvc    # Solo MSVC
scripts/build_extracted_tests.bash all     # Todos (por defecto)
scripts/build_extracted_tests.bash         # Todos (por defecto)
```

**Salida:**
- GCC: `build/build_tests/gcc/release/test_uint128_extracted.exe`
- Clang: `build/build_tests/clang/release/uint128_extracted_tests.exe`
- MSVC: `build/build_tests/msvc/release/uint128_extracted_tests.exe`

---

### 2. `scripts/run_uint128_extracted_tests.bash` - Ejecutar tests
Ejecuta los tests previamente compilados.

**Uso:**
```bash
scripts/run_uint128_extracted_tests.bash [gcc|clang|msvc|all]
```

**Ejemplos:**
```bash
scripts/run_uint128_extracted_tests.bash gcc     # Solo GCC
scripts/run_uint128_extracted_tests.bash clang   # Solo Clang
scripts/run_uint128_extracted_tests.bash msvc    # Solo MSVC
scripts/run_uint128_extracted_tests.bash all     # Todos (por defecto)
scripts/run_uint128_extracted_tests.bash         # Todos (por defecto)
```

**Nota:** Los ejecutables deben existir (compilados previamente).

---

### 3. `scripts/test_extracted.bash` - Compilar y Ejecutar (Todo en Uno) ⭐
Compila y ejecuta los tests en un solo paso.

**Uso:**
```bash
scripts/test_extracted.bash [gcc|clang|msvc|all]
```

**Ejemplos:**
```bash
scripts/test_extracted.bash gcc     # Compilar y ejecutar con GCC
scripts/test_extracted.bash clang   # Compilar y ejecutar con Clang
scripts/test_extracted.bash msvc    # Compilar y ejecutar con MSVC
scripts/test_extracted.bash all     # Compilar y ejecutar con todos
scripts/test_extracted.bash         # Compilar y ejecutar con todos (por defecto)
```

Este es el script más conveniente para uso rápido.

---

## Configuración para MSVC

### Activación Automática (Recomendado)

Los scripts intentan activar automáticamente el entorno de Visual Studio usando `vcvarsall.py`.

**Desde terminal bash de MSYS64/UCRT64:**
```bash
./test.bash msvc
# o
scripts/test_extracted.bash msvc
```

El script detectará automáticamente si `cl.exe` no está disponible y activará el entorno.

### Activación Manual (Alternativa)

Si la activación automática no funciona, puedes activar manualmente:

```bash
source activate_msvc.bash
```

Luego ejecuta los scripts normalmente:

```bash
./test.bash msvc
```

---

## Flujo de Trabajo Recomendado

### Desarrollo Rápido (un compilador)
```bash
scripts/test_extracted.bash gcc
```

### Verificación Completa (todos los compiladores)
```bash
scripts/test_extracted.bash all
```

### Solo Compilar (sin ejecutar)
```bash
scripts/build_extracted_tests.bash all
```

### Solo Ejecutar (sin recompilar)
```bash
scripts/run_uint128_extracted_tests.bash gcc
```

---

## Códigos de Salida

- `0`: Éxito (todos los tests pasaron)
- `1`: Fallo (al menos un test falló o error de compilación)

---

## Salida Visual

Los scripts usan colores para facilitar la lectura:
- 🟢 Verde: Éxito
- 🔴 Rojo: Error
- 🟡 Amarillo: Advertencia
- 🔵 Cyan: Información

---

## Notas

1. Los scripts están ubicados en `scripts/` pero se ejecutan desde el directorio raíz del proyecto
2. Los scripts automáticamente navegan al directorio raíz, así que pueden ejecutarse desde cualquier ubicación
3. Los ejecutables se colocan en subdirectorios separados por compilador
4. El argumento `all` es el valor por defecto si no se especifica ninguno
5. Si un compilador no está disponible, se mostrará una advertencia pero los otros continuarán
6. Para MSVC, el script intentará activar automáticamente el entorno de Visual Studio

---

## Estado de Pruebas

✅ **Todos los compiladores funcionan correctamente:**
- GCC: Compilación y ejecución exitosas (84/84 tests pasan)
- Clang: Compilación y ejecución exitosas (84/84 tests pasan, 1 warning esperado en self-assignment)
- MSVC: Compilación y ejecución exitosas (con activación automática desde bash MSYS64/UCRT64)

## Cobertura de Tests

### Tests Originales (66)
- Constructores y asignaciones
- Operadores aritméticos y bitwise
- Operadores de comparación
- División y resto (divrem)
- Conversiones de/a string
- Funciones auxiliares (leading_zeros, trailing_zeros, etc.)

### Tests Adicionales (18)
1. **Semántica de Copia/Movimiento** (5 tests)
   - Copy/move constructors
   - Copy/move assignment
   - Compatibilidad con `std::vector`

2. **Literales Definidos por Usuario** (3 tests)
   - Literales enteros: `1000_u128`
   - Literales de string: `"12345"_u128`
   - Formatos específicos: `_u128_hex`, `_u128_bin`, `_u128_oct`

3. **Operadores de Flujo** (2 tests)
   - Output stream: `operator<<`
   - Input stream: `operator>>`

4. **Casos Extremos y Límites** (4 tests)
   - Overflow behavior (wrap-around)
   - Underflow behavior
   - Valores límite (0, max)
   - División por cero

5. **Constexpr y Type Traits** (2 tests)
   - Operaciones constexpr
   - Validación de type traits

6. **Casos Extremos Adicionales** (2 tests)
   - Desplazamientos extremos (64, 128+ bits)
   - Operaciones con tipos mixtos

**Total: 84 tests** cubriendo todas las funcionalidades críticas de `uint128_t`.
