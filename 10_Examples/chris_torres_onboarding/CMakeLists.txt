cmake_minimum_required(VERSION 3.8)
project(chris_torres_onboarding)

# Add Global CMake Config for Ghost Packages
set(Ghost_DIR "$ENV{VEXU_HOME}/cmake")
find_package(Ghost CONFIG REQUIRED)

# Adds all our package dependencies to one list
set(DEPENDENCIES
  ament_cmake
  ament_cmake_gtest
  rclcpp
  sensor_msgs
  geometry_msgs
  std_msgs
  )

# Iterates through the list and finds the packages!
foreach(pkg ${DEPENDENCIES})
  find_package(${pkg} REQUIRED)
endforeach()

# Tell any packages that depend on us what dependencies we need.
ament_export_dependencies(${DEPENDENCIES})

# Add any include directories to a list
# Usually this is just "include", but for some depedencies there could be extras.
set(INCLUDE
  include
)

# Tells the package where all our header files live
include_directories(${INCLUDE})

# Tells any packages that depend on us what include files we use.
ament_export_include_directories(${INCLUDE})

# Hello World example executable
add_executable(hello_world src/hello_world.cpp)
install(TARGETS
  hello_world
  DESTINATION lib/${PROJECT_NAME})

# ROS Publisher
add_executable(onboarding_ros_publisher src/onboarding_ros_publisher.cpp)
ament_target_dependencies(onboarding_ros_publisher ${DEPENDENCIES})
install(TARGETS
  onboarding_ros_publisher
  DESTINATION lib/${PROJECT_NAME})

# ROS Subscriber Executable
add_executable(onboarding_ros_subscriber_main src/onboarding_ros_subscriber_main.cpp)
ament_target_dependencies(onboarding_ros_subscriber_main ${DEPENDENCIES})
target_link_libraries(onboarding_ros_subscriber_main
  onboarding_ros_subscriber
)
install(TARGETS
  onboarding_ros_subscriber_main
  DESTINATION lib/${PROJECT_NAME})

# ROS Subscriber Library
add_library(onboarding_ros_subscriber SHARED src/onboarding_ros_subscriber.cpp)
ament_target_dependencies(onboarding_ros_subscriber
  ${DEPENDENCIES}
)
ament_export_targets(onboarding_ros_subscriber HAS_LIBRARY_TARGET)
install(
  TARGETS onboarding_ros_subscriber
  EXPORT onboarding_ros_subscriber
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

ament_add_gtest(test_chris_torres_onboarding test/test_chris_torres_onboarding.cpp)
target_link_libraries(test_chris_torres_onboarding
  onboarding_ros_subscriber
  gtest
)

install(
  DIRECTORY include/
  DESTINATION include
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/launch")
  install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/urdf")
  install(DIRECTORY urdf DESTINATION share/${PROJECT_NAME})
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config")
  install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
endif()

ament_package()