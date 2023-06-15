#ifndef _smotor_h
#define _smotor_h

#include "headfile.h"


void arm_init(void);
void arm_carry(void);

void arm_box(void);
void ARM_UP_angle(int angle);
void ARM_LOW_angle(int angle);
void arm_up(void);
void arm_down(void);
void arm_pick(void);

void magnet1_appeal(void);
void magnet1_release(void);
void magnet2_appeal(void);
void magnet2_release(void);


//void smotor2_control(int16 duty);
//void smotor3_control(int16 duty);
//    

#endif
