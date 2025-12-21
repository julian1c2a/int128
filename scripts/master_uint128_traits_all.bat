@echo off
REM Script maestro para compilar y ejecutar todo el proceso de uint128_traits tests y benchmarks
REM Este script hace todo el proceso automaticamente

setlocal enabledelayedexpansion

set PROJECT_DIR=C:\msys64\ucrt64\home\julian\CppProjects\int128
cd /d "%PROJECT_DIR%"

echo ============================================================
echo   uint128_traits.hpp - Proceso Completo
echo   Tests + Benchmarks + 4 Compiladores
echo ============================================================
echo.

REM FASE 1: Compilacion
echo ============================================================
echo   FASE 1: Compilacion con 4 compiladores
echo ============================================================
echo.

call scripts\compile_uint128_traits_extracted.bat

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilacion fallida
    pause
    exit /b 1
)

echo.
echo [OK] Compilacion completada exitosamente
echo.

REM FASE 2: Ejecucion de tests y benchmarks
echo ============================================================
echo   FASE 2: Ejecucion de tests y benchmarks
echo ============================================================
echo.

call scripts\run_uint128_traits_benchmarks.bat

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Ejecucion fallida
    pause
    exit /b 1
)

echo.
echo [OK] Ejecucion completada exitosamente
echo.

REM RESUMEN FINAL
echo ============================================================
echo                     PROCESO COMPLETADO
echo ============================================================
echo.

echo [OK] Todos los pasos completados exitosamente
echo.
echo Resultados disponibles en:
echo   benchmark_results\
echo.

REM Buscar el reporte mas reciente
for /f "delims=" %%i in ('dir /b /o-d "benchmark_results\report_*.txt" 2^>nul') do (
    set LATEST_REPORT=benchmark_results\%%i
    goto :found_report
)
:found_report

if defined LATEST_REPORT (
    echo Para ver el reporte mas reciente:
    echo   type %LATEST_REPORT%
    echo.
    echo Vista previa del reporte:
    echo ------------------------------------------------------------
    type "%LATEST_REPORT%" | more +1
    echo ------------------------------------------------------------
    echo (ver archivo completo para mas detalles^)
    echo.
)

echo Proceso completado!
echo.

pause
