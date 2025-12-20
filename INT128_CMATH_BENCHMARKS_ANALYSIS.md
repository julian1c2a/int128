# INT128_CMATH.HPP - ANÁLISIS DE BENCHMARKS

**Fecha:** 20 de diciembre de 2025  
**Compiladores:** GCC 15.2.0, Clang 21.1.7, MSVC 19.50, Intel oneAPI 2025.3.0  
**Optimización:** GCC/Clang/Intel: `-O3`, MSVC: `/O2`  
**Medición:** RDTSC (ciclos CPU) + `std::chrono::high_resolution_clock` (nanosegundos)

---

## 📊 RESUMEN EJECUTIVO

### Mejor Compilador por Categoría

| Categoría | Ganador | Rendimiento Típico | Observaciones |
|-----------|---------|-------------------|---------------|
| **GCD** | GCC | 1-11 ns (3-28 ciclos) | Optimización agresiva, 10x más rápido |
| **LCM** | GCC | 6-9 ns (20-29 ciclos) | 3-4x más rápido que otros |
| **ABS** | **EMPATE (4)** | 0-0.5 ns | Todos optimizan a constante |
| **POW** | GCC | 0 ns (0 ciclos) | Completamente optimizado |
| **SQRT** | GCC | 0 ns (0 ciclos) | Newton altamente optimizado |
| **SIGN** | GCC/Clang/Intel | 0 ns (0 ciclos) | MSVC: 0.7-0.9 ns |
| **MIN/MAX** | GCC/Clang/Intel | 0 ns (0 ciclos) | MSVC: 1.1 ns |
| **CLAMP** | GCC/Clang/Intel | 0 ns (0 ciclos) | MSVC: 5-8 ns |
| **MIDPOINT** | GCC | 1.3-1.8 ns (3-5 ciclos) | 10-16x más rápido |
| **COMBINED** | GCC | 0-14 ns | Mejor fusión de operaciones |

### Veredicto General

🥇 **GCC 15.2.0**: Dominante absoluto, optimización excepcional  
🥈 **Clang 21.1.7**: Segundo lugar, consistente  
🥈 **Intel oneAPI 2025.3.0**: Similar a Clang (basado en LLVM)  
🥉 **MSVC 19.50**: Rendimiento aceptable, margen de mejora en operaciones simples

---

## 📈 ANÁLISIS DETALLADO POR FUNCIÓN

### 1. GCD (Greatest Common Divisor)

