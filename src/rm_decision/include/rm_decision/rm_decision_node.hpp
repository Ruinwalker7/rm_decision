#ifndef RM_DECISION__RM_DECISION_NODE_HPP_
#define RM_DECISION__RM_DECISION_NODE_HPP_

#include "behaviortree_cpp/bt_factory.h"
#include "rm_interfaces/SerialReceiveMsg.h"
#include <ros/ros.h>
#include <std_msgs/Int32.h>

namespace chr = std::chrono;
static chr::system_clock::time_point completion_time = chr::system_clock::now();

namespace Decision {

//手动维护一个黑板
static int mode_now = 0;
static int time = 500;
static int blood = 600;
static int outpose_HP = 1000;
static std_msgs::Int32 isspin; //0不小陀螺
static ros::Publisher pub_isspin;
static int Positionstatus = 0;

//检测是否到想要的模式（开始、结束）
class checkMode : public BT::SyncActionNode {
public:
  checkMode(const std::string &name, const BT::NodeConfig &config)
      : SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("checkmode")};
  }

  BT::NodeStatus tick() override {
    auto res = getInput<int>("checkmode");
    int mode = res.value();
    std::cout << "检测模式是否为：" << mode << std::endl;
    std::cout << "当前模式为：" << mode_now << std::endl << std::endl;
    if (mode_now == mode) {
      return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::FAILURE;
  };
};

//判断status
class IsStatus : public BT::SyncActionNode
{
public:
  IsStatus(const std::string& name, const BT::NodeConfig& config)
    : SyncActionNode(name, config)
  { }

  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("status")};
  }

  BT::NodeStatus tick() override
  {
    auto res = getInput<int>("status");
    int Status = res.value();
    if(Positionstatus==Status)
      return BT::NodeStatus::SUCCESS;
    else
      return BT::NodeStatus::FAILURE;
  }
};


//设置status
class SetStatus : public BT::SyncActionNode
{
public:
  SetStatus(const std::string& name, const BT::NodeConfig& config)
    : SyncActionNode(name, config)
  { }

  static BT::PortsList providedPorts() {
    return {BT::OutputPort<int>("status")};
  }

  BT::NodeStatus tick() override
  { setOutput("status", Positionstatus );
      return BT::NodeStatus::SUCCESS;

  }
};

//检测是否小于想要的时间
class IsBelowTime : public BT::SyncActionNode {
public:
  IsBelowTime(const std::string &name, const BT::NodeConfig &config)
      : SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("checktime")};
  }

  BT::NodeStatus tick() override {
    ros::spinOnce();
    auto res = getInput<int>("checktime");
    int checkTime = res.value();
    std::cout << "检测剩余时间是否小于：" << checkTime << std::endl;
    std::cout << "当前时间为：" << time << std::endl << std::endl;
    if (checkTime < time) {
      return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::FAILURE;
  };
};

//开启小陀螺
class Spin : public BT::SyncActionNode {
public:
  Spin(const std::string &name)
      : SyncActionNode(name,{}) {}

  BT::NodeStatus tick() override {
    // ros::spinOnce();
    isspin.data = 1;
    pub_isspin.publish(isspin);
    std::cout << "开启小陀螺" << std::endl;
    return BT::NodeStatus::SUCCESS;
  };
};

//关闭小陀螺
class StopSpin : public BT::SyncActionNode {
public:
  StopSpin(const std::string &name)
      : SyncActionNode(name,{}) {}

  BT::NodeStatus tick() override {
    ros::spinOnce();
    isspin.data = 0;
    pub_isspin.publish(isspin);
    std::cout << "关闭小陀螺" << std::endl;
    return BT::NodeStatus::SUCCESS;
  };
};

//检测是否大于想要的血量
class IsAboveBlood : public BT::SyncActionNode {
public:
  IsAboveBlood(const std::string &name, const BT::NodeConfig &config)
      : SyncActionNode(name,config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("checkblood")};
  }

  BT::NodeStatus tick() override {
    ros::spinOnce();
    auto res = getInput<int>("checkblood");
    int checkblood = res.value();
    std::cout << "检测血量是否大于：" << checkblood << std::endl;
    std::cout << "当前血量为：" << blood << std::endl << std::endl;
    if (blood > checkblood) {
      return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::FAILURE;
  };
};

//检测是否被攻击
class IsAttacked : public BT::SyncActionNode {
public:
  IsAttacked(const std::string &name, const BT::NodeConfig &config)
      : SyncActionNode(name, config) {
    _blood = blood;
  }
  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("checktime")};
  }

  BT::NodeStatus tick() override {
    std::cout << "检测是否被攻击" << std::endl;
    ros::spinOnce();
    if (blood < _blood) {
      _blood = blood;
      std::cout << "被攻击" << std::endl << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
    _blood = blood;
    return BT::NodeStatus::FAILURE;
  };

private:
  int _blood;
};

//被攻击后小陀螺
class SpinAction : public BT::StatefulActionNode {
public:
  SpinAction(const std::string &name, const BT::NodeConfig &config)
      : StatefulActionNode(name, config) {}

  BT::NodeStatus onStart() {
    if (chr::system_clock::now() > completion_time){
      std::cout<<"No attacked\n\n";
      return BT::NodeStatus::SUCCESS;
    }
    isspin.data = 1;
    pub_isspin.publish(isspin);
    std::cout << "[ Spin: START ]" << std::endl;
    return BT::NodeStatus::RUNNING;
  }

  static BT::PortsList providedPorts() {
    return {BT::InputPort<int>("checktime")};
  }

  BT::NodeStatus onRunning() {
    std::this_thread::sleep_for(chr::milliseconds(100));
    if (chr::system_clock::now() >= completion_time) {
      std::cout << "[ Spin: FINISHED ]" << std::endl;
      isspin.data = 0;
      pub_isspin.publish(isspin);
      return BT::NodeStatus::SUCCESS;
    }
    isspin.data = 1;
    pub_isspin.publish(isspin);
    std::cout << "[ Spin: RUNNING ]" << std::endl;
    return BT::NodeStatus::RUNNING;
  }
  void onHalted() {
    std::cout << "[ Spin: STOP ]" << std::endl;
    isspin.data = 0;
    pub_isspin.publish(isspin);
  }
};


class DecisionNode {
public:
  DecisionNode();
  BT::BehaviorTreeFactory factory;
  BT::Tree tree;

private:
  void callback(const rm_interfaces::SerialReceiveMsgConstPtr &msg);
  ros::NodeHandle nh_;
  ros::Subscriber serial_sub;
};

} // namespace Decision

#endif // RM_DECISION__RM_DECISION_NODE_HPP_