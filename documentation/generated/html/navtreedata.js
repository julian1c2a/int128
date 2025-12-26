/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "int128 - 128-bit Integer Types for C++", "index.html", [
    [ "int128 - Implementación de enteros de 128 bits", "index.html", "index" ],
    [ "architecture", "md__2ucrt64_2home_2julian_2_cpp_projects_2int128_2documentation_2doxygen_2pages_2architecture.html", null ],
    [ "Arquitectura del Proyecto", "architecture.html", [
      [ "Vista General", "architecture.html#arch_overview", null ],
      [ "Tipos Principales", "architecture.html#arch_types", [
        [ "uint128_t (Unsigned)", "architecture.html#arch_uint128", null ],
        [ "int128_t (Signed)", "architecture.html#arch_int128", null ]
      ] ],
      [ "Representación Interna", "architecture.html#arch_storage", null ],
      [ "Optimizaciones", "architecture.html#arch_optimization", [
        [ "Intrínsecos", "architecture.html#arch_intrinsics", null ],
        [ "Evaluación en Tiempo de Compilación", "architecture.html#arch_constexpr", null ],
        [ "Prevención de Errores", "architecture.html#arch_nodiscard", null ]
      ] ],
      [ "Type Traits y Conceptos", "architecture.html#arch_traits", [
        [ "Especialización de Traits STL", "architecture.html#arch_std_traits", null ],
        [ "C++20 Concepts", "architecture.html#arch_concepts", null ]
      ] ],
      [ "Thread Safety", "architecture.html#arch_thread_safety", [
        [ "Soporte Atómico", "architecture.html#arch_atomic", null ],
        [ "Lock-Free", "architecture.html#arch_lock_free", null ]
      ] ],
      [ "Testing y Validación", "architecture.html#arch_testing", [
        [ "Suite de Tests", "architecture.html#arch_test_suite", null ],
        [ "Benchmarks", "architecture.html#arch_benchmarks", null ]
      ] ],
      [ "Sistema de Build", "architecture.html#arch_build", [
        [ "CMake", "architecture.html#arch_cmake", null ],
        [ "Makefile Genérico", "architecture.html#arch_makefile", null ]
      ] ],
      [ "Documentación", "architecture.html#arch_documentation", [
        [ "Doxygen", "architecture.html#arch_doxygen", null ],
        [ "Markdown", "architecture.html#arch_markdown", null ]
      ] ],
      [ "CI/CD", "architecture.html#arch_ci_cd", [
        [ "GitHub Actions", "architecture.html#arch_github_actions", null ]
      ] ],
      [ "Roadmap Futuro", "architecture.html#arch_future", [
        [ "Características Planeadas", "architecture.html#arch_planned", null ],
        [ "Compatibilidad Futura", "architecture.html#arch_compatibility", null ]
      ] ]
    ] ],
    [ "examples", "md__2ucrt64_2home_2julian_2_cpp_projects_2int128_2documentation_2doxygen_2pages_2examples.html", null ],
    [ "Ejemplos de Uso", "examples.html", [
      [ "Ejemplos Prácticos", "examples.html#examples_intro", null ],
      [ "Ejemplos Básicos", "examples.html#examples_basic", [
        [ "Calculadora Básica", "examples.html#ex_basic_calc", null ],
        [ "Conversiones", "examples.html#ex_conversions", null ]
      ] ],
      [ "Ejemplos Matemáticos", "examples.html#examples_math", [
        [ "Factorial", "examples.html#ex_factorial", null ],
        [ "Números de Fibonacci", "examples.html#ex_fibonacci", null ]
      ] ],
      [ "Casos de Uso Reales", "examples.html#examples_real_world", [
        [ "Direcciones IPv6", "examples.html#ex_ipv6", null ],
        [ "UUIDs (128-bit)", "examples.html#ex_uuid", null ]
      ] ],
      [ "Algoritmos y STL", "examples.html#examples_algorithms", [
        [ "Ordenamiento", "examples.html#ex_sorting", null ],
        [ "Rangos y Algoritmos", "examples.html#ex_ranges", null ]
      ] ],
      [ "Optimización y Rendimiento", "examples.html#examples_performance", [
        [ "Uso de Intrínsecos", "examples.html#ex_intrinsics", null ]
      ] ],
      [ "Más Ejemplos", "examples.html#examples_more", null ]
    ] ],
    [ "quickstart", "md__2ucrt64_2home_2julian_2_cpp_projects_2int128_2documentation_2doxygen_2pages_2quickstart.html", null ],
    [ "Guía de Inicio Rápido", "quickstart.html", [
      [ "Introducción", "quickstart.html#qs_intro", null ],
      [ "Instalación", "quickstart.html#qs_install", [
        [ "Header-Only", "quickstart.html#qs_install_header", null ],
        [ "Con CMake", "quickstart.html#qs_install_cmake", null ]
      ] ],
      [ "Uso Básico", "quickstart.html#qs_basic_usage", [
        [ "Creación de Números", "quickstart.html#qs_creation", null ],
        [ "Operaciones Aritméticas", "quickstart.html#qs_arithmetic", null ],
        [ "Operaciones Bitwise", "quickstart.html#qs_bitwise", null ],
        [ "Comparaciones", "quickstart.html#qs_comparison", null ],
        [ "Entrada/Salida", "quickstart.html#qs_io", null ],
        [ "Formateo Avanzado", "quickstart.html#qs_format", null ]
      ] ],
      [ "Funciones Matemáticas", "quickstart.html#qs_math", [
        [ "Funciones cmath", "quickstart.html#qs_cmath", null ],
        [ "Funciones Numéricas", "quickstart.html#qs_numeric", null ]
      ] ],
      [ "Características Avanzadas", "quickstart.html#qs_advanced", [
        [ "Operaciones Seguras", "quickstart.html#qs_safe", null ],
        [ "Thread Safety", "quickstart.html#qs_threads", null ],
        [ "Algoritmos STL", "quickstart.html#qs_algorithms", null ]
      ] ],
      [ "Siguientes Pasos", "quickstart.html#qs_next", null ],
      [ "Soporte y Compiladores", "quickstart.html#qs_support", [
        [ "Compiladores Soportados", "quickstart.html#qs_compilers", null ],
        [ "Plataformas", "quickstart.html#qs_platforms", null ],
        [ "Estándar C++", "quickstart.html#qs_cpp_version", null ]
      ] ]
    ] ],
    [ "Lista de pruebas", "test.html", null ],
    [ "Espacios de nombres", "namespaces.html", [
      [ "Lista de espacios de nombres", "namespaces.html", "namespaces_dup" ],
      [ "Miembros del espacio de nombres ", "namespacemembers.html", [
        [ "Todos", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Funciones", "namespacemembers_func.html", "namespacemembers_func" ],
        [ "Enumeraciones", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Conceptos", "concepts.html", "concepts" ],
    [ "Clases", "annotated.html", [
      [ "Lista de clases", "annotated.html", "annotated_dup" ],
      [ "Índice de clases", "classes.html", null ],
      [ "Jerarquía de clases", "hierarchy.html", "hierarchy" ],
      [ "Miembros de clases", "functions.html", [
        [ "Todos", "functions.html", "functions_dup" ],
        [ "Funciones", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "typedefs", "functions_type.html", null ]
      ] ]
    ] ],
    [ "Archivos", "files.html", [
      [ "Lista de archivos", "files.html", "files_dup" ],
      [ "Miembros de los archivos", "globals.html", [
        [ "Todos", "globals.html", null ],
        [ "Funciones", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "defines", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Ejemplos", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"_2ucrt64_2home_2julian_2_cpp_projects_2int128_2include_2uint128_2uint128_t_8hpp-example.html",
"classstd_1_1numeric__limits_3_01int128__t_01_4.html#a88403f551943bc90ea3c5276e898d76c",
"classuint128__threadsafe_1_1_thread_safe_uint128_r_w.html#ac63b3873257f20b7cb8bb6347e740527",
"int128__factory_8hpp.html#a23d79d9b3582a7bf14cfb92af83e9fde",
"namespacestd.html#a9a37eb52ae266dff2470a5fe8b8d3505",
"structstd_1_1common__type_3_01int128__t_00_01uint32__t_01_4.html#ae67e660c18af6a591d048b6d3a5f9e6d",
"uint128__t_8hpp.html#a3595211ad92f4a63690cdbc8c0fd2109"
];

var SYNCONMSG = 'pulsar para deshabilitar sincronización';
var SYNCOFFMSG = 'pulsar para habilitar sincronización';
var LISTOFALLMEMBERS = 'Lista de todos los miembros';