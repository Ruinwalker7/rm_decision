#include <ros/ros.h>
#include "rm_interfaces/SerialReceiveMsg.h"
#include "configure/SerialConfig.h"

static int mode = 0;
static int brood=600;
void configure_cb(const serial::SerialConfig &config, uint32_t level) {
  mode = config.mode;
  brood =config.brood;
}

int main(int argc, char** argv) {
  // create ros2 node
  ros::init(argc, argv, "serial");
  ros::NodeHandle _nh;
    ros::Publisher serial_pub = _nh.advertise<rm_interfaces::SerialReceiveMsg>("/serial_receive",2);
    rm_interfaces::SerialReceiveMsg msg;
    msg.pitch=0;
    msg.yaw=0;
    msg.time = 420;
    msg.blood=brood;
    msg.mode=mode;
    msg.header.stamp=ros::Time::now();

    while (ros::ok()) {
        // serial_pub.publish(msg);
        ros::spinOnce();
        sleep(1);
    };

  return 0;
}