cmake_minimum_required(VERSION 3.10)

project(int128_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones recomendadas para MSVC (Advertencias y manejo de excepciones)
if(MSVC)
    add_compile_options(/W4 /EHsc)
endif()

# Biblioteca header-only para uint128_t
add_library(uint128 INTERFACE)
target_include_directories(uint128 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Crea el ejecutable
add_executable(int128 int128.cpp)
target_link_libraries(int128 PRIVATE uint128)

# Habilita las pruebas y agrega el directorio de pruebas
enable_testing()
add_subdirectory(tests)