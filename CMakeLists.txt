cmake_minimum_required(VERSION 3.10)

project(int128_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones recomendadas para MSVC (Advertencias y manejo de excepciones)
if(MSVC)
    add_compile_options(/W4 /EHsc)
endif()

# Crea el ejecutable
add_executable(int128 int128.cpp)