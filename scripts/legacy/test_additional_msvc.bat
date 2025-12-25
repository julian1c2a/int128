@echo off
REM Script para compilar y ejecutar tests adicionales con MSVC

echo Compilando tests adicionales con MSVC...

REM Crear directorios si no existen
if not exist "build\build_tests\msvc\release" mkdir "build\build_tests\msvc\release"

REM Compilar
cl.exe /EHsc /std:c++20 /O2 /W4 /I include tests\uint128_additional_tests.cpp /Fe:build\build_tests\msvc\release\uint128_additional_tests.exe

if %ERRORLEVEL% EQU 0 (
    echo Compilación exitosa.
    echo.
    echo Ejecutando tests...
    build\build_tests\msvc\release\uint128_additional_tests.exe
) else (
    echo Error en la compilación.
    exit /b 1
)
