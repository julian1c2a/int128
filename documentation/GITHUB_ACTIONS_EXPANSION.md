# GitHub Actions - Configuración Multiplataforma (Ejemplo)

Este archivo muestra cómo expandir el CI/CD actual (solo Windows) para incluir Linux y macOS.

## 📋 Estado Actual

**Archivo**: `.github/workflows/ci.yml`
- ✅ Windows x86_64 (GCC, Clang, MSVC)
- ❌ Linux x86_64
- ❌ macOS (Intel + ARM64)

## 🚀 Expansión Propuesta

### Opción 1: Archivo Multi-Plataforma Completo

Reemplazar `.github/workflows/ci.yml` con el contenido de este archivo:
`docs/github-actions-multiplatform.yml.example`

**Ventajas**:
- Testing en 3 SOs diferentes
- Múltiples compiladores
- Detección temprana de problemas de portabilidad

**Desventajas**:
- Más tiempo de ejecución (múltiples runners)
- Puede requerir GitHub Actions Pro para paralelización
- Scripts actuales (build_*.bash) son específicos de Windows/MSYS2

### Opción 2: CI Separado por Plataforma

Crear workflows separados:
- `.github/workflows/ci-windows.yml` (ya existe)
- `.github/workflows/ci-linux.yml` (nuevo)
- `.github/workflows/ci-macos.yml` (nuevo)

**Ventajas**:
- Más modular
- Fácil de deshabilitar plataformas problemáticas
- Mejor organización

### Opción 3: Solo Compilación (Sin Tests)

Agregar jobs de compilación simple:

```yaml
  linux-compile-check:
    runs-on: ubuntu-22.04
    steps:
    - uses: actions/checkout@v4
    - run: g++ -std=c++20 -I./include -c tests/test_interoperability_uint128_int128.cpp
```

**Ventajas**:
- Rápido
- Verifica que el código compila en otras plataformas
- Sin necesidad de adaptar scripts

## 📝 Recomendación

**Para empezar**: Opción 3 (solo compilación)
- Agrega verificación básica sin complejidad
- No requiere modificar scripts
- Puede expandirse gradualmente

**Para testing completo**: Opción 1
- Una vez adaptados los scripts o usando comandos directos
- Proporciona confianza máxima en portabilidad

## 💻 Comandos Directos por Plataforma

### Linux (GCC/Clang)

```bash
# Compilar
g++ -std=c++20 -O2 -I./include tests/test_interoperability_uint128_int128.cpp -o test_interop

# Ejecutar
./test_interop
```

### macOS (Clang)

```bash
# Compilar
clang++ -std=c++20 -O2 -I./include tests/test_interoperability_uint128_int128.cpp -o test_interop

# Ejecutar
./test_interop
```

### Windows (MSVC)

```cmd
# Compilar
cl /std:c++20 /EHsc /O2 /I.\include tests\test_interoperability_uint128_int128.cpp /Fe:test_interop.exe

# Ejecutar
test_interop.exe
```

## 🔧 Script de Adaptación

Si decides usar los scripts actuales en Linux/macOS, necesitarás:

1. Remover `cygpath` (solo existe en Cygwin/MSYS2)
2. Ajustar rutas de ejecutables (`.exe` → sin extensión)
3. Verificar que los comandos del compilador funcionan

Ejemplo de función portable:

```bash
# Función para obtener ruta portable
get_portable_path() {
    if command -v cygpath &> /dev/null; then
        cygpath -w "$1"  # Windows MSYS2
    else
        echo "$1"  # Linux/macOS
    fi
}
```

## 📦 Contenido del Archivo Example

Ver contenido completo en: `docs/github-actions-multiplatform.yml.example`

Este archivo contiene:
- Jobs para Ubuntu 22.04 y 24.04
- GCC 13, 14 y Clang 16, 17
- macOS Intel y ARM64
- Windows MSVC (ya existente)
- Job de resumen final

## ✅ Siguientes Pasos

1. **Revisar** el archivo example
2. **Decidir** estrategia (Opción 1, 2 o 3)
3. **Testear** localmente en VM si es posible
4. **Implementar** gradualmente
5. **Monitorear** resultados en GitHub Actions

---

**Fecha**: 25 de diciembre de 2025
**Estado**: Documentación de expansión CI/CD
