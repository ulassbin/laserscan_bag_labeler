cmake_minimum_required(VERSION 3.0.2)
project(rosbag_labeler)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
set(rosbag_labeler_ROS_DEPS
	roscpp
	rosbag
	sensor_msgs
	nav_msgs
)

find_package(catkin REQUIRED COMPONENTS
	${rosbag_labeler_ROS_DEPS}
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS ${rosbag_labeler_ROS_DEPS}
)

# add_library(rosbag_labeler
#   src/labeler.cpp
# )


add_executable(rosbag_labeler
  src/labeler.cpp
)

add_dependencies(rosbag_labeler
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(rosbag_labeler
 ${catkin_LIBRARIES}
)

#############
## Install ##
#############

install(
    TARGETS
	        rosbag_labeler
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)
