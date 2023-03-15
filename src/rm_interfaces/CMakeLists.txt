cmake_minimum_required(VERSION 3.0.2)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
project(rm_interfaces)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  geometry_msgs
  std_msgs
)

 add_message_files(
   FILES
   SerialReceiveMsg.msg
 )

 generate_messages(
   DEPENDENCIES
   geometry_msgs
   std_msgs
 )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rm_interfaces
#  CATKIN_DEPENDS geometry_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

