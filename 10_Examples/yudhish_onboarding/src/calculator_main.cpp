#include <iostream>
#include "yudhish_onboarding/calculator.hpp"

int main() {
  yudhish_onboarding::Calculator calc;
  std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
  return 0;
}