# Testing Local - Guía Completa

Esta guía describe cómo ejecutar tests localmente en múltiples compiladores, replicando el entorno CI/CD de GitHub Actions.

## 📋 Tabla de Contenidos

1. [Requisitos](#requisitos)
2. [Instalación de Compiladores](#instalación-de-compiladores)
3. [Script Maestro CI/CD](#script-maestro-cicd)
4. [Configuración WSL Ubuntu](#configuración-wsl-ubuntu)
5. [Troubleshooting](#troubleshooting)
6. [Ejemplos de Uso](#ejemplos-de-uso)

---

## 🔧 Requisitos

### Requisitos Mínimos

- **Linux/WSL**: Ubuntu 22.04+, Debian 12+, o similar
- **RAM**: 4 GB (8 GB recomendado para compilación paralela)
- **Disco**: 10 GB libres para compiladores y build artifacts
- **Git**: 2.30+

### Compiladores Soportados

| Compilador | Versión Mínima | Versión Recomendada | Notas |
|------------|----------------|---------------------|-------|
| **GCC**    | 13.1           | 14.x                | Soporte completo C++20 |
| **Clang**  | 18.0           | 19.x                | Mejor soporte constexpr |
| **MSVC**   | 19.35 (VS2022) | 19.38+              | Requiere vcvarsall.bat |
| **Intel**  | 2024.0 (icpx)  | 2024.2+             | Opcional, mejor SIMD |

---

## 📦 Instalación de Compiladores

### Ubuntu 25.04 (WSL o Nativo)

#### GCC 13, 14, 15

```bash
# Repositorios oficiales
sudo apt update
sudo apt install -y gcc-13 g++-13 gcc-14 g++-14 gcc-15 g++-15

# Verificar instalación
gcc-13 --version
gcc-14 --version
gcc-15 --version

# Configurar alternatives (opcional)
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-15 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-15 100
```

#### Clang 18, 19, 20, 21

```bash
# Agregar repositorio LLVM oficial
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh

# Instalar versiones específicas
sudo ./llvm.sh 18
sudo ./llvm.sh 19
sudo ./llvm.sh 20
sudo ./llvm.sh 21

# Verificar instalación
clang-18 --version
clang-19 --version
clang-20 --version
clang-21 --version

# Configurar alternatives (opcional)
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-21 100
```

#### Intel oneAPI (icpx)

```bash
# Método 1: APT Repository (recomendado)
wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | \
    gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | \
    sudo tee /etc/apt/sources.list.d/oneAPI.list

sudo apt update
sudo apt install intel-oneapi-compiler-dpcpp-cpp

# Activar entorno Intel
source /opt/intel/oneapi/setvars.sh

# Verificar
icpx --version

# Método 2: Download directo (offline installer)
# https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler-download.html
```

#### MSVC (Solo en Windows/WSL con acceso a filesystem Windows)

```bash
# En WSL, MSVC se ejecuta desde la instalación Windows
# Requiere Visual Studio 2022 instalado en Windows

# Script vcvars.py automatiza la búsqueda de vcvarsall.bat
# Ver: scripts/vcvars.py

# Alternativa: Detectar desde WSL
ls /mnt/c/Program\ Files/Microsoft\ Visual\ Studio/2022/*/VC/Auxiliary/Build/vcvarsall.bat
```

---

## 🚀 Script Maestro CI/CD

### Descripción

`scripts/run_all_compilers.bash` ejecuta todos los tests en todos los compiladores disponibles, replicando el workflow de GitHub Actions.

### Características

- ✅ Auto-detección de compiladores instalados
- ✅ Compilación paralela (debug + release)
- ✅ Tabla de resultados con colores ANSI
- ✅ Estadísticas finales (total, passed, failed)
- ✅ Exit codes apropiados (0 = éxito, 1 = fallos)
- ✅ Opciones de filtrado por compilador

### Uso Básico

```bash
# Ejecutar todos los compiladores disponibles
bash scripts/run_all_compilers.bash

# Solo GCC
bash scripts/run_all_compilers.bash --only gcc

# Solo Clang
bash scripts/run_all_compilers.bash --only clang

# Omitir MSVC (útil en Linux sin acceso a Windows)
bash scripts/run_all_compilers.bash --skip-msvc

# Combinaciones
bash scripts/run_all_compilers.bash --only intel
```

### Salida de Ejemplo

```
========================================
  CI/CD Local - Multi-Compiler Build
========================================

Detected Compilers:
  [✓] g++ (gcc 14.2.0)
  [✓] clang++ (clang 19.0.0)
  [✓] icpx (Intel 2024.2)
  [✗] cl.exe (not found)

========================================

Building with gcc...
  [DEBUG]   Compiling 27 tests... ✓ (27/27 passed)
  [RELEASE] Compiling 27 tests... ✓ (27/27 passed)

Building with clang...
  [DEBUG]   Compiling 27 tests... ✓ (27/27 passed)
  [RELEASE] Compiling 27 tests... ✓ (27/27 passed)

Building with intel...
  [DEBUG]   Compiling 27 tests... ✓ (27/27 passed)
  [RELEASE] Compiling 27 tests... ✓ (27/27 passed)

========================================
            Results Summary
========================================
┌──────────┬────────┬────────┬──────┐
│ Compiler │ Debug  │ Release│ Total│
├──────────┼────────┼────────┼──────┤
│ gcc      │ 27/27  │ 27/27  │ ✓    │
│ clang    │ 27/27  │ 27/27  │ ✓    │
│ intel    │ 27/27  │ 27/27  │ ✓    │
└──────────┴────────┴────────┴──────┘

Total Tests: 162
Passed: 162 (100%)
Failed: 0 (0%)

Elapsed Time: 145s

🎉 All tests passed!
```

---

## 🐧 Configuración WSL Ubuntu

### Instalación WSL 2

```powershell
# En PowerShell (Windows 11)
wsl --install -d Ubuntu-24.04

# Actualizar a Ubuntu 25.04 (opcional)
# Dentro de WSL:
sudo do-release-upgrade -d
```

### Integración con Windows

#### Acceso a Filesystem Windows desde WSL

```bash
# Proyectos en Windows son accesibles desde /mnt/c/
cd /mnt/c/Users/julian/Projects/int128

# Clonar en filesystem WSL (mejor rendimiento)
cd ~
git clone https://github.com/julian1c2a/int128.git
cd int128
```

#### Ejecutar MSVC desde WSL

```bash
# vcvars.py busca automáticamente vcvarsall.bat
# Ver scripts/build_extracted_tests.bash línea 80-100

# Ejemplo manual:
/mnt/c/Program\ Files/Microsoft\ Visual\ Studio/2022/Professional/VC/Auxiliary/Build/vcvarsall.bat x64
cl.exe /std:c++20 /EHsc test.cpp
```

### Performance Tips

- **Filesystem**: Usa filesystem WSL (~/) para mejor I/O
- **RAM**: Asigna más RAM a WSL en `.wslconfig`:

  ```ini
  [wsl2]
  memory=8GB
  processors=8
  ```

- **Compilación paralela**: `-j$(nproc)` usa todos los cores

---

## 🔍 Troubleshooting

### Problema: `g++-15: command not found`

**Solución**:

```bash
# Verificar versiones disponibles
apt-cache search gcc-

# Instalar versión específica
sudo apt install gcc-15 g++-15

# Verificar
which g++-15
g++-15 --version
```

### Problema: Clang no encuentra headers de C++ stdlib

**Solución**:

```bash
# Instalar headers de desarrollo
sudo apt install libstdc++-14-dev

# Verificar
clang++ -v -x c++ /dev/null -fsyntax-only 2>&1 | grep "Selected GCC installation"
```

### Problema: Intel icpx falla con "cannot find -lstdc++"

**Solución**:

```bash
# Re-activar entorno Intel
source /opt/intel/oneapi/setvars.sh

# Verificar library paths
icpx -v test.cpp

# Si persiste, agregar path manualmente
icpx -L/usr/lib/gcc/x86_64-linux-gnu/14 test.cpp
```

### Problema: MSVC desde WSL falla con "vcvarsall.bat not found"

**Solución**:

```bash
# Verificar instalación Visual Studio en Windows
ls /mnt/c/Program\ Files/Microsoft\ Visual\ Studio/2022/

# Editar scripts/vcvars.py para agregar path personalizado
# O usar variable de entorno:
export VCVARSALL_PATH="/mnt/c/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Auxiliary/Build/vcvarsall.bat"
```

### Problema: Tests pasan en CI/CD pero fallan localmente

**Posibles causas**:

1. **Versión diferente de compilador**: CI usa GCC 13, local tiene GCC 15
2. **Flags diferentes**: Verificar `scripts/build_extracted_tests.bash`
3. **Sanitizers**: CI usa `-fsanitize=address,undefined`
4. **Optimización**: CI usa `-O2` en release, verifica que sea igual

**Solución**:

```bash
# Reproducir exactamente las flags de CI
# Ver .github/workflows/build.yml para flags exactos

# Ejemplo GCC con sanitizers:
g++-13 -std=c++20 -O2 -fsanitize=address,undefined test.cpp
./a.out
```

---

## 🎯 Ejemplos de Uso

### Ejemplo 1: CI/CD Completo Local

```bash
# Ejecutar todo como CI/CD
bash scripts/run_all_compilers.bash

# Guardar logs
bash scripts/run_all_compilers.bash > ci_results.log 2>&1

# Verificar exit code
bash scripts/run_all_compilers.bash
echo $?  # 0 = éxito, 1 = fallos
```

### Ejemplo 2: Testing Rápido con Solo un Compilador

```bash
# Solo GCC debug
bash scripts/build_extracted_tests.bash gcc debug

# Solo Clang release
bash scripts/build_extracted_tests.bash clang release

# Ejecutar tests después de compilar
bash scripts/run_extracted_tests.bash gcc release
```

### Ejemplo 3: Benchmark en Múltiples Compiladores

```bash
# Compilar benchs con GCC
cd benchs
make gcc-release

# Compilar benchs con Clang
make clang-release

# Compilar benchs con Intel
make intel-release

# Ejecutar todos
./int128_algorithm_extracted_benchs_gcc
./int128_algorithm_extracted_benchs_clang
./int128_algorithm_extracted_benchs_intel
```

### Ejemplo 4: Integración con Git Hooks

```bash
# Crear pre-push hook
cat > .git/hooks/pre-push << 'EOF'
#!/bin/bash
echo "Running CI/CD tests before push..."
bash scripts/run_all_compilers.bash
if [ $? -ne 0 ]; then
    echo "❌ Tests failed! Push aborted."
    exit 1
fi
echo "✅ All tests passed! Pushing..."
EOF

chmod +x .git/hooks/pre-push
```

### Ejemplo 5: Testing Continuo con `watch`

```bash
# Re-compilar automáticamente cada 5 segundos
watch -n 5 'bash scripts/build_extracted_tests.bash gcc debug'

# Mejor: usar inotifywait para compilar al modificar archivos
sudo apt install inotify-tools
while inotifywait -e modify include/; do
    bash scripts/build_extracted_tests.bash gcc debug
done
```

---

## 📊 Comparación CI/CD vs Local

| Aspecto            | GitHub Actions CI/CD | Local WSL Ubuntu       |
|--------------------|----------------------|------------------------|
| **Compiladores**   | GCC 13/14, Clang 18/19, MSVC | GCC 13-15, Clang 18-21, Intel |
| **Runtime**        | ubuntu-24.04, windows-latest | Ubuntu 25.04, WSL 2 |
| **Tiempo**         | ~10 minutos (paralelo) | ~3-5 minutos (local) |
| **Sanitizers**     | ASan, UBSan          | Manuales (opcionales) |
| **Artifacts**      | Logs guardados automáticamente | Manuales |
| **Costo**          | Gratuito (GitHub)    | Gratuito (local) |
| **Flexibilidad**   | Configurado en YAML  | Total control manual |

---

## 🔗 Referencias

- **GitHub Actions Workflow**: [`.github/workflows/build.yml`](../.github/workflows/build.yml)
- **Script Build Tests**: [`scripts/build_extracted_tests.bash`](../scripts/build_extracted_tests.bash)
- **Script Run Tests**: [`scripts/run_extracted_tests.bash`](../scripts/run_extracted_tests.bash)
- **Script Maestro**: [`scripts/run_all_compilers.bash`](../scripts/run_all_compilers.bash)
- **Proyecto README**: [`README.md`](../README.md)

---

## 📝 Notas Adicionales

### ARM64 Support (Experimental)

Posible soporte futuro para ARM64 (Apple Silicon, Raspberry Pi, servidores ARM):

```bash
# Opción 1: Docker ARM64
docker run --platform linux/arm64 -it ubuntu:24.04

# Opción 2: QEMU User Emulation
sudo apt install qemu-user-static
docker run --platform linux/arm64 -it ubuntu:24.04

# Opción 3: Cross-compilation
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
aarch64-linux-gnu-g++ -std=c++20 test.cpp
```

### Extensiones Futuras

- [ ] Script maestro con soporte ARM64
- [ ] Docker images con todos los compiladores
- [ ] Integración con CTest (CMake)
- [ ] Coverage reports con gcov/lcov
- [ ] Benchmarks automáticos en CI/CD

---

**Autor**: Julian Catumba  
**Fecha**: Diciembre 2024  
**Versión**: 1.0
