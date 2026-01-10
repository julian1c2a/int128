# Entorno de Desarrollo - Variables y Dependencias

Este documento describe todas las dependencias y variables de entorno necesarias para compilar el proyecto con los 4 compiladores: GCC, Clang, MSVC e Intel oneAPI.

---

## 📋 Tabla de Contenidos

1. [Resumen de Compiladores](#resumen-de-compiladores)
2. [Script de Configuración Intel oneAPI](#script-de-configuración-intel-oneapi)
3. [Dependencias del Sistema](#dependencias-del-sistema)
4. [Variables de Entorno Críticas](#variables-de-entorno-críticas)
5. [Troubleshooting](#troubleshooting)

---

## Resumen de Compiladores

| Compilador | Comando | Ubicación | Notas |
|------------|---------|-----------|-------|
| **GCC** | `g++` | `/c/msys64/ucrt64/bin/` | Includes y libs en `/c/msys64/ucrt64/` |
| **Clang** | `clang++` | `/c/msys64/clang64/bin/` | Includes y libs en `/c/msys64/clang64/` |
| **Intel** | `icx`/`icpx` | Intel oneAPI | Requiere configuración especial |
| **MSVC** | `cl` | Visual Studio | Requiere vcvarsall.bat |

---

## Script de Configuración Intel oneAPI

### 🎯 Propósito

El script `scripts/setup_intel_combined.bash` configura un entorno **híbrido crítico**:

- **Compilador**: Intel oneAPI (icx/icpx)
- **STL/Headers**: Visual Studio MSVC
- **Shell**: MSYS2 Bash

Este entorno cruzado es necesario porque Intel oneAPI en Windows utiliza las bibliotecas estándar de MSVC.

### 📂 Archivos Principales

```
int128/
├── scripts/
│   ├── setup_intel_combined.bash    ← Script principal (Bash)
│   └── intel_debug.log              ← Log de errores (generado)
└── get_combined_env.py              ← Generador de variables (Python)
```

### 🔄 Flujo de Ejecución

```
┌─────────────────────────────────────────────────┐
│ 1. BASH: setup_intel_combined.bash             │
│    - Verifica Python                            │
│    - Verifica get_combined_env.py               │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│ 2. PYTHON: get_combined_env.py                  │
│    - Busca Visual Studio (vcvarsall.bat)        │
│    - Busca Intel oneAPI (setvars.bat)           │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│ 3. CMD.EXE: Ejecución batch                     │
│    call vcvarsall.bat x64                       │
│    call setvars.bat intel64                     │
│    set                                          │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│ 4. PYTHON: Conversión de entorno                │
│    - Extrae variables críticas                  │
│    - Convierte PATH a formato UNIX              │
│    - Genera exports para bash                   │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│ 5. BASH: Carga variables                        │
│    source <temp_file>                           │
│    export CC=icx                                │
│    export CXX=icx                               │
└─────────────────────────────────────────────────┘
```

---

## Dependencias del Sistema

### 🔴 DEPENDENCIAS OBLIGATORIAS

#### A) Python 3.x

- **Comando**: `python3` o `python`
- **Propósito**: Ejecutar `get_combined_env.py`
- **Ubicación**: Debe estar en PATH del sistema o MSYS2
- **Verificación**:

  ```bash
  python3 --version
  # o
  python --version
  ```

#### B) Visual Studio 2026 (versión 18)

- **Componente requerido**: `Microsoft.VisualStudio.Component.VC.Tools.x86.x64`
- **Archivo crítico**: `vcvarsall.bat`
- **Ubicaciones buscadas** (en orden de prioridad):
  1. Detectado por `vswhere.exe`:
     - `C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe`
  2. Rutas manuales (VS 2026 = versión 18):
     - `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`
     - `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`
  3. Rutas legacy (VS 2022/2019, si 18 no existe):
     - `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat`
     - `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat`

#### C) Intel oneAPI

- **Componente requerido**: Intel C++ Compiler (icx/icpx)
- **Archivo crítico**: `setvars.bat`
- **Ubicaciones buscadas** (en orden):
  1. Variable de entorno `ONEAPI_ROOT` (si existe):
     - `%ONEAPI_ROOT%\setvars.bat`
  2. Rutas manuales:
     - `C:\Program Files (x86)\Intel\oneAPI\setvars.bat`
     - `D:\Program Files (x86)\Intel\oneAPI\setvars.bat`
     - `C:\Program Files\Intel\oneAPI\setvars.bat`
     - `D:\Program Files\Intel\oneAPI\setvars.bat`

#### D) MSYS2 Bash

- **Comandos requeridos**:
  - `bash` - Shell principal
  - `mktemp` - Creación de archivos temporales
  - `source` - Carga de variables de entorno
  - `command` - Verificación de comandos disponibles
  - `which` - Localización de ejecutables
  - `cat` - Visualización de logs
  - `rm` - Limpieza de archivos temporales

### ⚪ DEPENDENCIAS OPCIONALES

#### vswhere.exe

- **Ubicación**: `C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe`
- **Propósito**: Detectar automáticamente la instalación de Visual Studio
- **Fallback**: Si no existe, se buscan rutas manuales

---

## Variables de Entorno Críticas

### 🔧 Variables Generadas por `get_combined_env.py`

#### Variables Principales

| Variable | Contenido | Propósito |
|----------|-----------|-----------|
| **PATH** | Rutas convertidas a formato UNIX | Localizar ejecutables (icx, cl, link, etc.) |
| **INCLUDE** | Rutas de headers de MSVC | Archivos de cabecera C++ estándar |
| **LIB** | Rutas de bibliotecas MSVC | Archivos .lib para linking |
| **LIBPATH** | Rutas adicionales de bibliotecas | Bibliotecas .NET y Windows Runtime |

#### Variables con Patrones Específicos

El script exporta automáticamente todas las variables que contengan:

- `INTEL` - Variables de Intel oneAPI
- `ONEAPI` - Variables de configuración oneAPI
- `VC` - Variables de Visual C++
- `SDK` - Variables del Windows SDK

#### Variables de Compilador

```bash
export CC=icx
export CXX=icx
```

### 📝 Ejemplo de Variables Exportadas

```bash
# PATH (convertido a formato UNIX)
export PATH="/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin:/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.xx.xxxxx/bin/Hostx64/x64:$PATH"

# Headers de MSVC
export INCLUDE="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.xx.xxxxx\include;C:\Program Files (x86)\Windows Kits\10\Include\10.0.xxxxx.0\ucrt;..."

# Bibliotecas
export LIB="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.xx.xxxxx\lib\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.xxxxx.0\ucrt\x64;..."

# Compiladores
export CC=icx
export CXX=icx
```

### 🎯 Conversión de PATH (Windows → UNIX)

El script Python realiza la siguiente conversión:

```python
# Entrada (Windows):
C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin

# Salida (UNIX/MSYS2):
/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin
```

**Reglas de conversión:**

1. `\` → `/` (separadores de ruta)
2. `C:` → `/c` (letra de unidad)
3. `;` → `:` (separador de PATH)

---

## Orden de Carga de Entorno

### ⚠️ ORDEN CRÍTICO

El orden de carga es **fundamental** para el correcto funcionamiento:

```cmd
call "vcvarsall.bat" x64 && call "setvars.bat" intel64 && set
```

**Secuencia:**

1. **PRIMERO**: `vcvarsall.bat x64`
   - Configura Visual Studio para x64
   - Establece PATH, INCLUDE, LIB para MSVC
   - Configura Windows SDK

2. **SEGUNDO**: `setvars.bat intel64`
   - Configura Intel oneAPI
   - **AÑADE** rutas de Intel al PATH (no las reemplaza)
   - Configura variables INTEL_*y ONEAPI_*

3. **TERCERO**: `set`
   - Exporta todas las variables de entorno actuales
   - Capturado por el script Python

**Por qué este orden:**

- Intel oneAPI en Windows depende de las bibliotecas estándar de MSVC
- Si se invierte el orden, Intel no encuentra los headers/libs de MSVC
- El PATH debe incluir primero Intel y luego MSVC para que `icx` tenga prioridad

---

## Uso del Script

### Carga del Entorno Intel

```bash
# Desde terminal MSYS2/Bash en la raíz del proyecto
source scripts/setup_intel_combined.bash x64
```

### Verificación de Éxito

Después de cargar el script, deberías ver:

```
---------------------------------------------------------
   Iniciando configuración de entorno Intel oneAPI + MSVC
---------------------------------------------------------
🔎 Generando variables de entorno...
---------------------------------------------------------
✅ ÉXITO: Entorno cargado.
   Compilador: /c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin/icx
   MSVC Headers: DETECTADOS
---------------------------------------------------------
```

### Verificación Manual

```bash
# Verificar que icx está disponible
which icx
# Salida esperada: /c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin/icx

# Verificar que puede compilar
icx --version
# Salida esperada: Intel(R) oneAPI DPC++/C++ Compiler ...

# Verificar variables de entorno críticas
echo $INCLUDE | grep "Visual Studio"
echo $LIB | grep "Visual Studio"
```

---

## Troubleshooting

### ❌ Error: "get_combined_env.py no encontrado"

**Causa**: El archivo Python no está en `scripts/`

**Solución**:

```bash
# Verificar ubicación del archivo
ls -la get_combined_env.py
# Debe estar en la raíz del proyecto

# Si está en otro lugar, moverlo
mv path/to/get_combined_env.py ./
```

### ❌ Error: "Python no encontrado"

**Causa**: Python no está instalado o no está en PATH

**Solución**:

```bash
# En MSYS2, instalar Python
pacman -S python3

# O añadir Python de Windows al PATH
export PATH="/c/Python312:$PATH"
```

### ❌ Error: "No se encontró Visual Studio"

**Causa**: Visual Studio no está instalado o está en una ubicación no estándar

**Solución**:

1. Instalar Visual Studio 2019 o 2022 con componente C++
2. Si está en ubicación personalizada, editar `MSVC_CANDIDATES` en `get_combined_env.py`

```python
MSVC_CANDIDATES = [
    r"TU_RUTA_PERSONALIZADA\VC\Auxiliary\Build\vcvarsall.bat",
    # ... otras rutas
]
```

### ❌ Error: "No se encontró Intel oneAPI"

**Causa**: Intel oneAPI no está instalado

**Solución**:

1. Descargar e instalar [Intel oneAPI Base Toolkit](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html)
2. Instalar también [Intel oneAPI HPC Toolkit](https://www.intel.com/content/www/us/en/developer/tools/oneapi/hpc-toolkit.html) para icx/icpx

### ❌ Error: "'icx' no está en el PATH"

**Causa**: El entorno no se cargó correctamente

**Solución**:

```bash
# Revisar el log de errores
cat scripts/intel_debug.log

# Verificar manualmente las rutas
ls "/c/Program Files (x86)/Intel/oneAPI/"

# Intentar cargar setvars manualmente
source "/c/Program Files (x86)/Intel/oneAPI/setvars.sh" intel64
```

### ⚠️ Warning: "Headers de MSVC no detectados"

**Causa**: La variable `INCLUDE` no contiene rutas de Visual Studio

**Diagnóstico**:

```bash
# Verificar contenido de INCLUDE
echo $INCLUDE

# Debería contener algo como:
# C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\...
```

**Solución**:

- Verificar que Visual Studio está correctamente instalado
- Ejecutar `vcvarsall.bat` manualmente y verificar la salida
- Revisar el log: `cat scripts/intel_debug.log`

---

## Compilación con Intel

Una vez cargado el entorno, puedes compilar con Intel:

```bash
# Compilar tests
scripts/build_int128_concepts_extracted_tests.bash intel debug

# Compilar benchmarks
scripts/build_int128_concepts_extracted_benchs.bash intel release

# Ejecutar tests
scripts/check_int128_concepts_extracted_tests.bash intel debug

# Ejecutar benchmarks
scripts/run_int128_concepts_extracted_benchs.bash intel release
```

---

## Resumen de Archivos

| Archivo | Tipo | Propósito |
|---------|------|-----------|
| `setup_intel_combined.bash` | Bash | Script principal de configuración |
| `get_combined_env.py` | Python | Generador de variables de entorno |
| `intel_debug.log` | Log | Errores de ejecución (si ocurren) |
| `vcvarsall.bat` | Batch | Configuración de Visual Studio |
| `setvars.bat` | Batch | Configuración de Intel oneAPI |
| `vswhere.exe` | Ejecutable | Detector de Visual Studio (opcional) |

---

## Referencias

- [Intel oneAPI Documentation](https://www.intel.com/content/www/us/en/docs/oneapi/programming-guide/current/overview.html)
- [MSVC Command-Line Tools](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line)
- [MSYS2 Documentation](https://www.msys2.org/)

---

**Última actualización**: 24 de diciembre de 2025
