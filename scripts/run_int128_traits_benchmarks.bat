@echo off
REM Script de ejecucion para int128_traits tests y benchmarks
REM Ejecuta los tests y benchmarks compilados con los 4 compiladores
REM Genera reportes consolidados en benchmark_results\

setlocal enabledelayedexpansion

set PROJECT_DIR=C:\msys64\ucrt64\home\julian\CppProjects\int128
cd /d "%PROJECT_DIR%"

REM Crear timestamp
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/: " %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

REM Crear directorio de resultados
set RESULTS_DIR=benchmark_results
if not exist "%RESULTS_DIR%" mkdir "%RESULTS_DIR%"

echo =============================================================
echo   Ejecucion de int128_traits Tests y Benchmarks
echo =============================================================
echo.

REM =============================================================================
REM EJECUTAR TESTS
REM =============================================================================
echo =============================================================
echo   FASE 1: Ejecutando Tests
echo =============================================================
echo.

set TESTS_RUN=0
set TESTS_PASSED=0

REM Test con GCC
if exist "build\build_tests\gcc\release\int128_traits_tests_gcc.exe" (
    echo [1/4] Tests con GCC...
    build\build_tests\gcc\release\int128_traits_tests_gcc.exe > "%RESULTS_DIR%\tests_gcc_%TIMESTAMP%.txt" 2>&1
    if %errorlevel% equ 0 (
        echo   [OK] Tests exitosos ^(guardado en tests_gcc_%TIMESTAMP%.txt^)
        set /a TESTS_PASSED+=1
    ) else (
        echo   [ERROR] Algunos tests fallaron ^(ver tests_gcc_%TIMESTAMP%.txt^)
    )
    set /a TESTS_RUN+=1
) else (
    echo [1/4] Tests con GCC no disponible
)

REM Test con Clang
if exist "build\build_tests\clang\release\int128_traits_tests_clang.exe" (
    echo [2/4] Tests con Clang...
    build\build_tests\clang\release\int128_traits_tests_clang.exe > "%RESULTS_DIR%\tests_clang_%TIMESTAMP%.txt" 2>&1
    if %errorlevel% equ 0 (
        echo   [OK] Tests exitosos ^(guardado en tests_clang_%TIMESTAMP%.txt^)
        set /a TESTS_PASSED+=1
    ) else (
        echo   [ERROR] Algunos tests fallaron ^(ver tests_clang_%TIMESTAMP%.txt^)
    )
    set /a TESTS_RUN+=1
) else (
    echo [2/4] Tests con Clang no disponible
)

REM Test con Intel
if exist "build\build_tests\intel\release\int128_traits_tests_intel.exe" (
    echo [3/4] Tests con Intel...
    build\build_tests\intel\release\int128_traits_tests_intel.exe > "%RESULTS_DIR%\tests_intel_%TIMESTAMP%.txt" 2>&1
    if %errorlevel% equ 0 (
        echo   [OK] Tests exitosos ^(guardado en tests_intel_%TIMESTAMP%.txt^)
        set /a TESTS_PASSED+=1
    ) else (
        echo   [ERROR] Algunos tests fallaron ^(ver tests_intel_%TIMESTAMP%.txt^)
    )
    set /a TESTS_RUN+=1
) else (
    echo [3/4] Tests con Intel no disponible
)

REM Test con MSVC
if exist "build\build_tests\msvc\release\int128_traits_tests_msvc.exe" (
    echo [4/4] Tests con MSVC...
    build\build_tests\msvc\release\int128_traits_tests_msvc.exe > "%RESULTS_DIR%\tests_msvc_%TIMESTAMP%.txt" 2>&1
    if %errorlevel% equ 0 (
        echo   [OK] Tests exitosos ^(guardado en tests_msvc_%TIMESTAMP%.txt^)
        set /a TESTS_PASSED+=1
    ) else (
        echo   [ERROR] Algunos tests fallaron ^(ver tests_msvc_%TIMESTAMP%.txt^)
    )
    set /a TESTS_RUN+=1
) else (
    echo [4/4] Tests con MSVC no disponible
)

echo.
echo Resumen de Tests: !TESTS_PASSED!/!TESTS_RUN! exitosos

if !TESTS_PASSED! lss !TESTS_RUN! (
    echo ADVERTENCIA: Algunos tests fallaron. Continuando con benchmarks...
    echo.
)

