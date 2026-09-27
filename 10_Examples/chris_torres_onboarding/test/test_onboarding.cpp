#include "gtest/gtest.h"
#include "chris_torres_onboarding/count_to_ten.hpp"
#include <vector>

class CountToTenTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(CountToTenTest, CountToTen)
{
    CountToTen counter;

    std::vector<int> result = counter.count();
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_EQ(result, expected);
};

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}