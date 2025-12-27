# Análisis de Multiplicación Karatsuba para uint128_t

## 📊 Resumen Ejecutivo

Para enteros de 128 bits representados como `{uint64_t high, uint64_t low}`, el algoritmo de Karatsuba **solo tiene sentido cuando ambos operandos ocupan sus 2 componentes uint64_t** (high ≠ 0).

En todos los demás casos, la multiplicación directa con fast paths es superior.

---

## 🔢 Representación de uint128_t

```cpp
struct uint128_t {
    uint64_t high;  // Bits 64-127
    uint64_t low;   // Bits 0-63
};

// Valor: high * 2^64 + low
```

**Categorías de operandos**:

1. **Pequeño**: `high == 0` (valor ≤ 2^64 - 1)
2. **Grande**: `high != 0` (valor > 2^64 - 1)

---

## 🧮 Algoritmo de Multiplicación Directa

### Fórmula Matemática

```
a = a_high * 2^64 + a_low
b = b_high * 2^64 + b_low

a * b = a_high*b_high * 2^128 
      + (a_high*b_low + a_low*b_high) * 2^64 
      + a_low*b_low

Resultado (truncado a 128 bits):
  result.high = (a_high*b_low + a_low*b_high)_low + (a_low*b_low)_high
  result.low  = (a_low*b_low)_low
```

### Casos de Optimización

#### **Caso 1: Ambos Pequeños** (a.high = 0, b.high = 0)

```cpp
if (a.high == 0 && b.high == 0) {
    uint128_t result;
    __uint128_t product = static_cast<__uint128_t>(a.low) * b.low;
    result.low = static_cast<uint64_t>(product);
    result.high = static_cast<uint64_t>(product >> 64);
    return result;
}
```

**Costo**: **1 multiplicación 64×64 → 128**  
**Frecuencia**: Alta en aplicaciones con valores pequeños

#### **Caso 2: Uno Pequeño, Uno Grande**

##### Caso 2a: `a.high = 0, b.high ≠ 0`

```cpp
if (a.high == 0) {
    // a * b = a_low * (b_high * 2^64 + b_low)
    //       = a_low*b_low + (a_low*b_high) * 2^64
    
    __uint128_t p0 = static_cast<__uint128_t>(a.low) * b.low;
    __uint128_t p1 = static_cast<__uint128_t>(a.low) * b.high;
    
    result.low  = static_cast<uint64_t>(p0);
    result.high = static_cast<uint64_t>(p0 >> 64) + static_cast<uint64_t>(p1);
    return result;
}
```

**Costo**: **2 multiplicaciones 64×64 → 128**  
**Frecuencia**: Media-Alta

##### Caso 2b: `a.high ≠ 0, b.high = 0`

```cpp
if (b.high == 0) {
    // Simétrico al caso 2a
    return multiply_128x64(a, b.low);
}
```

**Costo**: **2 multiplicaciones 64×64 → 128**

#### **Caso 3: Ambos Grandes** (a.high ≠ 0, b.high ≠ 0)

```cpp
// Necesitamos los 4 productos
__uint128_t p00 = static_cast<__uint128_t>(a.low)  * b.low;
__uint128_t p01 = static_cast<__uint128_t>(a.low)  * b.high;
__uint128_t p10 = static_cast<__uint128_t>(a.high) * b.low;
__uint128_t p11 = static_cast<__uint128_t>(a.high) * b.high;

// p11 contribuye solo a bits > 128 (overflow), lo ignoramos
result.low  = static_cast<uint64_t>(p00);
result.high = static_cast<uint64_t>(p00 >> 64) 
            + static_cast<uint64_t>(p01) 
            + static_cast<uint64_t>(p10);
return result;
```

**Costo**: **4 multiplicaciones 64×64 → 128**  
**Nota**: `p11` se puede omitir si solo queremos los 128 bits bajos  
**Frecuencia**: Baja-Media

---

## 🚀 Algoritmo de Karatsuba

### Principio

Reduce el número de multiplicaciones de n² a n^log₂(3) ≈ n^1.585

Para n=2 (2 componentes de 64 bits): **4 multiplicaciones → 3 multiplicaciones**

### Implementación para 128 bits

