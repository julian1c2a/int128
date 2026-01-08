# PROMPT.md - Especificación Técnica del Proyecto int128

> 📋 **Documentos relacionados:** [CHANGELOG.md](CHANGELOG.md) | [TODO.md](TODO.md) | [README.md](README.md) | [API_INT128_BASE_T.md](API_INT128_BASE_T.md)
>
> ⏰ **Última actualización:** 2026-01-07 23:30

---

## 🔄 FASE 1.5: Arquitectura Template Unificada (EN PROGRESO)

**Rama actual:** `phase-1.5-template-unification`

### Nueva Arquitectura de Headers

A partir de Fase 1.5, los tipos `uint128_t` e `int128_t` se definen como **type aliases** de un template unificado:

```cpp
// include_new/int128_base_tt.hpp
namespace nstd {
    enum class signedness : bool { unsigned_type = false, signed_type = true };
    
    template<signedness S>
    class int128_base_t { /* implementación unificada */ };
    
    using uint128_t = int128_base_t<signedness::unsigned_type>;
    using int128_t = int128_base_t<signedness::signed_type>;
}
```

### Nomenclatura Actualizada (Fase 1.5+)

| Variable | Valores | Descripción |
|----------|---------|-------------|
| **[signedness]** | `unsigned` \| `signed` | Distingue tipo signed/unsigned |
| **[limbs]** (futuro) | `2` \| `4` \| `8` | Número de palabras de 64 bits (128/256/512 bits) |
| **[feature]** | Ver lista | Característica específica |

**Nomenclatura de headers nueva (Fase 1.5):**

```
include/
├── int128_base.hpp              # Template unificado principal
├── int128_base_[feature].hpp    # Features del template unificado
└── type_traits.hpp              # Concepts y traits
```

**Nomenclatura legacy (pre-1.5, deprecada pero mantenida):**

```
include/
├── uint128/uint128_[feature].hpp
└── int128/int128_[feature].hpp
```

---

## 🔒 Sanitizadores, Hardening y Análisis Estático

### Flags de Sanitización (GCC/Clang)

| Sanitizador | Flag | Propósito |
|-------------|------|----------|
| AddressSanitizer | `-fsanitize=address` | Buffer overflow, use-after-free |
| UndefinedBehaviorSan | `-fsanitize=undefined` | Comportamiento indefinido |
| ThreadSanitizer | `-fsanitize=thread` | Data races |
| MemorySanitizer | `-fsanitize=memory` | Memoria no inicializada (solo Clang) |

### Flags de Hardening

| Flag | Propósito |
|------|----------|
| `-fstack-protector-strong` | Protección de stack |
| `-fcf-protection=full` | Control Flow Integrity |
| `-fPIE -pie` | Position Independent Executable |
| `-D_FORTIFY_SOURCE=2` | Protección funciones libc |

### Analizadores Estáticos

| Herramienta | Comando | Instalación MSYS2 |
|-------------|---------|------------------|
| **cppcheck** | `cppcheck --enable=all include/` | `pacman -S cppcheck` |
| **clang-tidy** | `clang-tidy file.cpp --` | Incluido con Clang |
| **PVS-Studio** | Integración IDE | Gratis para OSS |
| **Infer** | `infer run -- g++ file.cpp` | `pacman -S infer` |

### Modos de Compilación con Sanitizadores

```bash
# Modo debug con AddressSanitizer
[mode]=debug-asan     -> -O0 -g -fsanitize=address

# Modo debug con UBSan
[mode]=debug-ubsan    -> -O0 -g -fsanitize=undefined

# Modo debug con ThreadSanitizer
[mode]=debug-tsan     -> -O0 -g -fsanitize=thread
```

---

Seguiremos con haciendo pruebas extractadas por cada función de uint128_traits.hpp, poniéndoles nombre test_(function name).cpp, y en un archivo tests/uint128_traits_extracted_tests.cpp y pruebas de benchmark en benchs/uint128_traits_extracted_benchs.cpp. Los tests y los benchs han de compilarse con los 4 compiladores que llevamos g++ en /c/msys64/ucrt64/bin/ (includes y libs también en /c/msys64/ucrt64), clang en /c/msys64/clang64/bin/ (includes y libs también en /c/msys64/clang64), y con Intel OneApi, en C:\Program Files (x86)\Intel\oneAPI, como con msvc en C:\Program Files\Microsoft Visual Studio\18\Community\ . Los benchmarks han de dar tanto tiempos como ciclos de reloj.

Tengo cuatro terminales msys/bash abiertas, uno para cada compilador (gnu g++, llvm clang++, intel icpx/icx, msvc cl). En cada terminal navego a la raíz del proyecto y ejecuto el script de build (o la acción correspondiente).

Las acciones que se hagan tienen una finalidad última entre tres: testear, medir tiempos/ciclos de reloj (benchmarking), o demostrar funcionalidades. Por tanto, las acciones se dividen en tres grandes grupos: tests, benchmarks y demos. A estas tres finalidades últimas corresponden tres valores de la variable [ultimate_target]:
     - [ultimate_target]=["tests"|"benchs"|"demos"]
     - "tests"  ->  testear las funcionalidades del proyecto
     - "benchs" ->  medir tiempos y ciclos de reloj de las funcionalidades del proyecto
     - "demos"  ->  demostrar funcionalidades del proyecto con ejemplos interactivos

Para demos, existe una estructura adicional de categorías:
     - [demo_category]=[tutorials|examples|showcase|general|comparison|performance|integration]
     - root_project/demos/[demo_category]/[demo_name].cpp

Los scripts de demos siguen el patrón:
     - scripts/build_demo.bash [demo_category] [demo_name] [compiler] [mode]
     - scripts/run_demo.bash [demo_category] [demo_name] [compiler] [mode]
     - make demo CATEGORY=[demo_category] DEMO=[demo_name] COMPILER=[compiler] MODE=[mode]

Los archivos header están en el directorio:
    - root_project/include [directorio de headers del proyecto]
Hay un archivo header específico unificador para los dos tipos que se codifican en este proyecto, uint128_t e int128_t, se unifican en:
    - root_project/include/int128.hpp
Las codificaciones específicas para cada tipo definidio están en los siguientes directorios:
     - root_project/include/uint128/ [para el tipo uint128_t]
     - root_project/include/int128/  [para el tipo int128_t]
Los headers nucleares son:
     - root_project/include/uint128/uint128_t.hpp
     - root_project/include/int128/int128_t.hpp
