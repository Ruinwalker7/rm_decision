#include <ros/ros.h>
// #include <rm_decision/SerialReceiveMsg.h>

int main(int argc, char** argv) {
  // create ros2 node
  ros::init(argc, argv, "serial");
  ros::NodeHandle _nh;
    // ros::Publisher serial_pub = _nh.advertise<rm_decision::SerialReceiveMsg>("/serial_receive",2);
    // rm_decision::SerialReceiveMsg msg;
    // msg.pitch=0;
    // msg.yaw=0;
    // msg.time = 420;
    // msg.blood=600;
    // msg.mode=0;
    // msg.header.stamp=ros::Time::now();

    while (ros::ok()) {
        // serial_pub.publish(msg);
        ros::spinOnce();
        sleep(1);
    };

  return 0;
}