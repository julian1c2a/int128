#include <cassert>
#include <iostream>
#include <limits>
#include <optional>

// Incluir el archivo a probar
#include "include/int128_safe.hpp"

void test_safe_conversions_to_integral()
{
    std::cout << "🔄 Probando conversiones seguras a tipos integrales...\n";

    // Test conversión exitosa
    int128_t value1(42);
    auto result_int = int128_safe::safe_cast<int>(value1);
    assert(result_int.is_valid() && result_int.value == 42);
    std::cout << "  [OK] Conversión exitosa a int: " << result_int.value << "\n";

    // Test overflow a tipo pequeño - usar valor que definitivamente cause overflow
    int128_t large_value(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // Máximo valor
    auto result_overflow = int128_safe::safe_cast<int32_t>(large_value);
    assert(!result_overflow.is_valid() &&
           result_overflow.status == int128_safe::conversion_result::overflow);
    std::cout << "  [OK] Detección de overflow funciona\n";

    // Test valor negativo
    int128_t negative_value(-123);
    auto result_neg = int128_safe::safe_cast<int>(negative_value);
    assert(result_neg.is_valid() && result_neg.value == -123);
    std::cout << "  [OK] Conversión de valor negativo: " << result_neg.value << "\n";

    // Test underflow a tipo unsigned
    auto result_underflow = int128_safe::safe_cast<unsigned int>(negative_value);
    assert(!result_underflow.is_valid() &&
           result_underflow.status == int128_safe::conversion_result::underflow);
    std::cout << "  [OK] Detección de underflow funciona\n";
}

void test_safe_conversions_to_float()
{
    std::cout << "🔢 Probando conversiones seguras a tipos float...\n";

    // Test conversión exitosa
    int128_t value(12345);
    auto result = int128_safe::safe_cast_float<double>(value);
    assert(result.is_valid() && result.value == 12345.0);
    std::cout << "  [OK] Conversión a double: " << result.value << "\n";

    // Test valor negativo
    int128_t negative(-67890);
    auto result_neg = int128_safe::safe_cast_float<double>(negative);
    assert(result_neg.is_valid() && result_neg.value == -67890.0);
    std::cout << "  [OK] Conversión negativa a double: " << result_neg.value << "\n";

    // Test valor muy grande (debería funcionar con double)
    int128_t large = int128_t(1) << 100;
    auto result_large = int128_safe::safe_cast_float<double>(large);
    assert(result_large.is_valid());
    std::cout << "  [OK] Conversión de valor grande funciona\n";
}

void test_checked_conversions()
{
    std::cout << "[OK] Probando conversiones con excepciones...\n";

    // Test conversión exitosa
    int128_t value(100);
    try {
        int result = int128_safe::checked_cast<int>(value);
        assert(result == 100);
        std::cout << "  [OK] checked_cast exitoso: " << result << "\n";
    } catch (...) {
        assert(false); // No debería lanzar excepción
    }

    // Test excepción en overflow
    int128_t large_value(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // Máximo valor
    try {
        int result = int128_safe::checked_cast<int>(large_value);
        assert(false); // Debería lanzar excepción
    } catch (const std::overflow_error& e) {
        std::cout << "  [OK] Excepción de overflow capturada correctamente\n";
    } catch (...) {
        assert(false); // Tipo de excepción incorrecto
    }
}

void test_optional_conversions()
{
    std::cout << "❓ Probando conversiones opcionales...\n";

    // Test conversión exitosa
    int128_t value(200);
    auto result = int128_safe::try_cast<int>(value);
    assert(result.has_value() && result.value() == 200);
    std::cout << "  [OK] try_cast exitoso: " << result.value() << "\n";

    // Test conversión fallida
    int128_t large_value = int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // Máximo valor
    auto result_fail = int128_safe::try_cast<short>(large_value);
    assert(!result_fail.has_value());
    std::cout << "  [OK] try_cast falla correctamente\n";
}

void test_safe_arithmetic()
{
    std::cout << "➕ Probando operaciones aritméticas seguras...\n";

    // Test adición segura
    int128_t a(1000), b(2000);
    auto add_result = int128_safe::safe_add(a, b);
    assert(add_result.is_valid() && add_result.value == int128_t(3000));
    std::cout << "  [OK] Adición segura: " << add_result.value << "\n";

    // Test overflow en adición - usar valor muy grande
    int128_t max_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL); // Máximo valor
    auto overflow_result = int128_safe::safe_add(max_val, int128_t(1));
    assert(!overflow_result.is_valid() &&
           overflow_result.status == int128_safe::conversion_result::overflow);
    std::cout << "  [OK] Detección de overflow en adición\n";

    // Test sustracción segura
    auto sub_result = int128_safe::safe_sub(b, a);
    assert(sub_result.is_valid() && sub_result.value == int128_t(1000));
    std::cout << "  [OK] Sustracción segura: " << sub_result.value << "\n";

    // Test underflow en sustracción - usar valor mínimo
    int128_t min_val(0x8000000000000000ULL, 0x0000000000000000ULL); // Mínimo valor
    auto underflow_result = int128_safe::safe_sub(min_val, int128_t(1));
    assert(!underflow_result.is_valid() &&
           underflow_result.status == int128_safe::conversion_result::underflow);
    std::cout << "  [OK] Detección de underflow en sustracción\n";

    // Test multiplicación segura
    int128_t x(123), y(456);
    auto mul_result = int128_safe::safe_mul(x, y);
    assert(mul_result.is_valid() && mul_result.value == int128_t(56088));
    std::cout << "  [OK] Multiplicación segura: " << mul_result.value << "\n";

    // Test división segura
    auto div_result = int128_safe::safe_div(mul_result.value, x);
    assert(div_result.is_valid() && div_result.value == y);
    std::cout << "  [OK] División segura: " << div_result.value << "\n";

    // Test división por cero
    auto div_zero = int128_safe::safe_div(a, int128_t(0));
    assert(!div_zero.is_valid() &&
           div_zero.status == int128_safe::conversion_result::invalid_input);
    std::cout << "  [OK] Detección de división por cero\n";
}

void test_safe_shifts()
{
    std::cout << "↔️ Probando operaciones de desplazamiento seguras...\n";

    // Test shift izquierdo seguro
    int128_t value(1);
    auto shl_result = int128_safe::safe_shl(value, 10);
    assert(shl_result.is_valid() && shl_result.value == int128_t(1024));
    std::cout << "  [OK] Shift izquierdo seguro: " << shl_result.value << "\n";

    // Test shift derecho seguro
    int128_t large(1024);
    auto shr_result = int128_safe::safe_shr(large, 2);
    assert(shr_result.is_valid() && shr_result.value == int128_t(256));
    std::cout << "  [OK] Shift derecho seguro: " << shr_result.value << "\n";

    // Test shift inválido
    auto invalid_shift = int128_safe::safe_shl(value, 128);
    assert(!invalid_shift.is_valid() &&
           invalid_shift.status == int128_safe::conversion_result::invalid_input);
    std::cout << "  [OK] Detección de shift inválido\n";
}

void test_range_utilities()
{
    std::cout << "[INFO] Probando utilidades de rango...\n";

    // Test in_range
    int128_t value(50);
    assert(int128_safe::in_range(value, int128_t(10), int128_t(100)));
    assert(!int128_safe::in_range(value, int128_t(60), int128_t(100)));
    std::cout << "  [OK] in_range funciona correctamente\n";

    // Test clamp
    int128_t clamped = int128_safe::clamp(int128_t(150), int128_t(10), int128_t(100));
    assert(clamped == int128_t(100));
    clamped = int128_safe::clamp(int128_t(5), int128_t(10), int128_t(100));
    assert(clamped == int128_t(10));
    std::cout << "  [OK] clamp funciona correctamente\n";

    // Test saturating operations
    int128_t max_test_val(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
    int128_t sat_add = int128_safe::saturating_add(max_test_val, int128_t(1));
    assert(sat_add == max_test_val); // Should clamp to max value
    std::cout << "  [OK] saturating_add funciona\n";

    int128_t sat_sub = int128_safe::saturating_sub(
        int128_t(0x8000000000000000ULL, 0x0000000000000000ULL), int128_t(1));
    assert(sat_sub == int128_t(0x8000000000000000ULL, 0x0000000000000000ULL));
    std::cout << "  [OK] saturating_sub funciona\n";
}

void test_safe_abs()
{
    std::cout << "📐 Probando valor absoluto seguro...\n";

    // Test abs normal
    int128_t negative(-42);
    auto abs_result = int128_safe::safe_abs(negative);
    assert(abs_result.is_valid() && abs_result.value == int128_t(42));
    std::cout << "  [OK] safe_abs normal: " << abs_result.value << "\n";

    // Test abs de valor positivo
    int128_t positive(42);
    auto abs_pos = int128_safe::safe_abs(positive);
    assert(abs_pos.is_valid() && abs_pos.value == int128_t(42));
    std::cout << "  [OK] safe_abs de positivo: " << abs_pos.value << "\n";

    // Test overflow en abs(min)
    auto abs_min = int128_safe::safe_abs(int128_t(0x8000000000000000ULL, 0x0000000000000000ULL));
    assert(!abs_min.is_valid() && abs_min.status == int128_safe::conversion_result::overflow);
    std::cout << "  [OK] Detección de overflow en abs(min)\n";

    // Test saturating_abs
    int128_t sat_abs =
        int128_safe::saturating_abs(int128_t(0x8000000000000000ULL, 0x0000000000000000ULL));
    assert(sat_abs == int128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    std::cout << "  [OK] saturating_abs funciona\n";
}

void test_construction_from_other_types()
{
    std::cout << "🏗️ Probando construcción segura desde otros tipos...\n";

    // Test desde int
    auto from_int = int128_safe::safe_make_int128(12345);
    assert(from_int.is_valid() && from_int.value == int128_t(12345));
    std::cout << "  [OK] Construcción desde int: " << from_int.value << "\n";

    // Test desde float
    auto from_float = int128_safe::safe_make_int128_float(123.45);
    assert(from_float.is_valid() && from_float.value == int128_t(123));
    std::cout << "  [OK] Construcción desde float: " << from_float.value << "\n";

    // Test desde float negativo
    auto from_neg_float = int128_safe::safe_make_int128_float(-456.78);
    assert(from_neg_float.is_valid() && from_neg_float.value == int128_t(-456));
    std::cout << "  [OK] Construcción desde float negativo: " << from_neg_float.value << "\n";
}

int main()
{
    std::cout << "🚀 Iniciando tests de int128_safe.hpp\n";
    std::cout << "=====================================\n\n";

    try {
        test_safe_conversions_to_integral();
        std::cout << "\n";

        test_safe_conversions_to_float();
        std::cout << "\n";

        test_checked_conversions();
        std::cout << "\n";

        test_optional_conversions();
        std::cout << "\n";

        test_safe_arithmetic();
        std::cout << "\n";

        test_safe_shifts();
        std::cout << "\n";

        test_range_utilities();
        std::cout << "\n";

        test_safe_abs();
        std::cout << "\n";

        test_construction_from_other_types();
        std::cout << "\n";

        std::cout << "🎉 ¡TODOS LOS TESTS DE SAFE PASARON!\n";
        std::cout << "=====================================\n";
        std::cout << "[OK] Conversiones seguras funcionando\n";
        std::cout << "[OK] Detección de overflow/underflow funcionando\n";
        std::cout << "[OK] Operaciones aritméticas seguras funcionando\n";
        std::cout << "[OK] Operaciones de desplazamiento seguras funcionando\n";
        std::cout << "[OK] Utilidades de rango funcionando\n";
        std::cout << "[OK] Valor absoluto seguro funcionando\n";
        std::cout << "[OK] Construcción segura funcionando\n";

        return 0;

    } catch (const std::exception& e) {
        std::cout << "[FAIL] Error en tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "[FAIL] Error desconocido en tests" << std::endl;
        return 1;
    }
}