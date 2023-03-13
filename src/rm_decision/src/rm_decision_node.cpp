#include "rm_decision/rm_decision_node.hpp"
#include <std_msgs/Int32.h>
#include <ros/package.h>
#include <string>

namespace Decision{


// void DecisionNode::callback1(const std_msgs::Int32 &msg){
//     // mode = msg;
// }

DecisionNode::DecisionNode():nh_(){
    //    serial_sub = nh_.subscribe<std_msgs::Int32>(
    //   "serial_sendmsg", 100, &DecisionNode::callback1, this);
      std::string model_path = ros::package::getPath("rm_decision") + "/my_tree.xml";
        factory.registerNodeType<checkMode>("checkMode");
        tree = factory.createTreeFromFile(model_path);
    }
}