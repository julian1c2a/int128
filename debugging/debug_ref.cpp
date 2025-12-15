#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>

// Función de referencia exacta del test original
uint64_t reference_fullmult_times_uint64(uint64_t high, uint64_t low, uint64_t multiplier)
{
    const uint64_t mask32 = 0xFFFFFFFFULL;

    // Descomponer en partes de 32 bits
    uint64_t a0 = low & mask32;
    uint64_t a1 = low >> 32;
    uint64_t a2 = high & mask32;
    uint64_t a3 = high >> 32;

    uint64_t b0 = multiplier & mask32;
    uint64_t b1 = multiplier >> 32;

    // Calcular todos los productos parciales
    uint64_t p00 = a0 * b0;
    uint64_t p01 = a0 * b1;
    uint64_t p10 = a1 * b0;
    uint64_t p11 = a1 * b1;
    uint64_t p20 = a2 * b0;
    uint64_t p21 = a2 * b1;
    uint64_t p30 = a3 * b0;
    uint64_t p31 = a3 * b1;

    std::cout << "\n=== PRODUCTOS PARCIALES ===" << std::endl;
    std::cout << "a0=0x" << std::hex << a0 << " a1=0x" << a1 << " a2=0x" << a2 << " a3=0x" << a3
              << std::endl;
    std::cout << "b0=0x" << std::hex << b0 << " b1=0x" << b1 << std::endl;
    std::cout << "p00=0x" << std::hex << p00 << " p01=0x" << p01 << " p10=0x" << p10 << " p11=0x"
              << p11 << std::endl;
    std::cout << "p20=0x" << std::hex << p20 << " p21=0x" << p21 << " p30=0x" << p30 << " p31=0x"
              << p31 << std::endl;

    // Construir el resultado de 192 bits en partes de 64 bits
    uint64_t low64 = p00;

    uint64_t mid64 = (p00 >> 32) + (p01 & mask32) + (p10 & mask32);
    mid64 += (low64 >> 32);

    uint64_t high64 = p11 + (p01 >> 32) + (p10 >> 32) + p20;
    high64 += (mid64 >> 32);

    uint64_t highest64 = p21 + p30 + (p20 >> 32);
    highest64 += (high64 >> 32);

    uint64_t overflow = p31 + (p21 >> 32) + (p30 >> 32);
    overflow += (highest64 >> 32);

    std::cout << "\n=== CONSTRUCCIÓN DE RESULTADO ===" << std::endl;
    std::cout << "low64=0x" << std::hex << low64 << std::endl;
    std::cout << "mid64=0x" << std::hex << mid64 << std::endl;
    std::cout << "high64=0x" << std::hex << high64 << std::endl;
    std::cout << "highest64=0x" << std::hex << highest64 << std::endl;
    std::cout << "overflow=0x" << std::hex << overflow << " ← RESULTADO FINAL" << std::endl;

    return overflow;
}

int main()
{
    std::cout << "🔍 REFERENCIA PARA CASOS BÁSICOS" << std::endl;

    // Caso específico: (2^128 - 1) * 2
    uint64_t high = UINT64_MAX;
    uint64_t low = UINT64_MAX;
    uint64_t multiplier = 2;

    std::cout << "\n=== DATOS ===" << std::endl;
    std::cout << "high = 0x" << std::hex << high << std::endl;
    std::cout << "low  = 0x" << std::hex << low << std::endl;
    std::cout << "mult = " << std::dec << multiplier << std::endl;

    uint64_t result = reference_fullmult_times_uint64(high, low, multiplier);

    std::cout << "\n=== ANÁLISIS ===" << std::endl;
    std::cout << "Resultado final: 0x" << std::hex << result << " = " << std::dec << result
              << std::endl;
    std::cout << "¿Esperado 1?: " << (result == 1 ? "SÍ ✓" : "NO ✗") << std::endl;

    return 0;
}