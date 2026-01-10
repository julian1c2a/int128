/**
 * Tutorial 01: Operaciones Básicas
 *
 * Este tutorial muestra las operaciones aritméticas fundamentales
 * con uint128_t e int128_t.
 *
 * Aprenderás:
 * - Crear números de 128 bits
 * - Suma, resta, multiplicación, división
 * - Módulo (resto de división)
 * - Diferencias entre signed y unsigned
 */

#include <int128_simple.hpp> // Header simplificado para demos
#include <iomanip>
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 01: Operaciones Basicas\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. CREACION DE NUMEROS
    // ============================================================
    std::cout << "--- 1. Creacion de numeros ---\n";

    // Unsigned (sin signo, solo positivos)
    // NOTA: Los constructores son EXPLICITOS - usar {} o ()
    uint128_t a{1000};
    uint128_t b{2000};

    // Signed (con signo, puede ser negativo)
    int128_t x{500};
    int128_t y{-300}; // Puede ser negativo!

    std::cout << "uint128_t a = " << a << "\n";
    std::cout << "uint128_t b = " << b << "\n";
    std::cout << "int128_t  x = " << x << "\n";
    std::cout << "int128_t  y = " << y << "\n\n";

    // ============================================================
    // 2. SUMA
    // ============================================================
    std::cout << "--- 2. Suma ---\n";

    auto sum_unsigned = a + b;
    auto sum_signed = x + y;

    std::cout << a << " + " << b << " = " << sum_unsigned << "\n";
    std::cout << x << " + " << y << " = " << sum_signed << "\n\n";

    // ============================================================
    // 3. RESTA
    // ============================================================
    std::cout << "--- 3. Resta ---\n";

    auto diff_unsigned = b - a;
    auto diff_signed = x - y; // 500 - (-300) = 800

    std::cout << b << " - " << a << " = " << diff_unsigned << "\n";
    std::cout << x << " - " << y << " = " << diff_signed << "\n\n";

    // ============================================================
    // 4. MULTIPLICACION
    // ============================================================
    std::cout << "--- 4. Multiplicacion ---\n";

    uint128_t factor1{1000000};
    uint128_t factor2{2000000};
    auto product = factor1 * factor2;

    std::cout << factor1 << " x " << factor2 << " = " << product << "\n";

    // Con signed
    int128_t neg{-100};
    int128_t pos{50};
    auto product_signed = neg * pos;

    std::cout << neg << " x " << pos << " = " << product_signed << "\n\n";

    // ============================================================
    // 5. DIVISION
    // ============================================================
    std::cout << "--- 5. Division ---\n";

    uint128_t dividend{10000};
    uint128_t divisor{25};
    auto quotient = dividend / divisor;

    std::cout << dividend << " / " << divisor << " = " << quotient << "\n";

    // Division con truncamiento (no redondeo)
    uint128_t div1{100};
    uint128_t div2{3};
    auto result = div1 / div2; // 33, no 33.333...

    std::cout << div1 << " / " << div2 << " = " << result << " (truncado)\n\n";

    // ============================================================
    // 6. MÓDULO (RESTO)
    // ============================================================
    std::cout << "--- 6. Módulo (resto de división) ---\n";

    auto remainder = div1 % div2; // 100 % 3 = 1

    std::cout << div1 << " % " << div2 << " = " << remainder << "\n";

    // Verificación: dividend = quotient * divisor + remainder
    auto verification = result * div2 + remainder;
    std::cout << "Verificación: " << result << " × " << div2 << " + " << remainder << " = "
              << verification << "\n\n";

    // ============================================================
    // 7. OPERACIONES COMPUESTAS
    // ============================================================
    std::cout << "--- 7. Operaciones compuestas ---\n";

    uint128_t n{100};

    n += 50; // n = n + 50
    std::cout << "Después de += 50: " << n << "\n";

    n -= 20; // n = n - 20
    std::cout << "Después de -= 20: " << n << "\n";

    n *= 2; // n = n * 2
    std::cout << "Después de *= 2:  " << n << "\n";

    n /= 5; // n = n / 5
    std::cout << "Después de /= 5:  " << n << "\n\n";

    // ============================================================
    // 8. INCREMENTO Y DECREMENTO
    // ============================================================
    std::cout << "--- 8. Incremento y decremento ---\n";

    uint128_t counter{10};

    std::cout << "Valor inicial: " << counter << "\n";
    std::cout << "Pre-incremento (++counter): " << ++counter << "\n";
    std::cout << "Post-incremento (counter++): " << counter++ << "\n";
    std::cout << "Después del post-incremento: " << counter << "\n";

    std::cout << "Pre-decremento (--counter): " << --counter << "\n";
    std::cout << "Post-decremento (counter--): " << counter-- << "\n";
    std::cout << "Después del post-decremento: " << counter << "\n\n";

    // ============================================================
    // 9. NÚMEROS GRANDES
    // ============================================================
    std::cout << "--- 9. Operaciones con números GRANDES ---\n";

    // Factorial de 20: 2,432,902,008,176,640,000
    uint128_t factorial_20{1};
    for (int i = 2; i <= 20; ++i)
    {
        factorial_20 *= i;
    }

    std::cout << "20! = " << factorial_20 << "\n";

    // Potencias de 2
    uint128_t power{1};
    power = power << 64; // 2^64
    std::cout << "2^64 = " << power << "\n\n";

    // ============================================================
    // 10. PITFALL: UNSIGNED UNDERFLOW
    // ============================================================
    std::cout << "--- 10. [PITFALL] Unsigned underflow ---\n";

    uint128_t small{5};
    uint128_t large{10};

    // Correcto: large - small
    std::cout << "Correcto: " << large << " - " << small << " = " << (large - small) << "\n";

    // ¡CUIDADO! Unsigned: small - large da un número MUY GRANDE
    auto underflow = small - large;
    std::cout << "⚠️  Underflow: " << small << " - " << large << " = " << underflow
              << " (¡WRAP AROUND!)\n";
    std::cout << "Usa int128_t si necesitas números negativos.\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 01 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 02_bitwise_operations.cpp\n";

    return 0;
}

