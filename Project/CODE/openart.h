#ifndef __OPENART_H__
#define __OPENART_H__

#include "headfile.h"


extern rt_sem_t point_sem;


extern uint8 ART1_POINT_X[40],ART1_POINT_Y[40];
extern uint8 point_num; // 数据个数
// extern char discern_kind[2];
// extern int ART1_flag;
// extern int Max_points;
// extern float CSI_angle_radian;
extern uint8 ART1_dat[82];
// extern char ART1_Dat[4];
// extern char Large_categories;
// extern int Car_run_time_ms;
// extern int Car_run_time_s;
// extern int tar_turn;//调转向环用
// void WIRE_uart_Init(void);
void ART1_UART_Init(void);
// void Wireless_serial_port_data_processing(void);
// char* itoa(int num,char* str,int radix);
// void buzzer_warning(void);


#endif