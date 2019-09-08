cmake_minimum_required(VERSION 2.8.3)
project(image_anonymization)

find_package(catkin REQUIRED COMPONENTS roscpp image_transport sensor_msgs cv_bridge)

find_package(OpenCV REQUIRED)

include_directories(include ${catkin_INCLUDE_DIRS})

catkin_package()

add_executable(image_anonymization src/image_anonymization.cpp)

target_link_libraries(image_anonymization
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES})

if(catkin_EXPORTED_TARGETS)
  add_dependencies(image_anonymization ${catkin_EXPORTED_TARGETS})
endif()
