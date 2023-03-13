#ifndef RM_DECISION__RM_DECISION_NODE_HPP_
#define RM_DECISION__RM_DECISION_NODE_HPP_

#include "behaviortree_cpp/bt_factory.h"
#include <ros/ros.h>
#include <std_msgs/Int32.h>

namespace Decision{

//手动维护一个黑板
static int mode_now=1;

//检测是否到想要的模式（开始、结束）
class checkMode: public BT::SyncActionNode{
public:
    checkMode(const std::string& name, const BT::NodeConfig& config) :
    SyncActionNode(name, config)
  {}

      static BT::PortsList providedPorts()
  {
    return {
        BT::InputPort<int>("checkmode")
        };
  }

  BT::NodeStatus tick() override
  {
      auto res = getInput<int>("checkmode");
      int mode = res.value();
      std::cout<<mode<<std::endl;
      if(mode_now==mode)
      {
        return BT::NodeStatus::SUCCESS;
      }
        return BT::NodeStatus::FAILURE;
  };
};

class DecisionNode{
public:
    DecisionNode();
    BT::BehaviorTreeFactory factory;
    BT::Tree tree;
private:
    // void callback(const std_msgs::Int32 &msg);
    ros::NodeHandle nh_;
    ros::Subscriber serial_sub;
    int mode;
};

}

#endif //RM_DECISION__RM_DECISION_NODE_HPP_