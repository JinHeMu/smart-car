#ifndef _motor_h
#define _motor_h

#include "headfile.h"





extern double speed_tar;
extern int32 duty1,duty2,duty3,duty4;//电机PWM值


extern float Incremental_kp[4], Incremental_ki[4], Incremental_kd[4];//增量式PID，控制电机输出值
extern float Angel_kp, Angel_ki, Angel_kd;//角度环
extern float Position_kp, Position_ki, Position_kd;

void motor_init(void);
void motor_control(bool run);
void car_omni(float x, float y, float z);
void car_omni_angle(float x, float y, float z);


int angel_pid(int NowAngel,int TargetAngel);
int picture_x_pid(int16 now_x,int16 target_x);
int picture_y_pid(int16 now_y,int16 target_y);
int correct_x_pid(int16 now_x,int16 target_y);
int correct_y_pid(int16 now_y,int16 target_y);


void car_ahead();
void car_back();
void car_lsideWay();
void car_rsideWay();
void car_diagonal();
void car_turnround();
void car_anticlockwise();
void car_concerning();
void car_stop();

#endif