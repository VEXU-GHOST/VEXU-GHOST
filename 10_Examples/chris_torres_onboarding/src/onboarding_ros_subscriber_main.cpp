#include "chris_torres_onboarding/onboarding_ros_subscriber.hpp"

#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<chris_torres_onboarding::OnboardingROSSubscriber>());
  rclcpp::shutdown();
  return 0;
}