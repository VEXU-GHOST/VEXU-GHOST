#include "chris_torres_onboarding/onboarding_ros_subscriber.hpp"

namespace chris_torres_onboarding
{
    OnboardingROSSubscriber::OnboardingROSSubscriber()
        : Node("onboarding_ros_subscriber")
    {
        subscription_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "onboarding_topic", 10, std::bind(&OnboardingROSSubscriber::topic_callback, this, _1));
    }

    void OnboardingROSSubscriber::topic_callback(
        const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
    {
        auto now = this->get_clock()->now();
        auto diff = now - msg->header.stamp;
        RCLCPP_INFO(this->get_logger(), "%ld us", diff.nanoseconds() / 1000);
    }

    int OnboardingROSSubscriber::add_ints(int x, int y)
    {
        return x + y;
    }
}