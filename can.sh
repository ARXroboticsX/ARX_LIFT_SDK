#!/bin/bash
source ~/.bashrc

gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S slcand -o -f -s8 /dev/arxcan1 can1;sudo ifconfig can1 up;exec bash;"
sleep 1

gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S slcand -o -f -s8 /dev/arxcan3 can3;sudo ifconfig can3 up;exec bash;"
sleep 1
gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S slcand -o -f -s8 /dev/arxcan5 can5;sudo ifconfig can5 up;exec bash;"








