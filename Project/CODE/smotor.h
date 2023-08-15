#ifndef _smotor_h
#define _smotor_h

#include "headfile.h"


void arm_init(void);

void ARM_UP_angle(int angle);
void ARM_LOW_angle(int angle);
void ARM_MID_angle(int angle);
void ARM_LEFT_angle(int angle);

void arm_carry(void);
void arm_putbox(uint8 angle);
void arm_down(void);
void arm_closebox(void);
void arm_openbox(uint8 angle, uint8 location);
void arm_return(void);
void arm_set_mid(uint8 angle);


void magnet_front_release(void);
void magnet_front_appeal(void);
void magnet_left_release(void);
void magnet_left_appeal(void);


//void smotor2_control(int16 duty);
//void smotor3_control(int16 duty);
//    

#endif