Codificaremos el comportamiento de los nombres (dependientes del tipo) en:
     - root_project/include/[type_base]/[type_base]_[t].hpp
     - root_project/include/[type_base]/[type_base]_[traits].hpp
     - root_project/include/[type_base]/[type_base]_[limits].hpp
     - root_project/include/[type_base]/[type_base]_[concepts].hpp
     - root_project/include/[type_base]/[type_base]_[algorithms].hpp
     - root_project/include/[type_base]/[type_base]_[iostreams].hpp
     - root_project/include/[type_base]/[type_base]_[bits].hpp
     - root_project/include/[type_base]/[type_base]_[cmath].hpp
     - root_project/include/[type_base]/[type_base]_[numeric].hpp
     - root_project/include/[type_base]/[type_base]_[ranges].hpp
     - root_project/include/[type_base]/[type_base]_[format].hpp
     - root_project/include/[type_base]/[type_base]_[safe].hpp
     - root_project/include/[type_base]/[type_base]_[thread_safety].hpp
Esto se puede codificar aún más, llamando a la característica feature:
     [feature] = [t|traits|limits|concepts|algorithms|iostreams|bits|cmath|numeric|ranges|format|safe|thread_safety]
, y siendo type_base:
     [type_base]=[uint128|int128]
, quedando finalmente la codificación de los nombres y rutas de los headers como:
     - root_project/include/[type_base]/[type_base]_[feature].hpp

Los nombres y rutas de los archivos de tests serán:
     - root_project/tests/[type_base]_[feature]_extracted_tests.cpp
Y los nombres y rutas de los archivos de benchmarks serán:
     - root_project/benchs/[type_base]_[feature]_extracted_benchs.cpp
Los nombres y rutas de los scripts de build de tests serán:
     - root_project/scripts/build_[type_base]_[feature]_extracted_tests.bash
Y los nombres y rutas de los scripts de build de benchmarks serán:
     - root_project/scripts/build_[type_base]_[feature]_extracted_benchs.bash
Los nombres y rutas de los scripts de ejecución de tests serán:
     - root_project/scripts/check_[type_base]_[feature]_extracted_tests.bash
Y los nombres y rutas de los scripts de ejecución de benchmarks serán:
     - root_project/scripts/run_[type_base]_[feature]_extracted_benchs.bash

