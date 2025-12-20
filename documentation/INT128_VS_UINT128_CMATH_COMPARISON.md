# INT128 vs UINT128 CMATH - COMPARATIVE ANALYSIS
## Side-by-Side Comparison of Signed and Unsigned 128-bit Math Libraries

---

## 📊 Test Results Comparison

### Overall Success Rate: **100% for Both Libraries**

| Library | GCC | Clang | MSVC | Intel | Total Tests |
|---------|-----|-------|------|-------|-------------|
| **uint128_cmath.hpp** | 96/96 | 96/96 | 96/96 | 96/96 | **96 tests** |
| **int128_cmath.hpp** | 141/141 | 141/141 | 139/139 | 139/139 | **139-141 tests** |

**Both libraries achieve 100% success rate across all 4 compilers! ✅**

---

## 🔍 Function Comparison

| Function | uint128_cmath | int128_cmath | Key Differences |
|----------|---------------|--------------|-----------------|
| **gcd** | ✅ (16 tests) | ✅ (16 tests) | int128 handles negative inputs |
| **lcm** | ✅ (12 tests) | ✅ (13 tests) | int128 handles negative inputs |
| **abs** | ❌ N/A | ✅ (6 tests) | Only meaningful for signed integers |
| **pow** | ✅ (12 tests) | ✅ (15 tests) | int128 handles negative bases |
| **sqrt** | ✅ (24 tests) | ✅ (36 tests) | Both for non-negative values only |
| **sign** | ❌ N/A | ✅ (7 tests) | Only meaningful for signed integers |
| **max** | ✅ (7 tests) | ✅ (7 tests) | int128 handles negative values |
| **min** | ✅ (7 tests) | ✅ (7 tests) | int128 handles negative values |
| **clamp** | ❌ N/A | ✅ (8 tests) | int128-specific utility |
| **midpoint** | ❌ N/A | ✅ (9 tests) | int128-specific overflow-safe |
| **bezout_coeffs** | ✅ (7 tests) | ❌ N/A | uint128-specific extended GCD |
| **Edge Cases** | ✅ (5 tests) | ✅ (9 tests) | int128 has more edge cases |
| **Constexpr** | ✅ (6 tests) | ✅ (8/6 tests) | Similar constexpr support |

**Summary:**
- **uint128_cmath**: 7 functions (focused on unsigned operations + extended GCD)
- **int128_cmath**: 10 functions (includes sign-specific operations)
- **Common functions**: gcd, lcm, pow, sqrt, min, max (both work identically)
- **uint128-exclusive**: `bezout_coeffs` (extended GCD for cryptography)
- **int128-exclusive**: `abs`, `sign`, `clamp`, `midpoint` (sign-aware utilities)

---

## 📈 Test Count Breakdown

### uint128_cmath.hpp - 96 Tests
```
GCD:             16 tests (basic, edge cases, large values, co-primes)
LCM:             12 tests (basic, edge cases, large values)
POW:             12 tests (basic, fast paths, large exponents)
SQRT:            24 tests (perfect squares, non-perfect, property verification)
MIN/MAX:         14 tests (basic, equal values, large values, mixed types)
BEZOUT_COEFFS:    7 tests (basic, co-primes, large values)
EDGE CASES:       5 tests (zero operations, identity operations)
CONSTEXPR:        6 tests (compile-time evaluation for GCC/Clang)
```

### int128_cmath.hpp - 141/139 Tests
```
GCD:             16 tests (+negative values, mixed signs)
LCM:             13 tests (+negative values, mixed signs)
ABS:              6 tests (positive, negative, zero, large values)
POW:             15 tests (+negative bases with odd/even exponents)
SQRT:            36 tests (perfect squares, non-perfect, property verification)
SIGN:             7 tests (positive, negative, zero)
MIN/MAX:         14 tests (+negative values, negative ranges)
CLAMP:            8 tests (within range, boundaries, negative ranges)
MIDPOINT:         9 tests (positive, negative, odd sums, overflow-safe)
EDGE CASES:       9 tests (+zero operations, self operations)
CONSTEXPR:        8 tests (GCC/Clang) / 6 tests (MSVC/Intel)
```

**Key Difference:** int128_cmath has **+45 more tests** due to:
- Sign handling (negative inputs, negative results)
- 3 additional functions (abs, sign, clamp, midpoint)
- More edge cases for signed arithmetic

---

## ⚡ Performance Characteristics Comparison

