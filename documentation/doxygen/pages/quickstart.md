/**
@page quickstart Guía de Inicio Rápido

@tableofcontents

@section qs_intro Introducción

La biblioteca **int128** proporciona dos tipos de enteros de 128 bits para C++20:
- `uint128_t`: Entero sin signo (0 a 2^128-1)
- `int128_t`: Entero con signo (-2^127 a 2^127-1)

@section qs_install Instalación

@subsection qs_install_header Header-Only

Esta es una biblioteca **header-only**. Solo necesitas incluir los headers:

@code{.cpp}
#include <uint128/uint128_t.hpp>  // Para uint128_t
#include <int128/int128_t.hpp>    // Para int128_t
@endcode

@subsection qs_install_cmake Con CMake

@code{.cmake}
# En tu CMakeLists.txt
add_subdirectory(path/to/int128)
target_link_libraries(tu_proyecto PRIVATE int128)
@endcode

@section qs_basic_usage Uso Básico

@subsection qs_creation Creación de Números

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>

int main() {
    // Usando literales definidos por el usuario (UDL)
    auto a = 1000_u128;
    auto b = 2000_u128;
    
    // Desde enteros nativos
    uint128_t c{100};
    uint128_t d = 200;
    
    // Desde strings
    auto e = uint128_t::from_string("999999999999999999999999");
    
    std::cout << "a = " << a << "\n";
    std::cout << "e = " << e.value() << "\n";
}
@endcode

@subsection qs_arithmetic Operaciones Aritméticas

@code{.cpp}
auto sum = a + b;           // Suma
auto diff = a - b;          // Resta
auto product = a * b;       // Multiplicación
auto quotient = b / a;      // División
auto remainder = b % a;     // Módulo

// Operadores compuestos
a += 100;
b *= 2;
c /= 5;
@endcode

@subsection qs_bitwise Operaciones Bitwise

@code{.cpp}
auto and_result = a & b;    // AND
auto or_result = a | b;     // OR
auto xor_result = a ^ b;    // XOR
auto not_result = ~a;       // NOT

auto shifted_left = a << 5; // Shift izquierda
auto shifted_right = a >> 3;// Shift derecha
@endcode

@subsection qs_comparison Comparaciones

@code{.cpp}
if (a == b) { /* ... */ }
if (a != b) { /* ... */ }
if (a < b)  { /* ... */ }
if (a <= b) { /* ... */ }
if (a > b)  { /* ... */ }
if (a >= b) { /* ... */ }

// Comparación de tres vías (C++20)
auto cmp = a <=> b;
@endcode

@subsection qs_io Entrada/Salida

@code{.cpp}
#include <uint128/uint128_iostreams.hpp>

uint128_t num;
std::cout << "Ingresa un número: ";
std::cin >> num;

// Salida con manipuladores
std::cout << std::hex << num << "\n";    // Hexadecimal
std::cout << std::oct << num << "\n";    // Octal
std::cout << std::dec << num << "\n";    // Decimal
@endcode

@subsection qs_format Formateo Avanzado

@code{.cpp}
#include <uint128/uint128_format.hpp>

auto num = 255_u128;

std::cout << format_hex(num) << "\n";    // 0xff
std::cout << format_bin(num) << "\n";    // 0b11111111
std::cout << format_oct(num) << "\n";    // 0377

// Con ancho y relleno
std::cout << format_hex(num, 8, '0') << "\n"; // 0x000000ff
@endcode

@section qs_math Funciones Matemáticas

@subsection qs_cmath Funciones cmath

@code{.cpp}
#include <uint128/uint128_cmath.hpp>

auto num = 16_u128;
auto sq = sqrt(num);           // 4
auto cb = cbrt(27_u128);       // 3
auto pw = pow(2_u128, 10);     // 1024

auto a = 48_u128, b = 18_u128;
auto g = gcd(a, b);            // 6
auto l = lcm(a, b);            // 144
@endcode

@subsection qs_numeric Funciones Numéricas

@code{.cpp}
#include <uint128/uint128_numeric.hpp>

auto mid = midpoint(100_u128, 200_u128);  // 150
auto clamped = clamp(value, min, max);
auto abs_val = abs(-10_i128);              // int128_t
@endcode

@section qs_advanced Características Avanzadas

@subsection qs_safe Operaciones Seguras

@code{.cpp}
#include <uint128/uint128_safe.hpp>

auto result = safe_add(a, b);
if (result.has_value()) {
    std::cout << "Suma: " << result.value() << "\n";
} else {
    std::cout << "Overflow detectado!\n";
}
@endcode

@subsection qs_threads Thread Safety

@code{.cpp}
#include <uint128/uint128_atomic.hpp>
#include <thread>

std::atomic<uint128_t> counter{0};

void increment() {
    for (int i = 0; i < 1000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    
    std::cout << "Counter: " << counter.load() << "\n"; // 2000
}
@endcode

@subsection qs_algorithms Algoritmos STL

@code{.cpp}
#include <uint128/uint128_algorithm.hpp>
#include <vector>
#include <algorithm>

std::vector<uint128_t> nums = {5_u128, 2_u128, 8_u128, 1_u128};

// Ordenar
std::sort(nums.begin(), nums.end());

// Buscar
auto it = std::find(nums.begin(), nums.end(), 8_u128);

// Acumular
auto sum = std::accumulate(nums.begin(), nums.end(), 0_u128);
@endcode

@section qs_next Siguientes Pasos

- @ref examples "Ver Ejemplos Completos"
- @ref architecture "Arquitectura del Proyecto"
- @ref benchmarks "Análisis de Rendimiento"
- @ref api "Referencia API Completa"

@section qs_support Soporte y Compiladores

@subsection qs_compilers Compiladores Soportados

- **GCC**: 10+ (Recomendado: 13+)
- **Clang**: 12+ (Recomendado: 15+)
- **MSVC**: Visual Studio 2022+ (17.0+)
- **Intel ICX**: oneAPI 2023+

@subsection qs_platforms Plataformas

- **Windows**: x86_64 (MSYS2, Visual Studio)
- **Linux**: x86_64, ARM64
- **macOS**: x86_64, ARM64 (Apple Silicon)

@subsection qs_cpp_version Estándar C++

Requiere **C++20** o superior:

@code{.cmake}
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
@endcode

*/