Compilo mediante dos scripts: scripts/build_[type base]_[feature]_extracted_[ultimate_target == "tests"].bash y scripts/build_[type_base]_[feature]_extracted_[ultimate_target == "benchs].bash, que se encuentran en la raíz del proyecto. El actual scripts se llama scripts/build_[type_base]_[feature]_extracted_[ultimate_target].bash. Hay que pasarle un argumento (no vale uno por defecto) que es el compilador a usar: intel, msvc, gcc, clang u all. Hay que pasarle obligatoriamente un segundo argumento que es el modo de compilación (ver tabla abajo). Si se pasa all como segundo argumento, compila en ambos modos básicos (debug y release). Además hay un tercer argumento opcional "print" que si se pasa hace que se impriman en un archivo de texto los resultados de los tests. La estructura de los argumentos es:
    - [compiler]=[intel|msvc|gcc|clang|all].
    - [mode]=[debug|release|release-O0|release-O1|release-O2|release-O3|all].
    - ["print"|""]

     Modos de Compilación y Flags de Optimización
     =============================================
     
     | Modo        | GCC/Clang (MSYS2/WSL) | MSVC (Windows)      | Intel icx (Windows) | Intel icpx (Linux) |
     |-------------|----------------------|---------------------|-------------|---------------------|
     | debug       | -O0 -g               | /Od /Zi             | /Od /Zi             | -O0 -g              |
     | release     | -O2                  | /O2                 | /O2                 | -O2                 |
     | release-O0  | -O0                  | /Od                 | /Od                 | -O0                 |
     | release-O1  | -O1                  | /O1                 | /O1                 | -O1                 |
     | release-O2  | -O2                  | /O2                 | /O2                 | -O2                 |
     | release-O3  | -O3                  | /Ox                 | /O3                 | -O3                 |
     
     Flags adicionales para benchmarks (GCC/Clang):
     - -faggressive-loop-optimizations (solo GCC, incluido en -O3)
     - -fexpensive-optimizations (solo GCC, incluido en -O2/-O3)
     - -march=native (optimizar para CPU actual)
     - -mtune=native (ajustar para CPU actual)
     
     Flags adicionales para benchmarks (MSVC):
     - /GL (optimización global, whole program optimization)
     - /Ot (favorecer velocidad)
     - /Oi (generar intrínsecos)
     
     Flags adicionales para benchmarks (Intel):
     - -xHost / /QxHost (optimizar para CPU actual)
     - -ipo / /Qipo (optimización interprocedural)

La llamada queda como:
     - scripts/build_[type_base]_[feature]_extracted_[ultimate_target].bash [compiler] [mode] ["print"|""]

Los directorios de los builds de compilación ejecutables se llamarán:
     - build/build_[ultimate_target]/[compiler]/[mode]/[type_base]_[feature]_extracted_[ultimate_target].exe

Si se ha explicitado el argumento "print" en la llamada al script de build de test o de benchmark, se generará un archivo de texto con los resultados de los tests o benchmarks respectivamente:
     - build_log/[ultimate_target]/[type_base]_[feature]_[compiler]_[mode]_extracted_[ultimate_target]_build_log_[timestamp].txt

Para correr los tests y benchmarks uso dos scripts: scripts/check_[type_base]_[feature]_extracted_[ultimate_target].bash y scripts/run_[type_base]_[feature]_extracted_[ultimate_target].bash, que se encuentran en la raíz del proyecto. Al igual que los scripts de build, estos scripts reciben como primer argumento el compilador a usar: intel, msvc, gcc, clang u all (no se admite argumento compiler por defecto). Y como segundo argumento obligatorio el modo de compilación: debug, release u all. La estructura de los argumentos de llamada es:
    - [action]=["check"|"run"].
    - [type_base]=[uint128|int128].
    - [feature]=[t|traits|limits|concepts|algorithms|iostreams|bits|cmath|numeric|ranges|format|safe|thread_safety].
    - [compiler]=[intel|msvc|gcc|clang|all].
    - [mode]=[debug|release|release-O0|release-O1|release-O2|release-O3|all].
    - ["print"|""]
La llamada queda como:
     - scripts/[action]_[type_base]_[feature]_extracted_[ultimate_target].bash [compiler] [mode] ["print"|""]
Los resultados de la llamada serán los resultados de los tests o benchmarks respectivamente, que se muestran en terminal, y si se pasa el argumento "print" (opcional) se volcará la salida también en:
     - build/build_[ultimate_target]_results/[compiler]/[mode]/[type_base]_[feature]_extracted_[ultimate_target]_results_[timestamp].txt

 Las compilaciones con el compilador del intel son especialmente críticas ya que estamos un entorno muy cruzado (msys2 bash + Intel Compiler icpx/icx + STL Visual Studio libs). Hay que asegurarse de que todo enlace y funcione correctamente. Para que compilen todos llamo al siguiente script desde MSYS2/bash terminal:
     - source (scripts/setup_intel_combined.bash x64)

     Estructura de directorios para logs y resultados
     =================================================
     La organización de archivos de salida se divide claramente en dos categorías:
     
     A) LOGS DE COMPILACIÓN (stdout/stderr del proceso de compilación):
        - Ubicación: build_log/[ultimate_target]/
        - Generados por: scripts/build_*_extracted_[ultimate_target].bash con argumento "print"
        - Contenido: Salida del compilador, warnings, errores de compilación
        - Formato: [type_base]_[feature]_[compiler]_[mode]_extracted_[ultimate_target]_build_log_[timestamp].txt
        - Ejemplos:
          * build_log/tests/uint128_traits_gcc_debug_extracted_tests_build_log_20231224_153045.txt
          * build_log/benchs/int128_cmath_msvc_release_extracted_benchs_build_log_20231224_154112.txt
     
     B) RESULTADOS DE EJECUCIÓN (salida de los programas compilados):
        - Ubicación: build/build_[ultimate_target]_results/[compiler]/[mode]/
        - Generados por: scripts/check_*_extracted_tests.bash o scripts/run_*_extracted_benchs.bash con argumento "print"
        - Contenido: Resultados de tests o mediciones de benchmarks (tiempos, ciclos de reloj)
        - Formato: [name_header]_extracted_[ultimate_target]_results_[timestamp].txt
        - Ejemplos:
          * build/build_tests_results/gcc/debug/uint128_traits_extracted_tests_results_20231224_153120.txt
          * build/build_benchs_results/msvc/release/int128_cmath_extracted_benchs_results_20231224_154200.txt
     
     C) EJECUTABLES COMPILADOS:
        - Ubicación: build/build_[ultimate_target]/[compiler]/[mode]/
        - Formato: [type_base]_[feature]_extracted_[ultimate_target].exe
        - Ejemplos:
          * build/build_tests/gcc/debug/uint128_traits_extracted_tests.exe
          * build/build_benchs/msvc/release/int128_cmath_extracted_benchs.exe

     Instrucciones para compilar y correr los tests y benchmarks extractados de [name header].hpp
     ============================================================================================
     1. Los archivos del proyecto son casi exclusivamente headers (.hpp) en include/.../[name_header].hpp.
     2. El name_header es el nombre del archivo header sin la extensión .hpp, estructurada como [type_base]_[feature]:
         - [name_header]=[type_base]_[feature].hpp
         - [type_base]=[uint128|int128]
         - [feature]=[t|traits|limits|concepts|algorithms|iostreams|bits|cmath|numeric|ranges|format|safe|thread_safety]
     3. Las finalidades son dos: tests y benchmarks (ultimate_target):
        1. "tests"  ->  testear las funcionalidades del proyecto
        2. "benchs" ->  medir tiempos y ciclos de reloj de las funcionalidades del proyecto
        3. [ultimate_target]=["tests"|"benchs"]
     4. Los tests verifican la corrección funcional de las características implementadas en [name_header].hpp.
     5. Los benchmarks miden tiempos y ciclos de reloj de las características implementadas en [name_header].hpp.
     6. Los nombre de los archivos [name_header].hpp tienen la forma [type_base]_[feature].hpp
     7. Los posibles [type_base] son:
         - uint128
         - int128
     8. Los posibles [feature] son:
         - t          [tipo de dato propiamente dicho, uint128_t o int128_t, en el ámbito global]
         - traits     [traits asociados al tipo de dato, extensión de los type_traits estándar <type_traits>]
         - limits     [límites del tipo de dato, al estilo std::numeric_limits, extensión de <limits>]
         - concepts   [conceptos C++20 asociados al tipo de dato, extensión de los conceptos estándar <concepts>]
         - algorithms [algoritmos y operaciones genéricas para el tipo de dato, extensiones del estándar <algorithms>]
         - iostreams  [operaciones de entrada/salida extensiones de std::istream/std::ostream]
         - bits       [operaciones a nivel de bits]
         - cmath      [operaciones matemáticas, extensiones de std::cmath]
         - numeric    [operaciones numéricas genéricas, extensiones de std::numeric]
         - ranges     [operaciones con rangos, extensiones de std::ranges]
         - format     [operaciones de formateo, extensiones de std::format]
         - safe       [operaciones y características de seguridad]
         - thread_safety [operaciones y características de seguridad en entornos multihilo]
     9. Los posibles compilers son:
         - intel      [Intel OneApi Compiler icpx/icx]
         - msvc       [Microsoft Visual Studio Compiler cl]
         - gcc        [GNU g++ Compiler]
         - clang      [LLVM clang++ Compiler]
         - all        [Todos los compiladores anteriores]
         - No hay compilador por defecto, se debe especificar uno de estos 5 anteriores.
     10. Los posibles modos de compilación son:
         - debug      [Compilación con información de depuración y sin optimizaciones]
         - release    [Compilación optimizada para rendimiento y sin información de depuración]
         - all        [Ambos modos de compilación anteriores]
         - No hay modo por defecto, se debe especificar uno estos tres anteriores.
     11. Para correr los build (los de tests y los de benchmarks) se usa un árgumento más para el script build:
         - root_project/scripts/build_[name_header]_extracted_[ultimate_target].bash [compiler] [mode] ["print"|""]
     12. Dado un archivo de cabecera [name_header].hpp en include/.../[name_header].hpp, crea los siguientes archivos c++ en el proyecto (si no existen ya):
         - tests/[name_header]_extracted_tests.cpp
         -   Contiene pruebas unitarias extractadas para las funciones y características definidas en [name_header].hpp.
         - benchs/[name_header]_extracted_benchs.cpp
         -   Contiene benchmarks extractados para medir el rendimiento de las funciones definidas en [name_header].hpp.   
     13. Asegúrate (construye los siguientes scripts) de tener los scripts de compilación y ejecución en la raíz del proyecto:
         - scripts/build_[name_header]_extracted_tests.bash
         - scripts/check_[name_header]_extracted_tests.bash
         - scripts/build_[name_header]_extracted_benchs.bash
         - scripts/run_[name_header]_extracted_benchs.bash
         - Esto se resume en el nombre de script:
         - root_project/scripts/[action]_[name_header]_extracted_[ultimate_target].bash
     15.  Usa los siguientes comandos para compilar y correr los tests y benchmarks:
     15.1. Compilar los tests o los benchs con:
         root_project/scripts/build_[name_header]_extracted_[ultimate_target].bash [compiler] [mode] [print]
     15.2. Resultados de la compilación (ejecutables):
         root_project/build/build_[ultimate_target]/[compiler]/[mode]/[name_header]_extracted_[ultimate_target].exe
     15.3. Corre los tests con:
         root_project/scripts/check_[name_header]_extracted_tests.bash [compiler] [mode] ["print"|""]
     15.4. Los resultados de los tests se muestran en terminal, y si se pasa el argumento "print" (opcional) se volcará la salida también en:
         root_project/build/build_tests_results/[compiler]/[mode]/[name_header]_extracted_tests_results_[timestamp].txt
     15.5. Compilamos los benchmarks con: 
         root_project/scripts/build_[name_header]_extracted_[ultimate_target].bash [compiler] [mode]
     15.6. Resultados de la compilación de benchmarks:
         root_project/build/build_benchs/[compiler]/[mode]/[name_header]_extracted_[ultimate_target].exe
     15.7. Corremos los benchmarks con: 
         root_project/scripts/run_[name_header]_extracted_[ultimate_target].bash [compiler] [mode]
         15.7.1. Este script genera resultados de benchmarks con tiempos y ciclos de reloj, para el compiler y el mode expresamente indicados (y no los demás).
         15.7.2. También muestra los resultados en terminal en tiempos y ciclos de reloj, y solo para el compiler y el mode expresamente indicados (y no los demás).
     15.8. Los resultados de los benchmarks se almacenan en:
         root_project/build/build_benchs_results/[compiler]/[mode]/[name_header]_extracted_benchs_results_[timestamp].txt

     Scripts Genéricos (Sistema Estandarizado)
     ==========================================
     A partir de diciembre 2025, se implementó un sistema de scripts genéricos para eliminar la duplicación 
     de código y garantizar la consistencia de rutas y nomenclatura.
     
     PROBLEMA RESUELTO:
     - Anteriormente: 128 scripts individuales (~19,200 líneas, 98% duplicadas)
       * 58 scripts build_[type_base]_[feature]_extracted_[ultimate_target].bash
       * 28 scripts check_[type_base]_[feature]_extracted_tests.bash  
       * 42 scripts run_[type_base]_[feature]_extracted_benchs.bash
     - Ahora: 4 scripts genéricos (~800 líneas, mantenimiento centralizado)
     
     SCRIPTS GENÉRICOS DISPONIBLES:
     1. build_generic.bash    - Compilación genérica (recomendado para tests/benchs)
     2. build_generic.py      - Compilación genérica en Python (alternativa)
     3. check_generic.bash    - Ejecución de tests con matriz de resultados
     4. check_generic.py      - Verificación de compilación Python
     5. run_generic.bash      - Ejecución de benchmarks y demos
     6. run_generic.py        - Ejecución genérica Python
     
     SINTAXIS DE SCRIPTS GENÉRICOS PARA TESTS/BENCHS:
     
     A) COMPILACIÓN (build):
        bash scripts/build_generic.bash <type_base> <feature> <ultimate_target> [compiler] [mode] [print]
        python scripts/build_generic.py <type_base> <feature> <ultimate_target> [compiler] [mode] [print]
        
        Parámetros:
        - type_base       : uint128 | int128
        - feature         : t | traits | limits | concepts | algorithms | iostreams | bits | cmath | numeric | ranges | format | safe | thread_safety | comparison_boost | interop
        - ultimate_target : tests | benchs
        - compiler        : gcc | clang | intel | msvc | all (default: all)
        - mode            : debug | release | all (default: all)
        - print           : yes | no (default: no) - Imprime comandos de compilación
        
        Ejemplos:
        bash scripts/build_generic.bash uint128 bits tests
        bash scripts/build_generic.bash int128 numeric benchs gcc release
        bash scripts/build_generic.bash uint128 algorithm tests all all yes
        python scripts/build_generic.py uint128 bits tests gcc release
     
     B) TESTING (check):
        bash scripts/check_generic.bash <type_base> <feature> [compiler] [mode]
        python scripts/check_generic.py <type_base> <feature> [compiler] [mode]
        
        Parámetros:
        - type_base : uint128 | int128
        - feature   : t | traits | limits | concepts | algorithms | iostreams | bits | cmath | numeric | ranges | format | safe | thread_safety | comparison_boost | interop
        - compiler  : gcc | clang | intel | msvc | all (default: all)
        - mode      : debug | release | all (default: all)
        
        Ejemplos:
        bash scripts/check_generic.bash uint128 bits
        bash scripts/check_generic.bash uint128 numeric gcc release
        bash scripts/check_generic.bash int128 cmath all all
     
     C) BENCHMARKING (run):
        bash scripts/run_generic.bash <type_base> <feature> [compiler] [mode]
        
        Parámetros:
        - type_base : uint128 | int128
        - feature   : t | traits | limits | concepts | algorithms | iostreams | bits | cmath | numeric | ranges | format | safe | thread_safety | comparison_boost | interop
        - compiler  : gcc | clang | intel | msvc | all (default: all)
        - mode      : debug | release | all (default: all)
        
        Ejemplos:
        bash scripts/run_generic.bash uint128 algorithm
        bash scripts/run_generic.bash int128 numeric clang release
     
     SISTEMA DE DEMOS (Diciembre 2025)
     ==================================
     
     ESTRUCTURA DE DEMOS:
     El sistema de demos proporciona ejemplos ejecutables organizados en 7 categorías:
     
     demos/
     ├── general/          - Conceptos generales y análisis (6 demos)
     ├── tutorials/        - Tutoriales paso a paso (16 demos: 01-13 + extras)
     ├── examples/         - Casos de uso reales (9 demos)
     ├── showcase/         - Demostraciones avanzadas (4 demos)
     ├── comparison/       - Comparaciones con otras bibliotecas
     ├── performance/      - Análisis de rendimiento
     └── integration/      - Ejemplos de integración
     
     SCRIPTS GENÉRICOS PARA DEMOS:
     
     D) COMPILACIÓN DE DEMOS:
        bash scripts/build_generic.bash demos <category> <demo_name> [compiler] [mode]
        python scripts/build_generic.py demos <category> <demo_name> [compiler] [mode] [print]
        
        Parámetros:
        - category   : general | tutorials | examples | showcase | comparison | performance | integration
        - demo_name  : Nombre del demo (sin extensión .cpp)
        - compiler   : gcc | clang | intel | msvc | all (default: all)
        - mode       : debug | release | all (default: all)
        - print      : yes | no (default: no) - Solo Python
        
        Ejemplos:
        bash scripts/build_generic.bash demos tutorials 01_basic_operations gcc release
        bash scripts/build_generic.bash demos examples ipv6_address clang debug
        python scripts/build_generic.py demos showcase main gcc release
        
        NOTA: Los demos que usan threading se compilan automáticamente con -pthread y -latomic.
     
     E) EJECUCIÓN DE DEMOS:
        bash scripts/run_generic.bash demos <category> <demo_name> [compiler] [mode] [args...]
        python scripts/run_generic.py demos <category> <demo_name> [compiler] [mode] [args...]
        
        Parámetros:
        - category   : general | tutorials | examples | showcase | comparison | performance | integration
        - demo_name  : Nombre del demo (sin extensión .cpp)
        - compiler   : gcc | clang | intel | msvc (default: gcc)
        - mode       : debug | release (default: release)
        - args...    : Argumentos adicionales para el demo
        
        Ejemplos:
        bash scripts/run_generic.bash demos tutorials 01_basic_operations
        bash scripts/run_generic.bash demos examples big_integer_calculator gcc release
        python scripts/run_generic.py demos showcase main clang release --help
        
     F) VERIFICACIÓN DE DEMOS (compilación masiva):
        python scripts/check_generic.py demos <category|all> [compiler] [mode]
        
        Parámetros:
        - category   : general | tutorials | examples | showcase | all
        - compiler   : gcc | clang | intel | msvc | all (default: all)
        - mode       : debug | release | all (default: all)
        
        Ejemplos:
        python scripts/check_generic.py demos tutorials gcc release
        python scripts/check_generic.py demos all gcc release
        
        Salida: Matriz de resultados mostrando cuántos demos compilan correctamente.
     
     SCRIPTS BASH ESPECÍFICOS DE DEMOS:
     - build_demo.bash          - Compila un demo individual
     - run_demo.bash            - Ejecuta un demo individual  
     - build_all_demos.bash     - Compila todos los demos de una categoría
     - test_demos.bash          - Verifica compilación de demos
     - catalog_demos.bash       - Genera catálogo de demos disponibles
     
     Ejemplos:
     bash scripts/build_demo.bash tutorials 01_basic_operations gcc release
     bash scripts/run_demo.bash examples ipv6_address gcc release
     bash scripts/build_all_demos.bash tutorials gcc release
     bash scripts/test_demos.bash tutorials gcc
     bash scripts/catalog_demos.bash > DEMOS_CATALOG.md
     
     INTEGRACIÓN CON MAKE.PY Y MAKEFILE:
     
     G) MAKE.PY (Sistema Python unificado):
        python make.py <command> [type] [feature] [compiler] [mode] [args...]
        
        Comandos disponibles:
        - build       : Compila tests, benchs o demos
        - run         : Ejecuta benchs o demos
        - check       : Verifica tests o demos
        - demo        : Compila y ejecuta un demo (atajo)
        - list        : Lista tests/benchs/demos disponibles
        - clean       : Limpia directorios build
        
        Ejemplos para demos:
        python make.py build demos tutorials 01_basic_operations gcc release
        python make.py run demos examples ipv6_address gcc release
        python make.py check demos all gcc release
        python make.py demo tutorials 01_basic_operations gcc release
        python make.py list demos
        
        Ejemplos para tests/benchs (sintaxis tradicional):
        python make.py build uint128 bits tests gcc release
        python make.py check uint128 bits gcc release
        python make.py run uint128 algorithm benchs gcc release
        
     H) MAKEFILE (Interfaz GNU Make):
        make <target> [VARIABLES]
        
        Targets para demos:
        - build_demo    : Compila un demo
        - run_demo      : Ejecuta un demo
        - check_demos   : Verifica compilación de demos
        - demo          : Compila y ejecuta (atajo)
        
        Variables:
        - CATEGORY=<category>   - Categoría del demo
        - DEMO=<demo_name>      - Nombre del demo
        - COMPILER=<compiler>   - Compilador (default: gcc)
        - MODE=<mode>          - Modo (default: release)
        
        Ejemplos:
        make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release
        make run_demo CATEGORY=examples DEMO=ipv6_address
        make check_demos CATEGORY=all COMPILER=gcc MODE=release
        make demo CATEGORY=tutorials DEMO=01_basic_operations
        
        Targets tradicionales (tests/benchs):
        make build_tests TYPE=uint128 FEATURE=bits
        make check_tests TYPE=uint128 FEATURE=bits
        make run_benchs TYPE=uint128 FEATURE=algorithm
     
     DETECCIÓN AUTOMÁTICA DE THREADING:
     Los scripts de compilación detectan automáticamente si un demo/test/bench usa:
     - <thread> o std::thread → Agrega -pthread
     - <atomic> o thread_safety.hpp → Agrega -latomic (GCC/Clang)
     
     Esto garantiza que los demos de threading compilen sin flags manuales.
     
     DIRECTORIOS DE BUILD PARA DEMOS:
     - build/build_demos/<compiler>/<mode>/<demo_name>
     
     Ejemplos:
     - build/build_demos/gcc/release/01_basic_operations
     - build/build_demos/clang/debug/ipv6_address
     
     CATÁLOGO DE DEMOS:
     El archivo DEMOS_CATALOG.md contiene una lista completa de todos los demos con:
     - Categoría, nombre y descripción
     - Líneas de código y características usadas
     - Comandos de compilación y ejecución
     
     Generación:
     bash scripts/catalog_demos.bash > DEMOS_CATALOG.md
        bash scripts/run_generic.bash uint128 bits all all
     
     USO CON MAKEFILE (RECOMENDADO):
     El Makefile del proyecto ha sido actualizado para usar automáticamente los scripts genéricos:
     
     # Compilación
     make build_tests TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release
     make build_benchs TYPE=int128 FEATURE=numeric COMPILER=all MODE=all
     
     # Testing
     make check TYPE=uint128 FEATURE=bits
     make check TYPE=uint128 FEATURE=numeric COMPILER=gcc MODE=release
     
     # Benchmarking
     make run TYPE=int128 FEATURE=algorithm
     make run TYPE=uint128 FEATURE=numeric COMPILER=clang MODE=release
     
     NOMENCLATURA CONSISTENTE DE EJECUTABLES:
     Los scripts genéricos SIEMPRE generan nombres consistentes según el estándar:
     
     Tests:    build/build_tests/[compiler]/[mode]/[type_base]_[feature]_tests_[compiler][.exe]
     Benchs:   build/build_benchs/[compiler]/[mode]/[type_base]_[feature]_benchs_[compiler][.exe]
     
     Ejemplos:
     - build/build_tests/gcc/release/uint128_bits_tests_gcc.exe
     - build/build_benchs/msvc/debug/int128_numeric_benchs_msvc.exe
     - build/build_tests/clang/debug/uint128_algorithm_tests_clang
     
     ARCHIVOS FUENTE ESPERADOS:
     Los scripts genéricos buscan archivos en las siguientes rutas estándar:
     
     Tests:    tests/[type_base]_[feature]_extracted_tests.cpp
     Benchs:   benchs/[type_base]_[feature]_extracted_benchs.cpp
     
     VENTAJAS DEL SISTEMA GENÉRICO:
     ✅ Mantenimiento centralizado (4 archivos vs 128)
     ✅ Imposible desajustar rutas (lógica en un solo lugar)
     ✅ Validación automática de parámetros y archivos
     ✅ Reducción del 96% de código duplicado (19,200 → 800 líneas)
     ✅ Consistencia garantizada en nomenclatura y estructura
     ✅ Python puede ejecutar compiladores sin problemas (subprocess)
     
     COMPATIBILIDAD:
     Los scripts individuales (build_*_extracted_*.bash, check_*_extracted_tests.bash, 
     run_*_extracted_benchs.bash) están deprecados pero se mantienen en scripts/individualized/ 
     por compatibilidad. Se recomienda migrar al sistema genérico para futuros desarrollos.
     
     DOCUMENTACIÓN COMPLETA:
     Ver documentation/BUILD_SCRIPTS_GENERIC.md para información detallada sobre:
     - Uso avanzado de scripts genéricos
     - Variables de entorno personalizables
     - Flags de compilación por modo
     - Solución de problemas
     - Comparación Bash vs Python

