#!/bin/bash

# --------------------------------------------------------------------------
# CONFIGURACIÓN
# Ajusta esta ruta a tu versión de Visual Studio (Community/Professional/Enterprise)
# --------------------------------------------------------------------------
VS_BAT_PATH="C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"

# Rutas de herramientas de MSYS2 (convertidas a formato Windows para CMD)
MINTTY_PATH=$(cygpath -w /usr/bin/mintty.exe)
BASH_PATH=$(cygpath -w /usr/bin/bash.exe)
ICON_PATH=$(cygpath -w /msys2.ico)

# --------------------------------------------------------------------------
# VALIDACIÓN
# --------------------------------------------------------------------------
# Convertimos la ruta del bat de VS a formato Unix para comprobar si existe desde Bash
VS_BAT_UNIX_PATH=$(cygpath -u "$VS_BAT_PATH")

if [ ! -f "$VS_BAT_UNIX_PATH" ]; then
    echo "Error: No se encuentra vcvars64.bat en:"
    echo "$VS_BAT_UNIX_PATH"
    echo "Por favor edita el script y corrige la variable VS_BAT_PATH."
    exit 1
fi

# --------------------------------------------------------------------------
# CONSTRUCCIÓN DEL COMANDO
# --------------------------------------------------------------------------
# Explicación de la cadena de comandos que pasaremos a CMD:
# 1. call vcvars64.bat  -> Carga las variables de entorno de MSVC
# 2. set DEV_ENV=MSVC   -> Define nuestra variable para colorear el prompt (ver tu .bashrc)
# 3. set MSYSTEM=UCRT64 -> Define el sistema base de MSYS2
# 4. bash.exe -l -i     -> Lanza el bash interactivo FINAL dentro de ese entorno
# --------------------------------------------------------------------------

CMD_COMMAND="call \"$VS_BAT_PATH\" && set DEV_ENV=MSVC && set MSYSTEM=UCRT64 && \"$BASH_PATH\" -l -i"

echo "Lanzando terminal MSVC..."

# --------------------------------------------------------------------------
# EJECUCIÓN (Lanza nueva ventana Mintty)
# --------------------------------------------------------------------------
# -i: Icono
# -t: Título de la ventana
# -e: Ejecutar comando (en este caso, cmd.exe que hace de puente)

"$MINTTY_PATH" -i "$ICON_PATH" -t "MSYS2 - MSVC Shell" -e cmd.exe /c "$CMD_COMMAND" &

# Salimos limpiamente sin bloquear la terminal actual
disown