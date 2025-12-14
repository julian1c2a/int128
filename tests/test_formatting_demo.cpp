#include "../include/uint128_simple_traits.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

void demo_basic_formatting()
{
    std::cout << "\n━━━ FORMATEO BÁSICO DE uint128_t ━━━\n" << std::endl;

    uint128_t value(0, 255);

    std::cout << "Valor 255 en diferentes bases:" << std::endl;
    std::cout << "  Decimal:     " << std::dec << value << std::endl;
    std::cout << "  Hexadecimal: " << std::hex << value << std::endl;
    std::cout << "  Octal:       " << std::oct << value << std::endl;

    std::cout << "\nCon prefijos de base:" << std::endl;
    std::cout << "  Hexadecimal: " << std::hex << std::showbase << value << std::noshowbase
              << std::endl;
    std::cout << "  Octal:       " << std::oct << std::showbase << value << std::noshowbase
              << std::endl;
}

void demo_width_and_alignment()
{
    std::cout << "\n━━━ CONTROL DE ANCHO Y ALINEACIÓN ━━━\n" << std::endl;

    uint128_t value(0, 42);

    std::cout << "Valor 42 con ancho 15:" << std::endl;
    std::cout << "  Derecha:   |" << std::dec << std::setw(15) << std::right << value << "|"
              << std::endl;
    std::cout << "  Izquierda: |" << std::setw(15) << std::left << value << "|" << std::endl;
    std::cout << "  Relleno 0: |" << std::setw(15) << std::right << std::setfill('0') << value
              << "|" << std::endl;

    std::cout << std::setfill(' '); // Restaurar
}

void demo_hex_formatting()
{
    std::cout << "\n━━━ FORMATEO HEXADECIMAL AVANZADO ━━━\n" << std::endl;

    uint128_t large_value(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    std::cout << "Valor: 0x123456789ABCDEF0FEDCBA9876543210" << std::endl;
    std::cout << "  Básico:       " << std::hex << large_value << std::endl;
    std::cout << "  Con prefijo:  " << std::showbase << large_value << std::noshowbase << std::endl;
    std::cout << "  Mayúsculas:   " << std::uppercase << large_value << std::nouppercase
              << std::endl;
    std::cout << "  Ancho 40:     " << std::setw(40) << std::setfill('0') << large_value
              << std::endl;
    std::cout << "  Interno:      " << std::setw(42) << std::showbase << std::internal
              << large_value << std::noshowbase << std::endl;

    std::cout << std::setfill(' ') << std::right; // Restaurar
}

void demo_convenience_functions()
{
    std::cout << "\n━━━ FUNCIONES DE CONVENIENCIA ━━━\n" << std::endl;

    uint128_t value(0x1234, 0x5678);

    std::cout << "Valor para demostrar: " << std::dec << value << std::endl;
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

    std::cout << "ID de transacción (hex, mayúsc.):  " << std::hex << std::uppercase
              << std::showbase << transaction_id << std::endl;

    std::cout << "Cantidad monetaria (decimal):      " << std::dec << std::noshowbase
              << large_amount << std::endl;

    std::cout << "Dirección de memoria (hex, pad):   "
              << uint128_format::hex(memory_address, 20, true, true, '0') << std::endl;

    // Tabla formateada
    std::cout << "\nTabla de valores:" << std::endl;
    std::cout << "┌─────────────────┬──────────────────────────┬──────────────┐" << std::endl;
    std::cout << "│      Decimal    │       Hexadecimal       │    Octal     │" << std::endl;
    std::cout << "├─────────────────┼──────────────────────────┼──────────────┤" << std::endl;

    for (uint64_t i : {255, 1024, 65535}) {
        uint128_t val(0, i);
        std::cout << "│ " << std::dec << std::setw(15) << val << " │ ";
        std::cout << std::hex << std::setw(24) << std::showbase << val << std::noshowbase << " │ ";
        std::cout << std::oct << std::setw(12) << std::showbase << val << std::noshowbase << " │"
                  << std::endl;
    }
    std::cout << "└─────────────────┴──────────────────────────┴──────────────┘" << std::endl;
}

int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║               DEMOSTRACIÓN DE FORMATEO uint128_t             ║" << std::endl;
    std::cout << "║        Todas las capacidades de iostream/iomanip            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;

    demo_basic_formatting();
    demo_width_and_alignment();
    demo_hex_formatting();
    demo_convenience_functions();
    demo_realistic_usage();

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                       ✅ CONCLUSIONES                        ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║ uint128_t ahora tiene SOPORTE COMPLETO de formateo:         ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║ 🔸 Bases: dec, hex, oct con std::dec, std::hex, std::oct    ║" << std::endl;
    std::cout << "║ 🔸 Ancho: std::setw() con relleno std::setfill()           ║" << std::endl;
    std::cout << "║ 🔸 Alineación: left, right, internal                       ║" << std::endl;
    std::cout << "║ 🔸 Prefijos: std::showbase (0x, 0)                         ║" << std::endl;
    std::cout << "║ 🔸 Mayúsculas: std::uppercase para hexadecimal             ║" << std::endl;
    std::cout << "║ 🔸 Funciones de conveniencia personalizadas                ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║ ✅ uint128_t es TAN FÁCIL de formatear como int/long!      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}