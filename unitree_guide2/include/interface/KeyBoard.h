/**********************************************************************
 Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "interface/CmdPanel.h"
#include "common/mathTools.h"

#ifdef RUN_ROS
#include <rclcpp/rclcpp.hpp>
#include <ros2_unitree_legged_msgs/msg/gait_cmd.hpp>
#endif

class KeyBoard : public CmdPanel{
public:
#ifdef RUN_ROS
    KeyBoard(rclcpp::Node::SharedPtr node);
#else
    KeyBoard();
#endif
    ~KeyBoard();
private:
    static void* runKeyBoard(void *arg);
    void* run(void *arg);
    UserCommand checkCmd();
    void changeValue();

    pthread_t _tid;
    float sensitivityLeft = 0.05;
    float sensitivityRight = 0.05;
    struct termios _oldSettings, _newSettings;
    fd_set set;
    int res;
    int ret;
    char _c;

#ifdef RUN_ROS
    rclcpp::Node::SharedPtr _node;
    rclcpp::Publisher<ros2_unitree_legged_msgs::msg::GaitCmd>::SharedPtr _gaitPub;
    void publishGaitCmd(double period, double beta, double b1, double b2, double b3, double b4);
#endif
};

#endif