/* SALIDA ESPERADA:
========================================
Tutorial 01: Operaciones Básicas
========================================

--- 1. Creación de números ---
uint128_t a = 1000
uint128_t b = 2000
int128_t  x = 500
int128_t  y = -300

--- 2. Suma ---
1000 + 2000 = 3000
500 + -300 = 200

--- 3. Resta ---
2000 - 1000 = 1000
500 - -300 = 800

--- 4. Multiplicación ---
1000000 × 2000000 = 2000000000000
-100 × 50 = -5000

--- 5. División ---
10000 ÷ 25 = 400
100 ÷ 3 = 33 (truncado)

--- 6. Módulo (resto de división) ---
100 % 3 = 1
Verificación: 33 × 3 + 1 = 100

--- 7. Operaciones compuestas ---
Después de += 50: 150
Después de -= 20: 130
Después de *= 2:  260
Después de /= 5:  52

--- 8. Incremento y decremento ---
Valor inicial: 10
Pre-incremento (++counter): 11
Post-incremento (counter++): 11
Después del post-incremento: 12
Pre-decremento (--counter): 11
Post-decremento (counter--): 11
Después del post-decremento: 10

--- 9. Operaciones con números GRANDES ---
20! = 2432902008176640000
2^64 = 18446744073709551616

--- 10. ⚠️  PITFALL: Unsigned underflow ---
Correcto: 10 - 5 = 5
⚠️  Underflow: 5 - 10 = 340282366920938463463374607431768211451 (¡WRAP AROUND!)
Usa int128_t si necesitas números negativos.

========================================
✅ Tutorial 01 completado
========================================

Próximo tutorial: 02_bitwise_operations.cpp
*/
