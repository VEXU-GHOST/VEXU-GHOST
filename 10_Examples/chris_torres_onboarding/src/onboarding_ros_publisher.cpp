#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class OnboardingROSPublisher : public rclcpp::Node
{
public:
  OnboardingROSPublisher()
  : Node("onboarding_ros_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("pose_topic", 10);
    timer_ = this->create_wall_timer(500ms, std::bind(&OnboardingROSPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = geometry_msgs::msg::PoseWithCovarianceStamped();
    message.header.stamp = this->now();
    message.header.frame_id = "map";
    message.pose.pose.position.x = static_cast<double>(count_++);

    publisher_->publish(message);
  }

  rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OnboardingROSPublisher>());
  rclcpp::shutdown();
  return 0;
}