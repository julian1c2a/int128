#!/bin/bash
# Script para ejecutar benchmarks de uint128_traits con los 4 compiladores
# y recopilar resultados de tiempo y ciclos de reloj

set -e

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR"

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Directorio de resultados
RESULTS_DIR="benchmark_results/uint128_traits_extracted"
mkdir -p "$RESULTS_DIR"

# Timestamp para los archivos de resultados
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Ejecución de Benchmarks - uint128_traits.hpp${NC}"
echo -e "${BLUE}============================================================${NC}"
echo -e "${CYAN}Fecha: $(date)${NC}"
echo -e "${CYAN}Timestamp: $TIMESTAMP${NC}\n"

# =============================================================================
# EJECUTAR TESTS PRIMERO (Validación)
# =============================================================================
echo -e "${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 1: Ejecutando Tests (Validación)${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

TESTS_PASSED=0
TESTS_TOTAL=0

# Test con GCC
if [ -f "build/build_tests/gcc/release/uint128_traits_tests_gcc.exe" ]; then
    echo -e "${GREEN}[1/4] Tests con GCC...${NC}"
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if ./build/build_tests/gcc/release/uint128_traits_tests_gcc.exe > "$RESULTS_DIR/tests_gcc_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ FAIL (ver $RESULTS_DIR/tests_gcc_$TIMESTAMP.txt)${NC}"
    fi
else
    echo -e "${RED}[1/4] Tests con GCC no disponible (ejecutable no encontrado)${NC}"
fi

# Test con Clang
if [ -f "build/build_tests/clang/release/uint128_traits_tests_clang.exe" ]; then
    echo -e "${GREEN}[2/4] Tests con Clang...${NC}"
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if ./build/build_tests/clang/release/uint128_traits_tests_clang.exe > "$RESULTS_DIR/tests_clang_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ FAIL (ver $RESULTS_DIR/tests_clang_$TIMESTAMP.txt)${NC}"
    fi
else
    echo -e "${RED}[2/4] Tests con Clang no disponible${NC}"
fi

# Test con Intel
if [ -f "build/build_tests/intel/release/uint128_traits_tests_intel.exe" ]; then
    echo -e "${GREEN}[3/4] Tests con Intel...${NC}"
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if ./build/build_tests/intel/release/uint128_traits_tests_intel.exe > "$RESULTS_DIR/tests_intel_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ FAIL (ver $RESULTS_DIR/tests_intel_$TIMESTAMP.txt)${NC}"
    fi
else
    echo -e "${RED}[3/4] Tests con Intel no disponible${NC}"
fi

# Test con MSVC
if [ -f "build/build_tests/msvc/release/uint128_traits_tests_msvc.exe" ]; then
    echo -e "${GREEN}[4/4] Tests con MSVC...${NC}"
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if ./build/build_tests/msvc/release/uint128_traits_tests_msvc.exe > "$RESULTS_DIR/tests_msvc_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ FAIL (ver $RESULTS_DIR/tests_msvc_$TIMESTAMP.txt)${NC}"
    fi
else
    echo -e "${RED}[4/4] Tests con MSVC no disponible${NC}"
fi

echo -e "\n${CYAN}Tests completados: $TESTS_PASSED/$TESTS_TOTAL${NC}"

if [ $TESTS_PASSED -lt $TESTS_TOTAL ]; then
    echo -e "${RED}ADVERTENCIA: Algunos tests fallaron. Continuando con benchmarks...${NC}\n"
fi

# =============================================================================
# EJECUTAR BENCHMARKS
# =============================================================================
echo -e "\n${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 2: Ejecutando Benchmarks${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

BENCHS_RUN=0

# Benchmark con GCC
if [ -f "build/build_benchmarks/gcc/release/uint128_traits_benchs_gcc.exe" ]; then
    echo -e "${GREEN}[1/4] Benchmarks con GCC...${NC}"
    ./build/build_benchmarks/gcc/release/uint128_traits_benchs_gcc.exe > "$RESULTS_DIR/benchmarks_gcc_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_gcc_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[1/4] Benchmarks con GCC no disponible${NC}"
fi

# Benchmark con Clang
if [ -f "build/build_benchmarks/clang/release/uint128_traits_benchs_clang.exe" ]; then
    echo -e "${GREEN}[2/4] Benchmarks con Clang...${NC}"
    ./build/build_benchmarks/clang/release/uint128_traits_benchs_clang.exe > "$RESULTS_DIR/benchmarks_clang_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_clang_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[2/4] Benchmarks con Clang no disponible${NC}"
fi

# Benchmark con Intel
if [ -f "build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe" ]; then
    echo -e "${GREEN}[3/4] Benchmarks con Intel...${NC}"
    ./build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe > "$RESULTS_DIR/benchmarks_intel_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_intel_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[3/4] Benchmarks con Intel no disponible${NC}"
fi

