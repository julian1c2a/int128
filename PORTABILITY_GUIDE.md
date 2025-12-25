# Guía de Portabilidad - uint128_t & int128_t

## 📋 Resumen

Esta biblioteca de enteros de 128 bits está diseñada para ser **portable por diseño**, usando principalmente características estándar de C++20. Sin embargo, incluye optimizaciones específicas de plataforma que se activan automáticamente cuando están disponibles.

## ✅ Estado de Testing por Plataforma

### Plataformas Completamente Testeadas

#### ✅ Windows x86_64 (MSYS2)
**Estado**: 100% Funcional - Todas las features testeadas

| Compilador | Versión | Modo | Features | Estado |
|------------|---------|------|----------|--------|
| GCC | 15.2.0 (UCRT64) | Debug + Release | 13/13 | ✅ Completo |
| Clang | 19.1.5 (CLANG64) | Debug + Release | 13/13 | ✅ Completo |
| Intel ICX | OneAPI 2024 | Release | Básico | ✅ Funcional |
| MSVC | 19.42 (VS 2022) | Debug + Release | 13/13 | ✅ Completo |

**Intrínsecos usados**:
- `_BitScanReverse64()` / `_BitScanForward64()` (MSVC)
- `__builtin_clzll()` / `__builtin_ctzll()` (GCC/Clang)
- `_umul128()` / `_udiv128()` (MSVC)
- `__uint128_t` como backend opcional (GCC/Clang)

**Tests ejecutados**: 108 scripts × 4 compiladores × 2 modos = 864 configuraciones posibles

---

### Plataformas Sin Testear (Debería Funcionar)

#### 📋 Linux x86_64

**Compiladores esperados**:
- GCC 10+ (requiere `-std=c++20`)
- Clang 12+ (requiere `-std=c++20`)
- Intel OneAPI ICX

**Intrínsecos disponibles**: Mismos que Windows x86_64
- `__builtin_clzll()` / `__builtin_ctzll()`
- `__uint128_t` como backend

**Comandos de compilación** (ejemplo):
```bash
# GCC
g++ -std=c++20 -O2 -I./include test.cpp -o test

# Clang
clang++ -std=c++20 -O2 -I./include test.cpp -o test

# Intel
icpx -std=c++20 -O2 -I./include test.cpp -o test
```

**Requisitos de testing**:
- VM con Ubuntu 22.04+ / Debian 12+ / Fedora 38+
- Hardware físico con Linux
- WSL2 (Windows Subsystem for Linux)

---

#### 📋 macOS x86_64

**Compiladores esperados**:
- Clang (Apple toolchain) - Viene con Xcode
- GCC (Homebrew) - `brew install gcc`

**Intrínsecos disponibles**: 
- `__builtin_clzll()` / `__builtin_ctzll()` (Clang/GCC)
- `__uint128_t` como backend (Clang/GCC)

**Comandos de compilación** (ejemplo):
```bash
# Clang (Apple)
clang++ -std=c++20 -O2 -I./include test.cpp -o test

# GCC (Homebrew)
g++-13 -std=c++20 -O2 -I./include test.cpp -o test
```

**Requisitos de testing**:
- Hardware Mac con procesador Intel
- macOS 12 (Monterey) o superior
- Xcode Command Line Tools instaladas

---

#### 📋 macOS ARM64 (Apple Silicon)

**Compiladores esperados**:
- Clang (Apple toolchain) - Optimizado para M1/M2/M3
- GCC (Homebrew) - Con traducción Rosetta 2

**Intrínsecos**:
⚠️ **Diferentes de x86_64**:
- `__builtin_clzll()` está disponible pero usa instrucciones ARM
- `__uint128_t` disponible (implementado nativamente en ARM64)
- Sin `_umul128()` / `_udiv128()` (específicos de MSVC x86)

**Código genérico automático**: La biblioteca detecta la arquitectura y usa código portable cuando los intrínsecos x86 no están disponibles.

