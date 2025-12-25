@echo off
REM Script PowerShell para ejecutar benchmarks de uint128_traits con los 4 compiladores
REM y recopilar resultados de tiempo y ciclos de reloj

setlocal enabledelayedexpansion

set PROJECT_DIR=C:\msys64\ucrt64\home\julian\CppProjects\int128
cd /d "%PROJECT_DIR%"

REM Directorio de resultados
set RESULTS_DIR=benchmark_results\uint128_traits_extracted
if not exist "%RESULTS_DIR%" mkdir "%RESULTS_DIR%"

REM Timestamp para los archivos de resultados
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

echo ============================================================
echo   Ejecucion de Benchmarks - uint128_traits.hpp
echo ============================================================
echo Fecha: %date% %time%
echo Timestamp: %TIMESTAMP%
echo.

REM =============================================================================
REM EJECUTAR TESTS PRIMERO (Validacion)
REM =============================================================================
echo ============================================================
echo   FASE 1: Ejecutando Tests (Validacion)
echo ============================================================
echo.

set TESTS_PASSED=0
set TESTS_TOTAL=0

REM Test con GCC
if exist "build\build_tests\gcc\release\uint128_traits_tests_gcc.exe" (
    echo [1/4] Tests con GCC...
    set /a TESTS_TOTAL+=1
    build\build_tests\gcc\release\uint128_traits_tests_gcc.exe > "%RESULTS_DIR%\tests_gcc_%TIMESTAMP%.txt" 2>&1
    if !errorlevel! equ 0 (
        echo   [OK] PASS
        set /a TESTS_PASSED+=1
    ) else (
        echo   [FAIL] ver %RESULTS_DIR%\tests_gcc_%TIMESTAMP%.txt
    )
) else (
    echo [1/4] Tests con GCC no disponible
)

REM Test con Clang
if exist "build\build_tests\clang\release\uint128_traits_tests_clang.exe" (
    echo [2/4] Tests con Clang...
    set /a TESTS_TOTAL+=1
    build\build_tests\clang\release\uint128_traits_tests_clang.exe > "%RESULTS_DIR%\tests_clang_%TIMESTAMP%.txt" 2>&1
    if !errorlevel! equ 0 (
        echo   [OK] PASS
        set /a TESTS_PASSED+=1
    ) else (
        echo   [FAIL] ver %RESULTS_DIR%\tests_clang_%TIMESTAMP%.txt
    )
) else (
    echo [2/4] Tests con Clang no disponible
)

REM Test con Intel
if exist "build\build_tests\intel\release\uint128_traits_tests_intel.exe" (
    echo [3/4] Tests con Intel...
    set /a TESTS_TOTAL+=1
    build\build_tests\intel\release\uint128_traits_tests_intel.exe > "%RESULTS_DIR%\tests_intel_%TIMESTAMP%.txt" 2>&1
    if !errorlevel! equ 0 (
        echo   [OK] PASS
        set /a TESTS_PASSED+=1
    ) else (
        echo   [FAIL] ver %RESULTS_DIR%\tests_intel_%TIMESTAMP%.txt
    )
) else (
    echo [3/4] Tests con Intel no disponible
)

REM Test con MSVC
if exist "build\build_tests\msvc\release\uint128_traits_tests_msvc.exe" (
    echo [4/4] Tests con MSVC...
    set /a TESTS_TOTAL+=1
    build\build_tests\msvc\release\uint128_traits_tests_msvc.exe > "%RESULTS_DIR%\tests_msvc_%TIMESTAMP%.txt" 2>&1
    if !errorlevel! equ 0 (
        echo   [OK] PASS
        set /a TESTS_PASSED+=1
    ) else (
        echo   [FAIL] ver %RESULTS_DIR%\tests_msvc_%TIMESTAMP%.txt
    )
) else (
    echo [4/4] Tests con MSVC no disponible
)

echo.
echo Tests completados: !TESTS_PASSED!/!TESTS_TOTAL!
echo.

REM =============================================================================
REM EJECUTAR BENCHMARKS
REM =============================================================================
echo ============================================================
echo   FASE 2: Ejecutando Benchmarks
echo ============================================================
echo.

set BENCHS_RUN=0

REM Benchmark con GCC
if exist "build\build_benchmarks\gcc\release\uint128_traits_benchs_gcc.exe" (
    echo [1/4] Benchmarks con GCC...
    build\build_benchmarks\gcc\release\uint128_traits_benchs_gcc.exe > "%RESULTS_DIR%\benchmarks_gcc_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_gcc_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [1/4] Benchmarks con GCC no disponible
)

REM Benchmark con Clang
if exist "build\build_benchmarks\clang\release\uint128_traits_benchs_clang.exe" (
    echo [2/4] Benchmarks con Clang...
    build\build_benchmarks\clang\release\uint128_traits_benchs_clang.exe > "%RESULTS_DIR%\benchmarks_clang_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_clang_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [2/4] Benchmarks con Clang no disponible
)

REM Benchmark con Intel
if exist "build\build_benchmarks\intel\release\uint128_traits_benchs_intel.exe" (
    echo [3/4] Benchmarks con Intel...
    build\build_benchmarks\intel\release\uint128_traits_benchs_intel.exe > "%RESULTS_DIR%\benchmarks_intel_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_intel_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [3/4] Benchmarks con Intel no disponible
)

