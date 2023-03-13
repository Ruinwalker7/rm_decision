#include "rm_decision/rm_decision_node.hpp"


int main(int argc, char** argv) {
  // create ros2 node
  ros::init(argc, argv, "robot_base");
  Decision::DecisionNode node;
  
  while (ros::ok()) {
    node.tree.tickWhileRunning();
    ros::spinOnce();
    sleep(1);
  };

  return 0;
}