#include "include_new/int128_base.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

using namespace nstd;

void test_constructor_from_cstr()
{
    std::cout << "Test 1: Constructor desde const char* (auto-deteccion)\n";

    // Decimal
    uint128_t u1("12345");
    assert(u1 == uint128_t(0ull, 12345ull));
    std::cout << "  12345 (decimal) -> " << u1.to_string() << " OK\n";

    // Hexadecimal (prefijo 0x)
    uint128_t u2("0xFF");
    assert(u2 == uint128_t(0ull, 255ull));
    std::cout << "  0xFF (hex) -> " << u2.to_string() << " OK\n";

    // Binario (prefijo 0b)
    uint128_t u3("0b11111111");
    assert(u3 == uint128_t(0ull, 255ull));
    std::cout << "  0b11111111 (bin) -> " << u3.to_string() << " OK\n";

    // Octal (prefijo 0)
    uint128_t u4("0377");
    assert(u4 == uint128_t(0ull, 255ull));
    std::cout << "  0377 (oct) -> " << u4.to_string() << " OK\n";

    // Signed positivo
    int128_t i1("42");
    assert(i1 == int128_t(0ull, 42ull));
    std::cout << "  42 (signed positivo) -> " << i1.to_string() << " OK\n";

    // Signed negativo
    int128_t i2("-42");
    assert(i2.is_negative());
    std::cout << "  -42 (signed negativo) -> " << i2.to_string() << " OK\n";

    std::cout << "  PASS\n\n";
}

void test_constructor_from_cstr_with_base()
{
    std::cout << "Test 2: Constructor desde const char* con base explicita\n";

    // Base 2 (binario)
    uint128_t u1("11111111", 2);
    assert(u1 == uint128_t(0ull, 255ull));
    std::cout << "  11111111 (base 2) -> " << u1.to_string() << " OK\n";

    // Base 8 (octal)
    uint128_t u2("377", 8);
    assert(u2 == uint128_t(0ull, 255ull));
    std::cout << "  377 (base 8) -> " << u2.to_string() << " OK\n";

    // Base 16 (hexadecimal)
    uint128_t u3("FF", 16);
    assert(u3 == uint128_t(0ull, 255ull));
    std::cout << "  FF (base 16) -> " << u3.to_string() << " OK\n";

    // Base 36 (maximo)
    uint128_t u4("ZZ", 36);
    std::cout << "  ZZ (base 36) -> " << u4.to_string() << " OK\n";

    std::cout << "  PASS\n\n";
}

void test_constructor_from_string()
{
    std::cout << "Test 3: Constructor desde std::string\n";

    std::string s1 = "12345";
    uint128_t u1(s1);
    assert(u1 == uint128_t(0ull, 12345ull));
    std::cout << "  std::string(\"12345\") -> " << u1.to_string() << " OK\n";

    std::string s2 = "0xDEADBEEF";
    uint128_t u2(s2);
    std::cout << "  std::string(\"0xDEADBEEF\") -> " << u2.to_string() << " OK\n";

    std::string s3 = "-999";
    int128_t i1(s3);
    assert(i1.is_negative());
    std::cout << "  std::string(\"-999\") -> " << i1.to_string() << " OK\n";

    std::cout << "  PASS\n\n";
}

void test_constructor_from_string_view()
{
    std::cout << "Test 4: Constructor desde std::string_view\n";

    std::string_view sv1 = "67890";
    uint128_t u1(sv1);
    assert(u1 == uint128_t(0ull, 67890ull));
    std::cout << "  std::string_view(\"67890\") -> " << u1.to_string() << " OK\n";

    std::string_view sv2 = "0xABCD";
    uint128_t u2(sv2);
    std::cout << "  std::string_view(\"0xABCD\") -> " << u2.to_string() << " OK\n";

    std::string_view sv3 = "-123";
    int128_t i1(sv3);
    assert(i1.is_negative());
    std::cout << "  std::string_view(\"-123\") -> " << i1.to_string() << " OK\n";

    std::cout << "  PASS\n\n";
}

