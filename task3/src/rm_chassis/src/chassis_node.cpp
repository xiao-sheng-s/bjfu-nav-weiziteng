#include "rm_chassis/chassis_node.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>

#include "geometry_msgs/msg/transform_stamped.hpp"

namespace rm_chassis
{

ChassisNode::ChassisNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("chassis_node", options),
  odom_frame_("odom"),
  base_frame_("base_link"),
  publish_rate_(50.0),
  x_(0.0),
  y_(0.0),
  yaw_(0.0),
  vx_(0.0),
  vy_(0.0),
  wz_(0.0),
  last_time_(this->now())
{
  odom_frame_ = this->declare_parameter<std::string>("odom_frame", odom_frame_);
  base_frame_ = this->declare_parameter<std::string>("base_frame", base_frame_);
  publish_rate_ = this->declare_parameter<double>("publish_rate", publish_rate_);
  publish_rate_ = std::max(1.0, publish_rate_);

  cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "cmd_vel", rclcpp::QoS(10),
    std::bind(&ChassisNode::cmdVelCallback, this, std::placeholders::_1));

  odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", rclcpp::QoS(10));

  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

  const auto period = std::chrono::duration<double>(1.0 / publish_rate_);
  timer_ = this->create_wall_timer(
    std::chrono::duration_cast<std::chrono::nanoseconds>(period),
    std::bind(&ChassisNode::updateAndPublish, this));

  RCLCPP_INFO(this->get_logger(), "ChassisNode 已启动：%s -> %s，%.1f Hz",
    odom_frame_.c_str(), base_frame_.c_str(), publish_rate_);
}

void ChassisNode::cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  vx_ = msg->linear.x;
  vy_ = msg->linear.y;
  wz_ = msg->angular.z;
}

void ChassisNode::updateAndPublish()
{
  const rclcpp::Time now = this->now();

  double dt = (now - last_time_).seconds();
  last_time_ = now;

  if (dt < 0.0) {
    dt = 0.0;
  } else if (dt > 0.2) {
    dt = 0.2;
  }

  const double cy = std::cos(yaw_);
  const double sy = std::sin(yaw_);

  x_   += (vx_ * cy - vy_ * sy) * dt;
  y_   += (vx_ * sy + vy_ * cy) * dt;
  yaw_ += wz_ * dt;

  yaw_ = std::atan2(std::sin(yaw_), std::cos(yaw_));

  publishOdometry(now);
}

void ChassisNode::publishOdometry(const rclcpp::Time & stamp)
{
  const double half = yaw_ * 0.5;
  const double qx = 0.0;
  const double qy = 0.0;
  const double qz = std::sin(half);
  const double qw = std::cos(half);

  nav_msgs::msg::Odometry odom;
  odom.header.stamp = stamp;
  odom.header.frame_id = odom_frame_;
  odom.child_frame_id = base_frame_;

  odom.pose.pose.position.x = x_;
  odom.pose.pose.position.y = y_;
  odom.pose.pose.position.z = 0.0;
  odom.pose.pose.orientation.x = qx;
  odom.pose.pose.orientation.y = qy;
  odom.pose.pose.orientation.z = qz;
  odom.pose.pose.orientation.w = qw;

  odom.twist.twist.linear.x = vx_;
  odom.twist.twist.linear.y = vy_;
  odom.twist.twist.angular.z = wz_;

  odom.pose.covariance[0]  = 0.02;
  odom.pose.covariance[7]  = 0.02;
  odom.pose.covariance[14] = 1e6;
  odom.pose.covariance[21] = 1e6;
  odom.pose.covariance[28] = 1e6;
  odom.pose.covariance[35] = 0.02;

  odom_pub_->publish(odom);

  geometry_msgs::msg::TransformStamped tf_msg;
  tf_msg.header.stamp = stamp;
  tf_msg.header.frame_id = odom_frame_;
  tf_msg.child_frame_id = base_frame_;
  tf_msg.transform.translation.x = x_;
  tf_msg.transform.translation.y = y_;
  tf_msg.transform.translation.z = 0.0;
  tf_msg.transform.rotation.x = qx;
  tf_msg.transform.rotation.y = qy;
  tf_msg.transform.rotation.z = qz;
  tf_msg.transform.rotation.w = qw;

  tf_broadcaster_->sendTransform(tf_msg);
}

}  // namespace rm_chassis