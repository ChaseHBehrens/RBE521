cmake_minimum_required(VERSION 3.8)
project(go1_control)

find_package(ament_python REQUIRED)
find_package(rclpy REQUIRED)
find_package(ros2_unitree_legged_msgs REQUIRED)

add_executable(motor_control src/motor_control.py)
ament_target_dependencies(motor_control rclpy ros2_unitree_legged_msgs)

install(TARGETS motor_control
  DESTINATION lib/${PROJECT_NAME})

ament_package()
