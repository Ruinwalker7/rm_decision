#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Imu

def imu_cb(msg=Imu()):
    new_msg = Imu()
    new_msg.header = msg.header
    new_msg.header.frame_id = "imu_modify"
    new_msg.linear_acceleration.z = -msg.linear_acceleration.x
    new_msg.linear_acceleration.y = msg.linear_acceleration.z
    new_msg.linear_acceleration.x = -msg.linear_acceleration.y
    new_msg.angular_velocity.z = -msg.angular_velocity.x
    new_msg.angular_velocity.y = msg.angular_velocity.z
    new_msg.angular_velocity.x =- msg.angular_velocity.y
    imu_pub.publish(new_msg)

if __name__ == "__main__":
    global imu_pub
    rospy.init_node("imu_modify")
    imu_sub = rospy.Subscriber("/stereo_inertial_publisher/imu",Imu,imu_cb,queue_size=1)
    imu_pub = rospy.Publisher("/imu_modify",Imu,queue_size=1)
    if not rospy.is_shutdown():
        rospy.spin()