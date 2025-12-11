cmake_minimum_required(VERSION 3.20)
project(Uint128)

set(CMAKE_CXX_STANDARD 20)

# vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# Buscar Catch2
find_package(Catch2 3 CONFIG REQUIRED)

# Biblioteca uint128_t (header-only)
add_library(uint128_t INTERFACE)
target_include_directories(uint128_t INTERFACE include)

# Tests
add_executable(uint128_tests test_uint128_t.cpp)
target_link_libraries(uint128_tests PRIVATE 
    uint128_t 
    Catch2::Catch2WithMain
)

# Habilitar testing
enable_testing()
add_test(NAME uint128_tests COMMAND uint128_tests)

# Additional Include and Library Paths
if(DEFINED ENV{VCPKG_ROOT})
    include_directories("$ENV{VCPKG_ROOT}/installed/x64-windows/include")
    link_directories("$ENV{VCPKG_ROOT}/installed/x64-windows/lib")
endif()

set_property(DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES "${VCPKG_ROOT}/installed/x64-windows/include")
set_property(DIRECTORY APPEND PROPERTY LINK_DIRECTORIES "${VCPKG_ROOT}/installed/x64-windows/lib")

# Language Standard
set_property(TARGET uint128_t PROPERTY CXX_STANDARD 20)
set_property(TARGET uint128_t PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET uint128_t PROPERTY CXX_EXTENSIONS OFF)

set_target_properties(uint128_t PROPERTIES
    COMPILE_OPTIONS "-std:c++latest"
)
set_target_properties(uint128_tests PROPERTIES
    COMPILE_OPTIONS "-std:c++latest"
)

# Abrir Visual Studio Developer Command Prompt
# Navegar al directorio
# cd "ruta\al\proyecto"

# Compilar
# msbuild Uint128Tests.vcxproj /p:Configuration=Release

# Ejecutar
# .\x64\Release\Uint128Tests.exe

# Verificar que Catch2 está instalado
vcpkg list | findstr catch2

# Si no está instalado
vcpkg install catch2:x64-windows
vcpkg integrate install

winget install Kitware.CMake