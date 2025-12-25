@echo off
setlocal

set "MSYS_DIR=C:\msys64"
set "SCRIPT_DIR=%~dp0"

echo Lanzando terminal Intel (DEBUG MODE)...
echo Directorio del script: %SCRIPT_DIR%

set MSYSTEM=CLANG64
set CHERE_INVOKING=1

REM Comando modificado: Añadimos un 'read' o 'sleep' al final para que la ventana no se cierre
"%MSYS_DIR%\usr\bin\mintty.exe" -i "/msys2.ico" -t "Intel Shell DEBUG" ^
    /usr/bin/bash --login -c "echo 'Iniciando...'; cd '$(cygpath -u '%SCRIPT_DIR%')'; ls -l setup_intel_combined.bash; source ./setup_intel_combined.bash || echo 'FALLO EN SOURCE'; echo 'Presiona una tecla para salir...'; read -n 1; exec bash"

endlocal
pause