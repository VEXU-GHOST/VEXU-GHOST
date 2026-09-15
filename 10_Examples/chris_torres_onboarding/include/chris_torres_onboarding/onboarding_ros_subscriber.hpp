#pragma once

#include <memory>
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;

namespace chris_torres_onboarding
{
    class OnboardingROSSubscriber : public rclcpp::Node
    {
    public:
        OnboardingROSSubscriber();
        void topic_callback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);
        int add_ints(int x, int y);

    private:
        rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr subscription_;
    };
}