#include "rm_referee_mock/referee_mock_node.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <string>

namespace rm_referee_mock
{

RefereeMockNode::RefereeMockNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("referee_mock_node", options),
  max_hp_(400),
  max_ammo_(300),
  publish_rate_(5.0),
  auto_start_delay_(5.0),
  ammo_consume_per_sec_(0.0),
  hp_(400),
  ammo_(300),
  stage_(1),
  ammo_accum_(0.0),
  start_time_(this->now()),
  last_time_(this->now())
{
    const int p_max_hp = static_cast<int>(this->declare_parameter<int>("max_hp", 400));
  max_hp_ = static_cast<uint16_t>(std::max(1, p_max_hp));

  const int p_max_ammo = static_cast<int>(this->declare_parameter<int>("max_ammo", 300));
  max_ammo_ = static_cast<uint16_t>(std::max(0, p_max_ammo));

  publish_rate_ = std::max(0.5, static_cast<double>(
    this->declare_parameter<double>("publish_rate", publish_rate_)));
  auto_start_delay_ = std::max(0.0, static_cast<double>(
    this->declare_parameter<double>("auto_start_delay", auto_start_delay_)));
  ammo_consume_per_sec_ = std::max(0.0, static_cast<double>(
    this->declare_parameter<double>("ammo_consume_per_sec", ammo_consume_per_sec_)));

  const int init_hp = static_cast<int>(
    this->declare_parameter<int>("initial_hp", static_cast<int>(max_hp_)));
  hp_ = static_cast<uint16_t>(std::max(0, std::min(static_cast<int>(max_hp_), init_hp)));
  ammo_ = max_ammo_;

  stage_pub_ = this->create_publisher<std_msgs::msg::UInt16>("referee/stage", rclcpp::QoS(10));
  hp_pub_ = this->create_publisher<std_msgs::msg::UInt16>("referee/hp", rclcpp::QoS(10));
  ammo_pub_ = this->create_publisher<std_msgs::msg::UInt16>("referee/ammo", rclcpp::QoS(10));

  supply_srv_ = this->create_service<std_srvs::srv::Trigger>(
    "supply_hp",
    std::bind(&RefereeMockNode::handleSupplyHp, this,
      std::placeholders::_1, std::placeholders::_2));

  timer_ = this->create_wall_timer(
    std::chrono::milliseconds(static_cast<int64_t>(1000.0 / publish_rate_)),
    std::bind(&RefereeMockNode::publishStatus, this));

  RCLCPP_INFO(this->get_logger(),
    "RefereeMockNode 已启动：血量上限 %u，弹量上限 %u，%.1f Hz",
    static_cast<unsigned>(max_hp_), static_cast<unsigned>(max_ammo_), publish_rate_);
}

void RefereeMockNode::publishStatus()
{
  const rclcpp::Time now = this->now();

  const double elapsed = (now - start_time_).seconds();
  stage_ = (elapsed < auto_start_delay_) ? 1 : 2;

  double dt = (now - last_time_).seconds();
  last_time_ = now;
  if (dt < 0.0) {
    dt = 0.0;
  } else if (dt > 1.0) {
    dt = 1.0;
  }

  if (stage_ == 2 && ammo_consume_per_sec_ > 0.0) {
    ammo_accum_ += ammo_consume_per_sec_ * dt;
    const auto whole = static_cast<uint16_t>(ammo_accum_);
    if (whole > 0U) {
      ammo_accum_ -= static_cast<double>(whole);
      ammo_ = (whole >= ammo_) ? 0 : static_cast<uint16_t>(ammo_ - whole);
    }
  }

  std_msgs::msg::UInt16 msg;
  msg.data = stage_;
  stage_pub_->publish(msg);
  msg.data = hp_;
  hp_pub_->publish(msg);
  msg.data = ammo_;
  ammo_pub_->publish(msg);
}

void RefereeMockNode::handleSupplyHp(
  const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
  std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
  (void)request;

  const uint16_t before = hp_;
  const uint32_t room = static_cast<uint32_t>(max_hp_) - static_cast<uint32_t>(before);
  hp_ = max_hp_;

  response->success = true;
  response->message = "补给完成：回复 " + std::to_string(room) +
    " 点，当前血量 " + std::to_string(hp_) + "/" + std::to_string(max_hp_);

  RCLCPP_INFO(this->get_logger(), "%s", response->message.c_str());
}

}  // namespace rm_referee_mock