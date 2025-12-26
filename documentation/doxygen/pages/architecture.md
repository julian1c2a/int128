/**
@page architecture Arquitectura del Proyecto

@tableofcontents

@section arch_overview Vista General

El proyecto int128 está organizado en módulos especializados para máxima eficiencia y mantenibilidad.

@section arch_types Tipos Principales

@subsection arch_uint128 uint128_t (Unsigned)

Implementación principal de entero sin signo de 128 bits.

**Archivo principal**: `include/uint128/uint128_t.hpp`

**Módulos complementarios**:
- `uint128_limits.hpp` - Especialización de `std::numeric_limits`
- `uint128_traits.hpp` - Type traits y `std::hash`
- `uint128_concepts.hpp` - Concepts de C++20
- `uint128_algorithm.hpp` - Algoritmos optimizados
- `uint128_numeric.hpp` - Funciones numéricas (`midpoint`, `clamp`, etc.)
- `uint128_ranges.hpp` - Soporte para rangos STL
- `uint128_format.hpp` - Formateo avanzado (hex, oct, bin)
- `uint128_safe.hpp` - Operaciones con detección de overflow
- `uint128_iostreams.hpp` - Operadores de stream
- `uint128_cmath.hpp` - Funciones matemáticas (`sqrt`, `pow`, `gcd`, etc.)

@subsection arch_int128 int128_t (Signed)

Implementación de entero con signo de 128 bits.

**Archivo principal**: `include/int128/int128_t.hpp`

**Arquitectura modular**:
- `int128_format.hpp` - Formateo (incluye `showpos`)
- `int128_algorithm.hpp` - Algoritmos específicos para signed
- `int128_arithmetic.hpp` - Operaciones aritméticas
- `int128_bitwise.hpp` - Operaciones bitwise
- `int128_cmath.hpp` - Funciones matemáticas para signed

@section arch_storage Representación Interna

@code{.cpp}
class uint128_t {
    uint64_t data[2];
    // data[1] -> High 64 bits (Little Endian)
    // data[0] -> Low 64 bits (Little Endian)
};
@endcode

**Ventajas de esta representación**:
- Compatible con layout de memoria de `__uint128_t` (GCC/Clang)
- Alineación óptima para operaciones de 64 bits
- Soporte directo para intrínsecos x86_64
- Portabilidad entre plataformas

@section arch_optimization Optimizaciones

@subsection arch_intrinsics Intrínsecos

La biblioteca utiliza intrínsecos específicos de la plataforma cuando están disponibles:

**x86_64 (Intel/AMD)**:
- `_mulx_u64` - Multiplicación 64x64→128
- `_addcarry_u64` - Suma con carry
- `_subborrow_u64` - Resta con borrow
- `__builtin_clzll` - Count leading zeros
- `__builtin_ctzll` - Count trailing zeros

**Fallback genérico**:
Cuando los intrínsecos no están disponibles, se usa código C++ portable.

@subsection arch_constexpr Evaluación en Tiempo de Compilación

Casi todas las operaciones son `constexpr`, permitiendo:

@code{.cpp}
constexpr uint128_t factorial_5 = []() {
    uint128_t result = 1;
    for (int i = 2; i <= 5; ++i) {
        result *= i;
    }
    return result;
}();  // Evaluado en tiempo de compilación!
@endcode

@subsection arch_nodiscard Prevención de Errores

Funciones críticas usan `[[nodiscard]]`:

@code{.cpp}
[[nodiscard]] std::optional<uint128_t> safe_add(uint128_t a, uint128_t b);
[[nodiscard]] std::optional<uint128_t> from_string(const std::string& str);
@endcode

El compilador advierte si ignoras el resultado.

@section arch_traits Type Traits y Conceptos

@subsection arch_std_traits Especialización de Traits STL

@code{.cpp}
namespace std {
    template<>
    struct is_integral<uint128_t> : true_type {};
    
    template<>
    struct is_unsigned<uint128_t> : true_type {};
    
    template<>
    struct is_signed<int128_t> : true_type {};
    
    template<>
    struct numeric_limits<uint128_t> {
        static constexpr uint128_t min() { return 0; }
        static constexpr uint128_t max() { return ~uint128_t{0}; }
        // ... más traits
    };
}
@endcode

@subsection arch_concepts C++20 Concepts

@code{.cpp}
template<typename T>
concept uint128_convertible = 
    std::is_integral_v<T> || 
    std::same_as<T, uint128_t> ||
    std::same_as<T, int128_t>;

template<uint128_convertible T>
uint128_t process(T value) {
    return uint128_t{value};
}
@endcode

@section arch_thread_safety Thread Safety

@subsection arch_atomic Soporte Atómico

@code{.cpp}
#include <uint128/uint128_atomic.hpp>

std::atomic<uint128_t> counter{0};

// Lock-free en x86_64 con soporte de cmpxchg16b
// Fallback a mutex en otras plataformas
counter.fetch_add(1, std::memory_order_relaxed);
@endcode

@subsection arch_lock_free Lock-Free

En plataformas con instrucción `CMPXCHG16B` (x86_64):
- `std::atomic<uint128_t>` es **lock-free**
- `is_lock_free()` devuelve `true`
- Rendimiento óptimo en multithreading

@section arch_testing Testing y Validación

@subsection arch_test_suite Suite de Tests

**Ubicación**: `tests/`

- `uint128_extracted_tests.cpp` - Suite completa uint128_t (1000+ tests)
- `int128_extracted_tests.cpp` - Suite completa int128_t (800+ tests)
- `interop_tests.cpp` - Tests de interoperabilidad
- `thread_safety_tests.cpp` - Tests de concurrencia

**Framework**: Catch2 v3

@subsection arch_benchmarks Benchmarks

**Ubicación**: `benchs/`

- Comparación vs `__uint128_t` (GCC/Clang builtin)
- Comparación vs Boost.Multiprecision
- Análisis de intrínsecos vs código genérico
- Benchmarks por operación (add, mul, div, etc.)

@section arch_build Sistema de Build

@subsection arch_cmake CMake

@code{.cmake}
# Mínimo CMake 3.20
cmake_minimum_required(VERSION 3.20)

project(int128 CXX)

# Requiere C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only library
add_library(int128 INTERFACE)
target_include_directories(int128 INTERFACE include/)
@endcode

@subsection arch_makefile Makefile Genérico

Sistema de build alternativo con soporte multi-compilador:
- GCC, Clang, MSVC, Intel ICX
- Detección automática de compilador
- Múltiples targets (debug, release, benchmarks, tests)

@section arch_documentation Documentación

@subsection arch_doxygen Doxygen

- Generación automática de API reference
- Búsqueda integrada
- Gráficos de clases (requiere Graphviz)
- Exportación a HTML, LaTeX, XML

**Generar documentación**:
@code{.bash}
doxygen Doxyfile
# Output en: documentation/generated/html/
@endcode

@subsection arch_markdown Markdown

- `README.md` - Documentación principal
- `documentation/` - Guías adicionales
- Inline en código fuente

@section arch_ci_cd CI/CD

@subsection arch_github_actions GitHub Actions

**Tests automáticos** en:
- Ubuntu (GCC, Clang)
- Windows (MSVC, MinGW)
- macOS (Clang, GCC Homebrew)

**Workflow**:
1. Build en múltiples compiladores
2. Ejecución de tests
3. Benchmarks de rendimiento
4. Generación de documentación
5. Publicación en GitHub Pages

@section arch_future Roadmap Futuro

@subsection arch_planned Características Planeadas

- ✅ Soporte completo de `std::format` (C++20)
- ✅ Concepts adicionales para generic programming
- ⏳ Optimizaciones específicas para ARM NEON
- ⏳ Soporte para AVX-512 (multiplicación vectorizada)
- ⏳ Conversión directa a/desde `std::bitset<128>`
- ⏳ Serialización (binaria, JSON, XML)

@subsection arch_compatibility Compatibilidad Futura

- C++23 features cuando esté disponible
- Soporte para módulos C++20 (cuando los compiladores maduren)
- Interoperabilidad con `std::int128_t` si se estandariza

*/
