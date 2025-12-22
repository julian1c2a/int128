@echo off
setlocal

REM Ajusta esta ruta si tu MSYS2 está en otro sitio
set "MSYS_DIR=C:\msys64"

REM Obtenemos el directorio donde está este script .bat
set "SCRIPT_DIR=%~dp0"

echo Lanzando terminal MSYS2 con entorno Intel...

REM Lanzamos Bash.
REM Usamos CHERE_INVOKING=1 para que se abra en el directorio actual.
set MSYSTEM=CLANG64
set DEV_ENV=INTEL
set CHERE_INVOKING=1

"%MSYS_DIR%\usr\bin\mintty.exe" -i "/msys2.ico" -t "Intel oneAPI + MSVC" ^
    /usr/bin/bash --login -c "cd '$(cygpath -u '%SCRIPT_DIR%')'; source ./setup_intel_combined.bash; exec bash"

endlocal