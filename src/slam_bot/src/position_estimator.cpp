#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <random>
#include <vector>

class PositionEstimator : public rclcpp::Node
{
public:
  PositionEstimator() : Node("position_estimator"), gen_(rd_()), dist_(0.0, 0.2)
  {
    // 이동 경로 정의 (dx, dy)
    waypoints_ = {
      {1.0, 0.0},   // 오른쪽으로
      {1.0, 0.0},
      {1.0, 0.0},
      {1.0, 0.0},
      {1.0, 0.0},
      {0.0, 1.0},   // 위로
      {0.0, 1.0},
      {0.0, 1.0},
      {0.0, 1.0},
      {0.0, 1.0},
      {-1.0, 0.0},  // 왼쪽으로
      {-1.0, 0.0},
      {-1.0, 0.0},
      {-1.0, 0.0},
      {-1.0, 0.0},
    };

    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("/sensor_data", 10);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&PositionEstimator::publish_sensor_data, this));

    RCLCPP_INFO(this->get_logger(), "Position Estimator Node Started!");
  }

private:
  void publish_sensor_data()
  {
    if (step_ >= waypoints_.size()) {
      RCLCPP_INFO(this->get_logger(), "Route complete!");
      return;
    }

    // 이동 + 노이즈
    current_x_ += waypoints_[step_].first + dist_(gen_);
    current_y_ += waypoints_[step_].second + dist_(gen_);
    step_++;

    geometry_msgs::msg::Point msg;
    msg.x = current_x_;
    msg.y = current_y_;
    msg.z = 0.0;
    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Step %zu: (%.3f, %.3f)", step_, msg.x, msg.y);
  }

  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::random_device rd_;
  std::default_random_engine gen_;
  std::normal_distribution<double> dist_;

  std::vector<std::pair<double, double>> waypoints_;
  double current_x_ = 0.0;
  double current_y_ = 0.0;
  size_t step_ = 0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PositionEstimator>());
  rclcpp::shutdown();
  return 0;
}