#include "rm_decision/rm_decision_node.hpp"
#include <std_msgs/Int32.h>

namespace Decision{

static const char* xml_text = R"(

 <root BTCPP_format="4" >
     <BehaviorTree ID="MainTree">
        <Sequence name="root">
            <checkMode checkmode="1"/>
            <checkMode checkmode="2"/>
        </Sequence>
     </BehaviorTree>
 </root>
 )";


// void DecisionNode::callback1(const std_msgs::Int32 &msg){
//     // mode = msg;
// }

DecisionNode::DecisionNode():nh_(){
    //    serial_sub = nh_.subscribe<std_msgs::Int32>(
    //   "serial_sendmsg", 100, &DecisionNode::callback1, this);
        factory.registerNodeType<checkMode>("checkMode");
        tree = factory.createTreeFromText(xml_text);
    }
}