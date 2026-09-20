cmake_minimum_required(VERSION 3.8)
project(yudhish_onboarding)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_library(calculator src/calculator.cpp)
target_include_directories(calculator PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_executable(calculator_main src/calculator_main.cpp)
target_link_libraries(calculator_main calculator)

add_executable(onboarding_node src/onboarding_node.cpp)
target_link_libraries(onboarding_node calculator)
ament_target_dependencies(onboarding_node rclcpp std_msgs)

install(TARGETS calculator
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(TARGETS calculator_main onboarding_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(calculator_test test/calculator_test.cpp)
  target_link_libraries(calculator_test calculator)
endif()

ament_package()