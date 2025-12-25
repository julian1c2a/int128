# Scripts Genéricos del Proyecto

## Problema Resuelto

Anteriormente teníamos **128 scripts individuales** con código 98% duplicado:
- **58 scripts de compilación** (build_*_extracted_{tests|benchs}.bash)
- **42 scripts de ejecución** (run_*_extracted_benchs.bash)
- **28 scripts de testing** (check_*_extracted_tests.bash)

Solo cambiaban 2-4 líneas por script (TYPE, FEATURE, archivo fuente).

## Solución Implementada

Se han creado **4 scripts genéricos** que reemplazan todos los scripts individuales:

### Scripts de Compilación
1. **`build_generic.bash`** - Script bash de compilación (recomendado)
2. **`build_generic.py`** - Script Python de compilación (alternativa)

### Scripts de Ejecución
3. **`check_generic.bash`** - Script bash para ejecutar tests con matriz de resultados
4. **`run_generic.bash`** - Script bash para ejecutar benchmarks

### Ventajas

✅ **Mantenimiento**: Cambios en 4 archivos en vez de 128 scripts  
✅ **Consistencia**: Imposible que se desajusten las rutas  
✅ **Reducción de código**: 96% menos (19,200 líneas → 800 líneas)  
✅ **Validación**: Comprueba archivos y parámetros antes de ejecutar  
✅ **Makefile actualizado**: Usa automáticamente los scripts genéricos

## Uso

### 1. Compilación (build)

```bash
# Bash
bash scripts/build_generic.bash <type> <feature> <target> [compiler] [mode] [print]

# Python
python scripts/build_generic.py <type> <feature> <target> [compiler] [mode] [print]
```

### 2. Testing (check)

```bash
bash scripts/check_generic.bash <type> <feature> [compiler] [mode]
```

### 3. Benchmarking (run)

```bash
bash scripts/run_generic.bash <type> <feature> [compiler] [mode]
```
 Completos

#### Compilación
```bash
# Compilar tests de uint128 bits con todos los compiladores
bash scripts/build_generic.bash uint128 bits tests

# Compilar benchmarks de int128 numeric solo con GCC en release
bash scripts/build_generic.bash int128 numeric benchs gcc release

# Compilar tests con todos los compiladores mostrando comandos
bash scripts/build_generic.bash uint128 algorithm tests all all yes

# Usando Python (alternativa)
python scripts/build_generic.py uint128 bits tests gcc release
```

#### Testing
```bash
# Ejecutar tests de uint128 bits con todos los compiladores
bash scripts/check_generic.bash uint128 bits

# Ejecutar tests solo con GCC en release
bash scripts/check_generic.bash uint128 numeric gcc release

# Ejecutar todos los tests de int128 cmath
bash scripts/check_generic.bash int128 cmath all all
```

#### Benchmarking
```bashlos scripts genéricos automáticamente:

```bash
# COMPILACIÓN
# Compila uint128 bits tests con GCC en release
make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release

# Compila int128 numeric benchs con todos los compiladores
make build_benchs TYPE=int128 FEATURE=numeric COMPILER=all MODE=all

# TESTING
# Ejecuta tests de uint128 bits con todos los compiladores
make check TYPE=uint128 FEATURE=bits

# Ejecuta tests solo con GCC en release
make check TYPE=uint128 FEATURE=numeric COMPILER=gcc MODE=release

# BENCHMARKING
# Ejecuta benchmarks de int128 algorithm
make run TYPE=int128 FEATURE=algorithm

# Ejecuta benchmarks solo con Clang en release
make run TYPE=uint128 FEATURE=numeric COMPILER=clang MODE=releaseric solo con GCC en release
bash scripts/build_generic.bash int128 numeric benchs gcc release

# Compilar tests con todos los compiladores mostrando comandos
bash scripts/build_generic.bash uint128 algorithm tests all all yes

# Usando Python (alternativa)
python scripts/build_generic.py uint128 bits tests gcc release
```

### Uso desde Makefile (RECOMENDADO)

El Makefile ya está configurado para usar el script genérico automáticamente:

```bash
# Compila uint128 bits tests con GCC en release
make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release

# Compila int128 numeric benchs con todos los compiladores
make build_benchs TYPE=int128 FEATURE=numeric COMPILER=all MODE=all

# Ejecuta tests
make check TYPE=uint128 FEATURE=bits

# Ejecuta benchmarks
make run TYPE=int128 FEATURE=numeric
```

## Estructura Generada

Los scripts genéricos mantienen la estructura estándar del proyecto:

```
build/
├── build_tests/
│   ├── gcc/
│   │   ├── debug/
│   │   │   └── uint128_bits_tests_gcc.exe
│   │   └── release/
│   │       └── uint128_bits_tests_gcc.exe
│   ├── clang/
│   ├── intel/
│   └── msvc/
└── build_benchs/
    ├── gcc/
    │   ├── debug/
    │   │   └── int128_numeric_benchs_gcc.exe
    │   └── release/
    │       └── int128_numeric_benchs_gcc.exe
    ├── clang/
    ├── intel/
    └── msvc/