REM Benchmark con MSVC
if exist "build\build_benchmarks\msvc\release\uint128_traits_benchs_msvc.exe" (
    echo [4/4] Benchmarks con MSVC...
    build\build_benchmarks\msvc\release\uint128_traits_benchs_msvc.exe > "%RESULTS_DIR%\benchmarks_msvc_%TIMESTAMP%.txt" 2>&1
    echo   [OK] Completado ^(guardado en benchmarks_msvc_%TIMESTAMP%.txt^)
    set /a BENCHS_RUN+=1
) else (
    echo [4/4] Benchmarks con MSVC no disponible
)

REM =============================================================================
REM CONSOLIDAR RESULTADOS CSV
REM =============================================================================
echo.
echo ============================================================
echo   FASE 3: Consolidando Resultados
echo ============================================================
echo.

set SUMMARY_CSV=%RESULTS_DIR%\summary_%TIMESTAMP%.csv

echo Compiler,Benchmark,Time(ns),Cycles,Iterations > "%SUMMARY_CSV%"

echo Extrayendo datos CSV...

REM Extraer CSV de GCC
if exist "%RESULTS_DIR%\benchmarks_gcc_%TIMESTAMP%.txt" (
    findstr /V "Benchmark,Time" "%RESULTS_DIR%\benchmarks_gcc_%TIMESTAMP%.txt" | findstr "," | (
        for /f "tokens=*" %%a in ('more') do (
            if not "%%a"=="" echo GCC,%%a >> "%SUMMARY_CSV%"
        )
    ) 2>nul
)

REM Extraer CSV de Clang
if exist "%RESULTS_DIR%\benchmarks_clang_%TIMESTAMP%.txt" (
    findstr /V "Benchmark,Time" "%RESULTS_DIR%\benchmarks_clang_%TIMESTAMP%.txt" | findstr "," | (
        for /f "tokens=*" %%a in ('more') do (
            if not "%%a"=="" echo Clang,%%a >> "%SUMMARY_CSV%"
        )
    ) 2>nul
)

REM Extraer CSV de Intel
if exist "%RESULTS_DIR%\benchmarks_intel_%TIMESTAMP%.txt" (
    findstr /V "Benchmark,Time" "%RESULTS_DIR%\benchmarks_intel_%TIMESTAMP%.txt" | findstr "," | (
        for /f "tokens=*" %%a in ('more') do (
            if not "%%a"=="" echo Intel,%%a >> "%SUMMARY_CSV%"
        )
    ) 2>nul
)

REM Extraer CSV de MSVC
if exist "%RESULTS_DIR%\benchmarks_msvc_%TIMESTAMP%.txt" (
    findstr /V "Benchmark,Time" "%RESULTS_DIR%\benchmarks_msvc_%TIMESTAMP%.txt" | findstr "," | (
        for /f "tokens=*" %%a in ('more') do (
            if not "%%a"=="" echo MSVC,%%a >> "%SUMMARY_CSV%"
        )
    ) 2>nul
)

echo [OK] CSV consolidado guardado en: %SUMMARY_CSV%

REM =============================================================================
REM GENERAR REPORTE
REM =============================================================================
echo.
echo Generando reporte de comparacion...

set REPORT_FILE=%RESULTS_DIR%\report_%TIMESTAMP%.txt

(
    echo ============================================================
    echo   uint128_traits.hpp - Reporte de Benchmarks
    echo ============================================================
    echo Fecha: %date% %time%
    echo Timestamp: %TIMESTAMP%
    echo.
    echo ------------------------------------------------------------
    echo RESUMEN DE TESTS
    echo ------------------------------------------------------------
    echo Tests ejecutados: !TESTS_TOTAL!
    echo Tests pasados: !TESTS_PASSED!
    echo.
    echo ------------------------------------------------------------
    echo RESUMEN DE BENCHMARKS
    echo ------------------------------------------------------------
    echo Benchmarks ejecutados: !BENCHS_RUN! compiladores
    echo.
    echo Archivos generados:
    dir /b "%RESULTS_DIR%\*_%TIMESTAMP%.*"
    echo.
    echo ------------------------------------------------------------
    echo DATOS CSV CONSOLIDADOS
    echo ------------------------------------------------------------
    type "%SUMMARY_CSV%"
) > "%REPORT_FILE%"

echo [OK] Reporte guardado en: %REPORT_FILE%

REM =============================================================================
REM RESUMEN FINAL
REM =============================================================================
echo.
echo ============================================================
echo                     RESUMEN FINAL
echo ============================================================
echo Tests completados: !TESTS_PASSED!/!TESTS_TOTAL!
echo Benchmarks ejecutados: !BENCHS_RUN! compiladores
echo Directorio de resultados: %RESULTS_DIR%
echo.
echo Archivos generados:
echo   - %SUMMARY_CSV%
echo   - %REPORT_FILE%
echo   - tests_*_%TIMESTAMP%.txt ^(logs de tests^)
echo   - benchmarks_*_%TIMESTAMP%.txt ^(logs de benchmarks^)
echo.
echo ============================================================
echo   Benchmarks completados exitosamente!
echo ============================================================
echo.

pause
