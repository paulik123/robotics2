cmake_minimum_required(VERSION 2.8.3)
project(joy_simulator)
find_package(catkin REQUIRED COMPONENTS roscpp joy)
catkin_package()
include_directories(include ${catkin_INCLUDE_DIRS})
add_executable(joy_simulator src/joy_simulator.cpp)
target_link_libraries(joy_simulator ${catkin_LIBRARIES})