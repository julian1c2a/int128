#include <int128_simple.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace nstd;

using namespace uint128_literals; // Importar literales UDL

void demo_crypto_like_operations()
{
    std::cout << "=== DEMO: Operaciones Tipo Criptografía ===" << std::endl;

    // Simular claves y valores hash usando literales UDL
    auto private_key = "DEADBEEFCAFEBABE123456789ABCDEF0"_u128_hex;
    auto public_key = "FEDCBA9876543210ABCDEF0123456789"_u128_hex;
    auto message_hash = "0x1A2B3C4D5E6F7890"_u128;

    std::cout << "Clave privada: " << private_key.to_string_hex(true) << std::endl;
    std::cout << "Clave pública: " << public_key.to_string_hex(true) << std::endl;
    std::cout << "Hash mensaje:  " << message_hash.to_string_hex(true) << std::endl;

    // Operación XOR para "cifrado" simple
    auto encrypted = message_hash ^ private_key;
    std::cout << "Mensaje cifrado: " << encrypted.to_string_hex(true) << std::endl;

    // Descifrado
    auto decrypted = encrypted ^ private_key;
    std::cout << "Mensaje descifrado: " << decrypted.to_string_hex(true) << std::endl;
    std::cout << "Descifrado correcto: " << (decrypted == message_hash ? "✓" : "✗") << std::endl;
}

void demo_memory_addressing()
{
    std::cout << "\n=== DEMO: Direccionamiento de Memoria ===" << std::endl;

    // Simular direcciones de memoria de 128 bits
    auto base_addr = "0x7FFF800000000000"_u128;
    auto page_size = 4096_u128;

    std::cout << "Dirección base: " << base_addr.to_string_hex(true) << std::endl;
    std::cout << "Tamaño página: " << page_size.to_string() << " bytes" << std::endl;

    // Calcular direcciones de páginas
    std::vector<uint128_t> page_addresses;
    for (int i = 0; i < 5; ++i) {
        auto page_addr = base_addr + (page_size * uint128_t(i));
        page_addresses.push_back(page_addr);
        std::cout << "Página " << i << ": " << page_addr.to_string_hex(true) << std::endl;
    }

    // Verificar alineación
    for (size_t i = 0; i < page_addresses.size(); ++i) {
        auto aligned = (page_addresses[i] % page_size) == 0_u128;
        std::cout << "Página " << i << " alineada: " << (aligned ? "✓" : "✗") << std::endl;
    }
}

void demo_bit_manipulation()
{
    std::cout << "\n=== DEMO: Manipulación de Bits ===" << std::endl;

    // Crear máscaras usando literales binarios
    auto bit_mask = "11110000111100001111000011110000"_u128_bin;
    auto data = "0x12345678"_u128;

    std::cout << "Datos originales: " << data.to_string_hex(true) << std::endl;
    std::cout << "Máscara de bits:  " << bit_mask.to_string_hex(true) << std::endl;
    std::cout << "Máscara binaria:  " << bit_mask.to_string_bin(true) << std::endl;

    // Aplicar máscara
    auto masked_data = data & bit_mask;
    std::cout << "Datos enmascarados: " << masked_data.to_string_hex(true) << std::endl;

    // Invertir bits específicos
    auto inverted = data ^ bit_mask;
    std::cout << "Bits invertidos: " << inverted.to_string_hex(true) << std::endl;

    // Configurar bits específicos
    auto set_bits = data | bit_mask;
    std::cout << "Bits configurados: " << set_bits.to_string_hex(true) << std::endl;
}

void demo_number_systems()
{
    std::cout << "\n=== DEMO: Sistemas de Numeración ===" << std::endl;

    // Mismo número en diferentes representaciones
    auto decimal_val = "1234567890"_u128;
    auto hex_val = "499602D2"_u128_hex;                           // Mismo valor en hex
    auto binary_val = "1001001100101100000001011010010"_u128_bin; // Primeros bits
    auto octal_val = "11145401322"_u128_oct;                      // Mismo valor en octal

    std::cout << "Número: 1234567890 en diferentes bases:" << std::endl;
    std::cout << "Decimal: " << decimal_val.to_string() << std::endl;
    std::cout << "Hexadecimal: " << hex_val.to_string_hex(true) << std::endl;
    std::cout << "Octal: " << octal_val.to_string_oct(true) << std::endl;
    std::cout << "Binario (truncado): " << binary_val.to_string_bin(true) << std::endl;

    // Verificar que todos representan valores relacionados
    std::cout << "\nVerificaciones:" << std::endl;
    std::cout << "decimal == hex: " << (decimal_val == hex_val ? "✓" : "✗") << std::endl;
    std::cout << "decimal == octal: " << (decimal_val == octal_val ? "✓" : "✗") << std::endl;
}

void demo_large_numbers()
{
    std::cout << "\n=== DEMO: Números Extremadamente Grandes ===" << std::endl;

    // Usar literales para números que no caben en tipos nativos
    auto huge_number = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"_u128_hex;
    auto almost_max = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE"_u128_hex;

    std::cout << "Número enorme: " << huge_number.to_string() << std::endl;
    std::cout << "Casi máximo: " << almost_max.to_string() << std::endl;

    // Operaciones con números grandes
    auto difference = huge_number - almost_max;
    std::cout << "Diferencia: " << difference.to_string() << std::endl;

    // Suma que causaría overflow en tipos nativos
    auto big1 = "FFFFFFFFFFFFFFFF0000000000000000"_u128_hex;
    auto big2 = "0000000000000000FFFFFFFFFFFFFFFF"_u128_hex;
    auto big_sum = big1 + big2;

    std::cout << "Suma grande:" << std::endl;
    std::cout << "  " << big1.to_string_hex(true) << std::endl;
    std::cout << "+ " << big2.to_string_hex(true) << std::endl;
    std::cout << "= " << big_sum.to_string_hex(true) << std::endl;
    std::cout << "Decimal: " << big_sum.to_string() << std::endl;
}

int main()
{
    std::cout << "🚀 DEMOSTRACIÓN PRÁCTICA: LITERALES UDL Y FORMATOS 🚀" << std::endl;
    std::cout << "========================================================" << std::endl;

    demo_crypto_like_operations();
    demo_memory_addressing();
    demo_bit_manipulation();
    demo_number_systems();
    demo_large_numbers();

    std::cout << "\n✨ DEMOSTRACIÓN COMPLETADA ✨" << std::endl;
    std::cout << "\n🎯 BENEFICIOS DEMOSTRADOS:" << std::endl;
    std::cout << "   ✓ Literales UDL hacen el código más legible y expresivo" << std::endl;
    std::cout << "   ✓ Conversiones automáticas de formato simplifican el trabajo" << std::endl;
    std::cout << "   ✓ Soporte multi-base facilita trabajo con diferentes sistemas" << std::endl;
    std::cout << "   ✓ uint128_t ahora se integra naturalmente en código C++ moderno" << std::endl;
    std::cout << "\n💡 uint128_t está listo para aplicaciones reales de 128 bits!" << std::endl;

    return 0;
}
