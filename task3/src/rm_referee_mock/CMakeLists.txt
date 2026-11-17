cmake_minimum_required(VERSION 3.20)
project(rm_referee_mock)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-cpp)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(referee_mock_node
  src/referee_mock_node_main.cpp
  src/referee_mock_node.cpp
)

target_include_directories(referee_mock_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(referee_mock_node PUBLIC cxx_std_20)

target_link_libraries(referee_mock_node PUBLIC
  rclcpp::rclcpp
  std_msgs::std_msgs
  std_srvs::std_srvs
)

install(TARGETS referee_mock_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

ament_export_include_directories(include)
ament_export_dependencies(rclcpp std_msgs std_srvs)

ament_package()