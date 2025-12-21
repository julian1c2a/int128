# Notas sobre Compilación con Intel oneAPI en Windows

## Problema Actual - ACTUALIZADO

La compilación con Intel oneAPI (`icpx`/`icx`) **versión 2025.3** con **Visual Studio 2025 (versión 18)** presenta conflictos **irresolvables**:

### Conflictos de Tipos en immintrin.h

```
error: conflicting types for '_mm_div_epi8'
error: conflicting types for '_mm_rem_epi8'
...
```

**Causa**: Intel oneAPI 2025.3 tiene builtins para funciones SIMD de división/resto (`_mm_div_epi*`, `_mm_rem_epi*`) que conflictúan con las declaraciones en `immintrin.h` de Visual Studio 2025.

## Diagnóstico Completo

### Verificaciones Realizadas

1. ✅ Intel oneAPI instalado correctamente
2. ✅ Variables de entorno configuradas (`PATH`, `INCLUDE`, `LIB`)
3. ❌ `setvars-vcvarsall.bat` **NO soporta Visual Studio 2025**
   - Solo busca VS 2019 y VS 2022
   - Código en setvars-vcvarsall.bat:
     ```bat
     if /i [%1] == [vs2019]   (set TARGET_VS=vs2019)
     if /i [%1] == [vs2022]   (set TARGET_VS=vs2022)
     ```
4. ❌ Al usar rutas manuales de VS 2025, hay conflictos de builtins

### Estado del Entorno

- **Intel oneAPI**: 2025.3.0 (2025.3.0.20251010)
- **Visual Studio**: 18 (2025) Community
- **Windows SDK**: 10.0.26100.0
- **Target**: x86_64-pc-windows-msvc
- **MSYS2 Environment**: UCRT64

### Comportamiento de setvars

```bash
$ python3 scripts/setvarsall_intel.py
Usando setvars-vcvarsall.bat (incluye Visual Studio)...
setvars-vcvarsall.bat falló, intentando con setvars.bat normal...
Usando setvars.bat (solo Intel)...
```

Resultado: Solo se cargan variables de Intel, **SIN** las variables de Visual Studio (INCLUDE y LIB no contienen rutas de MSVC).

## Incompatibilidad de Entornos

### Problema Fundamental

1. **Intel oneAPI en Windows** está diseñado para usarse con el runtime de MSVC
2. **MSYS2** usa el runtime de MinGW-w64
3. Estos runtimes son **fundamentalmente incompatibles**:
   - MSVC usa `vcruntime_exception.h`, `__declspec(...)`
   - MinGW usa `__MINGW_ATTRIB_*`, diferentes ABIs

### Intentos Realizados

#### 1. Usar headers de MSYS2 Clang64
```bash
-I"/c/msys64/clang64/include/c++/v1"
```
**Resultado**: Error - Intel busca `vcruntime_exception.h` (MSVC), pero Clang64 tiene MinGW runtime

#### 2. Usar headers de Visual Studio
```bash
-I"C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/include"
-I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt"
```
**Resultado**: Los headers se encuentran, pero `immintrin.h` declara funciones que Intel ya tiene como builtins

#### 3. Excluir intrin.h/immintrin.h
```bash
-D__INTRIN_H_ -D_INTRIN_H_INCLUDED
```
**Resultado**: El código usa intrínsecos (`_addcarry_u64`, `_subborrow_u64`, `__umulh`) que requieren intrin.h

#### 4. Ignorar warnings de conflictos
```bash
-Wno-ignored-attributes -Wno-implicit-function-declaration
```
**Resultado**: Los conflictos son **errores** (conflicting types), no warnings

## Soluciones Posibles

### Opción 1: Downgrade a Visual Studio 2022 (RECOMENDADO)

Intel oneAPI 2025.3 **oficialmente soporta** Visual Studio 2022. Instalar VS 2022 Community resolvería el problema:

1. Instalar [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/older-downloads/)
2. `setvars-vcvarsall.bat` detectará VS 2022 automáticamente
3. No habrá conflictos de builtins (versiones compatibles)

### Opción 2: Actualizar setvars-vcvarsall.bat para VS 2025

Modificar el script de Intel para agregar soporte de VS 2025:

```bat
rem En C:\Program Files (x86)\Intel\oneAPI\setvars-vcvarsall.bat
rem Agregar esta línea después de vs2022:
if /i [%1] == [vs2025]   (set TARGET_VS=vs2025)
```

⚠️ **Advertencia**: Aún puede haber conflictos de builtins si Intel 2025.3 no fue diseñado para VS 2025.

### Opción 3: Esperar actualización de Intel oneAPI

Intel lanzará versiones futuras que soporten VS 2025 oficialmente.

### Opción 4: Usar GCC o Clang desde MSYS2 (ACTUAL)

**Estado**: ✅ **100% funcional y COMPLETADO**

Ambos compiladores funcionan perfectamente desde MSYS2:

```bash
# GCC (UCRT64)
./scripts/compile_int128_traits_extracted.sh gcc
# Resultado: ✅ 100% exitoso

# Clang (CLANG64) con libc++
./scripts/compile_int128_traits_extracted.sh clang
# Resultado: ✅ 100% exitoso (objetivo principal del proyecto)
```

## Conclusión y Recomendación

**Para desarrollo inmediato**:

```cmd
:: Abrir Developer Command Prompt for VS 2022 (o tu versión)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

:: Luego activar Intel
"C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

:: Compilar
icpx -std=c++20 -Iinclude -O3 tests\int128_traits_extracted_tests.cpp -o test.exe
```

### Alternativas desde MSYS2

Si necesitas compilar desde MSYS2, usa:

1. **GCC (UCRT64)**: ✅ Funciona perfectamente
   ```bash
   ./scripts/compile_int128_traits_extracted.sh gcc
   ```

2. **Clang (CLANG64)**: ✅ Funciona perfectamente con libc++
   ```bash
   ./scripts/compile_int128_traits_extracted.sh clang
   ```

3. **MSVC (cl.exe)**: ⚠️ Requiere Developer Command Prompt pero puede funcionar

## Estado del Proyecto

- ✅ **GCC**: Compilación 100% exitosa
- ✅ **Clang**: Compilación 100% exitosa (objetivo principal completado)
- ❌ **Intel desde MSYS2**: Bloqueado por incompatibilidad de entornos
- ⚠️ **Intel desde VS Dev Prompt**: Debería funcionar (no probado desde ese entorno)

## Conclusión

La limitación de Intel oneAPI en Windows no impacta el objetivo principal del proyecto (compatibilidad con Clang/libc++), que ya está **100% completo**.

Para validación adicional con Intel, se requiere:
- Usar Developer Command Prompt for Visual Studio
- O ejecutar desde un entorno que ya tenga el ambiente de VS configurado

## Información Técnica

- **Intel oneAPI Version**: 2025.3.0
- **Visual Studio Version**: 18 (2025)
- **Windows SDK**: 10.0.26100.0
- **MSYS2 Environment**: UCRT64
- **Target**: x86_64-pc-windows-msvc
