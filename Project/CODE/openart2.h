#ifndef __OPENART2_H__
#define __OPENART2_H__

#include "headfile.h"

extern int8 ART2_center_x;
extern int8 ART2_center_y;
extern int8 ART2_angle;
extern uint8 ART2_mode;


extern int8 ART2_CORRECT_X;
extern int8 ART2_CORRECT_Y;
extern int8 ART2_CORRECT_Flag;

void ART2_UART_Init(void);

#endif