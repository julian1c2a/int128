# Agrega el ejecutable de la prueba
add_executable(test_uint128 test_uint128.cpp)

# Enlaza la prueba con nuestra biblioteca y con Catch2
target_link_libraries(test_uint128 PRIVATE int128 Catch2::Catch2WithMain)

# Descubre automáticamente los tests de Catch2 y los agrega a CTest
include(Catch2/Catch.cmake)
catch_discover_tests(test_uint128)

set_target_properties(test_uint128 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build_tests")
