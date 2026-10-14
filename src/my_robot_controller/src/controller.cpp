#include "my_robot_controller/controller.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace my_robot_controller
{

Controller::Controller(const rclcpp::NodeOptions & options)
: rclcpp::Node("controller", options),
  count_(0),
  current_speed_(0.0),
  last_command_("none")
{
  declare_parameters();
  load_parameters();

  status_pub_ = this->create_publisher<std_msgs::msg::String>("status", 10);

  command_sub_ = this->create_subscription<std_msgs::msg::String>(
    "command", 10,
    std::bind(&Controller::command_callback, this, std::placeholders::_1));

  param_callback_handle_ = this->add_on_set_parameters_callback(
    std::bind(&Controller::on_parameter_change, this, std::placeholders::_1));

  start_timer();

  RCLCPP_INFO(
    get_logger(), "Controller '%s' started (update_rate=%d Hz, max_speed=%.2f m/s)",
    robot_name_.c_str(), update_rate_, max_speed_);
}

void Controller::declare_parameters()
{
  this->declare_parameter<std::string>("robot_name", "diffbot");
  this->declare_parameter<int>("update_rate", 10);
  this->declare_parameter<double>("max_speed", 1.0);
  this->declare_parameter<bool>("verbose", true);
}

void Controller::load_parameters()
{
  this->get_parameter("robot_name", robot_name_);
  this->get_parameter("update_rate", update_rate_);
  this->get_parameter("max_speed", max_speed_);
  this->get_parameter("verbose", verbose_);
}

void Controller::start_timer()
{
  const int rate = std::max(1, update_rate_);
  const auto period = std::chrono::milliseconds(1000 / rate);
  timer_ = this->create_wall_timer(period, std::bind(&Controller::timer_callback, this));
}

rcl_interfaces::msg::SetParametersResult Controller::on_parameter_change(
  const std::vector<rclcpp::Parameter> & parameters)
{
  auto result = rcl_interfaces::msg::SetParametersResult();
  result.successful = true;

  for (const auto & parameter : parameters) {
    const std::string & name = parameter.get_name();

    if (name == "max_speed") {
      if (parameter.as_double() < 0.0) {
        result.successful = false;
        result.reason = "max_speed must be non-negative";
        return result;
      }
      max_speed_ = parameter.as_double();
      RCLCPP_INFO(get_logger(), "max_speed updated to: %.2f m/s", max_speed_);
    } else if (name == "robot_name") {
      robot_name_ = parameter.as_string();
      RCLCPP_INFO(get_logger(), "robot_name updated to: %s", robot_name_.c_str());
    } else if (name == "update_rate") {
      update_rate_ = static_cast<int>(parameter.as_int());
      // 重新创建定时器，使新的频率立即生效
      timer_->cancel();
      start_timer();
      RCLCPP_INFO(get_logger(), "update_rate updated to: %d Hz", update_rate_);
    } else if (name == "verbose") {
      verbose_ = parameter.as_bool();
      RCLCPP_INFO(get_logger(), "verbose updated to: %s", verbose_ ? "true" : "false");
    }
  }

  return result;
}

void Controller::command_callback(const std_msgs::msg::String::SharedPtr msg)
{
  last_command_ = msg->data;
  RCLCPP_INFO(get_logger(), "Received command: '%s'", last_command_.c_str());
}

void Controller::timer_callback()
{
  ++count_;
  // 演示用的简单动力学：速度逐步爬升到 max_speed
  current_speed_ = std::min(max_speed_, current_speed_ + 0.1 * max_speed_);

  publish_status();

  if (verbose_) {
    RCLCPP_INFO(
      get_logger(), "tick=%zu speed=%.2f last_cmd='%s'",
      count_, current_speed_, last_command_.c_str());
  }
}

void Controller::publish_status()
{
  std_msgs::msg::String message;
  message.data = "[" + robot_name_ + "] tick=" + std::to_string(count_) +
    " speed=" + std::to_string(current_speed_);
  status_pub_->publish(message);
}

}  // namespace my_robot_controller