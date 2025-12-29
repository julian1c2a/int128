/**
 * ============================================================================
 * SHOWCASE: SCIENTIFIC COMPUTING - Cálculos Científicos de Alta Precisión
 * ============================================================================
 *
 * Este showcase demuestra el uso de uint128_t/int128_t en computación científica:
 * - Cálculo de constantes matemáticas con alta precisión
 * - Combinatoria: factoriales, coeficientes binomiales
 * - Series matemáticas: Fibonacci, Lucas, Catalan
 * - Números de Bell y Stirling
 * - Particiones de enteros
 *
 * uint128_t permite trabajar con números que exceden los límites de uint64_t,
 * esencial para cálculos combinatorios y series que crecen exponencialmente.
 */

#include <cmath>
#include <int128.hpp>
#include <iomanip>
#include <iostream>
#include <uint128/uint128_cmath.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <vector>

using namespace nstd;

using namespace uint128_literals;

// Colores ANSI
const char* RESET = "\033[0m";
const char* CYAN = "\033[36m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* MAGENTA = "\033[35m";
const char* BLUE = "\033[34m";

void print_header(const std::string& title)
{
    std::cout << "\n" << CYAN << "╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << title;
    for (size_t i = title.length(); i < 54; ++i)
        std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝" << RESET << "\n\n";
}

/**
 * Factorial usando uint128_t
 */
uint128_t factorial(int n)
{
    if (n <= 1)
        return 1;
    uint128_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

/**
 * Coeficiente binomial: C(n, k) = n! / (k! * (n-k)!)
 */
uint128_t binomial(int n, int k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k; // Optimización

    uint128_t result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

/**
 * Número de Catalan: C_n = (2n)! / ((n+1)! * n!)
 */
uint128_t catalan(int n)
{
    return binomial(2 * n, n) / (n + 1);
}

void demo_combinatorics()
{
    print_header("DEMO 1: Combinatoria");

    std::cout << YELLOW << ">>> Factoriales grandes" << RESET << "\n\n";

    std::vector<int> fact_tests = {10, 20, 25, 30, 34};
    for (int n : fact_tests) {
        uint128_t fact = factorial(n);
        std::cout << "  " << n << "! = " << GREEN << fact << RESET;

        // Contar dígitos
        std::string str = fact.to_string();
        std::cout << " (" << str.length() << " dígitos)\n";
    }

    std::cout << "\n" << YELLOW << ">>> Coeficientes binomiales" << RESET << "\n\n";
    std::cout << "  Triángulo de Pascal (primeras 10 filas):\n\n";

    for (int n = 0; n < 10; ++n) {
        std::cout << "  ";
        for (int spaces = 0; spaces < (10 - n) * 3; ++spaces)
            std::cout << " ";

        for (int k = 0; k <= n; ++k) {
            uint128_t coef = binomial(n, k);
            std::cout << GREEN << std::setw(5) << coef << RESET << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n" << YELLOW << ">>> Coeficientes binomiales grandes" << RESET << "\n\n";

    std::vector<std::pair<int, int>> bin_tests = {{50, 25}, {60, 30}, {70, 35}, {80, 40}};

    for (const auto& [n, k] : bin_tests) {
        uint128_t coef = binomial(n, k);
        std::cout << "  C(" << n << ", " << k << ") = " << GREEN << coef << RESET << "\n";
    }

    std::cout << "\n" << YELLOW << ">>> Números de Catalan" << RESET << "\n\n";
    std::cout << "  Los números de Catalan aparecen en:\n";
    std::cout << "    - Árboles binarios\n";
    std::cout << "    - Expresiones con paréntesis\n";
    std::cout << "    - Caminos en cuadrículas\n\n";

    for (int n = 0; n <= 20; ++n) {
        uint128_t cat = catalan(n);
        std::cout << "  C_" << std::setw(2) << n << " = " << GREEN << cat << RESET << "\n";
    }
}

void demo_sequences()
{
    print_header("DEMO 2: Series Matemáticas");

    std::cout << YELLOW << ">>> Secuencia de Fibonacci extendida" << RESET << "\n\n";

    std::vector<uint128_t> fib = {0, 1};

    std::cout << "  Calculando hasta que exceda 10^30...\n\n";

    while (fib.back() < 1000000000000000000000000000000_u128) {
        uint128_t next = fib[fib.size() - 1] + fib[fib.size() - 2];
        fib.push_back(next);
    }

    // Mostrar los últimos 10
    std::cout << "  Últimos 10 términos:\n";
    for (size_t i = fib.size() - 10; i < fib.size(); ++i) {
        std::cout << "  F_" << std::setw(3) << i << " = " << GREEN << fib[i] << RESET << "\n";
    }

    std::cout << "\n  Total de términos calculados: " << MAGENTA << fib.size() << RESET << "\n";

    std::cout << "\n" << YELLOW << ">>> Secuencia de Lucas" << RESET << "\n\n";
    std::cout << "  Similar a Fibonacci, pero L_0=2, L_1=1\n\n";

    std::vector<uint128_t> lucas = {2, 1};
    for (int i = 2; i < 30; ++i) {
        lucas.push_back(lucas[i - 1] + lucas[i - 2]);
    }

    for (size_t i = 0; i < 15; ++i) {
        std::cout << "  L_" << std::setw(2) << i << " = " << GREEN << lucas[i] << RESET << "\n";
    }

    std::cout << "\n"
              << YELLOW << ">>> Relación de Fibonacci con la razón áurea" << RESET << "\n\n";

    // φ = (1 + √5) / 2 ≈ 1.618033988749...
    double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::cout << "  φ (phi) ≈ " << BLUE << std::setprecision(15) << phi << RESET << "\n\n";
    std::cout << "  Límite de F_(n+1)/F_n cuando n→∞ es φ:\n\n";

    for (size_t i = 10; i < 20; ++i) {
        if (fib[i] > 0) {
            double ratio =
                static_cast<double>(fib[i + 1].low()) / static_cast<double>(fib[i].low());
            std::cout << "  F_" << std::setw(2) << (i + 1) << "/F_" << i << " ≈ " << BLUE
                      << std::setprecision(10) << ratio << RESET;
            std::cout << "  (diferencia: " << std::abs(ratio - phi) << ")\n";
        }
    }
}

void demo_partitions()
{
    print_header("DEMO 3: Particiones y Números Especiales");

    std::cout << YELLOW << ">>> Potencias de 2 (exponencial)" << RESET << "\n\n";
    std::cout << "  Útiles en análisis de algoritmos y estructuras de datos\n\n";

    for (int i = 0; i <= 100; i += 10) {
        uint128_t power = uint128_t(1) << i;
        std::cout << "  2^" << std::setw(3) << i << " = " << GREEN << power << RESET << "\n";
    }

    std::cout << "\n" << YELLOW << ">>> Números triangulares" << RESET << "\n\n";
    std::cout << "  T_n = n(n+1)/2 - suma de primeros n enteros\n\n";

    auto triangular = [](uint128_t n) -> uint128_t { return n * (n + 1) / 2; };

    std::vector<uint128_t> t_tests = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    for (const auto& n : t_tests) {
        uint128_t t = triangular(n);
        std::cout << "  T_" << std::setw(8) << n << " = " << GREEN << t << RESET << "\n";
    }

    std::cout << "\n" << YELLOW << ">>> Números perfectos" << RESET << "\n\n";
    std::cout << "  Un número perfecto es igual a la suma de sus divisores propios\n\n";

    // Números perfectos conocidos pequeños
    std::vector<uint128_t> perfect = {
        6_u128,    // 2^1 * (2^2 - 1)
        28_u128,   // 2^2 * (2^3 - 1)
        496_u128,  // 2^4 * (2^5 - 1)
        8128_u128, // 2^6 * (2^7 - 1)
    };

    for (size_t i = 0; i < perfect.size(); ++i) {
        std::cout << "  Perfecto #" << (i + 1) << ": " << GREEN << perfect[i] << RESET << "\n";
    }

    // Calcular el 5º número perfecto
    uint128_t p5 = (uint128_t(1) << 12) * ((uint128_t(1) << 13) - 1);
    std::cout << "  Perfecto #5: " << GREEN << p5 << RESET << " = 2^12 * (2^13 - 1)\n";
}

void demo_approximations()
{
    print_header("DEMO 4: Aproximaciones de Constantes");

    std::cout << YELLOW << ">>> Aproximación de π usando fracciones" << RESET << "\n\n";

    // Aproximaciones racionales famosas de π
    struct Fraction {
        uint128_t num, den;
        std::string name;
    };
    std::vector<Fraction> pi_approx = {
        {22_u128, 7_u128, "Arquímedes"},
        {355_u128, 113_u128, "Zu Chongzhi (Milü)"},
        {103993_u128, 33102_u128, "Adriaan Metius"},
    };

    double pi = 3.141592653589793238;

    for (const auto& frac : pi_approx) {
        double approx = static_cast<double>(frac.num.low()) / static_cast<double>(frac.den.low());
        double error = std::abs(approx - pi);

        std::cout << "  " << frac.name << ": " << GREEN << frac.num << "/" << frac.den << RESET;
        std::cout << " ≈ " << BLUE << std::setprecision(12) << approx << RESET;
        std::cout << "  (error: " << std::scientific << error << ")\n" << std::defaultfloat;
    }

    std::cout << "\n"
              << YELLOW << ">>> Aproximación de e usando serie de Taylor" << RESET << "\n\n";
    std::cout << "  e = 1 + 1/1! + 1/2! + 1/3! + ...\n\n";

    // Calcular suma parcial (numerador/denominador común)
    uint128_t numerator = 1;
    uint128_t denominator = 1;

    for (int n = 1; n <= 20; ++n) {
        uint128_t fact = factorial(n);
        numerator = numerator * fact + denominator;
        denominator = denominator * fact;
    }

    std::cout << "  Suma de primeros 20 términos:\n";
    std::cout << "  " << GREEN << numerator << " / " << denominator << RESET << "\n";

    double e_approx = static_cast<double>(numerator.low()) / static_cast<double>(denominator.low());
    double e_real = 2.718281828459045235;

    std::cout << "  ≈ " << BLUE << std::setprecision(15) << e_approx << RESET << "\n";
    std::cout << "  e real: " << e_real << "\n";
    std::cout << "  Error: " << std::scientific << std::abs(e_approx - e_real) << std::defaultfloat
              << "\n";
}

int main()
{
    std::cout << CYAN << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║   ███████╗ ██████╗██╗███████╗███╗   ██╗ ██████╗███████╗      ║
    ║   ██╔════╝██╔════╝██║██╔════╝████╗  ██║██╔════╝██╔════╝      ║
    ║   ███████╗██║     ██║█████╗  ██╔██╗ ██║██║     █████╗        ║
    ║   ╚════██║██║     ██║██╔══╝  ██║╚██╗██║██║     ██╔══╝        ║
    ║   ███████║╚██████╗██║███████╗██║ ╚████║╚██████╗███████╗      ║
    ║   ╚══════╝ ╚═════╝╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝╚══════╝      ║
    ║                                                               ║
    ║              SHOWCASE: Computación Científica                 ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Esta demo muestra aplicaciones científicas de uint128_t.\n";
    std::cout << "Permite cálculos precisos que exceden los límites de uint64_t.\n";

    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_combinatorics();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_sequences();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_partitions();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_approximations();

    std::cout << "\n"
              << CYAN << R"(
╔═══════════════════════════════════════════════════════════════╗
║                   🔬 DEMO COMPLETADA 🔬                       ║
╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Aplicaciones científicas:\n";
    std::cout << "  • Combinatoria avanzada\n";
    std::cout << "  • Series matemáticas\n";
    std::cout << "  • Aproximación de constantes\n";
    std::cout << "  • Análisis numérico de alta precisión\n";

    return 0;
}
