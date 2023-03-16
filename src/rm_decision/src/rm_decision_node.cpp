#include "rm_decision/rm_decision_node.hpp"
#include <std_msgs/Int32.h>
#include <ros/package.h>
#include <string>
#include "rm_decision/move_base.hpp"
#include "rm_decision/patrol.hpp"

namespace Decision{
void DecisionNode::callback(const rm_interfaces::SerialReceiveMsgConstPtr &msg){
    mode_now = msg->mode;
    blood = msg->blood;
    time = msg->time;
}

DecisionNode::DecisionNode():nh_(){

    serial_sub=nh_.subscribe<rm_interfaces::SerialReceiveMsg>(
        "/serial_receive",100,&DecisionNode::callback, this);
    
    std::string model_path = ros::package::getPath("rm_decision") + "/my_tree.xml";
    //注册树
    factory.registerNodeType<checkMode>("checkMode");
    factory.registerNodeType<PatrolAction>("patrol");
    factory.registerNodeType<MoveBaseAction>("movebase");
    factory.registerNodeType<checkTime>("checkTime");
    factory.registerNodeType<checkBlood>("checkBlood");
    tree = factory.createTreeFromFile(model_path);
    }
}