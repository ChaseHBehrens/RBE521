#include "common/mathTypes.h"
#include "common/mathTools.h"
#include <memory>
#ifndef GAITCMD_H
#define GAITCMD_H

struct bias {
    double l1;
    double l2;
    double l3;
    double l4;
};

struct GaitCmd {
    bias b;
    double beta;
    double period;
    using SharedPtr = std::shared_ptr<GaitCmd>;
};

#ifdef RUN_ROS
#include <ros2_unitree_legged_msgs/msg/gait_cmd.hpp>
using GaitCmdMsg = ros2_unitree_legged_msgs::msg::GaitCmd;
#endif

#endif
