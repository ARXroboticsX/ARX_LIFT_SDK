#!/bin/bash
source ~/.bashrc

workspace=$(pwd)

sleep 1



gnome-terminal -t "launcher" -x bash -c "source ~/.bashrc;\
cd ${workspace};\
source devel/setup.bash;roslaunch arm_control arx5.launch;exec bash;"

gnome-terminal -t "pub" -x bash -c "source ~/.bashrc;\
cd ${workspace};\
rosrun joy joy_node;exec bash;"

gnome-terminal -t "pub" -x bash -c "source ~/.bashrc;\
cd ${workspace};\
source devel/setup.bash;exec bash;"




