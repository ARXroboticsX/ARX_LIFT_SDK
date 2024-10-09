#include <ros/ros.h>
#include <cmath>
#include <iostream>
#include <std_msgs/Float32MultiArray.h>
#include "utility.h"
#include "Hardware/can.h"
#include "Hardware/motor.h"
#include "Hardware/teleop.h"
#include "App/arm_control.h"
#include "App/keyboard.h"
#include "App/play.h"
#include "App/solve.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <atomic>
#include <signal.h>
#include "arm_control/PosCmd.h"
#include "arm_control/JointControl.h"
#include "arm_control/JointInformation.h"
#include "arm_control/ChassisCtrl.h"

int CONTROL_MODE=0;
command cmd;

bool app_stopped = false;
void sigint_handler(int sig);
void safe_stop(can CAN_Handlej);
int lift_mode =2;  

int main(int argc, char **argv)
{
    ros::init(argc, argv, "body_node"); 
    ros::NodeHandle node;
    Teleop_Use()->teleop_init(node);

    arx_arm ARX_ARM((int) CONTROL_MODE);


    ros::Subscriber sub_head = node.subscribe<arm_control::PosCmd>("/ARX_VR_L", 10, 
                                  [&ARX_ARM](const arm_control::PosCmd::ConstPtr& msg) {
                                        ARX_ARM.set_high_t1 = ARX_ARM.set_high;
                                        ARX_ARM.set_high  = msg-> height;
                                        ARX_ARM.arx5_cmd.head_pit  = msg-> head_pit;
                                        ARX_ARM.arx5_cmd.head_yaw  = 0;
                                        ARX_ARM.ros_chx    = msg->chx/2;
                                        ARX_ARM.ros_chy    = msg->chy/2;
                                        ARX_ARM.ros_chz    = -msg->chz;
                                        ARX_ARM.chassis_mode1 = msg->mode1;
                                        ARX_ARM.chassis_mode2 = msg->mode2;

                                  });

    ros::Subscriber sub_chassis = node.subscribe<arm_control::ChassisCtrl>("/chassis_ctrl", 10, 
                                  [&ARX_ARM](const arm_control::ChassisCtrl::ConstPtr& msg) {
                                    
                                      ARX_ARM.arx_ros_vel[0] =msg->chassis_vel[0];
                                      ARX_ARM.arx_ros_vel[1] =msg->chassis_vel[1];
                                      ARX_ARM.arx_ros_vel[2] =msg->chassis_vel[2];
                                      ARX_ARM.chassis_mode1 = msg->mode1;//1 运行 2保护 3ros_vel
                                    
                                  });


    ros::Publisher pub_chassis = node.advertise<arm_control::ChassisCtrl>("/chassis_back", 10);


    arx5_keyboard ARX_KEYBOARD;

    ros::Rate loop_rate(200);
    can CAN_Handlej;

    std::thread keyThread(&arx5_keyboard::detectKeyPress, &ARX_KEYBOARD);

    CAN_Handlej.Set_Zero(0x01);
    sleep(3);

    while(ros::ok())
    { 
        char key = ARX_KEYBOARD.keyPress.load();
        ARX_ARM.getKey(key);

        ARX_ARM.get_joint();
        ARX_ARM.update_real(lift_mode);
        ARX_ARM.update_head();
        ARX_ARM.update_chassis();

        arm_control::ChassisCtrl msg_chassis;        

            msg_chassis.chassis_vel[0] = chassis_motor[0].spd;  
            msg_chassis.chassis_vel[1] = chassis_motor[1].spd;  
            msg_chassis.chassis_vel[2] = chassis_motor[2].spd;  
            msg_chassis.high = ARX_ARM.lift_total_pos;
            msg_chassis.chx = ARX_ARM.ros_chx;
            msg_chassis.chy = ARX_ARM.ros_chy;
            msg_chassis.chz = ARX_ARM.ros_chz;
            msg_chassis.mode1 = ARX_ARM.chassis_mode1;

        pub_chassis.publish(msg_chassis);   

        ros::spinOnce();
        loop_rate.sleep();
        
        CAN_Handlej.arx_1();
    }
    CAN_Handlej.arx_2();
    return 0;
}

