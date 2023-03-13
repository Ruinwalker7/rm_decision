#include "rm_decision/rm_decision_node.hpp"
#include <std_msgs/Int32.h>
#include <ros/package.h>
#include <string>
#include "rm_decision/move_base.hpp"

namespace Decision{

DecisionNode::DecisionNode():nh_(){
    std::string model_path = ros::package::getPath("rm_decision") + "/my_tree.xml";
    //注册树
    factory.registerNodeType<checkMode>("checkMode");
    factory.registerNodeType<MoveBaseAction>("movebase");

    tree = factory.createTreeFromFile(model_path);
    }
}