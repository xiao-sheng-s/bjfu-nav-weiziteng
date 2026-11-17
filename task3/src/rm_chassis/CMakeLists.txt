cmake_minimum_required(VERSION 3.20)
project(rm_chassis)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-cpp)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(chassis_node
  src/chassis_node_main.cpp
  src/chassis_node.cpp
)

target_include_directories(chassis_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(chassis_node PUBLIC cxx_std_20)

target_link_libraries(chassis_node PUBLIC
  rclcpp::rclcpp
  geometry_msgs::geometry_msgs
  nav_msgs::nav_msgs
  tf2::tf2
  tf2_ros::tf2_ros
)

install(TARGETS chassis_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

ament_export_include_directories(include)
ament_export_dependencies(rclcpp geometry_msgs nav_msgs tf2 tf2_ros)

ament_package()