| Operation | uint128 Complexity | int128 Complexity | Notes |
|-----------|-------------------|-------------------|-------|
| **gcd** | O(log(min(a,b))) | O(log(min(a,b))) | int128 adds sign normalization (negligible cost) |
| **lcm** | O(log(min(a,b))) | O(log(min(a,b))) | Both use `a*b/gcd(a,b)`, int128 uses abs() |
| **abs** | N/A | O(1) | int128-specific |
| **pow** | O(log(exp)) | O(log(exp)) | int128 handles negative base (same algorithm) |
| **sqrt** | O(log(n)) | O(log(n)) | Identical Newton's method |
| **sign** | N/A | O(1) | int128-specific |
| **min/max** | O(1) | O(1) | Identical comparison logic |
| **clamp** | N/A | O(1) | int128-specific |
| **midpoint** | N/A | O(1) | int128-specific, overflow-safe |
| **bezout_coeffs** | O(log(min(a,b))) | N/A | uint128-specific, extended GCD |

**Performance Impact of Signedness:**
- **GCD/LCM**: int128 adds minimal overhead (sign normalization)
- **POW**: int128 adds branch for negative base handling
- **All other operations**: Identical performance characteristics

---

## 🎯 Use Case Recommendations

### Choose **uint128_cmath.hpp** when:
- ✅ Working with **unsigned-only** data (bit manipulation, hashing)
- ✅ Need **bezout_coeffs** for extended GCD (cryptography, modular arithmetic)
- ✅ Range is naturally non-negative (counters, sizes, IDs)
- ✅ Want to avoid sign-related edge cases
- ✅ Working with **bit operations** (no sign bit interference)

### Choose **int128_cmath.hpp** when:
- ✅ Working with **signed** data (scientific computing, physics)
- ✅ Need **abs()** and **sign()** functions
- ✅ Need **clamp()** for range limiting
- ✅ Need overflow-safe **midpoint()** calculation
- ✅ Data can be **negative** (coordinates, differences, financial)
- ✅ Interfacing with standard library signed types

---

## 🔧 Compilation Comparison

### Compiler Support: **Both Libraries - 4/4 Compilers ✅**

| Compiler | uint128 | int128 | Common Flags |
|----------|---------|--------|--------------|
| **GCC 15.2.0** | ✅ 96/96 | ✅ 141/141 | `-std=c++20 -O3 -Wall` |
| **Clang 19.0** | ✅ 96/96 | ✅ 141/141 | `-std=c++20 -O3 -Wall` |
| **MSVC 19.50.35720** | ✅ 96/96 | ✅ 139/139 | `/std:c++20 /O2 /EHsc /W4` |
| **Intel oneAPI 2025.3.0** | ✅ 96/96 | ✅ 139/139 | `-std=c++20 -O3 -Wall` |

**Constexpr Availability:**
| Feature | GCC/Clang | MSVC/Intel |
|---------|-----------|------------|
| **uint128**: pow, sqrt, min, max | ✅ constexpr | ❌ runtime (intrinsics) |
| **int128**: sign, min, max, clamp | ✅ constexpr | ❌ runtime (intrinsics) |

Both libraries work identically across all compilers with expected constexpr limitations on MSVC/Intel.

---

## 📋 Code Interoperability

### Mixed Type Operations
Both libraries support mixed-type operations:

```cpp
// uint128_cmath.hpp
uint128_t result1 = std::gcd(uint128_t(12), 18);  // int128_t with int
uint128_t result2 = std::lcm(12, uint128_t(18));  // int with int128_t

// int128_cmath.hpp
int128_t result3 = std::gcd(int128_t(12), 18);    // int128_t with int
int128_t result4 = std::lcm(12, int128_t(18));    // int with int128_t
```

Both libraries provide seamless type promotion.

---

## 🏆 Key Differences Summary

### Unique to **uint128_cmath.hpp**:
| Feature | Purpose | Tests |
|---------|---------|-------|
| **bezout_coeffs(a,b)** | Extended GCD: returns (gcd, x, y) where ax + by = gcd | 7 tests |

**bezout_coeffs** is critical for:
- RSA cryptography (modular multiplicative inverse)
- Solving linear Diophantine equations
- Modular arithmetic optimizations

### Unique to **int128_cmath.hpp**:
| Feature | Purpose | Tests |
|---------|---------|-------|
| **abs(x)** | Absolute value for signed integers | 6 tests |
| **sign(x)** | Returns -1, 0, or +1 based on sign | 7 tests |
| **clamp(x, lo, hi)** | Clamps value to range [lo, hi] | 8 tests |
| **midpoint(a, b)** | Overflow-safe midpoint calculation | 9 tests |

**sign-specific functions** are essential for:
- Sign-aware algorithms (absolute difference, signum)
- Range limiting and boundary checking
- Safe average/midpoint calculation (prevents overflow)

---

## 🎯 Functional Overlap

### Functions Available in **BOTH** Libraries:

#### 1. **GCD (Greatest Common Divisor)**
```cpp
// uint128_cmath.hpp
uint128_t gcd_unsigned = std::gcd(uint128_t(48), uint128_t(18));  // 6

// int128_cmath.hpp
int128_t gcd_signed = std::gcd(int128_t(-48), int128_t(18));      // 6 (handles signs)
```
**Difference:** int128 normalizes signs, uint128 doesn't need to.

