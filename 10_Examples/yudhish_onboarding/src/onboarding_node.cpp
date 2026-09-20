#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include "yudhish_onboarding/calculator.hpp"

class OnboardingNode : public rclcpp::Node {
    public:
    OnboardingNode() : Node("onboarding_node") {
        pub_ = create_publisher<std_msgs::msg::Int32>("number_out", 10);
        sub_ = create_subscription<std_msgs::msg::Int32>(
        "number_in", 10,
        [this](const std_msgs::msg::Int32::SharedPtr msg) {
            std_msgs::msg::Int32 out;
            out.data = calc_.square(msg->data);
            RCLCPP_INFO(get_logger(), "Got %d, publishing %d", msg->data, out.data);
            pub_->publish(out);
        });
    }

    private:
    yudhish_onboarding::Calculator calc_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OnboardingNode>());
  rclcpp::shutdown();
  return 0;
}