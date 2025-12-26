/**
 * Tutorial 06: Entrada/Salida con iostream
 *
 * Aprenderás:
 * - Lectura desde cin
 * - Escritura a cout
 * - Manipuladores (hex, oct, dec, showbase, etc.)
 * - Formato de salida
 * - Archivos
 */

#include <fstream>
#include <int128.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 06: iostream\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. COUT BÁSICO
    // ============================================================
    std::cout << "--- 1. Salida básica con cout ---\n";

    uint128_t a = 1000;
    int128_t b = -500;

    std::cout << "uint128_t: " << a << "\n";
    std::cout << "int128_t:  " << b << "\n\n";

    // ============================================================
    // 2. MANIPULADORES NUMÉRICOS
    // ============================================================
    std::cout << "--- 2. Manipuladores numéricos ---\n";

    uint128_t value = 255;

    std::cout << "Decimal:     " << std::dec << value << "\n";
    std::cout << "Hexadecimal: " << std::hex << value << "\n";
    std::cout << "Octal:       " << std::oct << value << "\n";
    std::cout << std::dec; // Volver a decimal
    std::cout << "\n";

    // ============================================================
    // 3. SHOWBASE Y UPPERCASE
    // ============================================================
    std::cout << "--- 3. showbase y uppercase ---\n";

    uint128_t num = 0xABC;

    std::cout << std::showbase << std::uppercase;
    std::cout << "Hex: " << std::hex << num << "\n";
    std::cout << "Oct: " << std::oct << num << "\n";
    std::cout << std::noshowbase << std::nouppercase << std::dec << "\n";

    // ============================================================
    // 4. ANCHO Y RELLENO
    // ============================================================
    std::cout << "--- 4. Ancho y relleno ---\n";

    std::cout << std::setw(15) << std::right << 123 << "\n";
    std::cout << std::setw(15) << std::left << 456 << "|\n";
    std::cout << std::setfill('0') << std::setw(10) << 789 << "\n";
    std::cout << std::setfill(' ') << "\n";

    // ============================================================
    // 5. TABLA FORMATEADA
    // ============================================================
    std::cout << "--- 5. Tabla formateada ---\n";

    struct Data {
        std::string name;
        uint128_t value;
    };

    Data table[] = {{"Alpha", 100}, {"Beta", 5000}, {"Gamma", 250000}};

    std::cout << std::left << std::setw(10) << "Nombre" << std::right << std::setw(12) << "Valor"
              << "\n";
    std::cout << std::string(22, '-') << "\n";

    for (const auto& row : table) {
        std::cout << std::left << std::setw(10) << row.name << std::right << std::setw(12)
                  << row.value << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 6. STRINGSTREAM
    // ============================================================
    std::cout << "--- 6. stringstream ---\n";

    std::ostringstream oss;
    oss << "El valor es: " << uint128_t(12345);
    std::string result = oss.str();

    std::cout << result << "\n\n";

    // ============================================================
    // 7. LECTURA DESDE STRING
    // ============================================================
    std::cout << "--- 7. Lectura desde string ---\n";

    std::string data = "100 200 300";
    std::istringstream iss(data);

    uint128_t x, y, z;
    iss >> x >> y >> z;

    std::cout << "Leídos: " << x << ", " << y << ", " << z << "\n\n";

    // ============================================================
    // 8. ARCHIVO (SIMULADO CON STRINGSTREAM)
    // ============================================================
    std::cout << "--- 8. Escritura/lectura tipo archivo ---\n";

    // Simular escritura a archivo
    std::ostringstream file_out;
    file_out << uint128_t(111) << " " << uint128_t(222) << " " << uint128_t(333);

    std::cout << "Escrito: " << file_out.str() << "\n";

    // Simular lectura
    std::istringstream file_in(file_out.str());
    uint128_t a1, a2, a3;
    file_in >> a1 >> a2 >> a3;

    std::cout << "Leído: " << a1 << ", " << a2 << ", " << a3 << "\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 06 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 07_format.cpp\n";

    return 0;
}