---

## Sistema de Gestión de Entornos de Compiladores

### Descripción

El proyecto incluye un sistema avanzado de gestión de entornos de compiladores que permite:

- **Detección automática** de compiladores (GCC, Clang, Intel oneAPI, MSVC)
- **Entornos aislados** sin modificar variables globales del sistema
- **Configuraciones persistentes** guardadas en archivos JSON
- **Scripts Python genéricos** que usan estos entornos automáticamente

### Estructura de Archivos

```
scripts/
├── env_setup/                           # Módulo de gestión de entornos
│   ├── compiler_env.py                  # Módulo principal Python
│   ├── setup_gcc_env.bash               # Setup para GCC
│   ├── setup_clang_env.bash             # Setup para Clang
│   ├── setup_intel_env.bash             # Setup para Intel oneAPI
│   ├── setup_msvc_env.bash              # Setup para MSVC
│   └── [otros scripts de detección]
├── individualized/                      # Scripts individuales (deprecados)
│   ├── build_*_extracted_{tests|benchs}.bash  # 58 scripts build
│   ├── check_*_extracted_tests.bash           # 28 scripts check
│   └── run_*_extracted_benchs.bash            # 30 scripts run
├── build_generic.{bash|py}              # Build genérico (bash y Python)
├── check_generic.{bash|py}              # Check genérico (bash y Python)
├── run_generic.{bash|py}                # Run genérico (bash y Python)
└── init_project.py                      # Inicialización del proyecto

build/
└── compiler_envs/                       # Configuraciones guardadas
    ├── gcc_env.json
    ├── clang_env.json
    ├── intel_env.json
    └── msvc_env.json
```

