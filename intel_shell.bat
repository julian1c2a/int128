@echo on
REM 1. Cargar entorno Intel (Ajusta la ruta)
source <(C:\Program Files (x86)\Intel\oneAPI\setvars-vcvarsall.bat)
source <(C:\Program Files (x86)\Intel\oneAPI\setvars.bat)

REM 2. Configurar variable
set DEV_ENV=INTEL
set MSYSTEM=CLANG64

REM 3. Lanzar Bash
C:\msys64\usr\bin\mintty.exe -i /msys2.ico -t "Intel Shell" /usr/bin/bash -l -i