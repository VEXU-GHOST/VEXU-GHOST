#include "chris_torres_onboarding/count_to_ten.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    CountToTen counter;

    std::vector<int> numbers = counter.count();

    std::cout << "Numbers:" << std::endl;

    std::cout << "Hello World!" << std::endl;

    return 0;
}