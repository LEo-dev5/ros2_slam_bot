#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <Eigen/Dense>
#include <vector>

class KalmanFilter : public rclcpp::Node
{
public:
  KalmanFilter() : Node("kalman_filter")
  {
    estimated_pos_ << 0.0, 0.0;
    P_ = Eigen::Matrix2d::Identity() * 0.1;
    Q_ = Eigen::Matrix2d::Identity() * 0.3;
    R_ = Eigen::Matrix2d::Identity() * 0.5;

    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("/estimated_pos", 10);
    subscriber_ = this->create_subscription<geometry_msgs::msg::Point>(
      "/sensor_data", 10,
      std::bind(&KalmanFilter::update, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Kalman Filter Node Started!");
  }

private:
 
  void update(const geometry_msgs::msg::Point::SharedPtr msg)
  {
    if (!is_initialized_) {
      estimated_pos_ << msg->x, msg->y;
      is_initialized_ = true;
      return;
    }
    // Predict
    P_ = P_ + Q_;
    
    // Update
    Eigen::Matrix2d K = P_ * (P_ + R_).inverse();
    Eigen::Vector2d z(msg->x, msg->y);
    estimated_pos_ = estimated_pos_ + K * (z - estimated_pos_);
    P_ = (Eigen::Matrix2d::Identity() - K) * P_;

    RCLCPP_INFO(this->get_logger(), "Estimated: (%.3f, %.3f)", 
      estimated_pos_.x(), estimated_pos_.y());
      geometry_msgs::msg::Point est_msg;
      est_msg.x = estimated_pos_.x();
      est_msg.y = estimated_pos_.y();
      est_msg.z = 0.0;
      publisher_->publish(est_msg);
      for (const auto& lm : landmarks_) {
        double dist = (lm - estimated_pos_).norm();
        if (dist < 1.5) {
          Eigen::Matrix2d K_lm = P_ * (P_ + R_).inverse();
          estimated_pos_ = estimated_pos_ + K_lm * (lm - estimated_pos_);
          P_ = (Eigen::Matrix2d::Identity() - K_lm) * P_;
          RCLCPP_INFO(this->get_logger(), "Landmark detected at (%.1f, %.1f)! Position corrected.", lm.x(), lm.y());
        }
      }
  }
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscriber_;
  Eigen::Vector2d estimated_pos_;
  Eigen::Matrix2d P_, Q_, R_;
  bool is_initialized_ = false;
   std::vector<Eigen::Vector2d> landmarks_ = {
  {2.5, 0.0},
  {5.0, 2.5},
  {2.5, 5.0}
  };
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<KalmanFilter>());
  rclcpp::shutdown();
  return 0;
}