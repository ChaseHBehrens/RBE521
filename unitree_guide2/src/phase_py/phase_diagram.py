#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from std_msgs.msg import String
import matplotlib.pyplot as plt
import numpy as np

import matplotlib.animation as animation
from functools import partial
from matplotlib.axes import Axes
import os

from ros2_unitree_legged_msgs.msg import GaitCmd

class GaitSubscriber(Node):

    def __init__(self):
        super().__init__('phase_diag_node')
        self.subscription = self.create_subscription(
            GaitCmd,
            'gait_out',
            self.listener_callback,
            10)

        self.get_logger().info('init')
        self.subscription  # prevent unused variable warning
        self.phase_data = []
        self.y_tot = []
        self.n = 4 # num legs
        
        plt.ion()
        self.fig, self.ax = plt.subplots()
        self.ax = init_phase_diagram(self.ax, self.n)

    def listener_callback(self, msg):
        beta = msg.beta
        rel_phases = [msg.b.l1, msg.b.l2, msg.b.l3, msg.b.l4]
    
        xs = []
        ys = []
    
        for i in range(self.n):
            wrap = rel_phases[i] + beta - 1
            if wrap > 0:
                x = [(rel_phases[i], 1 - rel_phases[i]), (0, wrap)]
            else:
                x = [(rel_phases[i], beta)]
            y = (self.n-i-1.5, 1)
            xs.append(x)
            ys.append(y)
        
        self.phase_data.append(xs)
        self.y_tot.append(ys)
    
        self.get_logger().info('running')
        latest = len(self.phase_data) - 1
        self.ax = update(latest, self.ax, self.phase_data, self.y_tot, self.n)
        
        self.fig.canvas.draw()
        self.fig.canvas.flush_events()



def test_data_collection(n:int):
    """
    Simulates collecting relative phase data from a ROS2 node
    Args:
        n(int): number of legs"""
    phase_data = []
    y_tot = []
    for _ in range(5):
        beta = 0.4
        rel_phases = np.random.uniform(0, 1, n)
        xs = []
        ys = []

        for i in range(n):
            wrap = rel_phases[i] + beta - 1
            # Bug 2 fixed: same swap here
            if wrap > 0:
                x = [(rel_phases[i], 1 - rel_phases[i]), (0, wrap)]
            else:
                x = [(rel_phases[i], beta)]
            y = (n-i-1.5, 1)
            xs.append(x)
            ys.append(y)
            
        phase_data.append(xs)
        y_tot.append(ys)

    return phase_data, y_tot

def init_phase_diagram(ax:Axes, n:int):
    """Formating for kinematic phase diagram"""

    low = 0.5 
    high = n-low
    ax.set_yticks(range(n), labels=range(1, n+1))
    ax.set_yticks(np.arange(low, high, 1), minor=True)
    ax.invert_yaxis()
    ax.set_xlim(0, 1)
    ax.set_ylim(-low, high)
    ax.set_ylabel("Leg Number")
    ax.set_xlabel("Kinematic Phase")
    ax.set_title("Kinematic Phase Diagram")
    ax.grid(visible=True, which="minor")
    return ax
    

def update(frame:int, ax:Axes, x:list, y:list, n:int):
    """Update the plot based on the current frame"""
    ax.cla()
    # Bug 4 fixed: hardcoded range(4) replaced with range(n)
    for i in range(n):
        ax.broken_barh(x[frame][i], y[frame][i])

    ax = init_phase_diagram(ax, n)
    return ax


def main(args=None):
    rclpy.init(args=args)

    gait_tracker = GaitSubscriber()

    rclpy.spin(gait_tracker)

    # Bug 3 fixed: gait_tracker.y -> gait_tracker.y_tot
    phase_data, y = gait_tracker.phase_data, gait_tracker.y_tot
    gait_tracker.destroy_node()
    rclpy.shutdown()
    return phase_data, y


if __name__ == '__main__':
    n = 4 # legs
    phase_data, y = main()
    # phase_data, y = test_data_collection(n)
    
    fig, ax = plt.subplots()
    ax = init_phase_diagram(ax, n)
    l = len(phase_data)

    # Bug 5 fixed: blit=True -> blit=False (update() doesn't return artists)
    ani = animation.FuncAnimation(fig, partial(update, ax=ax, x=phase_data, y=y, n=n), frames=l, interval=30, blit=False, repeat=False)

    fp = "/media/phase_diagram.gif"

    try:
        if os.path.exists(fp):
            os.chmod(fp, 0o666)
            print("File permissions modified successfully!")
            ani.save(filename=fp, writer="pillow")
        else:
            print("File not found:", fp)
    except PermissionError:
        print("Permission denied: You don't have the necessary permissions to change the permissions of this file.")

    plt.show()
