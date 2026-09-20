#include <gtest/gtest.h>
#include "yudhish_onboarding/calculator.hpp"

TEST(CalculatorTest, Add) {
  yudhish_onboarding::Calculator calc;
  EXPECT_EQ(calc.add(2, 3), 5);
}

TEST(CalculatorTest, Square) {
  yudhish_onboarding::Calculator calc;
  EXPECT_EQ(calc.square(4), 16);
}