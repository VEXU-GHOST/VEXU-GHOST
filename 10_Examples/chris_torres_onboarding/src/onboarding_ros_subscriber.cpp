#include "chris_torres_onboarding/onboarding_ros_subscriber.hpp"

#include <functional>

namespace chris_torres_onboarding
{
OnboardingROSSubscriber::OnboardingROSSubscriber() : Node("onboarding_ros_subscriber")
{
  subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "scan", 10, std::bind(&OnboardingROSSubscriber::laser_scan_callback, this, _1));
}

void OnboardingROSSubscriber::laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(), "Received LaserScan message with %zu ranges", msg->ranges.size());
}

int OnboardingROSSubscriber::add_ints(int x, int y)
{
  return x + y;
}
}