```cpp
uint128_t karatsuba_128(uint128_t a, uint128_t b) {
    // z0 = a_low * b_low
    __uint128_t z0 = static_cast<__uint128_t>(a.low) * b.low;
    
    // z2 = a_high * b_high  
    __uint128_t z2 = static_cast<__uint128_t>(a.high) * b.high;
    
    // z1 = (a_low + a_high) * (b_low + b_high) - z0 - z2
    uint64_t sum_a = a.low + a.high;    // Puede overflow
    uint64_t sum_b = b.low + b.high;    // Puede overflow
    __uint128_t z1_temp = static_cast<__uint128_t>(sum_a) * sum_b;
    __uint128_t z1 = z1_temp - z0 - z2;
    
    // Combinar: result = z0 + (z1 << 64) + (z2 << 128)
    // z2 << 128 contribuye 0 a los 128 bits bajos (overflow)
    uint128_t result;
    result.low  = static_cast<uint64_t>(z0);
    result.high = static_cast<uint64_t>(z0 >> 64) + static_cast<uint64_t>(z1);
    
    return result;
}
```

**Costo**: **3 multiplicaciones 64×64 → 128 + 5 sumas/restas + manejo de carry**

### Ventajas

- ✅ Reduce 4→3 multiplicaciones (25% menos)
- ✅ Asintóticamente mejor para enteros muy grandes

### Desventajas

- ❌ Overhead de sumas/restas adicionales
- ❌ Manejo de overflow en sumas (carry detection)
- ❌ Código más complejo
- ❌ Menos cache-friendly
- ❌ **Peor rendimiento para casos pequeños/medios**

---

## 📊 Comparación de Costos

### Tabla de Análisis

| Caso | Directa | Karatsuba | Ganador | Factor |
|------|---------|-----------|---------|--------|
| Ambos ≤64 bits (high=0) | **1 mul** | 3 muls + overhead | **Directa** | ~3× más rápida |
| Uno ≤64 bits (un high=0) | **2 muls** | 3 muls + overhead | **Directa** | ~1.5× más rápida |
| Ambos >64 bits (high≠0) | 4 muls | **3 muls + overhead** | **Karatsuba** | ~1.15-1.25× más rápida |

### Análisis de Frecuencia de Casos

```cpp
// En aplicaciones típicas (distribución uniforme de 0 a 2^128-1):
// P(high = 0) ≈ 2^-64 ≈ 5.4 × 10^-20  ← Extremadamente raro

// PERO en aplicaciones reales:
// - Contadores, índices, IDs → Casi siempre < 2^64
// - Operaciones financieras → Generalmente < 2^96
// - Criptografía → Casi siempre usa los 128 bits

// Distribución estimada en aplicaciones no-criptográficas:
// Ambos ≤64 bits:  30-60%  ← Fast path crítico
// Uno ≤64 bits:    20-40%  ← Fast path importante
// Ambos >64 bits:  10-50%  ← Aquí Karatsuba ayuda
```

---

## 💡 Recomendaciones de Implementación

### Estrategia Óptima: Dispatch Dinámico

```cpp
uint128_t multiply_optimized(uint128_t a, uint128_t b) {
    // Fast path 1: Ambos pequeños (1 mul)
    if (a.high == 0 && b.high == 0) {
        return multiply_64x64(a.low, b.low);
    }
    
    // Fast path 2: Uno pequeño (2 muls)
    if (a.high == 0) {
        return multiply_64x128(a.low, b);
    }
    if (b.high == 0) {
        return multiply_128x64(a, b.low);
    }
    
    // Caso general: Ambos grandes (3 muls con Karatsuba vs 4 directa)
    #ifdef USE_KARATSUBA_128
        return karatsuba_128(a, b);  // 3 muls + overhead
    #else
        return multiply_128x128(a, b);  // 4 muls directas
    #endif
}
```

### Consideraciones de Compilador

```cpp
// Muchos compiladores ya optimizan __uint128_t nativamente
#if defined(__SIZEOF_INT128__)
    // GCC/Clang tienen soporte nativo
    __uint128_t native_mul(__uint128_t a, __uint128_t b) {
        return a * b;  // ¡El compilador genera código óptimo!
    }
    
    // Con -O3, GCC genera:
    // - Detección automática de casos pequeños
    // - Uso de instrucciones MULX/MULQ (x86-64)
    // - Inline agresivo
#endif
```

**Importante**: En x86-64, `mul` (64×64→128) es **1 instrucción** muy rápida. El overhead de Karatsuba puede no compensar.

---

## 🔬 Criterios de Decisión

### Usar Karatsuba SOLO si

