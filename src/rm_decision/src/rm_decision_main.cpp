#include "rm_decision/rm_decision_node.hpp"


int main(int argc, char** argv) {
  // create ros2 node
  ros::init(argc, argv, "robot_base");
  // serial::RobotBaseNode node;
  while (ros::ok()) {
    ros::spinOnce();
  };

  return 0;
}