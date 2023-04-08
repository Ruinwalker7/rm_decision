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
- [x] 是否被击打(完成了一半，目前未解决第一次被击打的判断)
- [ ] 小陀螺（创建了类）
- [x] 回血达标
- [x] 根据状态是否移动
- [ ] 三维旋转和四元数 