**Comandos de compilación**:
```bash
# Clang (Apple Silicon nativo)
clang++ -std=c++20 -O2 -I./include test.cpp -o test -arch arm64

# GCC (Homebrew)
g++-13 -std=c++20 -O2 -I./include test.cpp -o test
```

**Requisitos de testing**:
- Mac con Apple Silicon (M1/M2/M3)
- macOS 12+ (Monterey o superior)

---

#### 📋 Linux ARM 64-bit (aarch64)

**Plataformas típicas**:
- Raspberry Pi 4/5 (64-bit OS)
- NVIDIA Jetson
- AWS Graviton instances
- Emulación QEMU

**Compiladores esperados**:
- GCC 10+ (ARM toolchain)
- Clang 12+

**Intrínsecos ARM**:
```cpp
// Equivalentes ARM de intrínsecos x86
__builtin_clzll()   // Count leading zeros (ARM: CLZ)
__builtin_ctzll()   // Count trailing zeros (ARM: rbit + clz)
__uint128_t         // Disponible en ARM64
```

**Comandos de compilación**:
```bash
# GCC ARM
aarch64-linux-gnu-g++ -std=c++20 -O2 -I./include test.cpp -o test

# O directamente en la plataforma ARM
g++ -std=c++20 -O2 -I./include test.cpp -o test
```

**Requisitos de testing**:
- Hardware ARM64 real (Raspberry Pi 4+)
- Emulación: `qemu-aarch64-static`
- Cross-compilation desde x86_64

---

#### 📋 Linux ARM 32-bit (armv7l)

**Plataformas típicas**:
- Raspberry Pi 3 (32-bit OS)
- Dispositivos embebidos antiguos

**Limitaciones**:
⚠️ **Sin `__uint128_t` nativo** (requiere 64-bit)
- La biblioteca usa emulación completa de 128 bits
- Rendimiento reducido comparado con 64-bit

**Compiladores esperados**:
- GCC 10+ (ARM 32-bit toolchain)
- Clang 12+

**Comandos de compilación**:
```bash
# GCC ARM 32-bit
arm-linux-gnueabihf-g++ -std=c++20 -O2 -I./include test.cpp -o test

# En la plataforma directamente
g++ -std=c++20 -O2 -I./include test.cpp -o test
```

**Requisitos de testing**:
- Hardware ARM 32-bit
- Emulación: `qemu-arm-static`

---

#### 📋 Linux RISC-V 64-bit

**Plataformas típicas**:
- SiFive boards
- Emulación QEMU
- Cloud instances (futuro)

**Limitaciones**:
⚠️ **Sin intrínsecos optimizados**:
- No hay equivalente a `__builtin_clzll` optimizado
- `__uint128_t` puede estar disponible pero sin aceleración
- La biblioteca usa código genérico portable

**Compiladores esperados**:
- GCC 10+ (RISC-V toolchain)
- Clang 12+ (experimental)

**Comandos de compilación**:
```bash
# GCC RISC-V
riscv64-linux-gnu-g++ -std=c++20 -O2 -I./include test.cpp -o test

# Emulación QEMU
qemu-riscv64-static ./test
```

**Requisitos de testing**:
- Emulación: `qemu-riscv64-static`
- Hardware RISC-V (raro)

---

#### 📋 Linux RISC-V 32-bit

**Estado**: Similar a ARM 32-bit
- Sin `__uint128_t` nativo
- Emulación completa de 128 bits
- Rendimiento reducido

**Comandos de compilación**:
```bash
riscv32-linux-gnu-g++ -std=c++20 -O2 -I./include test.cpp -o test
```

---

## 🔧 Detección Automática de Plataforma

La biblioteca detecta automáticamente la plataforma y arquitectura en tiempo de compilación:

```cpp
// En uint128_t.hpp
#if defined(_MSC_VER)
    // Usar intrínsecos MSVC
    #include <intrin.h>
    #define UINT128_USE_MSVC_INTRINSICS
#elif defined(__GNUC__) || defined(__clang__)
    // Usar intrínsecos GCC/Clang
    #if defined(__x86_64__) || defined(_M_X64)
        #define UINT128_USE_X86_INTRINSICS
    #elif defined(__aarch64__) || defined(__arm__)
        #define UINT128_USE_ARM_INTRINSICS
    #else
        // Código genérico portable
        #define UINT128_USE_GENERIC_CODE
    #endif
#endif
```

