#include "rm_decision/rm_decision_node.hpp"
#include <signal.h> 

void DoShutdown(int sig)
{
	//这里主要进行退出前的数据保存、内存清理、告知其他节点等工作
	ROS_INFO("shutting down!");
	ros::shutdown(); 
  exit(sig); //为了更完整，处理一下退出的signal
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "robot_base");
  Decision::DecisionNode node;
  
  signal(SIGINT, DoShutdown);
  while (ros::ok()) {
    node.tree.tickWhileRunning();
    ros::spinOnce();
    sleep(1);
  };

  return 0;
}