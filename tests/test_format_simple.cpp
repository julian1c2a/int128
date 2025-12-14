#include "../include/uint128_format.hpp"
#include "../include/uint128_t.hpp"
#include <iomanip>
#include <iostream>

void demo_basic_formatting()
{
    std::cout << "\n━━━ FORMATEO BÁSICO DE uint128_t ━━━\n" << std::endl;

    uint128_t value(0, 255);

    std::cout << "Valor 255 en diferentes bases:" << std::endl;
    std::cout << "  Decimal:     " << uint128_format::dec(value) << std::endl;
    std::cout << "  Hexadecimal: " << uint128_format::hex(value) << std::endl;
    std::cout << "  Octal:       " << uint128_format::oct(value) << std::endl;

    std::cout << "\nCon prefijos de base:" << std::endl;
    std::cout << "  Hexadecimal: " << uint128_format::hex(value, 0, true) << std::endl;
    std::cout << "  Octal:       " << uint128_format::oct(value, 0, true) << std::endl;
}

void demo_width_and_alignment()
{
    std::cout << "\n━━━ CONTROL DE ANCHO Y ALINEACIÓN ━━━\n" << std::endl;

    uint128_t value(0, 42);

    std::cout << "Valor 42 con ancho 15:" << std::endl;
    std::cout << "  Derecha:   |"
              << uint128_format::format(value, 10, 15, ' ', false, false, false, false) << "|"
              << std::endl;
    std::cout << "  Izquierda: |"
              << uint128_format::format(value, 10, 15, ' ', false, false, true, false) << "|"
              << std::endl;
    std::cout << "  Relleno 0: |"
              << uint128_format::format(value, 10, 15, '0', false, false, false, false) << "|"
              << std::endl;
}

void demo_hex_formatting()
{
    std::cout << "\n━━━ FORMATEO HEXADECIMAL AVANZADO ━━━\n" << std::endl;

    uint128_t large_value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    std::cout << "Valor: 0x123456789ABCDEF0FEDCBA9876543210" << std::endl;
    std::cout << "  Básico:       " << uint128_format::hex(large_value) << std::endl;
    std::cout << "  Con prefijo:  " << uint128_format::hex(large_value, 0, true) << std::endl;
    std::cout << "  Mayúsculas:   " << uint128_format::hex(large_value, 0, true, true) << std::endl;
    std::cout << "  Ancho 40:     " << uint128_format::hex(large_value, 40, false, false, '0')
              << std::endl;
    std::cout << "  Interno:      "
              << uint128_format::format(large_value, 16, 42, '0', true, false, false, true)
              << std::endl;
}

void demo_convenience_functions()
{
    std::cout << "\n━━━ FUNCIONES DE CONVENIENCIA ━━━\n" << std::endl;

    uint128_t value(0x1234, 0x5678);

    std::cout << "Valor para demostrar: " << value << std::endl;
    std::cout << "  hex():                    " << uint128_format::hex(value) << std::endl;
    std::cout << "  hex(20, true, true):      " << uint128_format::hex(value, 20, true, true)
              << std::endl;
    std::cout << "  oct(25, true):            " << uint128_format::oct(value, 25, true)
              << std::endl;
    std::cout << "  dec(20, '*', true):       " << uint128_format::dec(value, 20, '*', true)
              << std::endl;
}

void demo_realistic_usage()
{
    std::cout << "\n━━━ EJEMPLOS REALISTAS ━━━\n" << std::endl;

    // Simular diferentes tipos de datos
    uint128_t transaction_id(0x12345678, 0x9ABCDEF0);
    uint128_t large_amount = uint128_t::from_string("99999999999999999999999");
    uint128_t memory_address(0x7FFF, 0x12345678);

    std::cout << "ID de transacción (hex, mayúsc.):  "
              << uint128_format::hex(transaction_id, 0, true, true) << std::endl;

    std::cout << "Cantidad monetaria (decimal):      " << uint128_format::dec(large_amount)
              << std::endl;

    std::cout << "Dirección de memoria (hex, pad):   "
              << uint128_format::hex(memory_address, 20, true, true, '0') << std::endl;

    // Tabla formateada
    std::cout << "\nTabla de valores:" << std::endl;
    std::cout << "┌─────────────────┬──────────────────────────┬──────────────┐" << std::endl;
    std::cout << "│      Decimal    │       Hexadecimal       │    Octal     │" << std::endl;
    std::cout << "├─────────────────┼──────────────────────────┼──────────────┤" << std::endl;

    for (uint64_t i : {255, 1024, 65535}) {
        uint128_t val(0, i);
        std::cout << "│ " << uint128_format::format(val, 10, 15, ' ') << " │ ";
        std::cout << uint128_format::format(val, 16, 24, ' ', true, false) << " │ ";
        std::cout << uint128_format::format(val, 8, 12, ' ', true, false) << " │" << std::endl;
    }
    std::cout << "└─────────────────┴──────────────────────────┴──────────────┘" << std::endl;
}

void demo_comparison_with_builtin()
{
    std::cout << "\n━━━ COMPARACIÓN CON TIPOS BUILTIN ━━━\n" << std::endl;

    uint64_t builtin_val = 0xDEADBEEF;
    uint128_t our_val(0, 0xDEADBEEF);

    std::cout << "Formateo idéntico para la misma funcionalidad:" << std::endl;

    // Formateo builtin usando iostream tradicional
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::showbase << std::setw(15) << std::setfill('0')
        << builtin_val;
    std::cout << "  uint64_t (iostream):  " << oss.str() << std::endl;

    // Formateo nuestro usando funciones de conveniencia
    std::cout << "  uint128_t (format):   "
              << uint128_format::format(our_val, 16, 15, '0', true, true, false, true) << std::endl;
}

int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║               DEMOSTRACIÓN DE FORMATEO uint128_t             ║" << std::endl;
    std::cout << "║     Funciones que simulan iostream/iomanip para uint128_t   ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;

    demo_basic_formatting();
    demo_width_and_alignment();
    demo_hex_formatting();
    demo_convenience_functions();
    demo_realistic_usage();
    demo_comparison_with_builtin();

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                       ✅ CONCLUSIONES                        ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║ RESPUESTA: ¿Son fáciles de conseguir las posibilidades      ║" << std::endl;
    std::cout << "║ de formateo de iostream/iomanip para uint128_t?              ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║ ✅ SÍ - Con funciones de conveniencia es MUY FÁCIL:         ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║ 🔸 Bases: uint128_format::hex(), oct(), dec()               ║" << std::endl;
    std::cout << "║ 🔸 Ancho y relleno: width, fill parameters                  ║" << std::endl;
    std::cout << "║ 🔸 Alineación: left_align, internal_align params           ║" << std::endl;
    std::cout << "║ 🔸 Prefijos: show_base parameter                            ║" << std::endl;
    std::cout << "║ 🔸 Mayúsculas: uppercase parameter                          ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║ 📝 NOTA: Los manipuladores iostream estándar no funcionan   ║" << std::endl;
    std::cout << "║ automáticamente, pero nuestras funciones proporcionan       ║" << std::endl;
    std::cout << "║ la MISMA funcionalidad de manera intuitiva y fácil.         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}