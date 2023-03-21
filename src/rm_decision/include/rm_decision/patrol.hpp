#ifndef PATROL_HPP
#define PATROL_HPP

#include "behaviortree_cpp/behavior_tree.h"
#include <actionlib/client/simple_action_client.h> // 引用actionlib库
#include <move_base_msgs/MoveBaseAction.h>         // 引用move_base的信息
#include <ros/ros.h>

namespace chr = std::chrono;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
MoveBaseClient *ac = nullptr;
struct PatrolRange {
        double x, y, x_, y_;
};

namespace BT {
template <> inline PatrolRange convertFromString(StringView key) {
  auto parts = BT::splitString(key, ';');
  if (parts.size() != 4) {
    throw BT::RuntimeError("invalid input)");
  } else {
    PatrolRange output;
    output.x = convertFromString<double>(parts[0]);
    output.y = convertFromString<double>(parts[1]);
    output.x_ = convertFromString<double>(parts[2]);
    output.y_ = convertFromString<double>(parts[3]);
    return output;
  }
}
} // end namespace BT


class PatrolAction : public BT::StatefulActionNode {
public:
  // Any TreeNode with ports must have a constructor with this signature
  PatrolAction(const std::string &name, const BT::NodeConfig &config)
      : StatefulActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<PatrolRange>("goal")};
  }

  BT::NodeStatus onStart() override;

  BT::NodeStatus onRunning() override;

  void onHalted() override;

  void patrol();

private:
  PatrolRange _goal;
  move_base_msgs::MoveBaseGoal goal;
};

void PatrolAction::patrol() {
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.orientation.w = 1;
  _goal.x_ = -_goal.x_;
  _goal.y_ = -_goal.y_;
  goal.target_pose.pose.position.x = _goal.x + _goal.x_;
  goal.target_pose.pose.position.y = _goal.y + _goal.y_;
  printf("[ Patrol: SEND GOAL ]. goal: x=%.1f y=%.1f \n",
         goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
  ac->sendGoal(goal);
}

BT::NodeStatus PatrolAction::onStart() {
  if (ac == nullptr)
    ac = new MoveBaseClient("move_base", true);
  if (ac->getState() == actionlib::SimpleClientGoalState::ACTIVE)
    return BT::NodeStatus::RUNNING;

  while (!ac->waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  if (!getInput<PatrolRange>("goal", _goal)) {
    throw BT::RuntimeError("missing required input [goal]");
  }

  patrol();
  ros::spinOnce();
  
  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus PatrolAction::onRunning() {
  ros::spinOnce();
  sleep(1);
  if (ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    patrol();
    ros::spinOnce();
    return BT::NodeStatus::SUCCESS;
  } else if (ac->getState() == actionlib::SimpleClientGoalState::ABORTED)
    return BT::NodeStatus::FAILURE;
  std::this_thread::sleep_for(chr::milliseconds(200));
  std::cout << "GOAL RUNNING" << std::endl << std::endl;
  return BT::NodeStatus::RUNNING;
};

void PatrolAction::onHalted() {
  ac->cancelGoal();
  printf("[ MoveBase: ABORTED ]");
};
#endif // PATROL_HPP
