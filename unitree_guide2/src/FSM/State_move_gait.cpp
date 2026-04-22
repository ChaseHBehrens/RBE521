/**********************************************************************
 Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/
#ifdef COMPILE_WITH_MOVE_BASE

#include "FSM/State_move_gait.h"
#include "ros2_unitree_legged_msgs/msg/GaitMsg"

State_move_gait::State_move_gait(CtrlComponents *ctrlComp)
    :State_GaitTransition(ctrlComp){
    _stateName = FSMStateName::MOVE_GAIT;
    _stateNameString = "move_gait";
    initRecv();
    
}

FSMStateName State_move_gait::checkChange(){
    if(_lowState->userCmd == UserCommand::L2_B){
        return FSMStateName::PASSIVE;
    }
    else if(_lowState->userCmd == UserCommand::L2_A){
        return FSMStateName::FIXEDSTAND;
    }
    else{
        return FSMStateName::MOVE_GAIT;
    }
}

void State_move_gait::getUserCmd(){
    setHighCmd(_vx, _vy, _wz);
    ros::spinOnce();
}

void State_move_gait::twistCallback(const geometry_msgs::Twist& msg){
    _vx = msg.linear.x;
    _vy = msg.linear.y;
    _wz = msg.angular.z;
}
void State_move_gait::gaitCallback(const ros2_unitree_legged_msgs::GaitCmd& msg){
    _beta = msg.beta;
    _period = msg.period;
    _bias = msg.bias;
}

void State_move_gait::initRecv(){
    _cmdSub = _nm.subscribe("/cmd_vel", 1, &State_move_gait::twistCallback, this);
    _gaitSub = _nm.subscribe("/cmd_gait", 1, &State_move_gait::gaitCallback, this);
}

#endif  // COMPILE_WITH_MOVE_BASE

#ifdef COMPILE_WITH_ROS2_MB

#include "FSM/State_move_gait.h"

State_move_gait::State_move_gait(CtrlComponents *ctrlComp)
:State_GaitTransition(ctrlComp){
    _stateName = FSMStateName::MOVE_GAIT;
    _stateNameString = "move_gait";
    _nm = rclcpp::Node::make_shared("state_mg");
    auto executor = std::make_shared<rclcpp::executors::MultiThreadedExecutor>(
        rclcpp::ExecutorOptions(), 2
    );
    executor->add_node(_nm);
    executor_thread = std::thread([executor] (){
        executor->spin();
    });
    executor_thread.detach();
    initRecv();
}

FSMStateName State_move_gait::checkChange(){
    if(_lowState->userCmd == UserCommand::L2_B){
        return FSMStateName::PASSIVE;
    }
    else if(_lowState->userCmd == UserCommand::L2_A){
        return FSMStateName::FIXEDSTAND;
    }
    else{
        return FSMStateName::MOVE_GAIT;
    }
}

void State_move_gait::getUserCmd(){
    setHighCmd(_vx, _vy, _wz);
}

void State_move_gait::twistCallback(const geometry_msgs::msg::Twist::SharedPtr msg){
    _vx = msg->linear.x;
    _vy = msg->linear.y;
    _wz = msg->angular.z;
}
void State_move_gait::gaitCallback(const GaitCmd::SharedPtr msg){
    _beta= msg.beta;
    _period = msg.period;
    _bias = Vec4([msg.b.l1, msg.b.l2, msg.b.l3, msg.b.l4]);
}
void State_move_gait::initRecv(){
    std::cout << "Initialized cmd vel sub" << std::endl;
    _cmdSub = _nm->create_subscription<geometry_msgs::msg::Twist>("/cmd_vel", 1, std::bind(&State_move_gait::twistCallback, this, std::placeholders::_1));
    _gaitSub = _nm->create_subscription<GaitCmd>("/cmd_gait", 1, std::bind(&State_move_gait::gaitCallback, this, std::placeholders::_1));
}

#endif  // COMPILE_WITH_ROS2_MB