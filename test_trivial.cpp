#include "include/int128_t.hpp"
#include "include/uint128_t.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "=== Propiedades de Tipos de 128 bits ===\n\n";

    std::cout << "uint128_t:\n";
    std::cout << "  is_trivial: " << std::is_trivial_v<uint128_t> << "\n";
    std::cout << "  is_trivially_copyable: " << std::is_trivially_copyable_v<uint128_t> << "\n";
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<uint128_t> << "\n";
    std::cout << "  is_pod: " << std::is_pod_v<uint128_t> << "\n\n";

    std::cout << "int128_t:\n";
    std::cout << "  is_trivial: " << std::is_trivial_v<int128_t> << "\n";
    std::cout << "  is_trivially_copyable: " << std::is_trivially_copyable_v<int128_t> << "\n";
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<int128_t> << "\n";
    std::cout << "  is_pod: " << std::is_pod_v<int128_t> << "\n\n";

    std::cout << "Tamaños:\n";
    std::cout << "  sizeof(uint128_t): " << sizeof(uint128_t) << " bytes\n";
    std::cout << "  sizeof(int128_t): " << sizeof(int128_t) << " bytes\n";
    std::cout << "  alignof(uint128_t): " << alignof(uint128_t) << " bytes\n";
    std::cout << "  alignof(int128_t): " << alignof(int128_t) << " bytes\n";

    return 0;
}
