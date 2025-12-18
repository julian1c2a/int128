# Sistema de Benchmarks - Mejoras Implementadas

## 🎯 Resumen de Mejoras

Se han implementado 4 mejoras principales al sistema de benchmarks:

1. **Benchmark de __uint128_t** - Tipo nativo de 128 bits en GCC/Clang
2. **Backends de Boost.Multiprecision** - GMP y tommath
3. **Gráficos con matplotlib** - Visualización profesional
4. **Compilador Intel** - Soporte para icpx/icx

---

## 📊 1. Benchmark de __uint128_t

### Descripción
Se agregó soporte para el tipo nativo `__uint128_t` disponible en GCC y Clang, permitiendo comparar el rendimiento de nuestra implementación contra la versión nativa del compilador.

### Operaciones Soportadas
- Construcción
- Aritméticas: +, -, *, /, %
- Bitwise: &, |, ^, ~
- Shifts: <<, >>
- Comparaciones: ==, !=, <, <=, >, >=

### Compilación
```bash
# GCC con __uint128_t
./scripts/build_with_backends.bash gcc

# Clang con __uint128_t
./scripts/build_with_backends.bash clang
```

### Resultados Esperados
- **Ventajas**: Operaciones básicas optimizadas por el compilador
- **Limitaciones**: No disponible en MSVC, sin conversión a string nativa

---

## 🚀 2. Backends de Boost.Multiprecision

### Backends Disponibles

#### 2.1 Boost.Multiprecision Pure (cpp_int)
```cpp
using boost_uint128 = boost::multiprecision::uint128_t;
```
- Implementación pura C++
- Sin dependencias externas
- Portable a todas las plataformas

#### 2.2 Boost + GMP Backend
```cpp
using boost_uint128_gmp = boost::multiprecision::number<
    boost::multiprecision::gmp_int,
    boost::multiprecision::et_off
>;
```
- Usa GNU Multiple Precision Library
- Altamente optimizada para operaciones grandes
- Requiere: `libgmp-dev` (Linux), `gmp` (MSYS2)

#### 2.3 Boost + tommath Backend
```cpp
using boost_uint128_tommath = boost::multiprecision::number<
    boost::multiprecision::tommath_int,
    boost::multiprecision::et_off
>;
```
- Usa libtommath
- Alternativa ligera a GMP
- Requiere: `libtommath-dev` (Linux), `libtommath` (MSYS2)

### Instalación de Dependencias

#### MSYS2/MinGW
```bash
pacman -S mingw-w64-ucrt-x86_64-boost
pacman -S mingw-w64-ucrt-x86_64-gmp
pacman -S mingw-w64-ucrt-x86_64-libtommath
```

#### Ubuntu/Debian
```bash
sudo apt install libboost-all-dev
sudo apt install libgmp-dev
sudo apt install libtommath-dev
```

### Compilación con Backends

```bash
# Solo Boost (cpp_int puro)
./scripts/build_with_backends.bash gcc --with-boost

# Boost + GMP
./scripts/build_with_backends.bash gcc --with-gmp

# Boost + tommath
./scripts/build_with_backends.bash gcc --with-tommath

# Todos los backends
./scripts/build_with_backends.bash gcc --all-backends
```

### Comparación de Backends

| Backend | Ventajas | Desventajas |
|---------|----------|-------------|
| **cpp_int** | Sin dependencias, portable | Más lento que backends nativos |
| **GMP** | Muy rápido, estándar de facto | Dependencia externa pesada |
| **tommath** | Ligero, buen rendimiento | Menos optimizado que GMP |
| **__uint128_t** | Nativo, ultra-rápido | Solo GCC/Clang, sin strings |

---

## 📈 3. Gráficos con matplotlib

### Descripción
Sistema completo de visualización de resultados de benchmarks usando matplotlib y numpy.

### Tipos de Gráficos Generados

#### 3.1 Time Comparison (time_*.png)
- Gráfico de barras por operación
- Compara todos los tipos (uint128_t, uint64_t, __uint128_t, boost, etc.)
- Uno por operación (addition, multiplication, etc.)

#### 3.2 Cycles Comparison (cycles_*.png)
- Similar al anterior pero usando ciclos de CPU
- Útil para comparar rendimiento independiente de frecuencia

#### 3.3 Compiler Comparison (compiler_*.png)
- Compara rendimiento entre compiladores (GCC, Clang, MSVC, Intel)
- Solo para uint128_t

