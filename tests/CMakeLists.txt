# Agrega el ejecutable de la prueba
add_executable(test_uint128 test_uint128.cpp)

# Enlaza la prueba con nuestra biblioteca y con Catch2
target_link_libraries(test_uint128 PRIVATE int128 Catch2::Catch2WithMain)

# Integra Catch2 con CTest para descubrir las pruebas automáticamente

find_package(Catch2 REQUIRED)
catch_discover_tests(test_uint128)

