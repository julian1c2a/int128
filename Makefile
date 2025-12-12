# Makefile para compilar int128 con MSVC desde MSYS2/Bash

# Compilador
CXX = cl.exe

# Flags de compilación:
# /std:c++latest - Usar último estándar (C++20/23)
# /EHsc - Manejo de excepciones estándar
# /W4   - Nivel de advertencia alto
# /nologo - Ocultar banner de copyright de Microsoft
# /I include - Añadir carpeta include al path de búsqueda
CXXFLAGS = /std:c++latest /EHsc /W4 /nologo /I include

# Ruta a Catch2 (Ajusta esto si la ruta cambia en tu sistema)
# Usamos la ruta generada por tu script install_deps.bash anterior
CATCH2_DIR = libs_install/msvc/Catch2
CATCH2_FLAGS = /I "$(CATCH2_DIR)/include"

# Targets (Objetivos)
.PHONY: all clean test

all: int128.exe

# Regla para el ejecutable principal (Standalone)
int128.exe: int128.cpp include/uint128_t.hpp
	@echo "--- Compilando Main ---"
	$(CXX) $(CXXFLAGS) int128.cpp /Fe:int128.exe

# Regla para los tests (Requiere Catch2 instalado)
test: test_uint128.exe
	@echo "--- Ejecutando Tests ---"
	./test_uint128.exe

test_uint128.exe: tests/test_uint128.cpp include/uint128_t.hpp
	@echo "--- Compilando Tests con Catch2 ---"
	$(CXX) $(CXXFLAGS) $(CATCH2_FLAGS) tests/test_uint128.cpp /Fe:test_uint128.exe

# Limpieza
clean:
	rm -f *.exe *.obj *.asm