#ifndef _chassis_motor_H_
#define _chassis_motor_H_

#include <stdint.h>
#include "math_ops.h"
#include <iostream>


typedef struct
{
    float pos=0;
    float spd=0;
    float cur=0;

} Chassis_motor_t;

extern Chassis_motor_t chassis_motor[8];

void send_chassis_cmd(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint8_t* Data);


#endif