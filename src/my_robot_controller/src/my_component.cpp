// 步骤4：组件化开发
// 组件不写 main，通过 RCLCPP_COMPONENTS_REGISTER_NODE 注册，
// 运行时可由组件容器（component_container）动态加载/卸载。
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"

namespace my_robot_controller
{

class MyComponent : public rclcpp::Node
{
public:
  explicit MyComponent(const rclcpp::NodeOptions & options)
  : Node("my_component", options)
  {
    sub_ = this->create_subscription<std_msgs::msg::String>(
      "input", 10,
      [this](const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());
      });
    RCLCPP_INFO(this->get_logger(), "MyComponent loaded");
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};

}  // namespace my_robot_controller

RCLCPP_COMPONENTS_REGISTER_NODE(my_robot_controller::MyComponent)