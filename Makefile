#!/usr/bin/make -f
# Makefile unificado para int128/uint128 - Todas las features
# Uso: make [type] [feature] [action] [compiler] [mode] [print]
#
# Ejemplos:
#   make uint128 concepts build_tests gcc debug
#   make int128 concepts check all all print
#   make uint128 concepts run gcc release
#   make int128 limits build_benchs intel debug

.PHONY: help all clean
.DEFAULT_GOAL := help

# =============================================================================
# VARIABLES
# =============================================================================

PROJECT_ROOT := $(shell pwd)

# Tipos válidos
VALID_TYPES := uint128 int128
# Features válidas
VALID_FEATURES := concepts limits algorithm cmath traits bits thread_safety
# Acciones válidas
VALID_ACTIONS := build_tests build_benchs check run
# Compiladores válidos
VALID_COMPILERS := gcc clang intel msvc all
# Modos válidos
VALID_MODES := debug release all

# =============================================================================
# VALIDACIÓN DE ARGUMENTOS
# =============================================================================

# Función para validar argumentos
define validate_arg
$(if $(filter $(2),$(3)),,$(error Error: $(1) inválido: '$(2)'. Válidos: $(3)))
endef

# =============================================================================
# TARGET PRINCIPAL
# =============================================================================

# Pattern rule para capturar los argumentos
$(VALID_TYPES): TYPE := $@
$(VALID_TYPES):
	@if [ -z "$(word 2,$(MAKECMDGOALS))" ]; then \
		echo "❌ ERROR: Faltan argumentos"; \
		echo "Uso: make [type] [feature] [action] [compiler] [mode] [print]"; \
		echo ""; \
		echo "Tipos:       $(VALID_TYPES)"; \
		echo "Features:    $(VALID_FEATURES)"; \
		echo "Acciones:    $(VALID_ACTIONS)"; \
		echo "Compiladores: $(VALID_COMPILERS)"; \
		echo "Modos:       $(VALID_MODES)"; \
		echo "Print:       print (opcional)"; \
		exit 1; \
	fi

# Capturar feature como segundo argumento
$(VALID_FEATURES): FEATURE := $@
$(VALID_FEATURES):
	@# No hacer nada, solo capturar

# Capturar action como tercer argumento
$(VALID_ACTIONS): ACTION := $@
$(VALID_ACTIONS):
	@# No hacer nada, solo capturar

# Capturar compiler como cuarto argumento
$(VALID_COMPILERS): COMPILER := $@
$(VALID_COMPILERS):
	@# No hacer nada, solo capturar

# Capturar mode como quinto argumento
$(VALID_MODES): MODE := $@
$(VALID_MODES):
	@# No hacer nada, solo capturar

# Capturar print como sexto argumento opcional
print: PRINT := print
print:
	@# No hacer nada, solo capturar

# =============================================================================
# REGLAS DE EJECUCIÓN
# =============================================================================

# Regla que ejecuta cuando se tienen todos los parámetros
exec_%:
	@TYPE="$(word 1,$(MAKECMDGOALS))"; \
	FEATURE="$(word 2,$(MAKECMDGOALS))"; \
	ACTION="$(word 3,$(MAKECMDGOALS))"; \
	COMPILER="$(word 4,$(MAKECMDGOALS))"; \
	MODE="$(word 5,$(MAKECMDGOALS))"; \
	PRINT="$(word 6,$(MAKECMDGOALS))"; \
	\
	if [ "$$ACTION" = "build_tests" ]; then \
		echo "🔨 Compilando tests: $$TYPE $$FEATURE con $$COMPILER en modo $$MODE"; \
		bash scripts/build_$${TYPE}_$${FEATURE}_extracted_tests.bash $$COMPILER $$MODE $$PRINT; \
	elif [ "$$ACTION" = "build_benchs" ]; then \
		echo "🔨 Compilando benchmarks: $$TYPE $$FEATURE con $$COMPILER en modo $$MODE"; \
		bash scripts/build_$${TYPE}_$${FEATURE}_extracted_benchs.bash $$COMPILER $$MODE $$PRINT; \
	elif [ "$$ACTION" = "check" ]; then \
		echo "🧪 Ejecutando tests: $$TYPE $$FEATURE con $$COMPILER en modo $$MODE"; \
		bash scripts/check_$${TYPE}_$${FEATURE}_extracted_tests.bash $$COMPILER $$MODE $$PRINT; \
	elif [ "$$ACTION" = "run" ]; then \
		echo "⚡ Ejecutando benchmarks: $$TYPE $$FEATURE con $$COMPILER en modo $$MODE"; \
		bash scripts/run_$${TYPE}_$${FEATURE}_extracted_benchs.bash $$COMPILER $$MODE; \
	else \
		echo "❌ ERROR: Acción inválida: $$ACTION"; \
		echo "Acciones válidas: $(VALID_ACTIONS)"; \
		exit 1; \
	fi

