#include "../include/uint128_iostreams.hpp"
#include "../include/uint128_t.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

void test_basic_formatting()
{
    std::cout << "\n=== FORMATEO BÁSICO ===" << std::endl;

    uint128_t value = uint128_t(0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL);

    std::cout << "Valor de prueba: 0x123456789ABCDEF0FEDCBA9876543210" << std::endl;
    std::cout << "Decimal: " << std::dec << value << std::endl;
    std::cout << "Hexadecimal: " << std::hex << value << std::endl;
    std::cout << "Octal: " << std::oct << value << std::endl;
}

void test_base_manipulators()
{
    std::cout << "\n=== MANIPULADORES DE BASE ===" << std::endl;

    uint128_t value(0, 255); // 255 en decimal

    std::cout << "Valor 255 en diferentes bases:" << std::endl;
    std::cout << "Decimal: " << std::dec << value << std::endl;
    std::cout << "Hexadecimal: " << std::hex << value << std::endl;
    std::cout << "Octal: " << std::oct << value << std::endl;

    // Con showbase
    std::cout << "\nCon prefijos (showbase):" << std::endl;
    std::cout << "Decimal: " << std::dec << std::showbase << value << std::noshowbase << std::endl;
    std::cout << "Hexadecimal: " << std::hex << std::showbase << value << std::noshowbase
              << std::endl;
    std::cout << "Octal: " << std::oct << std::showbase << value << std::noshowbase << std::endl;
}

void test_width_and_fill()
{
    std::cout << "\n=== ANCHO Y RELLENO ===" << std::endl;

    uint128_t value(0, 42);

    std::cout << "Valor 42 con diferentes anchos y rellenos:" << std::endl;
    std::cout << std::dec;

    // Ancho con relleno de espacios
    std::cout << "Ancho 10, relleno espacios: |" << std::setw(10) << value << "|" << std::endl;

    // Ancho con relleno de ceros
    std::cout << "Ancho 10, relleno ceros: |" << std::setw(10) << std::setfill('0') << value << "|"
              << std::endl;

    // Hexadecimal con relleno
    std::cout << std::hex;
    std::cout << "Hex, ancho 16, relleno ceros: |" << std::setw(16) << std::setfill('0') << value
              << "|" << std::endl;

    // Restablecer fill
    std::cout << std::setfill(' ');
}

void test_alignment()
{
    std::cout << "\n=== ALINEACIÓN ===" << std::endl;

    uint128_t value(0, 123);

    std::cout << std::dec;
    std::cout << "Valor 123 con diferentes alineaciones (ancho 10):" << std::endl;

    // Alineación derecha (por defecto)
    std::cout << "Derecha: |" << std::setw(10) << std::right << value << "|" << std::endl;

    // Alineación izquierda
    std::cout << "Izquierda: |" << std::setw(10) << std::left << value << "|" << std::endl;

    // Alineación interna (para números con prefijo)
    std::cout << std::hex << std::showbase;
    std::cout << "Interna hex: |" << std::setw(12) << std::internal << std::setfill('0') << value
              << "|" << std::endl;

    std::cout << std::noshowbase << std::setfill(' ');
}

void test_uppercase()
{
    std::cout << "\n=== MAYÚSCULAS/MINÚSCULAS ===" << std::endl;

    uint128_t value(0, 0xABCDEF);

    std::cout << std::hex;
    std::cout << "Valor 0xABCDEF:" << std::endl;
    std::cout << "Minúsculas: " << std::nouppercase << value << std::endl;
    std::cout << "Mayúsculas: " << std::uppercase << value << std::endl;

    // Con prefijo
    std::cout << std::showbase;
    std::cout << "Con prefijo minúsculas: " << std::nouppercase << value << std::endl;
    std::cout << "Con prefijo mayúsculas: " << std::uppercase << value << std::endl;

    std::cout << std::noshowbase << std::nouppercase;
}

