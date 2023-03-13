#ifndef RM_DECISION__RM_DECISION_NODE_HPP_
#define RM_DECISION__RM_DECISION_NODE_HPP_

#include "behaviortree_cpp/bt_factory.h"
#include <ros/ros.h>
#include <std_msgs/Int32.h>


struct Position2D{
    double x, y;
};

namespace BT
{
template <>
inline Position2D convertFromString(StringView str)
{
  printf("Converting string: \"%s\"\n", str.data());
  // real numbers separated by semicolons
  auto parts = splitString(str, ';');
  if (parts.size() != 2)
  {
    throw RuntimeError("invalid input)");
  }
  else
  {
    Position2D output;
    output.x = convertFromString<double>(parts[0]);
    output.y = convertFromString<double>(parts[1]);
    return output;
  }
}
}


namespace Decision{

//手动维护黑板
static int mode_now=1;

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