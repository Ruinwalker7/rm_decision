#ifndef PATROL_HPP
#define PATROL_HPP

#include "behaviortree_cpp/behavior_tree.h"
#include <move_base_msgs/MoveBaseAction.h>   // 引用move_base的信息
#include <actionlib/client/simple_action_client.h>   // 引用actionlib库

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct PatrolRange
{
    double x, y, x_,y_;
};

namespace BT
{
template <> inline
PatrolRange convertFromString(StringView key)
{
    auto parts = BT::splitString(key, ';');
    if (parts.size() != 4)
    {
        throw BT::RuntimeError("invalid input)");
    }
    else
    {
        PatrolRange output;
        output.x     = convertFromString<double>(parts[0]);
        output.y     = convertFromString<double>(parts[1]);
        output.x_  =  convertFromString<double>(parts[2]);
        output.y_     = convertFromString<double>(parts[3]);
        return output;
    }
}
} // end namespace BT


class PatrolAction : public BT::StatefulActionNode
{
  public:
    // Any TreeNode with ports must have a constructor with this signature
    PatrolAction(const std::string& name, const BT::NodeConfig& config)
      : StatefulActionNode(name, config)
    {
        
    }

    static BT::PortsList providedPorts()
    {
        return{ BT::InputPort<PatrolRange>("goal") };
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override;

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override;

    // callback to execute if the action was aborted by another node
    void onHalted() override;

  private:
    PatrolRange _goal;
    move_base_msgs::MoveBaseGoal goal;
    MoveBaseClient *ac;
};

BT::NodeStatus PatrolAction::onStart()
{
    ac = new MoveBaseClient("move_base", true);

    while(!ac->waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    if ( !getInput<PatrolRange>("goal", _goal))
    {
    throw BT::RuntimeError("missing required input [goal]");
    }
    printf("[ MoveBase: SEND GOAL ]. goal: x=%.1f y=%.1f \n",
         _goal.x, _goal.y);

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = _goal.x+_goal.x_;
    goal.target_pose.pose.position.y = _goal.y+_goal.y_;
    goal.target_pose.pose.orientation.w = 1;

    ac->sendGoal(goal);
    return BT::NodeStatus::RUNNING;
}


BT::NodeStatus PatrolAction::onRunning()
{

    if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
    //     _goal.x_=-_goal.x;
    //     _goal.y_=-_goal.y;
    // goal.target_pose.pose.position.x=_goal.x+_goal.x_;
    // goal.target_pose.pose.position.y=_goal.y+_goal.y_;
    // ac->sendGoal(goal);
    return BT::NodeStatus::SUCCESS;
  }
    else if(ac->getState() == actionlib::SimpleClientGoalState::ABORTED)
        return BT::NodeStatus::FAILURE;

    sleep(1);
    std::cout<<"GOAL RUNNING"<<std::endl<<std::endl;
    return BT::NodeStatus::RUNNING;
};

void PatrolAction::onHalted()
{
    ac->cancelGoal();
    printf("[ MoveBase: ABORTED ]");
};
#endif   // PATROL_HPP
