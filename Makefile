#!/usr/bin/make -f
# Makefile unificado para int128/uint128
# Uso: make [action] TYPE=[type] FEATURE=[feature] COMPILER=[compiler] MODE=[mode] PRINT=[print]
#      make demo CATEGORY=[category] DEMO=[demo] COMPILER=[compiler] MODE=[mode]

.PHONY: help build_tests build_benchs build_demo check check_demos run run_demo clean
.DEFAULT_GOAL := help

# =============================================================================
# VARIABLES
# =============================================================================

# Valores por defecto
TYPE ?=
FEATURE ?=
COMPILER ?= all
MODE ?= all
PRINT ?=

# Variables para demos
CATEGORY ?=
DEMO ?=

# Validación (completo según PROMPT.md)
VALID_TYPES := uint128 int128
VALID_FEATURES := t traits limits concepts algorithms iostreams bits cmath numeric ranges format safe thread_safety comparison_boost interop
VALID_CATEGORIES := general tutorials examples showcase comparison performance integration
VALID_COMPILERS := gcc clang intel msvc all
VALID_MODES := debug release all

# =============================================================================
# VALIDACIÓN
# =============================================================================

define validate_values
	@if [ -z "$(TYPE)" ]; then \
		echo "❌ ERROR: Falta TYPE"; \
		echo "   Valores válidos: $(VALID_TYPES)"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[feature] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if [ -z "$(FEATURE)" ]; then \
		echo "❌ ERROR: Falta FEATURE"; \
		echo "   Valores válidos: $(VALID_FEATURES)"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[feature] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_TYPES)" | grep -wq "$(TYPE)"; then \
		echo "❌ ERROR: TYPE inválido '$(TYPE)'"; \
		echo "   Valores válidos: $(VALID_TYPES)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_FEATURES)" | grep -wq "$(FEATURE)"; then \
		echo "❌ ERROR: FEATURE inválido '$(FEATURE)'"; \
		echo "   Valores válidos: $(VALID_FEATURES)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_COMPILERS)" | grep -wq "$(COMPILER)"; then \
		echo "❌ ERROR: COMPILER inválido '$(COMPILER)'"; \
		echo "   Valores válidos: $(VALID_COMPILERS)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_MODES)" | grep -wq "$(MODE)"; then \
		echo "❌ ERROR: MODE inválido '$(MODE)'"; \
		echo "   Valores válidos: $(VALID_MODES)"; \
		exit 1; \
	fi
endef

# =============================================================================
# TARGETS PRINCIPALES
# =============================================================================

build_tests:
	$(call validate_values,build_tests)
	@echo "🔨 Compilando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_generic.bash $(TYPE) $(FEATURE) tests $(COMPILER) $(MODE) $(PRINT)

build_benchs:
	$(call validate_values,build_benchs)
	@echo "🔨 Compilando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_generic.bash $(TYPE) $(FEATURE) benchs $(COMPILER) $(MODE) $(PRINT)

check:
	$(call validate_values,check)
	@echo "🧪 Ejecutando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/check_generic.bash $(TYPE) $(FEATURE) $(COMPILER) $(MODE)

run:
	$(call validate_values,run)
	@echo "⚡ Ejecutando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/run_generic.bash $(TYPE) $(FEATURE) $(COMPILER) $(MODE)

# =============================================================================
# TARGETS PARA DEMOS
# =============================================================================

build_demo:
	@if [ -z "$(CATEGORY)" ]; then \
		echo "❌ ERROR: Falta CATEGORY"; \
		echo "   Valores válidos: $(VALID_CATEGORIES)"; \
		echo "Uso: make build_demo CATEGORY=[category] DEMO=[demo_name] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if [ -z "$(DEMO)" ]; then \
		echo "❌ ERROR: Falta DEMO"; \
		echo "Uso: make build_demo CATEGORY=[category] DEMO=[demo_name] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_CATEGORIES)" | grep -wq "$(CATEGORY)"; then \
		echo "❌ ERROR: CATEGORY inválido '$(CATEGORY)'"; \
		echo "   Valores válidos: $(VALID_CATEGORIES)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_COMPILERS)" | grep -wq "$(COMPILER)"; then \
		echo "❌ ERROR: COMPILER inválido '$(COMPILER)'"; \
		echo "   Valores válidos: $(VALID_COMPILERS)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_MODES)" | grep -wq "$(MODE)"; then \
		echo "❌ ERROR: MODE inválido '$(MODE)'"; \
		echo "   Valores válidos: $(VALID_MODES)"; \
		exit 1; \
	fi
	@echo "🔨 Compilando demo: $(CATEGORY)/$(DEMO) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_generic.bash demos $(CATEGORY) $(DEMO) $(COMPILER) $(MODE) $(PRINT)

