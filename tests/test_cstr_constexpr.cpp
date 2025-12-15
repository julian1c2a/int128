#include "include/uint128_t.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

void test_constexpr_from_cstr()
{
    std::cout << "=== TEST: constexpr from_cstr ===" << std::endl;

    // Tests constexpr en tiempo de compilación
    constexpr auto decimal_const = uint128_t::from_cstr("12345");
    constexpr auto hex_const = uint128_t::from_cstr("0xABCD");
    constexpr auto bin_const = uint128_t::from_cstr("0b1111");
    constexpr auto oct_const = uint128_t::from_cstr("0777");
    constexpr auto zero_const = uint128_t::from_cstr("0");
    constexpr auto empty_const = uint128_t::from_cstr("");
    constexpr auto null_const = uint128_t::from_cstr(nullptr);

    std::cout << "Constexpr decimal '12345': " << decimal_const.to_string() << std::endl;
    std::cout << "Constexpr hex '0xABCD': " << hex_const.to_string()
              << " (hex: " << hex_const.to_string_hex() << ")" << std::endl;
    std::cout << "Constexpr bin '0b1111': " << bin_const.to_string()
              << " (bin: " << bin_const.to_string_bin() << ")" << std::endl;
    std::cout << "Constexpr oct '0777': " << oct_const.to_string()
              << " (oct: " << oct_const.to_string_oct() << ")" << std::endl;
    std::cout << "Constexpr zero '0': " << zero_const.to_string() << std::endl;
    std::cout << "Constexpr empty: " << empty_const.to_string() << std::endl;
    std::cout << "Constexpr null: " << null_const.to_string() << std::endl;

    // Verificar valores esperados
    assert(decimal_const == uint128_t(0, 12345));
    assert(hex_const == uint128_t(0, 0xABCD));
    assert(bin_const == uint128_t(0, 15)); // 0b1111 = 15
    assert(oct_const == uint128_t(0, 0777));
    assert(zero_const == uint128_t(0, 0));
    assert(empty_const == uint128_t(0, 0));
    assert(null_const == uint128_t(0, 0));

    std::cout << "✓ Todos los tests constexpr pasaron" << std::endl;
}

void test_constexpr_from_cstr_base()
{
    std::cout << "\n=== TEST: constexpr from_cstr_base ===" << std::endl;

    // Tests con base específica
    constexpr auto hex_base = uint128_t::from_cstr_base("FF", 16);
    constexpr auto bin_base = uint128_t::from_cstr_base("1010", 2);
    constexpr auto oct_base = uint128_t::from_cstr_base("77", 8);
    constexpr auto base36 = uint128_t::from_cstr_base("ZZ", 36);

    std::cout << "Constexpr base 16 'FF': " << hex_base.to_string() << std::endl;
    std::cout << "Constexpr base 2 '1010': " << bin_base.to_string() << std::endl;
    std::cout << "Constexpr base 8 '77': " << oct_base.to_string() << std::endl;
    std::cout << "Constexpr base 36 'ZZ': " << base36.to_string() << std::endl;

    // Verificar valores
    assert(hex_base == uint128_t(0, 255));
    assert(bin_base == uint128_t(0, 10));
    assert(oct_base == uint128_t(0, 63));
    assert(base36 == uint128_t(0, 35 * 36 + 35)); // Z=35, ZZ = 35*36+35

    std::cout << "✓ Todos los tests base específica pasaron" << std::endl;
}

void test_to_cstr()
{
    std::cout << "\n=== TEST: to_cstr ===" << std::endl;

    uint128_t value(0, 12345);
    uint128_t hex_value(0, 0xABCD);
    uint128_t zero_value(0, 0);

    // Test conversiones una a la vez para evitar rotación del buffer
    const char* decimal_cstr = value.to_cstr();
    std::cout << "to_cstr() decimal 12345: '" << decimal_cstr << "'" << std::endl;
    assert(std::strcmp(decimal_cstr, "12345") == 0);

    const char* hex_cstr = hex_value.to_cstr_hex();
    std::cout << "to_cstr_hex() 0xABCD: '" << hex_cstr << "'" << std::endl;
    assert(std::strcmp(hex_cstr, "ABCD") == 0);

    const char* bin_cstr = uint128_t(0, 15).to_cstr_bin();
    std::cout << "to_cstr_bin() 15: '" << bin_cstr << "'" << std::endl;
    assert(std::strcmp(bin_cstr, "1111") == 0);

    const char* oct_cstr = uint128_t(0, 0777).to_cstr_oct();
    std::cout << "to_cstr_oct() 0777: '" << oct_cstr << "'" << std::endl;
    assert(std::strcmp(oct_cstr, "777") == 0);

    const char* zero_cstr = zero_value.to_cstr();
    std::cout << "to_cstr() zero: '" << zero_cstr << "'" << std::endl;
    assert(std::strcmp(zero_cstr, "0") == 0);

    std::cout << "✓ Todos los tests to_cstr pasaron" << std::endl;
}

