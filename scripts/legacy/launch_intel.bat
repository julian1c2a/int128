@echo off
setlocal

REM --- Ajusta ruta de MSYS2 si es diferente ---
set "MSYS_DIR=C:\msys64"

set "SCRIPT_DIR=%~dp0"
echo Lanzando terminal Intel...

set DEV_ENV=INTEL
set MSYSTEM=CLANG64
set CHERE_INVOKING=1

REM Usamos cygpath para manejar la ruta del script de forma segura y evitar el error "/c/C/..."
"%MSYS_DIR%\usr\bin\mintty.exe" -i "/msys2.ico" -t "Intel oneAPI + MSVC Shell" ^
    /usr/bin/bash --login -c "cd '$(cygpath -u '%SCRIPT_DIR%')'; source ./setup_intel_combined.bash; exec bash"

endlocal