@echo off
REM Script para compilar test con Intel ICX

REM Cargar entorno Intel oneAPI
call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64 vs2022 > nul 2>&1

REM Ir al directorio del proyecto
cd /d C:\msys64\ucrt64\home\julian\CppProjects\int128

REM Compilar el test
echo Compilando con Intel ICX...
icx -std=c++20 -W4 -I include_new test_intel_intrinsics.cpp -o test_intel_intrinsics_icx.exe 2>&1

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [OK] Compilacion exitosa!
    echo.
    echo Ejecutando test...
    test_intel_intrinsics_icx.exe
) else (
    echo.
    echo [ERROR] Fallo la compilacion (codigo: %ERRORLEVEL%)
)
