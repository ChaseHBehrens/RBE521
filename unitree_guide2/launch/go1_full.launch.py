import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():

    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('go1_gazebo'),
                'launch',
                'spawn_go1_gz.launch.py'
            )
        )
    )

    gait_node = TimerAction(
        period=45.0,
        actions=[
            Node(
                package='unitree_guide2',
                executable='gait_node',
                name='gait_node',
                output='screen',
            )
        ]
    )


    phase_diag_node = TimerAction(
        period=45.5,
        actions=[
            Node(
                package='unitree_guide2',
                executable='phase_diagram.py',
                name='phase_diag_node',
                output='screen',
            )
        ]
    )

    #junior_ctrl = TimerAction(
    #    period=46.0,
    #    actions=[
    #        Node(
    #            package='unitree_guide2',
    #            executable='junior_ctrl',
    #            name='junior_ctrl',
    #            output='screen',
    #            prefix="alacritty --hold -e",
    #        )
    #    ]
    #)

    return LaunchDescription([
        gazebo_launch,
        gait_node,
        phase_diag_node,
        #junior_ctrl,
    ])
