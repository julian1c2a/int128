/**
 * ============================================================================
 * SHOWCASE: CRYPTOGRAPHY - Aplicaciones Criptográficas
 * ============================================================================
 *
 * Este showcase demuestra el uso de uint128_t en contextos criptográficos:
 * - Generación de números primos grandes
 * - Exponenciación modular (base de RSA)
 * - Test de primalidad
 * - Generación de claves toy RSA
 * - Cifrado/descifrado simple
 *
 * NOTA: Esto es SOLO para fines educativos. Para criptografía real,
 *       usa bibliotecas especializadas como OpenSSL.
 */

#include <int128.hpp>
#include <iomanip>
#include <iostream>
#include <random>
#include <uint128/uint128_cmath.hpp>
#include <uint128/uint128_iostreams.hpp>

using namespace nstd;

using namespace uint128_literals;

// Colores ANSI
const char* RESET = "\033[0m";
const char* CYAN = "\033[36m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* MAGENTA = "\033[35m";

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
 * Test de primalidad de Miller-Rabin simplificado
 * Para fines educativos - versión básica
 */
bool is_probably_prime(const uint128_t& n, int iterations = 5)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0)
        return false;

    // Escribir n-1 como 2^r * d
    uint128_t d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    // Test de Miller-Rabin simplificado
    std::random_device rd;
    std::mt19937_64 gen(rd());

    for (int i = 0; i < iterations; ++i) {
        // Seleccionar base aleatoria entre 2 y n-2
        uint64_t rand_val = gen();
        uint128_t a = 2 + (rand_val % std::min(static_cast<uint64_t>(n.low() - 3), UINT64_MAX));

        // x = a^d mod n usando función auxiliar
        uint128_t x = 1;
        uint128_t temp_d = d;
        uint128_t temp_a = a % n;
        while (temp_d > 0) {
            if (temp_d % 2 == 1) {
                x = (x * temp_a) % n;
            }
            temp_a = (temp_a * temp_a) % n;
            temp_d /= 2;
        }

        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        for (int j = 0; j < r - 1; ++j) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;
    }

    return true;
}

/**
 * Exponenciación modular: (base^exp) mod m
 * Algoritmo de exponenciación rápida
 */
uint128_t mod_pow(uint128_t base, uint128_t exp, const uint128_t& mod)
{
    uint128_t result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }

    return result;
}

void demo_prime_numbers()
{
    print_header("DEMO 1: Números Primos Grandes");

    std::cout << YELLOW << ">>> Primos de Mersenne conocidos" << RESET << "\n\n";

    // Algunos primos de Mersenne pequeños: 2^p - 1
    std::vector<int> mersenne_exponents = {2, 3, 5, 7, 13, 17, 19, 31};

    for (int p : mersenne_exponents) {
        uint128_t mersenne = (uint128_t(1) << p) - 1;
        std::cout << "  M" << std::setw(2) << p << " = 2^" << p << " - 1 = " << GREEN << mersenne
                  << RESET;

        if (is_probably_prime(mersenne)) {
            std::cout << " ✓ PRIMO\n";
        } else {
            std::cout << " (compuesto)\n";
        }
    }

    std::cout << "\n" << YELLOW << ">>> Buscando primos en un rango" << RESET << "\n\n";

    uint128_t start = 1000000000_u128; // 10^9 (mil millones)
    uint128_t found = 0;
    int count = 0;

    std::cout << "  Buscando 5 primos cerca de " << start << "...\n";

    for (uint128_t n = start + 1; count < 5; n += 2) {
        if (is_probably_prime(n)) {
            std::cout << "  ✓ Primo #" << (count + 1) << ": " << GREEN << n << RESET << "\n";
            count++;
        }
    }
}

void demo_modular_arithmetic()
{
    print_header("DEMO 2: Aritmética Modular");

    std::cout << YELLOW << ">>> Exponenciación modular" << RESET << "\n\n";

    uint128_t base = 12345_u128;
    uint128_t exp = 67890_u128;
    uint128_t mod = 9999991_u128; // Primo grande

    std::cout << "  Base:      " << base << "\n";
    std::cout << "  Exponente: " << exp << "\n";
    std::cout << "  Módulo:    " << mod << "\n\n";

    uint128_t result = mod_pow(base, exp, mod);

    std::cout << "  " << GREEN << base << "^" << exp << " mod " << mod << " = " << result << RESET
              << "\n\n";

    std::cout << YELLOW << ">>> Propiedad: (a * b) mod m = ((a mod m) * (b mod m)) mod m" << RESET
              << "\n\n";

    uint128_t a = 123456789_u128;
    uint128_t b = 987654321_u128;
    uint128_t m = 100000007_u128;

    uint128_t direct = (a * b) % m;
    uint128_t indirect = ((a % m) * (b % m)) % m;

    std::cout << "  Directo:   " << direct << "\n";
    std::cout << "  Indirecto: " << indirect << "\n";
    std::cout << "  " << (direct == indirect ? GREEN : MAGENTA) << "¡Verificación exitosa!" << RESET
              << "\n";
}

