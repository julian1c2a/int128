@echo off
setlocal

# REM --- Configuración de rutas ---
set "MSYS_DIR=C:\msys64"
set "INTEL_SCRIPT_DIR=%~dp0"

# REM Eliminar barra invertida final si existe
if "%INTEL_SCRIPT_DIR:~-1%"=="\" set "INTEL_SCRIPT_DIR=%INTEL_SCRIPT_DIR:~0,-1%"

# REM --- Configurar variables de MSYS2 ---
set MSYSTEM=CLANG64
set CHERE_INVOKING=1
set DEV_ENV=INTEL

# REM --- Mensaje de arranque ---
echo Iniciando entorno Intel oneAPI + MSYS2...

# REM --- Lanzar Mintty ---
# REM Usamos /bin/bash con un comando de inicio específico para cargar el entorno Intel
"%MSYS_DIR%\usr\bin\mintty.exe" -i "/msys2.ico" -t "Intel oneAPI Shell" ^
    /usr/bin/bash --login -c "source '%INTEL_SCRIPT_DIR:\=/%/setup_intel_env.bash'; exec bash"

endlocal