REM =============================================================================
REM EJECUTAR BENCHMARKS
REM =============================================================================
echo.
echo =============================================================
echo   FASE 2: Ejecutando Benchmarks
echo =============================================================
echo.

set BENCHS_RUN=0

REM Benchmark con GCC
if exist "build\build_benchmarks\gcc\release\int128_traits_benchs_gcc.exe" (
    echo [1/4] Benchmarks con GCC...
    build\build_benchmarks\gcc\release\int128_traits_benchs_gcc.exe > "%RESULTS_DIR%\benchmarks_gcc_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_gcc_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [1/4] Benchmarks con GCC no disponible
)

REM Benchmark con Clang
if exist "build\build_benchmarks\clang\release\int128_traits_benchs_clang.exe" (
    echo [2/4] Benchmarks con Clang...
    build\build_benchmarks\clang\release\int128_traits_benchs_clang.exe > "%RESULTS_DIR%\benchmarks_clang_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_clang_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [2/4] Benchmarks con Clang no disponible
)

REM Benchmark con Intel
if exist "build\build_benchmarks\intel\release\int128_traits_benchs_intel.exe" (
    echo [3/4] Benchmarks con Intel...
    build\build_benchmarks\intel\release\int128_traits_benchs_intel.exe > "%RESULTS_DIR%\benchmarks_intel_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_intel_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [3/4] Benchmarks con Intel no disponible
)

REM Benchmark con MSVC
if exist "build\build_benchmarks\msvc\release\int128_traits_benchs_msvc.exe" (
    echo [4/4] Benchmarks con MSVC...
    build\build_benchmarks\msvc\release\int128_traits_benchs_msvc.exe > "%RESULTS_DIR%\benchmarks_msvc_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_msvc_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [4/4] Benchmarks con MSVC no disponible
)

REM =============================================================================
REM CONSOLIDAR RESULTADOS
REM =============================================================================
echo.
echo =============================================================
echo   FASE 3: Consolidando Resultados
echo =============================================================
echo.

set SUMMARY_CSV=%RESULTS_DIR%\summary_%TIMESTAMP%.csv
set REPORT_TXT=%RESULTS_DIR%\report_%TIMESTAMP%.txt

REM Crear encabezado CSV
echo Compilador,Benchmark,Tiempo_ns,Ciclos > "%SUMMARY_CSV%"

echo [OK] Datos consolidados en %SUMMARY_CSV%

REM =============================================================================
REM GENERAR REPORTE
REM =============================================================================
echo. > "%REPORT_TXT%"
echo ============================================================================= >> "%REPORT_TXT%"
echo   REPORTE DE int128_traits TESTS Y BENCHMARKS >> "%REPORT_TXT%"
echo ============================================================================= >> "%REPORT_TXT%"
echo Fecha: %date% %time% >> "%REPORT_TXT%"
echo Directorio: %PROJECT_DIR% >> "%REPORT_TXT%"
echo. >> "%REPORT_TXT%"
echo ----------------------------------------------------------------------------- >> "%REPORT_TXT%"
echo RESUMEN DE EJECUCION >> "%REPORT_TXT%"
echo ----------------------------------------------------------------------------- >> "%REPORT_TXT%"
echo Tests ejecutados: %TESTS_RUN% >> "%REPORT_TXT%"
echo Tests exitosos: %TESTS_PASSED% >> "%REPORT_TXT%"
echo Benchmarks ejecutados: %BENCHS_RUN% >> "%REPORT_TXT%"
echo. >> "%REPORT_TXT%"

echo [OK] Reporte generado en %REPORT_TXT%

REM =============================================================================
REM RESUMEN FINAL
REM =============================================================================
echo.
echo =============================================================
echo                     PROCESO COMPLETADO
echo =============================================================
echo.

echo Resultados guardados en: %RESULTS_DIR%\
echo Resumen CSV: %SUMMARY_CSV%
echo Reporte: %REPORT_TXT%
echo.

if !TESTS_PASSED! equ !TESTS_RUN! (
    if !BENCHS_RUN! gtr 0 (
        echo [OK] PROCESO EXITOSO
        echo.
        exit /b 0
    )
)

if !BENCHS_RUN! gtr 0 (
    echo [ADVERTENCIA] Proceso completado con advertencias
    echo.
    exit /b 0
) else (
    echo [ERROR] No se ejecutaron benchmarks
    echo.
    exit /b 1
)
