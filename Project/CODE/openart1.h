#ifndef __OPENART1_H__
#define __OPENART1_H__

#include "headfile.h"


extern rt_sem_t uart_corrdinate_sem;

extern char classified[10];

extern uint8 ART1_POINT_X[40],ART1_POINT_Y[40];
extern uint8 coordinate_num;
extern uint8 ART1_mode;
extern uint8 point_num; // 数据个数
extern uint8 ART1_CLASS_Flag;

extern int8 ART1_CORRECT_Boundary_Angle;
extern uint8 ART1_dat[82];
extern uint8 ART1_CORRECT_Boundary_Flag;

void ART1_UART_Init(void);



#endif