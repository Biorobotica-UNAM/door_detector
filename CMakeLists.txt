cmake_minimum_required(VERSION 3.8)
project(door_detector)

# -----------------------------
# Compiler options
# -----------------------------
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----------------------------
# Find dependencies
# -----------------------------
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# -----------------------------
# Build node
# -----------------------------
add_executable(door_detector_node
  src/door_detector_node.cpp
)

ament_target_dependencies(door_detector_node
  rclcpp
  sensor_msgs
  std_srvs
)

# -----------------------------
# Install
# -----------------------------
install(TARGETS door_detector_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

ament_package()

