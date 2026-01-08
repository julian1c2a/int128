#include "int128_base_tt.hpp"
#include <iostream>
using namespace nstd;

int main() {
  int128_t a(-100);
  int128_t b(10);
  int128_t c(-10);

  std::cout << "a = " << a.to_string() << std::endl;
  std::cout << "b = " << b.to_string() << std::endl;
  std::cout << "c = " << c.to_string() << std::endl;

  auto div1 = a / b;
  std::cout << "-100 / 10 = " << div1.to_string() << std::endl;
  std::cout << "Expected: -10" << std::endl;

  auto div2 = a / c;
  std::cout << "-100 / -10 = " << div2.to_string() << std::endl;
  std::cout << "Expected: 10" << std::endl;

  int128_t d(-107);
  auto mod1 = d % b;
  std::cout << "-107 % 10 = " << mod1.to_string() << std::endl;
  std::cout << "Expected: -7" << std::endl;

  return 0;
}
