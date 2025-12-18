# Scripts de Compilación y Ejecución - uint128_extracted_tests

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

## Configuración Especial para MSVC

Para compilar con MSVC, primero debes activar el entorno de Visual Studio:

```bash
source activate_msvc.bash
```

Luego puedes usar cualquiera de los scripts:

```bash
./test_extracted.bash msvc
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