run_demo:
	@if [ -z "$(CATEGORY)" ]; then \
		echo "❌ ERROR: Falta CATEGORY"; \
		echo "Uso: make run_demo CATEGORY=[category] DEMO=[demo_name] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if [ -z "$(DEMO)" ]; then \
		echo "❌ ERROR: Falta DEMO"; \
		echo "Uso: make run_demo CATEGORY=[category] DEMO=[demo_name] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi
	@echo "⚡ Ejecutando demo: $(CATEGORY)/$(DEMO) [$(COMPILER)/$(MODE)]"
	@bash scripts/run_generic.bash demos $(CATEGORY) $(DEMO) $(COMPILER) $(MODE)

check_demos:
	@if [ -z "$(CATEGORY)" ]; then \
		echo "❌ ERROR: Falta CATEGORY (o usa 'all')"; \
		echo "Uso: make check_demos CATEGORY=[category|all] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi
	@echo "🧪 Verificando demos: $(CATEGORY) [$(COMPILER)/$(MODE)]"
	@python scripts/check_generic.py demos $(CATEGORY) $(COMPILER) $(MODE)

# Shortcut para build + run demo
demo: build_demo run_demo
	@echo "✅ Demo $(CATEGORY)/$(DEMO) completado"

# =============================================================================
# SISTEMA DE ATAJOS GENÉRICOS POR FEATURE
# =============================================================================

# Función genérica para crear atajos automáticos para cualquier feature
define feature_shortcuts
.PHONY: build-all-$(1) check-all-$(1) run-all-$(1) $(1)-full

build-all-$(1):
	@echo "========================================="
	@echo " Building ALL $(1) (tests + benchs)"
	@echo "========================================="
	@$$(MAKE) build_tests TYPE=uint128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)
	@$$(MAKE) build_tests TYPE=int128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)
	@$$(MAKE) build_benchs TYPE=uint128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)
	@$$(MAKE) build_benchs TYPE=int128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)

check-all-$(1):
	@echo "========================================="
	@echo " Checking ALL $(1) tests"
	@echo "========================================="
	@$$(MAKE) check TYPE=uint128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)
	@$$(MAKE) check TYPE=int128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)

run-all-$(1):
	@echo "========================================="
	@echo " Running ALL $(1) benchmarks"
	@echo "========================================="
	@$$(MAKE) run TYPE=uint128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)
	@$$(MAKE) run TYPE=int128 FEATURE=$(1) COMPILER=$$(COMPILER) MODE=$$(MODE)

$(1)-full: build-all-$(1) check-all-$(1) run-all-$(1)
	@echo "========================================="
	@echo " ✅ $(1): Pipeline completo"
	@echo "========================================="
endef

# Generar atajos automáticamente para TODAS las features
$(foreach feature,$(VALID_FEATURES),$(eval $(call feature_shortcuts,$(feature))))

# =============================================================================
# LIMPIEZA
# =============================================================================

# Función para crear targets de limpieza por feature (granulares)
define feature_clean
.PHONY: clean-$(1) clean-$(1)-gcc clean-$(1)-clang clean-$(1)-intel clean-$(1)-msvc
.PHONY: clean-$(1)-gcc-debug clean-$(1)-gcc-release
.PHONY: clean-$(1)-clang-debug clean-$(1)-clang-release
.PHONY: clean-$(1)-intel-debug clean-$(1)-intel-release
.PHONY: clean-$(1)-msvc-debug clean-$(1)-msvc-release

