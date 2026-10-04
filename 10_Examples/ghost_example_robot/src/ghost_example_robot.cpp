/*
 *   Copyright (c) 2024 Maxx Wilson
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ghost_example_robot/ghost_example_robot.hpp>
#include <pluginlib/class_list_macros.hpp>

using ghost_planners::RobotTrajectory;
using ghost_ros_interfaces::msg_helpers::fromROSMsg;
using std::placeholders::_1;

namespace ghost_example_robot
{

GhostExampleRobot::GhostExampleRobot()
{
}

void GhostExampleRobot::initialize()
{
  // Only called once when program starts!
  std::cout << "initialize" << std::endl;

  // Drivetrain geometry for odometry (defaults match the competition robots, see example_ros_config.yaml)
  node_ptr_->declare_parameter("odometry.odom_topic", "/sensors/wheel_odom");
  node_ptr_->declare_parameter("odometry.drive_gear_ratio", 1.15);      // motor rotations per wheel rotation
  node_ptr_->declare_parameter("odometry.drive_wheel_rad_in", 1.375);
  node_ptr_->declare_parameter("odometry.wheel_base_inches", 12.6875);

  constexpr double INCHES_TO_METERS = 0.0254;
  double gear_ratio = node_ptr_->get_parameter("odometry.drive_gear_ratio").as_double();
  double wheel_rad_m = node_ptr_->get_parameter("odometry.drive_wheel_rad_in").as_double() * INCHES_TO_METERS;
  wheelbase_m_ = node_ptr_->get_parameter("odometry.wheel_base_inches").as_double() * INCHES_TO_METERS;

  // Motor encoders report degrees (encoder_units: DEGREES in example_hardware_config.yaml)
  meters_per_degree_ = (2.0 * M_PI * wheel_rad_m) / (360.0 * gear_ratio);

  odom_pub_ = node_ptr_->create_publisher<nav_msgs::msg::Odometry>(
    node_ptr_->get_parameter("odometry.odom_topic").as_string(), rclcpp::SensorDataQoS());
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*node_ptr_);
}

void GhostExampleRobot::onNewSensorData()
{
  // Optional, called before disabled/autonomous/teleop when new data arrives.
  updateAndPublishOdometry();
}

void GhostExampleRobot::updateAndPublishOdometry()
{
  double left_deg = rhi_ptr_->getMotorPosition("left_motor");
  double right_deg = rhi_ptr_->getMotorPosition("right_motor");

  // First reading only sets the reference so the robot starts at the origin
  if (!odom_initialized_) {
    prev_left_deg_ = left_deg;
    prev_right_deg_ = right_deg;
    odom_initialized_ = true;
  }

  double dl = (left_deg - prev_left_deg_) * meters_per_degree_;
  double dr = (right_deg - prev_right_deg_) * meters_per_degree_;
  prev_left_deg_ = left_deg;
  prev_right_deg_ = right_deg;

  // Movement in the robot frame: straight line, or an arc when the wheels differ
  double local_x = (dl + dr) / 2.0;
  double local_y = 0.0;
  double dtheta = (dr - dl) / wheelbase_m_;
  if (dtheta != 0.0) {
    double turn_radius = local_x / dtheta;
    local_x = turn_radius * std::sin(dtheta);
    local_y = turn_radius * (1.0 - std::cos(dtheta));
  }

  // Rotate into the odom frame
  x_ += local_x * std::cos(theta_) - local_y * std::sin(theta_);
  y_ += local_x * std::sin(theta_) + local_y * std::cos(theta_);
  theta_ = std::atan2(std::sin(theta_ + dtheta), std::cos(theta_ + dtheta));

  auto stamp = node_ptr_->get_clock()->now();

  nav_msgs::msg::Odometry msg{};
  msg.header.frame_id = "odom";
  msg.header.stamp = stamp;
  msg.child_frame_id = "base_link";
  msg.pose.pose.position.x = x_;
  msg.pose.pose.position.y = y_;
  msg.pose.pose.orientation.w = std::cos(theta_ / 2.0);
  msg.pose.pose.orientation.z = std::sin(theta_ / 2.0);
  odom_pub_->publish(msg);

  // The competition robots get odom -> base_link from their EKF; publish it here so Foxglove can follow the robot
  geometry_msgs::msg::TransformStamped tf{};
  tf.header = msg.header;
  tf.child_frame_id = "base_link";
  tf.transform.translation.x = x_;
  tf.transform.translation.y = y_;
  tf.transform.rotation = msg.pose.pose.orientation;
  tf_broadcaster_->sendTransform(tf);
}

void GhostExampleRobot::disabled()
{
  std::cout << "disabled" << std::endl;
}

void GhostExampleRobot::autonomous(double current_time)
{
  std::cout << "Autonomous" << current_time << std::endl;
}

void GhostExampleRobot::teleop(double current_time)
{
  static int loop_count = 0;
  if (loop_count++ % 100 == 0) {
    std::cout << "Teleop " << current_time << std::endl;
  }

  auto joy_data = rhi_ptr_->getMainJoystickData();
////////////////////////////////////////////////////////////////////










  /////////////////////////////////////////////////////////
  if (joy_data->btn_a) {
    std::cout << "Button A!" << std::endl;
  } else if (joy_data->btn_b) {
    std::cout << "Button B!" << std::endl;
  } else if (joy_data->btn_x) {
    std::cout << "Button X!" << std::endl;
  } else if (joy_data->btn_y) {
    std::cout << "Button Y!" << std::endl;
  } else if (joy_data->btn_u) {
    std::cout << "Button U!" << std::endl;
  } else if (joy_data->btn_d) {
    std::cout << "Button D!" << std::endl;
  } else if (joy_data->btn_l) {
    std::cout << "Button L!" << std::endl;
  } else if (joy_data->btn_r) {
    std::cout << "Button R!" << std::endl;
  } else if (joy_data->btn_l1) {
    std::cout << "Button L1!" << std::endl;
  } else if (joy_data->btn_l2) {
    std::cout << "Button L2!" << std::endl;
  }

  // Print joystick data!
  if (joy_data->btn_r1) {
    // Left joystick up-down axis is "left_y", left-right axis is "left_x"
    // Right joystick up-down axis is "right_y", left-right axis is "right_x"
    std::cout << "Left X: " << joy_data->left_x << std::endl;
    std::cout << "Left Y: " << joy_data->left_y << std::endl;
    std::cout << "Right X: " << joy_data->right_x << std::endl;
    std::cout << "Right Y: " << joy_data->right_y << std::endl;
    std::cout << std::endl;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arcade Drive Controls

if (joy_data->btn_r2){

  // Adding both left y axis and right y axis
  double forward_vel = (joy_data->left_y) / 127.0;
  //Adding both right x axis and left x axis
  double angular_vel = (joy_data->right_x ) / 127.0;

  //dead zone (must happen before the values are sent to the motors)
  double threshold = 0.05;
  forward_vel = (std::fabs(forward_vel) < threshold) ? 0.0 : forward_vel;
  angular_vel = (std::fabs(angular_vel) < threshold) ? 0.0 : angular_vel;

  // Mix forward and turning into one command per motor, clamped to -1.0 <-> 1.0.
  // Each motor only keeps its last command, so it must be set exactly once.
  double left_power = std::clamp(forward_vel + angular_vel, -1.0, 1.0);
  double right_power = std::clamp(forward_vel - angular_vel, -1.0, 1.0);

        // setMotorVoltageCommandPercent maps -1.0 <-> 1.0 to -12000 <-> 12000 milliVolts behind the scenes.
    rhi_ptr_->setMotorVoltageCommandPercent("left_motor", left_power);
    rhi_ptr_->setMotorVoltageCommandPercent("right_motor", right_power);


        // Each motor has a current limit that defaults to zero.
    // This is so we can carefully allocate battery power between systems.
    // If we don't set these, the motors will be extremely weak, if they move at all.
    rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 2500.0);
    rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 2500.0);

     // Now we can get motor data and print it.
    double y_axis_motor = rhi_ptr_->getMotorPosition("left_motor");
    double x_axis_motor = rhi_ptr_->getMotorPosition("right_motor");

    // These are in degrees. Units and other data can be configured in example_hardware_config.yaml.
    std::cout << "Y axis both motor: " << forward_vel << " deg" << std::endl;
    std::cout << "X axis both motor: " << angular_vel << " deg" << std::endl;
    std::cout << std::endl;

} else {
   // Don't forget to turn motors off!
    rhi_ptr_->setMotorVoltageCommandPercent("left_motor", 0.0);
    rhi_ptr_->setMotorVoltageCommandPercent("right_motor", 0.0);

    rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 0.0);
    rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 0.0);
}




















//////////////////////////////////////////////////////////////////////////////////////////////////////
//Tank Drive Controls

  // While holding button R2, send motor commands based on joystick values
  // if (joy_data->btn_r2) {
  //   // Joysticks go from -127 to 127, but motors take a value from -1.0 to 1.0.
  //   double left_wheel_power = joy_data->left_y / 127.0;
  //   double right_wheel_power = joy_data->right_y / 127.0;

  //   // setMotorVoltageCommandPercent maps -1.0 <-> 1.0 to -12000 <-> 12000 milliVolts behind the scenes.
  //   rhi_ptr_->setMotorVoltageCommandPercent("left_motor", left_wheel_power);
  //   rhi_ptr_->setMotorVoltageCommandPercent("right_motor", right_wheel_power);

  //   // Each motor has a current limit that defaults to zero.
  //   // This is so we can carefully allocate battery power between systems.
  //   // If we don't set these, the motors will be extremely weak, if they move at all.
  //   rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 2500.0);
  //   rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 2500.0);

  //   // Now we can get motor data and print it.
  //   double left_position = rhi_ptr_->getMotorPosition("left_motor");
  //   double right_position = rhi_ptr_->getMotorPosition("right_motor");

  //   // These are in degrees. Units and other data can be configured in example_hardware_config.yaml.
  //   std::cout << "Left Motor: " << left_position << " deg" << std::endl;
  //   std::cout << "Right Motor: " << right_position << " deg" << std::endl;
  //   std::cout << std::endl;
  // } else {
  //   // Don't forget to turn motors off!
  //   rhi_ptr_->setMotorVoltageCommandPercent("left_motor", 0.0);
  //   rhi_ptr_->setMotorVoltageCommandPercent("right_motor", 0.0);

  //   rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 0.0);
  //   rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 0.0);
  // }
}
} // namespace ghost_example_robot

PLUGINLIB_EXPORT_CLASS(
  ghost_example_robot::GhostExampleRobot,
  ghost_ros_interfaces::V5RobotBase)
