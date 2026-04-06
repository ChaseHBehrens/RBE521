#include "rclcpp/rclcpp.hpp"
#include "ros2_unitree_legged_msgs/msg/motor_cmd.hpp"

class MyMotorControl : public rclcpp::Node
{
public:
    MyMotorControl() : Node("my_motor_control")
    {
        // Create publishers for each joint you want to control
        // (or all 12 if you want full control)
        FR_hip_pub_   = create_publisher<Cmd>("/FR_hip_controller/command", 1);
        FR_thigh_pub_ = create_publisher<Cmd>("/FR_thigh_controller/command", 1);
        FR_calf_pub_  = create_publisher<Cmd>("/FR_calf_controller/command", 1);
        // ... repeat for all 12 joints

        timer_ = create_wall_timer(
            std::chrono::milliseconds(2),  // 500Hz
            std::bind(&MyMotorControl::send_cmd, this));
    }

private:
    using Cmd = ros2_unitree_legged_msgs::msg::MotorCmd;

    void send_cmd()
    {
        Cmd cmd;
        cmd.mode = 0x0A;   // position control
        cmd.kp   = 20.0;   // stiffness - start LOW (5.0) for safety
        cmd.kd   = 0.5;    // damping
        cmd.tau  = 0.0;

        // FR hip to 0 radians
        cmd.q = 0.0;
        FR_hip_pub_->publish(cmd);

        // FR thigh to 0.8 rad (~45 deg, standing pose)
        cmd.q = 0.8;
        FR_thigh_pub_->publish(cmd);

        // FR calf to -1.6 rad
        cmd.q = -1.6;
        FR_calf_pub_->publish(cmd);
    }

    rclcpp::Publisher<Cmd>::SharedPtr FR_hip_pub_, FR_thigh_pub_, FR_calf_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyMotorControl>());
    rclcpp::shutdown();
}
