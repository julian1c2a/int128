Seguiremos con haciendo pruebas extractadas por cada función de uint128_traits.hpp, poniéndoles nombre test_(function name).cpp, y en un archivo tests/uint128_traits_extracted_tests.cpp y pruebas de benchmark en benchmarks/uint128_traits_extracted_benchs.cpp. Los tests y los benchs han de compilarse con los 4 compiladores que llevamos g++ en /c/msys64/ucrt64/bin/ (includes y libs también en /c/msys64/ucrt64), clang en /c/msys64/clang64/bin/ (includes y libs también en /c/msys64/clang64), y con Intel OneApi, en C:\Program Files (x86)\Intel\oneAPI, como con msvc en C:\Program Files\Microsoft Visual Studio\18\Community\. Los benchmarks han de dar tanto tiempos como cilcos de reloj.

Compilo mediante dos scripts: scripts/build_uint128_traits_extracted_tests.bash y scripts/build_uint128_traits_extracted_benchs.bash, que se encuentran en la raíz del proyecto. Actualmente solo tengo un build que hace las dos cosas, pero prefiero separarlos para tener más control. El actual scripts se llama scripts/build_uint128_traits_extracted.bash. Hay que pasarle un argumento (no vale uno por defecto) que es el compilador a usar: intel, msvc, gcc, clang u all. Opcionalmente se le puede pasar un segundo argumento que es el modo de compilación: debug o release (por defecto release).

La estructura general es que los tests se construyen en build/build_tests/[compiler=intel|msvc|gcc|clang]/[mode=debug|release]/. y para los benchmarks en build/build_benchmarks/[compiler=intel|msvc|gcc|clang]/[mode=debug|release]/., los resultados de los benchmarks se colocarán en benchmarks_results/[compiler=intel|msvc|gcc|clang]/[mode=debug|release]/.

Tengo cuatro terminales msys/bash abiertas, uno para cada compilador (gnu g++, llvm clang++, intel icpx/icx, msvc cl). En cada terminal navego a la raíz del proyecto y ejecuto el script de build correspondiente. Necesito otros dos scripts para correr los tests y los benchmarks respectivamente: scripts/check_uint128_traits_extracted_tests.bash y scripts/run_uint128_traits_extracted_benchs.bash. Al igual que los scripts de build, estos scripts reciben como primer argumento el compilador a usar: intel, msvc, gcc, clang u all (no se admite argumento compiler por defecto). Y como segundo argumento opcional el modo de compilación: debug o release (por defecto release).

Las compilaciones con el compilador del intel son especialmente críticas ya que estamos un entorno muy cruzado (msys2 bash + Intel Compiler icpx/icx + STL Visual Studio libs). Hay que asegurarse de que todo enlace y funcione correctamente. Los ejemplos que funcionan son `scripts/build_uint128_limits_extracted_tests.bash intel`, `scripts/build_int128_limits_extracted_tests.bash intel`, `scripts/build_uint128_limits_extracted_benchs.bash intel` y  `scripts/build_int128_limits_extracted_benchs.bash intel`.

    Instrucciones para compilar y correr los tests y benchmarks extractados de [name header].hpp
    ============================================================================================
    1. Los archivos del proyecto son casi exclusivamente headers (.hpp) en include/.../[name header].hpp.
    2. Los nombre de los archivos [name_header].hpp tienen la forma [integral type base]_[feature].hpp
    3. Los posibles [integral type base] son:
       - uint128
       - int128
    4. Los posibles [feature] son:
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
    5. Los posibles compilers son:
       - intel      [Intel OneApi Compiler icpx/icx]
       - msvc       [Microsoft Visual Studio Compiler cl]
       - gcc        [GNU g++ Compiler]
       - clang      [LLVM clang++ Compiler]
       - all        [Todos los compiladores anteriores]
       - No hay compilador por defecto, se debe especificar uno de estos 5 anteriores.
    6. Los posibles modos de compilación son:
       - debug      [Compilación con información de depuración y sin optimizaciones]
       - release    [Compilación optimizada para rendimiento y sin información de depuración]
       - all        [Ambos modos de compilación anteriores]
       - No hay modo por defecto, se debe especificar uno estos tres anteriores.
    7. Para correr los build (los de tests y los de benchmarks) se usa un árgumento más para el script build:
       - scripts/build_[name header]_extracted_tests.bash [compiler] [mode] ["print"|""]
       - scripts/build_[name header]_extracted_benchs.bash [compiler] [mode] ["print"|""]
    8. Dado un archivo de cabecera [name header].hpp en include/.../[name header].hpp, crea los siguientes archivos c++ en el proyecto (si no existen ya):
       - tests/[name header]_extracted_tests.cpp
       -   Contiene pruebas unitarias extractadas para las funciones y características definidas en [name header].hpp.
       - benchmarks/[name header]_extracted_benchs.cpp
       -   Contiene benchmarks extractados para medir el rendimiento de las funciones definidas en [name header].hpp.   
    9. Asegúrate (construye los siguientes scripts) de tener los scripts de compilación y ejecución en la raíz del proyecto:
       - scripts/build_[name header]_extracted_tests.bash
       - scripts/check_[name header]_extracted_tests.bash
       - scripts/build_[name header]_extracted_benchs.bash
       - scripts/run_[name header]_extracted_benchs.bash
    10.  Abre una terminal msys/bash y navega a la raíz del proyecto.
    11.   Usa los siguientes comandos para compilar y correr los tests y benchmarks:
    11.1. Compilar los tests con:
         scripts/build_[name header]_extracted_tests.bash [compiler] [mode]
    11.2. Resultados de la compilación de tests:
         build/build_tests/[compiler]/[mode]/[name header]_extracted_tests.exe
    11.3. Corre los tests con:
         scripts/check_[name header]_extracted_tests.bash [compiler] [mode] ["print"|""]
    11.4. Los resultados de los tests se muestran en terminal, y si se pasa el argumento "print" (opcional) se volcará la salida también en:
         build/build_tests_results/[compiler]/[mode]/[name header]_extracted_tests_results.txt
    11.5. Compilamos los benchmarks con: 
         scripts/build_[name header]_extracted_benchs.bash [compiler] [mode]
    11.6. Resultados de la compilación de benchmarks:
         build/build_benchmarks/[compiler]/[mode]/[name header]_extracted_benchs.exe
    11.7. Corremos los benchmarks con: 
         scripts/run_[name header]_extracted_benchs.bash [compiler] [mode]
    11.8. Los resultados de los benchmarks se almacenan en:
         benchmarks_results/[compiler]/[mode]/[name header]_extracted_benchs_results.txt
