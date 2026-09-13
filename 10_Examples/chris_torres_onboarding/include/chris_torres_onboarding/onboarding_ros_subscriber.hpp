#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using std::placeholders::_1;

namespace chris_torres_onboarding
{
    class OnboardingROSSubscriber : public rclcpp::Node
    {
    public:
        OnboardingROSSubscriber();
        int add_ints(int x, int y);

    private:
        void laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
    };
}