# =============================================================================
# ATAJOS ÚTILES
# =============================================================================

# Construir todos los tests de concepts para ambos tipos
.PHONY: build_all_concepts_tests
build_all_concepts_tests:
	@echo "========================================="
	@echo " Building ALL concepts tests"
	@echo "========================================="
	@bash scripts/build_uint128_concepts_extracted_tests.bash all all
	@bash scripts/build_int128_concepts_extracted_tests.bash all all

# Ejecutar todos los tests de concepts
.PHONY: check_all_concepts
check_all_concepts:
	@echo "========================================="
	@echo " Checking ALL concepts tests"
	@echo "========================================="
	@bash scripts/check_uint128_concepts_extracted_tests.bash all all
	@bash scripts/check_int128_concepts_extracted_tests.bash all all

# Construir todos los benchmarks de concepts
.PHONY: build_all_concepts_benchs
build_all_concepts_benchs:
	@echo "========================================="
	@echo " Building ALL concepts benchmarks"
	@echo "========================================="
	@bash scripts/build_uint128_concepts_extracted_benchs.bash all all
	@bash scripts/build_int128_concepts_extracted_benchs.bash all all

# Ejecutar todos los benchmarks de concepts
.PHONY: run_all_concepts_benchs
run_all_concepts_benchs:
	@echo "========================================="
	@echo " Running ALL concepts benchmarks"
	@echo "========================================="
	@bash scripts/run_uint128_concepts_extracted_benchs.bash all all
	@bash scripts/run_int128_concepts_extracted_benchs.bash all all

# Pipeline completo de concepts: build + test + bench
.PHONY: concepts_full
concepts_full: build_all_concepts_tests check_all_concepts build_all_concepts_benchs run_all_concepts_benchs
	@echo "========================================="
	@echo " ✅ CONCEPTS: Full pipeline completed"
	@echo "========================================="

# =============================================================================
# LIMPIEZA
# =============================================================================

.PHONY: clean_build
clean_build:
	@echo "🧹 Limpiando directorios de build..."
	@rm -rf build/build_tests
	@rm -rf build/build_benchmarks
	@echo "✅ Directorios de build limpiados"

.PHONY: clean_results
clean_results:
	@echo "🧹 Limpiando resultados..."
	@rm -rf build/build_tests_results
	@rm -rf benchmarks_results
	@echo "✅ Resultados limpiados"

.PHONY: clean
clean: clean_build clean_results
	@echo "✅ Limpieza completa"

# =============================================================================
# AYUDA
# =============================================================================

.PHONY: help
help:
	@echo "========================================="
	@echo " INT128/UINT128 - Build System"
	@echo "========================================="
	@echo ""
	@echo "USO PRINCIPAL:"
	@echo "  make [type] [feature] [action] [compiler] [mode] [print]"
	@echo ""
	@echo "ARGUMENTOS:"
	@echo "  type:       $(VALID_TYPES)"
	@echo "  feature:    $(VALID_FEATURES)"
	@echo "  action:     $(VALID_ACTIONS)"
	@echo "  compiler:   $(VALID_COMPILERS)"
	@echo "  mode:       $(VALID_MODES)"
	@echo "  print:      print (opcional, solo para tests)"
	@echo ""
	@echo "EJEMPLOS:"
	@echo "  make uint128 concepts build_tests gcc debug"
	@echo "  make int128 concepts check all all print"
	@echo "  make uint128 concepts run gcc release"
	@echo "  make int128 concepts build_benchs intel all"
	@echo ""
	@echo "ATAJOS ÚTILES:"
	@echo "  make build_all_concepts_tests  - Compilar todos los tests de concepts"
	@echo "  make check_all_concepts        - Ejecutar todos los tests de concepts"
	@echo "  make build_all_concepts_benchs - Compilar todos los benchs de concepts"
	@echo "  make run_all_concepts_benchs   - Ejecutar todos los benchs de concepts"
	@echo "  make concepts_full             - Pipeline completo (build+test+bench)"
	@echo ""
	@echo "LIMPIEZA:"
	@echo "  make clean_build               - Limpiar directorios de compilación"
	@echo "  make clean_results             - Limpiar resultados de tests/benchs"
	@echo "  make clean                     - Limpieza completa"
	@echo ""
	@echo "========================================="

# Evitar que make intente interpretar los argumentos como targets
%:
	@:
