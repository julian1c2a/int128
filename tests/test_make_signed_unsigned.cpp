#include "int128/int128_t.hpp"
#include "uint128/uint128_t.hpp"
#include <iostream>
#include <type_traits>

int main()
{
    std::cout << "Testing std::make_signed and std::make_unsigned...\n\n";

    // ========================================================================
    // TEST 1: make_signed<uint128_t> debe dar int128_t
    // ========================================================================
    std::cout << "TEST 1: std::make_signed<uint128_t>\n";
    using SignedFromUnsigned = std::make_signed_t<uint128_t>;

    if constexpr (std::is_same_v<SignedFromUnsigned, int128_t>) {
        std::cout << "   [OK] std::make_signed_t<uint128_t> = int128_t\n";
    } else {
        std::cout << "   [FAIL] std::make_signed_t<uint128_t> != int128_t\n";
        return 1;
    }

    // ========================================================================
    // TEST 2: make_unsigned<int128_t> debe dar uint128_t
    // ========================================================================
    std::cout << "\nTEST 2: std::make_unsigned<int128_t>\n";
    using UnsignedFromSigned = std::make_unsigned_t<int128_t>;

    if constexpr (std::is_same_v<UnsignedFromSigned, uint128_t>) {
        std::cout << "   [OK] std::make_unsigned_t<int128_t> = uint128_t\n";
    } else {
        std::cout << "   [FAIL] std::make_unsigned_t<int128_t> != uint128_t\n";
        return 1;
    }

    // ========================================================================
    // TEST 3: Conversión práctica int128_t -> uint128_t
    // ========================================================================
    std::cout << "\nTEST 3: Conversión práctica int128_t -> uint128_t\n";
    int128_t i1(42);
    UnsignedFromSigned u1 = static_cast<UnsignedFromSigned>(i1);

    if (u1 == uint128_t(42)) {
        std::cout << "   [OK] int128_t(42) -> uint128_t(42)\n";
    } else {
        std::cout << "   [FAIL] Conversión incorrecta\n";
        return 1;
    }

    // ========================================================================
    // TEST 4: Conversión práctica uint128_t -> int128_t
    // ========================================================================
    std::cout << "\nTEST 4: Conversión práctica uint128_t -> int128_t\n";
    uint128_t u2(84);
    SignedFromUnsigned i2 = static_cast<SignedFromUnsigned>(u2);

    if (i2 == int128_t(84)) {
        std::cout << "   [OK] uint128_t(84) -> int128_t(84)\n";
    } else {
        std::cout << "   [FAIL] Conversión incorrecta\n";
        return 1;
    }

    // ========================================================================
    // TEST 5: Valor negativo se convierte a unsigned correctamente
    // ========================================================================
    std::cout << "\nTEST 5: Valor negativo -> unsigned (two's complement)\n";
    int128_t i_neg(-1);
    auto u_neg = static_cast<std::make_unsigned_t<int128_t>>(i_neg);

    // -1 en two's complement es todos los bits en 1 (máximo uint128_t)
    uint128_t max_val = ~uint128_t(0); // Todos los bits en 1
    if (u_neg == max_val) {
        std::cout << "   [OK] int128_t(-1) -> uint128_t::max() (two's complement)\n";
    } else {
        std::cout << "   [FAIL] Conversión de negativo incorrecta\n";
        return 1;
    }

    // ========================================================================
    // RESUMEN
    // ========================================================================
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "🎉 ¡Todos los tests de make_signed/make_unsigned pasaron!\n";
    std::cout << std::string(70, '=') << "\n";

    return 0;
}