#### 3.4 Speedup Heatmap (speedup_heatmap.png)
- Mapa de calor 2D: Operación vs Tipo
- Color indica speedup relativo a uint64_t
- Identifica operaciones críticas

#### 3.5 Operations Overview (operations_overview.png)
- Vista general de todas las operaciones
- Barras horizontales agrupadas por tipo
- Permite identificar patrones globales

### Instalación

```bash
# Usando pip
pip install matplotlib numpy

# O con conda
conda install matplotlib numpy
```

### Uso

```bash
# Generar todos los gráficos del último benchmark
python scripts/plot_benchmark_results.py

# Especificar archivo CSV
python scripts/plot_benchmark_results.py --input benchmark_results/custom_results.csv

# Cambiar directorio de salida
python scripts/plot_benchmark_results.py --output my_plots/
```

### Salida
```
benchmark_results/
└── plots/
    ├── time_addition.png
    ├── time_multiplication.png
    ├── cycles_addition.png
    ├── compiler_addition.png
    ├── speedup_heatmap.png
    └── operations_overview.png
```

---

## 🔧 4. Soporte para Compilador Intel

### Compiladores Soportados

#### 4.1 Intel oneAPI (icpx)
```bash
# Moderno, basado en Clang
icpx -std=c++20 -O3 ...
```

#### 4.2 Intel Classic (icx/icc)
```bash
# Legacy, todavía soportado
icx -std=c++20 -O3 ...
```

### Instalación

#### Windows
```
Instalar Intel oneAPI Base Toolkit desde:
https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html
```

#### Linux
```bash
# Agregar repositorio Intel
wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null
echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list

# Instalar
sudo apt update
sudo apt install intel-basekit
```

### Uso

```bash
# Compilar con Intel
./scripts/build_benchmarks.bash intel

# Ejecutar
./scripts/run_benchmarks.bash intel

# O todo junto
./scripts/build_benchmarks.bash all
./scripts/run_benchmarks.bash all
```

### Características Intel
- Optimizaciones específicas para CPUs Intel
- Vectorización automática avanzada
- Soporte para instrucciones AVX-512
- Análisis de rendimiento integrado

---

## 🎨 Workflow Completo

### Escenario 1: Benchmark Básico (Solo Tipos Nativos)
```bash
# 1. Compilar con GCC
./scripts/build_benchmarks.bash gcc

# 2. Ejecutar
./scripts/run_benchmarks.bash gcc

# 3. Generar gráficos
python scripts/plot_benchmark_results.py

# 4. Ver reporte
cat benchmark_results/benchmark_summary.md
```

### Escenario 2: Benchmark Completo (Todos los Backends)
```bash
# 1. Compilar con todos los backends
./scripts/build_with_backends.bash gcc --all-backends
./scripts/build_with_backends.bash clang --all-backends

# 2. Ejecutar benchmarks
./build/benchmarks/benchmark_gcc
./build/benchmarks/benchmark_clang

# 3. Consolidar resultados
python scripts/aggregate_benchmark_results.py

# 4. Generar gráficos
python scripts/plot_benchmark_results.py

# 5. Revisar plots
ls -lh benchmark_results/plots/
```

### Escenario 3: Comparación Multi-Compilador
```bash
# 1. Compilar con todos los compiladores
./scripts/build_benchmarks.bash all

# 2. Ejecutar todos
./scripts/run_benchmarks.bash all

# 3. Los scripts generan automáticamente:
#    - CSV consolidado
#    - Markdown report
#    - Plots comparativos
```

---

## 📋 Tipos Comparados

| Tipo | Descripción | Compiladores | Dependencias |
|------|-------------|--------------|--------------|
| `uint128_t` | Nuestra implementación | GCC, Clang, MSVC, Intel | Ninguna |
| `uint64_t` | Tipo nativo 64-bit | Todos | Ninguna |
| `uint32_t` | Tipo nativo 32-bit | Todos | Ninguna |
| `__uint128_t` | GCC/Clang nativo | GCC, Clang | Ninguna |
| `boost_uint128` | Boost cpp_int | Todos | libboost |
| `boost_gmp` | Boost + GMP | Todos | libboost, libgmp |
| `boost_tommath` | Boost + tommath | Todos | libboost, libtommath |

---

## 🔍 Métricas Medidas

