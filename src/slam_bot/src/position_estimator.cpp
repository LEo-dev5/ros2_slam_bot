#include <rclcpp/rclcpp.hpp>

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("position_estimator");
  RCLCPP_INFO(node->get_logger(), "Position Estimator Node Started!");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}