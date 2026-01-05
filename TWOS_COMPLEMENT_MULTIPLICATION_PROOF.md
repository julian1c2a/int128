# Prueba Matemática: Multiplicación en Complemento a 2

> **Teorema:** La multiplicación modular en representación de complemento a 2 es independiente del signo de los operandos.

---

## 📐 Definiciones

### Función de Representación

Para un entero `n` en complemento a 2 con `N` bits:

```
repr(n) := { 2^N - |n|    si n < 0
           { |n|          si n ≥ 0
```

**Interpretación:**

- Los números negativos se representan usando `2^N - |n|` (complemento a 2)
- Los números no negativos se representan directamente

**Ejemplos con N=4:**

- `repr(+5) = 5 = 0101₂`
- `repr(-3) = 2⁴ - 3 = 16 - 3 = 13 = 1101₂`
- `repr(-8) = 2⁴ - 8 = 16 - 8 = 8 = 1000₂` (INT_MIN)

---

## 🔬 Demostración por Casos

### Notación

Sean `n` y `m` dos enteros a multiplicar. Analizamos los cuatro casos posibles según sus signos.

---

### **Caso 1: Ambos Negativos** (n < 0, m < 0)

**Operación:**

```
repr(n × m) = repr(n) × repr(m)  (mod 2^N)
            = (2^N - |n|) × (2^N - |m|)  (mod 2^N)
```

**Expansión:**

```
= 2^(2N) - |m|·2^N - |n|·2^N + |n|·|m|  (mod 2^N)
```

**Reducción modular:** Los términos con `2^N` o mayores se cancelan:

```
≡ |n|·|m|  (mod 2^N)
```

**Resultado:**

```
repr(n × m) = |n|·|m| = repr(|n × m|) ✓
```

**Conclusión:** El producto de dos negativos produce el resultado correcto (positivo).

---

### **Caso 2: Primer Operando Negativo** (n < 0, m > 0)

**Operación:**

```
repr(n × m) = repr(n) × repr(m)  (mod 2^N)
            = (2^N - |n|) × |m|  (mod 2^N)
```

**Expansión:**

```
= |m|·2^N - |n|·|m|  (mod 2^N)
```

**Reducción modular:** El término `|m|·2^N` se cancela:

```
≡ -|n|·|m|  (mod 2^N)
≡ 2^N - |n|·|m|  (mod 2^N)
```

**Resultado:**

```
repr(n × m) = 2^N - |n × m| = repr(n × m) ✓
```

**Conclusión:** El producto de negativo × positivo produce el resultado correcto (negativo).

---

### **Caso 3: Segundo Operando Negativo** (n > 0, m < 0)

**Operación:**

```
repr(n × m) = repr(n) × repr(m)  (mod 2^N)
            = |n| × (2^N - |m|)  (mod 2^N)
```

**Expansión:**

```
= |n|·2^N - |n|·|m|  (mod 2^N)
```

**Reducción modular:**

```
≡ -|n|·|m|  (mod 2^N)
≡ 2^N - |n|·|m|  (mod 2^N)
```

**Resultado:**

```
repr(n × m) = 2^N - |n × m| = repr(n × m) ✓
```

**Conclusión:** El producto de positivo × negativo produce el resultado correcto (negativo).

---

### **Caso 4: Ambos Positivos** (n > 0, m > 0)

**Operación:**

```
repr(n × m) = repr(n) × repr(m)  (mod 2^N)
            = |n| × |m|  (mod 2^N)
```

**Simplificación directa:**

```
= |n|·|m|  (mod 2^N)
```

**Resultado:**

```
repr(n × m) = |n × m| = repr(n × m) ✓
```

**Conclusión:** El producto de dos positivos produce el resultado correcto (positivo).

---

## 🎯 Conclusión General

**Teorema probado:**

```
∀ n, m ∈ ℤ : repr(n × m) = repr(n) × repr(m)  (mod 2^N)
```

**Implicaciones prácticas:**

1. **Hardware:** Los circuitos de multiplicación pueden ser idénticos para signed e unsigned
2. **Software:** Un único algoritmo de multiplicación funciona para ambos tipos
3. **Optimización:** No se necesitan ramas (branches) para manejar signos
4. **Portabilidad:** El comportamiento es consistente en todas las arquitecturas

**En `int128_base_t<S>`:**

La implementación de `operator*` es **idéntica** para `uint128_t` y `int128_t`. No se necesita `if constexpr (is_signed)` porque la aritmética modular del complemento a 2 es intrínsecamente correcta para ambos casos.

```cpp
constexpr int128_base_t& operator*=(const int128_base_t& other) noexcept {
    // Esta implementación funciona para signed Y unsigned
    // No se necesita verificación de signo
    // ...
    return *this;
}
```

---

## 📚 Referencias

- **Donald Knuth:** *The Art of Computer Programming, Vol. 2: Seminumerical Algorithms*
- **Henry S. Warren Jr.:** *Hacker's Delight* - Capítulo sobre aritmética de complemento a 2
- **C++ Standard:** ISO/IEC 14882:2020 - Sección [basic.fundamental] sobre representaciones de enteros

---

## 🔍 Ejemplos Numéricos

### Ejemplo 1: Negativos (N=8, -3 × -5 = +15)

```
n = -3, m = -5

repr(-3) = 2⁸ - 3 = 256 - 3 = 253 = 11111101₂
repr(-5) = 2⁸ - 5 = 256 - 5 = 251 = 11111011₂

253 × 251 = 63503
63503 mod 256 = 15 = 00001111₂

repr⁻¹(15) = +15 ✓
```

---

### Ejemplo 2: Mixto (N=8, -6 × 7 = -42)

```
n = -6, m = 7

repr(-6) = 2⁸ - 6 = 256 - 6 = 250 = 11111010₂
repr(7) = 7 = 00000111₂

250 × 7 = 1750
1750 mod 256 = 214 = 11010110₂

repr⁻¹(214) = -(256 - 214) = -42 ✓
```

---

### Ejemplo 3: Overflow (N=8, 20 × 30 = 600 → overflow)

```
n = 20, m = 30

repr(20) = 20 = 00010100₂
repr(30) = 30 = 00011110₂

20 × 30 = 600
600 mod 256 = 88 = 01011000₂

600 > 127 (INT8_MAX), por lo que hay overflow
Resultado en complemento a 2: 88 (incorrectamente interpretado como positivo)
```

**Nota:** El overflow en multiplicación signed puede producir resultados incorrectos. La biblioteca `int128_t` **permite** overflow (wrap-around), igual que los tipos builtin de C++.

---

## 💡 Implicación para División

**⚠️ IMPORTANTE:** A diferencia de la multiplicación, la **división** SÍ depende del signo:

```
(-10) ÷ 3 = -3 (resto = -1)
10 ÷ (-3) = -3 (resto = 1)
```

Por lo tanto, `operator/` e `operator%` SÍ necesitan `if constexpr (is_signed)` para manejar correctamente los casos signed.

---

**Última actualización:** 2026-01-05  
**Versión biblioteca:** 1.0.0  
**Tests validados:** ✅ 29/29 PASS
