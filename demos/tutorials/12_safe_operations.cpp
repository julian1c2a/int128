/**
 * Tutorial 12: Operaciones Seguras (Safe Operations)
 *
 * Aprenderás:
 * - Detección de overflow
 * - Operaciones seguras que retornan std::optional
 * - Validación antes de operar
 * - Manejo de errores
 */

#include <int128.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <limits>
#include <optional>

using namespace uint128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 12: Operaciones Seguras\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. PROBLEMA: OVERFLOW
    // ============================================================
    std::cout << "--- 1. ⚠️  Overflow silencioso ---\n";

    uint128_t max = std::numeric_limits<uint128_t>::max();

    std::cout << "Máximo uint128_t: " << max << "\n";

    // Esto causa overflow (wrap around)
    auto overflow_result = max + 1;

    std::cout << "max + 1 = " << overflow_result << " (¡overflow!)\n\n";

    // ============================================================
    // 2. SUMA SEGURA
    // ============================================================
    std::cout << "--- 2. Suma segura (detección de overflow) ---\n";

    auto safe_add = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        uint128_t result = a + b;
        if (result < a) { // Overflow ocurrió
            return std::nullopt;
        }
        return result;
    };

    auto result1 = safe_add(1000_u128, 2000_u128);
    auto result2 = safe_add(max, 100_u128);

    if (result1) {
        std::cout << "1000 + 2000 = " << *result1 << " ✅\n";
    }

    if (!result2) {
        std::cout << "max + 100 = OVERFLOW ⚠️\n";
    }
    std::cout << "\n";

    // ============================================================
    // 3. MULTIPLICACIÓN SEGURA
    // ============================================================
    std::cout << "--- 3. Multiplicación segura ---\n";

    auto safe_mul = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        if (a == 0 || b == 0)
            return uint128_t(0);
        uint128_t result = a * b;
        if (result / a != b) { // Overflow check
            return std::nullopt;
        }
        return result;
    };

    auto mul1 = safe_mul(100_u128, 200_u128);
    auto mul2 = safe_mul(max, 2_u128);

    if (mul1) {
        std::cout << "100 × 200 = " << *mul1 << " ✅\n";
    }

    if (!mul2) {
        std::cout << "max × 2 = OVERFLOW ⚠️\n";
    }
    std::cout << "\n";

    // ============================================================
    // 4. DIVISIÓN SEGURA (DIVISIÓN POR CERO)
    // ============================================================
    std::cout << "--- 4. División segura ---\n";

    auto safe_div = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        if (b == 0) {
            return std::nullopt; // División por cero
        }
        return a / b;
    };

    auto div1 = safe_div(1000_u128, 10_u128);
    auto div2 = safe_div(1000_u128, 0_u128);

    if (div1) {
        std::cout << "1000 ÷ 10 = " << *div1 << " ✅\n";
    }

    if (!div2) {
        std::cout << "1000 ÷ 0 = ERROR (división por cero) ⚠️\n";
    }
    std::cout << "\n";

    // ============================================================
    // 5. RESTA SEGURA (UNSIGNED UNDERFLOW)
    // ============================================================
    std::cout << "--- 5. Resta segura (unsigned) ---\n";

    auto safe_sub = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        if (b > a) { // Resultaría negativo
            return std::nullopt;
        }
        return a - b;
    };

    auto sub1 = safe_sub(1000_u128, 500_u128);
    auto sub2 = safe_sub(100_u128, 200_u128);

    if (sub1) {
        std::cout << "1000 - 500 = " << *sub1 << " ✅\n";
    }

    if (!sub2) {
        std::cout << "100 - 200 = UNDERFLOW ⚠️\n";
    }
    std::cout << "\n";

    // ============================================================
    // 6. CONVERSIÓN SEGURA A TIPOS MENORES
    // ============================================================
    std::cout << "--- 6. Conversión segura a uint64_t ---\n";

    auto safe_to_u64 = [](uint128_t val) -> std::optional<uint64_t> {
        if (val > std::numeric_limits<uint64_t>::max()) {
            return std::nullopt;
        }
        return static_cast<uint64_t>(val);
    };

    auto conv1 = safe_to_u64(1000_u128);
    auto conv2 = safe_to_u64(uint128_t(1) << 100);

    if (conv1) {
        std::cout << "Conversión de 1000: " << *conv1 << " ✅\n";
    }

    if (!conv2) {
        std::cout << "Conversión de 2^100: OVERFLOW ⚠️\n";
    }
    std::cout << "\n";

    // ============================================================
    // 7. PATTERN: CHECKED OPERATIONS
    // ============================================================
    std::cout << "--- 7. 📘 Pattern: Operaciones verificadas ---\n";
    std::cout << "template<typename T>\n";
    std::cout << "std::optional<T> safe_op(T a, T b, Op op) {\n";
    std::cout << "    if (!is_safe(a, b, op)) return std::nullopt;\n";
    std::cout << "    return op(a, b);\n";
    std::cout << "}\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 12 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 13_udl_literals.cpp\n";

    return 0;
}
