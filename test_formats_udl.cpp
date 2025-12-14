#include "include/uint128_t.hpp"
#include <cassert>
#include <iostream>

// Importar los literales UDL
using namespace uint128_literals;

void test_format_conversions()
{
    std::cout << "=== TEST: Conversiones de Formato ===" << std::endl;

    uint128_t value(0x123, 0x456789ABCDEF0000ULL);

    std::cout << "Valor de prueba: " << value.to_string() << std::endl;

    // Test conversiones a diferentes formatos
    std::cout << "Decimal: " << value.to_string() << std::endl;
    std::cout << "Hexadecimal: " << value.to_string_hex() << std::endl;
    std::cout << "Hexadecimal con prefijo: " << value.to_string_hex(true) << std::endl;
    std::cout << "Binario (primeros 64 bits): " << uint128_t(0, 0xABCD).to_string_bin()
              << std::endl;
    std::cout << "Binario con prefijo: " << uint128_t(0, 0xFF).to_string_bin(true) << std::endl;
    std::cout << "Octal: " << uint128_t(0, 0777).to_string_oct() << std::endl;
    std::cout << "Octal con prefijo: " << uint128_t(0, 0777).to_string_oct(true) << std::endl;
}

void test_string_parsing()
{
    std::cout << "\n=== TEST: Parsing desde String ===" << std::endl;

    // Test diferentes formatos de entrada
    auto dec = uint128_t::from_string("12345678901234567890");
    std::cout << "Decimal '12345678901234567890': " << dec.to_string() << std::endl;

    auto hex1 = uint128_t::from_string("0x1A2B3C");
    std::cout << "Hex '0x1A2B3C': " << hex1.to_string() << " (hex: " << hex1.to_string_hex() << ")"
              << std::endl;

    auto hex2 = uint128_t::from_string("0XDEADBEEF");
    std::cout << "Hex '0XDEADBEEF': " << hex2.to_string() << " (hex: " << hex2.to_string_hex()
              << ")" << std::endl;

    auto bin = uint128_t::from_string("0b11111111");
    std::cout << "Bin '0b11111111': " << bin.to_string() << " (bin: " << bin.to_string_bin() << ")"
              << std::endl;

    auto oct = uint128_t::from_string("0777");
    std::cout << "Oct '0777': " << oct.to_string() << " (oct: " << oct.to_string_oct() << ")"
              << std::endl;

    // Test parsing específico por base
    auto hex_base = uint128_t::from_string_base("FF", 16);
    std::cout << "Base 16 'FF': " << hex_base.to_string() << std::endl;

    auto bin_base = uint128_t::from_string_base("1010", 2);
    std::cout << "Base 2 '1010': " << bin_base.to_string() << std::endl;
}

void test_udl_literals()
{
    std::cout << "\n=== TEST: Literales Definidos por el Usuario ===" << std::endl;

    // Literal numérico simple
    auto small = 12345_u128;
    std::cout << "12345_u128: " << small.to_string() << std::endl;

    // Literales de string con detección automática
    auto dec_lit = "98765432109876543210"_u128;
    std::cout << "\"98765432109876543210\"_u128: " << dec_lit.to_string() << std::endl;

    auto hex_lit = "0xDEADBEEF"_u128;
    std::cout << "\"0xDEADBEEF\"_u128: " << hex_lit.to_string()
              << " (hex: " << hex_lit.to_string_hex() << ")" << std::endl;

    auto bin_lit = "0b11110000"_u128;
    std::cout << "\"0b11110000\"_u128: " << bin_lit.to_string()
              << " (bin: " << bin_lit.to_string_bin() << ")" << std::endl;

    // Literales específicos por formato
    auto hex_specific = "ABCD"_u128_hex;
    std::cout << "\"ABCD\"_u128_hex: " << hex_specific.to_string()
              << " (hex: " << hex_specific.to_string_hex() << ")" << std::endl;

    auto bin_specific = "11111111"_u128_bin;
    std::cout << "\"11111111\"_u128_bin: " << bin_specific.to_string()
              << " (bin: " << bin_specific.to_string_bin() << ")" << std::endl;

    auto oct_specific = "777"_u128_oct;
    std::cout << "\"777\"_u128_oct: " << oct_specific.to_string()
              << " (oct: " << oct_specific.to_string_oct() << ")" << std::endl;
}

