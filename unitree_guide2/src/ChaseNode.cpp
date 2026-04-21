#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include "common/mathTypes.h"

using namespace std::chrono_literals;

struct Gait {
    Vec4 bias;
    double beta;
    double period;
};

class GaitNode : public rclcpp::Node {
public:
    GaitNode() : Node("gait_node") {
        publisher_ = this->create_publisher<your_package::msg::Gait>("gait_out", 10);

        subscription_ = this->create_subscription<your_package::msg::Gait>(
            "gait_cmd", 10,
            std::bind(&GaitNode::on_gait_command, this, std::placeholders::_1)
        );

        timer_ = this->create_wall_timer(
            5ms,
            std::bind(&GaitNode::timer_callback, this)
        );
    }

private:
    void on_gait_command(const your_package::msg::Gait & msg) {
        initial = curr;

        //extract data from msg
        target.bias << msg.bias[0], msg.bias[1], msg.bias[2], msg.bias[3];
        target.beta = msg.beta;
        target.period = msg.period;

        bias_step = (target.bias - initial.bias) / n;
        beta_step = (target.beta - initial.beta) / n;
        period_step = (target.period - curr.period) / n;
        i = 0;
    }

    // Fires every 5ms — advances interpolation and publishes current gait
    void timer_callback() {
        if (i < n) {
            curr.bias += bias_step;
            curr.beta += beta_step;
            curr.period += period_step;
            i++;
        }
        // publish gait msg
        auto msg = your_package::msg::Gait();
        msg.bias   = {curr.bias[0], curr.bias[1], curr.bias[2], curr.bias[3]};
        msg.beta   = curr.beta;
        msg.period = curr.period;
        publisher_->publish(msg);
    }

    int i  = 0;
    int n  = 100;

    Gait target{};
    Gait curr{};
    Gait initial{};

    Vec4   bias_step = Vec4::Zero();
    double beta_step = 0.0;
    double period_step = 0.0;

    rclcpp::Publisher<your_package::msg::Gait>::SharedPtr publisher_;
    rclcpp::Subscription<your_package::msg::Gait>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GaitNode>());
    rclcpp::shutdown();
    return 0;
}
