#ifndef RM_DECISION__RM_DECISION_NODE_HPP_
#define RM_DECISION__RM_DECISION_NODE_HPP_

#include "behaviortree_cpp/bt_factory.h"
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "rm_interfaces/SerialReceiveMsg.h"

namespace Decision{

//手动维护一个黑板
static int mode_now=1;
static int time = 240;
static int brood = 600;
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
      std::cout<<"检测模式是否为："<<mode<<std::endl;
      if(mode_now==mode)
      {
        return BT::NodeStatus::SUCCESS;
      }
      std::cout<<"当前模式为："<<mode_now<<std::endl;
        return BT::NodeStatus::FAILURE;
  };
};

//检测是否小于想要的时间
class checkTime: public BT::SyncActionNode{
public:
    checkTime(const std::string& name, const BT::NodeConfig& config) :
    SyncActionNode(name, config)
  {}

      static BT::PortsList providedPorts()
  {
    return {
        BT::InputPort<int>("checktime")
        };
  }
  BT::NodeStatus tick() override
  {
      auto res = getInput<int>("checktime");
      int checkTime = res.value();
      std::cout<<"检测剩余时间是否小于："<<checkTime<<std::endl;
      if(checkTime<time)
      {
        return BT::NodeStatus::SUCCESS;
      }
      std::cout<<"当前时间为："<<time<<std::endl;
        return BT::NodeStatus::FAILURE;
  };
};

class checkTime: public BT::SyncActionNode{
public:
    checkTime(const std::string& name, const BT::NodeConfig& config) :
    SyncActionNode(name, config)
  {}

  BT::NodeStatus tick() override
  {
      if(time<240)
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
    void callback(const rm_interfaces::SerialReceiveMsgConstPtr &msg);
    ros::NodeHandle nh_;
    ros::Subscriber serial_sub;
};

}

#endif //RM_DECISION__RM_DECISION_NODE_HPP_