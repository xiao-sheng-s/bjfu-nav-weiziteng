#include <memory>
 
#include "rclcpp/rclcpp.hpp"
#include "rm_chassis/chassis_node.hpp"
 
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);                              
  auto node = std::make_shared<rm_chassis::ChassisNode>();  
  rclcpp::spin(node);                                    
  rclcpp::shutdown();                                    
  return 0;
}