### Inicialización del Proyecto

**Primera vez que usas el proyecto:**

```bash
python scripts/init_project.py
```

Este comando:

1. Detecta todos los compiladores disponibles en tu sistema
2. Configura sus entornos (PATH, INCLUDE, LIB, etc.)
3. Guarda las configuraciones en `build/compiler_envs/*.json`
4. Muestra un resumen de compiladores detectados

**Salida esperada:**

```
╔════════════════════════════════════════════════════════════╗
║        INT128 PROJECT - COMPILER CONFIGURATION             ║
╚════════════════════════════════════════════════════════════╝

[GCC]
-> Detectando entorno para gcc...
[OK] Configuración guardada en build/compiler_envs/gcc_env.json

[CLANG]
-> Detectando entorno para clang...
[OK] Configuración guardada en build/compiler_envs/clang_env.json

[MSVC]
-> Detectando entorno para msvc...
[OK] Configuración guardada en build/compiler_envs/msvc_env.json

[INTEL]
-> Detectando entorno para intel...
[OK] Configuración guardada en build/compiler_envs/intel_env.json

╔════════════════════════════════════════════════════════════╗
║  Compiladores detectados: 4/4                              ║
╚════════════════════════════════════════════════════════════╝
```

### Uso de Scripts Python Genéricos

