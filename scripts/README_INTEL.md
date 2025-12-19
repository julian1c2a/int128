# Scripts - Guía de Uso

## Compiladores Soportados

Los scripts de construcción y testing soportan 4 compiladores:
- **GCC** (GNU Compiler Collection)
- **Clang** (LLVM)
- **MSVC** (Microsoft Visual C++)
- **Intel** (Intel oneAPI)

## Configuración de Intel oneAPI

### ⚠️ IMPORTANTE: Configuración Recomendada

La forma **más confiable** de configurar Intel oneAPI es desde CMD de Windows:

**Paso a paso:**
1. Abrir **Símbolo del sistema (CMD)** de Windows
2. Ejecutar el script de configuración:
   ```cmd
   "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"
   ```
3. Desde ese mismo CMD, abrir bash:
   ```cmd
   bash
   ```
4. Ahora puede usar los scripts con el compilador Intel:
   ```bash
   ./scripts/build_extracted_tests.bash intel
   ```

**¿Por qué este método?**
- ✅ Configura **todas** las variables de entorno necesarias
- ✅ Evita conflictos de PATH
- ✅ Funciona de forma confiable
- ✅ Bash hereda el entorno completo de CMD

### Windows con MSYS2/Git Bash (Alternativa)

Si prefiere configurar desde MSYS2/Git Bash directamente (puede tener limitaciones):

```bash
# Opción 1: Helper script simplificado (solo añade al PATH)
source scripts/setup_intel_env.bash

# Opción 2: Ejecutar setvars.bat desde bash usando cmd
cmd.exe //c "\"C:\\Program Files (x86)\\Intel\\oneAPI\\setvars.bat\" && set" | grep -E "^(PATH|LIB|INCLUDE|INTEL)" | while IFS='=' read -r key value; do export "$key=$value"; done

# Opción 3: Configuración manual (más simple pero incompleto)
export PATH="/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin:$PATH"
export PATH="/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin/intel64:$PATH"
```

**Nota:** 
- ⚠️ **No se puede ejecutar `.bat` directamente en bash:** Use `cmd.exe //c` para ejecutar archivos batch
- La configuración desde MSYS2/Git Bash solo añade los binarios al PATH, pero no configura todas las variables de entorno que setvars.bat establece
- Para un entorno completo, use **CMD + setvars.bat** (método recomendado)

### Windows con PowerShell

```powershell
# Dot-source el script de configuración
. .\scripts\setup_intel_env.ps1
```

### Scripts Python de Intel oneAPI

Se incluyen scripts Python similares a los de MSVC para automatizar la detección y configuración:

**setvars_intel.py** - Detección simple:
```bash
# Ver información de instalación
python scripts/setvars_intel.py

# Uso desde otros scripts Python
from setvars_intel import find_setvars_bat, get_compiler_paths
```

**setvarsall_intel.py** - Configuración completa:
```bash
# Configurar entorno en bash (source el output)
source <(python scripts/setvarsall_intel.py)

# Con argumentos específicos
python scripts/setvarsall_intel.py intel64

# Formato CMD
python scripts/setvarsall_intel.py --format=cmd

# Uso desde otros scripts Python
from setvarsall_intel import get_intel_environment, win_to_unix_path
```

**Funcionalidades:**
- ✓ Detección automática de Intel oneAPI en rutas estándar
- ✓ Extracción completa de variables de entorno (PATH, INCLUDE, LIB, INTEL_*, ONEAPI_*)
- ✓ Conversión de rutas Windows → MSYS2 (C:\\ → /c/)
- ✓ Generación de exports para bash o SET para CMD

### Ubicaciones de Intel oneAPI

- **Instalación:** `C:\Program Files (x86)\Intel\oneAPI`
- **setvars.bat:** `C:\Program Files (x86)\Intel\oneAPI\setvars.bat`
- **Compiladores:**
  - icpx (oneAPI): `C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\icpx.exe`
  - icx (legacy): `C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\icx.exe`

## Uso de Scripts

### Build Scripts

Compilar tests:
```bash
./scripts/build_extracted_tests.bash [gcc|clang|msvc|intel|all]
./scripts/build_additional_tests.bash [gcc|clang|msvc|intel|all]
```

Compilar benchmarks:
```bash
./scripts/build_benchmarks.bash [gcc|clang|msvc|intel|all]
```

