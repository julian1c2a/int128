@echo off
REM Script PowerShell para compilar tests y benchmarks de uint128_traits con los 4 compiladores
REM Compiladores: g++ (ucrt64), clang (clang64), Intel OneAPI, MSVC

setlocal enabledelayedexpansion

set PROJECT_DIR=C:\msys64\ucrt64\home\julian\CppProjects\int128
cd /d "%PROJECT_DIR%"

echo ============================================================
echo   Compilacion de uint128_traits tests y benchmarks
echo ============================================================

REM Crear directorios de salida si no existen
if not exist build\build_tests\gcc\release mkdir build\build_tests\gcc\release
if not exist build\build_benchmarks\gcc\release mkdir build\build_benchmarks\gcc\release
if not exist build\build_tests\clang\release mkdir build\build_tests\clang\release
if not exist build\build_benchmarks\clang\release mkdir build\build_benchmarks\clang\release
if not exist build\build_tests\intel\release mkdir build\build_tests\intel\release
if not exist build\build_benchmarks\intel\release mkdir build\build_benchmarks\intel\release
if not exist build\build_tests\msvc\release mkdir build\build_tests\msvc\release
if not exist build\build_benchmarks\msvc\release mkdir build\build_benchmarks\msvc\release

REM =============================================================================
REM COMPILADOR 1: G++ (UCRT64)
REM =============================================================================
echo.
echo [1/4] Compilando con G++ (ucrt64)...

set GCC_BIN=C:\msys64\ucrt64\bin
set GCC_INCLUDE=C:\msys64\ucrt64\include
set GCC_LIB=C:\msys64\ucrt64\lib

echo   - Compilando tests...
"%GCC_BIN%\g++.exe" -std=c++20 -O3 -march=native -Wall -Wextra ^
    -I"%PROJECT_DIR%\include" ^
    -I"%GCC_INCLUDE%" ^
    -L"%GCC_LIB%" ^
    tests\uint128_traits_extracted_tests.cpp ^
    -o build\build_tests\gcc\release\uint128_traits_tests_gcc.exe

echo   - Compilando benchmarks...
"%GCC_BIN%\g++.exe" -std=c++20 -O3 -march=native -Wall -Wextra ^
    -I"%PROJECT_DIR%\include" ^
    -I"%GCC_INCLUDE%" ^
    -L"%GCC_LIB%" ^
    benchmarks\uint128_traits_extracted_benchs.cpp ^
    -o build\build_benchmarks\gcc\release\uint128_traits_benchs_gcc.exe

echo   [OK] G++ compilacion completada

REM =============================================================================
REM COMPILADOR 2: CLANG (CLANG64)
REM =============================================================================
echo.
echo [2/4] Compilando con Clang (clang64)...

set CLANG_BIN=C:\msys64\clang64\bin
set CLANG_INCLUDE=C:\msys64\clang64\include
set CLANG_LIB=C:\msys64\clang64\lib

echo   - Compilando tests...
"%CLANG_BIN%\clang++.exe" -std=c++20 -O3 -march=native -Wall -Wextra ^
    -I"%PROJECT_DIR%\include" ^
    -I"%CLANG_INCLUDE%" ^
    -L"%CLANG_LIB%" ^
    tests\uint128_traits_extracted_tests.cpp ^
    -o build\build_tests\clang\release\uint128_traits_tests_clang.exe

echo   - Compilando benchmarks...
"%CLANG_BIN%\clang++.exe" -std=c++20 -O3 -march=native -Wall -Wextra ^
    -I"%PROJECT_DIR%\include" ^
    -I"%CLANG_INCLUDE%" ^
    -L"%CLANG_LIB%" ^
    benchmarks\uint128_traits_extracted_benchs.cpp ^
    -o build\build_benchmarks\clang\release\uint128_traits_benchs_clang.exe

echo   [OK] Clang compilacion completada

REM =============================================================================
REM COMPILADOR 3: INTEL ONEAPI
REM =============================================================================
echo.
echo [3/4] Compilando con Intel OneAPI...

set INTEL_ONEAPI=C:\Program Files (x86)\Intel\oneAPI\setvars.bat

if exist "%INTEL_ONEAPI%" (
    call "%INTEL_ONEAPI%" intel64 vs2022 > nul
    
    echo   - Compilando tests...
    icx -std=c++20 -O3 -xHost -Wall ^
        -I"%PROJECT_DIR%\include" ^
        tests\uint128_traits_extracted_tests.cpp ^
        -o build\build_tests\intel\release\uint128_traits_tests_intel.exe
    
    echo   - Compilando benchmarks...
    icx -std=c++20 -O3 -xHost -Wall ^
        -I"%PROJECT_DIR%\include" ^
        benchmarks\uint128_traits_extracted_benchs.cpp ^
        -o build\build_benchmarks\intel\release\uint128_traits_benchs_intel.exe
    
    echo   [OK] Intel OneAPI compilacion completada
) else (
    echo   [WARNING] Intel OneAPI no encontrado, saltando...
)

REM =============================================================================
REM COMPILADOR 4: MSVC
REM =============================================================================
echo.
echo [4/4] Compilando con MSVC...

set VCVARSALL=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat
if not exist "%VCVARSALL%" (
    set VCVARSALL=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat
)

if exist "%VCVARSALL%" (
    call "%VCVARSALL%" x64 > nul
    
    echo   - Compilando tests...
    cl /std:c++20 /O2 /EHsc /W4 ^
        /I"%PROJECT_DIR%\include" ^
        tests\uint128_traits_extracted_tests.cpp ^
        /Fe:build\build_tests\msvc\release\uint128_traits_tests_msvc.exe ^
        /link /MACHINE:X64 > nul
    
    echo   - Compilando benchmarks...
    cl /std:c++20 /O2 /EHsc /W4 ^
        /I"%PROJECT_DIR%\include" ^
        benchmarks\uint128_traits_extracted_benchs.cpp ^
        /Fe:build\build_benchmarks\msvc\release\uint128_traits_benchs_msvc.exe ^
        /link /MACHINE:X64 > nul
    
    echo   [OK] MSVC compilacion completada
) else (
    echo   [WARNING] MSVC no encontrado, saltando...
)

REM =============================================================================
REM RESUMEN
REM =============================================================================
echo.
echo ============================================================
echo                     RESUMEN DE COMPILACION
echo ============================================================
echo.
echo Ejecutables generados:
echo.
echo Tests:
dir /b build\build_tests\*\release\*.exe 2>nul
echo.
echo Benchmarks:
dir /b build\build_benchmarks\*\release\*.exe 2>nul

echo.
echo Para ejecutar los tests:
echo   build\build_tests\gcc\release\uint128_traits_tests_gcc.exe
echo   build\build_tests\clang\release\uint128_traits_tests_clang.exe
echo   build\build_tests\intel\release\uint128_traits_tests_intel.exe
echo   build\build_tests\msvc\release\uint128_traits_tests_msvc.exe
echo.
echo Para ejecutar los benchmarks:
echo   build\build_benchmarks\gcc\release\uint128_traits_benchs_gcc.exe
echo   build\build_benchmarks\clang\release\uint128_traits_benchs_clang.exe
echo   build\build_benchmarks\intel\release\uint128_traits_benchs_intel.exe
echo   build\build_benchmarks\msvc\release\uint128_traits_benchs_msvc.exe
echo.
echo ============================================================

pause
