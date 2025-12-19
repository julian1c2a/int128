#!/usr/bin/env bash
# Script para compilar int128_bits_benchmarks.cpp con diferentes compiladores

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Directorio de salida
BUILD_DIR="${PROJECT_ROOT}/build/build_benchmarks"
SOURCE_FILE="${PROJECT_ROOT}/benchmarks/int128_bits_benchmarks.cpp"

# Colores para output
COLOR_RESET="\033[0m"
COLOR_GREEN="\033[32m"
COLOR_RED="\033[31m"
COLOR_YELLOW="\033[33m"
COLOR_BLUE="\033[34m"

echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Build int128_bits_benchmarks${COLOR_RESET}"
echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"

# Función para compilar con GCC
build_gcc() {
    local mode="${1:-release}"
    local output_dir="${BUILD_DIR}/gcc/${mode}"
    
    echo -e "\n${COLOR_YELLOW}[INFO] Compilando con GCC (${mode})...${COLOR_RESET}"
    
    mkdir -p "${output_dir}"
    
    local flags="-std=c++20 -I${PROJECT_ROOT}/include -Wall -Wextra"
    if [[ "${mode}" == "release" ]]; then
        flags="${flags} -O3 -DNDEBUG"
    else
        flags="${flags} -O0 -g"
    fi
    
    if g++ ${flags} "${SOURCE_FILE}" -o "${output_dir}/int128_bits_benchmarks.exe"; then
        echo -e "${COLOR_GREEN}[OK] GCC compilacion exitosa${COLOR_RESET}"
        echo -e "${COLOR_GREEN}     Output: ${output_dir}/int128_bits_benchmarks.exe${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] GCC compilacion fallida${COLOR_RESET}"
        return 1
    fi
}

# Función para compilar con Clang
build_clang() {
    local mode="${1:-release}"
    local output_dir="${BUILD_DIR}/clang/${mode}"
    
    echo -e "\n${COLOR_YELLOW}[INFO] Compilando con Clang (${mode})...${COLOR_RESET}"
    
    mkdir -p "${output_dir}"
    
    local flags="-std=c++20 -I${PROJECT_ROOT}/include -Wall -Wextra"
    if [[ "${mode}" == "release" ]]; then
        flags="${flags} -O3 -DNDEBUG"
    else
        flags="${flags} -O0 -g"
    fi
    
    if clang++ ${flags} "${SOURCE_FILE}" -o "${output_dir}/int128_bits_benchmarks.exe"; then
        echo -e "${COLOR_GREEN}[OK] Clang compilacion exitosa${COLOR_RESET}"
        echo -e "${COLOR_GREEN}     Output: ${output_dir}/int128_bits_benchmarks.exe${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] Clang compilacion fallida${COLOR_RESET}"
        return 1
    fi
}

# Función para compilar con MSVC
build_msvc() {
    local mode="${1:-release}"
    local output_dir="${BUILD_DIR}/msvc/${mode}"
    
    echo -e "\n${COLOR_YELLOW}[INFO] Compilando con MSVC (${mode})...${COLOR_RESET}"
    
    mkdir -p "${output_dir}"
    
    # Convertir rutas para Windows
    local win_source_file=$(cygpath -m "${SOURCE_FILE}")
    local win_include_dir=$(cygpath -m "${PROJECT_ROOT}/include")
    local win_output_file=$(cygpath -m "${output_dir}/int128_bits_benchmarks.exe")
    
    local flags="/std:c++20 /I${win_include_dir} /EHsc /W4"
    if [[ "${mode}" == "release" ]]; then
        flags="${flags} /O2 /DNDEBUG"
    else
        flags="${flags} /Od /Zi"
    fi
    
    # Ejecutar MSVC
    export MSYS_NO_PATHCONV=1
    export MSYS2_ARG_CONV_EXCL='*'
    
    if cl.exe ${flags} "${win_source_file}" /Fe"${win_output_file}" 2>&1 | grep -v "Generando código" | grep -v "Código generado"; then
        echo -e "${COLOR_GREEN}[OK] MSVC compilacion exitosa${COLOR_RESET}"
        echo -e "${COLOR_GREEN}     Output: ${output_dir}/int128_bits_benchmarks.exe${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] MSVC compilacion fallida${COLOR_RESET}"
        return 1
    fi
}

# Función para compilar con Intel
build_intel() {
    local mode="${1:-release}"
    local output_dir="${BUILD_DIR}/intel/${mode}"
    
    echo -e "\n${COLOR_YELLOW}[INFO] Compilando con Intel (${mode})...${COLOR_RESET}"
    
    mkdir -p "${output_dir}"
    
    # Convertir rutas para Windows
    local win_source_file=$(cygpath -m "${SOURCE_FILE}")
    local win_include_dir=$(cygpath -m "${PROJECT_ROOT}/include")
    local win_output_file=$(cygpath -m "${output_dir}/int128_bits_benchmarks.exe")
    
    local flags="-std=c++20 -I${win_include_dir} -Wall"
    if [[ "${mode}" == "release" ]]; then
        flags="${flags} -O3 -DNDEBUG"
    else
        flags="${flags} -O0 -g"
    fi
    
    # Ejecutar Intel con rutas de Windows
    export MSYS_NO_PATHCONV=1
    export MSYS2_ARG_CONV_EXCL='*'
    
    if icpx ${flags} "${win_source_file}" -o "${win_output_file}"; then
        echo -e "${COLOR_GREEN}[OK] Intel compilacion exitosa${COLOR_RESET}"
        echo -e "${COLOR_GREEN}     Output: ${output_dir}/int128_bits_benchmarks.exe${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] Intel compilacion fallida${COLOR_RESET}"
        return 1
    fi
}

# Función principal
main() {
    local compiler="${1:-all}"
    local mode="${2:-release}"
    
    if [[ "${mode}" != "release" && "${mode}" != "debug" ]]; then
        echo -e "${COLOR_RED}[ERROR] Modo invalido: ${mode}${COLOR_RESET}"
        echo "Uso: $0 [compiler] [mode]"
        echo "  compiler: gcc, clang, msvc, intel, all (default: all)"
        echo "  mode: release, debug (default: release)"
        exit 1
    fi
    
    local failed=0
    local total=0
    
    case "${compiler}" in
        gcc)
            total=1
            build_gcc "${mode}" || ((failed++))
            ;;
        clang)
            total=1
            build_clang "${mode}" || ((failed++))
            ;;
        msvc)
            total=1
            build_msvc "${mode}" || ((failed++))
            ;;
        intel)
            total=1
            build_intel "${mode}" || ((failed++))
            ;;
        all)
            total=4
            build_gcc "${mode}" || ((failed++))
            build_clang "${mode}" || ((failed++))
            build_msvc "${mode}" || ((failed++))
            build_intel "${mode}" || ((failed++))
            ;;
        *)
            echo -e "${COLOR_RED}[ERROR] Compilador invalido: ${compiler}${COLOR_RESET}"
            echo "Uso: $0 [compiler] [mode]"
            echo "  compiler: gcc, clang, msvc, intel, all (default: all)"
            echo "  mode: release, debug (default: release)"
            exit 1
            ;;
    esac
    
    echo -e "\n${COLOR_BLUE}========================================${COLOR_RESET}"
    if [[ ${failed} -eq 0 ]]; then
        echo -e "${COLOR_GREEN}[OK] Compilacion completada exitosamente${COLOR_RESET}"
        echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
        exit 0
    else
        echo -e "${COLOR_RED}[FAIL] ${failed} compilacion(es) fallida(s)${COLOR_RESET}"
        echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
        exit 1
    fi
}

main "$@"
