#ifndef __OPENART3_H__
#define __OPENART3_H__

#include "headfile.h"


extern uint8	ART3_DETECT_DISTANCE;//记录卡片距离
extern uint8	ART3_DETECT_Flag;//记录是否发现卡片


void ART3_UART_Init(void);

#endif