void test_assignment_from_cstr()
{
    std::cout << "Test 5: Asignacion desde const char*\n";

    uint128_t u1;
    u1 = "99999";
    assert(u1 == uint128_t(0ull, 99999ull));
    std::cout << "  u1 = \"99999\" -> " << u1.to_string() << " OK\n";

    u1 = "0xCAFE";
    std::cout << "  u1 = \"0xCAFE\" -> " << u1.to_string() << " OK\n";

    int128_t i1;
    i1 = "-777";
    assert(i1.is_negative());
    std::cout << "  i1 = \"-777\" -> " << i1.to_string() << " OK\n";

    std::cout << "  PASS\n\n";
}

void test_to_string_base10()
{
    std::cout << "Test 6: to_string() base 10 (decimal)\n";

    uint128_t u1(0ull, 0ull);
    assert(u1.to_string() == "0");
    std::cout << "  0 -> \"" << u1.to_string() << "\" OK\n";

    uint128_t u2(0ull, 12345ull);
    assert(u2.to_string() == "12345");
    std::cout << "  12345 -> \"" << u2.to_string() << "\" OK\n";

    uint128_t u3(0ull, 0xFFFFFFFFFFFFFFFFull);
    std::cout << "  0xFFFFFFFFFFFFFFFF -> \"" << u3.to_string() << "\" OK\n";

    uint128_t u4(0x1ull, 0ull);
    std::cout << "  0x1_0000000000000000 -> \"" << u4.to_string() << "\" OK\n";

    // Signed positivo
    int128_t i1(0ull, 999ull);
    assert(i1.to_string() == "999");
    std::cout << "  999 (signed) -> \"" << i1.to_string() << "\" OK\n";

    // Signed negativo
    int128_t i2(0ull, 42ull);
    i2 = -i2;
    assert(i2.to_string() == "-42");
    std::cout << "  -42 (signed) -> \"" << i2.to_string() << "\" OK\n";

    std::cout << "  PASS\n\n";
}

void test_to_string_other_bases()
{
    std::cout << "Test 7: to_string(base) con bases 2-36\n";

    uint128_t u(0ull, 255ull);

    // Base 2 (binario)
    std::string bin = u.to_string(2);
    assert(bin == "11111111");
    std::cout << "  255 -> base 2: \"" << bin << "\" OK\n";

    // Base 8 (octal)
    std::string oct = u.to_string(8);
    assert(oct == "377");
    std::cout << "  255 -> base 8: \"" << oct << "\" OK\n";

    // Base 16 (hexadecimal)
    std::string hex = u.to_string(16);
    assert(hex == "ff");
    std::cout << "  255 -> base 16: \"" << hex << "\" OK\n";

    // Base 36 (maximo)
    uint128_t u2(0ull, 1295ull); // ZZ en base 36 = 35*36 + 35 = 1295
    std::string b36 = u2.to_string(36);
    assert(b36 == "zz");
    std::cout << "  1295 -> base 36: \"" << b36 << "\" OK\n";

    std::cout << "  PASS\n\n";
}

void test_stream_output()
{
    std::cout << "Test 8: operator<< (salida a stream)\n";

    std::ostringstream oss;

    uint128_t u1(0ull, 12345ull);
    oss << u1;
    assert(oss.str() == "12345");
    std::cout << "  uint128_t(12345) -> \"" << oss.str() << "\" OK\n";

    oss.str("");
    int128_t i1(0ull, 999ull);
    oss << i1;
    assert(oss.str() == "999");
    std::cout << "  int128_t(999) -> \"" << oss.str() << "\" OK\n";

    oss.str("");
    int128_t i2(0ull, 42ull);
    i2 = -i2;
    oss << i2;
    assert(oss.str() == "-42");
    std::cout << "  int128_t(-42) -> \"" << oss.str() << "\" OK\n";

    std::cout << "  PASS\n\n";
}

