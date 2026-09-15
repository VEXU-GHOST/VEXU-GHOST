#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

class OnboardingROSPublisher : public rclcpp::Node
{
public:
    OnboardingROSPublisher()
        : Node("onboarding_ros_publisher"), count_(0)
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "onboarding_topic", 10);
        timer_ = this->create_wall_timer(
            500ms, std::bind(&OnboardingROSPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto message = geometry_msgs::msg::PoseWithCovarianceStamped();
        message.header.stamp = this->get_clock()->now();
        publisher_->publish(message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OnboardingROSPublisher>());
    rclcpp::shutdown();
    return 0;
}