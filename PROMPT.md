Seguiremos con haciendo pruebas extractadas por cada función de uint128_traits.hpp, poniéndoles nombre test_(function name).cpp, y en un archivo tests/uint128_traits_extracted_tests.cpp y pruebas de benchmark en benchmarks/uint128_traits_extracted_benchs.cpp. Los tests y los benchs han de compilarse con los 4 compiladores que llevamos g++ en /c/msys64/ucrt64/bin/ (includes y libs también en /c/msys64/ucrt64), clang en /c/msys64/clang64/bin/ (includes y libs también en /c/msys64/clang64), y con Intel OneApi, en C:\Program Files (x86)\Intel\oneAPI, como con msvc en C:\Program Files\Microsoft Visual Studio\18\Community\ . Los benchmarks han de dar tanto tiempos como ciclos de reloj.

Tengo cuatro terminales msys/bash abiertas, uno para cada compilador (gnu g++, llvm clang++, intel icpx/icx, msvc cl). En cada terminal navego a la raíz del proyecto y ejecuto el script de build (o la acción correspondiente).

Las acciones que se hagan tienen una finalidad última entre dos: o testear o medir tiempos/ciclos de reloj (benchmarking). Por tanto, las acciones se dividen en dos grandes grupos: tests y benchmarks. A estas dos finalidades últimas corresponden dos valores de la variable [ultimate_target]:
     - [ultimate_target]=["tests"|"benchs"]
     - "tests"  ->  testear las funcionalidades del proyecto
     - "benchs" ->  medir tiempos y ciclos de reloj de las funcionalidades del proyecto

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

Compilo mediante dos scripts: scripts/build_[type base]_[feature]_extracted_[ultimate_target == "tests"].bash y scripts/build_[type_base]_[feature]_extracted_[ultimate_target == "benchs].bash, que se encuentran en la raíz del proyecto. El actual scripts se llama scripts/build_[type_base]_[feature]_extracted_[ultimate_target].bash. Hay que pasarle un argumento (no vale uno por defecto) que es el compilador a usar: intel, msvc, gcc, clang u all. Hay que pasarle obligatoriamente un segundo argumento que es el modo de compilación: debug, release u all, al que llamamos mode. Si se pasa all como segundo argumento, compila en ambos modos. Además hay un tercer argumento opcional "print" que si se pasa hace que se impriman en un archivo de texto los resultados de los tests. La estructura de los argumentos es:
    - [compiler]=[intel|msvc|gcc|clang|all].
    - [mode]=[debug|release|all].
    - ["print"|""]
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
    - [mode]=[debug|release|all].
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
     1. build_generic.bash    - Compilación genérica (recomendado)
     2. build_generic.py      - Compilación genérica en Python (alternativa)
     3. check_generic.bash    - Ejecución de tests con matriz de resultados
     4. run_generic.bash      - Ejecución de benchmarks
     
     SINTAXIS DE SCRIPTS GENÉRICOS:
     
     A) COMPILACIÓN (build):
        bash scripts/build_generic.bash <type_base> <feature> <ultimate_target> [compiler] [mode] [print]
        
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
     run_*_extracted_benchs.bash) están deprecados pero se mantienen por compatibilidad.
     Se recomienda migrar al sistema genérico para futuros desarrollos.
     
     DOCUMENTACIÓN COMPLETA:
     Ver documentation/BUILD_SCRIPTS_GENERIC.md para información detallada sobre:
     - Uso avanzado de scripts genéricos
     - Variables de entorno personalizables
     - Flags de compilación por modo
     - Solución de problemas
     - Comparación Bash vs Python
