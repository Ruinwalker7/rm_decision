cmake_minimum_required(VERSION 3.0.2)
project(rm_decision)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

# find_package(behaviortree_cpp)
find_package(behaviortree_cpp)
catkin_package(
  INCLUDE_DIRS include
 LIBRARIES rm_decision
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

set(SRC
  "src/rm_decision_node.cpp"
  "src/rm_decision_main.cpp"
)
add_executable(${PROJECT_NAME}_node ${SRC})
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_node 
${catkin_LIBRARIES}
BT::behaviortree_cpp)