// 步骤2：参数声明、读取与动态回调
// declare_parameter / get_parameter / add_on_set_parameters_callback
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"

using namespace std::chrono_literals;

class ParameterNode : public rclcpp::Node
{
public:
  ParameterNode()
  : Node("parameter_node")
  {
    this->declare_parameter<std::string>("robot_name", "default_robot");
    this->declare_parameter<int>("update_rate", 100);
    this->declare_parameter<double>("max_speed", 1.0);

    this->get_parameter("robot_name", robot_name_);
    this->get_parameter("update_rate", update_rate_);
    this->get_parameter("max_speed", max_speed_);

    RCLCPP_INFO(
      this->get_logger(), "Robot: %s, Rate: %d, Max speed: %.2f",
      robot_name_.c_str(), update_rate_, max_speed_);

    param_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&ParameterNode::param_callback, this, std::placeholders::_1));

    // 周期性打印当前参数，方便运行时通过 ros2 param set 观察改动生效
    timer_ = this->create_wall_timer(2000ms, std::bind(&ParameterNode::report, this));
  }

private:
  rcl_interfaces::msg::SetParametersResult param_callback(
    const std::vector<rclcpp::Parameter> & params)
  {
    auto result = rcl_interfaces::msg::SetParametersResult();
    result.successful = true;
    for (const auto & param : params) {
      if (param.get_name() == "max_speed") {
        max_speed_ = param.as_double();
        RCLCPP_INFO(this->get_logger(), "Max speed updated to: %.2f", max_speed_);
      } else if (param.get_name() == "robot_name") {
        robot_name_ = param.as_string();
        RCLCPP_INFO(this->get_logger(), "Robot name updated to: %s", robot_name_.c_str());
      } else if (param.get_name() == "update_rate") {
        update_rate_ = static_cast<int>(param.as_int());
        RCLCPP_INFO(this->get_logger(), "Update rate updated to: %d", update_rate_);
      }
    }
    return result;
  }

  void report()
  {
    RCLCPP_INFO(
      this->get_logger(), "[param] robot_name=%s update_rate=%d max_speed=%.2f",
      robot_name_.c_str(), update_rate_, max_speed_);
  }

  std::string robot_name_;
  int update_rate_;
  double max_speed_;
  rclcpp::TimerBase::SharedPtr timer_;
  OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParameterNode>());
  rclcpp::shutdown();
  return 0;
}