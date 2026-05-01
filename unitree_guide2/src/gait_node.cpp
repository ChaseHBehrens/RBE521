#include <rclcpp/rclcpp.hpp>
#include "common/mathTypes.h"
#include <ros2_unitree_legged_msgs/msg/gait_cmd.hpp>
#include <ros2_unitree_legged_msgs/msg/bias.hpp>
using namespace std::chrono_literals;

struct Gait {
    double beta;
    double period;
    Vec4 bias;
};

class GaitNode : public rclcpp::Node {
public:
    GaitNode() : Node("gait_node") {
        publisher_ = this->create_publisher<ros2_unitree_legged_msgs::msg::GaitCmd>("gait_out", 10);
        subscription_ = this->create_subscription<ros2_unitree_legged_msgs::msg::GaitCmd>(
            "gait_cmd", 10,
            std::bind(&GaitNode::on_gait_command, this, std::placeholders::_1)
        );
        timer_ = this->create_wall_timer(
            5ms,
            std::bind(&GaitNode::timer_callback, this)
        );
    }

private:
    void on_gait_command(const ros2_unitree_legged_msgs::msg::GaitCmd & msg) {
        initial = curr;
        target.bias << msg.b.l1, msg.b.l2, msg.b.l3, msg.b.l4;
        target.beta = msg.beta;
        target.period = msg.period;
        bias_step   = (target.bias   - initial.bias)   / n;
        beta_step   = (target.beta   - initial.beta)   / n;
        period_step = (target.period - initial.period)  / n;
        i = 0;
    }

    void timer_callback() {
        if (i < n) {
            curr.bias   += bias_step;
            curr.beta   += beta_step;
            curr.period += period_step;
            i++;
        }
        auto msg = ros2_unitree_legged_msgs::msg::GaitCmd();
        msg.b.l1 = curr.bias[0];
        msg.b.l2 = curr.bias[1];
        msg.b.l3 = curr.bias[2];
        msg.b.l4 = curr.bias[3];
        msg.beta   = curr.beta;
        msg.period = curr.period;
        publisher_->publish(msg);
    }

    int i  = 0;
    int n  = 1000;
    Gait target {0.5, 1,  Vec4(0, 0.5, 0.5, 0)};
    Gait curr {0.5, 1,  Vec4(0, 0.5, 0.5, 0)};
    Gait initial {0.5, 1,  Vec4(0, 0.5, 0.5, 0)};
    Vec4   bias_step   = Vec4::Zero();
    double beta_step   = 0.0;
    double period_step = 0.0;

    rclcpp::Publisher<ros2_unitree_legged_msgs::msg::GaitCmd>::SharedPtr    publisher_;
    rclcpp::Subscription<ros2_unitree_legged_msgs::msg::GaitCmd>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GaitNode>());
    rclcpp::shutdown();
    return 0;
}
