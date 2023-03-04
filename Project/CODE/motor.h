#ifndef _motor_h
#define _motor_h

#include "headfile.h"





extern double speed_tar;
extern int32 duty1,duty2,duty3,duty4;//µç»úPWMÖµ
extern float kp, ki, kd;
extern float Angel_KP ;
extern float Angel_KI ;
extern float Angel_KD ;
void motor_init(void);
//void motor_pid(int16 expect_speed);
void motor_control(bool run);
void car_omni(float x, float y, float z);
void car_stop();
int angel_pid(int NowAngel,int TargetAngel);
//int picture_xerror_pid(int16 now_x,int16 target_x);
//int picture_yerror_pid(int16 now_y,int16 target_y);
void car_ahead();
void car_back();
void car_lsideWay();
void car_rsideWay();
void car_diagonal();
void car_turnround();
void car_anticlockwise();
void car_concerning();

#endif