cmake_minimum_required(VERSION 3.8)
project(go1_control)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ros2_unitree_legged_msgs REQUIRED)

add_executable(motor_control src/motor_control.cpp)
ament_target_dependencies(motor_control rclcpp ros2_unitree_legged_msgs)

install(TARGETS motor_control
  DESTINATION lib/${PROJECT_NAME})

ament_package()