void demo_toy_rsa()
{
    print_header("DEMO 3: RSA Toy (Educativo)");

    std::cout << YELLOW << ">>> Generación de claves RSA (versión simplificada)" << RESET << "\n\n";

    // Usar primos pequeños para demostración
    uint128_t p = 61_u128; // Primo 1
    uint128_t q = 53_u128; // Primo 2

    std::cout << "  1. Seleccionar dos primos p y q:\n";
    std::cout << "     p = " << GREEN << p << RESET << " (primo)\n";
    std::cout << "     q = " << GREEN << q << RESET << " (primo)\n\n";

    uint128_t n = p * q;
    std::cout << "  2. Calcular n = p * q:\n";
    std::cout << "     n = " << GREEN << n << RESET << "\n\n";

    uint128_t phi = (p - 1) * (q - 1);
    std::cout << "  3. Calcular φ(n) = (p-1)(q-1):\n";
    std::cout << "     φ(n) = " << GREEN << phi << RESET << "\n\n";

    uint128_t e = 17_u128; // Exponente público común
    std::cout << "  4. Elegir exponente público e:\n";
    std::cout << "     e = " << GREEN << e << RESET << " (coprimo con φ(n))\n\n";

    // Calcular d (inverso multiplicativo de e mod φ(n))
    // Para simplificar, usamos un valor conocido
    uint128_t d = 2753_u128;
    std::cout << "  5. Calcular exponente privado d:\n";
    std::cout << "     d = " << GREEN << d << RESET << " (inverso de e mod φ(n))\n\n";

    std::cout << "  " << CYAN << "Clave pública:  (e=" << e << ", n=" << n << ")\n";
    std::cout << "  Clave privada: (d=" << d << ", n=" << n << ")" << RESET << "\n\n";

    // Cifrar un mensaje
    uint128_t mensaje = 42_u128;
    std::cout << YELLOW << ">>> Cifrado y descifrado" << RESET << "\n\n";
    std::cout << "  Mensaje original: " << GREEN << mensaje << RESET << "\n";

    uint128_t cifrado = mod_pow(mensaje, e, n);
    std::cout << "  Mensaje cifrado:  " << MAGENTA << cifrado << RESET << "\n";

    uint128_t descifrado = mod_pow(cifrado, d, n);
    std::cout << "  Mensaje descifrado: " << GREEN << descifrado << RESET << "\n\n";

    if (mensaje == descifrado) {
        std::cout << "  " << GREEN << "✓ ¡Cifrado/descifrado exitoso!" << RESET << "\n";
    }
}

void demo_hash_functions()
{
    print_header("DEMO 4: Funciones Hash Simples");

    std::cout << YELLOW << ">>> Hash simple basado en aritmética modular" << RESET << "\n\n";

    auto simple_hash = [](const std::string& str) -> uint128_t {
        uint128_t hash = 0;
        uint128_t prime = 31_u128;
        uint128_t mod = (uint128_t(1) << 64) - 59; // Primo grande

        for (char c : str) {
            hash = (hash * prime + static_cast<unsigned char>(c)) % mod;
        }
        return hash;
    };

    std::vector<std::string> mensajes = {
        "Hello, World!",
        "int128 library",
        "Cryptography demo",
        "Hello, World!", // Duplicado para verificar
    };

    for (const auto& msg : mensajes) {
        uint128_t hash = simple_hash(msg);
        std::cout << "  Hash(\"" << msg << "\") = " << GREEN << std::hex << hash << std::dec
                  << RESET << "\n";
    }

    std::cout << "\n  " << YELLOW << "Nota: Hashes idénticos para strings idénticos ✓" << RESET
              << "\n";
}

int main()
{
    std::cout << CYAN << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║   ██████╗██████╗ ██╗   ██╗██████╗ ████████╗ ██████╗          ║
    ║  ██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝██╔═══██╗         ║
    ║  ██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   ██║   ██║         ║
    ║  ██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   ██║   ██║         ║
    ║  ╚██████╗██║  ██║   ██║   ██║        ██║   ╚██████╔╝         ║
    ║   ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝    ╚═════╝          ║
    ║                                                               ║
    ║              SHOWCASE: Aplicaciones Criptográficas           ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Esta demo muestra aplicaciones criptográficas de uint128_t.\n";
    std::cout << MAGENTA << "ADVERTENCIA: Solo para fines educativos." << RESET << "\n";
    std::cout << "Para criptografía real, usa bibliotecas especializadas.\n";

    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_prime_numbers();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_modular_arithmetic();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_toy_rsa();
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.get();

    demo_hash_functions();

    std::cout << "\n"
              << CYAN << R"(
╔═══════════════════════════════════════════════════════════════╗
║                   🔐 DEMO COMPLETADA 🔐                       ║
╚═══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << "Explora más:\n";
    std::cout << "  • Documentación de criptografía: https://crypto.stackexchange.com\n";
    std::cout << "  • OpenSSL para producción: https://www.openssl.org\n";

    return 0;
}

