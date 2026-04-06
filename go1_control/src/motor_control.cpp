#include "rclcpp/rclcpp.hpp"
#include "ros2_unitree_legged_msgs/msg/motor_cmd.hpp"

class MyMotorControl : public rclcpp::Node
{
public:
    MyMotorControl() : Node("my_motor_control")
    {
        FR_hip_pub_   = create_publisher<Cmd>("/FR_hip_controller/command", 1);
        FR_thigh_pub_ = create_publisher<Cmd>("/FR_thigh_controller/command", 1);
        FR_calf_pub_  = create_publisher<Cmd>("/FR_calf_controller/command", 1);
        FL_hip_pub_   = create_publisher<Cmd>("/FL_hip_controller/command", 1);
        FL_thigh_pub_ = create_publisher<Cmd>("/FL_thigh_controller/command", 1);
        FL_calf_pub_  = create_publisher<Cmd>("/FL_calf_controller/command", 1);
        RR_hip_pub_   = create_publisher<Cmd>("/RR_hip_controller/command", 1);
        RR_thigh_pub_ = create_publisher<Cmd>("/RR_thigh_controller/command", 1);
        RR_calf_pub_  = create_publisher<Cmd>("/RR_calf_controller/command", 1);
        RL_hip_pub_   = create_publisher<Cmd>("/RL_hip_controller/command", 1);
        RL_thigh_pub_ = create_publisher<Cmd>("/RL_thigh_controller/command", 1);
        RL_calf_pub_  = create_publisher<Cmd>("/RL_calf_controller/command", 1);

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

        cmd.q = 0.0;
        FR_hip_pub_->publish(cmd);
        FL_hip_pub_->publish(cmd);
        RR_hip_pub_->publish(cmd);
        RL_hip_pub_->publish(cmd);

        cmd.q = 0.8;
        FR_thigh_pub_->publish(cmd);
        FL_thigh_pub_->publish(cmd);
        RR_thigh_pub_->publish(cmd);
        RL_thigh_pub_->publish(cmd);

        cmd.q = -1.6;
        FR_calf_pub_->publish(cmd);
        FL_calf_pub_->publish(cmd);
        RR_calf_pub_->publish(cmd);
        RL_calf_pub_->publish(cmd);
    }

    rclcpp::Publisher<Cmd>::SharedPtr FR_hip_pub_, FR_thigh_pub_, FR_calf_pub_;
    rclcpp::Publisher<Cmd>::SharedPtr FL_hip_pub_, FL_thigh_pub_, FL_calf_pub_;
    rclcpp::Publisher<Cmd>::SharedPtr RR_hip_pub_, RR_thigh_pub_, RR_calf_pub_;
    rclcpp::Publisher<Cmd>::SharedPtr RL_hip_pub_, RL_thigh_pub_, RL_calf_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyMotorControl>());
    rclcpp::shutdown();
}
