#ifndef RM_REFEREE_MOCK__REFEREE_MOCK_NODE_HPP_
#define RM_REFEREE_MOCK__REFEREE_MOCK_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int16.hpp"
#include "std_srvs/srv/trigger.hpp"

namespace rm_referee_mock
{

class RefereeMockNode : public rclcpp::Node
{
public:
  explicit RefereeMockNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
  void publishStatus();
  void handleSupplyHp(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  uint16_t max_hp_;
  uint16_t max_ammo_;
  double publish_rate_;
  double auto_start_delay_;
  double ammo_consume_per_sec_;

  uint16_t hp_;
  uint16_t ammo_;
  uint16_t stage_;
  double ammo_accum_;
  rclcpp::Time start_time_;
  rclcpp::Time last_time_;

  rclcpp::Publisher<std_msgs::msg::UInt16>::SharedPtr stage_pub_;
  rclcpp::Publisher<std_msgs::msg::UInt16>::SharedPtr hp_pub_;
  rclcpp::Publisher<std_msgs::msg::UInt16>::SharedPtr ammo_pub_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr supply_srv_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace rm_referee_mock

#endif  // RM_REFEREE_MOCK__REFEREE_MOCK_NODE_HPP_