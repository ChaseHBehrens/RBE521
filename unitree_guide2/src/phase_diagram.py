# import rclpy
# from rclpy.node import Node

# from std_msgs.msg import String
import matplotlib.pyplot as plt
import numpy as np

import matplotlib.animation as animation
from functools import partial
from matplotlib.axes import Axes
import os

"""
class GaitSubscriber(Node):

    def __init__(self):
        super().__init__('cur_gait_subscriber')
        self.subscription = self.create_subscription(
            String,
            'gait_out',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        self.artists = []

    def listener_callback(self, msg):
        beta = msg.beta
        period = msg.period
        rel_phases = msg.b
        n = 4 # number of legs

        phase_data = []
        y = []
        self.fig, self.ax = plt.subplots()
        for i in range(n):
            phase_data.append((rel_phases[i], beta))
            y.append((n-i, 1))
        container = self.ax.broken_barh(phase_data, y)
        self.artists.append(container)
        self.ax.set_yticks(range(4), labels=["1", "2", "3", "4"])
        self.ax.invert_yaxis()   
"""   

def test_data_collection(n:int):
    """
    Simulates collecting relative phase data from a ROS2 node
    Args:
        n(int): number of legs"""
    phase_data = []
    y_tot = []
    # Loop through simulation steps
    for _ in range(5):
        # duty factor and rel phase changes every simulation step
        beta = 0.4
        rel_phases = np.random.uniform(0, 1, n)
        xs = []
        ys = []

        # for each leg
        for i in range(n):
            wrap = rel_phases[i] + beta - 1
            if wrap > 0: 
                x = [(rel_phases[i], beta)]
            else:
                x = [(rel_phases[i], beta), (0, wrap)]
            y = (n-i-1.5, 1)
            xs.append(x)
            ys.append(y)
            
        phase_data.append(xs)
        y_tot.append(ys)
            

    # print(f"phase: {phase_data}")
    # print(f"y: {y}")
    return phase_data, y_tot

def plot_phase_diagram(ax:Axes, n:int):
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
    # Clear prev data and replot kinematic phases based on the current frame
    ax.cla()
    for i in range(4):
        ax.broken_barh(x[frame][i], y[frame][i])

    ax = plot_phase_diagram(ax, n)
    return ax


"""
def main(args=None):
    rclpy.init(args=args)

    gait_tracker = GaitSubscriber()

    rclpy.spin(gait_tracker)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    phase_data, y = gait_tracker.phase_data, gait_tracker.y
    gait_tracker.destroy_node()
    rclpy.shutdown()
    return phase_data, y
"""

if __name__ == '__main__':
    n = 4 # legs
    # phase_data, y = main()
    phase_data, y = test_data_collection(n)
    
    fig, ax = plt.subplots()
    ax = plot_phase_diagram(ax, n)
    l = len(phase_data)

    ani = animation.FuncAnimation(fig, partial(update, ax=ax, x=phase_data, y=y, n=n), frames=l, interval=30, repeat=False)

    # save animation
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


    
    # plt.show()