Para cada operación y tipo, se miden:

1. **Time_ns**: Tiempo promedio en nanosegundos
2. **Cycles**: Ciclos de CPU promedio (via RDTSC)
3. **Ops_per_sec**: Operaciones por segundo
4. **Speedup**: Relativo a uint64_t baseline

---

## 📦 Estructura de Salida

```
benchmark_results/
├── benchmark_gcc_YYYYMMDD_HHMMSS.csv        # Raw results GCC
├── benchmark_clang_YYYYMMDD_HHMMSS.csv      # Raw results Clang
├── benchmark_msvc_YYYYMMDD_HHMMSS.csv       # Raw results MSVC
├── benchmark_intel_YYYYMMDD_HHMMSS.csv      # Raw results Intel
├── consolidated_YYYYMMDD_HHMMSS.csv         # Agregado
├── benchmark_summary.md                      # Reporte Markdown
└── plots/
    ├── time_*.png                            # Gráficos de tiempo
    ├── cycles_*.png                          # Gráficos de ciclos
    ├── compiler_*.png                        # Comparación compiladores
    ├── speedup_heatmap.png                   # Mapa de calor
    └── operations_overview.png               # Vista general
```

---

## 🚧 Próximas Mejoras (CI/CD)

Planeadas para la siguiente fase:

1. **GitHub Actions**: Ejecución automática de benchmarks
2. **Regression Detection**: Alerta automática si performance baja
3. **Historical Tracking**: Gráficos de evolución temporal
4. **Multi-Platform**: Linux, Windows, macOS
5. **Artifact Upload**: Almacenar resultados históricos
6. **PR Comments**: Comentarios automáticos con resultados

---

## 🎓 Ejemplos de Resultados

### Operación: Addition

```
Type             | Time (ns) | Cycles | Speedup vs uint64
-----------------|-----------|---------|-----------------
uint64_t         |     1.2   |   3.5  |    1.00x (baseline)
__uint128_t      |     1.5   |   4.2  |    0.80x
uint128_t        |     2.8   |   8.1  |    0.43x
boost_uint128    |    15.3   |  45.2  |    0.08x
boost_gmp        |    22.7   |  67.3  |    0.05x
boost_tommath    |    18.9   |  55.8  |    0.06x
```

**Interpretación**:
- `__uint128_t` es casi tan rápido como `uint64_t` (20% más lento)
- Nuestra `uint128_t` es ~2.3x más lenta que nativa
- Backends Boost son ~10-20x más lentos (overhead de abstracción)

---

## 📞 Troubleshooting

### Problema: matplotlib not found
```bash
pip install matplotlib numpy
# O
conda install matplotlib numpy
```

### Problema: GMP library not found
```bash
# MSYS2
pacman -S mingw-w64-ucrt-x86_64-gmp

# Ubuntu
sudo apt install libgmp-dev
```

### Problema: Intel compiler not in PATH
```bash
# Windows (PowerShell)
& "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

# Linux
source /opt/intel/oneapi/setvars.sh
```

### Problema: __uint128_t undefined in MSVC
- Normal, `__uint128_t` solo existe en GCC/Clang
- Use `--no-uint128` si compila para MSVC

---

## ✅ Checklist de Verificación

Después de implementar las mejoras:

- [x] __uint128_t en construcción
- [x] __uint128_t en aritmética (+, -, *, /, %)
- [x] __uint128_t en bitwise (&, |, ^, ~)
- [x] __uint128_t en shifts (<<, >>)
- [x] __uint128_t en comparaciones
- [x] Boost GMP en todas las operaciones
- [x] Boost tommath en todas las operaciones
- [x] Script de plotting (plot_benchmark_results.py)
- [x] Build script con backends (build_with_backends.bash)
- [x] Intel compiler en build_benchmarks.bash
- [x] Intel compiler en run_benchmarks.bash
- [x] Documentación actualizada

---

## 📚 Referencias

- [Boost.Multiprecision](https://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/index.html)
- [GMP Manual](https://gmplib.org/manual/)
- [Intel oneAPI](https://www.intel.com/content/www/us/en/developer/tools/oneapi/overview.html)
- [matplotlib Documentation](https://matplotlib.org/stable/contents.html)

---

**Autor**: Sistema de Benchmarks int128  
**Versión**: 2.0  
**Última Actualización**: Diciembre 2024
