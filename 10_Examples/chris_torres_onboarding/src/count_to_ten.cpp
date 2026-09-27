#include "chris_torres_onboarding/count_to_ten.hpp"

#include <iostream>

std::vector<int> CountToTen::count()
{
    std::vector<int> numbers;
    for (int i = 1; i <= 10; ++i)
    {
        numbers.push_back(i);
    }
    return numbers;
}