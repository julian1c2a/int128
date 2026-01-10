/**
 * ===========================================================================
 * EJEMPLO: Búsqueda de Primos de Mersenne
 * ===========================================================================
 *
 * Los números de Mersenne tienen la forma M_p = 2^p - 1, donde p es primo.
 * Algunos números de Mersenne son primos (primos de Mersenne).
 *
 * Este ejemplo:
 * - Genera números de Mersenne
 * - Implementa test de primalidad Lucas-Lehmer simplificado
 * - Muestra los primeros primos de Mersenne conocidos
 * - Demuestra el crecimiento exponencial de estos números
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <int128_simple.hpp>
#include <vector>

using namespace nstd;

using namespace uint128_literals;

// Test de primalidad simple (para p pequeño)
bool is_prime_simple(int n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Generar número de Mersenne: 2^p - 1
uint128_t mersenne(int p)
{
    return (uint128_t(1) << p) - 1;
}

// Test de Lucas-Lehmer simplificado para números pequeños
bool lucas_lehmer_test(int p, const uint128_t& mp)
{
    if (p == 2)
        return true; // M_2 = 3 es primo

    uint128_t s = 4_u128;

    // Ejecutar p-2 iteraciones: s = (s^2 - 2) mod mp
    for (int i = 0; i < p - 2; ++i) {
        s = (s * s - 2) % mp;
    }

    return s == 0;
}

// Verificar divisibilidad por factores pequeños
bool has_small_factors(const uint128_t& n)
{
    std::vector<uint64_t> small_primes = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

    for (uint64_t p : small_primes) {
        if (n % p == 0)
            return true;
    }
    return false;
}

void demo_mersenne_numbers()
{
    std::cout << "\n=== Números de Mersenne (M_p = 2^p - 1) ===\n\n";

    std::cout << std::left;
    std::cout << std::setw(6) << "p" << std::setw(30) << "M_p = 2^p - 1" << std::setw(12)
              << "Dígitos"
              << "¿Primo?\n";
    std::cout << std::string(70, '-') << "\n";

    // Exponentes primos conocidos donde M_p es primo (primeros 15)
    std::vector<int> known_mersenne_primes = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127};

    for (int p : known_mersenne_primes) {
        if (p > 127)
            continue; // Limitación de uint128_t

        if (!is_prime_simple(p))
            continue;

        uint128_t mp = mersenne(p);
        std::string str = mp.to_string();

        std::cout << std::setw(6) << p << std::setw(30)
                  << (str.length() > 25 ? str.substr(0, 22) + "..." : str) << std::setw(12)
                  << str.length();

        // Test de primalidad (solo para p <= 31 por eficiencia)
        if (p <= 31) {
            bool is_prime = lucas_lehmer_test(p, mp);
            std::cout << (is_prime ? "✓ SÍ" : "✗ NO");
        } else {
            std::cout << "✓ SÍ (conocido)";
        }

        std::cout << "\n";
    }
}

void demo_mersenne_growth()
{
    std::cout << "\n=== Crecimiento Exponencial ===\n\n";

    std::cout << "Los números de Mersenne crecen exponencialmente:\n\n";

    for (int p : {2, 4, 8, 16, 32, 64, 100, 127}) {
        uint128_t mp = mersenne(p);
        std::string str = mp.to_string();

        std::cout << "M_" << std::setw(3) << p << " tiene " << std::setw(3) << str.length()
                  << " dígitos";

        if (p <= 127) {
            std::cout << " = " << mp;
        }
        std::cout << "\n";
    }

    std::cout << "\nNota: M_127 es el mayor primo de Mersenne que cabe en uint128_t\n";
}

void demo_composite_mersenne()
{
    std::cout << "\n=== Números de Mersenne Compuestos ===\n\n";

    std::cout << "No todos los números de Mersenne son primos:\n\n";

    std::vector<int> composite_exponents = {4, 6, 8, 9, 10, 11, 12, 14, 15, 16};

    std::cout << std::setw(6) << "p" << std::setw(20) << "M_p"
              << "Tiene factores pequeños?\n";
    std::cout << std::string(50, '-') << "\n";

    for (int p : composite_exponents) {
        uint128_t mp = mersenne(p);
        bool has_factors = has_small_factors(mp);

        std::cout << std::setw(6) << p << std::setw(20) << mp << (has_factors ? "✓ SÍ" : "NO")
                  << "\n";
    }

    std::cout << "\nEjemplos de factorización:\n";
    std::cout << "  M_4  = 15 = 3 × 5\n";
    std::cout << "  M_6  = 63 = 3^2 × 7\n";
    std::cout << "  M_8  = 255 = 3 × 5 × 17\n";
    std::cout << "  M_11 = 2047 = 23 × 89\n";
}

void demo_perfect_numbers()
{
    std::cout << "\n=== Relación con Números Perfectos ===\n\n";

    std::cout << "Teorema de Euclides-Euler:\n";
    std::cout << "Si 2^p - 1 es primo (Mersenne), entonces 2^(p-1) × (2^p - 1) es perfecto.\n\n";

    std::cout << "Los primeros números perfectos:\n\n";

    std::vector<int> mersenne_primes = {2, 3, 5, 7, 13, 17, 19, 31};

    std::cout << std::setw(4) << "p" << std::setw(10) << "M_p"
              << "Número perfecto\n";
    std::cout << std::string(60, '-') << "\n";

    for (int p : mersenne_primes) {
        if (p > 31)
            continue; // Limitación para visualización

        uint128_t mp = mersenne(p);
        uint128_t perfect = (uint128_t(1) << (p - 1)) * mp;

        std::cout << std::setw(4) << p << std::setw(10) << mp.to_string() << perfect << "\n";
    }
}

void demo_properties()
{
    std::cout << "\n=== Propiedades de los Números de Mersenne ===\n\n";

    std::cout << "1. Forma: M_p = 2^p - 1, donde p es primo\n\n";

    std::cout << "2. En binario, son todos unos:\n";
    for (int p : {3, 5, 7}) {
        uint128_t mp = mersenne(p);
        std::cout << "   M_" << p << " = ";
        for (int i = p - 1; i >= 0; --i) {
            std::cout << "1";
        }
        std::cout << "_2 = " << mp << "\n";
    }

    std::cout << "\n3. Primos de Mersenne conocidos:\n";
    std::cout << "   - Solo 51 primos de Mersenne conocidos (hasta 2023)\n";
    std::cout << "   - El mayor conocido es M_82,589,933 (~25 millones de dígitos)\n";
    std::cout << "   - Proyecto GIMPS busca nuevos primos de Mersenne\n";

    std::cout << "\n4. Aplicaciones:\n";
    std::cout << "   - Números perfectos\n";
    std::cout << "   - Generadores de números pseudoaleatorios\n";
    std::cout << "   - Teoría de números\n";
    std::cout << "   - Criptografía\n";
}

void demo_search_simulation()
{
    std::cout << "\n=== Simulación de Búsqueda ===\n\n";

    std::cout << "Buscando primos de Mersenne hasta p = 31...\n\n";

    int found = 0;
    std::cout << "Primos encontrados:\n";

    for (int p = 2; p <= 31; ++p) {
        if (!is_prime_simple(p))
            continue;

        uint128_t mp = mersenne(p);

        // Test simplificado
        bool is_probably_prime = lucas_lehmer_test(p, mp);

        if (is_probably_prime) {
            found++;
            std::cout << "  ✓ M_" << std::setw(2) << p << " = " << mp << "\n";
        }
    }

    std::cout << "\nTotal encontrados: " << found << " primos de Mersenne\n";
}

int main()
{
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║   BÚSQUEDA DE PRIMOS DE MERSENNE                          ║
║   M_p = 2^p - 1 (donde p es primo)                        ║
║   Usando uint128_t                                        ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
)" << "\n";

    std::cout << "Los primos de Mersenne son números de la forma 2^p - 1\n";
    std::cout << "donde tanto p como 2^p - 1 son primos.\n";

    demo_mersenne_numbers();
    demo_mersenne_growth();
    demo_composite_mersenne();
    demo_perfect_numbers();
    demo_properties();
    demo_search_simulation();

    std::cout << "\n=== Para Saber Más ===\n\n";
    std::cout << "• GIMPS (Great Internet Mersenne Prime Search):\n";
    std::cout << "  https://www.mersenne.org/\n";
    std::cout << "• Lista de primos de Mersenne conocidos:\n";
    std::cout << "  https://www.mersenne.org/primes/\n";

    return 0;
}

