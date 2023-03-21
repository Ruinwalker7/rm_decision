#ifndef MOVEBASE_BT_NODES_H
#define MOVEBASE_BT_NODES_H

#include "behaviortree_cpp/behavior_tree.h"
#include <actionlib/client/simple_action_client.h> // 引用actionlib库
#include <move_base_msgs/MoveBaseAction.h>         // 引用move_base的信息

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
    MoveBaseClient;

struct Pose2D {
  double x, y, theta;
};

namespace BT {
template <> inline Pose2D convertFromString(StringView key) {
  auto parts = BT::splitString(key, ';');
  if (parts.size() != 3) {
    throw BT::RuntimeError("invalid input)");
  } else {
    Pose2D output;
    output.x = convertFromString<double>(parts[0]);
    output.y = convertFromString<double>(parts[1]);
    output.theta = convertFromString<double>(parts[2]);
    return output;
  }
}
} // end namespace BT

class MoveBaseAction : public BT::StatefulActionNode {
public:
  MoveBaseAction(const std::string &name, const BT::NodeConfig &config)
      : StatefulActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<Pose2D>("goal")};
  }

  BT::NodeStatus onStart() override;

  BT::NodeStatus onRunning() override;

  void onHalted() override;

private:
  Pose2D _goal;
  move_base_msgs::MoveBaseGoal goal;
  MoveBaseClient *ac;
};

BT::NodeStatus MoveBaseAction::onStart() {
  ros::spinOnce();
  ac = new MoveBaseClient("move_base", true);

  while (!ac->waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  if (!getInput<Pose2D>("goal", _goal)) {
    throw BT::RuntimeError("missing required input [goal]");
  }
  printf("[ MoveBase: SEND GOAL ]. goal: x=%.1f y=%.1f \n\n", _goal.x, _goal.y);

  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = _goal.x;
  goal.target_pose.pose.position.y = _goal.y;
  goal.target_pose.pose.orientation.w = _goal.theta;

  ac->sendGoal(goal);
  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveBaseAction::onRunning() {
  ros::spinOnce();
  if (ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    std::cout << "[ MoveBase: FINISHED ]" << std::endl << std::endl;
    return BT::NodeStatus::SUCCESS;
  } else if (ac->getState() == actionlib::SimpleClientGoalState::ABORTED)
    return BT::NodeStatus::FAILURE;

  std::this_thread::sleep_for(chr::milliseconds(200));
  std::cout << "GOAL RUNNING" << std::endl << std::endl;
  return BT::NodeStatus::RUNNING;
};

void MoveBaseAction::onHalted() {
  ac->cancelGoal();
  printf("[ MoveBase: ABORTED ]\n\n");
};
#endif // MOVEBASE_BT_NODES_H