# Limpiar toda la feature (todos los compiladores y modos)
clean-$(1):
	@echo "🧹 Limpiando feature '$(1)' (tests + benchs)..."
	@rm -rf build/build_tests/*/debug/*$(1)*.exe build/build_tests/*/release/*$(1)*.exe
	@rm -rf build/build_benchs/*/debug/*$(1)*.exe build/build_benchs/*/release/*$(1)*.exe
	@rm -rf build/build_tests/*/debug/*$(1)*.obj build/build_tests/*/release/*$(1)*.obj
	@rm -rf build/build_benchs/*/debug/*$(1)*.obj build/build_benchs/*/release/*$(1)*.obj
	@echo "✅ Feature '$(1)' limpiada"

# Limpiar por compilador (todos los modos)
clean-$(1)-gcc:
	@echo "🧹 Limpiando $(1) [GCC]..."
	@rm -rf build/build_tests/gcc/debug/*$(1)*.exe build/build_tests/gcc/release/*$(1)*.exe
	@rm -rf build/build_benchs/gcc/debug/*$(1)*.exe build/build_benchs/gcc/release/*$(1)*.exe
	@rm -rf build/build_tests/gcc/debug/*$(1)*.obj build/build_tests/gcc/release/*$(1)*.obj
	@rm -rf build/build_benchs/gcc/debug/*$(1)*.obj build/build_benchs/gcc/release/*$(1)*.obj
	@echo "✅ $(1) [GCC] limpiado"

clean-$(1)-clang:
	@echo "🧹 Limpiando $(1) [Clang]..."
	@rm -rf build/build_tests/clang/debug/*$(1)*.exe build/build_tests/clang/release/*$(1)*.exe
	@rm -rf build/build_benchs/clang/debug/*$(1)*.exe build/build_benchs/clang/release/*$(1)*.exe
	@rm -rf build/build_tests/clang/debug/*$(1)*.obj build/build_tests/clang/release/*$(1)*.obj
	@rm -rf build/build_benchs/clang/debug/*$(1)*.obj build/build_benchs/clang/release/*$(1)*.obj
	@echo "✅ $(1) [Clang] limpiado"

clean-$(1)-intel:
	@echo "🧹 Limpiando $(1) [Intel]..."
	@rm -rf build/build_tests/intel/debug/*$(1)*.exe build/build_tests/intel/release/*$(1)*.exe
	@rm -rf build/build_benchs/intel/debug/*$(1)*.exe build/build_benchs/intel/release/*$(1)*.exe
	@rm -rf build/build_tests/intel/debug/*$(1)*.obj build/build_tests/intel/release/*$(1)*.obj
	@rm -rf build/build_benchs/intel/debug/*$(1)*.obj build/build_benchs/intel/release/*$(1)*.obj
	@echo "✅ $(1) [Intel] limpiado"

clean-$(1)-msvc:
	@echo "🧹 Limpiando $(1) [MSVC]..."
	@rm -rf build/build_tests/msvc/debug/*$(1)*.exe build/build_tests/msvc/release/*$(1)*.exe
	@rm -rf build/build_benchs/msvc/debug/*$(1)*.exe build/build_benchs/msvc/release/*$(1)*.exe
	@rm -rf build/build_tests/msvc/debug/*$(1)*.obj build/build_tests/msvc/release/*$(1)*.obj
	@rm -rf build/build_benchs/msvc/debug/*$(1)*.obj build/build_benchs/msvc/release/*$(1)*.obj
	@echo "✅ $(1) [MSVC] limpiado"

# Limpiar por compilador y modo específico
clean-$(1)-gcc-debug:
	@echo "🧹 Limpiando $(1) [GCC Debug]..."
	@rm -rf build/build_tests/gcc/debug/*$(1)*.exe build/build_benchs/gcc/debug/*$(1)*.exe
	@rm -rf build/build_tests/gcc/debug/*$(1)*.obj build/build_benchs/gcc/debug/*$(1)*.obj
	@echo "✅ $(1) [GCC Debug] limpiado"

clean-$(1)-gcc-release:
	@echo "🧹 Limpiando $(1) [GCC Release]..."
	@rm -rf build/build_tests/gcc/release/*$(1)*.exe build/build_benchs/gcc/release/*$(1)*.exe
	@rm -rf build/build_tests/gcc/release/*$(1)*.obj build/build_benchs/gcc/release/*$(1)*.obj
	@echo "✅ $(1) [GCC Release] limpiado"

clean-$(1)-clang-debug:
	@echo "🧹 Limpiando $(1) [Clang Debug]..."
	@rm -rf build/build_tests/clang/debug/*$(1)*.exe build/build_benchs/clang/debug/*$(1)*.exe
	@rm -rf build/build_tests/clang/debug/*$(1)*.obj build/build_benchs/clang/debug/*$(1)*.obj
	@echo "✅ $(1) [Clang Debug] limpiado"

clean-$(1)-clang-release:
	@echo "🧹 Limpiando $(1) [Clang Release]..."
	@rm -rf build/build_tests/clang/release/*$(1)*.exe build/build_benchs/clang/release/*$(1)*.exe
	@rm -rf build/build_tests/clang/release/*$(1)*.obj build/build_benchs/clang/release/*$(1)*.obj
	@echo "✅ $(1) [Clang Release] limpiado"

clean-$(1)-intel-debug:
	@echo "🧹 Limpiando $(1) [Intel Debug]..."
	@rm -rf build/build_tests/intel/debug/*$(1)*.exe build/build_benchs/intel/debug/*$(1)*.exe
	@rm -rf build/build_tests/intel/debug/*$(1)*.obj build/build_benchs/intel/debug/*$(1)*.obj
	@echo "✅ $(1) [Intel Debug] limpiado"

clean-$(1)-intel-release:
	@echo "🧹 Limpiando $(1) [Intel Release]..."
	@rm -rf build/build_tests/intel/release/*$(1)*.exe build/build_benchs/intel/release/*$(1)*.exe
	@rm -rf build/build_tests/intel/release/*$(1)*.obj build/build_benchs/intel/release/*$(1)*.obj
	@echo "✅ $(1) [Intel Release] limpiado"

clean-$(1)-msvc-debug:
	@echo "🧹 Limpiando $(1) [MSVC Debug]..."
	@rm -rf build/build_tests/msvc/debug/*$(1)*.exe build/build_benchs/msvc/debug/*$(1)*.exe
	@rm -rf build/build_tests/msvc/debug/*$(1)*.obj build/build_benchs/msvc/debug/*$(1)*.obj
	@echo "✅ $(1) [MSVC Debug] limpiado"

clean-$(1)-msvc-release:
	@echo "🧹 Limpiando $(1) [MSVC Release]..."
	@rm -rf build/build_tests/msvc/release/*$(1)*.exe build/build_benchs/msvc/release/*$(1)*.exe
	@rm -rf build/build_tests/msvc/release/*$(1)*.obj build/build_benchs/msvc/release/*$(1)*.obj
	@echo "✅ $(1) [MSVC Release] limpiado"
endef

# Generar targets de limpieza para todas las features
$(foreach feature,$(VALID_FEATURES),$(eval $(call feature_clean,$(feature))))

.PHONY: clean clean-build clean-results clean-logs

clean-build:
	@echo "🧹 Limpiando directorios de build..."
	@rm -rf build/build_tests build/build_benchs build/build_demos
	@echo "✅ Build limpiado"

clean-results:
	@echo "🧹 Limpiando resultados..."
	@rm -rf build/build_tests_results build/build_benchs_results
	@echo "✅ Resultados limpiados"

clean-logs:
	@echo "🧹 Limpiando logs de compilación..."
	@rm -rf build_log
	@echo "✅ Logs limpiados"

clean: clean-build clean-results clean-logs
	@echo "✅ Limpieza completa"

# =============================================================================
# TARGETS ÚTILES
# =============================================================================

.PHONY: list-features list-compilers list-modes list-all build-all-features

list-features:
	@echo "Features disponibles:"
	@echo "$(VALID_FEATURES)" | tr ' ' '\n' | sed 's/^/  - /'

list-compilers:
	@echo "Compiladores disponibles:"
	@echo "$(VALID_COMPILERS)" | tr ' ' '\n' | sed 's/^/  - /'

list-modes:
	@echo "Modos disponibles:"
	@echo "$(VALID_MODES)" | tr ' ' '\n' | sed 's/^/  - /'

list-all: list-features list-compilers list-modes

# Compilar todas las features (uint128 e int128)
build-all-features:
	@echo "========================================="
	@echo " Building TODAS las features"
	@echo "========================================="
	@for feature in $(VALID_FEATURES); do \
		echo ""; \
		echo "▶ Building $$feature..."; \
		$(MAKE) build-all-$$feature || exit 1; \
	done
	@echo ""
	@echo "========================================="
	@echo " ✅ TODAS las features compiladas"
	@echo "========================================="

# =============================================================================
# AYUDA
# =============================================================================

.PHONY: help

help:
	@echo "========================================="
	@echo " INT128/UINT128 - Build System"
	@echo "========================================="
	@echo ""
	@echo "USO:"
	@echo "  make [action] TYPE=[type] FEATURE=[feature] COMPILER=[compiler] MODE=[mode] PRINT=[print]"
	@echo ""
	@echo "ACCIONES:"
	@echo "  build_tests   - Compilar tests"
	@echo "  build_benchs  - Compilar benchmarks"
	@echo "  check         - Ejecutar tests"
	@echo "  run           - Ejecutar benchmarks"
	@echo ""
	@echo "DEMOS:"
	@echo "  build_demo    - Compilar un demo específico"
	@echo "  run_demo      - Ejecutar un demo específico"
	@echo "  check_demos   - Verificar compilación de demos"
	@echo "  demo          - Compilar y ejecutar un demo (build + run)"
	@echo ""
	@echo "  Uso demos:"
	@echo "    make build_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release"
	@echo "    make run_demo CATEGORY=tutorials DEMO=01_basic_operations COMPILER=gcc MODE=release"
	@echo "    make demo CATEGORY=tutorials DEMO=01_basic_operations"
	@echo "    make check_demos CATEGORY=tutorials"
	@echo "    make check_demos CATEGORY=all"
	@echo ""
	@echo "ARGUMENTOS:"
	@echo "  TYPE          uint128 | int128 (requerido)"
	@echo "  FEATURE       t | traits | limits | concepts | algorithms | iostreams"
	@echo "                bits | cmath | numeric | ranges | format | safe | thread_safety"
	@echo "                comparison_boost | interop (requerido)"
	@echo "  CATEGORY      general | tutorials | examples | showcase | comparison"
	@echo "                performance | integration (para demos)"
	@echo "  DEMO          nombre del demo sin .cpp (requerido para demos)"
	@echo "  COMPILER      gcc | clang | intel | msvc | all (default: all)"
	@echo "  MODE          debug | release | all (default: all)"
	@echo "  PRINT         print (opcional, genera logs/resultados en archivos)"
	@echo ""
	@echo "EJEMPLOS:"
	@echo "  make build_tests TYPE=uint128 FEATURE=concepts COMPILER=gcc MODE=debug"
	@echo "  make check TYPE=int128 FEATURE=concepts COMPILER=all MODE=all PRINT=print"
	@echo "  make run TYPE=uint128 FEATURE=cmath COMPILER=intel MODE=release PRINT=print"
	@echo ""
	@echo "ATAJOS POR FEATURE (disponibles para TODAS las features):"
	@echo "  make [feature]-full         - Pipeline completo (build+check+run)"
	@echo "  make build-all-[feature]    - Build tests+benchs de una feature"
	@echo "  make check-all-[feature]    - Ejecutar todos los tests de una feature"
	@echo "  make run-all-[feature]      - Ejecutar todos los benchs de una feature"
	@echo ""
	@echo "  Ejemplos:"
	@echo "    make concepts-full        make limits-full        make algorithms-full"
	@echo "    make cmath-full           make traits-full        make bits-full"
	@echo ""
	@echo "ATAJOS GLOBALES:"
	@echo "  make build-all-features     - Compilar TODAS las features"
	@echo "  make list-features          - Listar features disponibles"
	@echo "  make list-compilers         - Listar compiladores disponibles"
	@echo "  make list-modes             - Listar modos disponibles"
	@echo "  make list-all               - Listar todas las opciones"
	@echo ""
	@echo "LIMPIEZA:"
	@echo "  make clean-build            - Limpiar ejecutables compilados"
	@echo "  make clean-results          - Limpiar resultados de ejecución"
	@echo "  make clean-logs             - Limpiar logs de compilación"
	@echo "  make clean                  - Limpiar todo (build+results+logs)"
	@echo ""
	@echo "========================================="
