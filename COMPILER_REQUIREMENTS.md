# Requerimientos de Compiladores por Plataforma

## Matriz de Compatibilidad Actual

### Windows x86_64 (TESTEADO ✅)

| Compilador | Versión | Flags Requeridos | Estado |
|------------|---------|------------------|--------|
| GCC (UCRT64) | 15.2+ | `-std=c++20` | ✅ |
| Clang (clang64) | 19.1+ | `-std=c++20 -static` | ✅ |
| Intel icx | 2025.3+ | `/std:c++20 /EHsc` + vcvars64 | ✅ |
| MSVC cl | 19.50+ | `/std:c++20 /EHsc` | ✅ |

### Linux x86_64 via WSL (TESTEADO ✅)

| Compilador | Versión | Flags Requeridos | Estado |
|------------|---------|------------------|--------|
| g++-13 | 13.x | `-std=c++20` | ✅ |
| g++-14 | 14.x | `-std=c++20` | ✅ |
| g++-15 | 15.x | `-std=c++20 -static` | ✅ |
| clang++-18 | 18.x | `-std=c++20` | ✅ |
| clang++-19 | 19.x | `-std=c++20` | ✅ |
| clang++-20 | 20.x | `-std=c++20` | ✅ |
| clang++-21 | 21.x | `-std=c++20` | ✅ |
| Intel icpx | 2025.3+ | `-std=c++20` + setvars.sh | ✅ |

### ARM64 Linux (PENDIENTE 🔄)

| Compilador | Versión | Notas |
|------------|---------|-------|
| aarch64-linux-gnu-g++ | 12+ | Cross-compiler |
| aarch64-linux-gnu-clang++ | 15+ | Cross-compiler |

### ARM64 Windows (PENDIENTE 🔄)

| Compilador | Versión | Notas |
|------------|---------|-------|
| MSVC ARM64 | VS 2022+ | Requiere Windows on ARM SDK |
| Clang ARM64 | 15+ | Requiere LLVM for Windows ARM |

### RISC-V 64 Linux (PENDIENTE 🔄)

| Compilador | Versión | Notas |
|------------|---------|-------|
| riscv64-linux-gnu-g++ | 12+ | Cross-compiler |
| riscv64-linux-gnu-clang++ | 15+ | Cross-compiler |

---

## Cross-Compilers a Instalar

### En WSL Ubuntu

```bash
# ARM64 cross-compilers
sudo apt install g++-aarch64-linux-gnu
sudo apt install clang  # Usar --target=aarch64-linux-gnu

# RISC-V 64 cross-compilers
sudo apt install g++-riscv64-linux-gnu
# Clang puede usar --target=riscv64-linux-gnu
```

### En MSYS2 Windows

```bash
# ARM64 (experimental en MSYS2)
# No hay soporte oficial de MSYS2 para ARM64 cross-compilation
# Usar Clang con --target=aarch64-windows-msvc (requiere Windows SDK ARM64)
```

---

## Comandos de Cross-Compilación

### ARM64 Linux (desde WSL)

```bash
# GCC
aarch64-linux-gnu-g++ -std=c++20 -O2 -I. test.cpp -o test_arm64

# Clang
clang++ -std=c++20 -O2 --target=aarch64-linux-gnu -I. test.cpp -o test_arm64

# Ejecutar con QEMU
qemu-aarch64 -L /usr/aarch64-linux-gnu ./test_arm64
```

### RISC-V 64 Linux (desde WSL)

```bash
# GCC
riscv64-linux-gnu-g++ -std=c++20 -O2 -I. test.cpp -o test_riscv64

# Clang
clang++ -std=c++20 -O2 --target=riscv64-linux-gnu -I. test.cpp -o test_riscv64

# Ejecutar con QEMU
qemu-riscv64 -L /usr/riscv64-linux-gnu ./test_riscv64
```

---

## Diferencias de Intrínsecos por Arquitectura

### x86_64

- `_BitScanReverse64` / `__builtin_clzll` - Count leading zeros
- `_BitScanForward64` / `__builtin_ctzll` - Count trailing zeros
- `_umul128` / `__uint128_t` - 128-bit multiplication
- `_udiv128` - 128-bit division (MSVC only)

### ARM64

- `__builtin_clzll` - Disponible
- `__builtin_ctzll` - Disponible  
- `__uint128_t` - Disponible en GCC/Clang
- NO hay `_umul128` - Usar código genérico o `__uint128_t`

### RISC-V 64

- `__builtin_clzll` - Disponible (puede ser lento)
- `__builtin_ctzll` - Disponible
- `__uint128_t` - Disponible en GCC/Clang recientes
- Sin intrínsecos optimizados - Usar código genérico

---

## Restricciones del Proyecto

### Obligatorias (MUST)

1. C++20 o superior (`-std=c++20` o `/std:c++20`)
2. Soporte para `constexpr` extensivo
3. Soporte para concepts C++20

### Recomendadas (SHOULD)

1. `__uint128_t` disponible para mejor rendimiento
2. Intrínsecos de bits (`clz`, `ctz`) para optimización
3. Soporte para `std::bit_cast` (C++20)

### Opcionales (MAY)

1. Intrínsecos específicos de arquitectura
2. SIMD (futuro)
3. Atomics de 128 bits

---

## Plan de Testing por Arquitectura

### Fase 1: x86_64 (COMPLETADA ✅)

- Windows: GCC, Clang, MSVC, Intel icx
- Linux WSL: GCC 13-15, Clang 18-21, Intel icpx

### Fase 2: ARM64 (PRÓXIMA)

- Cross-compile desde WSL
- Ejecutar con QEMU
- Verificar código genérico funciona

### Fase 3: RISC-V 64 (FUTURA)

- Cross-compile desde WSL
- Ejecutar con QEMU
- Puede requerir ajustes de código genérico
