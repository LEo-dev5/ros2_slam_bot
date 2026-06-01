import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class HelloNode(Node):
    def __init__(self):
        super().__init__('hello_node')
        self.publisher = self.create_publisher(String, '/greeting', 10)
        self.timer = self.create_timer(1.0, self.timer_callback)
        self.count = 0
        self.get_logger().info('노드 시작!')

    def timer_callback(self):
        msg = String()
        msg.data = f'Hello, ROS2! Count: {self.count}'
        self.publisher.publish(msg)
        self.get_logger().info('Hello, ROS2!')
        self.count += 1

def main(args=None):
    rclpy.init(args=args)
    node = HelloNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()