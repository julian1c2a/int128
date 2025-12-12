### Cómo usar esto en tu proyecto `int128`

1.  Coloca `vcvarsall.py` y `activate_msvc.bash` en la raíz de tu carpeta (donde está el `Makefile`).
2.  Desde tu terminal MSYS2 UCRT64, ejecuta:

    ```bash
    source activate_msvc.bash x64
    ```

    *(Nota: Debes usar `source` o `.`, no `./activate...`, para que las variables se queden en tu terminal actual).*

3.  Ahora ejecuta `make` (usando el Makefile que te di en la respuesta anterior).

    ```bash
    make
    ```

Esto debería compilar `int128.exe` utilizando el compilador nativo de Microsoft pero orquestado desde tu terminal Bash favorita.