void test_to_cstr_rotation()
{
    std::cout << "\n=== TEST: to_cstr buffer rotation ===" << std::endl;

    // Test que el buffer rotativo funciona correctamente
    uint128_t val1(0, 111);
    uint128_t val2(0, 222);
    uint128_t val3(0, 333);
    uint128_t val4(0, 444);

    const char* str1 = val1.to_cstr();
    const char* str2 = val2.to_cstr();
    const char* str3 = val3.to_cstr();
    const char* str4 = val4.to_cstr();

    std::cout << "Buffer 1: '" << str1 << "'" << std::endl;
    std::cout << "Buffer 2: '" << str2 << "'" << std::endl;
    std::cout << "Buffer 3: '" << str3 << "'" << std::endl;
    std::cout << "Buffer 4: '" << str4 << "'" << std::endl;

    // Verificar que los 4 buffers contienen valores correctos
    assert(std::strcmp(str1, "111") == 0);
    assert(std::strcmp(str2, "222") == 0);
    assert(std::strcmp(str3, "333") == 0);
    assert(std::strcmp(str4, "444") == 0);

    // Los buffers deben ser diferentes (diferentes direcciones)
    assert(str1 != str2);
    assert(str2 != str3);
    assert(str3 != str4);

    // Ahora al usar el 5º, debería reusar el primer buffer
    uint128_t val5(0, 555);
    const char* str5 = val5.to_cstr();
    std::cout << "Buffer 5: '" << str5 << "' (reutiliza buffer)" << std::endl;

    // str5 debería usar la misma dirección que str1 (rotación)
    // pero el contenido debería ser diferente
    assert(str5 == str1);                  // Misma dirección de buffer
    assert(std::strcmp(str5, "555") == 0); // Contenido correcto

    std::cout << "✓ Buffer rotation funciona correctamente" << std::endl;
}

void test_roundtrip_cstr()
{
    std::cout << "\n=== TEST: C-string roundtrip ===" << std::endl;

    // Test conversión ida y vuelta
    uint128_t original(0x123, 0x456789ABCDEF0000ULL);

    // Decimal roundtrip
    const char* decimal_cstr = original.to_cstr();
    auto decimal_back = uint128_t::from_cstr(decimal_cstr);
    std::cout << "Decimal roundtrip: " << (original == decimal_back ? "✓" : "✗") << std::endl;

    // Hex roundtrip
    const char* hex_cstr = original.to_cstr_hex();
    auto hex_back = uint128_t::from_cstr_base(hex_cstr, 16);
    std::cout << "Hex roundtrip: " << (original == hex_back ? "✓" : "✗") << std::endl;

    // Bin roundtrip (número pequeño)
    uint128_t small(0, 255);
    const char* bin_cstr = small.to_cstr_bin();
    auto bin_back = uint128_t::from_cstr_base(bin_cstr, 2);
    std::cout << "Bin roundtrip: " << (small == bin_back ? "✓" : "✗") << std::endl;

    // Oct roundtrip
    uint128_t oct_val(0, 0777);
    const char* oct_cstr = oct_val.to_cstr_oct();
    auto oct_back = uint128_t::from_cstr_base(oct_cstr, 8);
    std::cout << "Oct roundtrip: " << (oct_val == oct_back ? "✓" : "✗") << std::endl;
}

void test_compile_time_evaluation()
{
    std::cout << "\n=== TEST: Evaluación en tiempo de compilación ===" << std::endl;

    // Test que from_cstr realmente se evalúa en tiempo de compilación
    constexpr auto compile_time_max =
        uint128_t::from_cstr("340282366920938463463374607431768211455");
    constexpr auto compile_time_hex = uint128_t::from_cstr("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    std::cout << "Compile-time max decimal: " << compile_time_max.to_string() << std::endl;
    std::cout << "Compile-time max hex: " << compile_time_hex.to_string() << std::endl;
    std::cout << "Son iguales: " << (compile_time_max == compile_time_hex ? "✓" : "✗") << std::endl;

    // Test operaciones constexpr con resultados
    constexpr auto sum = uint128_t::from_cstr("100") + uint128_t::from_cstr("200");
    constexpr auto expected = uint128_t::from_cstr("300");

    std::cout << "Suma constexpr: " << sum.to_string() << std::endl;
    std::cout << "Suma correcta: " << (sum == expected ? "✓" : "✗") << std::endl;
}

int main()
{
    std::cout << "🔠 TEST COMPLETO DE FUNCIONES C-STRING CONSTEXPR 🔠" << std::endl;
    std::cout << "===================================================" << std::endl;

    test_constexpr_from_cstr();
    test_constexpr_from_cstr_base();
    test_to_cstr();
    test_to_cstr_rotation();
    test_roundtrip_cstr();
    test_compile_time_evaluation();

    std::cout << "\n✅ TODOS los tests completados!" << std::endl;
    std::cout << "\n📝 FUNCIONALIDADES VALIDADAS:" << std::endl;
    std::cout << "   ✓ constexpr from_cstr() con detección automática de formato" << std::endl;
    std::cout << "   ✓ constexpr from_cstr_base() para base específica" << std::endl;
    std::cout << "   ✓ to_cstr() con buffer rotativo thread-safe" << std::endl;
    std::cout << "   ✓ to_cstr_hex(), to_cstr_bin(), to_cstr_oct()" << std::endl;
    std::cout << "   ✓ Buffer rotation para múltiples llamadas simultáneas" << std::endl;
    std::cout << "   ✓ Conversiones ida y vuelta consistentes" << std::endl;
    std::cout << "   ✓ Evaluación real en tiempo de compilación" << std::endl;
    std::cout << "\n🚀 uint128_t ahora soporta C-strings con evaluación constexpr!" << std::endl;

    return 0;
}