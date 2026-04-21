#include "common/mathTypes.h"

struct Gait {
    Vec4 bias;
    double beta;
    double period;
};

void transition(int n, Gait initial, Gait target) {
    Vec4 biasStepSize = (target.bias - initial.bias) / n;
    double betaStepSize = (target.beta - initial.beta) / n;
    double periodStepSize = (target.period - initial.period) / n;
}

#include <chrono>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

class TimedPublisher : public rclcpp::Node
{
    public:
        TimedPublisher() : Node("timed_publisher"), count_(0) {
            publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
            timer_ = this->create_wall_timer(
                    500ms,
                    std::bind(&TimedPublisher::timer_callback, this)
                    );
        }

    private:
        int i = 0;
        int n = 100; 
        Vec4 biasStepSize;
        double betaStepSize;
        double periodStepSize;
        Gait currGait;

        void timer_callback() {
            if (i < n) {
                currGait.bias += biasStepSize;
                currGait.beta += betaStepSize;
                currGait.period += periodStepSize;
                i++;
            }

            auto message = std_msgs::msg::String();
            message.data = "Hello, ROS2! count: " + std::to_string(count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            publisher_->publish(message);
        }

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        size_t count_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TimedPublisher>());
    rclcpp::shutdown();
    return 0;
}