void test_stream_input()
{
    std::cout << "Test 9: operator>> (entrada desde stream)\n";

    std::istringstream iss;
    uint128_t u1;

    iss.str("54321");
    iss >> u1;
    assert(u1 == uint128_t(0ull, 54321ull));
    std::cout << "  stream(\"54321\") -> uint128_t(" << u1.to_string() << ") OK\n";

    iss.clear();
    iss.str("0xABCD");
    uint128_t u2;
    iss >> u2;
    std::cout << "  stream(\"0xABCD\") -> uint128_t(" << u2.to_string() << ") OK\n";

    iss.clear();
    iss.str("-777");
    int128_t i1;
    iss >> i1;
    assert(i1.is_negative());
    std::cout << "  stream(\"-777\") -> int128_t(" << i1.to_string() << ") OK\n";

    std::cout << "  PASS\n\n";
}

void test_error_handling()
{
    std::cout << "Test 10: Manejo de errores (excepciones)\n";

    // Null pointer
    try {
        uint128_t u(nullptr);
        std::cout << "  ERROR: debio lanzar excepcion para nullptr\n";
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  nullptr -> excepcion: \"" << e.what() << "\" OK\n";
    }

    // String vacio
    try {
        uint128_t u("");
        std::cout << "  ERROR: debio lanzar excepcion para string vacio\n";
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  \"\" -> excepcion: \"" << e.what() << "\" OK\n";
    }

    // Caracter invalido
    try {
        uint128_t u("123abc");
        std::cout << "  ERROR: debio lanzar excepcion para caracter invalido\n";
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  \"123abc\" -> excepcion: \"" << e.what() << "\" OK\n";
    }

    // Base invalida
    try {
        uint128_t u("123", 1); // Base < 2
        std::cout << "  ERROR: debio lanzar excepcion para base invalida\n";
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  base 1 -> excepcion: \"" << e.what() << "\" OK\n";
    }

    // Base invalida (to_string)
    try {
        uint128_t u(0ull, 100ull);
        std::string s = u.to_string(37); // Base > 36
        std::cout << "  ERROR: debio lanzar excepcion para base 37\n";
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  to_string(37) -> excepcion: \"" << e.what() << "\" OK\n";
    }

    std::cout << "  PASS\n\n";
}

void test_roundtrip()
{
    std::cout << "Test 11: Roundtrip (string -> valor -> string)\n";

    // Decimal
    std::string s1 = "123456789012345";
    uint128_t u1(s1);
    std::string result1 = u1.to_string();
    assert(s1 == result1);
    std::cout << "  \"" << s1 << "\" -> uint128_t -> \"" << result1 << "\" OK\n";

    // Hexadecimal
    uint128_t u2("0xDEADBEEFCAFE");
    std::string hex = u2.to_string(16);
    uint128_t u3(("0x" + hex).c_str());
    assert(u2 == u3);
    std::cout << "  0xDEADBEEFCAFE -> \"" << hex << "\" -> uint128_t OK\n";

    // Signed negativo
    int128_t i1("-987654321");
    std::string si = i1.to_string();
    int128_t i2(si);
    assert(i1 == i2);
    std::cout << "  \"-987654321\" -> int128_t -> \"" << si << "\" OK\n";

    std::cout << "  PASS\n\n";
}

int main()
{
    std::cout << "============================================================\n";
    std::cout << "  TESTS DE OPERACIONES DE STRING Y STREAM\n";
    std::cout << "============================================================\n\n";

    test_constructor_from_cstr();
    test_constructor_from_cstr_with_base();
    test_constructor_from_string();
    test_constructor_from_string_view();
    test_assignment_from_cstr();
    test_to_string_base10();
    test_to_string_other_bases();
    test_stream_output();
    test_stream_input();
    test_error_handling();
    test_roundtrip();

    std::cout << "============================================================\n";
    std::cout << "  TODOS LOS TESTS PASARON (11/11)\n";
    std::cout << "============================================================\n";

    return 0;
}
