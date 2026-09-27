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
}

void GhostExampleRobot::onNewSensorData()
{
  // Optional, called before disabled/autonomous/teleop when new data arrives.
  // std::cout << "onNewSensorData" << std::endl;
}

void GhostExampleRobot::disabled()
{
  std::cout << "disabled" << std::endl;
}

void GhostExampleRobot::autonomous(double current_time)
{
  std::cout << "Autonomous" << current_time << std::endl;
}

enum class DriveMode {
    TankDrive,
    ArcadeDrive    
};

void GhostExampleRobot::teleop(double current_time)
{
  constexpr double wheelDiameterInches = 3.25;
  constexpr double trackWidthInches = 12.0;
  constexpr double pi = 3.14159265358979323846;

  constexpr double wheelCircumferenceInches = pi*wheelDiameterInches;
  constexpr double degreesPerInch = 360.0/wheelCircumferenceInches;

  constexpr double turnNinetyDegreesInches = (pi * trackWidthInches) / 4.0;
  constexpr double turnNinetyDegreesDegrees = turnNinetyDegreesInches * degreesPerInch;

  constexpr double Kp = 0.0035; 
  constexpr double Kd = 0.0002;

  static DriveMode currentDriveMode = DriveMode::TankDrive;

  static bool is_auton_moving = false;
  static double target_left_deg = 0.0;
  static double target_right_deg = 0.0;

  static bool prev_btn_u = false;
  static bool prev_btn_d = false;
  static bool prev_btn_l = false;
  static bool prev_btn_r = false;

  auto joy_data = rhi_ptr_->getMainJoystickData();

  double curr_left_pos = rhi_ptr_->getMotorPosition("left_motor");
  double curr_right_pos = rhi_ptr_->getMotorPosition("right_motor");
  double curr_left_vel = rhi_ptr_->getMotorVelocity("left_motor");
  double curr_right_vel = rhi_ptr_->getMotorVelocity("right_motor");

  if (joy_data->btn_a) {
    currentDriveMode = DriveMode::TankDrive;
    std::cout << "Switched to Tank Drive Mode" << std::endl;
  } 
  else if (joy_data->btn_b) {
    currentDriveMode = DriveMode::ArcadeDrive;
    std::cout << "Switched to Arcade Drive Mode" << std::endl;
  }

  bool btn_u_rising = joy_data->btn_u && !prev_btn_u;
  bool btn_d_rising = joy_data->btn_d && !prev_btn_d;
  bool btn_l_rising = joy_data->btn_l && !prev_btn_l;
  bool btn_r_rising = joy_data->btn_r && !prev_btn_r;

  prev_btn_u = joy_data->btn_u;
  prev_btn_d = joy_data->btn_d;
  prev_btn_l = joy_data->btn_l;
  prev_btn_r = joy_data->btn_r;

  if (btn_u_rising) {
    target_left_deg = curr_left_pos + (10.0 * degreesPerInch);
    target_right_deg = curr_right_pos + (10.0 * degreesPerInch);
    is_auton_moving  = true;
  }
  else if (btn_d_rising) {
    target_left_deg = curr_left_pos - (10.0 * degreesPerInch);
    target_right_deg = curr_right_pos - (10.0 * degreesPerInch);
    is_auton_moving = true;
  }
  else if (btn_l_rising) {
    target_left_deg = curr_left_pos - turnNinetyDegreesDegrees;
    target_right_deg = curr_right_pos + turnNinetyDegreesDegrees;
    is_auton_moving = true;
  }
  else if (btn_r_rising) {
    target_left_deg = curr_left_pos + turnNinetyDegreesDegrees;
    target_right_deg = curr_right_pos - turnNinetyDegreesDegrees;
    is_auton_moving = true;
  }

  double left_wheel_power = 0.0;
  double right_wheel_power = 0.0;

  bool joystick_active = (std::abs(joy_data->left_y)  > 10) || (std::abs(joy_data->right_y) > 10) || (std::abs(joy_data->right_x) > 10);

  if (joystick_active) {
    is_auton_moving = false;
  }

  if (is_auton_moving) {
    double err_left = target_left_deg - curr_left_pos;
    double err_right = target_right_deg - curr_right_pos;

    if (std::abs(err_left) < 1.0 && std::abs(err_right) < 1.0) {
      is_auton_moving = false;
    } 
    else {
      left_wheel_power = (Kp * err_left) - (Kd * curr_left_vel);
      right_wheel_power = (Kp * err_right) - (Kd * curr_right_vel);

      left_wheel_power  = std::clamp(left_wheel_power,  -1.0, 1.0);
      right_wheel_power = std::clamp(right_wheel_power, -1.0, 1.0);
    }
  } 
  
  if (!is_auton_moving) {
    if (currentDriveMode == DriveMode::TankDrive) {
      left_wheel_power = joy_data->left_y / 127.0;
      right_wheel_power = joy_data->right_y / 127.0;
    } 
    else if (currentDriveMode == DriveMode::ArcadeDrive) {
      double forward = joy_data->left_y / 127.0;
      double turn = joy_data->right_x / 127.0;

      left_wheel_power = forward + turn;
      right_wheel_power = forward - turn;

      double max_mag = std::max(std::abs(left_wheel_power), std::abs(right_wheel_power));
      if (max_mag > 1.0) {
        left_wheel_power /= max_mag;
        right_wheel_power /= max_mag;
      }
    }
  }

  if (std::abs(left_wheel_power) > 0.01 || std::abs(right_wheel_power) > 0.01) {
    rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 2500.0);
    rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 2500.0);

    rhi_ptr_->setMotorVoltageCommandPercent("left_motor", left_wheel_power);
    rhi_ptr_->setMotorVoltageCommandPercent("right_motor", right_wheel_power);
  } 
  else {
    rhi_ptr_->setMotorVoltageCommandPercent("left_motor", 0.0);
    rhi_ptr_->setMotorVoltageCommandPercent("right_motor", 0.0);

    rhi_ptr_->setMotorCurrentLimitMilliAmps("left_motor", 0.0);
    rhi_ptr_->setMotorCurrentLimitMilliAmps("right_motor", 0.0);
  }
}
} // namespace ghost_example_robot

PLUGINLIB_EXPORT_CLASS(
  ghost_example_robot::GhostExampleRobot,
  ghost_ros_interfaces::V5RobotBase)
