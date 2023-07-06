#ifndef __ICM20602_H__
#define __ICM20602_H__

#include "headfile.h"

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} arhs_source_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_zero_param_t;

extern arhs_source_param_t source_data;
extern float angle_x,angle_y,angle_z;



void icm_zeroBias(void);
void ARHS_getValues();
void Mahony_computeAngles();


#endif