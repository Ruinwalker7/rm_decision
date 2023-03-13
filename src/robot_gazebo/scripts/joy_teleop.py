#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float64
from std_msgs.msg import Header
from geometry_msgs.msg import TwistStamped

PI = 3.1415926535
VELOCITY = 3
ANGULAR = PI/4
YAW_MAX_ANGLE = PI
PITCH_MAX_ANGLE = PI/3

def joy_callback(msg):
  global twist, pitch, yaw
  axes = msg.axes
  buttons = msg.buttons
  linear_x = axes[1]
  linear_y = axes[0]
  pitch_ = -axes[3]
  yaw_ = axes[2]
  angular_1 = axes[5]
  angular_2 = axes[4]
  
  vel_msg = TwistStamped()
  vel_msg.header = Header()
  vel_msg.header.stamp = rospy.Time.now()
  vel_msg.header.frame_id = "base_link"
  vel_msg.twist.linear.x = linear_x*VELOCITY
  vel_msg.twist.linear.y = linear_y*VELOCITY
  
  if angular_1 < 0 and angular_2 > 0:
    vel_msg.twist.angular.z = ANGULAR
  elif angular_1 > 0 and angular_2 < 0:
    vel_msg.twist.angular.z = -ANGULAR
  
  pitch_msg = Float64()
  yaw_msg = Float64()
  pitch_msg.data = pitch_*PITCH_MAX_ANGLE
  yaw_msg.data = yaw_*YAW_MAX_ANGLE

  twist = vel_msg
  pitch = pitch_msg
  yaw = yaw_msg


def publish_cmd(timer):
  vel_pub.publish(twist)
  pitch_pub.publish(pitch)
  yaw_pub.publish(yaw)
  print('-'*50)
  print("linear:\n\tx:{} y:{}\nangular:\n\tz:{}\ngimbal:\n\tyaw:{} pitch:{}".format(twist.twist.linear.x, twist.twist.linear.y,twist.twist.angular.z,yaw.data,pitch.data))
  #print('-'*50)

if __name__ == "__main__":
  rospy.init_node("joy_teleop")
  global vel_pub
  global pitch_pub
  global yaw_pub
  global twist
  global yaw
  global pitch
  twist = TwistStamped()
  yaw = Float64()
  pitch = Float64()
  timer = rospy.Timer(rospy.Duration(1/30.0),publish_cmd)

  vel_pub = rospy.Publisher("/cmd_vel",TwistStamped,queue_size=1)
  pitch_pub = rospy.Publisher("/pitch",Float64,queue_size=1)
  yaw_pub = rospy.Publisher("/yaw",Float64,queue_size=1)
  joy_sub = rospy.Subscriber("/joy",Joy,callback=joy_callback,queue_size=1)

  if not rospy.is_shutdown():
    timer.run()
    rospy.spin()
  
