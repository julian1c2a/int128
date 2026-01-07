# AI Agent Instructions for int128 Project

## ⚠️ CRITICAL RULES (HIGH PRIORITY)

### 1. PROMPT.md is MANDATORY
**PROMPT.md must be followed for ALL actions.** It defines:
- Build scripts naming conventions
- Directory structure for outputs
- Compiler configurations
- Test/benchmark execution patterns

### 2. Keep Root Directory CLEAN
**NEVER create these files in the project root:**
- `*.exe` - Executables
- `*.obj` - Object files  
- `*.pdb` - Debug symbols
- `*.o` - Object files (Linux)

**Use these directories instead:**
- `build_temp/` - For non-normalized executables and temporary builds
- `scripts_temp/` - For non-normalized/experimental scripts
- `build/build_[tests|benchs]/[compiler]/[mode]/` - For normalized builds (per PROMPT.md)

### 3. ASCII-Only Output in Code
**All console output in C++ code must use ASCII characters only:**
- NO Unicode box drawing (╔═║╚) → Use `+=-|` instead
- NO Unicode checkmarks (✓✗) → Use `[OK]` and `[FAIL]` instead
- NO Spanish accents in strings (á é í ó ú ñ) → Use unaccented equivalents
- Run `python fix_unicode_chars.py` to clean existing files

### 4. Report Status Every 5 Minutes
During long tasks, provide brief status updates every ~5 minutes to maintain user awareness.

---

## Project Overview

Header-only C++20 library implementing 128-bit integers (`uint128_t` and `int128_t`) with full STL integration, optimized for cross-compiler support (GCC, Clang, Intel, MSVC) on Windows/MSYS2.

**Status**: Production-ready (14/14 features complete for both types)  
**Next Phase**: Template unification (`int128_base_t<signedness S>`) - see TODO.md Phase 1.5

## Core Architecture

