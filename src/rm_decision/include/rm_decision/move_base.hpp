#ifndef MOVEBASE_BT_NODES_H
#define MOVEBASE_BT_NODES_H

#include "behaviortree_cpp/behavior_tree.h"
#include <move_base_msgs/MoveBaseAction.h>   // 引用move_base的信息
#include <actionlib/client/simple_action_client.h>   // 引用actionlib库

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct Pose2D
{
    double x, y, theta;
};

namespace BT
{
template <> inline
Pose2D convertFromString(StringView key)
{
    auto parts = BT::splitString(key, ';');
    if (parts.size() != 3)
    {
        throw BT::RuntimeError("invalid input)");
    }
    else
    {
        Pose2D output;
        output.x     = convertFromString<double>(parts[0]);
        output.y     = convertFromString<double>(parts[1]);
        output.theta = convertFromString<double>(parts[2]);
        return output;
    }
}
} // end namespace BT


class MoveBaseAction : public BT::StatefulActionNode
{
  public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
      : StatefulActionNode(name, config)
    {
        
    }

    static BT::PortsList providedPorts()
    {
        return{ BT::InputPort<Pose2D>("goal") };
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override;

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override;

    // callback to execute if the action was aborted by another node
    void onHalted() override;

  private:
    Pose2D _goal;
    move_base_msgs::MoveBaseGoal goal;
    MoveBaseClient *ac;
};

BT::NodeStatus MoveBaseAction::onStart()
{
    ac = new MoveBaseClient("move_base", true);
    if ( !getInput<Pose2D>("goal", _goal))
    {
    throw BT::RuntimeError("missing required input [goal]");
    }
    printf("[ MoveBase: SEND GOAL ]. goal: x=%.1f y=%.1f theta=%.1f\n",
         _goal.x, _goal.y, _goal.theta);

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = _goal.x;
    goal.target_pose.pose.position.y = _goal.y;
    goal.target_pose.pose.orientation.w = _goal.theta;

    // ac->sendGoal(goal);

    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveBaseAction::onRunning()
{

    if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
    std::cout << "[ MoveBase: FINISHED ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
    else if(ac->getState() == actionlib::SimpleClientGoalState::ABORTED)
        return BT::NodeStatus::FAILURE;

    sleep(1);
    std::cout<<"GOAL RUNNING"<<std::endl;
    return BT::NodeStatus::RUNNING;
};

void MoveBaseAction::onHalted()
{
    ac->cancelGoal();
    printf("[ MoveBase: ABORTED ]");
};
#endif   // MOVEBASE_BT_NODES_H
