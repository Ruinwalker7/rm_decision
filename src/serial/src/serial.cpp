#include <ros/ros.h>
#include "rm_interfaces/SerialReceiveMsg.h"
#include "configure/SerialConfig.h"
#include <dynamic_reconfigure/server.h>
typedef dynamic_reconfigure::Server<serial::SerialConfig> config_srv;


static int mode = 0;
static int blood=600;
static int time_=420;
void configure_cb(const serial::SerialConfig &config, uint32_t level) {
  mode = config.mode;
  blood =config.blood;
  time_= config.time;
}

int main(int argc, char** argv) {
  // create ros2 node

  ros::init(argc, argv, "serial");
  ros::NodeHandle _nh;
  config_srv config_server_;
  config_srv::CallbackType  f_;

  f_ = boost::bind(configure_cb, _1, _2);
  config_server_.setCallback(f_);

    ros::Publisher serial_pub = _nh.advertise<rm_interfaces::SerialReceiveMsg>("/serial_receive",1);
    rm_interfaces::SerialReceiveMsg msg;
    msg.pitch=0;
    msg.yaw=0;
    msg.outpose_HP=0;
    msg.time = 420;
    msg.blood=blood;
    msg.mode=mode;
    msg.header.stamp=ros::Time::now();
    msg.time = time_;
    ros::Rate freqency(ros::Duration(1/100.0));
    while (ros::ok()) {
      ros::spinOnce();
      msg.blood=blood;
      msg.mode=mode;
      msg.time = time_;
      msg.header.stamp=ros::Time::now();
      serial_pub.publish(msg);
      ros::spinOnce();
      freqency.sleep();
    };

  return 0;
}