## 📊 Resumen de Compatibilidad

| Característica | x86_64 | ARM64 | ARM32 | RISC-V 64 | RISC-V 32 |
|----------------|--------|-------|-------|-----------|-----------|
| C++20 estándar | ✅ | ✅ | ✅ | ✅ | ✅ |
| Operaciones básicas | ✅ | ✅ | ✅ | ✅ | ✅ |
| `__uint128_t` backend | ✅ | ✅ | ❌ | ⚠️ | ❌ |
| Intrínsecos optimizados | ✅ | ⚠️ | ❌ | ❌ | ❌ |
| Type traits completos | ✅ | ✅ | ✅ | ✅ | ✅ |
| STL compatibility | ✅ | ✅ | ✅ | ✅ | ✅ |
| Testeado formalmente | ✅ | ❌ | ❌ | ❌ | ❌ |

**Leyenda**:
- ✅ Completamente soportado
- ⚠️ Parcialmente soportado (diferentes instrucciones)
- ❌ No disponible (usa fallback)

## 🧪 Cómo Testear en Otras Plataformas

### Opción 1: Máquinas Virtuales

```bash
# Ubuntu en VirtualBox/VMware
sudo apt install build-essential
g++ -std=c++20 -I./include tests/basic_test.cpp -o test
./test
```

### Opción 2: WSL2 (Windows)

```powershell
# En PowerShell (Windows)
wsl --install -d Ubuntu-22.04

# Dentro de WSL
sudo apt update
sudo apt install build-essential clang
g++ -std=c++20 -I./include tests/basic_test.cpp -o test
./test
```

### Opción 3: Docker

```dockerfile
FROM ubuntu:22.04
RUN apt update && apt install -y build-essential
COPY . /project
WORKDIR /project
RUN g++ -std=c++20 -I./include tests/basic_test.cpp -o test
CMD ["./test"]
```

### Opción 4: GitHub Actions (CI/CD)

Ver archivo `.github/workflows/ci.yml` para configuración automática.

### Opción 5: Emulación QEMU

```bash
# Emular ARM64
sudo apt install qemu-user-static
qemu-aarch64-static ./test_arm64

# Emular RISC-V
qemu-riscv64-static ./test_riscv64
```

## 🚀 Próximos Pasos

### Para Contribuidores

Si tienes acceso a alguna de las plataformas no testeadas:

1. **Clonar el repositorio**
2. **Compilar tests básicos**:
   ```bash
   g++ -std=c++20 -O2 -I./include tests/basic_test.cpp -o test
   ./test
   ```
3. **Reportar resultados**:
   - Plataforma (OS, arquitectura, compilador)
   - Versión del compilador
   - Modo (Debug/Release)
   - Output completo de tests
   - Cualquier error o warning

### Para Usuarios

Si necesitas usar la biblioteca en plataformas no testeadas:

1. **Prueba primero con código simple**
2. **Reporta cualquier issue** específico de la plataforma
3. **Considera contribuir** con resultados de testing

## 📝 Conclusión

**La biblioteca está diseñada para ser portable**, pero solo ha sido formalmente testeada en:
- ✅ **Windows x86_64** con 4 compiladores (GCC, Clang, Intel, MSVC)

**Debería funcionar sin problemas** en:
- 📋 Linux x86_64 (muy probable)
- 📋 macOS x86_64/ARM64 (muy probable)
- 📋 ARM/RISC-V (con rendimiento reducido en código genérico)

**Testing adicional requiere**:
- Infraestructura de VM/hardware
- Tiempo de desarrollo para adaptaciones específicas
- Contribuciones de la comunidad

---

**Última actualización**: 25 de diciembre de 2025
**Estado**: Windows x86_64 completamente testeado y funcional
