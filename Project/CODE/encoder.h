#ifndef _encoder_
#define _encoder_

#include "headfile.h"

extern int16 encoder1;
extern int16 encoder2;
extern int16 encoder3;
extern int16 encoder4;
extern float MileageKx,MileageKy;
extern int16 RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4;//滤波之后encoder的值

void encoder_init(void);
void encoder_get(void);


#endif



