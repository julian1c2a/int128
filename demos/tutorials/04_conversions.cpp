/**
 * Tutorial 04: Conversiones
 *
 * Este tutorial muestra cómo convertir entre uint128_t/int128_t
 * y otros tipos de datos.
 *
 * Aprenderás:
 * - Conversión desde/hacia tipos nativos (int, uint64_t, etc.)
 * - Conversión explícita vs implícita
 * - Narrowing (pérdida de datos)
 * - Casts seguros
 * - Promoción de tipos
 */

#include <cstdint>
#include <int128_simple.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <limits>
#include <uint128/uint128_iostreams.hpp>

using namespace nstd;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 04: Conversiones\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. CONVERSIÓN DESDE TIPOS NATIVOS
    // ============================================================
    std::cout << "--- 1. Conversión desde tipos nativos ---\n";

    // Conversión implícita (automática)
    uint128_t from_int{42};
    uint128_t from_uint = 100u;
    uint128_t from_long = 500L;
    uint128_t from_uint64 = UINT64_MAX;

    std::cout << "Desde int:      " << from_int << "\n";
    std::cout << "Desde unsigned: " << from_uint << "\n";
    std::cout << "Desde long:     " << from_long << "\n";
    std::cout << "Desde uint64_t: " << from_uint64 << "\n\n";

    // ============================================================
    // 2. CONVERSIÓN A TIPOS NATIVOS
    // ============================================================
    std::cout << "--- 2. Conversión a tipos nativos ---\n";

    uint128_t big{12345};

    // Conversión explícita con static_cast
    int to_int = static_cast<int>(big);
    uint32_t to_uint32 = static_cast<uint32_t>(big);
    uint64_t to_uint64 = static_cast<uint64_t>(big);

    std::cout << "uint128_t: " << big << "\n";
    std::cout << "→ int:      " << to_int << "\n";
    std::cout << "→ uint32_t: " << to_uint32 << "\n";
    std::cout << "→ uint64_t: " << to_uint64 << "\n\n";

    // ============================================================
    // 3. NARROWING (PÉRDIDA DE DATOS)
    // ============================================================
    std::cout << "--- 3. ⚠️  Narrowing (pérdida de datos) ---\n";

    // Número que NO cabe en uint64_t
    uint128_t very_big = uint128_t(1) << 100; // 2^100

    std::cout << "Valor original (uint128_t): " << very_big << "\n";

    // Conversión a uint64_t trunca los bits altos
    uint64_t truncated = static_cast<uint64_t>(very_big);

    std::cout << "Después de conversión a uint64_t: " << truncated << "\n";
    std::cout << "⚠️  Se perdieron los bits superiores!\n\n";

    // ============================================================
    // 4. VERIFICAR SI CABE EN TIPO NATIVO
    // ============================================================
    std::cout << "--- 4. Verificación antes de convertir ---\n";

    uint128_t value1{1000};
    uint128_t value2 = uint128_t(1) << 70;

    // Verificar si cabe en uint64_t
    auto fits_in_u64 = [](const uint128_t& val) {
        return val <= std::numeric_limits<uint64_t>::max();
    };

    std::cout << "value1 = " << value1 << "\n";
    std::cout << "¿Cabe en uint64_t? " << (fits_in_u64(value1) ? "Sí" : "No") << "\n\n";

    std::cout << "value2 = " << value2 << "\n";
    std::cout << "¿Cabe en uint64_t? " << (fits_in_u64(value2) ? "Sí" : "No") << "\n\n";

    // ============================================================
    // 5. CONVERSIÓN ENTRE UINT128 Y INT128
    // ============================================================
    std::cout << "--- 5. Conversión entre uint128_t y int128_t ---\n";

    // NOTA: uint128_t no soporta conversión directa a/desde float/double
    // debido a limitaciones del tamaño del tipo. Para tales conversiones,
    // se recomienda usar conversión manual dividiendo en partes.

    uint128_t unsigned_val{1000};
    int128_t signed_val = static_cast<int128_t>(unsigned_val);

    std::cout << "uint128_t: " << unsigned_val << "\n";
    std::cout << "int128_t:  " << signed_val << "\n\n";

    // ⚠️ Cuidado con valores muy grandes!
    uint128_t too_big_for_signed = uint128_t(1) << 127;
    int128_t wrong_sign = static_cast<int128_t>(too_big_for_signed);

    std::cout << "⚠️  uint128_t muy grande: " << too_big_for_signed << "\n";
    std::cout << "Convertido a int128_t: " << wrong_sign << "\n";
    std::cout << "¡Se interpreta como negativo!\n\n";

    // ============================================================
    // 6. CONVERSIÓN DESDE DOUBLE/FLOAT
    // ============================================================
    std::cout << "--- 6. Conversión desde punto flotante ---\n";
    std::cout << "NOTA: uint128_t no soporta conversión directa desde double/float\n";
    std::cout << "Se requiere conversión manual para tipos de punto flotante\n\n";

    // double dbl = 123.456;
    // float flt = 789.012f;
    // uint128_t from_double = static_cast<uint128_t>(dbl);  // NO SOPORTADO
    // uint128_t from_float = static_cast<uint128_t>(flt);   // NO SOPORTADO

    // ============================================================
    // 7. CONVERSIÓN A DOUBLE/FLOAT
    // ============================================================
    std::cout << "--- 7. Conversión a punto flotante ---\n";
    std::cout << "NOTA: uint128_t no soporta conversión directa a double/float\n";
    std::cout << "Se requiere conversión manual para tipos de punto flotante\n\n";

    // uint128_t integer{123456789};
    // double to_double = static_cast<double>(integer);  // NO SOPORTADO
    // float to_float = static_cast<float>(integer);     // NO SOPORTADO

    // ============================================================
    // 8. CONVERSIÓN DESDE BOOL
    // ============================================================
    std::cout << "--- 8. Conversión desde bool ---\n";

    bool true_val = true;
    bool false_val = false;

    uint128_t from_true = true_val;
    uint128_t from_false = false_val;

    std::cout << "bool true  → uint128_t " << from_true << "\n";
    std::cout << "bool false → uint128_t " << from_false << "\n\n";

    // ============================================================
    // 9. CONVERSIÓN A BOOL
    // ============================================================
    std::cout << "--- 9. Conversión a bool ---\n";

    uint128_t zero{0};
    uint128_t non_zero{42};

    bool is_zero = static_cast<bool>(zero);
    bool is_non_zero = static_cast<bool>(non_zero);

    std::cout << "uint128_t(0)  → bool: " << std::boolalpha << is_zero << "\n";
    std::cout << "uint128_t(42) → bool: " << is_non_zero << "\n\n";

    // ============================================================
    // 10. PROMOCIÓN AUTOMÁTICA EN OPERACIONES
    // ============================================================
    std::cout << "--- 10. Promoción automática ---\n";

    uint128_t a{1000};
    int b = 500;

    // int se promociona automáticamente a uint128_t
    auto sum = a + b; // b se convierte a uint128_t

    std::cout << "uint128_t(1000) + int(500) = " << sum << "\n";
    std::cout << "Tipo resultado: uint128_t\n\n";

    // ============================================================
    // 11. EXTRACCIÓN DE HIGH/LOW WORDS
    // ============================================================
    std::cout << "--- 11. Extraer palabras high y low ---\n";

    uint128_t full = (uint128_t(0x1234567890ABCDEF) << 64) | 0xFEDCBA0987654321;

    // Extraer low 64 bits
    uint64_t low = static_cast<uint64_t>(full);

    // Extraer high 64 bits
    uint64_t high = static_cast<uint64_t>(full >> 64);

    std::cout << std::hex << std::uppercase;
    std::cout << "uint128_t completo: 0x" << full << "\n";
    std::cout << "High 64 bits: 0x" << high << "\n";
    std::cout << "Low 64 bits:  0x" << low << "\n";
    std::cout << std::dec << std::nouppercase << "\n";

    // ============================================================
    // 12. CONSTRUCCIÓN DESDE HIGH/LOW
    // ============================================================
    std::cout << "--- 12. Construir desde high y low ---\n";

    uint64_t high_word = 0x1234567890ABCDEF;
    uint64_t low_word = 0xFEDCBA0987654321;

    uint128_t reconstructed = (uint128_t(high_word) << 64) | low_word;

    std::cout << std::hex << std::uppercase;
    std::cout << "High: 0x" << high_word << "\n";
    std::cout << "Low:  0x" << low_word << "\n";
    std::cout << "Reconstruido: 0x" << reconstructed << "\n";
    std::cout << std::dec << std::nouppercase << "\n\n";

    // ============================================================
    // 13. CONVERSIÓN SEGURA CON VERIFICACIÓN
    // ============================================================
    std::cout << "--- 13. Conversión segura ---\n";

    auto safe_to_int = [](const uint128_t& val) -> std::optional<int> {
        if (val > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }
        return static_cast<int>(val);
    };

    uint128_t safe_value{100};
    uint128_t unsafe_value = uint128_t(1) << 100;

    auto result1 = safe_to_int(safe_value);
    auto result2 = safe_to_int(unsafe_value);

    std::cout << "Convertir " << safe_value << ": ";
    if (result1) {
        std::cout << "Éxito → " << *result1 << "\n";
    } else {
        std::cout << "Fallo (valor muy grande)\n";
    }

    std::cout << "Convertir " << unsafe_value << ": ";
    if (result2) {
        std::cout << "Éxito → " << *result2 << "\n";
    } else {
        std::cout << "Fallo (valor muy grande)\n";
    }
    std::cout << "\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 04 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 05_string_conversion.cpp\n";

    return 0;
}

