#ifndef RM_DECISION__RM_DECISION_NODE_HPP_
#define RM_DECISION__RM_DECISION_NODE_HPP_

#include "behaviortree_cpp/bt_factory.h"
#include <ros/ros.h>

namespace Decision{
class DecisionNode{
public:
    DecisionNode();
    BT::BehaviorTreeFactory factory;
private:
    ros::NodeHandle nh_;
    ros::Subscriber serial_sub;

    int mode;
};

}
#endif //RM_DECISION__RM_DECISION_NODE_HPP_