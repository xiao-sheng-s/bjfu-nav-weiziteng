#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rm_referee_mock/referee_mock_node.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rm_referee_mock::RefereeMockNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}