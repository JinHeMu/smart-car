#ifndef _smotor_h
#define _smotor_h

#include "headfile.h"


void smotor_init(void);
void smotor1_control(int16 duty);
void magnet_init(void);
void magnet_release(void);
void magnet_appeal(void);

//void smotor2_control(int16 duty);
//void smotor3_control(int16 duty);
//    

#endif
