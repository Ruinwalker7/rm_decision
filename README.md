# RM_Decision
哨兵自动决策代码

## 安装
安装BehaviorTree.cpp决策树依赖：

```
 sudo apt-get install libzmq3-dev libboost-coroutine-dev libncurses5-dev libncursesw5-dev
```
安装决策树：
```
git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git
cd BehaviorTree.cpp-master 
mkdir build; cd build
cmake ..
make
sudo make install
```
运行仿真
```
source devel/setup.bash
roslaunch robot_gazebo navigation.launch
roslaunch rf2o_laser_odometry launch/rf2o_laser_odometry.launch
roslaunch serial serial_node
rosrun rqt rqt_
rosrun rm_decision rm_decision
```


## TODO LIST
- [x] ~~巡逻~~
- [ ] 是否被击打
- [ ] 小陀螺
- [ ] 回血达标
- [ ] 三维旋转和四元数 
