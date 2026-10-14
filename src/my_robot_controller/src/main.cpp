#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "my_robot_controller/controller.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<my_robot_controller::Controller>());
  rclcpp::shutdown();
  return 0;
}