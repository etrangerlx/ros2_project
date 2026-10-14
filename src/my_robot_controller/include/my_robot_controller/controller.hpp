#ifndef MY_ROBOT_CONTROLLER__CONTROLLER_HPP_
#define MY_ROBOT_CONTROLLER__CONTROLLER_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "std_msgs/msg/string.hpp"

namespace my_robot_controller
{

/// 完整功能模块：一个把「发布者 + 订阅者 + 定时器 + 参数」组合起来的控制器节点。
class Controller : public rclcpp::Node
{
public:
  explicit Controller(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
  void declare_parameters();
  void load_parameters();
  void start_timer();

  rcl_interfaces::msg::SetParametersResult on_parameter_change(
    const std::vector<rclcpp::Parameter> & parameters);

  void command_callback(const std_msgs::msg::String::SharedPtr msg);
  void timer_callback();
  void publish_status();

  // 参数
  std::string robot_name_;
  int update_rate_;
  double max_speed_;
  bool verbose_;

  // 运行状态
  std::size_t count_;
  double current_speed_;
  std::string last_command_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr command_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
};

}  // namespace my_robot_controller

#endif  // MY_ROBOT_CONTROLLER__CONTROLLER_HPP_