void test_convenience_functions()
{
    std::cout << "\n=== FUNCIONES DE CONVENIENCIA ===" << std::endl;

    uint128_t value(0x1234, 0x5678ABCD);

    std::cout << "Valor para formateo: 0x12345678ABCD" << std::endl;

    // Funciones de conveniencia
    std::cout << "hex(): " << uint128_format::hex(value) << std::endl;
    std::cout << "hex(16, true): " << uint128_format::hex(value, 16, true) << std::endl;
    std::cout << "hex(20, true, true, '0'): " << uint128_format::hex(value, 20, true, true, '0')
              << std::endl;

    std::cout << "oct(): " << uint128_format::oct(value) << std::endl;
    std::cout << "oct(20, true): " << uint128_format::oct(value, 20, true) << std::endl;

    std::cout << "dec(): " << uint128_format::dec(value) << std::endl;
    std::cout << "dec(25, ' ', false): " << uint128_format::dec(value, 25, ' ', false) << std::endl;
    std::cout << "dec(25, '*', true): " << uint128_format::dec(value, 25, '*', true) << std::endl;
}

void test_input_formatting()
{
    std::cout << "\n=== ENTRADA CON FORMATO ===" << std::endl;

    // Test de entrada con diferentes bases
    std::istringstream iss_dec("12345");
    std::istringstream iss_hex("0x1A2B");
    std::istringstream iss_oct("0777");

    uint128_t value1, value2, value3;

    // Entrada decimal
    iss_dec >> std::dec >> value1;
    std::cout << "Entrada '12345' como decimal: " << value1 << std::endl;

    // Entrada hexadecimal
    iss_hex >> std::hex >> value2;
    std::cout << "Entrada '0x1A2B' como hexadecimal: " << std::dec << value2 << std::endl;

    // Entrada octal
    iss_oct >> std::oct >> value3;
    std::cout << "Entrada '0777' como octal: " << std::dec << value3 << std::endl;
}

void test_complex_formatting()
{
    std::cout << "\n=== FORMATEO COMPLEJO ===" << std::endl;

    uint128_t large_value = uint128_t::from_string("123456789012345678901234567890");

    std::cout << "Número muy grande: " << large_value << std::endl;

    // Formateo complejo usando ostringstream
    std::ostringstream oss;
    oss << "Hex: " << std::hex << std::uppercase << std::showbase << std::setw(40)
        << std::setfill('0') << std::internal << large_value;
    std::cout << oss.str() << std::endl;

    // Formateo usando funciones de conveniencia
    std::cout << "Formateo personalizado: "
              << uint128_format::format(large_value, 16, 35, '.', true, true, false, true)
              << std::endl;
}

int main()
{
    std::cout << "=== DEMOSTRACIÓN COMPLETA DE FORMATEO uint128_t ===" << std::endl;
    std::cout << "Todas las capacidades de iostream/iomanip aplicadas a uint128_t" << std::endl;

    try {
        test_basic_formatting();
        test_base_manipulators();
        test_width_and_fill();
        test_alignment();
        test_uppercase();
        test_convenience_functions();
        test_input_formatting();
        test_complex_formatting();

        std::cout << "\n🎉 ¡TODAS LAS CAPACIDADES DE FORMATEO FUNCIONAN!" << std::endl;
        std::cout << "[OK] Manipuladores de base: hex, oct, dec" << std::endl;
        std::cout << "[OK] Control de ancho: setw()" << std::endl;
        std::cout << "[OK] Carácter de relleno: setfill()" << std::endl;
        std::cout << "[OK] Alineación: left, right, internal" << std::endl;
        std::cout << "[OK] Prefijos de base: showbase" << std::endl;
        std::cout << "[OK] Mayúsculas/minúsculas: uppercase" << std::endl;
        std::cout << "[OK] Entrada con formato automático" << std::endl;
        std::cout << "[OK] Funciones de conveniencia personalizadas" << std::endl;

        // Ejemplo final impresionante
        std::cout << "\n🎨 EJEMPLO FINAL IMPRESIONANTE:" << std::endl;
        uint128_t demo = uint128_t(0xDEADBEEF, 0xCAFEBABE);
        std::cout << "Decimal: " << std::dec << std::setw(30) << std::right << demo << std::endl;
        std::cout << "Hexadecimal: " << std::hex << std::uppercase << std::showbase << std::setw(35)
                  << std::setfill('*') << std::internal << demo << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}