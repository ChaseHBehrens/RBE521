import rclpy
from rclpy.node import Node
from ros2_unitree_legged_msgs.msg import MotorCmd

class MyMotorControl(Node):
    def __init__(self):
        super().__init__('my_motor_control')

        topics = [
            'FR_hip', 'FR_thigh', 'FR_calf',
            'FL_hip', 'FL_thigh', 'FL_calf',
            'RR_hip', 'RR_thigh', 'RR_calf',
            'RL_hip', 'RL_thigh', 'RL_calf',
        ]

        self.pubs = {
            name: self.create_publisher(MotorCmd, f'/{name}_controller/command', 1)
            for name in topics
        }

        self.timer = self.create_timer(0.002, self.send_cmd)  # 500 Hz

    def send_cmd(self):
        cmd = MotorCmd()
        cmd.mode = 0x0A  # position control
        cmd.kp   = 20.0  # stiffness - start LOW (5.0) for safety
        cmd.kd   = 0.5   # damping
        cmd.tau  = 0.0

        cmd.q = 0.0
        for name in ('FR_hip', 'FL_hip', 'RR_hip', 'RL_hip'):
            self.pubs[name].publish(cmd)

        cmd.q = 0.8
        for name in ('FR_thigh', 'FL_thigh', 'RR_thigh', 'RL_thigh'):
            self.pubs[name].publish(cmd)

        cmd.q = -1.6
        for name in ('FR_calf', 'FL_calf', 'RR_calf', 'RL_calf'):
            self.pubs[name].publish(cmd)


def main(args=None):
    rclpy.init(args=args)
    node = MyMotorControl()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
