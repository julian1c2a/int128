# Guía Rápida: Scripts de Compilación con Argumento de Compilador

## Scripts Actualizados

Ambos scripts ahora soportan un argumento opcional para compilar solo con un compilador específico:

- `compile_uint128_traits_extracted.sh`
- `compile_int128_traits_extracted.sh`

## Uso

### Sintaxis

```bash
./scripts/compile_[uint128|int128]_traits_extracted.sh [gcc|clang|intel|msvc|all]
```

### Ejemplos

#### Compilar solo con GCC
```bash
./scripts/compile_uint128_traits_extracted.sh gcc
./scripts/compile_int128_traits_extracted.sh gcc
```

#### Compilar solo con Clang
```bash
./scripts/compile_uint128_traits_extracted.sh clang
./scripts/compile_int128_traits_extracted.sh clang
```

#### Compilar solo con Intel OneAPI
```bash
./scripts/compile_uint128_traits_extracted.sh intel
./scripts/compile_int128_traits_extracted.sh intel
```

#### Compilar solo con MSVC
```bash
./scripts/compile_uint128_traits_extracted.sh msvc
./scripts/compile_int128_traits_extracted.sh msvc
```

#### Compilar con TODOS los compiladores (por defecto)
```bash
./scripts/compile_uint128_traits_extracted.sh all
./scripts/compile_uint128_traits_extracted.sh     # Sin argumento = all
./scripts/compile_int128_traits_extracted.sh all
./scripts/compile_int128_traits_extracted.sh      # Sin argumento = all
```

## Salida

### Con compilador específico
```
=============================================================
  Compilación de uint128_traits tests y benchmarks
  Compilador: gcc
=============================================================

[GCC] Compilando con GCC (UCRT64)...
  ✅ Tests compilados
  ✅ Benchmarks compilados

=============================================================
  Resumen de Compilación
=============================================================
Compilaciones exitosas: 2 de 2

✅ TODAS LAS COMPILACIONES EXITOSAS
```

### Con todos los compiladores
```
=============================================================
  Compilación de uint128_traits tests y benchmarks
=============================================================

[GCC] Compilando con GCC (UCRT64)...
  ✅ Tests compilados
  ✅ Benchmarks compilados

[CLANG] Compilando con Clang (CLANG64)...
  ✅ Tests compilados
  ✅ Benchmarks compilados

[INTEL] Compilando con Intel OneAPI...
  ✅ Tests compilados
  ✅ Benchmarks compilados

[MSVC] Compilando con MSVC...
  ✅ Tests compilados
  ✅ Benchmarks compilados

=============================================================
  Resumen de Compilación
=============================================================
Compilaciones exitosas: 8 de 8

✅ TODAS LAS COMPILACIONES EXITOSAS
```

## Notas

1. **Case-insensitive**: Los argumentos no distinguen mayúsculas/minúsculas:
   - `gcc`, `GCC`, `Gcc` → todos funcionan igual

2. **Logs**: Los logs de compilación se guardan en `/tmp/`:
   - `/tmp/gcc_tests.log`
   - `/tmp/gcc_benchs.log`
   - `/tmp/clang_tests.log`
   - etc.

3. **Directorios**: Se crean automáticamente todos los directorios necesarios,
   independientemente del compilador seleccionado.

4. **Exit codes**:
   - `0` = Todas las compilaciones exitosas
   - `1` = Al menos una compilación falló

## Ventajas

✅ **Compilación rápida**: Compila solo con el compilador que necesitas
✅ **Iteración rápida**: Ideal para desarrollo y pruebas
✅ **Backward compatible**: Sin argumentos = comportamiento original (compila todo)
✅ **Flexible**: Combina fácilmente con otros scripts

## Ejemplo de Flujo de Trabajo

```bash
# 1. Compilar solo con GCC para pruebas rápidas
./scripts/compile_uint128_traits_extracted.sh gcc

# 2. Ejecutar tests de GCC
./build/build_tests/gcc/release/uint128_traits_tests_gcc.exe

# 3. Si funciona, compilar con todos los compiladores
./scripts/compile_uint128_traits_extracted.sh all

# 4. Ejecutar todos los benchmarks
./scripts/run_uint128_traits_benchmarks.sh
```

## Integración con Scripts Maestros

Los scripts maestros (`master_uint128_traits_all.sh` y `master_int128_traits_all.sh`)
siguen compilando con TODOS los compiladores automáticamente, sin cambios.