```

## Convenciones de Nombres

Los scripts genéricos **siempre** generan nombres consistentes:

### Tests
```
{type}_{feature}_tests_{compiler}[.exe]
```
Ejemplos: `uint128_bits_tests_gcc.exe`, `int128_numeric_tests_clang`

### Benchmarks
```
{type}_{feature}_benchs_{compiler}[.exe]
```
Ejemplos: `uint128_algorithm_benchs_gcc.exe`, `int128_cmath_benchs_msvc.exe`

## Archivos Fuente Esperados

Los scripts buscan archivos en estas rutas:

**Tests:**
```
tests/{type}_{feature}_extracted_tests.cpp
```

**Benchmarks:**
```
benchs/{type}_{feature}_extracted_benchs.cpp
```128 scripts individuales están **deprecados** pero se mantienen por compatibilidad temporal:
- `build_uint128_bits_extracted_tests.bash` → `build_generic.bash uint128 bits tests`
- `check_uint128_bits_extracted_tests.bash` → `check_generic.bash uint128 bits`
- `run_uint128_bits_extracted_benchs.bash` → `run_generic.bash uint128 bits`

### Equivalencias

#### Compilación
```bash
# Antes
bash scripts/build_uint128_bits_extracted_tests.bash gcc release

# Ahora
bash scripts/build_generic.bash uint128 bits tests gcc release

# O mejor aún (con Makefile)
make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release
```

#### Testing
```bash
# Antes
bash scripts/execution/check_uint128_bits_extracted_tests.bash gcc release

# Ahora
bash scripts/check_generic.bash uint128 bits gcc release

# O mejor aún (con Makefile)
make check TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release
```

#### Benchmarking
```bash
# Antes
bash scripts/execution/run_uint128_bits_extracted_benchs.bash gcc release

# Ahora
bash scripts/run_generic.bash uint128 bits gcc release

# O mejor aún (con Makefile)
make run

### Release
```
-O3 -march=native -DNDEBUG
```

## Variables de Entorno

Puedes personalizar los comandos de los compiladores:

```bash
export GCC_CXX="g++-13"
export CLANG_CXX="clang++-17"
export INTEL_CXX="icpx"
export MSVC_CXX="cl.exe"
```

## Migración desde Scripts Individuales

### ⚠️ Scripts Individuales Deprecados

Los 58 scripts individuales (`build_uint128_bits_extracted_tests.bash`, etc.) están **deprecados** pero se mantienen por compatibilidad temporal.

### Equivalencias

```bash
# Antes
bash scripts/build_uint128_bits_extracted_tests.bash gcc release

# Ahora
bash scripts/build_generic.bash uint128 bits tests gcc release

# O mejor aún (con Makefile)
make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release
```

### Eliminación Futura

En una versión futura, los scripts individuales se moverán a `scripts/legacy/` para mantener el directorio principal limpio.

## Comparación Bash vs Python

| Característica | Bash | Python |
|----------------|------|--------|
| **Velocidad** | ⚡ Más rápido (nativo) | 🐢 Overhead inicial |
| **Portabilidad** | ✅ Unix/Linux/Mac/MSYS2 | ✅ Cualquier plataforma |
| **Mantenimiento** | ✅ Más simple | ✅ Más estructurado |
| **Debugging** | ⚠️ Menos herramientas | ✅ Excelentes herramientas |
| **Recomendación** | **Uso normal** | Desarrollo/debugging |

## Verificación

Para verificar que todo funciona:

```bash
# Test rápido con bash
bas~~**Scripts genéricos para check/run**~~ ✅ **COMPLETADO**
2. **Generación automática de CMakeLists.txt**
3. **Caché de compilación** para evitar recompilaciones
4. **Compilación paralela** de múltiples features
5. **Integración con ccache/sccache**
6. **Migración de scripts individuales a legacy/**

## Estadísticas Finales

### Reducción de Código
- **Antes**: 128 scripts × ~150 líneas = **19,200 líneas**
- **Ahora**: 4 scripts × ~200 líneas = **800 líneas**
- **Reducción**: **96% menos código duplicado**

### Archivos Genéricos Creados
1. `scripts/build_generic.bash` (230 líneas)
2. `scripts/build_generic.py` (240 líneas)
3. `scripts/check_generic.bash` (200 líneas)
4. `scripts/run_generic.bash` (170 líneas)

### Scripts Deprecados
- 58 scripts de compilación en `scripts/`
- 28 scripts de testing en `scripts/execution/`
- 42 scripts de benchmarking en `scripts/execution/`
# Test completo con Makefile
make build_tests TYPE=uint128 FEATURE=bits COMPILER=all MODE=all
```

## Solución de Problemas

### Error: "Source file not found"

Verifica que el archivo existe en la ruta esperada:
```bash
ls -la tests/uint128_bits_extracted_tests.cpp
ls -la benchs/int128_numeric_extracted_benchs.cpp
```

### Error: "Compiler not found"

Instala el compilador o configura la variable de entorno:
```bash
export CLANG_CXX="clang++-17"
```

### Error de compilación

Revisa los errores específicos del código fuente. Usa `PRINT=yes` para ver el comando exacto:
```bash
make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release PRINT=yes
```

## Futuras Mejoras

Posibles extensiones del sistema:

1. **Scripts genéricos para check/run** (execution/)
2. **Generación automática de CMakeLists.txt**
3. **Caché de compilación** para evitar recompilaciones
4. **Compilación paralela** de múltiples features
5. **Integración con ccache/sccache**

## Referencias

- **Especificación**: Ver `PROMPT.md` para nomenclatura completa
- **Makefile**: Ver `Makefile` para targets disponibles
- **Scripts legacy**: Ver `scripts/legacy/` para scripts antiguos
