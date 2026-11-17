import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    use_rviz = LaunchConfiguration("use_rviz")
    rviz_config = os.path.join(
        get_package_share_directory("rm_bringup"), "rviz", "task3.rviz")

    return LaunchDescription([
        DeclareLaunchArgument(
            "use_rviz",
            default_value="true",
            description="是否启动 RViz2"),

        Node(
            package="rm_chassis",
            executable="chassis_node",
            name="chassis_node",
            output="screen",
            parameters=[{
                "publish_rate": 50.0,
                "odom_frame": "odom",
                "base_frame": "base_link",
            }],
        ),

        Node(
            package="rm_referee_mock",
            executable="referee_mock_node",
            name="referee_mock_node",
            output="screen",
            parameters=[{
                "max_hp": 400,
                "max_ammo": 300,
                "initial_hp": 120,
                "auto_start_delay": 5.0,
                "ammo_consume_per_sec": 0.0,
            }],
        ),

        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            arguments=["-d", rviz_config],
            condition=IfCondition(use_rviz),
            output="screen",
        ),
    ])