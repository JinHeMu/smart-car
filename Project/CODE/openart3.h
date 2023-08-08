#ifndef __OPENART3_H__
#define __OPENART3_H__

#include "headfile.h"


extern uint8	ART3_DETECT_DISTANCE;//��¼��Ƭ����
extern uint8	ART3_DETECT_Flag;//��¼�Ƿ��ֿ�Ƭ
extern uint8    ART3_mode;//模式1为目标检测模式


void ART3_UART_Init(void);

#endif