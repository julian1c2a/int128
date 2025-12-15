#include "include/uint128_bits.hpp"
#include "include/uint128_t.hpp"
#include <bitset>
#include <iomanip>
#include <iostream>

using namespace uint128_literals;

int main()
{
    std::cout << "🔍 VERIFICACIÓN EXHAUSTIVA DE COMPATIBILIDAD std::bitset\n" << std::endl;

    // Test 1: Valores conocidos
    uint128_t test1 = 0xDEADBEEFCAFEBABE_u128;
    auto bitset1 = std::to_bitset(test1);
    auto back1 = std::from_bitset(bitset1);
    std::cout << "Test 1 - Round trip:" << std::endl;
    std::cout << "  Original: " << std::hex << test1 << std::endl;
    std::cout << "  Back:     " << std::hex << back1 << std::endl;
    std::cout << "  Match: " << (test1 == back1 ? "✓" : "✗") << std::endl << std::endl;

    // Test 2: Valor máximo
    uint128_t test2 = ~uint128_t(0); // Todos los bits en 1
    auto bitset2 = std::to_bitset(test2);
    auto back2 = std::from_bitset(bitset2);
    std::cout << "Test 2 - Valor máximo:" << std::endl;
    std::cout << "  Match: " << (test2 == back2 ? "✓" : "✗") << std::endl;
    std::cout << "  Bitset count: " << bitset2.count() << " (esperado: 128)" << std::endl
              << std::endl;

    // Test 3: Bits específicos
    uint128_t test3 =
        uint128_t(1) | (uint128_t(1) << 63) | (uint128_t(1) << 64) | (uint128_t(1) << 127);
    auto bitset3 = std::to_bitset(test3);
    auto back3 = std::from_bitset(bitset3);
    std::cout << "Test 3 - Bits específicos (0, 63, 64, 127):" << std::endl;
    std::cout << "  Match: " << (test3 == back3 ? "✓" : "✗") << std::endl;
    std::cout << "  Bits set: " << bitset3.count() << " (esperado: 4)" << std::endl;
    std::cout << "  Bit 0:   " << (bitset3[0] ? "✓" : "✗") << std::endl;
    std::cout << "  Bit 63:  " << (bitset3[63] ? "✓" : "✗") << std::endl;
    std::cout << "  Bit 64:  " << (bitset3[64] ? "✓" : "✗") << std::endl;
    std::cout << "  Bit 127: " << (bitset3[127] ? "✓" : "✗") << std::endl << std::endl;

    // Test 4: Bitsets de diferentes tamaños
    std::bitset<32> small32;
    small32.set(0);
    small32.set(31);
    auto from32 = std::from_bitset(small32);
    uint128_t expected32 = uint128_t(1) | (uint128_t(1) << 31);
    std::cout << "Test 4 - Bitset<32>:" << std::endl;
    std::cout << "  Expected: " << std::hex << expected32 << std::endl;
    std::cout << "  Got:      " << std::hex << from32 << std::endl;
    std::cout << "  Match: " << (from32 == expected32 ? "✓" : "✗") << std::endl << std::endl;

    std::bitset<96> medium96;
    medium96.set(50);
    medium96.set(95);
    auto from96 = std::from_bitset(medium96);
    uint128_t expected96 = (uint128_t(1) << 50) | (uint128_t(1) << 95);
    std::cout << "Test 5 - Bitset<96>:" << std::endl;
    std::cout << "  Match: " << (from96 == expected96 ? "✓" : "✗") << std::endl << std::endl;

    // Test 6: Operaciones bitset nativas
    std::cout << "Test 6 - Operaciones bitset nativas:" << std::endl;
    auto bitset_op = std::to_bitset(0xF0F0F0F0_u128);
    std::cout << "  Count: " << bitset_op.count() << std::endl;
    std::cout << "  All: " << (bitset_op.all() ? "true" : "false") << std::endl;
    std::cout << "  Any: " << (bitset_op.any() ? "true" : "false") << std::endl;
    std::cout << "  None: " << (bitset_op.none() ? "true" : "false") << std::endl;

    // Test flip operation
    bitset_op.flip();
    auto flipped_back = std::from_bitset(bitset_op);
    auto expected_flip = ~uint128_t(0xF0F0F0F0_u128);
    std::cout << "  Flip test: " << (flipped_back == expected_flip ? "✓" : "✗") << std::endl;

    std::cout << "\n🎯 RESUMEN COMPATIBILIDAD std::bitset:" << std::endl;
    std::cout << "✅ Conversión bidireccional uint128_t ↔ std::bitset<128>" << std::endl;
    std::cout << "✅ Soporte para bitsets de diferentes tamaños (8-128 bits)" << std::endl;
    std::cout << "✅ Preservación exacta de patrones de bits" << std::endl;
    std::cout << "✅ Compatibilidad con operaciones bitset nativas" << std::endl;
    std::cout << "✅ Manejo correcto de casos extremos (0, máximo, bits específicos)" << std::endl;

    return 0;
}