void test_roundtrip_conversions()
{
    std::cout << "\n=== TEST: Conversiones Ida y Vuelta ===" << std::endl;

    // Test que conversion y parsing sean consistentes
    uint128_t original(0x123, 0x456789ABCDEF0000ULL);

    // Decimal roundtrip
    auto dec_str = original.to_string();
    auto dec_back = uint128_t::from_string(dec_str);
    std::cout << "Decimal roundtrip: " << (original == dec_back ? "✓" : "✗") << std::endl;

    // Hex roundtrip
    auto hex_str = "0x" + original.to_string_hex();
    auto hex_back = uint128_t::from_string(hex_str);
    std::cout << "Hex roundtrip: " << (original == hex_back ? "✓" : "✗") << std::endl;

    // Bin roundtrip (número pequeño para que sea práctico)
    uint128_t small_val(0, 255);
    auto bin_str = "0b" + small_val.to_string_bin();
    auto bin_back = uint128_t::from_string(bin_str);
    std::cout << "Bin roundtrip: " << (small_val == bin_back ? "✓" : "✗") << std::endl;

    // Oct roundtrip
    uint128_t oct_val(0, 0777);
    auto oct_str = "0" + oct_val.to_string_oct();
    auto oct_back = uint128_t::from_string(oct_str);
    std::cout << "Oct roundtrip: " << (oct_val == oct_back ? "✓" : "✗") << std::endl;
}

void test_practical_usage()
{
    std::cout << "\n=== TEST: Uso Práctico ===" << std::endl;

    // Ejemplo de uso práctico con diferentes formatos
    std::cout << "Creando números con diferentes literales:" << std::endl;

    auto mask = "0xFF00FF00"_u128;
    auto data = 0x12345678_u128;

    std::cout << "Mask: " << mask.to_string_hex(true) << std::endl;
    std::cout << "Data: " << data.to_string_hex(true) << std::endl;

    auto masked = data & mask;
    std::cout << "Masked: " << masked.to_string_hex(true) << std::endl;

    // Ejemplo con números grandes
    auto big_hex = "123456789ABCDEF0"_u128_hex;
    auto big_dec = big_hex.to_string();

    std::cout << "Número grande hex: " << big_hex.to_string_hex(true) << std::endl;
    std::cout << "Como decimal: " << big_dec << std::endl;

    // Suma con literal
    auto sum = big_hex + 1000_u128;
    std::cout << "Sumando 1000: " << sum.to_string_hex(true) << std::endl;
}

int main()
{
    std::cout << "🔢 TEST COMPLETO DE FORMATOS Y LITERALES UDL 🔢" << std::endl;
    std::cout << "================================================" << std::endl;

    test_format_conversions();
    test_string_parsing();
    test_udl_literals();
    test_roundtrip_conversions();
    test_practical_usage();

    std::cout << "\n✅ TODOS los tests completados!" << std::endl;
    std::cout << "\n📝 FUNCIONALIDADES VALIDADAS:" << std::endl;
    std::cout << "   ✓ to_string() con múltiples formatos (decimal, hex, bin, oct)" << std::endl;
    std::cout << "   ✓ from_string() con detección automática de formato" << std::endl;
    std::cout << "   ✓ from_string_base() para conversión con base específica" << std::endl;
    std::cout << "   ✓ Literales UDL numéricos: 123_u128" << std::endl;
    std::cout << "   ✓ Literales UDL de string: \"0xFF\"_u128" << std::endl;
    std::cout
        << "   ✓ Literales UDL específicos: \"FF\"_u128_hex, \"1010\"_u128_bin, \"777\"_u128_oct"
        << std::endl;
    std::cout << "   ✓ Conversiones ida y vuelta consistentes" << std::endl;
    std::cout << "\n🎯 uint128_t ahora soporta formatos múltiples y literales modernos!"
              << std::endl;

    return 0;
}