#### Compilación (build_generic.py)

```bash
# Compilar con todos los compiladores detectados
python scripts/build_generic.py uint128 bits tests

# Compilar solo con GCC en modo release
python scripts/build_generic.py uint128 bits tests gcc release

# Compilar con MSVC mostrando comandos
python scripts/build_generic.py int128 numeric benchs msvc release yes
```

**Ventaja clave:** Los scripts Python usan automáticamente las configuraciones guardadas,
inyectando el entorno correcto para cada compilador sin afectar el entorno global.

#### Ejecución de Tests (check_generic.py)

```bash
# Ejecutar tests con todos los compiladores
python scripts/check_generic.py uint128 bits

# Ejecutar solo tests compilados con GCC en release
python scripts/check_generic.py int128 algorithm gcc release
```

#### Ejecución de Benchmarks (run_generic.py)

```bash
# Ejecutar benchmarks con todos los compiladores
python scripts/run_generic.py uint128 cmath

# Ejecutar solo benchmarks de Intel en release
python scripts/run_generic.py int128 numeric intel release
```

### Gestión Manual de Entornos

#### Detectar un compilador específico

```bash
python scripts/env_setup/compiler_env.py --detect gcc
python scripts/env_setup/compiler_env.py --detect msvc
python scripts/env_setup/compiler_env.py --detect intel
```

#### Ver configuración guardada

```bash
python scripts/env_setup/compiler_env.py --show gcc
```

Muestra el JSON con todas las variables de entorno para ese compilador.

#### Listar compiladores configurados

```bash
python scripts/env_setup/compiler_env.py --list
```

### Entornos Aislados: Cómo Funciona

**Problema tradicional:**

```python
# Configurar MSVC contamina el entorno global
subprocess.run(["vcvarsall.bat", "x64"], shell=True)
# Ahora TODO usa MSVC, incluso si quieres compilar con GCC después
```

**Solución con entornos aislados:**

