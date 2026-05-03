/**********************************************************************
 Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/
#include "interface/KeyBoard.h"
#include <iostream>

#ifdef RUN_ROS
// KeyBoard::KeyBoard(rclcpp::Node::SharedPtr node) : _node(node) {
KeyBoard::KeyBoard(ros::NodeHandle node) : _node(node) {
    // _gaitPub = _node->create_publisher<ros2_unitree_legged_msgs::msg::GaitCmd>("gait_cmd", 10);
    _gaitPub = _node->create_publisher<unitree_guide::msg::GaitCmd>("gait_cmd", 10);
    userCmd = UserCommand::NONE;
    userValue.setZero();
    tcgetattr(fileno(stdin), &_oldSettings);
    _newSettings = _oldSettings;
    _newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &_newSettings);
    pthread_create(&_tid, NULL, runKeyBoard, (void*)this);
}

void KeyBoard::publishGaitCmd(
    double period, 
    double beta, 
    double b1, 
    double b2, 
    double b3, 
    double b4,
    const std::string& name
) {
    std::cout << "[GAIT] Switching to: " << name 
              << " (period=" << period 
              << ", beta=" << beta << ")" << std::endl;
    // auto msg = ros2_unitree_legged_msgs::msg::GaitCmd();
    auto msg = unitree_guide::msg::GaitCmd();
    msg.period = period;
    msg.beta   = beta;
    msg.b.l1   = b1;
    msg.b.l2   = b2;
    msg.b.l3   = b3;
    msg.b.l4   = b4;
    _gaitPub->publish(msg);
}
#else
KeyBoard::KeyBoard(){
    userCmd = UserCommand::NONE;
    userValue.setZero();

    tcgetattr(fileno(stdin), &_oldSettings);
    _newSettings = _oldSettings;
    _newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &_newSettings);

    pthread_create(&_tid, NULL, runKeyBoard, (void*)this);
}
#endif

KeyBoard::~KeyBoard(){
    pthread_cancel(_tid);
    pthread_join(_tid, NULL);
    tcsetattr(fileno(stdin), TCSANOW, &_oldSettings);
}

UserCommand KeyBoard::checkCmd(){
    switch (_c){
    case '1':
        return UserCommand::L2_B;
    case '2':
        return UserCommand::L2_A;
    case '3':
        return UserCommand::L2_X;
    case '4':
        return UserCommand::START;
#ifdef COMPILE_WITH_MOVE_BASE
    case '5':
        return UserCommand::L2_Y;
#endif  // COMPILE_WITH_MOVE_BASE
    case '0':
        return UserCommand::L1_X;
    case '9':
        return UserCommand::L1_A;
    case '8':
        return UserCommand::L1_Y;
#ifdef RUN_ROS
    case 't':
        publishGaitCmd(0.6, 0.8, 0, 0.5, 0.5, 0, "Trot");
        return UserCommand::NONE;
    case 'p': 
        publishGaitCmd(0.6, 0.8, 0, 0.5, 0, 0.5, "Pace");
        return UserCommand::NONE;
    case 'y': 
        publishGaitCmd(0.6, 0.8, 0, 0, 0.5, 0.5, "Bound");
        return UserCommand::NONE;
    case 'g': 
        publishGaitCmd(0.6, 0.45, 0, 0.33, 0.67, 0, "Canter");
        return UserCommand::NONE;
    case 'h': 
        publishGaitCmd(0.6, 0.8, 0, 0.5, 0.75, 0.25, "Walk");
        return UserCommand::NONE;
    case 'u': 
        publishGaitCmd(0.6, 0.5, 0, 0.6, 0.8, 0.3, "Amble");
        return UserCommand::NONE;
    case 'o': 
        publishGaitCmd(0.6, 0.3, 0, 0.25, 0.7, 0.75, "Gallop");
        return UserCommand::NONE;
#endif
    case ' ':
        userValue.setZero();
        return UserCommand::NONE;
    default:
        return UserCommand::NONE;
    }
}

void KeyBoard::changeValue(){
    switch (_c){
    case 'w':case 'W':
        userValue.ly = min<float>(userValue.ly+sensitivityLeft, 1.0);
        break;
    case 's':case 'S':
        userValue.ly = max<float>(userValue.ly-sensitivityLeft, -1.0);
        break;
    case 'd':case 'D':
        userValue.lx = min<float>(userValue.lx+sensitivityLeft, 1.0);
        break;
    case 'a':case 'A':
        userValue.lx = max<float>(userValue.lx-sensitivityLeft, -1.0);
        break;

    case 'i':case 'I':
        userValue.ry = min<float>(userValue.ry+sensitivityRight, 1.0);
        break;
    case 'k':case 'K':
        userValue.ry = max<float>(userValue.ry-sensitivityRight, -1.0);
        break;
    case 'l':case 'L':
        userValue.rx = min<float>(userValue.rx+sensitivityRight, 1.0);
        break;
    case 'j':case 'J':
        userValue.rx = max<float>(userValue.rx-sensitivityRight, -1.0);
        break;
    default:
        break;
    }
}

void* KeyBoard::runKeyBoard(void *arg){
    ((KeyBoard*)arg)->run(NULL);
    return NULL;
}

void* KeyBoard::run(void *arg){
    while(1){
        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);
        res = select(fileno(stdin)+1, &set, NULL, NULL, NULL);

        if(res > 0){
            ret = read( fileno( stdin ), &_c, 1 );
            userCmd = checkCmd();
            if(userCmd == UserCommand::NONE)
                changeValue();
            _c = '\0';
        }
        usleep(1000);
    }
    return NULL;
}