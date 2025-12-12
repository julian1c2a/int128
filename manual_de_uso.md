
### Instrucciones de Uso

Para que esto funcione, debes cargar el entorno en tu terminal actual **una sola vez** por sesión.

1.  **Carga el entorno:**
    ```bash
    source env_msvc.bash
    ```
    *(Verás un mensaje de éxito y la ruta de `cl.exe`)*.

2.  **Compila el programa principal:**
    ```bash
    make
    ```

3.  **Ejecuta el programa:**
    ```bash
    ./int128.exe
    ```

4.  **Compila y ejecuta los tests** (si ya tienes Catch2 en `libs_install`):
    ```bash
    make test
    ```
