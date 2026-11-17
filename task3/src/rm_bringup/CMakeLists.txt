cmake_minimum_required(VERSION 3.20)
project(rm_bringup)

find_package(ament_cmake REQUIRED)

install(DIRECTORY launch rviz
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
