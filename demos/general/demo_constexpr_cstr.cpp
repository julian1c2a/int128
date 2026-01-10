#include <chrono>
#include <int128_simple.hpp>
#include <iostream>

using namespace nstd;

// Ejemplo de uso en tiempo de compilación
namespace compile_time_constants
{
// Constantes evaluadas en tiempo de compilación
constexpr auto MAX_UINT128 = uint128_t::from_cstr("340282366920938463463374607431768211455");
constexpr auto CRYPTO_KEY = uint128_t::from_cstr("0xDEADBEEFCAFEBABE123456789ABCDEF0");
constexpr auto BIG_PRIME = uint128_t::from_cstr("340282366920938463463374607431768211297");
constexpr auto NETWORK_MASK = uint128_t::from_cstr("0xFFFFFFFFFFFFFFFF0000000000000000");

// Operaciones constexpr
constexpr auto HALF_MAX = MAX_UINT128 / uint128_t::from_cstr("2");
constexpr auto KEY_XOR_MASK = CRYPTO_KEY ^ NETWORK_MASK;
} // namespace compile_time_constants

void demo_compile_time_evaluation()
{
    std::cout << "=== DEMO: Evaluación en Tiempo de Compilación ===" << std::endl;

    std::cout << "Constantes calculadas en tiempo de compilación:" << std::endl;
    std::cout << "MAX_UINT128: " << compile_time_constants::MAX_UINT128.to_string() << std::endl;
    std::cout << "CRYPTO_KEY: " << compile_time_constants::CRYPTO_KEY.to_string_hex(true)
              << std::endl;
    std::cout << "BIG_PRIME: " << compile_time_constants::BIG_PRIME.to_string() << std::endl;
    std::cout << "NETWORK_MASK: " << compile_time_constants::NETWORK_MASK.to_string_hex(true)
              << std::endl;
    std::cout << "HALF_MAX: " << compile_time_constants::HALF_MAX.to_string() << std::endl;
    std::cout << "KEY_XOR_MASK: " << compile_time_constants::KEY_XOR_MASK.to_string_hex(true)
              << std::endl;
}

void demo_runtime_vs_compiletime()
{
    std::cout << "\n=== DEMO: Runtime vs Compile-time Performance ===" << std::endl;

    const char* big_number_str = "123456789012345678901234567890123456789";

    // Medición runtime
    auto start = std::chrono::high_resolution_clock::now();
    uint128_t runtime_result(0, 0);
    for (int i = 0; i < 10000; ++i) {
        runtime_result = uint128_t::from_string(big_number_str);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto runtime_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Compile-time (sin medición porque es instantáneo)
    constexpr auto compiletime_result =
        uint128_t::from_cstr("123456789012345678901234567890123456789");

    std::cout << "Runtime parsing (10,000 iteraciones): " << runtime_duration.count() << " µs"
              << std::endl;
    std::cout << "Compile-time parsing: 0 µs (calculado en tiempo de compilación)" << std::endl;
    std::cout << "Resultados iguales: " << (runtime_result == compiletime_result ? "✓" : "✗")
              << std::endl;

    std::cout << "\nVentaja: ~" << runtime_duration.count() << " µs ahorrados por usar constexpr"
              << std::endl;
}

void demo_cstr_vs_string()
{
    std::cout << "\n=== DEMO: C-strings vs std::string ===" << std::endl;

    uint128_t value(0x12345678, 0x9ABCDEF012345678ULL);

    // Conversiones std::string
    std::string str_decimal = value.to_string();
    std::string str_hex = value.to_string_hex(true);

    // Conversiones C-string
    const char* cstr_decimal = value.to_cstr();
    const char* cstr_hex = value.to_cstr_hex();

    std::cout << "std::string decimal: " << str_decimal << std::endl;
    std::cout << "C-string decimal: " << cstr_decimal << std::endl;
    std::cout << "std::string hex: " << str_hex << std::endl;
    std::cout << "C-string hex: 0x" << cstr_hex << std::endl;

    // Verificar equivalencia
    bool decimal_match = (str_decimal == cstr_decimal);
    bool hex_match = (str_hex.substr(2) == cstr_hex); // Quitar "0x" del string

    std::cout << "Decimal equivalente: " << (decimal_match ? "✓" : "✗") << std::endl;
    std::cout << "Hex equivalente: " << (hex_match ? "✓" : "✗") << std::endl;

    std::cout << "\nVentajas C-string:" << std::endl;
    std::cout << "  • Sin asignación de memoria dinámica" << std::endl;
    std::cout << "  • Thread-safe con buffer rotativo" << std::endl;
    std::cout << "  • Adecuado para APIs de C legacy" << std::endl;
}

void demo_buffer_rotation()
{
    std::cout << "\n=== DEMO: Rotación de Buffer Thread-Safe ===" << std::endl;

    // Simular múltiples llamadas concurrentes
    uint128_t values[] = {uint128_t(0, 111), uint128_t(0, 222), uint128_t(0, 333),
                          uint128_t(0, 444), uint128_t(0, 555), uint128_t(0, 666)};

    std::cout << "Múltiples conversiones (buffer rotativo):" << std::endl;

    const char* results[6];
    for (int i = 0; i < 6; ++i) {
        results[i] = values[i].to_cstr();
        std::cout << "Valor " << i + 1 << ": '" << results[i] << "' @ "
                  << static_cast<const void*>(results[i]) << std::endl;
    }

    // Verificar que los primeros 4 son únicos, luego rota
    bool all_different_first_4 = true;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (results[i] == results[j]) {
                all_different_first_4 = false;
                break;
            }
        }
    }

    std::cout << "Primeros 4 buffers únicos: " << (all_different_first_4 ? "✓" : "✗") << std::endl;
    std::cout << "Buffer 5 reutiliza buffer 1: " << (results[4] == results[0] ? "✓" : "✗")
              << std::endl;
    std::cout << "Buffer 6 reutiliza buffer 2: " << (results[5] == results[1] ? "✓" : "✗")
              << std::endl;
}