```python
from compiler_env import CompilerEnvironment

# Compilar con MSVC
msvc_env = CompilerEnvironment("msvc")
subprocess.run(["cl.exe", "test.cpp"], env=msvc_env.get_env())

# Compilar con GCC (entorno limpio, sin MSVC)
gcc_env = CompilerEnvironment("gcc")
subprocess.run(["g++", "test.cpp"], env=gcc_env.get_env())

# El entorno de Python NO se modificó en ningún momento
print(os.environ["PATH"])  # PATH original sin cambios
```

**Cada compilación usa un diccionario `env` personalizado que solo afecta
al proceso hijo (subprocess), nunca al proceso Python principal.**

### Configuración de Entornos (JSON)

Ejemplo de `gcc_env.json`:

```json
{
  "PATH": "/c/msys64/ucrt64/bin:/usr/bin:...",
  "CC": "gcc",
  "CXX": "g++",
  "GCC_PATH": "/c/msys64/ucrt64/bin/g++",
  "GCC_VERSION": "g++ (GCC) 15.2.0"
}
```

Ejemplo de `msvc_env.json`:

```json
{
  "PATH": "C:\\Program Files\\Microsoft Visual Studio\\...",
  "INCLUDE": "C:\\Program Files\\Microsoft Visual Studio\\...\\include;...",
  "LIB": "C:\\Program Files\\Microsoft Visual Studio\\...\\lib\\x64;...",
  "MSVC_VER": "14.50.35717"
}
```

### Flujo de Trabajo Recomendado

1. **Instalación inicial:**

   ```bash
   git clone <repo>
   cd int128
   python scripts/init_project.py
   ```

2. **Desarrollo:**

   ```bash
   # Compilar tests con GCC rápido
   python scripts/build_generic.py uint128 bits tests gcc release
   
   # Ejecutar tests
   python scripts/check_generic.py uint128 bits gcc release
   ```

3. **Validación completa:**

   ```bash
   # Compilar con todos los compiladores
   python scripts/build_generic.py uint128 bits tests all all
   
   # Ejecutar tests con todos
   python scripts/check_generic.py uint128 bits all all
   ```

4. **Benchmarking:**

   ```bash
   # Compilar benchmarks optimizados
   python scripts/build_generic.py uint128 algorithm benchs all release
   
   # Ejecutar y comparar rendimiento
   python scripts/run_generic.py uint128 algorithm all release
   ```

### Ventajas del Sistema

✅ **Aislamiento total**: Cada compilador usa su propio entorno sin conflictos  
✅ **Persistencia**: Configuraciones guardadas, no necesitas reconfigur cada vez  
✅ **Portabilidad**: Funciona en Windows (cmd/PowerShell), MSYS2, Linux  
✅ **Automatización**: Los scripts Python detectan y usan entornos automáticamente  
✅ **Mantenimiento**: Cambios en rutas de compiladores solo requieren re-detectar  
✅ **Paralelización**: Puedes compilar con múltiples compiladores simultáneamente  

### Solución de Problemas

**Compilador no detectado:**

```bash
# Re-detectar después de instalar un nuevo compilador
python scripts/env_setup/compiler_env.py --detect <compiler>
```

**Error "compilador no encontrado" al ejecutar scripts:**

```bash
# Verificar que la detección fue exitosa
python scripts/env_setup/compiler_env.py --list
python scripts/env_setup/compiler_env.py --show <compiler>

# Si la configuración existe pero falla, re-detectar
python scripts/env_setup/compiler_env.py --detect <compiler>
```

**Limpiar configuraciones:**

```bash
# Borrar configuraciones antiguas
rm -rf build/compiler_envs/*.json

# Re-detectar todo
python scripts/init_project.py
```

### Comparación: Bash vs Python

| Aspecto | Scripts Bash | Scripts Python |
|---------|--------------|----------------|
| Entornos aislados | ❌ Modifica entorno global | ✅ Aislamiento completo |
| Compiladores Windows | ⚠️ Requiere configuración manual | ✅ Detección automática |
| Persistencia config | ❌ Debes source cada vez | ✅ Guardado en JSON |
| Portabilidad | ⚠️ Depende de bash/msys2 | ✅ Funciona en cualquier Python |
| Paralelización | ❌ No soportado | ✅ Posible (futuro) |

**Recomendación:** Usa scripts Python para desarrollo serio. Los scripts Bash
se mantienen por compatibilidad con workflows existentes.

---

## make.py - Sistema de Build Centralizado

### Descripción

`make.py` es un **reemplazo completo del Makefile tradicional** implementado en Python,
que centraliza todas las operaciones de compilación, testing y benchmarking del proyecto.

### Ventajas sobre Make tradicional

✅ **Portabilidad total**: Funciona idéntico en Windows, Linux, macOS  
✅ **Entornos aislados**: Integración nativa con `compiler_env.py`  
✅ **Mejor manejo de errores**: Excepciones Python vs sintaxis Make  
✅ **Lógica compleja**: Condicionales, loops, y estructuras nativas  
✅ **Sin dependencias**: No requiere `make`, `nmake`, o versiones GNU  
✅ **Integración IDE**: VSCode puede ejecutar directamente  
✅ **Configuración JSON**: Más legible que variables Makefile  

### Comandos Principales

```bash
# Inicialización (detecta compiladores)
python make.py init

# Compilación
python make.py build <type> <feature> <target> [compiler] [mode]
python make.py build uint128 bits tests gcc release
python make.py build int128 numeric benchs all all

# Ejecución de tests
python make.py check <type> <feature> [compiler] [mode]
python make.py check uint128 bits gcc release
python make.py check int128 algorithm all all

# Ejecución de benchmarks
python make.py run <type> <feature> [compiler] [mode]
python make.py run uint128 cmath all release
python make.py run int128 numeric intel release

# Limpieza
python make.py clean              # Limpia build_tests/ y build_benchs/
python make.py clean --all        # Limpia también compiler_envs/
```

### Targets Especiales

```bash
# Compilar TODO el proyecto (tests + benchs, todos los compiladores)
python make.py all

# Compilar y ejecutar TODOS los tests
python make.py test               # Con todos los compiladores
python make.py test gcc release   # Solo GCC en release

# Compilar y ejecutar TODOS los benchmarks
python make.py bench              # Con todos los compiladores
python make.py bench intel        # Solo Intel

# Listar todas las combinaciones disponibles
python make.py list
```

### Ejemplos de Uso

**Desarrollo rápido:**

```bash
# Editas código de uint128_bits
python make.py build uint128 bits tests gcc release
python make.py check uint128 bits gcc release
```

**Validación completa:**

```bash
# Compilar todo el proyecto
python make.py all

# Ejecutar todos los tests
python make.py test

# Ejecutar todos los benchmarks
python make.py bench
```

