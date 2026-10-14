## 1 
[ROS2节点开发实战：从Hello World到完整功能模块](https://mp.weixin.qq.com/s/S9S2xoVkDc-B3-eX3ZThnw)

```
cd /home/leixian/workspaces/porject/ros2_projects
source /opt/ros/humble/setup.bash
colcon build --symlink-install 
source install/setup.bash 

# 完整模块（读取 config/params.yaml）
ros2 launch my_robot_controller controller.launch.py 

# 动态调参 
ros2 param set /controller max_speed 2.0 

# 各步骤节点 

ros2 run my_robot_controller minimal_node
ros2 run my_robot_controller parameter_node
ros2 run my_robot_controller lifecycle_driver_node  
# 另开：ros2 lifecycle set /lifecycle_driver_node activate 

# 组件动态加载 
ros2 run rclcpp_components component_container
ros2 component load /ComponentManager my_robot_controller my_robot_controller::MyComponent
```