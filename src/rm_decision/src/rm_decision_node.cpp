#include "rm_decision/rm_decision_node.hpp"
#include <std_msgs/Int32.h>
#include <ros/package.h>
#include <string>
#include "rm_decision/move_base.hpp"


namespace Decision{
void DecisionNode::callback(const rm_decision::SerialReceiveMsgConstPtr &msg){
    mode_now = msg->mode;
}

DecisionNode::DecisionNode():nh_(){

    serial_sub=nh_.subscribe<rm_decision::SerialReceiveMsg>(
        "/serial_receive",100,&DecisionNode::callback, this);
    
    std::string model_path = ros::package::getPath("rm_decision") + "/my_tree.xml";
    //注册树
    factory.registerNodeType<checkMode>("checkMode");
    factory.registerNodeType<MoveBaseAction>("movebase");

    tree = factory.createTreeFromFile(model_path);
    }
}