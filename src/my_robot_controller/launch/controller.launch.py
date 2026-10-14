# launch/controller.launch.py
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    params_file = os.path.join(
        get_package_share_directory('my_robot_controller'),
        'config',
        'params.yaml'
    )

    use_sim_time = LaunchConfiguration('use_sim_time')

    controller_node = Node(
        package='my_robot_controller',
        executable='controller_node',
        name='controller',
        parameters=[params_file, {'use_sim_time': use_sim_time}],
        remappings=[('status', 'diffbot/status')],
        output='screen'
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use simulation (Gazebo) clock if true'
        ),
        controller_node,
    ])