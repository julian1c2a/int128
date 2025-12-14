/*
 * Demostración de que int128_t y uint128_t usan exactamente los mismos datos
 * Solo cambia la interpretación (complemento a 2 vs unsigned)
 */

#include "../include/int128_t.hpp"
#include "../include/uint128_t.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

void show_bits(uint64_t high, uint64_t low, const std::string& description)
{
    std::cout << description << std::endl;
    std::cout << "  High: 0x" << std::hex << std::setfill('0') << std::setw(16) << high << " ("
              << std::dec << high << ")" << std::endl;
    std::cout << "  Low:  0x" << std::hex << std::setfill('0') << std::setw(16) << low << " ("
              << std::dec << low << ")" << std::endl;
    std::cout << "  MSB of High: "
              << ((high & 0x8000000000000000ULL) ? "1 (negative)" : "0 (positive)") << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "🔍 DEMOSTRACIÓN: MISMOS DATOS, DIFERENTE INTERPRETACIÓN" << std::endl;
    std::cout << "=====================================================" << std::endl;

    // Caso 1: Número positivo pequeño
    std::cout << "\n📊 CASO 1: Número positivo pequeño (42)" << std::endl;
    uint128_t u_pos(42);
    int128_t i_pos(42);

    std::cout << "uint128_t(42): " << u_pos << std::endl;
    std::cout << "int128_t(42):  " << i_pos << std::endl;

    show_bits(u_pos.high(), u_pos.low(), "Datos internos:");

    assert(u_pos.high() == i_pos.high());
    assert(u_pos.low() == i_pos.low());
    std::cout << "✅ Datos internos idénticos" << std::endl;

    // Caso 2: Número negativo
    std::cout << "\n📊 CASO 2: Número negativo (-42)" << std::endl;
    int128_t i_neg(-42);

    std::cout << "int128_t(-42): " << i_neg << std::endl;

    // Convertir a uint128_t para ver la interpretación sin signo
    uint128_t u_from_neg = i_neg.to_uint128();
    std::cout << "Como uint128_t: " << u_from_neg << std::endl;

    show_bits(i_neg.high(), i_neg.low(), "Datos internos (complemento a 2):");

    // Caso 3: Valor límite - máximo positivo
    std::cout << "\n📊 CASO 3: Máximo int128_t" << std::endl;
    int128_t i_max = int128_t_MAX;
    uint128_t u_max_equivalent(i_max.high(), i_max.low());

    std::cout << "int128_t_MAX:     " << i_max << std::endl;
    std::cout << "Como uint128_t:   " << u_max_equivalent << std::endl;

    show_bits(i_max.high(), i_max.low(), "Datos internos (MSB=0, positivo):");

    // Caso 4: Valor límite - mínimo negativo
    std::cout << "\n📊 CASO 4: Mínimo int128_t" << std::endl;
    int128_t i_min = int128_t_MIN;
    uint128_t u_min_equivalent(i_min.high(), i_min.low());

    std::cout << "int128_t_MIN:     " << i_min << std::endl;
    std::cout << "Como uint128_t:   " << u_min_equivalent << std::endl;

    show_bits(i_min.high(), i_min.low(), "Datos internos (MSB=1, negativo):");

    // Caso 5: Demostrar conversión bidireccional
    std::cout << "\n📊 CASO 5: Conversión bidireccional" << std::endl;
    uint128_t original_uint(0x8000000000000000ULL, 0x123456789ABCDEFULL);
    int128_t converted_int(original_uint);
    uint128_t back_to_uint = converted_int.to_uint128();

    std::cout << "uint128_t original: " << original_uint << std::endl;
    std::cout << "int128_t convertido: " << converted_int << std::endl;
    std::cout << "De vuelta a uint128_t: " << back_to_uint << std::endl;

    show_bits(original_uint.high(), original_uint.low(), "Datos originales:");
    show_bits(converted_int.high(), converted_int.low(), "Después de conversión:");
    show_bits(back_to_uint.high(), back_to_uint.low(), "De vuelta:");

    assert(original_uint.high() == back_to_uint.high());
    assert(original_uint.low() == back_to_uint.low());
    std::cout << "✅ Conversión perfectamente reversible" << std::endl;

    // Análisis final
    std::cout << "\n🔧 ANÁLISIS FINAL:" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "✅ Misma estructura interna: data[0] (low) + data[1] (high)" << std::endl;
    std::cout << "✅ Mismo tamaño: " << sizeof(uint128_t) << " bytes = " << sizeof(int128_t)
              << " bytes" << std::endl;
    std::cout << "✅ MSB de data[1] como bit de signo en int128_t" << std::endl;
    std::cout << "✅ Complemento a 2 para números negativos" << std::endl;
    std::cout << "✅ Conversiones bidireccionales perfectas" << std::endl;

    std::cout << "\n📈 RANGOS:" << std::endl;
    std::cout << "uint128_t: 0 a 2^128-1" << std::endl;
    std::cout << "int128_t:  -2^127 a 2^127-1" << std::endl;
    std::cout << "Ambos cubren el mismo espacio de 128 bits" << std::endl;

    return 0;
}