**Algoritmo:** Binary GCD (Stein's algorithm) con manejo de signos  
**Complejidad:** O(log(min(a,b)))

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| gcd(12, 18) - small | 2.5 (7c) | 7.4 (22c) | 12.0 (31c) | 8.0 (23c) | **GCC** 3x |
| gcd(1e9, 5e8) - large | 1.1 (3c) | 6.5 (20c) | 10.0 (28c) | 7.1 (23c) | **GCC** 6x |
| gcd(-48, 18) - neg first | 2.9 (8c) | 7.6 (23c) | 10.5 (32c) | 8.0 (24c) | **GCC** 2.6x |
| gcd(48, -18) - neg second | 5.1 (9c) | 7.6 (23c) | 10.3 (31c) | 8.2 (24c) | **GCC** 1.5x |
| gcd(-48, -18) - both neg | 3.1 (9c) | 7.8 (24c) | 10.8 (33c) | 8.2 (25c) | **GCC** 2.5x |
| gcd(999983, 1000003) - coprimes | 10.9 (28c) | 23.2 (72c) | 31.1 (96c) | 23.3 (74c) | **GCC** 2.1x |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **4.1 (12c)** | **0.0 (0c)** | GCC/Clang/Intel |

**Análisis:**
- GCC domina en todos los escenarios (1-11 ns)
- Manejo de negativos añade overhead mínimo (0-3 ns en GCC)
- Co-primos (peor caso) muestran ventaja de GCC más claramente
- MSVC no optimiza int64_t baseline (4.1 ns vs 0 ns)

---

### 2. LCM (Least Common Multiple)

**Algoritmo:** `abs(a*b)/gcd(a,b)` - previene overflow  
**Complejidad:** O(log(min(a,b))) + O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| lcm(12, 18) - small | 9.4 (29c) | 30.6 (133c) | 45.7 (143c) | 32.6 (101c) | **GCC** 3.3x |
| lcm(1000, 2000) - medium | 6.5 (20c) | 27.5 (87c) | 27.3 (91c) | 26.4 (140c) | **GCC** 4.2x |
| lcm(-12, 18) - neg first | 9.4 (29c) | 29.7 (92c) | 45.1 (141c) | 32.0 (115c) | **GCC** 3.2x |
| lcm(-12, -18) - both neg | 9.4 (29c) | 31.1 (81c) | 42.0 (130c) | 33.1 (88c) | **GCC** 3.3x |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **5.5 (12c)** | **0.0 (0c)** | GCC/Clang/Intel |

**Análisis:**
- GCC consistentemente 3-4x más rápido (6-9 ns)
- Clang/Intel similares (26-33 ns)
- MSVC más lento (27-46 ns), variabilidad alta
- Negativos no afectan rendimiento (overhead de `abs` es 0)

---

### 3. ABS (Absolute Value)

**Algoritmo:** Comparación con negación condicional  
**Complejidad:** O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| abs(42) - positive | 0.0 (0c) | 0.0 (0c) | 0.5 (1c) | 0.0 (0c) | **EMPATE** |
| abs(-42) - negative | 0.0 (0c) | 0.0 (0c) | 0.5 (1c) | 0.0 (0c) | **EMPATE** |
| abs(-1e9) - large neg | 0.0 (0c) | 0.0 (0c) | 0.5 (1c) | 0.0 (0c) | **EMPATE** |
| abs(0) - zero | 0.0 (0c) | 0.0 (0c) | 0.5 (1c) | 0.0 (0c) | **EMPATE** |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **EMPATE** |

**Análisis:**
- GCC/Clang/Intel: completamente optimizado (0 ciclos)
- MSVC: pequeño overhead (0.5 ns, 1 ciclo)
- Todos tratan int128_t como int64_t en términos de rendimiento

---

### 4. POW (Power / Exponentiation)

**Algoritmo:** Exponenciación rápida por cuadrados con soporte de base negativa  
**Complejidad:** O(log(exp))

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| pow(2, 3) - small | 0.0 (0c) | 9.4 (29c) | 20.6 (63c) | 9.8 (29c) | **GCC** ∞ |
| pow(2, 10) - medium exp | 0.0 (0c) | 11.1 (34c) | 27.2 (85c) | 11.5 (35c) | **GCC** ∞ |
| pow(-2, 3) - neg base, odd | 0.0 (0c) | 10.9 (33c) | 22.4 (69c) | 11.2 (34c) | **GCC** ∞ |
| pow(-2, 4) - neg base, even | 0.0 (0c) | 9.6 (29c) | 22.3 (73c) | 10.1 (30c) | **GCC** ∞ |
| pow(n, 0) - exp zero | 0.0 (0c) | 5.7 (17c) | 10.2 (24c) | 5.9 (18c) | **GCC** ∞ |
| pow(n, 1) - exp one | 0.0 (0c) | 4.8 (14c) | 6.5 (20c) | 6.3 (17c) | **GCC** ∞ |
| pow(1, 100) - base one | 0.0 (0c) | 5.9 (18c) | 7.9 (24c) | 6.2 (19c) | **GCC** ∞ |

**Análisis:**
- **GCC: Optimización espectacular** - todos los casos evaluados en tiempo de compilación
- Clang/Intel similares: 5-11 ns
- MSVC más lento: 7-27 ns
- Fast paths (exp 0/1, base 1) bien optimizados en Clang/Intel

---

### 5. SQRT (Square Root)

**Algoritmo:** Newton's method para valores no negativos  
**Complejidad:** O(log(n))

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| sqrt(4) - small perfect | 0.0 (0c) | 43.3 (131c) | 112.3 (337c) | 43.1 (134c) | **GCC** ∞ |
| sqrt(100) - medium perfect | 0.0 (0c) | 73.4 (228c) | 187.5 (581c) | 78.5 (244c) | **GCC** ∞ |
| sqrt(10000) - large perfect | 0.0 (0c) | 82.2 (254c) | 202.8 (611c) | 93.1 (291c) | **GCC** ∞ |
| sqrt(99) - non-perfect | 0.0 (0c) | 97.0 (299c) | 243.4 (765c) | 104.2 (323c) | **GCC** ∞ |
| sqrt(1e6) - large non-perf | 0.0 (0c) | 62.4 (192c) | 180.2 (587c) | 68.1 (213c) | **GCC** ∞ |
| sqrt(0) - zero | 0.0 (0c) | 2.9 (8c) | 9.5 (29c) | 3.2 (9c) | **GCC** ∞ |
| sqrt(1) - one | 0.0 (0c) | 6.3 (19c) | 9.5 (29c) | 6.6 (20c) | **GCC** ∞ |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **EMPATE** |

**Análisis:**
- **GCC: Optimización completa** - todos los casos constexpr
- Clang/Intel: 3-104 ns (buenos para Newton iterativo)
- MSVC: 9-243 ns (2-3x más lento que Clang/Intel)
- Casos especiales (0, 1) bien optimizados en todos excepto MSVC

---

### 6. SIGN (Sign Function)

**Algoritmo:** Comparaciones (< 0 ? -1 : > 0 ? 1 : 0)  
**Complejidad:** O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| sign(42) - positive | 0.0 (0c) | 0.0 (0c) | 0.9 (2c) | 0.0 (0c) | **EMPATE** |
| sign(-42) - negative | 0.0 (0c) | 0.0 (0c) | 0.8 (2c) | 0.0 (0c) | **EMPATE** |
| sign(0) - zero | 0.0 (0c) | 0.0 (0c) | 0.7 (2c) | 0.0 (0c) | **EMPATE** |
| sign(1e9) - large pos | 0.0 (0c) | 0.0 (0c) | 0.9 (2c) | 0.0 (0c) | **EMPATE** |
| sign(-1e9) - large neg | 0.0 (0c) | 0.0 (0c) | 0.7 (2c) | 0.0 (0c) | **EMPATE** |

**Análisis:**
- GCC/Clang/Intel: completamente optimizado (0 ciclos)
- MSVC: pequeño overhead (0.7-0.9 ns, 2 ciclos)
- Operación trivial, excelente para todos los compiladores

---

### 7. MIN/MAX (Minimum/Maximum)

**Algoritmo:** Comparación condicional  
**Complejidad:** O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| min(3, 5) - small pos | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| max(3, 5) - small pos | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| min(-3, 5) - mixed signs | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| max(-3, 5) - mixed signs | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| min(-5, -3) - both neg | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| max(-5, -3) - both neg | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| min(5, 5) - equal | 0.0 (0c) | 0.0 (0c) | 1.1 (3c) | 0.0 (0c) | **EMPATE** |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **EMPATE** |

**Análisis:**
- GCC/Clang/Intel: perfectamente optimizado (0 ciclos)
- MSVC: overhead constante (1.1 ns, 3 ciclos)
- Signos no afectan rendimiento
- MSVC optimiza int64_t pero no int128_t

---

### 8. CLAMP (Clamp to Range)

**Algoritmo:** `min(max(value, lo), hi)`  
**Complejidad:** O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| clamp(5, 0, 10) - within | 0.0 (0c) | 0.0 (0c) | 5.6 (17c) | 0.0 (0c) | **EMPATE** |
| clamp(-5, 0, 10) - below min | 0.0 (0c) | 0.0 (0c) | 5.6 (42c) | 0.0 (0c) | **EMPATE** |
| clamp(15, 0, 10) - above max | 0.0 (0c) | 0.0 (0c) | 5.4 (16c) | 0.0 (0c) | **EMPATE** |
| clamp(0, 0, 10) - at min | 0.0 (0c) | 0.0 (0c) | 5.4 (16c) | 0.0 (0c) | **EMPATE** |
| clamp(10, 0, 10) - at max | 0.0 (0c) | 0.0 (0c) | 4.7 (22c) | 0.0 (0c) | **EMPATE** |
| clamp(-5, -10, -1) - neg range | 0.0 (0c) | 0.0 (0c) | 7.9 (22c) | 0.0 (0c) | **EMPATE** |

**Análisis:**
- GCC/Clang/Intel: completamente optimizado (0 ciclos)
- MSVC: overhead consistente (5-8 ns, 16-42 ciclos)
- Variabilidad en ciclos de MSVC interesante (42 ciclos para `below min`)
- Fusión perfecta de min/max en GCC/Clang/Intel

---

### 9. MIDPOINT (Overflow-safe Midpoint)

**Algoritmo:** `a + (b-a)/2` - previene overflow  
**Complejidad:** O(1)

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| midpoint(0, 10) - small | 1.8 (4c) | 18.4 (56c) | 31.6 (98c) | 19.4 (59c) | **GCC** 10x |
| midpoint(100, 200) - medium | 1.8 (5c) | 20.8 (61c) | 31.5 (97c) | 19.7 (60c) | **GCC** 11x |
| midpoint(-10, 10) - across zero | 1.8 (5c) | 18.5 (57c) | 31.5 (98c) | 18.8 (58c) | **GCC** 10x |
| midpoint(-20, -10) - both neg | 1.8 (5c) | 19.4 (60c) | 31.5 (101c) | 20.2 (60c) | **GCC** 10x |
| midpoint(1e9, 2e9) - large | 1.8 (5c) | 19.4 (60c) | 30.7 (94c) | 19.6 (60c) | **GCC** 10x |
| midpoint(5, 5) - equal | 1.3 (3c) | 8.9 (27c) | 7.3 (22c) | 8.2 (26c) | **GCC** 5.6x |
| **int64_t baseline** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **0.0 (0c)** | **EMPATE** |

**Análisis:**
- GCC dominante: 1.3-1.8 ns (3-5 ciclos)
- Clang/Intel similares: 9-21 ns (10x más lentos que GCC)
- MSVC: 7-32 ns (16x más lento que GCC en general)
- Caso especial (valores iguales) optimizado mejor en MSVC que Clang/Intel

---

### 10. COMBINED OPERATIONS

**Algoritmo:** Composición de múltiples operaciones  
**Complejidad:** Varía según composición

| Operación | GCC (ns) | Clang (ns) | MSVC (ns) | Intel (ns) | Ganador |
|-----------|----------|------------|-----------|------------|---------|
| gcd + lcm combined | 14.1 (41c) | 30.8 (131c) | 46.4 (144c) | 33.2 (101c) | **GCC** 2.2x |
| abs(a - b) - abs diff | 0.0 (0c) | 0.0 (0c) | 0.5 (1c) | 0.0 (0c) | **EMPATE** |
| sign(a) * abs(a) - recons | 0.0 (0c) | 0.0 (0c) | 9.2 (28c) | 0.0 (0c) | **EMPATE** |
| clamp(midpoint(a,b), lo, hi) | 2.5 (7c) | 24.1 (59c) | 33.3 (103c) | 20.1 (61c) | **GCC** 9.6x |

**Análisis:**
- **gcd+lcm:** GCC 2.2x más rápido, buena fusión
- **abs(a-b):** Optimización completa excepto MSVC (0.5 ns)
- **sign*abs:** MSVC con overhead (9.2 ns), otros perfectos
- **clamp(midpoint):** GCC 8-13x más rápido, excelente fusión de operaciones

---

## 🔬 ANÁLISIS DE OPTIMIZACIÓN

### Patrón de Optimización por Compilador

#### GCC 15.2.0 🥇
- **Fortaleza:** Evaluación constexpr agresiva
- **Evaluación en tiempo de compilación:** POW, SQRT completos
- **Fusión de operaciones:** Excelente (midpoint, clamp)
- **Operaciones simples:** Perfectas (0 ciclos)
- **Operaciones complejas:** Líderes (GCD 1-11 ns)

#### Clang 21.1.7 🥈
- **Fortaleza:** Consistencia y predictibilidad
- **Operaciones simples:** Perfectas (0 ciclos)
- **Operaciones iterativas:** Buenos (sqrt 43-97 ns, pow 5-11 ns)
- **Fusión de operaciones:** Buena (no al nivel de GCC)
- **int64_t baselines:** Optimización perfecta

#### Intel oneAPI 2025.3.0 🥈
- **Fortaleza:** Basado en LLVM, similar a Clang
- **Rendimiento:** ±5% de Clang en la mayoría de casos
- **Operaciones simples:** Perfectas (0 ciclos)
- **Diferencias con Clang:** Mínimas (±1-2 ns típicamente)

#### MSVC 19.50 🥉
- **Fortaleza:** Compilación rápida, integración VS
- **Debilidad:** Optimización de operaciones simples
- **Overhead constante:** 0.5-1.1 ns en ABS/SIGN/MIN/MAX
- **Operaciones complejas:** 2-3x más lento que GCC/Clang
- **Oportunidades:** Mejorar constexpr evaluation, fusión de operaciones

---

## 📉 OVERHEAD DE MANEJO DE SIGNOS

### Comparación int128_t vs int64_t Baseline

| Función | GCC Overhead | Clang Overhead | MSVC Overhead | Intel Overhead |
|---------|--------------|----------------|---------------|----------------|
| **GCD** | 2.5 ns vs 0 ns | 7.4 ns vs 0 ns | 12.0 ns vs 4.1 ns | 8.0 ns vs 0 ns |
| **LCM** | 6.5 ns vs 0 ns | 27.5 ns vs 0 ns | 27.3 ns vs 5.5 ns | 26.4 ns vs 0 ns |
| **ABS** | 0 ns vs 0 ns | 0 ns vs 0 ns | 0.5 ns vs 0 ns | 0 ns vs 0 ns |
| **SQRT** | 0 ns vs 0 ns | 43.3 ns vs 0 ns | 112.3 ns vs 0 ns | 43.1 ns vs 0 ns |
| **MIN** | 0 ns vs 0 ns | 0 ns vs 0 ns | 1.1 ns vs 0 ns | 0 ns vs 0 ns |
| **MIDPOINT** | 1.8 ns vs 0 ns | 18.4 ns vs 0 ns | 31.6 ns vs 0 ns | 19.4 ns vs 0 ns |

**Observaciones:**
- **GCC:** Overhead mínimo o nulo (0-6.5 ns)
- **Clang/Intel:** Overhead moderado en operaciones complejas (7-43 ns)
- **MSVC:** Overhead más alto, especialmente en SQRT (112 ns)
- **Conclusión:** Manejo de signos bien optimizado en GCC, aceptable en Clang/Intel

---

## 🎯 RECOMENDACIONES

### Por Tipo de Aplicación

#### Aplicaciones de Alto Rendimiento
**Compilador:** GCC 15.2.0  
**Razón:** Optimización superior en todas las categorías  
**Ideal para:** Matemáticas intensivas, criptografía, simulaciones

#### Desarrollo Multiplataforma
**Compilador:** Clang 21.1.7 o Intel oneAPI 2025.3.0  
**Razón:** Consistencia, buenos resultados, disponibilidad  
**Ideal para:** Aplicaciones cross-platform, herramientas

#### Ecosistema Windows/Visual Studio
**Compilador:** MSVC 19.50 (con awareness de limitaciones)  
**Razón:** Integración VS, debugging, toolchain  
**Ideal para:** Desarrollo Windows-first, prototipos

### Por Operación

| Operación | Mejor Compilador | Alternativa | Evitar |
|-----------|------------------|-------------|--------|
| **GCD/LCM** | GCC (3-10x) | Clang/Intel | - |
| **POW** | GCC (∞) | Clang/Intel | MSVC (3x más lento) |
| **SQRT** | GCC (∞) | Clang/Intel | MSVC (3x más lento) |
| **MIDPOINT** | GCC (10x) | MSVC (para igual) | - |
| **Operaciones simples** | Cualquiera excepto MSVC | - | - |

---

## 📌 CONCLUSIONES

1. **GCC 15.2.0 es el claro ganador** en rendimiento absoluto
   - Optimización constexpr excepcional
   - Fusión de operaciones superior
   - 2-10x más rápido que competidores en operaciones clave

2. **Clang 21.1.7 e Intel oneAPI son sólidos segundos**
   - Rendimiento consistente y predecible
   - Buenos para producción multiplataforma
   - Diferencias mínimas entre ellos

3. **MSVC 19.50 tiene margen de mejora**
   - Overhead en operaciones simples (0.5-8 ns)
   - Operaciones complejas 2-3x más lentas
   - Suficiente para la mayoría de aplicaciones

4. **Manejo de signos eficiente**
   - Overhead mínimo en GCC (0-6.5 ns)
   - Aceptable en Clang/Intel (7-43 ns)
   - MSVC necesita mejoras (hasta 112 ns en sqrt)

5. **int128_t vs int64_t**
   - GCC/Clang/Intel optimizan int128_t casi como int64_t
   - MSVC tiene gap en operaciones simples
   - Operaciones complejas (GCD/LCM) tienen costo inherente

---

## 📂 ARCHIVOS DE DATOS

Resultados completos disponibles en:
- `benchmark_results/int128_cmath_benchmarks_gcc.txt`
- `benchmark_results/int128_cmath_benchmarks_clang.txt`
- `benchmark_results/int128_cmath_benchmarks_msvc.txt`
- `benchmark_results/int128_cmath_benchmarks_intel.txt`

Código fuente del benchmark:
- `benchmarks/int128_cmath_benchmarks.cpp`

---

**Generado automáticamente**  
**Fecha:** 20 de diciembre de 2025  
**Versión:** 1.0