# Benchmark con MSVC
if [ -f "build/build_benchmarks/msvc/release/uint128_traits_benchs_msvc.exe" ]; then
    echo -e "${GREEN}[4/4] Benchmarks con MSVC...${NC}"
    ./build/build_benchmarks/msvc/release/uint128_traits_benchs_msvc.exe > "$RESULTS_DIR/benchmarks_msvc_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_msvc_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[4/4] Benchmarks con MSVC no disponible${NC}"
fi

# =============================================================================
# EXTRAER Y CONSOLIDAR RESULTADOS CSV
# =============================================================================
echo -e "\n${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 3: Consolidando Resultados${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

SUMMARY_CSV="$RESULTS_DIR/summary_$TIMESTAMP.csv"

echo "Compiler,Benchmark,Time(ns),Cycles,Iterations" > "$SUMMARY_CSV"

# Función para extraer datos CSV de un archivo
extract_csv_data() {
    local file=$1
    local compiler=$2
    
    if [ -f "$file" ]; then
        # Buscar la sección CSV en el archivo
        awk '/^Benchmark,Time\(ns\),Cycles,Iterations$/,/^$/ {
            if ($0 !~ /^Benchmark,Time/ && $0 !~ /^$/) {
                print "'"$compiler"'," $0
            }
        }' "$file" >> "$SUMMARY_CSV"
    fi
}

echo -e "${CYAN}Extrayendo datos CSV...${NC}"

extract_csv_data "$RESULTS_DIR/benchmarks_gcc_$TIMESTAMP.txt" "GCC"
extract_csv_data "$RESULTS_DIR/benchmarks_clang_$TIMESTAMP.txt" "Clang"
extract_csv_data "$RESULTS_DIR/benchmarks_intel_$TIMESTAMP.txt" "Intel"
extract_csv_data "$RESULTS_DIR/benchmarks_msvc_$TIMESTAMP.txt" "MSVC"

echo -e "${GREEN}✅ CSV consolidado guardado en: $SUMMARY_CSV${NC}"

# =============================================================================
# GENERAR REPORTE DE COMPARACIÓN
# =============================================================================
echo -e "\n${YELLOW}Generando reporte de comparación...${NC}"

REPORT_FILE="$RESULTS_DIR/report_$TIMESTAMP.txt"

cat > "$REPORT_FILE" <<EOF
============================================================
  uint128_traits.hpp - Reporte de Benchmarks
============================================================
Fecha: $(date)
Timestamp: $TIMESTAMP

------------------------------------------------------------
RESUMEN DE TESTS
------------------------------------------------------------
Tests ejecutados: $TESTS_TOTAL
Tests pasados: $TESTS_PASSED
Tests fallados: $((TESTS_TOTAL - TESTS_PASSED))

------------------------------------------------------------
RESUMEN DE BENCHMARKS
------------------------------------------------------------
Benchmarks ejecutados: $BENCHS_RUN compiladores

Archivos generados:
EOF

ls -lh "$RESULTS_DIR"/*_$TIMESTAMP.* >> "$REPORT_FILE"

cat >> "$REPORT_FILE" <<EOF

------------------------------------------------------------
DATOS CSV CONSOLIDADOS
------------------------------------------------------------
EOF

cat "$SUMMARY_CSV" >> "$REPORT_FILE"

echo -e "${GREEN}✅ Reporte guardado en: $REPORT_FILE${NC}"

# =============================================================================
# RESUMEN FINAL
# =============================================================================
echo -e "\n${BLUE}============================================================${NC}"
echo -e "${BLUE}                    RESUMEN FINAL${NC}"
echo -e "${BLUE}============================================================${NC}"
echo -e "${CYAN}Tests completados:${NC} $TESTS_PASSED/$TESTS_TOTAL"
echo -e "${CYAN}Benchmarks ejecutados:${NC} $BENCHS_RUN compiladores"
echo -e "${CYAN}Directorio de resultados:${NC} $RESULTS_DIR"
echo -e "\n${GREEN}Archivos generados:${NC}"
echo -e "  - $SUMMARY_CSV"
echo -e "  - $REPORT_FILE"
echo -e "  - tests_*_$TIMESTAMP.txt (logs de tests)"
echo -e "  - benchmarks_*_$TIMESTAMP.txt (logs de benchmarks)"

# Mostrar una vista previa de los mejores tiempos
echo -e "\n${YELLOW}Vista previa - Mejores tiempos por benchmark:${NC}"
echo -e "${CYAN}------------------------------------------------------------${NC}"

if [ -f "$SUMMARY_CSV" ]; then
    # Procesar CSV para encontrar mejores tiempos
    tail -n +2 "$SUMMARY_CSV" | awk -F',' '
    {
        bench = $2
        compiler = $1
        time = $3
        if (!(bench in min_time) || time < min_time[bench]) {
            min_time[bench] = time
            min_compiler[bench] = compiler
        }
    }
    END {
        for (bench in min_time) {
            printf "  %-30s: %10.2f ns (%s)\n", bench, min_time[bench], min_compiler[bench]
        }
    }' | sort
fi

echo -e "${CYAN}------------------------------------------------------------${NC}"
echo -e "\n${GREEN}¡Benchmarks completados exitosamente!${NC}\n"
