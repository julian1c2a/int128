# Agrega el ejecutable de la prueba
add_executable(test_uint128 test_uint128.cpp)
set_target_properties(test_uint128 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_tests)

# Enlaza la prueba con la biblioteca int128 (si la tienes como biblioteca)
# Si int128.cpp es solo un ejecutable, necesitarás compilar los fuentes
# directamente en la prueba o crear una biblioteca.
# Por ahora, asumiremos que los encabezados son suficientes.
target_link_libraries(test_uint128 PRIVATE uint128)

# Agrega la prueba a CTest
add_test(NAME UInt128Test COMMAND test_uint128)