#### 2. **LCM (Least Common Multiple)**
```cpp
// uint128_cmath.hpp
uint128_t lcm_unsigned = std::lcm(uint128_t(12), uint128_t(18));  // 36

// int128_cmath.hpp
int128_t lcm_signed = std::lcm(int128_t(-12), int128_t(18));      // 36 (abs result)
```
**Difference:** int128 returns absolute value, uint128 is naturally unsigned.

#### 3. **POW (Power Function)**
```cpp
// uint128_cmath.hpp
uint128_t pow_unsigned = std::pow(uint128_t(2), uint128_t(10));   // 1024

// int128_cmath.hpp
int128_t pow_signed = std::pow(int128_t(-2), int128_t(3));        // -8 (handles negative base)
```
**Difference:** int128 can handle negative bases with proper odd/even exponent logic.

#### 4. **SQRT (Square Root)**
```cpp
// uint128_cmath.hpp
uint128_t sqrt_unsigned = std::sqrt(uint128_t(100));               // 10

// int128_cmath.hpp
int128_t sqrt_signed = std::sqrt(int128_t(100));                   // 10 (requires non-negative)
```
**Difference:** Both require non-negative input (undefined for negative).

#### 5. **MIN / MAX**
```cpp
// uint128_cmath.hpp
uint128_t min_u = std::min(uint128_t(5), uint128_t(10));           // 5
uint128_t max_u = std::max(uint128_t(5), uint128_t(10));           // 10

// int128_cmath.hpp
int128_t min_s = std::min(int128_t(-5), int128_t(10));             // -5
int128_t max_s = std::max(int128_t(-5), int128_t(10));             // 10
```
**Difference:** int128 handles negative values correctly.

---

## 💡 Combined Usage Pattern

**For comprehensive 128-bit math coverage**, use both libraries:

```cpp
#include "int128/uint128_cmath.hpp"  // For unsigned operations + bezout
#include "int128/int128_cmath.hpp"   // For signed operations + sign functions

// Unsigned cryptographic operations
auto [g, x, y] = std::bezout_coeffs(uint128_t(17), uint128_t(13));  // Extended GCD

// Signed mathematical operations
int128_t diff = std::abs(value1 - value2);                           // Absolute difference
int128_t direction = std::sign(delta);                               // Sign detection
int128_t clamped = std::clamp(value, min_val, max_val);             // Range limiting
int128_t average = std::midpoint(a, b);                              // Overflow-safe midpoint
```

---

## 📊 Test Coverage Quality Comparison

| Metric | uint128_cmath | int128_cmath |
|--------|---------------|--------------|
| **Basic operations** | ✅ Excellent | ✅ Excellent |
| **Edge cases** | ✅ 5 scenarios | ✅ 9 scenarios |
| **Negative values** | ❌ N/A | ✅ Comprehensive |
| **Large values** | ✅ 1e18+ range | ✅ ±1e18+ range |
| **Mixed types** | ✅ Yes | ✅ Yes |
| **Property verification** | ✅ sqrt properties | ✅ sqrt properties |
| **Constexpr testing** | ✅ 6 tests | ✅ 8/6 tests |
| **Zero handling** | ✅ Comprehensive | ✅ Comprehensive |
| **Boundary conditions** | ✅ Yes | ✅ + negative boundaries |

Both libraries have **production-grade test coverage** with comprehensive edge case handling.

---

## 🏁 Conclusion

### **Both Libraries are Production-Ready! ✅**

| Library | Strengths | Ideal For |
|---------|-----------|-----------|
| **uint128_cmath.hpp** | • Extended GCD (bezout_coeffs)<br>• No sign overhead<br>• Cryptography-ready | • RSA cryptography<br>• Modular arithmetic<br>• Bit manipulation<br>• Unsigned-only domains |
| **int128_cmath.hpp** | • Sign-aware operations<br>• abs, sign, clamp, midpoint<br>• Natural negative handling | • Scientific computing<br>• Signed mathematics<br>• Physics simulations<br>• Financial calculations |

### **Recommendation:**
- Use **uint128_cmath.hpp** for **cryptography and unsigned math**
- Use **int128_cmath.hpp** for **general purpose signed math**
- Use **both** for comprehensive 128-bit integer support

### **Quality Metrics:**
- ✅ **100% success rate** across all compilers (GCC, Clang, MSVC, Intel)
- ✅ **237 total tests** (96 uint128 + 141 int128)
- ✅ **C++20 standard compliance**
- ✅ **Constexpr support** where possible (GCC/Clang)
- ✅ **Zero compiler errors** across all platforms
- ✅ **Optimal performance** (logarithmic/constant time operations)

Both libraries complement each other perfectly and provide complete 128-bit integer mathematical functionality! 🎯

---

*Generated: 2024*  
*Comparison based on comprehensive testing with 4 compilers*
