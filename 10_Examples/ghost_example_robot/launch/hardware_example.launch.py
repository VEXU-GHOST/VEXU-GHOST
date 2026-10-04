import os
import xacro
from launch import LaunchDescription

from ament_index_python import get_package_share_directory
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    home_dir = os.path.expanduser("~")
    pkg_dir = os.path.join(home_dir, "VEXU_GHOST", "10_Examples", "ghost_example_robot")

    # This contains all the parameters for our ROS nodes
    ros_config_file = os.path.join(pkg_dir, "config/example_ros_config.yaml")

    # This contains all the port and device info that gets compiled on to the V5 Brain
    robot_config_yaml_path = os.path.join(
        pkg_dir, "config/example_hardware_config.yaml"
    )

    # Simple robot model; its fixed joints give Foxglove the base_link -> lidar_link transform
    urdf_path = os.path.join(pkg_dir, "urdf/example_robot.urdf")
    with open(urdf_path, "r") as urdf_file:
        robot_description = urdf_file.read()

    plugin_type = "ghost_example_robot::GhostExampleRobot"
    robot_name = "EXAMPLE_ROBOT"

    ########################
    ### Node Definitions ###
    ########################
    serial_node = Node(
        package="ghost_ros_interfaces",
        executable="jetson_v5_serial_node",
        name="ghost_serial_node",
        output="screen",
        parameters=[
            ros_config_file,
            {"robot_config_yaml_path": robot_config_yaml_path},
        ],
    )

    competition_state_machine_node = Node(
        package="ghost_ros_interfaces",
        executable="competition_state_machine_node",
        output="screen",
        parameters=[
            ros_config_file,
            {
                "robot_config_yaml_path": robot_config_yaml_path,
            },
        ],
        arguments=[plugin_type, robot_name],
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[{"robot_description": robot_description}],
    )

    # Same lidar settings as the competition robots (ghost_override hardware.launch.py)
    rplidar_node = Node(
        package="rplidar_ros",
        executable="rplidar_node",
        name="rplidar_node",
        output="screen",
        parameters=[
            {
                "channel_type": "serial",
                "serial_port": "/dev/ttyUSB0",
                "serial_baudrate": 256000,
                "frame_id": "lidar_link",
                "inverted": False,
                "angle_compensate": True,
            }
        ],
    )

    foxglove_bridge = Node(
        package="foxglove_bridge",
        executable="foxglove_bridge",
        name="foxglove_bridge",
        output="screen",
        parameters=[{"address": "0.0.0.0", "port": 8765}],
    )
    return LaunchDescription(
        [
            serial_node,
            competition_state_machine_node,
            robot_state_publisher,
            rplidar_node,
            foxglove_bridge,
        ]
    )