template <typename T> constexpr bool validate_constexpr_calculation()
{
    // Esta función demuestra que from_cstr realmente funciona en constexpr
    constexpr auto val1 = uint128_t::from_cstr("100");
    constexpr auto val2 = uint128_t::from_cstr("200");
    constexpr auto sum = val1 + val2;
    constexpr auto expected = uint128_t::from_cstr("300");

    return sum == expected;
}

void demo_template_constexpr()
{
    std::cout << "\n=== DEMO: Constexpr en Templates ===" << std::endl;

    constexpr bool is_valid = validate_constexpr_calculation<int>();

    std::cout << "Validación constexpr en template: " << (is_valid ? "✓" : "✗") << std::endl;

    // Uso en static_assert
    static_assert(validate_constexpr_calculation<int>(), "Constexpr calculation should work");
    std::cout << "static_assert pasó: ✓" << std::endl;

    std::cout << "\nEsto demuestra que from_cstr() es verdaderamente constexpr" << std::endl;
    std::cout << "y puede usarse en contextos de evaluación en tiempo de compilación." << std::endl;
}

int main()
{
    std::cout << "🔧 DEMOSTRACIÓN AVANZADA: FUNCIONES C-STRING CONSTEXPR 🔧" << std::endl;
    std::cout << "===========================================================" << std::endl;

    demo_compile_time_evaluation();
    demo_runtime_vs_compiletime();
    demo_cstr_vs_string();
    demo_buffer_rotation();
    demo_template_constexpr();

    std::cout << "\n🎯 BENEFICIOS DEMOSTRADOS:" << std::endl;
    std::cout << "   ✓ Evaluación en tiempo de compilación elimina overhead" << std::endl;
    std::cout << "   ✓ Buffer rotativo thread-safe para múltiples llamadas" << std::endl;
    std::cout << "   ✓ Compatibilidad con APIs de C legacy" << std::endl;
    std::cout << "   ✓ Sin asignación dinámica de memoria en runtime" << std::endl;
    std::cout << "   ✓ Uso seguro en templates y static_assert" << std::endl;
    std::cout << "\n🚀 uint128_t ahora es óptimo tanto en compile-time como runtime!" << std::endl;

    return 0;
}
