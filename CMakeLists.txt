cmake_minimum_required(VERSION 3.15)

project(int128_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This block is executed when CMAKE_TOOLCHAIN_FILE is set, which is what conan does
if(CMAKE_TOOLCHAIN_FILE)
  find_package(Catch2 REQUIRED)
endif()

# Opciones recomendadas para MSVC (Advertencias y manejo de excepciones)
if(MSVC)
    add_compile_options(/W4 /EHsc)
endif()

# Biblioteca header-only para int128
add_library(int128 INTERFACE)
target_include_directories(int128 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Habilita las pruebas y agrega el directorio de pruebas
enable_testing()
add_subdirectory(tests)