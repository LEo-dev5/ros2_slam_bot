#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <Eigen/Dense>

class KalmanFilter : public rclcpp::Node
{
public:
  KalmanFilter() : Node("kalman_filter")
  {
    estimated_pos_ << 0.0, 0.0;
    P_ = Eigen::Matrix2d::Identity() * 0.1;
    Q_ = Eigen::Matrix2d::Identity() * 0.3;
    R_ = Eigen::Matrix2d::Identity() * 0.5;

    subscriber_ = this->create_subscription<geometry_msgs::msg::Point>(
      "/sensor_data", 10,
      std::bind(&KalmanFilter::update, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Kalman Filter Node Started!");
  }

private:
  void update(const geometry_msgs::msg::Point::SharedPtr msg)
  {
    // Predict
    P_ = P_ + Q_;

    // Update
    Eigen::Matrix2d K = P_ * (P_ + R_).inverse();
    Eigen::Vector2d z(msg->x, msg->y);
    estimated_pos_ = estimated_pos_ + K * (z - estimated_pos_);
    P_ = (Eigen::Matrix2d::Identity() - K) * P_;

    RCLCPP_INFO(this->get_logger(), "Estimated: (%.3f, %.3f)", 
      estimated_pos_.x(), estimated_pos_.y());
  }

  rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscriber_;
  Eigen::Vector2d estimated_pos_;
  Eigen::Matrix2d P_, Q_, R_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<KalmanFilter>());
  rclcpp::shutdown();
  return 0;
}