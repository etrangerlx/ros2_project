// 步骤3：生命周期节点
// unconfigured -> inactive -> active <-> inactive -> finalized / shutdown
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

using LifecycleState = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using namespace std::chrono_literals;

class LifecycleDriverNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  LifecycleDriverNode()
  : rclcpp_lifecycle::LifecycleNode("lifecycle_driver_node"), count_(0)
  {
    RCLCPP_INFO(get_logger(), "Lifecycle driver created");
  }

  LifecycleState on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Configuring...");
    pub_ = this->create_publisher<std_msgs::msg::String>("output", 10);
    RCLCPP_INFO(get_logger(), "Configured successfully");
    return LifecycleState::SUCCESS;
  }

  LifecycleState on_activate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Activating...");
    pub_->on_activate();
    // 仅在 active 状态下运行定时器，避免无效发布
    timer_ = this->create_wall_timer(
      500ms, std::bind(&LifecycleDriverNode::timer_callback, this));
    RCLCPP_INFO(get_logger(), "Activated successfully");
    return LifecycleState::SUCCESS;
  }

  LifecycleState on_deactivate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Deactivating...");
    timer_.reset();
    pub_->on_deactivate();
    RCLCPP_INFO(get_logger(), "Deactivated successfully");
    return LifecycleState::SUCCESS;
  }

  LifecycleState on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Cleaning up...");
    timer_.reset();
    pub_.reset();
    return LifecycleState::SUCCESS;
  }

  LifecycleState on_shutdown(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Shutting down...");
    timer_.reset();
    pub_.reset();
    return LifecycleState::SUCCESS;
  }

private:
  void timer_callback()
  {
    std_msgs::msg::String message;
    message.data = "Lifecycle heartbeat #" + std::to_string(count_++);
    RCLCPP_INFO(get_logger(), "Publishing: '%s'", message.data.c_str());
    pub_->publish(message);
  }

  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LifecycleDriverNode>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}