Compilar con backends opcionales:
```bash
./scripts/build_with_backends.bash [gcc|clang|msvc|intel] [options]
```

### Run Scripts

Ejecutar tests:
```bash
./scripts/run_uint128_extracted_tests.bash [gcc|clang|msvc|intel|all]
```

Ejecutar benchmarks:
```bash
./scripts/run_benchmarks.bash [gcc|clang|msvc|intel|all]
```

### Test Scripts (Build + Run)

```bash
./scripts/test_extracted.bash [gcc|clang|msvc|intel|all]
```

## Ejemplos con Intel

```bash
# 1. Configurar entorno (solo una vez por sesión)
source scripts/setup_intel_env.bash

# 2. Compilar con Intel
./scripts/build_extracted_tests.bash intel

# 3. Ejecutar tests
./scripts/run_uint128_extracted_tests.bash intel

# 4. Todo en uno
./scripts/test_extracted.bash intel

# 5. Compilar y ejecutar con todos los compiladores
./scripts/test_extracted.bash all
```

## Estructura de Salida

Los ejecutables se organizan por compilador:

```
build/
├── build_tests/
│   ├── gcc/release/
│   ├── clang/release/
│   ├── msvc/release/
│   └── intel/release/      ← Ejecutables de Intel
├── build_benchmarks/
│   ├── gcc/release/
│   ├── clang/release/
│   ├── msvc/release/
│   └── intel/release/      ← Benchmarks de Intel
└── build_demos/
    ├── gcc/release/
    ├── clang/release/
    ├── msvc/release/
    └── intel/release/      ← Demos de Intel
```

## Troubleshooting

### Intel Compiler no encontrado

**Error:** `Intel C++ compiler (icpx/icx) not found`

**Solución:**
1. Verificar instalación: `dir "C:\Program Files (x86)\Intel\oneAPI"`
2. Ejecutar setvars.bat desde CMD: `"C:\Program Files (x86)\Intel\oneAPI\setvars.bat"`
3. Verificar PATH: `where icpx` o `where icx`

### No se puede ejecutar .bat desde Bash

**Error:** `bash: error de sintaxis cerca del elemento inesperado '('`

**Problema:** Intentar ejecutar directamente un archivo `.bat` desde bash:
```bash
# ❌ NO FUNCIONA - bash no puede ejecutar .bat directamente
/c/Program Files (x86)/Intel/oneAPI/setvars.bat
```

**Solución:** Use uno de estos métodos:
```bash
# ✓ Opción 1: Usar cmd.exe desde bash
cmd.exe //c "\"C:\\Program Files (x86)\\Intel\\oneAPI\\setvars.bat\""

# ✓ Opción 2: Usar el helper script (más fácil)
source scripts/setup_intel_env.bash

# ✓ Opción 3: Método recomendado - Abrir CMD primero
# En CMD: "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"
# Luego en CMD: bash
```

### Comandos ejecutan programa incorrecto (ej: PHONON)

**Problema:** Al ejecutar `icpx --version` o similar, se ejecuta un programa no relacionado como Quantum ESPRESSO PHONON.

**Causa:** Conflicto en el PATH - otro programa con el mismo nombre está ejecutándose primero.

**Soluciones:**
1. **Limpiar PATH:** Reinicie la terminal para limpiar el PATH
2. **Usar ruta completa:**
   ```bash
   "/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin/icpx.exe" --version
   ```
3. **Verificar aliases:** `alias | grep icpx`
4. **Usar CMD con setvars.bat** (recomendado) en lugar de MSYS2/Git Bash

### Variables de entorno no persisten

En Git Bash/MSYS2, las variables configuradas con setvars.bat pueden no persistir correctamente. 

**Solución recomendada:**
1. Abrir CMD de Windows
2. Ejecutar: `"C:\Program Files (x86)\Intel\oneAPI\setvars.bat"`
3. Desde ese mismo CMD, ejecutar: `bash`
4. Ahora bash heredará todas las variables de entorno correctamente

### Conflictos con otros compiladores

Si tiene múltiples versiones de compiladores, asegúrese de que las variables de entorno estén configuradas correctamente para cada sesión.

**Verificación del PATH:**
```bash
# Ver qué icpx se está usando
which icpx

# Ver todos los icpx en PATH
where.exe icpx  # En Windows/MSYS2

# Ver el PATH completo
echo $PATH | tr ':' '\n' | grep -i intel
```