**Workflow típico:**

```bash
# Inicialización (solo primera vez)
python make.py init

# Desarrollo iterativo
python make.py build uint128 algorithm tests gcc debug
python make.py check uint128 algorithm gcc debug

# Antes de commit
python make.py test gcc release

# Benchmarking
python make.py bench all
```

### Comparación: make.py vs Makefile

| Característica | Makefile | make.py |
|----------------|----------|---------|
| Portabilidad | ⚠️ Depende de make/nmake | ✅ Solo Python |
| Entornos aislados | ❌ Contamina global | ✅ Nativo |
| Sintaxis | ⚠️ Arcana (tabs, $@, etc) | ✅ Python estándar |
| Debugging | ❌ Difícil | ✅ Excepciones claras |
| Lógica compleja | ⚠️ Limitada | ✅ Python completo |
| Paralelización | ✅ make -j nativo | ⚠️ Requiere implementar |
| Curva aprendizaje | ⚠️ Todos conocen make | ✅ Python es ubicuo |
| Rendimiento | ✅ Muy rápido | ✅ Suficiente |

### Características Implementadas

✅ **Compilación completa**: `python make.py all`  
✅ **Testing masivo**: `python make.py test`  
✅ **Benchmarking masivo**: `python make.py bench`  
✅ **Auto-detección**: Lista automática de combinaciones type-feature  
✅ **Estadísticas**: Tiempo total, exitosas/fallidas  
✅ **Colores**: Output legible con códigos ANSI  
✅ **Limpieza**: Clean inteligente con opción --all  
✅ **Listado**: `python make.py list` muestra todo disponible  
✅ **Ayuda integrada**: `python make.py --help`  

### Integración con Entornos Aislados

`make.py` usa automáticamente el sistema de `compiler_env.py`, garantizando que:

1. Cada compilación usa su entorno correcto (PATH, INCLUDE, LIB)
2. No contamina el entorno global del sistema
3. Las configuraciones se cargan desde `build/compiler_envs/*.json`
4. Compiladores se detectan automáticamente con `python make.py init`

### Estructura Interna

```python
make.py
├── cmd_init()      # Detecta compiladores
├── cmd_build()     # Wrapper de build_generic.py
├── cmd_check()     # Wrapper de check_generic.py
├── cmd_run()       # Wrapper de run_generic.py
├── cmd_clean()     # Limpia directorios
├── cmd_all()       # Compila todo
├── cmd_test()      # Tests masivos
├── cmd_bench()     # Benchmarks masivos
└── cmd_list()      # Lista combinaciones
```

Cada comando delega en los scripts genéricos Python (`scripts/*.py`), que a su vez
usan entornos aislados de `compiler_env.py`.

### Migración desde Makefile

**Antes (Makefile):**

```bash
make TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release build_tests
make TYPE=uint128 FEATURE=bits COMPILER=gcc MODE=release check
make clean
```

**Ahora (make.py):**

```bash
python make.py build uint128 bits tests gcc release
python make.py check uint128 bits gcc release
python make.py clean
```

**O incluso más simple:**

```bash
python make.py test gcc release
```

### Salida de Ejemplo

```bash_output
$ python make.py test gcc release

============================================================
  EJECUCIÓN COMPLETA DE TESTS
============================================================

[1/14] uint128_algorithm
  Compilando...
  Ejecutando...
[OK]   gcc [release]: PASS

[2/14] uint128_bits
  Compilando...
  Ejecutando...
[OK]   gcc [release]: PASS

...

============================================================
  RESUMEN DE TESTS
============================================================
[OK] Pasaron: 14/14
[INFO] Tiempo total: 45.3s
```

### Extensibilidad

Para añadir nuevos comandos:

```python
def cmd_custom(args: argparse.Namespace) -> int:
    """Tu comando personalizado"""
    echo_info("Ejecutando comando custom...")
    # Tu lógica aquí
    return 0

# Registrar en main()
commands = {
    'custom': cmd_custom,
}
```

### Flujo de Trabajo Recomendado

1. **Setup inicial:**

   ```bash
   git clone <repo>
   cd int128
   python make.py init
   ```

2. **Desarrollo diario:**

   ```bash
   # Editar código
   python make.py build uint128 bits tests gcc debug
   python make.py check uint128 bits gcc debug
   ```

3. **Validación pre-commit:**

   ```bash
   python make.py test gcc release
   ```

4. **Validación completa:**

   ```bash
   python make.py all
   python make.py test
   ```

5. **Benchmarking:**

   ```bash
   python make.py bench all
   ```

---

**Con make.py, el proyecto tiene un sistema de build moderno, portable y
mantenible que reemplaza completamente el Makefile tradicional.**

     GENERACIÓN DE DOCUMENTACIÓN (Doxygen)
     =====================================
     
     El proyecto incluye configuración para generar documentación técnica automática usando Doxygen.
     
     1. Requisitos:
        - Doxygen (debe estar en el PATH)
        - Graphviz (opcional, para gráficos de dependencias)
     
     2. Configuración:
        - Archivo: Doxyfile (en la raíz del proyecto)
        - Directorio de entrada: include/ y documentation/doxygen/pages/
        - Directorio de salida: documentation/generated/html/
     
     3. Scripts de Generación:
        - Bash (MSYS2/Linux): scripts/generate_docs.bash
        - PowerShell (Windows): scripts/generate_docs.ps1
     
     4. Estructura de la Documentación:
        - API Reference: Generada desde los comentarios Doxygen en headers (.hpp)
        - Páginas Adicionales: documentation/doxygen/pages/ (quickstart.md, examples.md, architecture.md)
     
     5. Visualización:
        Abrir documentation/generated/html/index.html en un navegador web.

     PRUEBAS EN WSL (Windows Subsystem for Linux)
     ============================================
     
     El proyecto incluye un puente para ejecutar la batería de tests en un entorno Linux (WSL)
     directamente desde Windows, asegurando portabilidad total.
     
     1. Requisitos:
        - WSL instalado (Ubuntu recomendado)
        - Compiladores en WSL: g++, clang++, icpx (Intel oneAPI opcional)
        - Python 3 en WSL
     
     2. Ejecución:
        Desde una terminal de Windows (PowerShell/CMD) o MSYS2 en la raíz del proyecto:
        
        python scripts/run_wsl_tests.py
     
     3. Funcionamiento:
        - Monta la carpeta actual en WSL (/mnt/c/...)
        - Carga el entorno de Intel oneAPI (si existe)
        - Limpia builds previos para evitar conflictos de binarios
        - Ejecuta 'make.py init' y 'make.py test' dentro de Linux
