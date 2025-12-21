@echo off
REM Script de compilacion para int128_traits tests y benchmarks
REM Compila con 4 compiladores: GCC, Clang, Intel OneAPI, MSVC
REM Estructura: build\build_tests\[compiler]\[mode] y build\build_benchmarks\[compiler]\[mode]

setlocal enabledelayedexpansion

set PROJECT_DIR=C:\msys64\ucrt64\home\julian\CppProjects\int128
cd /d "%PROJECT_DIR%"

REM Definir rutas de compiladores
set GCC_PATH=C:\msys64\ucrt64\bin\g++.exe
set CLANG_PATH=C:\msys64\clang64\bin\clang++.exe
set INTEL_PATH=C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\icx.exe
set MSVC_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe

echo =============================================================
echo   Compilacion de int128_traits Tests y Benchmarks
echo =============================================================
echo.

REM Crear directorios
echo Creando estructura de directorios...
if not exist "build\build_tests\gcc\release" mkdir "build\build_tests\gcc\release"
if not exist "build\build_tests\clang\release" mkdir "build\build_tests\clang\release"
if not exist "build\build_tests\intel\release" mkdir "build\build_tests\intel\release"
if not exist "build\build_tests\msvc\release" mkdir "build\build_tests\msvc\release"
if not exist "build\build_benchmarks\gcc\release" mkdir "build\build_benchmarks\gcc\release"
if not exist "build\build_benchmarks\clang\release" mkdir "build\build_benchmarks\clang\release"
if not exist "build\build_benchmarks\intel\release" mkdir "build\build_benchmarks\intel\release"
if not exist "build\build_benchmarks\msvc\release" mkdir "build\build_benchmarks\msvc\release"
if not exist "benchmark_results" mkdir "benchmark_results"
echo [OK] Directorios creados
echo.

set SUCCESS_COUNT=0
set TOTAL_COMPILATIONS=8

REM =============================================================================
REM COMPILACION CON GCC (UCRT64)
REM =============================================================================
echo [1/8] Compilando con GCC (UCRT64)...

REM Tests
"%GCC_PATH%" -std=c++20 -Iinclude -O3 -march=native tests\int128_traits_extracted_tests.cpp -o build\build_tests\gcc\release\int128_traits_tests_gcc.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Tests compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en tests
)

REM Benchmarks
"%GCC_PATH%" -std=c++20 -Iinclude -O3 -march=native benchmarks\int128_traits_extracted_benchs.cpp -o build\build_benchmarks\gcc\release\int128_traits_benchs_gcc.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Benchmarks compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en benchmarks
)

REM =============================================================================
REM COMPILACION CON CLANG (CLANG64)
REM =============================================================================
echo.
echo [2/8] Compilando con Clang (CLANG64)...

REM Tests
"%CLANG_PATH%" -std=c++20 -Iinclude -O3 -march=native tests\int128_traits_extracted_tests.cpp -o build\build_tests\clang\release\int128_traits_tests_clang.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Tests compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en tests
)

REM Benchmarks
"%CLANG_PATH%" -std=c++20 -Iinclude -O3 -march=native benchmarks\int128_traits_extracted_benchs.cpp -o build\build_benchmarks\clang\release\int128_traits_benchs_clang.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Benchmarks compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en benchmarks
)

REM =============================================================================
REM COMPILACION CON INTEL ONEAPI
REM =============================================================================
echo.
echo [3/8] Compilando con Intel OneAPI...

REM Configurar entorno Intel
call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" > nul 2>&1

REM Tests
"%INTEL_PATH%" -std=c++20 -Iinclude -O3 tests\int128_traits_extracted_tests.cpp -o build\build_tests\intel\release\int128_traits_tests_intel.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Tests compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en tests
)

REM Benchmarks
"%INTEL_PATH%" -std=c++20 -Iinclude -O3 benchmarks\int128_traits_extracted_benchs.cpp -o build\build_benchmarks\intel\release\int128_traits_benchs_intel.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Benchmarks compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en benchmarks
)

REM =============================================================================
REM COMPILACION CON MSVC
REM =============================================================================
echo.
echo [4/8] Compilando con MSVC...

REM Configurar entorno MSVC
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1

REM Tests
"%MSVC_PATH%" /std:c++20 /EHsc /O2 /I include tests\int128_traits_extracted_tests.cpp /Fe:build\build_tests\msvc\release\int128_traits_tests_msvc.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Tests compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en tests
)

REM Benchmarks
"%MSVC_PATH%" /std:c++20 /EHsc /O2 /I include benchmarks\int128_traits_extracted_benchs.cpp /Fe:build\build_benchmarks\msvc\release\int128_traits_benchs_msvc.exe > nul 2>&1
if %errorlevel% equ 0 (
    echo   [OK] Benchmarks compilados
    set /a SUCCESS_COUNT+=1
) else (
    echo   [ERROR] Error en benchmarks
)

REM =============================================================================
REM RESUMEN
REM =============================================================================
echo.
echo =============================================================
echo   Resumen de Compilacion
echo =============================================================
echo Compilaciones exitosas: %SUCCESS_COUNT% de %TOTAL_COMPILATIONS%

if %SUCCESS_COUNT% equ %TOTAL_COMPILATIONS% (
    echo.
    echo [OK] TODAS LAS COMPILACIONES EXITOSAS
    echo.
    exit /b 0
) else (
    echo.
    echo [ADVERTENCIA] Algunas compilaciones fallaron
    echo.
    exit /b 1
)
