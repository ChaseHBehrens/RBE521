/**********************************************************************
 Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/
#ifdef COMPILE_WITH_MOVE_BASE

#ifndef STATE_MOVE_GAIT_H
#define STATE_MOVE_GAIT_H

#include "FSM/State_GaitTransition.h"
#include "mathTypes.h"
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "message/gaitCmd.h"

class State_move_gait : public State_GaitTransition{
public:
    State_move_gait(CtrlComponents *ctrlComp);
    ~State_move_gait(){}
    FSMStateName checkChange();
private:
    void getUserCmd();
    void initRecv();
    void twistCallback(const geometry_msgs::Twist::msg::SharedPtr msg);
    void gaitCallback(const GaitCmd::msg::SharedPtr msg);
    ros::NodeHandle _nm;
    ros::Subscriber _cmdSub;
    ros::Subscriver _gaitSub;
    double _vx, _vy;
    double _wz;
    double _beta, _period;
    Vec4 _bias;
};

#endif  // STATE_MOVE_BASE_H

#endif  // COMPILE_WITH_MOVE_BASE

#ifdef COMPILE_WITH_ROS2_MB

#ifndef STATE_MOVE_GAIT_H
#define STATE_MOVE_GAIT_H

#include "FSM/State_Trotting.h"
#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include "message/gaitCmd.h"

class State_move_gait : public State_GaitTransition{
public:
    State_move_gait(CtrlComponents *ctrlComp);
    ~State_move_gait(){}
    FSMStateName checkChange();
private:
    void getUserCmd();
    void initRecv();
    void twistCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
    rclcpp::Node::SharedPtr _nm;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr _cmdSub;
    double _vx, _vy;
    double _wz;
    rclcpp::executors::MultiThreadedExecutor::SharedPtr executor;
    std::thread executor_thread;
};

#endif  // STATE_MOVE_BASE_H

#endif  // COMPILE_WITH_ROS2_MB