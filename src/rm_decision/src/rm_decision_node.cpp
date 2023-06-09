#include "rm_decision/rm_decision_node.hpp"
#include "rm_decision/move_base.hpp"
#include "rm_decision/patrol.hpp"
#include <ros/package.h>
#include <std_msgs/Int32.h>
#include <string>

namespace Decision {
void DecisionNode::callback(
  const rm_interfaces::SerialReceiveMsgConstPtr &msg) {
  mode_now = msg->game_status;
  if (msg->blood < blood) {
    completion_time = chr::system_clock::now() + chr::milliseconds(6000);
  }
  blood = msg->blood;
  time = msg->remaining_time;
  outpose_HP = msg->outpose_HP;
}

DecisionNode::DecisionNode() : nh_() {
  serial_sub = nh_.subscribe<rm_interfaces::SerialReceiveMsg>(
      "/serial_receive", 100, &DecisionNode::callback, this);
  pub_isspin = nh_.advertise<std_msgs::Int32>("/isspin",1);

  std::string model_path =
      ros::package::getPath("rm_decision") + "/my_tree.xml";
  
  //注册树
  factory.registerNodeType<checkMode>("checkMode");
  factory.registerNodeType<PatrolAction>("patrol");
  factory.registerNodeType<MoveBaseAction>("movebase");
  factory.registerNodeType<IsBelowTime>("IsBelowTime");
  factory.registerNodeType<IsAboveBlood>("IsAboveBlood");
  factory.registerNodeType<SpinAction>("spinAction");
  factory.registerNodeType<IsAttacked>("attracked");
  factory.registerNodeType<SetStatus>("SetStatus");
  factory.registerNodeType<IsStatus>("IsStatus");
  factory.registerNodeType<Spin>("spin");
  
  tree = factory.createTreeFromFile(model_path);

}

} // namespace Decision