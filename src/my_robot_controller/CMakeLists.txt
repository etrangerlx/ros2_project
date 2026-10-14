cmake_minimum_required(VERSION 3.8)
project(my_robot_controller)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)

# ---------------------------------------------------------------------------
# 步骤1：最小发布者节点（Hello World）
# ---------------------------------------------------------------------------
add_executable(minimal_node src/minimal_node.cpp)
ament_target_dependencies(minimal_node rclcpp std_msgs)

# ---------------------------------------------------------------------------
# 步骤2：参数声明与动态读取节点
# ---------------------------------------------------------------------------
add_executable(parameter_node src/parameter_node.cpp)
ament_target_dependencies(parameter_node rclcpp rcl_interfaces)

# ---------------------------------------------------------------------------
# 步骤3：生命周期驱动节点
# ---------------------------------------------------------------------------
add_executable(lifecycle_driver_node src/lifecycle_driver_node.cpp)
ament_target_dependencies(lifecycle_driver_node rclcpp rclcpp_lifecycle std_msgs)

# ---------------------------------------------------------------------------
# 完整功能模块：控制器节点（发布 + 订阅 + 定时器 + 参数）
# ---------------------------------------------------------------------------
add_executable(controller_node src/controller.cpp src/main.cpp)
target_include_directories(controller_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(controller_node rclcpp std_msgs rcl_interfaces)

# ---------------------------------------------------------------------------
# 步骤4：组件化开发（可由组件容器动态加载）
# ---------------------------------------------------------------------------
add_library(my_component SHARED src/my_component.cpp)
target_include_directories(my_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(my_component rclcpp rclcpp_components std_msgs)

rclcpp_components_register_node(my_component
  PLUGIN "my_robot_controller::MyComponent"
  EXECUTABLE my_component_node)

# ---------------------------------------------------------------------------
# 安装
# ---------------------------------------------------------------------------
install(TARGETS
    minimal_node
    parameter_node
    lifecycle_driver_node
    controller_node
    my_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_package()