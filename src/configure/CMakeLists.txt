cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++17 -O3")
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

project(configure)

find_package(catkin REQUIRED COMPONENTS
  dynamic_reconfigure
  roscpp
  rospy
)

set (DYNAMIC_RECONFIGURE_PATH 
${CMAKE_CURRENT_SOURCE_DIR}/dynamic_configure/serial.cfg )

execute_process(COMMAND chmod a+x ${DYNAMIC_RECONFIGURE_PATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        RESULT_VARIABLE cmd_result
        OUTPUT_VARIABLE cmd_ver)

generate_dynamic_reconfigure_options(
  dynamic_configure/serial.cfg
  #...
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES camera
#  CATKIN_DEPENDS cv_bridge dynamic_reconfigure image_transport roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)