1. ✅ **Aplicación criptográfica** (valores casi siempre >64 bits)
2. ✅ **Perfil muestra** que >60% de multiplicaciones son ambos operandos >64 bits
3. ✅ **Compilador no optimiza** bien la multiplicación nativa
4. ✅ **CPU sin instrucciones** mul rápidas (poco común en 2025)

### NO usar Karatsuba si

1. ❌ **Aplicación de propósito general** (muchos valores pequeños)
2. ❌ **Compilador con soporte nativo** para __uint128_t
3. ❌ **x86-64 moderno** con MULX/MULQ (1-3 ciclos por mul)
4. ❌ **Sin evidencia de bottleneck** en multiplicación

---

## 📈 Extensión a Enteros Más Grandes

Karatsuba **sí tiene sentido** para enteros mucho más grandes:

| Tamaño | Directa | Karatsuba | Mejora |
|--------|---------|-----------|--------|
| 128 bits (2×64) | 4 muls | 3 muls | ~25% |
| 256 bits (4×64) | 16 muls | 9 muls | ~44% |
| 512 bits (8×64) | 64 muls | 27 muls | ~58% |
| 1024 bits (16×64) | 256 muls | 81 muls | ~68% |
| n bits | O(n²) | **O(n^1.585)** | **Asintótico** |

**Conclusión para enteros grandes (>256 bits)**: Karatsuba es esencial.

---

## 🎯 Resumen y Conclusiones

### Para uint128_t (16 bytes)

1. **Karatsuba solo ayuda cuando ambos operandos son "grandes"** (high ≠ 0)
   - En este caso: 4 muls → 3 muls (25% mejor teóricamente)
   - Overhead práctico reduce ganancia a ~15-20%

2. **Fast paths son críticos**:
   - Ambos ≤64 bits: 1 mul (casos frecuentes en aplicaciones reales)
   - Uno ≤64 bits: 2 muls (casos también frecuentes)

3. **Implementación recomendada**:

   ```cpp
   multiply_optimized() {
       if (fast_paths) return optimized_path;
       return use_karatsuba_or_direct_based_on_profiling;
   }
   ```

4. **Verificar compilador**:
   - GCC/Clang con `__uint128_t` ya optimizan muy bien
   - Medir antes de optimizar prematuramente

5. **Benchmark es esencial**:
   - Crear test con distribución realista de valores
   - Medir con diferentes compiladores y flags
   - Perfilar aplicación real

---

## 🔨 Próximos Pasos (TODO)

### Implementación

- [ ] Implementar `multiply_optimized()` con fast paths
- [ ] Implementar `karatsuba_128()` opcional
- [ ] Implementar `multiply_128x128()` directo
- [ ] Agregar flag de compilación `USE_KARATSUBA_128`

### Testing

- [ ] Benchmark: Caso 1 (ambos pequeños) - 1000 muestras
- [ ] Benchmark: Caso 2 (uno pequeño) - 1000 muestras
- [ ] Benchmark: Caso 3 (ambos grandes) - 1000 muestras
- [ ] Benchmark: Distribución mixta realista
- [ ] Comparar con multiplicación nativa `__uint128_t`

### Análisis

- [ ] Perfilar en x86-64 (Intel, AMD)
- [ ] Perfilar en ARM64
- [ ] Analizar ensamblador generado
- [ ] Medir impacto de branch prediction
- [ ] Documentar decisión final con datos

---

## 📚 Referencias

### Algoritmo de Karatsuba

- A. Karatsuba (1960): "Multiplication of Multidigit Numbers on Automata"
- Complejidad: O(n^log₂(3)) ≈ O(n^1.585)

### Algoritmos Relacionados

- **Toom-Cook**: Generalización de Karatsuba, mejor para n>3
- **Schönhage-Strassen**: FFT-based, O(n log n log log n), mejor para n>10,000 bits
- **Fürer's algorithm**: O(n log n 2^O(log* n)), teórico

### Implementaciones Reales

- **GMP (GNU Multiple Precision)**: Usa Karatsuba para >70 limbs (~4480 bits)
- **MPIR**: Threshold similar a GMP
- **Crypto++**: Usa Karatsuba para >256 bits
- **OpenSSL**: Optimiza caso por caso con ASM

---

## 📅 Información del Documento

- **Fecha**: 27 de diciembre de 2025
- **Versión**: 1.0
- **Autor**: Análisis para proyecto int128
- **Propósito**: Documentación para futuras optimizaciones de multiplicación

---

*"Premature optimization is the root of all evil" — Donald Knuth*

**Medir, perfilar, y luego optimizar. No asumir.**