### Namespace & Types
- **Namespace**: `nstd` (everything lives here)
- **Main types**: `nstd::uint128_t` (unsigned), `nstd::int128_t` (signed, two's complement)
- **Storage**: `uint64_t data[2]` (little-endian: `data[0]` = low, `data[1]` = high)
- **Design**: Intentional signed overflow wrap-around (not UB), mimics two's complement

### Critical Header Organization
```
include/
├── int128.hpp                    # Single-include header (includes both types)
├── uint128/
│   ├── uint128_t.hpp            # Core implementation (2573 lines)
│   ├── uint128_traits_specializations.hpp  # ⚠️ MUST be first (before stdlib)
│   └── uint128_[feature].hpp    # Modular extensions
├── int128/
│   ├── int128_t.hpp             # Signed implementation (wraps uint128_t)
│   └── int128_[feature].hpp
└── intrinsics/
    ├── compiler_detection.hpp   # Cross-compiler intrinsics
    └── arithmetic_operations.hpp
```

**⚠️ CRITICAL**: `uint128_traits_specializations.hpp` **must** be included before standard headers to properly specialize `std::is_integral<uint128_t>` and related traits.

### Feature Modules
Both types follow pattern: `[type]_[feature].hpp` (14 features total)
- `t` - Core type definition
- `traits` - Type traits & `std::hash`
- `limits` - `std::numeric_limits` specialization
- `concepts` - C++20 concepts
- `algorithm` - STL-compatible algorithms
- `iostreams` - Stream operators
- `cmath` - Math functions (sqrt, pow, gcd, lcm)
- `numeric` - Numeric operations
- `ranges` - Range adapters
- `format` - Formatting (hex, oct, bin, custom)
- `safe` - Overflow-checked operations
- `thread_safety` - Atomic wrappers (requires `-pthread -latomic`)
- `comparison_boost` - Benchmarks vs Boost.Multiprecision (uint128 only)
- `interop` - Interoperability tests/benchmarks (uint128 ↔ int128)

## Build System (4 Approaches)

### 1. make.py - Python Build System (Recommended for CI/CD)
```bash
# Unified Python interface with isolated compiler environments
python make.py init                                    # Detect & configure compilers (first time)
python make.py build uint128 bits tests gcc release
python make.py check uint128 bits                      # Run tests
python make.py run uint128 algorithm benchs            # Run benchmarks
python make.py test                                    # Run ALL tests
python make.py bench                                   # Run ALL benchmarks
python make.py all                                     # Build everything

# Demos
python make.py build demos tutorials 01_basic_operations gcc release
python make.py demo tutorials 01_basic_operations      # Compile + run
```

**Key advantage**: Isolated compiler environments (no global contamination)

### 2. Makefile (Quick commands)
```bash
# Pattern: make [action] TYPE=[uint128|int128] FEATURE=[feature] COMPILER=[gcc|clang|intel|msvc|all] MODE=[debug|release|all]
make build_tests TYPE=uint128 FEATURE=cmath COMPILER=gcc MODE=release
make check TYPE=int128 FEATURE=traits COMPILER=all MODE=all
make run TYPE=uint128 FEATURE=numeric COMPILER=clang MODE=debug

# Demos use CATEGORY instead of TYPE
make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release
make demo CATEGORY=tutorials DEMO=01_basic_operations           # Compile + run shortcut
```

### 3. Generic Scripts (Bash - portable)
```bash
# scripts/build_generic.bash <type> <feature> <target> [compiler] [mode] [print]
scripts/build_generic.bash uint128 cmath tests gcc release
scripts/check_generic.bash int128 traits all all
scripts/run_generic.bash uint128 numeric gcc debug

# Demos
scripts/build_generic.bash demos tutorials 01_basic_operations gcc release
scripts/run_generic.bash demos examples ipv6_address

# Output paths: 
build/build_[tests|benchs]/[compiler]/[mode]/[type]_[feature]_[target]_[compiler].exe
```

### 4. Direct Compilation
```bash
/ucrt64/bin/g++ -std=c++20 -Iinclude tests/uint128_cmath_extracted_tests.cpp -o build/test

# ⚠️ Thread safety requires extra flags:
g++ -std=c++20 -Iinclude -pthread tests/uint128_thread_safety_tests.cpp -latomic -o build/test
```

## Recent Project Changes (Phase 0.5 - Dec 2025)

### ✅ Operator Multiplication Optimization
**Completed Dec 31, 2025** - See TODO.md Phase 0.5

#### `operator*=(T other)` - Multiplication with assignment
**Two-level adaptive strategy:**

1. **Fast path** (`data[1] == 0`): Multiplicand fits in 64 bits
   - Algorithm: Single `umul128(data[0], b, &data[1])`
   - Improvement: 75% faster (1 vs 4 multiplications)
   - Use case: ~70-80% of operations in typical code

2. **General case** (`data[1] != 0`): 128×64 bit multiplication
   - Algorithm: `umul128` + cross-product
   - Improvement: 50% faster (2 vs 4 multiplications)

3. **Sign extension**: Automatic for signed types
   - Detection: `if constexpr (std::is_signed_v<T>)`
   - Correction: `if (other < 0) { high_part -= data[0]; }`

**Measured impact:**
- ✅ Fast path: 75% faster, General case: 50% faster
- ✅ No regressions in tests
- ✅ Correct sign extension for signed types

### 📋 Next Step: Phase 1.5 - Template Unification (Starting Jan 2026)
**Goal**: Unify `int128_t` and `uint128_t` into template `int128_base_t<signedness S>`

**Why unify first before expanding to N-width?**
1. ✅ Avoid duplicating effort (refactor once instead of twice)
2. ✅ Controlled scope (128 bits is known, tests validate everything)
3. ✅ Solid base (expanding N later will be mechanical: change `2` → `N`)
4. ✅ Reduce maintenance (4500 lines → 2500 lines, 45% less code)

**Expected benefits:**
- ✅ 45% less code (~2500 vs 4500 lines)
- ✅ Single point for bugs/features
- ✅ Foundation for N-width expansion (256/512 bits)
- ✅ Automatic consistency between types

**Implementation Plan:**
```bash
# 1. Create branch
git checkout -b unified-signedness

# 2. Create new file: include/wide_int_base.hpp
# - enum class signedness { unsigned_type, signed_type }
# - template<signedness S> class int128_base_t { uint64_t data[2]; }

# 3. Modify current headers to use type aliases
# include/uint128/uint128_t.hpp → using uint128_t = int128_base_t<signedness::unsigned_type>;
# include/int128/int128_t.hpp   → using int128_t = int128_base_t<signedness::signed_type>;

# 4. Validate exhaustively with all existing tests
```

**Operations requiring `if constexpr (is_signed)`:**
| Operation | Reason |
|-----------|--------|
| `operator-()` | Solo para signed |
| `operator>>=` | Arithmetic shift (signed) vs logical (unsigned) |
| `abs()` | Solo para signed |
| Constructor `int64_t` | Sign extension |
| `from_string("-123")` | Parsing negativos |
| `operator/`, `%` | División con signos |

**Operations identical (no `if constexpr` needed):**
- `+`, `-`, `*`, `&`, `|`, `^`, `~`, `<<` (same bit operations)
- `==`, `!=`, `<`, `>`, `<=`, `>=` (comparisons between same type)
- `high()`, `low()`, `set_high()`, `set_low()` (accessors)

**Patterns validated in Phase 0.5:**
1. **Value-based optimization** (`if (data[1] == 0)`) - works for both signed/unsigned
2. **Conditional branching** (`if constexpr (std::is_signed_v<T>)`) - zero overhead
3. **Intrinsic selection by case** (umul128 vs mul128) - choose algorithm by characteristics
4. **Uniform sign extension handling** - same logic in all paths

**Success Criteria:**
- ✅ All uint128 + int128 tests pass
- ✅ Benchmarks show equal or better performance
- ✅ Compiles with gcc, clang, msvc, intel
- ✅ Comprehensible error messages
- ✅ Acceptable compilation time

See TODO.md for complete detailed plan.

## Compiler-Specific Considerations

### Intel OneAPI (Most Complex)
```bash
# MUST source Intel environment first in MSYS2:
source scripts/setup_intel_combined.bash x64

# Uses MSVC STL but Intel compiler (icpx/icx)
# Critical env vars: INCLUDE, LIB, LIBPATH (see DEV_ENV_VARS.md)
```

### Intrinsics Detection
Uses `include/intrinsics/compiler_detection.hpp` to select:
- **MSVC**: `_umul128()`, `_udiv128()`, `_BitScanReverse64()`
- **GCC/Clang**: `__builtin_clzll()`, `__builtin_ctzll()`, `__uint128_t` (optional)
- **Fallback**: Portable C++20 implementations

## File Naming Conventions

### Tests & Benchmarks
```
[tests|benchs]/[type]_[feature]_extracted_[tests|benchs].cpp
scripts/build_[type]_[feature]_extracted_[tests|benchs].bash  # Legacy (use build_generic.bash)
scripts/check_[type]_[feature]_extracted_tests.bash           # Legacy (use check_generic.bash)
scripts/run_[type]_[feature]_extracted_benchs.bash            # Legacy (use run_generic.bash)
```

### Demos
```
demos/
├── tutorials/         # 01-13 basic tutorials
├── showcase/          # Interactive demos (cryptography, scientific, performance)
├── examples/          # Real-world use cases (IPv6, UUID, prime factorization)
└── [category]/[demo_name].cpp
```

## Testing & Validation

### Running All Tests
```bash
# Test single feature across all compilers
make check TYPE=uint128 FEATURE=cmath COMPILER=all MODE=all

# Test all features for a type
for f in t traits limits concepts algorithm iostreams cmath numeric ranges format safe thread_safety; do
    make check TYPE=uint128 FEATURE=$f COMPILER=gcc MODE=release
done
```

### Benchmarks Report Format
Benchmarks output **both**:
- Wall-clock time (nanoseconds)
- CPU cycles (via `__rdtsc()` intrinsic)

Output saved to: `build/build_benchs_results/[compiler]/[mode]/[type]_[feature]_results_[timestamp].txt`

## Common Patterns to Maintain

### 1. Trait Specializations Before Stdlib
```cpp
// ALWAYS include traits FIRST in any uint128_t header
#include "uint128_traits_specializations.hpp"
#include <iostream>  // stdlib headers come after
```

### 2. Constexpr + Noexcept Everywhere
```cpp
constexpr uint64_t high() const noexcept { return data[1]; }
constexpr uint64_t low() const noexcept { return data[0]; }
```

### 3. Template Conversions with Integral Check
```cpp
template <typename T>
constexpr void set_high(T value) noexcept {
    static_assert(std::is_integral<T>::value, "T must be integral");
    data[1] = static_cast<uint64_t>(value);
}
```

### 4. Friend Operators for Symmetry
```cpp
// Allows: uint64_t + uint128_t (not just uint128_t + uint64_t)
friend constexpr uint128_t operator+(const uint128_t& lhs, const uint128_t& rhs) noexcept;
```

### 5. Sign Extension Pattern (int128_t)
```cpp
// Constructor with automatic sign extension
template <integral_builtin T>
constexpr uint128_t(T value) noexcept : data{static_cast<uint64_t>(value), 0ull} {
    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            data[1] = ~0ull; // Extend sign for negatives
        }
    }
}
```

### 6. Operator Copy-Modify-Return Pattern
```cpp
// Non-modifying operator delegates to modifying version
template <integral_builtin T>
constexpr uint128_t operator*(T other) const noexcept {
    uint128_t result(*this);
    result *= other;  // Preserves type T for correct template resolution
    return result;
}
```

### 7. Adaptive Algorithm Selection
```cpp
// Choose algorithm based on runtime value
constexpr uint128_t& operator*=(T other) noexcept {
    const uint64_t b = static_cast<uint64_t>(other);
    
    // Fast path: value fits in 64 bits
    if (data[1] == 0) {
        // Use simple umul128 (1 multiplication)
        return *this;
    }
    
    // General case: full 128-bit value
    // Use umul128 + cross-product (2 multiplications)
    return *this;
}
```

### 8. Compile-Time Modulo Optimization
```cpp
// Template specialization avoids runtime division for common moduli
template <uint64_t Rad>
    requires(Rad > 1 && Rad < 64)
constexpr uint128_t mod() const noexcept {
    // Power of 2: use bitmask (0 divisions)
    if constexpr (uint128_mod_details::is_power_of_2(Rad)) {
        constexpr uint64_t mask = Rad - 1;
        return uint128_t(0, low() & mask);
    }
    // Power of 3 (3, 9, 27): specialized algorithm
    else if constexpr (uint128_mod_details::is_power_of_3(Rad)) {
        return uint128_t(0, mod_power_of_3_helper<Rad>());
    }
    // Other cases: optimized generic helper
    else {
        return uint128_t(0, modM_generic_helper<Rad>());
    }
}
```

**Benefit**: Eliminates expensive 128-bit division when modulus known at compile-time.  
**Special cases**: mod3(), mod5(), mod7(), mod10() for ultra-common values.

### 9. Buffer Rotation for Thread Safety
```cpp
const char* to_cstr_base(int base) const {
    // Rotating buffer prevents aliasing in single-threaded code
    static thread_local char buffers[4][64];
    static thread_local int current_buffer = 0;
    
    current_buffer = (current_buffer + 1) % 4;
    char* buffer = buffers[current_buffer];
    // ... conversion logic ...
    return buffer;
}
```

**Pattern**: Allows up to 4 concurrent `to_cstr()` calls in same expression without stomping.  
**Example**: `printf("%s %s %s", a.to_cstr(), b.to_cstr(), c.to_cstr())` works correctly.

## Compiler-Specific Considerations

### Intel OneAPI (Most Complex)
```bash
# MUST source Intel environment first in MSYS2:
source scripts/setup_intel_combined.bash x64

# Uses MSVC STL but Intel compiler (icpx/icx)
# Critical env vars: INCLUDE, LIB, LIBPATH (see DEV_ENV_VARS.md)
```

### Intrinsics Detection
Uses `include/intrinsics/compiler_detection.hpp` to select:
- **MSVC**: `_umul128()`, `_udiv128()`, `_BitScanReverse64()`
- **GCC/Clang**: `__builtin_clzll()`, `__builtin_ctzll()`, `__uint128_t` (optional)
- **Fallback**: Portable C++20 implementations

## Known Issues & Gotchas

1. **Intel linking**: Extremely finicky in MSYS2 environment. Always check `scripts/setup_intel_combined.bash` is sourced.

2. **Thread safety**: `-pthread -latomic` flags are **mandatory** for:
   - `uint128_thread_safety.hpp`
   - Any demo/test using atomic 128-bit operations
   - Auto-detected by build scripts when `<thread>` or `<atomic>` present

3. **MSVC vs GCC differences**:
   - MSVC: `_MSC_VER` macro, uses `<intrin.h>`
   - GCC/Clang: `__GNUC__`/`__clang__`, uses `<immintrin.h>` + `<x86intrin.h>`

4. **Signed integer overflow**: Intentionally uses wrap-around (two's complement), not UB.

5. **Generic scripts**: Always prefer `build_generic.bash` over individual `build_[type]_[feature]_extracted_*.bash` scripts (128 legacy scripts deprecated).

6. **Output paths inconsistency**: Legacy scripts may use different naming patterns:
   - **Modern (preferred)**: `[type]_[feature]_[target]_[compiler].exe`
   - **Legacy (deprecated)**: `[type]_[feature]_extracted_[target]_[compiler].exe`
   - Always use generic scripts (`build_generic.bash`, `check_generic.bash`, `run_generic.bash`) for consistency

7. **make.py vs Makefile**: Both work identically, but `make.py` provides isolated compiler environments.
   - Use `make.py` for CI/CD and when compiler environment contamination is a concern
   - Use `Makefile` for quick local development with traditional make syntax

## Demos System (35+ Demos in 7 Categories)

### Demo Structure
```
demos/
├── general/          # 6 demos - Ad-hoc features & experiments
├── tutorials/        # 16 demos - Sequential learning (01-13 + extras)
├── examples/         # 9 demos - Real-world use cases
├── showcase/         # 4 demos - Advanced demonstrations
├── comparison/       # Comparisons with other libraries
├── performance/      # Performance analysis
└── integration/      # Integration examples
```

**Total**: ~4878 lines of demo code

### Demo Naming Patterns
- **Tutorial demos**: `01_basic_operations.cpp`, `02_bitwise_operations.cpp`, etc.
- **Example demos**: `ipv6_address.cpp`, `uuid_generation.cpp`, `big_integer_calculator.cpp`
- **Showcase demos**: `main.cpp`, `showcase_cryptography.cpp`, `showcase_scientific.cpp`

### Building Demos
```bash
# Single demo (3 ways - all equivalent)
bash scripts/build_demo.bash tutorials 01_basic_operations gcc release
bash scripts/build_generic.bash demos tutorials 01_basic_operations gcc release
python make.py build demos tutorials 01_basic_operations gcc release

# All demos in category
bash scripts/build_all_demos.bash tutorials gcc release

# Run demo
bash scripts/run_demo.bash tutorials 01_basic_operations
make demo CATEGORY=tutorials DEMO=01_basic_operations
python make.py demo tutorials 01_basic_operations
```

### Demo Patterns

#### 1. **Threading Demos** (Automatic flag detection)
Scripts auto-detect `<thread>`, `<atomic>`, `thread_safety.hpp` and add `-pthread -latomic`:
- `demos/examples/demo_int128_thread_safety.cpp`
- `demos/examples/example_thread_safety.cpp`

#### 2. **Interactive Demos** (REPL pattern)
Main loop pattern for interactive exploration:
- `demos/examples/big_integer_calculator.cpp` - Math REPL
- `demos/showcase/main.cpp` - 7 interactive sections

#### 3. **Scientific Computing**
Demos using high-precision arithmetic:
- `demos/showcase/showcase_scientific.cpp` - Factorials, Fibonacci, π approximation
- `demos/examples/mersenne_primes.cpp` - Prime search
- `demos/examples/prime_factorization.cpp` - Pollard's Rho

#### 4. **Networking/Identifiers**
Real-world data structure demos:
- `demos/examples/ipv6_address.cpp` - Full IPv6 class with parsing
- `demos/examples/uuid_generation.cpp` - RFC 4122 UUID v4

### Demo Verification
```bash
# Check all demos compile (generates matrix report)
python make.py check demos all gcc release
bash scripts/test_demos.bash
```

Output example: "35/35 demos compiled successfully (100%)"

## File Naming Conventions

### Nomenclature System
**Variables used throughout:**
- `[action]` = `build` | `check` | `run` - Action to perform on executables
- `[type_base]` = `uint128` | `int128` - Type being compiled
- `[feature]` = `t` | `traits` | `limits` | `concepts` | `algorithm` | `iostreams` | `bits` | `cmath` | `numeric` | `ranges` | `format` | `safe` | `thread_safety` | `comparison_boost` | `interop`
- `[ultimate_target]` = `tests` | `benchs` - Final executable type
- `[compiler]` = `gcc` | `clang` | `intel` | `msvc` | `all` - Compiler to use
- `[mode]` = `debug` | `release` | `all` - Compilation mode
- `[print]` = `yes` | `no` | `` (optional) - Print compilation commands

**Script argument pattern (constructors of executables):**
```bash
# Pattern: [action] [type_base] [feature] [ultimate_target] [compiler] [mode] [print]
scripts/build_generic.bash uint128 cmath tests gcc release yes
scripts/check_generic.bash int128 traits tests all all
scripts/run_generic.bash uint128 algorithm benchs gcc release
```

### Tests & Benchmarks
```
tests/[type_base]_[feature]_extracted_tests.cpp
benchs/[type_base]_[feature]_extracted_benchs.cpp

scripts/build_[type_base]_[feature]_extracted_[ultimate_target].bash  # Legacy
scripts/check_[type_base]_[feature]_extracted_tests.bash               # Legacy
scripts/run_[type_base]_[feature]_extracted_benchs.bash                # Legacy

# Output paths:
build/build_[ultimate_target]/[compiler]/[mode]/[type_base]_[feature]_[ultimate_target]_[compiler].exe
```

### Demos
```
demos/[category]/[demo_name].cpp

# Output paths:
build/build_demos/[compiler]/[mode]/[demo_name].exe
```

### Build Logs
```
# Compilation logs (stdout/stderr):
build_log/[ultimate_target]/[type_base]_[feature]_[compiler]_[mode]_extracted_[ultimate_target]_build_log_[timestamp].txt

# Execution results:
build/build_[ultimate_target]_results/[compiler]/[mode]/[type_base]_[feature]_extracted_[ultimate_target]_results_[timestamp].txt
```

## Documentation

- **Main README**: [README.md](../README.md) - Full usage guide (~1800 lines)
- **Architecture**: In-depth comments in [uint128_t.hpp](../include/uint128/uint128_t.hpp)
- **Portability**: [PORTABILITY_GUIDE.md](../PORTABILITY_GUIDE.md) - Cross-platform considerations
- **Build Conventions**: [PROMPT.md](../PROMPT.md) - Original design spec (1000+ lines)
- **Roadmap**: [TODO.md](../TODO.md) - Phase 1.5 template unification plan
- **Implementation**: [IMPLEMENTATION_SUMMARY.md](../IMPLEMENTATION_SUMMARY.md) - Technical details
- **Demos**: [DEMOS_CATALOG.md](../DEMOS_CATALOG.md) - All 35 demos catalogued

## Quick Start for New Features

1. Create header: `include/uint128/uint128_[feature].hpp`
2. Create test: `tests/uint128_[feature]_extracted_tests.cpp`
3. Create benchmark: `benchs/uint128_[feature]_extracted_benchs.cpp`
4. Build & validate:
   ```bash
   make build_tests TYPE=uint128 FEATURE=[feature] COMPILER=all MODE=all
   make check TYPE=uint128 FEATURE=[feature] COMPILER=all MODE=all
   make run TYPE=uint128 FEATURE=[feature] COMPILER=gcc MODE=release
   ```
5. Add demo to `demos/examples/` or `demos/showcase/` (optional)
6. Update Makefile's `VALID_FEATURES` list if new feature
7. Replicate for int128_t: `include/int128/int128_[feature].hpp` with signed adaptations

## Environment Setup

```bash
# GCC (UCRT64) - Default in MSYS2
export PATH="/ucrt64/bin:$PATH"

# Clang (CLANG64)
export PATH="/clang64/bin:$PATH"

# Intel OneAPI
source scripts/setup_intel_combined.bash x64

# MSVC (from MSYS2 bash)
# Use scripts/build_generic.bash with COMPILER=msvc (auto-detects vcvars64.bat)
```

For detailed compiler setup: [DEV_ENV_VARS.md](../DEV_ENV_VARS.md)

## Scripts Organization

### Directory Structure
```
scripts/
├── build_generic.bash/.py       # Main build scripts (recommended)
├── check_generic.bash/.py       # Main test runners
├── run_generic.bash/.py         # Main benchmark/demo runners
├── build_demo.bash              # Specific demo builder
├── build_all_demos.bash         # Build all demos in category
├── run_demo.bash                # Run specific demo
├── test_demos.bash              # Verify all demos compile
├── catalog_demos.bash           # Generate DEMOS_CATALOG.md
├── init_project.py              # Detect and configure compilers
├── generate_docs.bash           # Generate Doxygen docs
├── env_setup/                   # Compiler environment scripts
│   ├── compiler_env.py          # Python compiler environment manager
│   ├── setup_gcc_env.bash
│   ├── setup_clang_env.bash
│   ├── setup_intel_env.bash
│   └── setup_msvc_env.bash
└── individualized/              # 128 legacy scripts (deprecated)
```

### Key Scripts Usage Patterns

**Generic scripts accept unified syntax:**
```bash
# Tests/Benchs: <type> <feature> <target> [compiler] [mode]
scripts/build_generic.bash uint128 cmath tests gcc release

# Demos: demos <category> <demo_name> [compiler] [mode]
scripts/build_generic.bash demos tutorials 01_basic_operations gcc release
```

**Python equivalent with isolated environments:**
```bash
python scripts/build_generic.py uint128 cmath tests gcc release
python scripts/build_generic.py demos tutorials 01_basic_operations gcc release
```

**Makefile shortcuts:**
```bash
make build_tests TYPE=uint128 FEATURE=cmath COMPILER=gcc MODE=release
make demo CATEGORY=tutorials DEMO=01_basic_operations
```
