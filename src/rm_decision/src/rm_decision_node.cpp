#include "rm_decision/rm_decision_node.hpp"

namespace Decision{

static const char* xml_text = R"(

 <root BTCPP_format="4" >
     <BehaviorTree ID="MainTree">
        <Sequence name="root">
        </Sequence>
     </BehaviorTree>
 </root>
 )";

    DecisionNode::DecisionNode():nh_(){
        
    }
}