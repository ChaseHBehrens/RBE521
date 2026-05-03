---

# **Unitree GO1 Simulation with ROS 2 Jazzy Gazebo**

This repository contains a simulation environment for the **Unitree GO1 robot** in **Gazebo Sim** and **ROS 2**, along with an interface for navigation.
The functionality has been tested with **ROS Jazzy** on **Ubuntu 24.04**.

---

## **Dependencies:**

* **[LCM](https://lcm-proj.github.io/lcm/)** – Needs to be built from source
* **[Navigation2](https://github.com/ros-navigation/navigation2)**
* **[ros2_control](https://github.com/ros-controls/ros2_control)**
* **[ros2_controllers](https://github.com/ros-controls/ros2_controllers)**
* **[Gazebo ROS 2 Plugins](https://github.com/ros-simulation/gazebo_ros_pkgs/tree/ros2/gazebo_plugins)**

---

### **1. ROS 2 Jazzy, Gazebo Sim & RViz (if not installed)**

Install **ROS 2**, **RViz2**, and **Gazebo integration packages**. Skip if already installed.

```bash
sudo apt update && sudo apt install -y \
    ros-jazzy-desktop \
    ros-jazzy-rviz2 \
    ros-jazzy-xacro \
    ros-jazzy-colcon-common-extensions

sudo apt install -y \
    ros-jazzy-ros-gz \
    ros-jazzy-ros-gz-sim \
    ros-jazzy-ros-gz-bridge \
    ros-jazzy-ros-gz-image
```

---

### **2. Build & Runtime Dependencies**

#### **2.1 Essential system packages**

```bash
sudo apt install -y \
    build-essential cmake git python3-pip \
    libeigen3-dev libprotobuf-dev protobuf-compiler
```

#### **2.2 Navigation2**

```bash
sudo apt install -y ros-jazzy-navigation2 ros-jazzy-nav2-bringup
```

#### **2.3 ros2_control and ros2_controllers**

```bash
sudo apt install -y \
    ros-jazzy-ros2-control \
    ros-jazzy-ros2-controllers \
    ros-jazzy-controller-manager \
    ros-jazzy-joint-state-broadcaster \
    ros-jazzy-joint-trajectory-controller \
    ros-jazzy-imu-sensor-broadcaster
```

#### **2.4 Gazebo ROS 2 Plugins**

```bash
sudo apt install ros-jazzy-ros-gz ros-jazzy-gz-ros2-control

```

#### **2.5 ROS 2 Message Packages**

```bash
sudo apt install -y \
    ros-jazzy-geometry-msgs \
    ros-jazzy-sensor-msgs \
    ros-jazzy-std-msgs \
    ros-jazzy-nav-msgs
```

#### **2.6 go1_description dependencies**

```bash
sudo apt install -y ros-jazzy-robot-state-publisher
```

#### **2.7 LCM (build from source)**

```bash
cd ~
git clone https://github.com/lcm-proj/lcm.git
cd lcm
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
```

---

## **3. Running the Application**

After installing dependencies clone the repo into the ros2_ws space folder. Then build the package with colcon.
```bash
colcon build --symlink-install
source install/setup.bash
```

Next run the launch file.
```bash
ros2 launch unitree_guide2 go1_full.launch.py
```

In a second terminal window cd into the ros2_ws and run the junior_ctrl node.

```bash
source install/setup.bash
ros2 run unitree_guide2 junior_ctrl
```

To initialize the program press `t`. You should see the phase diagram shift. Next press `2` to move the robot 
into a standing position. Then press `3` to switch to fixed stand. Finally press `4` to switch to the gait transition 
state. Use `w` `a` `s` `d` to control the robot movement. Switch gaits using the following commands. 
- `t` Trot
- `p` Pace
- `y` Bound
- `g` Canter
- `h` Walk
- `u` Amble
- `o` Gallop



