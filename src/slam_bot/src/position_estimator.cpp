#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <random>

class PositionEstimator : public rclcpp::Node
{
  public:
    PositionEstimator() : Node("position_estimator"),gen_(rd_()), dist_(0.0,0.2)
    {
      publisher_ = this->create_publisher<geometry_msgs::msg::Point>("/sensor_data", 10);
      timer_ = this->create_wall_timer(
        std::chrono::seconds(1),
        std::bind(&PositionEstimator::publish_sensor_data, this));
      RCLCPP_INFO(this->get_logger(), "Position Estimator Node Started!");      
    }
  private:
    void publish_sensor_data()
    {
      geometry_msgs::msg::Point msg;
      msg.x = 10.0 + dist_(gen_);
      msg.y = 0.0 + dist_(gen_);
      msg.z = 0.0;
      publisher_->publish(msg);
      RCLCPP_INFO(this->get_logger(), "Sensor Data: (%.3f,%.3f)",msg.x, msg.y);
    }
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::random_device rd_;
    std::default_random_engine gen_;
    std::normal_distribution<double> dist_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PositionEstimator>());
  rclcpp::shutdown();
  return 0;
}