#include "chris_torres_onboarding/onboarding_ros_subscriber.hpp"
#include "gtest/gtest.h"

#include <memory>

class TestOnboarding : public ::testing::Test
{
protected:
  TestOnboarding()
  {
    subscriber_node_ = std::make_shared<chris_torres_onboarding::OnboardingROSSubscriber>();
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }

  std::shared_ptr<chris_torres_onboarding::OnboardingROSSubscriber> subscriber_node_;
};

TEST_F(TestOnboarding, add_ints)
{
  EXPECT_EQ(subscriber_node_->add_ints(4, 5), 9);
  EXPECT_TRUE(subscriber_node_->add_ints(13, 5) == 18);
  EXPECT_FALSE(subscriber_node_->add_ints(4, 5) == 4);
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}