#ifndef RM_CHASSIS__NODE_HPP_
#define RM_CHASSIS__NODE_HPP_
#include<memory>
#include<string>
#include"geometry_msgs/msg/twist.hpp"
#include"nav_msgs/msg/odometry.hpp"
#include"rclcpp/rclcpp.hpp"
#include"tf2_ros/transform_broadcaster.h"
namespace rm_chassis{
    class ChassisNode:public rclcpp::Node
{
public:
explicit ChassisNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
private:
void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
void updateAndPublish();
void publishOdometry(const rclcpp::Time & stamp);

std::string odom_frame_;
std::string base_frame_;
double publish_rate_;

double x_,y_,yaw_,vx_,vy_,wz_;
rclcpp::Time last_time_;

rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr
cmd_vel_sub_;
rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr
odom_pub_;
std::unique_ptr<tf2_ros::TransformBroadcaster>
tf_broadcaster_;
rclcpp::TimerBase::SharedPtr timer_;
};
}
#endif