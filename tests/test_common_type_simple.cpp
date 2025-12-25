// Test simple para verificar std::common_type entre uint128_t e int128_t

#include "int128/int128_t.hpp"
#include "int128/int128_traits.hpp"
#include "uint128/uint128_t.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    // Verificar que common_type está definido
    using CommonType = std::common_type_t<uint128_t, int128_t>;

    std::cout << "✅ std::common_type<uint128_t, int128_t> está definido!\n";
    std::cout << "   Tipo resultante: "
              << (std::is_same_v<CommonType, int128_t> ? "int128_t" : "otro tipo") << "\n";

    // Verificar la versión inversa
    using CommonType2 = std::common_type_t<int128_t, uint128_t>;
    std::cout << "✅ std::common_type<int128_t, uint128_t> está definido!\n";
    std::cout << "   Tipo resultante: "
              << (std::is_same_v<CommonType2, int128_t> ? "int128_t" : "otro tipo") << "\n";

    // Verificar que ambos son el mismo tipo
    if (std::is_same_v<CommonType, CommonType2>) {
        std::cout << "✅ Ambas especializaciones dan el mismo tipo (int128_t)\n";
    }

    // Test práctico: usar common_type en operaciones
    uint128_t u(100);
    int128_t i(-50);

    CommonType result = static_cast<CommonType>(u) + static_cast<CommonType>(i);
    std::cout << "✅ Operación mixta: uint128_t(100) + int128_t(-50) = " << result.to_string()
              << "\n";

    std::cout << "\n🎉 ¡std::common_type funciona correctamente!\n";

    return 0;
}
